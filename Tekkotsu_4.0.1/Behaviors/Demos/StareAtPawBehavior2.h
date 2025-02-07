//-*-c++-*-
#ifndef INCLUDED_StareAtPawBehavior2_h_
#define INCLUDED_StareAtPawBehavior2_h_

#include "Behaviors/BehaviorBase.h"
#include "Motion/MotionManager.h"

//! Uses new-style ROBOOP kinematics to track the paw which last received a button press with the camera
class StareAtPawBehavior2 : public BehaviorBase {
public:
	//! constructor
	StareAtPawBehavior2()
		: BehaviorBase("StareAtPawBehavior2"), lastLeg(LFrLegOrder), pointID(MotionManager::invalid_MC_ID)
	{ }

	virtual void DoStart();
	virtual void DoStop();
	virtual void processEvent(const EventBase& e);

	static std::string getClassDescription() { return "Uses kinematics to track the paw which last received a button press with the camera"; }
	virtual std::string getDescription() const { return getClassDescription(); }
	
protected:
	LegOrder_t lastLeg; //!< last leg to have it's button pressed, i.e. the one we are looking at
	MotionManager::MC_ID pointID; //!< the HeadPointerMC we are using to do the looking
};

/*! @file
 * @brief Describes StareAtPawBehavior2, which uses new-style ROBOOP kinematics to track the paw which last received a button press with the camera
 * @author ejt (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.21 $
 * $State: Exp $
 * $Date: 2005/07/07 20:25:56 $
 */

#endif
