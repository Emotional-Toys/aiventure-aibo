//-*-c++-*-
#ifndef INCLUDED_PaceTargetsMachine_h_
#define INCLUDED_PaceTargetsMachine_h_

#include "Shared/RobotInfo.h"
#ifndef TGT_HAS_IR_DISTANCE
#error PaceTargetsMachine requires a distance rangefinder via IRDistOffset
#endif

#include "Behaviors/StateNode.h"
#include "Shared/ProjectInterface.h"

//! A StateMachine for walking back and forth between two (or more) pink balls
class PaceTargetsMachine : public StateNode {
public:
	//!constructor
	PaceTargetsMachine() : StateNode("PaceTargetsMachine","PaceTargetsMachine"), start(NULL) {}

	//!destructor, check if we need to call our teardown
	~PaceTargetsMachine() {
		if(issetup)
			teardown();
	}

	virtual void setup();
	virtual void DoStart();

	virtual void teardown();

protected:
	StateNode* start; //!< the node to be started first upon activation

private:
	PaceTargetsMachine(const PaceTargetsMachine&); //!< don't call
	PaceTargetsMachine operator=(const PaceTargetsMachine&); //!< don't call
};

/*! @file
 * @brief Describes PaceTargetsMachine, a StateMachine for walking back and forth between two (or more) pink balls
 * @author ejt (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.10 $
 * $State: Exp $
 * $Date: 2007/07/16 18:31:39 $
 */

#endif
