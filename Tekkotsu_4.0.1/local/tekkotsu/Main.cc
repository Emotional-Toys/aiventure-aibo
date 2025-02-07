#include "Main.h"
#include "SoundPlay.h"
#include "Motion.h"
#include "Simulator.h"
#include "TimerExecThread.h"
#include "local/LoadDataThread.h"
#include "SimConfig.h"
#include "MotionExecThread.h"

#include "IPC/RegionRegistry.h"
#include "IPC/MessageReceiver.h"
#include "IPC/PollThread.h"
#include "Motion/Kinematics.h"
#include "Motion/PostureEngine.h"
#include "Wireless/Wireless.h"
#include "Shared/ProjectInterface.h"
#include "Behaviors/BehaviorBase.h"
#include "Events/DataEvent.h"
#include "Events/EventRouter.h"
#include "Shared/Config.h"
#include "Shared/MarkScope.h"

#include "Events/EventBase.h"
#include "Events/LocomotionEvent.h"
#include "Events/TextMsgEvent.h"
#include "Events/VisionObjectEvent.h"

using namespace std;

Main::Main()
	: Process(getID(),getClassName()),
	sounds(ipc_setup->registerRegion(SoundPlay::getSoundPlayID(),sizeof(sim::SoundPlayQueue_t))),
	motions(ipc_setup->registerRegion(Motion::getMotionCommandID(),sizeof(sim::MotionCommandQueue_t))),
	events(ipc_setup->registerRegion(getEventsID(),sizeof(sim::EventQueue_t))),
	cameraFrames(ipc_setup->registerRegion(Simulator::getCameraQueueID(),sizeof(sim::CameraQueue_t))),
	sensorFrames(ipc_setup->registerRegion(Simulator::getSensorQueueID(),sizeof(sim::SensorQueue_t))),
	timerWakeup(ipc_setup->registerRegion(Simulator::getTimerWakeupID(),sizeof(sim::TimerWakeup_t))),
	statusRequest(ipc_setup->registerRegion(Simulator::getStatusRequestID(),sizeof(sim::StatusRequest_t))),
	motionmanager(ipc_setup->registerRegion(Motion::getMotionManagerID(),sizeof(MotionManager))),
	soundmanager(ipc_setup->registerRegion(SoundPlay::getSoundManagerID(),sizeof(SoundManager))),
	worldstatepool(ipc_setup->registerRegion(getWorldStatePoolID(),sizeof(WorldStatePool))),
	motionProf(ipc_setup->registerRegion(Motion::getMotionProfilerID(),sizeof(motionProfiler_t))),
	soundProf(ipc_setup->registerRegion(SoundPlay::getSoundProfilerID(),sizeof(soundProfiler_t))),
	visrecv(NULL), sensrecv(NULL), evtrecv(NULL), timerrecv(NULL), statusrecv(NULL), timerExec(NULL),
	visionRead(true), wireless_thread(), worldStateCache(), behaviorLock(*worldstatepool),
	curimgregion(NULL), img(), lastVisionSN(-1U), lastSensorSN(-1U), lastSensorUpdateTime(0)
{
	new (&(*events)) sim::EventQueue_t;
	new (&(*worldstatepool)) WorldStatePool;
	state=NULL;
	motman=&(*motionmanager);
	sndman=&(*soundmanager);
	::mainProfiler=new mainProfiler_t;
	::motionProfiler=&(*motionProf);
	::soundProfiler=&(*soundProf);

	if(sim::config.multiprocess) {
		//Setup wireless
		ASSERT(wireless==NULL,"global wireless already initialized before Main?");
		wireless = new Wireless();
		sout=wireless->socket(Socket::SOCK_STREAM,Wireless::WIRELESS_DEF_RECV_SIZE,Wireless::WIRELESS_DEF_SEND_SIZE*12);
		serr=wireless->socket(Socket::SOCK_STREAM,Wireless::WIRELESS_DEF_RECV_SIZE,Wireless::WIRELESS_DEF_SEND_SIZE*4);
		wireless->setDaemon(sout);
		wireless->setDaemon(serr);
		serr->setFlushType(Socket::FLUSH_BLOCKING);
		sout->setTextForward();
		serr->setForward(sout);
		
		//Setup Kinematics
		ASSERT(kine==NULL,"global kine already initialized before Main?");
		kine=new Kinematics();
	}
	wireless->setCallbackLock(behaviorLock);

	//EventRouter and Config are set up for all processes by main() before fork
}

Main::~Main() {
	if(sim::config.multiprocess) {
		delete wireless;
		wireless=NULL;
		delete kine;
		kine=NULL;
	}
}


void Main::DoStart() {
try {
	Process::DoStart();
	//These are constructed by other processes, so need to wait
	//until the construction runlevel is complete
	sndman->InitAccess(*sounds);
	motman->InitAccess(*motions,behaviorLock);

	wireless->listen(sout, config->main.console_port);
	wireless->listen(serr, config->main.stderr_port);
	wireless_thread.start();
	statusrecv=new MessageReceiver(*statusRequest,statusReport);

	if(globals->waitForSensors)
		erouter->addListener(this,EventBase::sensorEGID);
	evtrecv=new MessageReceiver(*events,gotEvent,false);
	visrecv=new MessageReceiver(*cameraFrames,gotCamera,false);
	sensrecv=new MessageReceiver(*sensorFrames,gotSensors,false);
	timerrecv=new MessageReceiver(*timerWakeup,gotTimer,false);
	timerExec=new TimerExecThread(behaviorLock,false);
	
} catch(const std::exception& ex) {
	if(!ProjectInterface::uncaughtException(__FILE__,__LINE__,"Occurred during Main DoStart",&ex))
		throw;
} catch(...) {
	if(!ProjectInterface::uncaughtException(__FILE__,__LINE__,"Occurred during Main DoStart",NULL))
		throw;
}
}

void Main::run() {
try {
	evtrecv->start();
	if(globals->waitForSensors) {
		sensrecv->start();
		globals->waitSensors();
	}

	// might have shutdown triggered while waiting
	// (perhaps device isn't available, user's killing the process...)
	if(globals->isShutdown())
		return; // skip running altogether
	
	{
		MarkScope bl(behaviorLock);
		ProjectInterface::startupBehavior().DoStart();
		globals->setNextTimer(erouter->getNextTimer());
	}
	
	if(!globals->waitForSensors)
		sensrecv->start();
	visrecv->start();
	timerrecv->start();
	timerExec->reset();
	
	// this is a bit of a hack, but once we're done launching, display the prompt:
	cout << sim::config.cmdPrompt << flush;
	
	Process::run();

	sensrecv->finish();
	visrecv->finish();
	evtrecv->finish();
	timerrecv->finish();
	
	{
		MarkScope bl(behaviorLock);
		ProjectInterface::startupBehavior().DoStop();
		globals->setNextTimer(erouter->getNextTimer());
	}
	timerExec->reset();

} catch(const std::exception& ex) {
	if(!ProjectInterface::uncaughtException(__FILE__,__LINE__,"Occurred during Main 'run' runlevel (startupBehavior initialization and startup)",&ex))
		throw;
} catch(...) {
	if(!ProjectInterface::uncaughtException(__FILE__,__LINE__,"Occurred during Main 'run' runlevel (startupBehavior initialization and startup)",NULL))
		throw;
}
}

void Main::DoStop() {
try {

	delete sensrecv;
	sensrecv=NULL;
	delete visrecv;
	visrecv=NULL;
	delete evtrecv;
	evtrecv=NULL;
	delete timerrecv;
	timerrecv=NULL;
	delete statusrecv;
	statusrecv=NULL;
	
	motman->RemoveAccess();
	
	if(curimgregion!=NULL)
		curimgregion->RemoveReference();

	wireless_thread.stop();
	wireless_thread.join();
	wireless->setDaemon(sout,false);
	wireless->close(sout);
	sout=NULL;
	wireless->setDaemon(serr,false);
	wireless->close(serr);
	serr=NULL;
	
	Process::DoStop();

} catch(const std::exception& ex) {
	if(!ProjectInterface::uncaughtException(__FILE__,__LINE__,"Occurred during Main DoStop",&ex))
		throw;
} catch(...) {
	if(!ProjectInterface::uncaughtException(__FILE__,__LINE__,"Occurred during Main DoStop",NULL))
		throw;
}
}

void Main::processEvent(const EventBase&) {
	erouter->removeListener(this);
	for(unsigned int i=0; i<NumOutputs; i++)
		motman->setOutput(NULL,i,state->outputs[i]);
	globals->signalHaveSensors();
}

bool Main::gotCamera(RCRegion* msg) {
	if(msg==NULL)
		return true;
	Main * main=dynamic_cast<Main*>(Process::getCurrent());
	ASSERTRETVAL(main!=NULL,"gotCamera, but not within Main process!",true);
	MarkScope bl(main->behaviorLock);
	PROFSECTION("GotImage()",*mainProfiler);

	try {
		BufferedImageGenerator::ImageSource& img=main->img;
		
		unsigned int verbose;
		unsigned int sn;
		string file;
		unsigned int payload;
		int l;
		char* buf=LoadDataThread::deserializeHeader(msg->Base(),msg->Size(),&verbose,&sn,&file,NULL,&payload);
		unsigned int remain=payload;
		if(verbose>=1 && sn-main->lastVisionSN!=1)
			cout << "Main dropped " << (sn-main->lastVisionSN-1) << " camera frame(s)" << endl;
		main->lastVisionSN=sn;
		if(verbose>=3 && remain==0)
			cout << "Main received image heartbeat at " << get_time() << endl;
		else if(verbose>=2 && remain!=0)
			cout << "Main received image data \"" << file << "\" at " << get_time() << endl;
		
		img.frameIndex=sn;
		if(remain==0) {
			if(img.width==0 || img.height==0 || img.img==NULL)
				return true; // can't do the heartbeat, don't have an initial image to replicate
		} else {
			LoadSave::decodeIncT(l,buf,remain);
			LoadSave::decodeIncT(img.width,buf,remain);
			LoadSave::decodeIncT(img.height,buf,remain);
			LoadSave::decodeIncT(img.channels,buf,remain);
			if(l!=0) {
				img.layer = (l<0)?ProjectInterface::defRawCameraGenerator->getNumLayers()+l:l-1;
			} else {
				// using "automatic" mode, pick the layer closest to resolution of provided image
				// assumes each layer doubles in size, with smallest layer at 0
				float fullRes=sqrt(CameraResolutionX*CameraResolutionY); // from RobotInfo
				float givenRes=sqrt(img.width*img.height);
				if(givenRes==0) {
					cerr << "Main received empty image!" << endl;
					return true;
				} else {
					float ratio=log2f(givenRes/fullRes);
					int layerOff=static_cast<int>(rintf(ratio));
					int tgtLayer=static_cast<int>(ProjectInterface::fullLayer)+layerOff;
					if(tgtLayer<0)
						img.layer=0;
					else if(ProjectInterface::defRawCameraGenerator!=NULL && static_cast<unsigned int>(tgtLayer)>=ProjectInterface::defRawCameraGenerator->getNumLayers())
						img.layer=ProjectInterface::defRawCameraGenerator->getNumLayers()-1;
					else
						img.layer=tgtLayer;
					if(static_cast<unsigned int>(tgtLayer)!=img.layer)
						cerr << "Image dimensions of " << img.width << "x" << img.height << " are well beyond the available resolution layers (full is " << CameraResolutionX << "x" << CameraResolutionY << ")" << endl;
				}
			}
			img.img=reinterpret_cast<unsigned char*>(buf);
			msg->AddReference();
			if(main->curimgregion!=NULL)
				main->curimgregion->RemoveReference();
			main->curimgregion=msg;
		}
		DataEvent<BufferedImageGenerator::ImageSource> dev(img,EventBase::visOFbkEGID,0,EventBase::activateETID);
		erouter->postEvent(dev);
		dev.setTypeID(EventBase::statusETID);
		erouter->postEvent(dev);
		dev.setTypeID(EventBase::deactivateETID);
		erouter->postEvent(dev);
	} catch(const std::exception& ex) {
		if(!ProjectInterface::uncaughtException(__FILE__,__LINE__,"Occurred during camera frame processing",&ex))
			throw;
	} catch(...) {
		if(!ProjectInterface::uncaughtException(__FILE__,__LINE__,"Occurred during camera frame processing",NULL))
			throw;
	}
	try {
		erouter->processTimers();
	} catch(const std::exception& ex) {
		if(!ProjectInterface::uncaughtException(__FILE__,__LINE__,"Occurred during timer processing",&ex))
			throw;
	} catch(...) {
		if(!ProjectInterface::uncaughtException(__FILE__,__LINE__,"Occurred during timer processing",NULL))
			throw;
	}
	if(globals->setNextTimer(erouter->getNextTimer()))
		main->timerExec->reset();
	return true;
}

bool Main::gotSensors(RCRegion* msg) {
	Main * main=dynamic_cast<Main*>(Process::getCurrent());
	ASSERTRETVAL(main!=NULL,"gotSensors, but not within Main process!",true);

	PROFSECTION("GotSensorFrame()",*mainProfiler);
	MarkScope l(main->behaviorLock);

	string exceptionPhase="sensor";
	try {
		unsigned int orgSN=main->lastSensorSN;
		WorldStatePool::UpdateInfo * info=main->worldstatepool->isUnread(*msg,globals->motion.frameNumber,main->lastSensorSN,false,globals->motion.override);
		int dif=0;
		if(info!=NULL) {
			bool generateFeedback=(globals->motion.hasUnprovidedOutput() || globals->motion.override) && globals->motion.feedbackDelay>=0;
			EntryPoint::WorldStateWrite wsw(info->frameNumber);
			MarkScope lw(main->behaviorLock, wsw);
			if(state!=NULL && wsw.frame==globals->motion.frameNumber) {
				// ^-- make sure a newer one didn't come in while we were waiting for the lock
				// (if one did, we'll report the accumulated drops next time we get one we don't drop)
				if(main->worldstatepool->read(*info,wsw,generateFeedback,globals->motion.zeroPIDFeedback,NULL)) {
					if(wsw.frame-main->lastSensorSN!=1 && info->verbose>=1)
						cout << ProcessID::getIDStr() << " dropped " << (wsw.frame-main->lastSensorSN-1) << " sensor frame(s)" << endl;
					main->lastSensorSN=wsw.frame;
				}
				if(wsw.getComplete()) {
					dif=state->lastSensorUpdateTime - main->lastSensorUpdateTime;
					ASSERT(dif>=0,"sensor update time is negative? " << dif);
					main->lastSensorUpdateTime=state->lastSensorUpdateTime;
					erouter->postEvent(EventBase::sensorEGID,SensorSrcID::UpdatedSID,EventBase::statusETID,dif,"SensorSrcID::UpdatedSID",1);
					exceptionPhase="timer";
					erouter->processTimers();
				}
			}
		} else if(orgSN!=main->lastSensorSN) {
			// received update after sensor frame was already parsed by other process
			l.reset(); //update state
			dif=state->lastSensorUpdateTime - main->lastSensorUpdateTime;
			ASSERT(dif>=0,"sensor update time is negative? " << dif);
			main->lastSensorUpdateTime=state->lastSensorUpdateTime;
			erouter->postEvent(EventBase::sensorEGID,SensorSrcID::UpdatedSID,EventBase::statusETID,dif,"SensorSrcID::UpdatedSID",1);
			exceptionPhase="timer";
			erouter->processTimers();
		}
		if(dif==0) {
			exceptionPhase="timer";
			erouter->processTimers();
		}
	} catch(const std::exception& ex) {
		if(!ProjectInterface::uncaughtException(__FILE__,__LINE__,("Occurred during "+exceptionPhase+" processing").c_str(),&ex))
			throw;
	} catch(...) {
		if(!ProjectInterface::uncaughtException(__FILE__,__LINE__,("Occurred during "+exceptionPhase+" processing").c_str(),NULL))
			throw;
	}
	if(globals->setNextTimer(erouter->getNextTimer()))
		main->timerExec->reset();
	return true;
}

bool Main::gotEvent(RCRegion* msg) {
	if(msg==NULL)
		return true;
	Main * main=dynamic_cast<Main*>(Process::getCurrent());
	ASSERTRETVAL(main!=NULL,"gotEvent, but not within Main process!",true);
	MarkScope l(main->behaviorLock);
	EventBase* evt=NULL;
	try {
		evt=EventTranslator::decodeEvent(msg->Base(),msg->Size());
		if(evt==NULL) {
			cerr << "ERROR: Main::gotEvent() failed to decode message" << endl;
			return true;
		}
		if(evt->getGeneratorID()==EventBase::sensorEGID)
			main->lastSensorUpdateTime=evt->getTimeStamp();
		erouter->postEvent(*evt);
	} catch(const std::exception& ex) {
		std::string emsg("Occurred during inter-process event processing");
		if(evt!=NULL)
			emsg+=": "+evt->getName();
		delete evt;
		evt=NULL;
		if(!ProjectInterface::uncaughtException(__FILE__,__LINE__,emsg.c_str(),&ex))
			throw;
	} catch(...) {
		std::string emsg("Occurred during inter-process event processing");
		if(evt!=NULL)
			emsg+=": "+evt->getName();
		delete evt;
		evt=NULL;
		if(!ProjectInterface::uncaughtException(__FILE__,__LINE__,emsg.c_str(),NULL))
			throw;
	}
	delete evt;
	evt=NULL;
	try {
		erouter->processTimers();
	} catch(const std::exception& ex) {
		if(!ProjectInterface::uncaughtException(__FILE__,__LINE__,"Occurred during timer processing",&ex))
			throw;
	} catch(...) {
		if(!ProjectInterface::uncaughtException(__FILE__,__LINE__,"Occurred during timer processing",NULL))
			throw;
	}
	if(globals->setNextTimer(erouter->getNextTimer()))
		main->timerExec->reset();
	return true;
}

bool Main::gotTimer(RCRegion* /*msg*/) {
	Main * main=dynamic_cast<Main*>(Process::getCurrent());
	ASSERTRETVAL(main!=NULL,"gotTimer, but not within Main process!",true);

	ASSERTRETVAL(main->timerExec!=NULL,"timerExec thread is NULL when timer wakeup received",true);
	main->timerExec->reset();
	
	if(globals->timeScale<=0) {
		MarkScope bl(main->behaviorLock);
		try {
			erouter->processTimers();
		} catch(const std::exception& ex) {
			if(!ProjectInterface::uncaughtException(__FILE__,__LINE__,"Occurred during timer processing",&ex))
				throw;
		} catch(...) {
			if(!ProjectInterface::uncaughtException(__FILE__,__LINE__,"Occurred during timer processing",NULL))
				throw;
		}
		globals->setNextTimer(erouter->getNextTimer());
		// don't need to reset timerExec (again) because we're only here if speed<=0, in which case timerExec isn't running	main->timerExec->reset();
	}
	return true;
}

/*! @file
 * @brief 
 * @author ejt (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.4 $
 * $State: Exp $
 * $Date: 2007/11/11 23:57:30 $
 */

