#include "Simulator.h"
#include "Main.h"
#include "Motion.h"
#include "sim.h"
#include "Shared/string_util.h"
#include "Shared/RobotInfo.h"
#include "SimConfig.h"
#include "Shared/debuget.h"
#include "Shared/MarkScope.h"
#include "IPC/MessageReceiver.h"
#include "IPC/RegionRegistry.h"
#include "local/DataSources/FileSystemDataSource.h"
#include "local/DataSources/FileSystemImageSource.h"
#include "local/CommPort.h"
#include "local/DeviceDriver.h"

#include <iostream>
#include <libxml/tree.h>

#ifndef DISABLE_READLINE
#  include <readline/readline.h>
#  include <readline/history.h>
#endif

using namespace std;

const float Simulator::avgSpeedupGamma=.99;

Simulator* Simulator::theSim=NULL;
std::set<MotionHook*> Simulator::motionHooks;

Simulator::Simulator()
: Process(getID(),getClassName()), plist::PrimitiveListener(), plist::CollectionListener(), MessageQueueStatusThread::StatusListener(), frameCounter(), cmdThread(),
cameraQueue(ipc_setup->registerRegion(Simulator::getCameraQueueID(),sizeof(sim::CameraQueue_t))),
sensorQueue(ipc_setup->registerRegion(Simulator::getSensorQueueID(),sizeof(sim::SensorQueue_t))),
timerWakeup(ipc_setup->registerRegion(Simulator::getTimerWakeupID(),sizeof(sim::TimerWakeup_t))),
motionWakeup(ipc_setup->registerRegion(Simulator::getMotionWakeupID(),sizeof(sim::MotionWakeup_t))),
statusRequest(ipc_setup->registerRegion(Simulator::getStatusRequestID(),sizeof(sim::StatusRequest_t))),
events(ipc_setup->registerRegion(Main::getEventsID(),sizeof(sim::EventQueue_t))),
motionout(ipc_setup->registerRegion(Motion::getMotionOutputID(),sizeof(sim::MotionOutput_t))),
commandQueue(ipc_setup->registerRegion(Simulator::getCommandQueueID(),sizeof(CommandQueue_t))),
cameraStatus(*cameraQueue), sensorStatus(*sensorQueue), timerStatus(*timerWakeup), motionStatus(*motionWakeup), eventsStatus(), commandrecv(NULL), motionrecv(NULL),
vision(&DeviceDriver::getImageSources,30, *cameraQueue),
sensors(&DeviceDriver::getSensorSources,1000.f/NumFrames/FrameTime, *sensorQueue),
frameTimes(), runSpeed(1), lastTimeScale(0), step(STEP_NONE), waitingSteps(0), curLevel(SharedGlobals::CONSTRUCTING),
fullspeedWallStart(), fullspeedSimStart(), lastFrameWallStart(), avgWallTime(), avgSimTime(),
simLock()
{
	theSim=this;
	new (&(*cameraQueue)) sim::CameraQueue_t;
	new (&(*sensorQueue)) sim::SensorQueue_t;
	new (&(*timerWakeup)) sim::TimerWakeup_t;
	new (&(*motionWakeup)) sim::MotionWakeup_t;
	new (&(*statusRequest)) sim::StatusRequest_t;
	new (&(*commandQueue)) CommandQueue_t;
	statusRequest->setOverflowPolicy(MessageQueueBase::WAIT);
	commandQueue->setOverflowPolicy(MessageQueueBase::WAIT);
	
	DeviceDriver::getRegistry().addCollectionListener(this);
	
	/* Since we do a two stage loading (some stuff in sim.cc launcher, then some command line
	 *  stuff, then the rest of it now that we're loaded), this is a little tricker than it normally would be. */
	// don't remove entries from parse tree, we're going to be adding more and want their values...
	sim::config.setSavePolicy(plist::Collection::UNION);
	// write out the current values for entries we do have, which may have been modified since originally loaded
	sim::config.writeParseTree();
	// now add the rest of our entries (could just get away with addEntry()'s, but this function is a little more robust)
	replaceEntry("CommPorts",CommPort::getRegistry(),"Communication portals for use by device drivers");
	replaceEntry("Drivers",DeviceDriver::getRegistry(),"Settings for device drivers");
	replaceEntry("Sensors",sensors,"Settings for the loading of sensor values");
	replaceEntry("Vision",vision,"Settings for the loading of camera frames");
	// now we're done adding entries, so if there's anything extra in the file, make note of it
	sim::config.setUnusedWarning(true);
	// reload from parse tree to get values for these new entries
	sim::config.readParseTree();
	// any future saves should be strict and remove those unused values (...maybe?)
	sim::config.setSavePolicy(plist::Collection::SYNC);
	
	motionWakeup->setOverflowPolicy(MessageQueueBase::DROP_OLDEST);
	timerWakeup->setOverflowPolicy(MessageQueueBase::DROP_OLDEST);
	cameraQueue->setOverflowPolicy(MessageQueueBase::DROP_OLDEST);
	sensorQueue->setOverflowPolicy(MessageQueueBase::DROP_OLDEST);
	sensorQueue->addMessageFilter(frameCounter);
	
	//handle arguments passed from command line
	vector<vector<string> > delayed; // will delay setting data queue sources until after other arguments
	for(unsigned int i=0; i<sim::cmdlineArgs.size(); i++) {
		if(sim::cmdlineArgs[i].find(".Source=")==string::npos) {
			if(!processCommand(sim::cmdlineArgs[i],false))
				cerr << "Occurred while processing " << sim::cmdlineArgs[i] << endl;
		} else {
			vector<string> setarg;
			setarg.push_back("set");
			setarg.push_back(sim::cmdlineArgs[i]);
			delayed.push_back(setarg); // delay setting of source until after options like Frozen or Speed have been set
		}
	}
	for(unsigned int i=0; i<delayed.size(); i++) {
		//this avoids dropping initial frame(s)
		//if we had set the source before freezing or pausing, might've dropped something between them
		if(!cmdSet(delayed[i]))
			cerr << "Occurred while processing " << delayed[i][1] << endl;
	}
	
	// if we were supposed to start up paused, reset clock now that we've set the speed
	if(globals->timeScale<=0)
		globals->resetBootTime();

	globals->timeScale.addPrimitiveListener(this);
	globals->motion.verbose.addPrimitiveListener(this);
	
	cmdThread.start();
	
	if(sim::config.tgtRunlevel>SharedGlobals::RUNNING)
		globals->signalShutdown();
	processRunlevel(SharedGlobals::CONSTRUCTING);
}

Simulator::~Simulator() {
	DeviceDriver::getRegistry().clear();
	CommPort::getRegistry().clear();
	DeviceDriver::getRegistry().removeCollectionListener(this);
	globals->timeScale.removePrimitiveListener(this);
	globals->motion.verbose.removePrimitiveListener(this);
	processRunlevel(SharedGlobals::DESTRUCTING);
	//::close(STDIN_FILENO); // seems to prevent proper terminal reset on exit...
}

void Simulator::DoStart() {
	Process::DoStart();
	eventsStatus.setMessageQueue(*events);
	commandrecv = new MessageReceiver(*commandQueue, gotCommand);
	class : public Thread {
	public:
		virtual void* run() { Simulator::setMotionStarting(); return NULL; }
	} doStartThread;
	cmdThread.runInitThread(doStartThread);
	processRunlevel(SharedGlobals::STARTING);
}

void Simulator::run() {
	for(unsigned int i=0; i<ProcessID::NumProcesses; ++i)
		cout << globals->processNames[i] << " pid=" << globals->pids[i] << ";  ";
	cout << endl;
	if(globals->timeScale!=0)
		runSpeed=globals->timeScale;

	motionrecv = new MessageReceiver(*motionout, gotMotion);
	
	DataSource::setNeedsSensor(globals->waitForSensors);
	resetSpeedMode();
	if(globals->timeScale<0)
		incrementTime();
	
	if(globals->waitForSensors) {
		if(sensors.source.size()==0) {
			cout << "Ignoring WaitForSensors flag because Sensors has no source" << endl;
		} else {
			if(DeviceDriver::getRegistry().getInstance(sensors.source)==NULL) {
				cout << "Ignoring WaitForSensors flag because Sensors source is invalid" << endl;
			} else {				
				class : public Thread {
				public:
					virtual void* run() { globals->waitSensors(); return NULL; }
				} waitSensorsThread;
				cmdThread.runInitThread(waitSensorsThread);
			}
		 }
	}
	DataSource::setNeedsSensor(false);

	if(sim::config.tgtRunlevel==SharedGlobals::RUNNING)
		Process::run();

	sensors.source="";
	vision.source="";
	if(sensors.isStarted())
		sensors.stop();
	if(vision.isStarted())
		vision.stop();
	
	if(globals->timeScale<0) {
		motionStatus.removeStatusListener(this);
		timerStatus.removeStatusListener(this);
		sensorStatus.removeStatusListener(this);
		cameraStatus.removeStatusListener(this);
	}
	globals->signalShutdown();
}

void Simulator::plistValueChanged(const plist::PrimitiveBase& pl) {
	MarkScope l(simLock);
	if(&pl==&globals->timeScale) {
		get_time(); // force SharedGlobals to notice the change and update its state
		resetSpeedMode();
		if(globals->timeScale<0)
			incrementTime();
		timerWakeup->sendMessage(NULL);
		motionWakeup->sendMessage(NULL);
	} else if(&pl==&globals->motion.verbose) {
		for(std::set<MotionHook*>::iterator it=motionHooks.begin(); it!=motionHooks.end(); ++it)
			(*it)->setMotionHookVerbose(globals->motion.verbose);
	} else {
		cerr << "WARNING: Simulator got a plistValueChanged for an unknown plist primitive";
	}
}
void Simulator::plistCollectionEntryAdded(plist::Collection& col, plist::ObjectBase& primitive) {
	MarkScope l(simLock);
	if(&col==&DeviceDriver::getRegistry()) {
		if(DeviceDriver * d=dynamic_cast<DeviceDriver*>(&primitive)) {
			if(MotionHook * mh = d->getMotionSink()) {
				mh->setMotionHookVerbose(globals->motion.verbose);
				motionHooks.insert(mh);
				if(curLevel==SharedGlobals::RUNNING) {
					mh->motionStarting();
					if(globals->timeScale>0)
						mh->enteringRealtime();
				}
			}
		} else {
			cerr << "WARNING: Simulator got a plistCollectionEntryAdded for an unknown primitive type";
		}
	} else {
		cerr << "WARNING: Simulator got a plistCollectionEntryAdded for an unknown plist collection";
	}
}
void Simulator::plistCollectionEntryRemoved(plist::Collection& col, plist::ObjectBase& primitive) {
	MarkScope l(simLock);
	if(&col==&DeviceDriver::getRegistry()) {
		if(DeviceDriver * d=dynamic_cast<DeviceDriver*>(&primitive)) {
			motionHooks.erase(d->getMotionSink());
		} else {
			cerr << "WARNING: Simulator got a plistCollectionEntryRemoved for an unknown primitive type";
		}
	} else {
		cerr << "WARNING: Simulator got a plistCollectionEntryRemoved for an unknown plist collection";
	}
}
void Simulator::plistCollectionEntriesChanged(plist::Collection& col) {
	MarkScope l(simLock);
	if(&col==&DeviceDriver::getRegistry()) {
		std::set<MotionHook*> oldmh;
		motionHooks.swap(oldmh);
		for(DeviceDriver::registry_t::const_iterator it=DeviceDriver::getRegistry().begin(); it!=DeviceDriver::getRegistry().end(); ++it) {
			if(DeviceDriver * d=DeviceDriver::getRegistry().getInstance(it->first)) {
				if(MotionHook * mh = d->getMotionSink()) {
					mh->setMotionHookVerbose(globals->motion.verbose);
					motionHooks.insert(mh);
					if(oldmh.find(mh)==oldmh.end() && curLevel==SharedGlobals::RUNNING) {
						mh->motionStarting();
						if(globals->timeScale>0)
							mh->enteringRealtime();
					}
				}
			} else {
				cerr << "WARNING: In Simulator::plistCollectionEntriesChanged, driver " << it->first << " does not correspond to a known instance" << endl;
			}
		}
	} else {
		cerr << "WARNING: Simulator got a plistCollectionEntriesChanged for an unknown plist collection";
	}
}

void Simulator::messagesRead(MessageQueueBase& mq, unsigned int /*n*/) {
	MarkScope l(simLock);
	if(globals->timeScale<0) {
		//clear corresponding bit in waitingSteps
		 if(&mq==&(*cameraQueue)) {
			//cout << "Camera read, ";
			waitingSteps&=~(1<<STEP_CAMERA);
		} else if(&mq==&(*sensorQueue)) {
			//cout << "Sensor read, ";
			waitingSteps&=~(1<<STEP_SENSOR);
		} else if(&mq==&(*timerWakeup)) {
			//cout << "Timer read, ";
			waitingSteps&=~(1<<STEP_TIMER);
		} else if(&mq==&(*motionWakeup)) {
			//cout << "Motion read, ";
			waitingSteps&=~(1<<STEP_MOTION);
		} else if(&mq==&(*events)) {
			//cout << "Main read event queue (" << events->getMessagesUnread() << "remain), ";
			// nothing to do, just waiting for main to catch up before incrementing
		} else {
			cout << "Unknown message base read (either you meant to add some code to Simulator::messagesRead, or why did you bother to register a listener?)" << endl;
		}
		//cout << " waiting " << waitingSteps << " events " << events->getMessagesUnread() << endl;
		
		if(waitingSteps==0 && events->getMessagesUnread()==0) //if that was the last one we were waiting for -- go for the next!
			incrementTime();
	}
}

void Simulator::sendCommand(const std::string& cmd) {
	static unsigned int cmdSN=0;
	char msgname[30];
	snprintf(msgname,30,"SimCommand.%d.%d",ProcessID::getID(),cmdSN++);
	RCRegion * msg = new RCRegion(msgname,cmd.size());
	strcpy(msg->Base(),cmd.c_str());
	SharedObject<CommandQueue_t> commandQ(ipc_setup->registerRegion(Simulator::getCommandQueueID(),sizeof(CommandQueue_t)));
	commandQ->sendMessage(msg,true);
}

void Simulator::setMotionStarting() {
	// don't need lock, this is only called during single-threaded operation (DoStart())
	//MarkScope l(theSim ? dynamic_cast<Resource&>(theSim->simLock) : ::emptyResource);
	for(std::set<MotionHook*>::iterator it=theSim->motionHooks.begin(); it!=theSim->motionHooks.end(); ++it)
		(*it)->motionStarting();
}
void Simulator::setMotionStopping() {
	// don't need lock, this is only called during single-threaded operation (DoStop())
	//MarkScope l(theSim ? dynamic_cast<Resource&>(theSim->simLock) : ::emptyResource);
	for(std::set<MotionHook*>::iterator it=theSim->motionHooks.begin(); it!=theSim->motionHooks.end(); ++it)
		(*it)->motionStopping();
}
void Simulator::updateMotion(const float outputs[][NumOutputs]) {
	MarkScope l(theSim ? dynamic_cast<Resource&>(theSim->simLock) : ::emptyResource);
	for(std::set<MotionHook*>::iterator it=theSim->motionHooks.begin(); it!=theSim->motionHooks.end(); ++it)
		(*it)->motionCheck(outputs);
}
void Simulator::setMotionLeavingRealtime(bool isFullSpeed) {
	MarkScope l(theSim ? dynamic_cast<Resource&>(theSim->simLock) : ::emptyResource);
	for(std::set<MotionHook*>::iterator it=theSim->motionHooks.begin(); it!=theSim->motionHooks.end(); ++it)
		(*it)->leavingRealtime(isFullSpeed);
}
void Simulator::setMotionEnteringRealtime() {
	MarkScope l(theSim ? dynamic_cast<Resource&>(theSim->simLock) : ::emptyResource);
	for(std::set<MotionHook*>::iterator it=theSim->motionHooks.begin(); it!=theSim->motionHooks.end(); ++it)
		(*it)->enteringRealtime();
}


void Simulator::DoStop() {
	if(sensors.isStarted() || vision.isStarted())
		sleep(1);
	if(sensors.isStarted() || vision.isStarted())
		sleep(1);
	if(sensors.isStarted())
		sensors.kill();
	if(vision.isStarted())
		vision.kill();
	commandrecv->finish();
	delete commandrecv;
	commandrecv=NULL;
	motionrecv->finish();
	delete motionrecv;
	motionrecv=NULL;
	setMotionStopping();
	processRunlevel(SharedGlobals::STOPPING);
	Process::DoStop();
}


void* Simulator::CommandThread::run() {
	Simulator * simp=dynamic_cast<Simulator*>(Process::getCurrent());
	ASSERTRETVAL(simp!=NULL,"CommandThread not in Simulator!",NULL);
	string prompt; // initially prompt is empty because it will be buried in startup text anyway
	// we'll display another prompt at the end of launch (or if user hits enter)
	while(true) {
		testCancel();
		string line;
#ifndef DISABLE_READLINE
		char* readin=readline(prompt.c_str());
		if(readin==NULL) {
			cout << endl;
			simp->cmdQuit(vector<string>());
			break;
		}
		line=readin;
		free(readin);
#else
		cout << prompt << flush;
		getline(cin,line);
		if(!cin) {
			cout << endl;
			simp->cmdQuit(vector<string>());
			break;
		}
#endif
		simp->processCommand(line,true);
		prompt = sim::config.cmdPrompt;
	}
	return NULL;
}

void Simulator::CommandThread::cancelled() {
#ifndef DISABLE_READLINE
	rl_reset_terminal(NULL);
#endif
}

void Simulator::CommandThread::runInitThread(Thread& th) {
	initThread=&th;
	th.start();
	th.join();
	initThread=NULL;
}

void Simulator::resetSpeedMode() {
	if(globals->timeScale<=0) {
		if(vision.isStarted()) {
			vision.stop();
			//vision.join(); // can't join because runfor/runto pause might be triggered within LoadFileThread's get_time() call
		}
		if(sensors.isStarted()) {
			sensors.stop();
			//sensors.join(); // can't join because runfor/runto pause might be triggered within LoadFileThread's get_time() call
		}
	} else {
		if(sim::config.tgtRunlevel==SharedGlobals::RUNNING) { 
			vision.start();
			sensors.start();
		}
	}
	if(lastTimeScale>0 && globals->timeScale<=0) {
		setMotionLeavingRealtime(globals->timeScale<0);
	} else if(lastTimeScale<=0 && globals->timeScale>0) {
		setMotionEnteringRealtime();
	}
	if(globals->timeScale<0) {
		cameraQueue->setOverflowPolicy(MessageQueueBase::WAIT);
		sensorQueue->setOverflowPolicy(MessageQueueBase::WAIT);
		// these status listener calls check to prevent duplicate listeners
		cameraStatus.addStatusListener(this);
		sensorStatus.addStatusListener(this);
		timerStatus.addStatusListener(this);
		motionStatus.addStatusListener(this);
		eventsStatus.addStatusListener(this);
		fullspeedWallStart.Set();
		fullspeedSimStart=globals->simulatorTime;
		lastFrameWallStart.Set();
		avgWallTime=avgSimTime=0;
	} else {
		cameraQueue->setOverflowPolicy(MessageQueueBase::DROP_OLDEST);
		sensorQueue->setOverflowPolicy(MessageQueueBase::DROP_OLDEST);
		eventsStatus.removeStatusListener(this);
		motionStatus.removeStatusListener(this);
		timerStatus.removeStatusListener(this);
		sensorStatus.removeStatusListener(this);
		cameraStatus.removeStatusListener(this);
	}
	if(globals->timeScale==0)
		globals->setAutoPauseTime(-1U);
	lastTimeScale=globals->timeScale;
}

void Simulator::replaceEntry(const std::string& name, plist::Dictionary& d, const std::string& comment) {
	plist::Dictionary::const_iterator it = sim::config.findEntry(name);
	if(it==sim::config.end()) {
		sim::config.addEntry(name,d,comment);
	} else {
		d.set(*it->second);
		sim::config.setEntry(name,d);
		sim::config.setComment(name,comment);
	}
}

void Simulator::incrementTime() {
	MarkScope l(simLock);
	waitingSteps=getNextFrame();
	if(waitingSteps==0)
		return;
	unsigned int next=*frameTimes.begin();
	if(next>globals->simulatorTime) {
		unsigned int adv=next-globals->simulatorTime;
		avgWallTime=avgWallTime*avgSpeedupGamma + lastFrameWallStart.Age().Value()*(1-avgSpeedupGamma);
		avgSimTime=avgSimTime*avgSpeedupGamma + adv*(1-avgSpeedupGamma);
		lastFrameWallStart.Set();
		//cout << "inc " << (avgSimTime/avgWallTime/1000) << " " << waitingSteps << endl;
		globals->simulatorTime=next;
	}
	if(waitingSteps & (1<<STEP_CAMERA))
		vision.advanceFrame(false);
	if(waitingSteps & (1<<STEP_SENSOR))
		sensors.advanceFrame(false);
	if(waitingSteps & (1<<STEP_TIMER))
		timerWakeup->sendMessage(NULL);
	if(waitingSteps & (1<<STEP_MOTION))
		motionWakeup->sendMessage(NULL);
	if(globals->getAutoPauseTime()<=globals->simulatorTime || (1<<step) & waitingSteps) {
		//that was the step we were waiting for, pause sim
		globals->timeScale=0;
		step=STEP_NONE;
		globals->setAutoPauseTime(-1U);
	}
}

unsigned int Simulator::getNextFrame() {
	frameTimes.clear();
	/*set<unsigned int>::iterator past=frameTimes.begin();
	while(past!=frameTimes.end() && *past<=globals->simulatorTime)
	past++;
	frameTimes.erase(frameTimes.begin(),past);*/
	unsigned int vis = vision.frozen && !vision.heartbeat ? -1U : vision.nextTimestamp();
	frameTimes.insert(vis);
	unsigned int sen = sensors.frozen && !sensors.heartbeat ? -1U : sensors.nextTimestamp();
	frameTimes.insert(sen);
	unsigned int tim=globals->getNextTimer();
	frameTimes.insert(tim);
	unsigned int mot=globals->getNextMotion();
	frameTimes.insert(mot);
	unsigned int next=*frameTimes.begin();
	//cout << "Testing: " << globals->simulatorTime << " => next camera: "<< vis << " next sensor: " << sen << " next timer: " << tim << " next motion: " << mot << " => " << next << endl;
	unsigned int steps=0;
	if(next!=-1U) {
		if(next==vis) {
			steps |= 1<<STEP_CAMERA;
		}
		if(next==sen) {
			steps |= 1<<STEP_SENSOR;
		}
		if(next==tim) {
			steps |= 1<<STEP_TIMER;
		}
		if(next==mot) {
			steps |= 1<<STEP_MOTION;
		}
	}
	return steps;
}

void Simulator::processRunlevel(SharedGlobals::runlevel_t curRunLevel) {
	curLevel=curRunLevel;
	if(sim::config.tgtRunlevel==curLevel && (!globals->isShutdown() || curLevel>SharedGlobals::RUNNING))
		cout << sim::config.cmdPrompt << flush;
	while(sim::config.tgtRunlevel==curLevel && (!globals->isShutdown() || curLevel>SharedGlobals::RUNNING)) {
		usleep(500000); // recheck status every half second
	}
}

bool Simulator::processCommand(const std::string& line, bool addToHistory) {
	vector<string> args;
	vector<unsigned int> offs;
	if(!string_util::parseArgs(line,args,offs)) {
		cerr << "Mismatched quotes" << endl;
		return false;
	}
	if(args.size()==0)
		return true;
#ifndef DISABLE_READLINE
	/*		if(current_history()==NULL)
		cout << "current_history()==NULL" << endl;
	else if(current_history()->line==NULL)
		cout << "line == NULL" << endl;
	else if(line!=current_history()->line)
		cout << "line is different" << endl;
	else {
		cout << "not added" << endl;
		cout << "new line: " << line << endl;
		cout << "old line: " << current_history()->line << endl;
	}
	if(history_get(-1)==NULL)
		cout << "history_get(0)==NULL" << endl;
	else if(history_get(-1)->line==NULL)
		cout << "line 0 == NULL" << endl;
	else {
		cout << "line 0: " << history_get(-1)->line << endl;
		if(line!=history_get(-1)->line)
			cout << "line 0 is different" << endl;
		else
			cout << "0 not added" << endl;
	}
	*/	
	if(addToHistory && (current_history()==NULL || current_history()->line==NULL || line!=current_history()->line))
		add_history(line.c_str());
#endif
	if(args[0]=="shutdown" || args[0]=="quit" || args[0]=="exit") {
		cmdQuit(args);
	} else if(args[0]=="load") {
		cmdLoad(args);
	} else if(args[0]=="save") {
		cmdSave(args);
	} else if(args[0]=="runlevel") {
		cmdRunlevel(args);
	} else if(args[0]=="get_time") {
		cout << "Current time is " << get_time() << endl;
	} else if(args[0]=="set") {
		cmdSet(args);
	} else if(args[0]=="runto") {
		cmdRun(args,false);
	} else if(args[0]=="runfor") {
		cmdRun(args,true);
	} else if(args[0]=="run" || args[0]=="r") {
		cmdRun(args);
	} else if(args[0]=="pause" || args[0]=="p") {
		cmdPause(args);
	} else if(args[0]=="help") {
		cmdHelp(args);
	} else if(args[0]=="step") {
		cmdStep(args);
	} else if(args[0]=="status") {
		cmdStatus(args);
	} else if(args[0]=="advance") {
		cmdAdvance(args);
	} else if(args[0]=="freeze") {
		cmdFreeze(true,args);
	} else if(args[0]=="unfreeze") {
		cmdFreeze(false,args);
	} else if(args[0]=="reset") {
		cmdReset(args);
	} else if(args[0]=="new") {
		cmdNew(args);
	} else if(args[0]=="delete") {
		cmdDelete(args);
	} else {
		unsigned int i;
		for(i=0; i<args.size(); ++i) {
			if(args[i].find("=")!=string::npos) {
				cmdSet(args);
				break;
			}
		}
		if(i==args.size()) {
			cout << "Unknown command '" << args[0] << "'" << endl;
			return false;
		}
	}
	return true;
}

bool Simulator::gotCommand(RCRegion* msg) {
	Simulator * simp=dynamic_cast<Simulator*>(Process::getCurrent());
	ASSERTRETVAL(simp!=NULL,"gotCommand, but not within Simulator process!",true);
	simp->processCommand(msg->Base(),false);
	return true;
}	

bool Simulator::gotMotion(RCRegion* msg) {
#ifdef DEBUG
	Simulator * simp=dynamic_cast<Simulator*>(Process::getCurrent());
	ASSERTRETVAL(simp!=NULL,"gotMotion, but not within Simulator process!",true);
#endif
	updateMotion(reinterpret_cast<float(*)[NumOutputs]>(msg->Base()));
	return true;
}

void Simulator::cmdQuit(const std::vector<std::string>& /*args*/) {
	sim::config.tgtRunlevel=SharedGlobals::DESTRUCTED;
	globals->signalShutdown();
	cmdThread.abortInitThread();
}
void Simulator::cmdLoad(const std::vector<std::string>& args) {
	if(args.size()>1)
		for(unsigned int i=1; i<args.size(); i++) {
			cout << "Loading from " << args[i] << "... " << flush;
			size_t res=sim::config.loadFile(args[i].c_str());
			cout << (res>0 ? "done." : "load failed.") << endl;
		}
	else {
		cout << "Loading from " << sim::config.getLastFile() << "... " << flush;
		size_t res=sim::config.loadFile(sim::config.getLastFile().c_str());
		cout << (res>0 ? "done." : "load failed.") << endl;
	}
}
void Simulator::cmdSave(const std::vector<std::string>& args) {
	if(args.size()>1)
		for(unsigned int i=1; i<args.size(); i++)
			sim::config.saveFile(args[i].c_str());
	else {
		cout << "Saving to " << sim::config.getLastFile() << "... " << flush;
		size_t res=sim::config.saveFile(sim::config.getLastFile().c_str());
		cout << (res>0 ? "done." : "save failed.") << endl;
	}
}
void Simulator::cmdRunlevel(const std::vector<std::string>& args) {
	if(args.size()<=1) {
		sim::config.tgtRunlevel=static_cast<SharedGlobals::runlevel_t>(sim::config.tgtRunlevel+1);
		cout << "Moving to next runlevel: " << SharedGlobals::runlevel_names[sim::config.tgtRunlevel] << endl;
	} else {
		try {
			sim::config.tgtRunlevel=string_util::makeUpper(args[1]);
		} catch(...) {
			cout << "Invalid runlevel specification.  Try one of:\n\t";
			for(unsigned int i=0; i<SharedGlobals::NUM_RUNLEVELS; i++)
				cout << i << ' ' << SharedGlobals::runlevel_names[i] << ", ";
			cout << "\nCurrently at " << SharedGlobals::runlevel_names[curLevel] << endl;
			return;
		}
		if(sim::config.tgtRunlevel<curLevel) {
			sim::config.tgtRunlevel=curLevel;
			cout << "Cannot reduce runlevel, currently at " << curLevel << ' ' << SharedGlobals::runlevel_names[curLevel] << "\n\t";
			for(unsigned int i=0; i<SharedGlobals::NUM_RUNLEVELS; i++)
				cout << i << ' ' << SharedGlobals::runlevel_names[i] << ", ";
			cout << endl;
			return;
		} else if(sim::config.tgtRunlevel==curLevel) {
			cout << "Already at " << curLevel << ' ' << SharedGlobals::runlevel_names[curLevel] << "\n\t";
			for(unsigned int i=0; i<SharedGlobals::NUM_RUNLEVELS; i++)
				cout << i << ' ' << SharedGlobals::runlevel_names[i] << ", ";
			cout << endl;
			return;
		}
	}
	if(sim::config.tgtRunlevel>SharedGlobals::RUNNING && curLevel<=SharedGlobals::RUNNING)
		globals->signalShutdown();
}
bool Simulator::cmdSet(const std::vector<std::string>& args) {
	if(args.size()==0 || args[0]=="set" && args.size()==1) {
		plist::filteredDisplay(cout,sim::config,"^[^.].*",REG_EXTENDED,3);
		return false;
	}
	string arg;
	for(unsigned int i=(args[0]=="set"?1:0); i<args.size(); i++) {
		arg+=args[i];
		if(i!=args.size()-1)
			arg+=" ";
	}
	if(arg.rfind("=")==string::npos) {
		plist::ObjectBase* ob=sim::config.resolveEntry(arg);
		if(ob==NULL) {
			cout << "'" << arg << "' is unknown" << endl;
			return false;
		}
		plist::filteredDisplay(cout,*ob,"^[^.].*",REG_EXTENDED,3);
	} else {
		string value=string_util::trim(arg.substr(arg.find("=")+1));
		string key=string_util::trim(arg.substr(0,arg.find("=")));
		plist::ObjectBase* ob=sim::config.resolveEntry(key);
		if(ob==NULL) {
			cout << "'" << key << "' is unknown" << endl;
			return false;
		}
		if(plist::PrimitiveBase* pbp=dynamic_cast<plist::PrimitiveBase*>(ob)) {
			try {
				pbp->set(value);
				return true;
			} catch(const XMLLoadSave::bad_format& bf) {
				cout << "'" << value << "' is a bad value for '" << key << "'" << endl;
				cout << bf.what() << endl;
			} catch(const std::exception& e) {
				cout << "An exception occured: " << e.what() << endl;
			}
		} else {
			cout << "Cannot assign to a dictionary" << endl;
			return false;
		}
	}
	return false; //exception occurred
}
void Simulator::cmdRun(const std::vector<std::string>& args, bool isRelative) {
	if(args.size()<=1) {
		cout << "runfor/runto requires an argument" << endl;
		return;
	}
	if(isRelative)
		globals->setAutoPauseTime(get_time()+atoi(args[1].c_str()));
	else
		globals->setAutoPauseTime(atoi(args[1].c_str()));
	if(globals->timeScale==0)
		globals->timeScale=runSpeed;
}
void Simulator::cmdRun(const std::vector<std::string>& args) {
	if(args.size()<=1) {
		if(globals->timeScale!=0) {
			cout << "Already running" << endl;
			return;
		}
		globals->timeScale=runSpeed;
	} else {
		float speed=atof(args[1].c_str());
		if(speed!=0)
			runSpeed=speed;
		globals->timeScale=speed;
	}
}
void Simulator::cmdPause(const std::vector<std::string>& args) {
	if(globals->timeScale==0) {
		if(find(args.begin(),args.end(),"quiet")==args.end())
			cout << "Already paused" << endl;
		return;
	}
	runSpeed=globals->timeScale;
	globals->timeScale=0;
}
void Simulator::cmdHelp(const std::vector<std::string>& args) {
	map<string,string> syntax;
	syntax["load"]="[file]";
	syntax["save"]="[file]";
	syntax["runlevel"]="[";
	for(unsigned int i=0; i<SharedGlobals::NUM_RUNLEVELS; i++) {
		stringstream ss;
		ss << i << "|" << SharedGlobals::runlevel_names[i];
		if(i!=SharedGlobals::NUM_RUNLEVELS-1)
			ss << " | ";
		syntax["runlevel"]+=ss.str();
	}
	syntax["runlevel"]+="]";
	syntax["get_time"]="";
	syntax["set"]="[var=value]";
	syntax["runto"]="time";
	syntax["runfor"]="time";
	syntax["run"]="[speed]";
	syntax["pause"]="";
	syntax["step"]="[camera|sensor|timer|motion]";
	syntax["status"]="[Main|Motion|SoundPlay|Simulator|all]*";
	syntax["advance"]=syntax["freeze"]=syntax["unfreeze"]="[camera|sensors|all]*";
	syntax["reset"]="[camera|sensors|all]";
	syntax["new"]="type [name]";
	syntax["delete"]="name";
	
	map<string,string> help;
	
	help["load"]="Load HAL configuration from file; if file unspecified, uses last specified file ('hal-$MODEL.plist' by default).\n"
		"Note that these files are human-readable XML (with comments!), and you can remove values to specify only a subset of settings.";
	
	help["save"]="Save HAL configuration to file; if file unspecified, uses last specified file ('hal-$MODEL.plist' by default).\n"
		"Note that these files are human-readable XML (with comments!), and you can remove values to specify only a subset of settings.";
	
	help["runlevel"]="You can specify a runlevel to move to, or if unspecified, the next one.\n"
		"You can only move forward runlevels, not backward.  Usually you'll only need RUNNING, "
		"unless you are debugging startup/shutdown code or the Tekkotsu itself.";
	
	help["get_time"]="Displays the simulator time.";
	
	help["set"]="Sets HAL configuration variables.  Without any arguments, displays all available variables and their current values.\n"
		"Type 'help set <variable>' to get more information about a particular variable.";
	
	help["runto"]="Triggers 'run' until the simulator time reaches the specified value and then pauses.";
	
	help["runfor"]="Triggers 'run' until the simulator time has moved by the specified number of milliseconds, then pauses.";
	
	help["run"]="Resets speed to last non-zero value (i.e. value prior to last 'pause'), can override by passing a new value as argument.  Can be abbreviated 'r'.";
	
	help["pause"]="Equivalent to 'set Speed=0'.  Can be abbreviated 'p'.  Stops the flow of time within the simulator.";
	
	help["step"]="Runs at \"full\" speed until the next indicated time frame, or the next available frame if no type is specified.\n"
		"See 'status' for available frames.";
	
	help["status"]="Displays a status report regarding current time, upcoming keyframes, and semaphore usage.  Specify one or more processes to get more in-depth, per-process status reports.";
	
	help["advance"]="Sends the next frame for the specified queue(s) in their listed order (can be listed more than once).\n"
		"Disregards timestamp information, and doesn't advance time, unlike 'step' command.  No arguments and \"all\" is the same as \"sensors camera\".";
	
	help["freeze"]="Equivalent to 'set queue.Frozen=true'.\n"
		"Stops sending frames from the specified queue(s), but still allows time to move (unlike 'pause').  No arguments is the same as \"all\".  See 'advance' and 'unfreeze'.";
	
	help["unfreeze"]="Equivalent to 'set queue.Frozen=false'.\n"
		"Begin sending frames from the specified queue(s) again.  Timestamps for the file listing are offset by the time spent frozen minus frames advanced so the queue(s) will continue from their current position.  No arguments is the same as \"all\".";
	
	help["reset"]="Moves the specified data queue(s) back to the first entry in their list.";
	
	help["new"]="Creates a new driver or communication port instance.\n  Driver types are:";
	set<string> driverNames;
	DeviceDriver::getRegistry().getTypeNames(driverNames);
	for(set<string>::iterator it=driverNames.begin(); it!=driverNames.end(); ++it)
		help["new"]+=" "+*it;
	set<string> commNames;
	CommPort::getRegistry().getTypeNames(commNames);
	help["new"]+="\n  Communication ports types are:";
	for(set<string>::iterator it=commNames.begin(); it!=commNames.end(); ++it)
		help["new"]+=" "+*it;
	
	help["delete"]="Remove an entry from the CommPort or Drivers list";
		
	if(args.size()==1) {
		cout << "Available commands: " << endl;
		for(map<string,string>::const_iterator it=help.begin(); it!=help.end(); ++it) {
			cout << '\t' << it->first << " " << syntax[it->first] << endl;
		}
		cout << "type 'help <command>' for more information" << endl;
	} else {
		if(help.find(args[1])==help.end()) {
			cout << "The command '"<< args[1] << "' was not found" << endl;
			return;
		}
		if(args.size()==2) {
			cout << args[1] << " " << syntax[args[1]] << endl;
			cout << help[args[1]] << endl;
		} else {
			if(args[1]=="set") {
				plist::ObjectBase* ob=sim::config.resolveEntry(args[2]);
				if(ob==NULL) {
					cout << "'" << args[2] << "' is unknown" << endl;
					return;
				}
				size_t n=args[2].rfind('.');
				if(n==string::npos)
					cout << sim::config.getComment(args[2]) << endl;
				else {
					ob=sim::config.resolveEntry(args[2].substr(0,n));
					if(const plist::Dictionary * dict=dynamic_cast<const plist::Dictionary*>(ob))
						cout << dict->getComment(args[2].substr(n+1)) << endl;
					else
						cout << "'" << args[2].substr(0,n) << "' is not a dictionary" << endl;
				}
			} else {
				cout << args[1] << " " << syntax[args[1]] << endl;
				cout << help[args[1]] << endl;
			}
		}
	}
}
void Simulator::cmdStep(const std::vector<std::string>& args) {
	if(args.size()<=1) {
		if(globals->timeScale!=0)
			globals->timeScale=0;
		step=STEP_NONE;
		incrementTime();
		return;
	}
	if(args.size()>2) {
		cout << args[0] << " takes 0 or 1 arguments; " << args.size()-1 << " supplied" << endl;
		return;
	}
	if(args[1]=="camera")
		step=STEP_CAMERA;
	else if(args[1]=="sensor" || args[1]=="sensors")
		step=STEP_SENSOR;
	else if(args[1]=="timer")
		step=STEP_TIMER;
	else if(args[1]=="motion")
		step=STEP_MOTION;
	else {
		cout << args[1] << " is not a valid argument for 'step'.  Type 'help step'." << endl;
		return;
	}
	if(step==STEP_CAMERA && vision.frozen && !vision.heartbeat) {
		cout << "Camera queue is frozen and has no heartbeat, cannot step (use 'advance' instead)" << endl;
		step=STEP_NONE;
	} else if(step==STEP_SENSOR && sensors.frozen && !sensors.heartbeat) {
		cout << "Sensor queue is frozen and has no heartbeat, cannot step (use 'advance' instead)" << endl;
		step=STEP_NONE;
	} else {
		unsigned int steps=getNextFrame();
		if((1<<step) & steps) { // the desired step is the next step -- just increment
			if(globals->timeScale!=0)
				globals->timeScale=0;
			step=STEP_NONE;
			incrementTime();
		} else if(globals->timeScale!=-1)
			globals->timeScale=-1;
	}
}
void Simulator::cmdStatus(const std::vector<std::string>& args) {
	cout << "Speed is " << static_cast<float>(globals->timeScale);
	if(globals->timeScale<0)
		cout << " (full speed mode: avg speed=" << ((globals->simulatorTime-fullspeedSimStart)/fullspeedWallStart.Age().Value()/1000) << "x, "
			<< " current speed=" << (avgSimTime/avgWallTime/1000) << "x)";
	cout << endl;
	cout << "Current time is " << get_time() << endl;
	unsigned int vis=vision.nextTimestamp();
	unsigned int sen=sensors.nextTimestamp();
	unsigned int tim=globals->getNextTimer();
	unsigned int mot=globals->getNextMotion();
	cout << "Next camera: ";
	if(vis==-1U) cout << "(none)"; else {
		if(vision.frozen)
			cout << "Frozen@";
		cout << vision.getDataSource()->nextName();
		if(!vision.frozen || vision.heartbeat) {
			if(vision.frozen && vision.heartbeat && vision.getDataSource()->nextName()!="heartbeat")
				cout << " heartbeat";
			cout << " scheduled at " << vis;
		}
	}
	cout << endl;
	cout << "Next sensor: ";
	if(sen==-1U) cout << "(none)"; else {
		if(sensors.frozen)
			cout << "Frozen@";
		cout << sensors.getDataSource()->nextName();
		if(!sensors.frozen || sensors.heartbeat) {
			if(sensors.frozen && sensors.heartbeat && sensors.getDataSource()->nextName()!="heartbeat")
				cout << " heartbeat";
			cout << " scheduled at " << sen;
		}
	}
	cout << endl;
	cout << "Next timer: ";
	if(tim==-1U) cout << "(none)"; else cout << tim;
	cout << endl;
	cout << "Next motion: ";
	if(mot==-1U) cout << "(none)"; else cout << mot;
	cout << endl;
	unsigned int semUsed=MessageQueueBase::getSemaphoreManager()->used();
	unsigned int semMax=semUsed+MessageQueueBase::getSemaphoreManager()->available();
	cout << "Semaphores used: " << semUsed << "/" << semMax << " (" << ((semUsed*10000+5)/semMax/10)/10.f << "%)" << endl;
	cout << endl;
	if(args.size()>1) {
		SemaphoreManager::semid_t sem=statusRequest->addReadStatusListener();
		for(unsigned int i=1; i<args.size(); ++i) {
			RCRegion * region=new RCRegion(args[i].size()+1);
			strncpy(region->Base(),args[i].c_str(),args[i].size()+1);
			statusRequest->sendMessage(region);
			region->RemoveReference();
		}
		//wait until they're done to put the prompt back up
		if(sem!=statusRequest->getSemaphoreManager()->invalid()) {
			statusRequest->getSemaphoreManager()->lower(sem,args.size()-1);
			statusRequest->removeReadStatusListener(sem);
		}
		//check to see if we're included:
		//haha, now I remember why I don't use functional programming
		/*if(find_if(args.begin()+1,args.end(),not1(__gnu_cxx::compose1(bind2nd(ptr_fun(strcasecmp),getName().c_str()),mem_fun_ref(&string::c_str))))!=args.end())
			statusReport(cout);*/
		for(unsigned int i=1; i<args.size(); ++i) {
			if(strcasecmp(args[i].c_str(),getName())==0 || strcasecmp(args[i].c_str(),"all")==0) {
				statusReport(cout);
				cout << endl;
			}
		}
	}
}
void Simulator::cmdAdvance(const std::vector<std::string>& args) {
	if(curLevel!=SharedGlobals::RUNNING) {
		cout << args[0] << " can only be used in the RUNNING runlevel" << endl;
		return;
	}
	std::string senstr="sensors";
	std::string camstr="camera";
	std::vector<std::string> queuenames;
	bool isAll=false;
	if(args.size()<=1) { // no arguments supplied, advance all
		queuenames.push_back(senstr);
		queuenames.push_back(camstr);
		isAll=true;
	}
	for(unsigned int i=1; i<args.size(); ++i) {
		if(args[i]==camstr)
			queuenames.push_back(camstr);
		else if(args[i]==senstr)
			queuenames.push_back(senstr);
		else if(args[i]=="all") {
			queuenames.push_back(senstr);
			queuenames.push_back(camstr);
			isAll=true;
		} else {
			cout << "invalid argument for advance command: " << args[i] << endl;
			return;
		}
	}
	for(std::vector<std::string>::iterator it=queuenames.begin(); it!=queuenames.end(); ++it) {
		LoadDataThread * lft=NULL;
		MessageQueueBase* q=NULL;
		if(*it==camstr) {
			lft=&vision;
			q=&(*cameraQueue);
		} else if(*it==senstr) {
			lft=&sensors;
			q=&(*sensorQueue);
		} else {
			cout << "Simulator: internal error, invalid queue " << *it << endl;
			return;
		}
		SemaphoreManager::semid_t sem=q->addReadStatusListener(); //register read status listener before sending!
		bool sent=lft->advanceFrame(true); // send frame
		if(!sent) { // no data in queue
			sent=lft->heartbeat; // but there may have been a heartbeat?
			if(!isAll) {
				// only report empty queue if the queue was explicitly specified
				cout << "No data in " << *it << " queue";
				if(lft->heartbeat)
					cout << ", sent heartbeat";
				cout << endl;
			}
		}
		if(sent)
			q->getSemaphoreManager()->lower(sem,true); //block until we know message was read
		q->removeReadStatusListener(sem);
	}
}
void Simulator::cmdFreeze(bool v, const std::vector<std::string>& args) {
	std::string senstr="sensors";
	std::string camstr="camera";
	std::set<std::string> queuenames;
	if(args.size()<=1) {
		queuenames.insert(camstr);
		queuenames.insert(senstr);
	}
	for(unsigned int i=1; i<args.size(); ++i) {
		if(args[i]==camstr)
			queuenames.insert(camstr);
		else if(args[i]==senstr)
			queuenames.insert(senstr);
		else if(args[i]=="all") {
			queuenames.insert(camstr);
			queuenames.insert(senstr);
		} else {
			cout << "invalid argument for freeze/unfreeze command: " << args[i] << endl;
			return;
		}
	}
	for(std::set<std::string>::iterator it=queuenames.begin(); it!=queuenames.end(); ++it) {
		if(*it==camstr)
			vision.frozen=v;
		else if(*it==senstr)
			sensors.frozen=v;
		else {
			cout << "Simulator: internal error, invalid queue " << *it << endl;
			return;
		}
	}
}
void Simulator::cmdReset(const std::vector<std::string>& args) {
	std::string senstr="sensors";
	std::string camstr="camera";
	for(unsigned int i=1; i<args.size(); ++i) {
		if(args[i]==camstr) {
			if(vision.getDataSource()!=NULL)
				vision.getDataSource()->reset();
		} else if(args[i]==senstr) {
			if(sensors.getDataSource()!=NULL)
				sensors.getDataSource()->reset();
		} else if(args[i]=="all") {
			if(vision.getDataSource()!=NULL)
				vision.getDataSource()->reset();
			if(sensors.getDataSource()!=NULL)
				sensors.getDataSource()->reset();
		} else
			cout << "invalid argument for reset command: " << args[i] << endl;
	}
}
void Simulator::cmdNew(const std::vector<std::string>& args) {
	set<string> driverNames;
	DeviceDriver::getRegistry().getTypeNames(driverNames);
	set<string> commNames;
	CommPort::getRegistry().getTypeNames(commNames);
	if(args.size()<2) {
		cerr << "Must specify type to instantiate:\n";
		cerr << "  Communication Ports:";
		for(set<string>::iterator it=commNames.begin(); it!=commNames.end(); ++it)
			cerr << ' ' << *it;
		cerr << endl;
		cerr << "  Device Drivers:";
		for(set<string>::iterator it=driverNames.begin(); it!=driverNames.end(); ++it)
			cerr << ' ' << *it;
		cerr << endl;
		return;
	}
	std::string type = args[1];
	std::string name = (args.size()>2) ? args[2] : args[1];
	try {
		if(driverNames.find(type)!=driverNames.end()) {
			DeviceDriver * d = DeviceDriver::getRegistry().create(type,name);
			if(d==NULL) {
				cerr << "Error instantiating " << type << " (instance with same name already exists?)" << endl;
				return;
			}
		} else if(commNames.find(type)!=commNames.end()) {
			CommPort * c = CommPort::getRegistry().create(type,name);
			if(c==NULL) {
				cerr << "Error instantiating " << type << " (instance with same name already exists?)" << endl;
				return;
			}
		} else {
			cerr << "'" << type << "' is not a valid type for instantiation.  Please choose one of:\n";
			cerr << "  Communication Ports:";
			for(set<string>::iterator it=commNames.begin(); it!=commNames.end(); ++it)
				cerr << ' ' << *it;
			cerr << endl;
			cerr << "  Device Drivers:";
			for(set<string>::iterator it=driverNames.begin(); it!=driverNames.end(); ++it)
				cerr << ' ' << *it;
			cerr << endl;
		}
	} catch(const std::exception& e) {
		cout << "An exception occured during construction of "<<type<<": " << e.what() << endl;
	} catch(...) {
		cout << "An exception occured during construction of "<<type << endl;
	}
}

void Simulator::cmdDelete(const std::vector<std::string>& args) {
	if(args.size()<2) {
		cerr << "Must specify instance to delete:\n";
		cerr << "  Communication Ports:";
		for(CommPort::registry_t::const_iterator it=CommPort::getRegistry().begin(); it!=CommPort::getRegistry().end(); ++it)
			cerr << ' ' << it->first;
		cerr << endl;
		cerr << "  Device Drivers:";
		for(DeviceDriver::registry_t::const_iterator it=DeviceDriver::getRegistry().begin(); it!=DeviceDriver::getRegistry().end(); ++it)
			cerr << ' ' << it->first;
		cerr << endl;
		return;
	}
	if(DeviceDriver::getRegistry().getInstance(args[1])!=NULL) {
		if(!DeviceDriver::getRegistry().destroy(args[1]))
			cerr << "Could not delete driver named '" << args[1] << "'" << endl;
	} else if(CommPort::getRegistry().getInstance(args[1])!=NULL) {
		if(!CommPort::getRegistry().destroy(args[1]))
			cerr << "Could not delete comm port named '" << args[1] << "'" << endl;
	} else {
		cerr << "Could not find driver to delete named '" << args[1] << "'" << endl;
		return;
	}
}


/*! @file
 * @brief 
 * @author ejt (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.13 $
 * $State: Exp $
 * $Date: 2007/11/19 21:00:41 $
 */

