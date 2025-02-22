//-*-c++-*-
#ifndef INCLUDED_WorldStateVelDaemon_h_
#define INCLUDED_WorldStateVelDaemon_h_

#include "Behaviors/BehaviorBase.h"
#include "Events/EventRouter.h"
#include "Events/LocomotionEvent.h"
#include "Shared/WorldState.h"
#include "Events/EventTrapper.h"

//! Listens for LocomotionEvents and updates the velocity fields of WorldState
/*! If we get multiple ways of locomoting, this would be a good place
 *  to manage them to determine the actual final velocity.
 *
 *  Right now it'll correctly handle one (or more i suppose) e-stops
 *  with a single other locomotor.  But if there's two active
 *  locomotors, I dunno how to handle that.
 */
class WorldStateVelDaemon : public BehaviorBase, public EventTrapper {
public:
	//! constructor
	WorldStateVelDaemon() : BehaviorBase("WorldStateVelDaemon"), estopTime(1), old_x(0), old_y(0), old_a(0) {}

	virtual void DoStart() {
		BehaviorBase::DoStart(); // do this first
		erouter->addTrapper(this,EventBase::locomotionEGID);
		erouter->addListener(this,EventBase::estopEGID);
	}

	virtual void DoStop() {
		erouter->removeListener(this);
		erouter->removeTrapper(this);
		BehaviorBase::DoStop(); // do this last
	}

	//! traps locomotion events - will filter them out if currently in EStop
	virtual bool trapEvent(const EventBase& e) {
		const LocomotionEvent& le=dynamic_cast<const LocomotionEvent&>(e);
		old_x=le.x;
		old_y=le.y;
		old_a=le.a;
		if(!estopTime) {
			state->vel_x=le.x;
			state->vel_y=le.y;
			state->vel_a=le.a;
			state->vel_time=le.getTimeStamp();
			return false;
		}
		return true;
	}

	virtual void processEvent(const EventBase& e) {
		if(e.getTypeID()==EventBase::deactivateETID) {
			if(estopTime) {
				estopTime=0;
				LocomotionEvent le(EventBase::locomotionEGID,e.getSourceID(),EventBase::statusETID,e.getTimeStamp()-state->vel_time);
				le.setXYA(old_x,old_y,old_a);
				erouter->postEvent(le);
			}
		} else {
			if(!estopTime) {
				float older_x=old_x;
				float older_y=old_y;
				float older_a=old_a;
				erouter->postEvent(LocomotionEvent(EventBase::locomotionEGID,e.getSourceID(),EventBase::statusETID,e.getTimeStamp()-state->vel_time));
				estopTime=e.getTimeStamp();
				old_x=older_x;
				old_y=older_y;
				old_a=older_a;
			}
		}
	}

	static std::string getClassDescription() { return "Keeps the WorldState's velocity fields up to date"; }
	virtual std::string getDescription() const { return getClassDescription(); }
	
protected:
	unsigned int estopTime; //!< time estop activation was received
	float old_x; //!< current velocity of underlying locomotor
	float old_y; //!< current velocity of underlying locomotor
	float old_a; //!< current velocity of underlying locomotor
};

/*! @file
 * @brief Defines WorldStateVelDaemon, which listens for LocomotionEvents and updates the velocity fields of WorldState
 * @author ejt (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.1 $
 * $State: Exp $
 * $Date: 2006/09/14 19:14:04 $
 */

#endif
