//-*-c++-*-
#ifndef INCLUDED_HeadLevelBehavior_h_
#define INCLUDED_HeadLevelBehavior_h_

#include "Behaviors/BehaviorBase.h"
#include "Motion/MotionManager.h"
#include "Motion/MMAccessor.h"
#include "Motion/OldHeadPointerMC.h"
#include "Motion/PIDMC.h"
#include <math.h>
#include "Shared/ERS210Info.h"
#include "Shared/ERS220Info.h"
#include "Shared/ERS7Info.h"

//! Tests the head leveling code of OldHeadPointerMC
class HeadLevelBehavior : public BehaviorBase {
 public:
	//! constructor
	HeadLevelBehavior()
		: BehaviorBase("HeadLevelBehavior"),
			head_release(EventBase::buttonEGID,0,EventBase::activateETID,0),
			head_lock(EventBase::buttonEGID,0,EventBase::deactivateETID,0),
			head(),
			head_id(MotionManager::invalid_MC_ID), pid_id(MotionManager::invalid_MC_ID)
	{
		head_release.setSourceID(HeadButOffset);
		head_lock.setSourceID(HeadButOffset);
		head.getRegion()->AddReference();
	}
	//! destructor
	virtual ~HeadLevelBehavior() {
		head.getRegion()->RemoveReference();
	}

	virtual void DoStart() {
		BehaviorBase::DoStart();
		head->setJoints(state->outputs[HeadOffset+TiltOffset],state->outputs[HeadOffset+PanOffset],state->outputs[HeadOffset+RollOffset]);
		head->setMode(OldHeadPointerMC::GravityRelative,true);
		head->noMaxSpeed(); // this is probably pretty safe - the whole point is to keep the head still
		head_id=motman->addPersistentMotion(head);
		erouter->addListener(this,head_lock);
		erouter->addListener(this,head_release);
	}

	virtual void DoStop() {
		erouter->removeListener(this);
		motman->removeMotion(head_id);
		head->setMode(OldHeadPointerMC::BodyRelative,false);
		BehaviorBase::DoStop();
	}

	virtual void processEvent(const EventBase &event) {
		if(event==head_lock) {
			for(unsigned int i=HeadOffset; i<HeadOffset+NumHeadJoints; i++)
				head->setJointValueFromMode((TPROffset_t)(i-HeadOffset),state->outputs[i],OldHeadPointerMC::BodyRelative);
			motman->removeMotion(pid_id);
			pid_id=MotionManager::invalid_MC_ID;
		} else if(event==head_release) {
			pid_id=motman->addPersistentMotion(SharedObject<PIDMC>(HeadOffset,HeadOffset+NumHeadJoints,0),MotionManager::kHighPriority);
			//motman->addPersistentMotion(SharedObject<PIDMC>(HeadOffset,HeadOffset+NumHeadJoints,0));
		} else
			ASSERTRET(false,"received unasked for event "<<event.getName());
	}
	static std::string getClassDescription() { return "Uses the internal accelerometers to attempt to keep the head level."; }
	virtual std::string getDescription() const { return getClassDescription(); }

 protected:
	EventBase head_release; //!< event mask for releasing head (chin button down)
	EventBase head_lock;    //!< event mask for locking head (chin button up)
	const SharedObject<OldHeadPointerMC> head; //!< might as well just hang on to the whole memory region and reuse it, we can peek for most of our stuff
	MotionManager::MC_ID head_id; //!< MCID of headpointer
	MotionManager::MC_ID pid_id; //!< MCID of pid controller
};

/*! @file
 * @brief Defines HeadLevelBehavior, which a prototypes head leveler
 * @author ejt (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.11 $
 * $State: Exp $
 * $Date: 2007/06/26 04:27:43 $
 */

#endif 
