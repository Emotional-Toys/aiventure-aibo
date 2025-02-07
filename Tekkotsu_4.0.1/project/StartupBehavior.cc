#include "StartupBehavior.h"

#include "Behaviors/Controller.h"
#ifdef TGT_HAS_POWER_STATUS
#  include "Behaviors/Controls/BatteryCheckControl.h"
#endif
#include "Behaviors/Controls/ControlBase.h"
#include "Behaviors/Controls/HelpControl.h"
#include "Behaviors/Controls/RebootControl.h"
#include "Behaviors/Controls/ShutdownControl.h"

#include "Motion/MotionCommand.h"
#include "Motion/EmergencyStopMC.h"
#include "Motion/PIDMC.h"
#include "Motion/MotionSequenceMC.h"
#include "Motion/MMAccessor.h"

#include "Sound/SoundManager.h"

#include "Shared/ERS210Info.h"
#include "Shared/ERS7Info.h"

#include "Shared/ProjectInterface.h"

using namespace std;

BehaviorBase& ProjectInterface::startupBehavior() {
	// used by Main process, called after environmental setup is complete
	static StartupBehavior * theStartup=NULL;
	if(!theStartup)
		theStartup=new StartupBehavior;
	return *theStartup;
}

StartupBehavior::StartupBehavior()
	: BehaviorBase("StartupBehavior"), spawned(),spawnedMC(),setup(),
		stop_id(MotionManager::invalid_MC_ID),
		pid_id(MotionManager::invalid_MC_ID)
{	
			cout << "StartupBehavior()" << endl;
			SetAutoDelete(false); // this is a global, so there's a global reference, don't delete yourself
	//could instead AddReference(), but then the teardown code has to know that it should dereference on exit
}

StartupBehavior::~StartupBehavior() {}

void StartupBehavior::DoStart() {
	BehaviorBase::DoStart();
	//Initialize the Vision pipeline (it's probably a good idea to do this
	//first in case later stuff wants to reference the vision stages)
	initVision();

	//This will "fade" in the PIDs so the joints don't jerk to full
	//power, also looks cooler
	pid_id=motman->addPersistentMotion(SharedObject<PIDMC>(0),MotionManager::kEmergencyPriority+2);
	//also, pause before we start fading in, PIDs take effect right
	//away, before the emergencystop is picked up
	erouter->addTimer(this,0,4*FrameTime*NumFrames,true);
	spawnedMC.push_back(pid_id);

	//This sets up the default emergency stop
	SharedObject<EmergencyStopMC> stop;
	//if you want to start off unpaused, either change 'true' to
	//'false', or just comment out the next line (estop's default is off)
	//Note that if you don't want to start in estop, you should then
	//also uncomment the line at the end of this function
	stop->setStopped(true,false); 
	stop_id=motman->addPersistentMotion(stop,MotionManager::kEmergencyPriority);
	spawnedMC.push_back(stop_id);
	
#ifdef TGT_HAS_POWER_STATUS
	//This displays the current battery conditions on the console
	BatteryCheckControl batchk;
	//	const SharedObject<LedMC> led;
	//	batchk.activate(motman->addPrunableMotion(led,MotionManager::kEmergencyPriority+1),NULL);
	batchk.activate(MotionManager::invalid_MC_ID,NULL);
	batchk.deactivate();
#endif

	//This is what runs the menu system
	Controller * controller=new Controller;
	controller->DoStart();
	controller->setEStopID(stop_id);
	controller->setRoot(SetupMenus()); //this triggers the layout of the menus themselves
	wireless->setReceiver(sout, Controller::console_callback);
	spawned.push_back(controller);
	
	sndman->playFile("roar.wav");

#ifdef TGT_HAS_MOUTH
	//This will close the mouth so it doesn't look stupid or get in the
	//way of head motions (ERS-210 or ERS-7 only)
	SharedObject<TinyMotionSequenceMC> closeMouth;
	closeMouth->advanceTime(3000); //take 3 seconds to close the mouth
	closeMouth->setOutputCmd(MouthOffset,outputRanges[MouthOffset][MaxRange]);
	closeMouth->advanceTime(500); //and hold it for another .5 seconds
	closeMouth->setOutputCmd(MouthOffset,outputRanges[MouthOffset][MaxRange]);
	MotionManager::MC_ID id=motman->addPrunableMotion(closeMouth,MotionManager::kEmergencyPriority+1);
	erouter->addTimer(this,1,3250,false);
	spawnedMC.push_back(id);
#endif
	
	//! we want to know if one of our MCs autoprunes (or is otherwise removed)
	for(unsigned int i=0; i<spawnedMC.size(); i++)
		erouter->addListener(this,EventBase::motmanEGID,spawnedMC[i],EventBase::deactivateETID);
	
#ifdef PLATFORM_APERIOS
	sout->printf("Remember, telnet to port 10001 for text entry (port 59000 is read only)\n");
#else
	sout->printf("All console output will be redirected to this terminal.\n");
	sout->printf("Alternatively, telnet to port 10001 to isolate data from sout as a separate stream.\n");
	sout->printf("If a prompt does not appear below, hit 'enter' or 'return' to refresh it.\n");
#endif
	
	//if you didn't want to start off paused, you should throw an
	//un-estop event.  This will make it clear to any background
	//behaviors (namely WorldStateVelDaemon) that we're not in estop
	//erouter->postEvent(new EventBase(EventBase::estopEGID,MotionManager::invalid_MC_ID,EventBase::deactivateETID,0));
}

void StartupBehavior::DoStop() {
	erouter->removeListener(this);
	for(vector<BehaviorBase*>::iterator it=spawned.begin(); it!=spawned.end(); it++) {
		cout << "StartupBehavior stopping spawned: " << (*it)->getName() << endl;
		(*it)->DoStop();
	}
	spawned.clear();
	for(vector<MotionManager::MC_ID>::iterator it=spawnedMC.begin(); it!=spawnedMC.end(); it++) {
		cout << "StartupBehavior stopping spawned MC: " << (*it) << endl;
		motman->removeMotion(*it);
	}
	spawnedMC.clear();
	BehaviorBase::DoStop();
}

/*!Uses a few timer events at the beginning to fade in the PID values, and closes the mouth too*/
void StartupBehavior::processEvent(const EventBase& event) {
	if(event.getGeneratorID()==EventBase::timerEGID) {
		if(event.getSourceID()==0) {
			//this will do the work of fading in the PID values.  It helps the joints
			//to power up without twitching
			static unsigned int start_time=-1U;
			const unsigned int tot_time=2000; 
			if(start_time==-1U) { //first time
				start_time=get_time();
			} else {
				float power=(get_time()-start_time)/(float)tot_time;
				if(power>1)
					power=1;
				MMAccessor<PIDMC>(pid_id)->setAllPowerLevel(power*power);
			}
			if((get_time()-start_time)>=tot_time) {
				erouter->removeTimer(this,0);
				//when this goes through, it will trigger the event handler below to follow through cleanup of the id
				motman->removeMotion(pid_id);
			}
#ifdef TGT_HAS_MOUTH
		} else if(event.getSourceID()==1) {
			//we're done closing the mouth... set the mouth to closed in the estop too
			//otherwise it might twitch a little when the MotionSequence expires and the estop takes over
			// (little == +/-.1 radians, aka estop's threshold for resetting an "out of place" joint) (details, details)
			MMAccessor<EmergencyStopMC> estop(stop_id);
			float weight=estop->getOutputCmd(MouthOffset).weight;
			estop->setOutputCmd(MouthOffset,OutputCmd(outputRanges[MouthOffset][MaxRange],weight));
#endif
		} else {
			cerr << "Warning: StartupBehavior got unknown timer event: " << event.getName() << endl;
		}
		
	} else if(event.getGeneratorID()==EventBase::motmanEGID) {
		vector<MotionManager::MC_ID>::iterator it=find(spawnedMC.begin(),spawnedMC.end(),event.getSourceID());
		ASSERTRET(it!=spawnedMC.end(),"Warning: StartupBehavior received deactive motion event for MC it didn't spawn");
		erouter->removeListener(this,EventBase::motmanEGID,event.getSourceID(),EventBase::deactivateETID);
		if(event.getSourceID()==stop_id) {
			//probably a bad idea to remove this -- spit out a warning to let user know it happened
			cerr << "Warning: Emergency stop MC has been removed" << endl;
			stop_id=MotionManager::invalid_MC_ID;
		}
		if(event.getSourceID()==pid_id) //is supposed to be removed when done, not something to get excited about
			pid_id=MotionManager::invalid_MC_ID;
		spawnedMC.erase(it);
	}
}

ControlBase*
StartupBehavior::SetupMenus() {
	cout << "CONTROLLER-INIT..." << flush;
	ControlBase* root=new ControlBase("Root Control");
	setup.push(root);

	// additional controls will be submenus of root:
	{
		SetupModeSwitch();
		SetupBackgroundBehaviors();
		SetupTekkotsuMon();
		SetupStatusReports();
		SetupFileAccess();
		SetupVision();
		addItem(new ControlBase("Shutdown?","Confirms decision to reboot or shut down"));
		startSubMenu();
		{ 
			addItem(new ShutdownControl());
#ifdef PLATFORM_APERIOS
			addItem(new RebootControl());
#endif
		}
		endSubMenu();
		addItem(new HelpControl(root,2));
	}
	
	if(endSubMenu()!=root)
		cout << "\n*** WARNING *** menu setup stack corrupted" << endl;
	cout << "DONE" << endl;
	return root;
}

void
StartupBehavior::startSubMenu() {
	setup.push(setup.top()->getSlots().back());
}

void
StartupBehavior::addItem(ControlBase * control) {
	setup.top()->pushSlot(control);
}

ControlBase*
StartupBehavior::endSubMenu() {
	ControlBase * tmp=setup.top();
	setup.pop();
	return tmp;
}
