//-*-c++-*-
#ifndef INCLUDED_SimpleChaseBallBehavior_h_
#define INCLUDED_SimpleChaseBallBehavior_h_

#include "Behaviors/BehaviorBase.h"
#include "Motion/MotionManager.h"
#include "Motion/MMAccessor.h"

#include "Events/EventRouter.h"
#include "Events/VisionObjectEvent.h"
#include "Shared/WorldState.h"
#include "Motion/WalkMC.h"
#include "Shared/ERS7Info.h"

//! A simple behavior to chase after any objects seen by the vision system
/*! Similar to ChaseBallBehavior, but this one doesn't try to move the head, so
 *  it's a little more... simple.  However, it does make sure to take into account
 *  which direction the head is pointing when it sees the object. */
class SimpleChaseBallBehavior : public BehaviorBase {
public:
	//!constructor
	SimpleChaseBallBehavior()
		: BehaviorBase("SimpleChaseBallBehavior"), walker_id(MotionManager::invalid_MC_ID)
	{}
	//!destructor
	virtual ~SimpleChaseBallBehavior() {}

	//! adds a headpointer and a walker, and a listens for vision events
	virtual void DoStart() {
		BehaviorBase::DoStart();
		walker_id = motman->addPersistentMotion(SharedObject<WalkMC>());
		erouter->addListener(this,EventBase::visObjEGID);
	}

	//! removes motion commands and stops listening
	virtual void DoStop() {
		erouter->removeListener(this);
		motman->removeMotion(walker_id);
		BehaviorBase::DoStop();
	}

	//! sets the head to point at the object and sets the body to move where the head points
	virtual void processEvent(const EventBase& event) {
		if(event.getGeneratorID()==EventBase::visObjEGID && event.getTypeID()==EventBase::statusETID) {
			// see if the camera is on a pan joint, we're using the ERS-7's pan joint as a prototype of what to look for...
			const unsigned int panIdx = capabilities.findOutputOffset(ERS7Info::outputNames[ERS7Info::HeadOffset+ERS7Info::PanOffset]);
			float panAngle = (panIdx!=-1U) ? state->outputs[panIdx] : 0; // get its angle, or assume 0 if not found
			
			//x and y are the direction to walk; positive x is forward and positive y is left
			//so these calculations walk the direction the head is pointing (at 120 mm/sec)
			float x=120.0f*cos(panAngle);
			float y=120.0f*sin(panAngle);

			//z is the amount to turn in radians; conveniently enough, we can use the
			//x parameter from the vision event as the speed to turn -- you could
			//scale this up or down to make it be more or less sensitive
			float z=-static_cast<const VisionObjectEvent*>(&event)->getCenterX();

			//now pass these values to the walk motion command:
			MMAccessor<WalkMC>(walker_id)->setTargetVelocity(x,y,z);
		}
	}
			
protected:
	MotionManager::MC_ID walker_id;      //!< a WalkMC object
};

/*! @file
 * @brief Describes SimpleChaseBallBehavior, which runs around after whatever the dog sees
 * @author ejt (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.13 $
 * $State: Exp $
 * $Date: 2007/11/15 05:19:22 $
 */

#endif
