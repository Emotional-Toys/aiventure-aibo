#include "Motion.h"
#include "Main.h"
#include "SoundPlay.h"
#include "Simulator.h"
#include "SimConfig.h"
#include "Shared/Config.h"
#include "MotionExecThread.h"

#include "IPC/RegionRegistry.h"
#include "IPC/MessageReceiver.h"
#include "Motion/Kinematics.h"
#include "Wireless/Wireless.h"
#include "Events/EventRouter.h"
#include "Shared/MarkScope.h"

#include "local/MotionHooks/IPCMotionHook.h"

using namespace std;

Motion::Motion()
	: Process(getID(),getClassName()),
	sounds(ipc_setup->registerRegion(SoundPlay::getSoundPlayID(),sizeof(sim::SoundPlayQueue_t))),
	motions(ipc_setup->registerRegion(getMotionCommandID(),sizeof(sim::MotionCommandQueue_t))),
	motionout(ipc_setup->registerRegion(getMotionOutputID(),sizeof(sim::MotionOutput_t))),
	events(ipc_setup->registerRegion(Main::getEventsID(),sizeof(sim::EventQueue_t))),
	sensorFrames(ipc_setup->registerRegion(Simulator::getSensorQueueID(),sizeof(sim::SensorQueue_t))),
	statusRequest(ipc_setup->registerRegion(Simulator::getStatusRequestID(),sizeof(sim::StatusRequest_t))),
	motionmanager(ipc_setup->registerRegion(getMotionManagerID(),sizeof(MotionManager))),
	soundmanager(ipc_setup->registerRegion(SoundPlay::getSoundManagerID(),sizeof(SoundManager))),
	worldstatepool(ipc_setup->registerRegion(Main::getWorldStatePoolID(),sizeof(WorldStatePool))),
	motionWakeup(ipc_setup->registerRegion(Simulator::getMotionWakeupID(),sizeof(sim::MotionWakeup_t))),
	motionProf(ipc_setup->registerRegion(getMotionProfilerID(),sizeof(motionProfiler_t))),
	etrans(NULL), sensrecv(NULL), statusrecv(NULL), wakeuprecv(NULL), motionExec(NULL), motionfwd(NULL),
	wireless_thread(), motionLock(*worldstatepool), lastSensorSN(-1U)
{
	new (&(*motions)) sim::MotionCommandQueue_t;
	new (&(*motionout)) sim::MotionOutput_t;
	new (&(*motionmanager)) MotionManager;
	new (&(*motionProf)) motionProfiler_t;
	motman=&(*motionmanager);
	motman->InitAccess(*motions,motionLock);
	sndman=&(*soundmanager);
	state=NULL;
	::motionProfiler=&(*motionProf);
	
	if(sim::config.multiprocess) {
		//Setup wireless
		ASSERT(wireless==NULL,"global wireless already initialized before Motion?");
		wireless = new Wireless();
		sout=wireless->socket(Socket::SOCK_STREAM,Wireless::WIRELESS_DEF_RECV_SIZE,Wireless::WIRELESS_DEF_SEND_SIZE*12);
		serr=wireless->socket(Socket::SOCK_STREAM,Wireless::WIRELESS_DEF_RECV_SIZE,Wireless::WIRELESS_DEF_SEND_SIZE*4);
		wireless->setDaemon(sout);
		wireless->setDaemon(serr);
		serr->setFlushType(Socket::FLUSH_BLOCKING);
		sout->setTextForward();
		serr->setForward(sout);

		//Setup Kinematics
		ASSERT(kine==NULL,"global kine already initialized before Motion?");
		kine=new Kinematics();
	}

	//EventRouter and Config are set up for all processes by main() before fork
}

Motion::~Motion() {
	MotionManager::setTranslator(NULL);
	delete etrans;
	etrans=NULL;
	if(sim::config.multiprocess) {
		delete wireless;
		wireless=NULL;
		delete kine;
		kine=NULL;
	}
}

void Motion::DoStart() {
	Process::DoStart();
	//These are constructed by other processes, so need to wait
	//until the construction runlevel is complete before we access them
	sndman->InitAccess(*sounds);
	if(!sim::config.multiprocess) {
		// don't use our own etrans here, because erouter will delete it for us, don't want a double-delete in our destructor...
		EventTranslator * forwardTrans = new IPCEventTranslator(*events);
		forwardTrans->setTrapEventValue(true);
		erouter->setForwardingAgent(getID(),forwardTrans);
		MotionManager::setTranslator(forwardTrans);
	} else {
		etrans=new IPCEventTranslator(*events);
		MotionManager::setTranslator(etrans);
		
		// Set up Event Translator to trap and send events to main process
		//send everything over except erouter events
		for(unsigned int i=0; i<EventBase::numEGIDs; i++)
			if(i!=EventBase::erouterEGID)
				erouter->addTrapper(etrans,static_cast<EventBase::EventGeneratorID_t>(i));
		
		wireless->listen(sout, config->motion.console_port);
		wireless->listen(serr, config->motion.stderr_port);
		wireless_thread.start();
	}
	
	sensrecv=new MessageReceiver(*sensorFrames,gotSensors);
	statusrecv=new MessageReceiver(*statusRequest,statusReport);
	wakeuprecv=new MessageReceiver(*motionWakeup,gotWakeup,false);

	motionExec=new MotionExecThread(motionLock);
	if(sim::config.multiprocess) {
		Simulator::registerMotionHook(*(motionfwd=new IPCMotionHook(*motionout)));
		Simulator::setMotionStarting();
		if(globals->timeScale>0)
			Simulator::setMotionEnteringRealtime();
	}
}


void Motion::run() {
	if(globals->waitForSensors)
		globals->waitSensors();
	
	// might have shutdown triggered while waiting
	// (perhaps device isn't available, user's killing the process...)
	if(globals->isShutdown())
		return; // skip running altogether
	
	motionExec->reset(); //starts the thread if appropriate
	wakeuprecv->start();
	Process::run();
	wakeuprecv->stop();
}

void Motion::DoStop() {
	Simulator::deregisterMotionHook(*motionfwd);
	delete motionfwd;
	
	wakeuprecv->finish();
	statusrecv->finish();

	delete motionExec;
	motionExec=NULL;
	delete wakeuprecv;
	wakeuprecv=NULL;
	delete statusrecv;
	statusrecv=NULL;
	
	sensrecv->finish();
	delete sensrecv;
	sensrecv=NULL;
	
	if(sim::config.multiprocess) {
		if(globals->timeScale>0)
			Simulator::setMotionLeavingRealtime(false);
		Simulator::setMotionStopping();
		
		erouter->removeTrapper(etrans);
		delete etrans;
		etrans=NULL;
		
		wireless_thread.stop();
		wireless_thread.join();
		wireless->setDaemon(sout,false);
		wireless->close(sout);
		sout=NULL;
		wireless->setDaemon(serr,false);
		wireless->close(serr);
		serr=NULL;
	}
	motman->RemoveAccess();
	
	Process::DoStop();
}

bool Motion::gotWakeup(RCRegion* /*msg*/) {
	Motion * motion=dynamic_cast<Motion*>(Process::getCurrent());
	ASSERTRETVAL(motion!=NULL,"gotWakeup, but not within motion process!",true);
	ASSERTRETVAL(motion->motionExec!=NULL,"motionExec thread is NULL when motion wakeup received",true);
	
	MarkScope l(motion->motionLock);
	if(sim::config.multiprocess) {
		if(globals->timeScale<=0 && motion->motionExec->isStarted())
			Simulator::setMotionLeavingRealtime(globals->timeScale<0);
		else if(globals->timeScale>0 && !motion->motionExec->isStarted())
			Simulator::setMotionEnteringRealtime();
	}
	if(globals->timeScale<=0 && get_time()>=globals->getNextMotion())
		motion->motionExec->poll();
	motion->motionExec->reset(); //reset will set globals->getNextMotion(), so have to do this after the poll itself
	return true;
}

bool Motion::gotSensors(RCRegion* msg) {
	Motion * motion=dynamic_cast<Motion*>(Process::getCurrent());
	ASSERTRETVAL(motion!=NULL,"gotSensors, but not within Motion process!",true);
	
	PROFSECTION("GotSensorFrame()",*motionProfiler);
	MarkScope l(motion->motionLock);
	
	EntryPoint::WorldStateWrite wsw(-1U);
	try {
		WorldStatePool::UpdateInfo * info=motion->worldstatepool->isUnread(*msg,globals->motion.frameNumber,motion->lastSensorSN,globals->motion.feedbackDelay>=0,globals->motion.override);
		int dif=0;
		if(info!=NULL) {
			bool generateFeedback=(globals->motion.hasUnprovidedOutput() || globals->motion.override) && globals->motion.feedbackDelay>=0;
			const PostureEngine* pose = generateFeedback ? &motion->motionExec->getPostureFeedback() : NULL;
			wsw.frame=info->frameNumber;
			MarkScope lw(motion->motionLock, wsw);
			if(state==NULL || wsw.frame<globals->motion.frameNumber) {
				//a newer one came in while we were waiting for the lock (we'll report the accumulated drops next time we get one we don't drop)
				return true;
			}
			ASSERT((wsw.getStatus()&WorldStatePool::FEEDBACK_APPLIED)==0,"motion feedback already applied?");
			bool completedPartial=!wsw.getComplete() && (wsw.getStatus()&WorldStatePool::SENSORS_APPLIED)!=0;
			if(generateFeedback && info->verbose>=2)
				cout << "Motion filling in sensor feedback" << endl;
			ASSERT(!completedPartial || generateFeedback,"partially complete, yet no feedback?");
			if(motion->worldstatepool->read(*info,wsw,generateFeedback,globals->motion.zeroPIDFeedback,pose)) {
				if(wsw.frame-motion->lastSensorSN!=1 && info->verbose>=1)
					cout << ProcessID::getIDStr() << " dropped " << (wsw.frame-motion->lastSensorSN-1) << " sensor frame(s)" << endl;
				motion->lastSensorSN=wsw.frame;
			}
			if(completedPartial && wsw.getComplete()) { // was partial before, now completed as well
				dif=state->lastSensorUpdateTime - wsw.src->lastSensorUpdateTime;
				ASSERT(dif>=0,"sensor update time is negative? " << dif << ' ' << state << ' ' << wsw.src);
			}
		}
		if(dif>0) {
			// need to notify main that the sensor update it started is now complete
			// do it outside the write lock so if main gets this instantly it can use the new data
			// some sleight of hand here -- we've previously registered an IPCEventTranslator as an event trapper.  By posting here, the event will be sent to the Main process
			erouter->postEvent(EventBase::sensorEGID,SensorSrcID::UpdatedSID,EventBase::statusETID,dif,"SensorSrcID::UpdatedSID",1);
		}
	} catch(const std::exception& ex) {
		if(!ProjectInterface::uncaughtException(__FILE__,__LINE__,"Occurred during sensor processing",&ex))
			throw;
	} catch(...) {
		if(!ProjectInterface::uncaughtException(__FILE__,__LINE__,"Occurred during sensor processing",NULL))
			throw;
	}
	
	return true;
}
	

/*! @file
 * @brief 
 * @author ejt (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.3 $
 * $State: Exp $
 * $Date: 2007/11/11 23:57:30 $
 */

