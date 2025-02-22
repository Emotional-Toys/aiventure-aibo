//-*-c++-*-
#ifndef INCLUDED_AutoGetupBehavior_h_
#define INCLUDED_AutoGetupBehavior_h_

#include "Behaviors/BehaviorBase.h"
#include "Shared/WorldState.h"
#include "Events/EventRouter.h"
#include "IPC/SharedObject.h"
#include "Motion/MotionManager.h"
#include "Motion/MotionSequenceMC.h"
#include "Shared/Config.h"
#include "Sound/SoundManager.h"

//! a little background behavior to keep the robot on its feet
class AutoGetupBehavior : public BehaviorBase {
public:
	//! constructor
	AutoGetupBehavior() : BehaviorBase("AutoGetupBehavior"), back(0), side(0), gamma(.9), sensitivity(.85*.85), waiting(false) {}
	//! destructor
	virtual ~AutoGetupBehavior() {}

	//! Listens for the SensorSrcID::UpdatedSID
	virtual void DoStart() {
		BehaviorBase::DoStart();
		erouter->addListener(this,EventBase::sensorEGID,SensorSrcID::UpdatedSID);
	}
	//! Stops listening for events
	virtual void DoStop() {
		erouter->removeListener(this);
		BehaviorBase::DoStop();
	}
	//! Run appropriate motion script if the robot falls over
	virtual void processEvent(const EventBase &event) {
		if(event.getGeneratorID()==EventBase::motmanEGID) {
			//previous attempt at getting up has completed
			std::cout << "Getup complete" << std::endl;
			erouter->removeListener(this,EventBase::motmanEGID);
			waiting=false;
			return;
		}
		back=back*gamma+(1-gamma)*state->sensors[BAccelOffset];
		side=side*gamma+(1-gamma)*state->sensors[LAccelOffset];
		if(!waiting && back*back+side*side>sensitivity*WorldState::g*WorldState::g) {
			//fallen down
			std::cout << "I've fallen!" << std::endl;
			sndman->playFile("yipper.wav");
			std::string gu;
			//config->motion.makePath will return a path relative to config->motion.root (from config file read at boot)
			if(fabs(back)<fabs(side))
				gu=config->motion.makePath("gu_side.mot");
			else if(back<0)
				gu=config->motion.makePath("gu_back.mot");
			else
				gu=config->motion.makePath("gu_front.mot");
			SharedObject<MediumMotionSequenceMC> getup(gu.c_str());
			MotionManager::MC_ID id=motman->addPrunableMotion(getup,MotionManager::kHighPriority);
			erouter->addListener(this,EventBase::motmanEGID,id,EventBase::deactivateETID);
			waiting=true;
		}
	}
	static std::string getClassDescription() { return "Monitors gravity's influence on the accelerometers - if it seems the robot has fallen over, it runs appropriate getup script"; }
	virtual std::string getDescription() const { return getClassDescription(); }

protected:
	float back;          //!< exponential average of backwards accel
	float side;          //!< exponential average of sideways accel
	float gamma;         //!< default 0.9, gamma parameter for exponential average of above
	float sensitivity;   //!< default 0.85*0.85, squared threshold to consider having fallen over, use values 0-1
	bool  waiting;       //!< true while we're waiting to hear from completion of MotionSequence, won't try again until this is cleared
};

/*! @file
 * @brief Defines AutoGetupBehavior, a little background behavior to keep the robot on its feet
 * @author ejt (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.4 $
 * $State: Exp $
 * $Date: 2007/11/12 18:00:40 $
 */

#endif
