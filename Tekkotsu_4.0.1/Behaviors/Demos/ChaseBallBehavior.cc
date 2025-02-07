#include "ChaseBallBehavior.h"
#include "Events/EventRouter.h"
#include "Events/VisionObjectEvent.h"
#include "Shared/WorldState.h"
#include "Motion/HeadPointerMC.h"
#include "Motion/WalkMC.h"
#include "Shared/WMclass.h"
#include "Shared/ProjectInterface.h"
#include "Shared/ERS7Info.h"
#include "Motion/MMAccessor.h"

void ChaseBallBehavior::DoStart() {
	BehaviorBase::DoStart();
	headpointer_id = motman->addPersistentMotion(SharedObject<HeadPointerMC>());
	walker_id = motman->addPersistentMotion(SharedObject<WalkMC>());
	erouter->addListener(this,EventBase::visObjEGID,ProjectInterface::visPinkBallSID);
}

void ChaseBallBehavior::DoStop() {
	erouter->removeListener(this);
	motman->removeMotion(headpointer_id);
	motman->removeMotion(walker_id);
	BehaviorBase::DoStop();
}

//this could be cleaned up event-wise (only use a timer when out of view)
void ChaseBallBehavior::processEvent(const EventBase& event) {
	WMreg(chase_ball_behavior);
	WMvari_(float, horiz, 0, chase_ball_behavior);
	WMvari_(float, vert, 0, chase_ball_behavior);
	
	if(event.getGeneratorID()==EventBase::visObjEGID && event.getTypeID()==EventBase::statusETID) {
		horiz=static_cast<const VisionObjectEvent*>(&event)->getCenterX();
		vert=static_cast<const VisionObjectEvent*>(&event)->getCenterY();
		std::cout << get_time() << ' ' << horiz << ' ' << vert << std::endl;
	}
		
	// For portability, look to see if the host hardware has a head pan & tilt joints,
	// using ERS-7's namespace to look up the canonical "name" of the pan and tilt joints.
	// Note if you were coding for a specific robot, could just do "tiltIdx = HeadOffset + TiltOffset"
	// directly and not bother with this...
	const char * const * ERS7HeadNames = &ERS7Info::outputNames[ERS7Info::HeadOffset];
	const unsigned int panIdx = capabilities.findOutputOffset(ERS7HeadNames[ERS7Info::PanOffset]);
	const unsigned int tiltIdx = capabilities.findOutputOffset(ERS7HeadNames[ERS7Info::TiltOffset]);
	if(panIdx==-1U || tiltIdx==-1U)
		return; // guess we're headless, leave now...
	
#if defined(TGT_QBOTPLUS) || defined(TGT_QWERK)
	// these *should* be millimeters per second, but we haven't calibrated Qwerk yet
	// so these are some unitless speed measurement for now :(
	const float FAST = 32, SLOW = 20;
#else 
	// these are millimeters per second
	const float FAST = 160, SLOW = 100;
#endif
	
	// We use the "Walk" motion command, but if it's wheel based, the WalkMC will still handle it anyway
	WalkMC * walker = (WalkMC*)motman->checkoutMotion(walker_id);
	if(state->outputs[panIdx]<-.05 || state->outputs[panIdx]>.05)
		walker->setTargetVelocity(SLOW,0,state->outputs[panIdx]);
	else
		walker->setTargetVelocity(FAST,0,0); // target straight ahead, full speed!
	motman->checkinMotion(walker_id);
	
	// proportional control to home in on the ball
	// http://en.wikipedia.org/wiki/Proportional_control
	double tilt=state->outputs[tiltIdx]-vert*CameraFOV/7.5;
	double pan=state->outputs[panIdx]-horiz*CameraFOV/6;
	
	// We'll limit tilt and pan to their range of motion, but this isn't actually necessary, just demonstration:
	tilt = std::max(tilt, outputRanges[tiltIdx][MinRange]);
	tilt = std::min(tilt, outputRanges[tiltIdx][MaxRange]);
	pan = std::max(pan, outputRanges[panIdx][MinRange]);
	pan = std::min(pan, outputRanges[panIdx][MaxRange]);
	
	//! for comparison, this is a one-line version of the checkout/checkin used for the WalkMC
	MMAccessor<HeadPointerMC>(headpointer_id)->setJoints(tilt,pan,0);
}

/*! @file
 * @brief Implements ChaseBallBehavior, which runs around after whatever the dog sees
 * @author ejt (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.14 $
 * $State: Exp $
 * $Date: 2007/11/16 15:19:40 $
 */


