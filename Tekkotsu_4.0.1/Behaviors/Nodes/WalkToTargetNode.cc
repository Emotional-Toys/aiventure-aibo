#include "WalkToTargetNode.h"
#include "Motion/HeadPointerMC.h"
#include "Motion/WalkMC.h"
#include "Motion/MMAccessor.h"
#include "Events/VisionObjectEvent.h"
#include "Shared/WorldState.h"
#include "Behaviors/Transitions/TimeOutTrans.h"
#include "Shared/RobotInfo.h"
#include "Shared/ERS7Info.h"
#ifdef TGT_HAS_IR_DISTANCE
#  include "Behaviors/Transitions/VisualTargetCloseTrans.h"
#endif

void WalkToTargetNode::DoStart() {
	StateNode::DoStart();

	headpointer_id = motman->addPersistentMotion(SharedObject<HeadPointerMC>());
	walker_id = motman->addPersistentMotion(SharedObject<WalkMC>());

	erouter->addListener(this,EventBase::visObjEGID,tracking);
}

void WalkToTargetNode::DoStop() {
	erouter->removeListener(this);

	motman->removeMotion(headpointer_id);
	headpointer_id=MotionManager::invalid_MC_ID;
	motman->removeMotion(walker_id);
	walker_id=MotionManager::invalid_MC_ID;

	StateNode::DoStop();
}

//this could be cleaned up event-wise (only use a timer when out of view)
void WalkToTargetNode::processEvent(const EventBase& event) {
	static float horiz=0,vert=0;
	const VisionObjectEvent *ve = dynamic_cast<const VisionObjectEvent*>(&event);
	if(ve!=NULL && event.getTypeID()==EventBase::statusETID) {
		horiz=ve->getCenterX();
		vert=ve->getCenterY();
	} else
		return;

	// for portability, look to see if the host hardware has a head pan & tilt joints
	const unsigned int panIdx = capabilities.findOutputOffset(ERS7Info::outputNames[ERS7Info::HeadOffset+ERS7Info::PanOffset]);
	const unsigned int tiltIdx = capabilities.findOutputOffset(ERS7Info::outputNames[ERS7Info::HeadOffset+ERS7Info::TiltOffset]);
	if(panIdx==-1U || tiltIdx==-1U)
		return; // guess not...
	
	//cout << "Pos: " << horiz << ' ' << vert << endl;

	double tilt=state->outputs[tiltIdx]-vert*M_PI/6;
	double pan=state->outputs[panIdx]-horiz*M_PI/7.5;
	if(tilt>outputRanges[tiltIdx][MaxRange])
		tilt=outputRanges[tiltIdx][MaxRange];
	if(tilt<outputRanges[tiltIdx][MinRange]*3/4)
		tilt=outputRanges[tiltIdx][MinRange]*3/4;
	if(pan>outputRanges[panIdx][MaxRange]*2/3)
		pan=outputRanges[panIdx][MaxRange]*2/3;
	if(pan<outputRanges[panIdx][MinRange]*2/3)
		pan=outputRanges[panIdx][MinRange]*2/3;
	MMAccessor<HeadPointerMC>(headpointer_id)->setJoints(tilt,pan,0); // note no variable name, one-line scope
	
	{
		MMAccessor<WalkMC> walker(walker_id);
		if(pan<-.05 || pan>.05)
			walker->setTargetVelocity(100,0,pan);
		else
			walker->setTargetVelocity(160,0,0);
	}
}

Transition* WalkToTargetNode::newDefaultLostTrans(StateNode* dest) {
	return new TimeOutTrans(dest,1500,EventBase::visObjEGID,tracking);
}

Transition* WalkToTargetNode::newDefaultCloseTrans(StateNode* dest) {
#ifdef TGT_HAS_IR_DISTANCE
	return new VisualTargetCloseTrans(dest,tracking);
#else
	(void)dest;
	return NULL;
#endif
}


/*! @file
 * @brief Implements WalkToTargetNode, a state node for walking towards a visual target
 * @author ejt (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.4 $
 * $State: Exp $
 * $Date: 2007/08/28 18:05:19 $
 */
