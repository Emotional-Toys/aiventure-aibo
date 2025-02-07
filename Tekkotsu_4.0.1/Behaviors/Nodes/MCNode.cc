#include "MCNode.h"
#include "Events/EventRouter.h"

const char MCNodeBase::defName[]="MCNode";
const char MCNodeBase::defDesc[]="A generic wrapper for any MotionCommand";

// These externs are declared in their respective header files, but
// defined here instead of corresponding .cc files to avoid file bloat
// (there would be nothing else in their .cc files)

//! name for HeadPointerNode to pass as template argument
extern const char defHeadPointerNodeName[]="HeadPointerNode";
//! description for HeadPointerNode to pass as template argument
extern const char defHeadPointerNodeDesc[]="Manages a HeadPointerMC to look in a given direction each time the node is activated";
//! name for TailWagNode to pass as template argument
extern const char defTailWagNodeName[]="TailWagNode";
//! description for TailWagNode to pass as template argument
extern const char defTailWagNodeDesc[]="Wags the tail for as long as the state is active";
//! name for PostureNode to pass as template argument
extern const char defPostureNodeName[]="PostureNode";
//! description for PostureNode to pass as template argument
extern const char defPostureNodeDesc[]="Moves the body to the specified posture";
//! name for WalkNode to pass as template argument
extern const char defWalkNodeName[]="WalkNode";
//! description for WalkNode to pass as template argument
extern const char defWalkNodeDesc[]="Manages a WalkMC node to walk in a direction each time the node is activated.";
//! name for WaypointWalkNode to pass as template argument
extern const char defWaypointWalkNodeName[]="WaypointWalkNode";
//! description for WaypointWalkNode to pass as template argument
extern const char defWaypointWalkNodeDesc[]="Manages a WaypointWalkMC to perform a waypoint walk each time the node is activated.";


void MCNodeBase::DoStart() {
	StateNode::DoStart(); // do this first (required)
	if(mc_id==MotionManager::invalid_MC_ID)
		mc_id = motman->addPersistentMotion(getPrivateMC());
	else
		motman->setPriority(mc_id,MotionManager::kStdPriority);
	erouter->addListener(this,EventBase::motmanEGID,mc_id,EventBase::statusETID);
}

void MCNodeBase::processEvent(const EventBase& /*e*/) {
	if(mcCompletes)
		postCompletionEvent();
}

void MCNodeBase::DoStop() {
	erouter->removeListener(this); //generally a good idea, unsubscribe all
	if(hasPrivateMC()) {
		motman->removeMotion(mc_id);
		mc_id=MotionManager::invalid_MC_ID;
	} else if(mc_id!=MotionManager::invalid_MC_ID) {
		motman->setPriority(mc_id,MotionManager::kIgnoredPriority);
	}
	StateNode::DoStop(); // do this last (required)
}

void MCNodeBase::setMC(MotionManager::MC_ID mcid) {
	if(hasPrivateMC()) {
		if(mc_id!=MotionManager::invalid_MC_ID)
			motman->removeMotion(mc_id);
		delete mc;
		mc=NULL;
	}
	mc_id=mcid;
}


/*! @file
 * @brief Implement's MCNode's default name and description strings (the class is templated, so everything else has to go in the header)
 * @author Ethan Tira-Thompson (ejt) (Creator)
 *
 * $Author: dst $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.6 $
 * $State: Exp $
 * $Date: 2007/03/13 08:31:26 $
 */
