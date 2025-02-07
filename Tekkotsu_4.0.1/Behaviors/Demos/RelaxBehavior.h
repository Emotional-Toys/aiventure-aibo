//-*-c++-*-
#ifndef INCLUDED_RelaxBehavior_h_
#define INCLUDED_RelaxBehavior_h_

#include <iostream>
#include "Behaviors/BehaviorBase.h"
#include "Motion/PIDMC.h"
#include "IPC/SharedObject.h"
#include "Motion/MotionManager.h"

//! A behavior that sets all the pids to zero for the tail and legs servos.
/*! This should hopefully make the robot quieter and consume less power. */
class RelaxBehavior : public BehaviorBase {
public:
	//! contstructor
	RelaxBehavior() : BehaviorBase("RelaxBehavior"), pidMCID(MotionManager::invalid_MC_ID) {}
	
	virtual void DoStart() {
		BehaviorBase::DoStart();
		SharedObject<PIDMC> pidMC(0);
		// if you want to keep some joints from being turned off, e.g.:
		//pidMC->setJointPowerLevel(HeadOffset+TiltOffset,1);
		// (or you could change the contructor too...)
		// http://www.tekkotsu.org/dox/classPIDMC.html

		// You could also change the priority level so that anytime
		// a joint is not in use it goes limp (try kBackgroundPriority)
		pidMCID=motman->addPersistentMotion(pidMC,MotionManager::kHighPriority);
		// the 'false' is to keep it from auto-pruning
	}
	virtual void DoStop() {
		motman->removeMotion(pidMCID);
		//this "one-shot" version of doing things will restore the PIDs
		motman->addPrunableMotion(SharedObject<PIDMC>(1));
		BehaviorBase::DoStop();
	}
	static std::string getClassDescription() { return "Sets PID parameters for all applicable joints to 0, allowing the joints to move freely, reducing noise and power consumption"; }
	virtual std::string getDescription() const { return getClassDescription(); }
protected:
	MotionManager::MC_ID pidMCID; //!< the id for the pid motion command
};

#endif // INCLUDED_RelaxBehavior_h_

/*! @file
 * @brief Describes RelaxBehavior, which sets all the pids to zero for the tail and legs servos.
 * @author Erik Berglund <tekkotsu.dev.spam.345@blog.no> (Creator)
 * @author ejt (Modifications)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.8 $
 * $State: Exp $
 * $Date: 2007/11/11 23:57:18 $
 */
