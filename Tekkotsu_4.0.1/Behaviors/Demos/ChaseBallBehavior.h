//-*-c++-*-
#ifndef INCLUDED_ChaseBallBehavior_h_
#define INCLUDED_ChaseBallBehavior_h_

#include "Behaviors/BehaviorBase.h"
#include "Motion/MotionManager.h"

//! A simple behavior to chase after any objects seen by the vision system
class ChaseBallBehavior : public BehaviorBase {
public:
	//!constructor
	ChaseBallBehavior()
		: BehaviorBase("ChaseBallBehavior"), headpointer_id(MotionManager::invalid_MC_ID), walker_id(MotionManager::invalid_MC_ID)
	{}
	//!destructor
	virtual ~ChaseBallBehavior() {}

	//! adds a headpointer and a walker, and a listens for vision events
	virtual void DoStart();

	//! removes motion commands and stops listening
	virtual void DoStop();

	//! sets the head to point at the object and sets the body to move where the head points
	virtual void processEvent(const EventBase& event);
			
	static std::string getClassDescription() { return "Follows ball with head and walks whereever the head is pointing"; }
	virtual std::string getDescription() const { return getClassDescription(); }

protected:
	MotionManager::MC_ID headpointer_id; //!< a HeadPointerMC object
	MotionManager::MC_ID walker_id;      //!< a WalkMC object
};

/*! @file
 * @brief Describes ChaseBallBehavior, which runs around after whatever the dog sees
 * @author ejt (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.5 $
 * $State: Exp $
 * $Date: 2004/11/11 01:45:35 $
 */

#endif
