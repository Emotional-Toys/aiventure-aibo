//-*-c++-*-
#ifndef INCLUDED_DriveMeBehavior_h_
#define INCLUDED_DriveMeBehavior_h_

#include "Behaviors/BehaviorBase.h"
#include "Motion/MotionManager.h"
#include "Motion/MotionSequenceMC.h"
#include "IPC/SharedObject.h"

//! A very simple behavior that asks the user for WalkMC walking parameters and a walk duration.
/*! The AIBO walks accordingly and then stands up, then asks again. And so on and so on.\n
 *  Input is from cin, not the tekkotsu console (sout) */
class DriveMeBehavior : public BehaviorBase {
public:
	DriveMeBehavior();              //!< constructor
	virtual ~DriveMeBehavior() {}   //!< destructor

	virtual void DoStart();
	virtual void DoStop();

	virtual void processEvent(const EventBase& event);
			
	static std::string getClassDescription() { return "Prompts for walk parameters and duration on system console (blocking read), and then executes, repeat until deactivation"; }
	virtual std::string getDescription() const { return getClassDescription(); }

protected:
	MotionManager::MC_ID walker_id; //!< walks
	MotionManager::MC_ID stand_id;  //!< stands up first
	SharedObject<SmallMotionSequenceMC> stand; //!< for standing

	double last_dx; //!< the last dx received
	double last_dy; //!< the last dy received
	double last_da; //!< the last da received
	unsigned int last_time; //!< timestamp of last parameter set
};

/*! @file
 * @brief Describes DriveMeBehavior, a very simple behavior that asks the user for WalkMC walking parameters and a walk duration.
 * @author tss (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.8 $
 * $State: Exp $
 * $Date: 2005/06/01 05:47:45 $
 */

#endif
