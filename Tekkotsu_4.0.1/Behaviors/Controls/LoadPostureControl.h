//-*-c++-*-
#ifndef INCLUDED_LoadPostureControl_h_
#define INCLUDED_LoadPostureControl_h_

#include "FileBrowserControl.h"
#include "Motion/PostureMC.h"
#include "Motion/MMAccessor.h"
#include "Motion/EmergencyStopMC.h"
#include "Events/EventRouter.h"
#ifdef TGT_HAS_LEDS
#  include "Motion/LedMC.h"
#endif
#include "Sound/SoundManager.h"
#include <string>

//! Upon activation, loads a position from a file name read from cin (stored in ms/data/motion...)
/*! Should switch this to use a MotionSequence so it can move more leisurely and not "snap" to position */
class LoadPostureControl : public FileBrowserControl, public EventListener {
 public:
	//! Constructor
	LoadPostureControl(const std::string& n, MotionManager::MC_ID estop_id)
		: FileBrowserControl(n,"Loads a posture from user-selected file",config->motion.root),
			estopid(estop_id), ledid(MotionManager::invalid_MC_ID), file()
	{
		setFilter("*.pos");
	}
	
	//! destructor
	virtual ~LoadPostureControl() {
		erouter->removeListener(this);
		motman->removeMotion(ledid);
		ledid=MotionManager::invalid_MC_ID;
	}

	//! only called when e-stop has been turned off and we're waiting to load a file
	virtual void processEvent(const EventBase& /*event*/) {
		erouter->removeListener(this);
		runFile();
		motman->removeMotion(ledid);
		ledid=MotionManager::invalid_MC_ID;
	}

protected:
	//! loads the motion sequence and runs it
	void runFile() {
		//TimeET timer;
		SharedObject< PostureMC > s(file);
		//cout << "Load Time: " << timer.Age() << endl;
		s->defaultMaxSpeed(.25); //no need to rush this, run at quarter of max speed
		motman->addPrunableMotion(s);
	}

	//!does the actual loading of the MotionSequence
	virtual ControlBase* selectedFile(const std::string& f) {
		file=f;
		if(!MMAccessor<EmergencyStopMC>(estopid)->getStopped()) {
			runFile();
		} else {
			//we have to wait for the estop to be turned off
			sndman->playFile("donkey.wav");
#ifdef TGT_HAS_LEDS
			SharedObject<LedMC> led;
			led->cset(FaceLEDMask,0);
			unsigned int botl = capabilities.findButtonOffset(ERS210Info::buttonNames[ERS210Info::BotLLEDOffset]);
			unsigned int botr = capabilities.findButtonOffset(ERS210Info::buttonNames[ERS210Info::BotRLEDOffset]);
			if(botl==-1U || botr==-1U) {
				botl=LEDOffset;
				botr=NumLEDs>1 ? botl+1 : botl;
			}
			led->cycle(1<<(botl-LEDOffset),1000,3,0,0);
			led->cycle(1<<(botr-LEDOffset),1000,3,0,500);
			ledid=motman->addPersistentMotion(led);
#endif
			erouter->addListener(this,EventBase::estopEGID,estopid,EventBase::deactivateETID);
		}
		return this;
	}

	MotionManager::MC_ID estopid; //!< MC_ID of the emergency stop (so we can tell if it's active and wait until it's turned off)
	MotionManager::MC_ID ledid; //!< MC_ID of the led we use to signal there's a MotionSequence lined up
	std::string file;             //!< last posture file loaded
};

/*! @file
 * @brief Defines LoadPostureControl, which presents a file selection submenu, and then loads the specified posture.
 * @author ejt (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.21 $
 * $State: Exp $
 * $Date: 2007/08/05 16:16:04 $
 */

#endif
