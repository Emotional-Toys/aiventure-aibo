//-*-c++-*-
#ifndef INCLUDED_BatteryMonitorBehavior_h_
#define INCLUDED_BatteryMonitorBehavior_h_

#include "Behaviors/BehaviorBase.h"
#include "Shared/debuget.h"
#include "Shared/WorldState.h"
#include "Events/EventRouter.h"
#include "IPC/SharedObject.h"
#include "Motion/MotionManager.h"
#include "Motion/PostureMC.h"
#include "Motion/LedMC.h"
#include "Shared/ERS210Info.h"
#include "Shared/ERS220Info.h"
#include "Shared/ERS7Info.h"
#include "Motion/MMAccessor.h"

//! A background behavior which will monitor the power level and flip the ears when appropriate on a 210, or blink the headlight if a 220
/*! Think of this as a simple example class.  For exercise, try using a MotionSequenceMC instead
 *  of switching the ears back manually using a PostureMC */
class BatteryMonitorBehavior : public BehaviorBase {
public:
	static const unsigned int max_t=10000; //!< max time between ear flips when at "high power" mark
	static const unsigned int high_power_p=20; //!< percent of 100 which is point at which to begin warning
	static const unsigned int no_power_p=14; //!< percent of 100 at which power will fail (approximate!)

	//! constructor
	BatteryMonitorBehavior() : BehaviorBase("BatteryMonitorBehavior"), pose(NULL), pose_id(MotionManager::invalid_MC_ID), led_id(MotionManager::invalid_MC_ID) {}
	//! destructor
	virtual ~BatteryMonitorBehavior() {}

	//! Listens for the PowerSrcID::LowPowerWarnSID
	virtual void DoStart() {
		BehaviorBase::DoStart();
		erouter->addListener(this,EventBase::powerEGID,PowerSrcID::LowPowerWarnSID);
		erouter->addListener(this,EventBase::powerEGID,PowerSrcID::ExternalPowerSID);
		erouter->addListener(this,EventBase::powerEGID,PowerSrcID::BatteryConnectSID);
		erouter->addListener(this,EventBase::powerEGID,PowerSrcID::UpdatedSID);
		//if the low power warning is *already* on, better forge an event and send it to myself
		if(shouldWarn())
			processEvent(EventBase(EventBase::powerEGID,PowerSrcID::UpdatedSID,EventBase::statusETID));
	}
	//! Stops listening for events
	virtual void DoStop() {
		if(pose!=NULL)
			stopWarning();
		erouter->removeListener(this);
		BehaviorBase::DoStop();
	}
	//! Adds a BatteryMonitorMC to motman if power goes low
	virtual void processEvent(const EventBase &event) {
		if(event.getGeneratorID()==EventBase::powerEGID) {
			//just check for low power status
			bool shouldwarn=shouldWarn();
			if(pose!=NULL && !shouldwarn)
				stopWarning();
			else if(pose==NULL && shouldwarn)
				startWarning();
		} else {
			ASSERTRET(event.getGeneratorID()==EventBase::timerEGID,"Unrequested event "<<event.getName());
			switch(event.getSourceID()) {
			case 1: { // toggle the ears (signals low battery), show battery level on LEDs
				ASSERTRET(pose!=NULL,"Extra timer 1");
				setFlipper(true);
				unsigned int flipdelay=calcFlipDelay();
				// if we're just constantly flipping the ears, a slight change is needed so the battery
				// level isn't obscuring the LED settings
				if(flipdelay<=NumFrames*FrameTime) {
					static bool on=false;
					on=!on;
					if(on) {
						motman->setPriority(led_id,MotionManager::kEmergencyPriority+1);
						MMAccessor<LedMC> led(led_id);
						led->displayPercent(state->sensors[PowerRemainOffset],LedEngine::major,LedEngine::major);
					} else
						motman->setPriority(led_id,MotionManager::kIgnoredPriority);
					erouter->addTimer(this,1,128+flipdelay,false);
				} else {
					motman->setPriority(led_id,MotionManager::kEmergencyPriority+1);
					MMAccessor<LedMC> led(led_id);
					led->displayPercent(state->sensors[PowerRemainOffset],LedEngine::major,LedEngine::major);
					erouter->addTimer(this,2,128,false);
				}
			} break;
			case 2: { // release ear until next flap, hide LEDs display
				ASSERTRET(pose!=NULL,"Extra timer 1");
				setFlipper(false);
				motman->setPriority(led_id,MotionManager::kIgnoredPriority);
				erouter->addTimer(this,1,calcFlipDelay(),false);
			} break;
			default:
				ASSERTRET(false,"Unrequested timer " << event.getName());
				break;
			}
		}
	}
	static std::string getClassDescription() { return "Reports the current battery status, and starts flicks the ears to warn when it gets too low"; }
	virtual std::string getDescription() const { return getClassDescription(); }

	//! returns true if the warning should be active (power remaining less than high_power_p, no external power, but also checks that a power update has been received)
	static bool shouldWarn() { return state!=NULL && state->powerFlags[PowerSrcID::BatteryConnectSID] && (state->sensors[PowerRemainOffset]*100<=high_power_p || state->powerFlags[PowerSrcID::LowPowerWarnSID]) && !state->powerFlags[PowerSrcID::ExternalPowerSID]; }

protected:
	//! adds a pose and a timer to get the ears flipping
	void startWarning() {
		serr->printf("LOW BATTERY\n");
		pose_id=motman->addPersistentMotion(SharedObject<PostureMC>(),MotionManager::kEmergencyPriority+1);
		pose=(PostureMC*)motman->peekMotion(pose_id);
		SharedObject<LedMC> led;
		led->displayPercent(state->sensors[PowerRemainOffset],LedEngine::major,LedEngine::major);
		led_id=motman->addPersistentMotion(led,MotionManager::kEmergencyPriority+1);
		setFlipper(true);
		erouter->addTimer(this,2,128,false);
	}
	//! removes pose, in case battery magically charges
	void stopWarning() {
		serr->printf("BATTERY GOOD\n");
		motman->removeMotion(pose_id);
		motman->removeMotion(led_id);
		led_id=pose_id=MotionManager::invalid_MC_ID;
		pose=NULL;
		erouter->removeTimer(this,1);
		erouter->removeTimer(this,2);
	}
	//! makes the ears flip more rapidly as power declines.  Flips back and forth once every 15 seconds at 15%, down to flipping constantly at 5%.
	unsigned int calcFlipDelay() {
		const float high_power=high_power_p/100.0;
		const float no_power=no_power_p/100.0;
		float cur_power=state->sensors[PowerRemainOffset];
		if(cur_power<no_power)
			return 0;
		return (unsigned int)(max_t*(cur_power-no_power)/(high_power-no_power));
	}
	//!sets the ears on a 210 & 7 or the headlight on a 220 - true toggles current, false clears
	void setFlipper(bool set) {
		if(RobotName == ERS210Info::TargetName) {
			int ear=capabilities.getOutputOffset(ERS210Info::outputNames[ERS210Info::EarOffset]);
			for(unsigned int i=ear; i<ear+ERS210Info::NumEarJoints; i++)
				pose->setOutputCmd(i,set?!state->outputs[i]:OutputCmd());
		} else if(RobotName == ERS220Info::TargetName) {
			int light=capabilities.getOutputOffset(ERS220Info::outputNames[ERS220Info::RetractableHeadLEDOffset]);
			pose->setOutputCmd(light,set?(state->outputs[light]>.5?0:1):OutputCmd());
		} else if(RobotName == ERS7Info::TargetName) {
			int ear=capabilities.getOutputOffset(ERS7Info::outputNames[ERS7Info::EarOffset]);
			for(unsigned int i=ear; i<ear+ERS7Info::NumEarJoints; i++)
				pose->setOutputCmd(i,set?!state->outputs[i]:OutputCmd());
		}
	}
	PostureMC* pose; //!< if we are currently warning of low battery, holds a pose, NULL otherwise
	MotionManager::MC_ID pose_id; //!< id of pose if we are currently warning, MotionManager::invalid_MC_ID otherwise
	MotionManager::MC_ID led_id; //!< id of LedMC if we are currently warning, MotionManager::invalid_MC_ID otherwise

private:
	BatteryMonitorBehavior(const BatteryMonitorBehavior&); //!< don't copy behaviors
	BatteryMonitorBehavior operator=(const BatteryMonitorBehavior&); //!< don't assign behaviors
};

/*! @file
 * @brief Defines BatteryMonitorBehavior, a background behavior to trigger BatteryMonitorMC to warn when the power is low
 * @author ejt (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.4 $
 * $State: Exp $
 * $Date: 2007/06/28 04:36:20 $
 */

#endif
