#include "Shared/RobotInfo.h"
#ifdef TGT_HAS_BUTTONS

#include "FollowHeadBehavior.h"
#include "Events/EventRouter.h"
#include "Shared/debuget.h"
#include "Shared/WorldState.h"
#include "Motion/MMAccessor.h"
#include "Motion/HeadPointerMC.h"
#include "Motion/WalkMC.h"
#include "Motion/PIDMC.h"

using namespace std;

FollowHeadBehavior::FollowHeadBehavior() :
	BehaviorBase("FollowHeadBehavior"),
	head_release(EventBase::buttonEGID,ChinButOffset,EventBase::activateETID,0),
	head_lock(EventBase::buttonEGID,ChinButOffset,EventBase::deactivateETID,0),
	clock(EventBase::timerEGID,0,EventBase::statusETID,250),
	walker_id(MotionManager::invalid_MC_ID)
{}

FollowHeadBehavior::~FollowHeadBehavior() {
	if(isActive())
		DoStop();
}

void FollowHeadBehavior::DoStart() {
	BehaviorBase::DoStart();
	//set up the shared motions
	walker_id=motman->addPersistentMotion(SharedObject<WalkMC>());
	//register for events and timers
	erouter->addListener(this,head_release);
	erouter->addListener(this,head_lock);
	//prime the head pointer and walk so they're going the right way
	processEvent(head_lock);
	processEvent(clock);
}

void FollowHeadBehavior::DoStop() {
	//remove timers and listeners
	erouter->removeListener(this);
	//remove motion commands, set them to invalid
	motman->removeMotion(walker_id);
	walker_id=MotionManager::invalid_MC_ID;
	BehaviorBase::DoStop();
}

void FollowHeadBehavior::processEvent(const EventBase& e) {
	if(e==clock) {
		//x,y,a are percentage of maximum speed in that direction
		float x=-(state->outputs[HeadOffset+TiltOffset]-outputRanges[HeadOffset+TiltOffset][MinRange])/(outputRanges[HeadOffset+TiltOffset][MaxRange]-outputRanges[HeadOffset+TiltOffset][MinRange])*2+1;
		float y=(state->outputs[HeadOffset+RollOffset]-outputRanges[HeadOffset+RollOffset][MinRange])/(outputRanges[HeadOffset+RollOffset][MaxRange]-outputRanges[HeadOffset+RollOffset][MinRange])*2-1;
		float a=(state->outputs[HeadOffset+PanOffset]-outputRanges[HeadOffset+PanOffset][MinRange])/(outputRanges[HeadOffset+PanOffset][MaxRange]-outputRanges[HeadOffset+PanOffset][MinRange])*2-1;
		MMAccessor<WalkMC> walk(walker_id);
		walk.mc()->setTargetVelocity(x*WalkMC::MAX_DX,y*WalkMC::MAX_DY,a*WalkMC::MAX_DA);

	} else if(e==head_release) {
		cout << "release" << endl;
		motman->addPrunableMotion(SharedObject<PIDMC>(HeadOffset,HeadOffset+NumHeadJoints,0));
		erouter->addTimer(this,clock);

	} else if(e==head_lock) {
		cout << "lock" << endl;
		motman->addPrunableMotion(SharedObject<PIDMC>(HeadOffset,HeadOffset+NumHeadJoints,1));
		for(unsigned int i=HeadOffset; i<HeadOffset+NumHeadJoints; i++)
			motman->setOutput(NULL,i,state->outputs[i]); //doing this prevents the head from jerking back when you released it to where it was before you pressed the button
		cout << state->outputs[HeadOffset+TiltOffset]/M_PI*180 << ' ' << state->outputs[HeadOffset+PanOffset]/M_PI*180 << ' ' << state->outputs[HeadOffset+RollOffset]/M_PI*180 << endl;
		erouter->removeTimer(this,clock.getSourceID());

	} else {
		ASSERT(false,"unprocessed event " << e.getName() << endl);
	}
}

#endif

/*! @file
 * @brief Implements FollowHeadBehavior, walks where the head is pointing
 * @author ejt (Creator)
 *
 * $Author: ejt $
 * $Name $
 * $Revision: 1.13 $
 * $State: Exp $
 * $Date: 2007/11/12 18:00:38 $
 */

