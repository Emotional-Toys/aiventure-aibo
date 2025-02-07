//-*-c++-*-
#ifndef INCLUDED_StepTest_h_
#define INCLUDED_StepTest_h_

#include "Behaviors/BehaviorBase.h"
#include "Motion/WalkMC.h"
#include "Motion/MotionManager.h"
#include "Motion/MMAccessor.h"
#include "Events/TextMsgEvent.h"
#include "Wireless/Socket.h"

//! tests taking a certain number of steps
class StepTest : public BehaviorBase {
public:
	//! constructor
	StepTest() : BehaviorBase("StepTest"), walk_id(MotionManager::invalid_MC_ID) {}

	virtual void DoStart() {
		BehaviorBase::DoStart(); // do this first
		walk_id=motman->addPersistentMotion(SharedObject<WalkMC>());
		erouter->addListener(this,EventBase::textmsgEGID);
	}

	virtual void DoStop() {
		motman->removeMotion(walk_id);
		walk_id=MotionManager::invalid_MC_ID;
		erouter->removeListener(this);
		BehaviorBase::DoStop(); // do this last
	}

	virtual void processEvent(const EventBase& e) {
		const TextMsgEvent * txt = dynamic_cast<const TextMsgEvent*>(&e);
		if(txt==NULL)
			serr->printf("WARNING: Illegal event in StepTest '%s'\n",e.getName().c_str());
		else {
			std::string s=txt->getText();
			if(s.find("StepTest ")==0) {
				float dx,dy,da;
				int n;
				int numread=sscanf(s.c_str(),"StepTest %f %f %f %d",&dx,&dy,&da,&n);
				//sout->printf("numread=%d: %f %f %f %d\n",numread,dx,dy,da,n);
				if(numread==1) {
					MMAccessor<WalkMC> walk_acc(walk_id);
					walk_acc->setStepThreshold(dx);
				} else if(numread<4) {
					serr->printf("ERROR: StepTest needs four values 'dx dy da steps'\n");
					return;
				} else {
					MMAccessor<WalkMC> walk_acc(walk_id);
					walk_acc->setTargetDisplacement(dx*n,dy*n,da*n,n);
					sout->printf("resulting velocity: %f %f %f\n",walk_acc->getTargetVelocity().x,walk_acc->getTargetVelocity().y,walk_acc->getTargetVelocity().z);
				}
			}
		}
	}

	static std::string getClassDescription() { return "tests taking a certain number of steps"; }
	virtual std::string getDescription() const { return getClassDescription(); }
	
protected:
	MotionManager::MC_ID walk_id; //!< the WalkMC to use for the stepping
};

/*! @file
 * @brief Defines StepTest, which tests taking a certain number of steps
 * @author ejt (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.3 $
 * $State: Exp $
 * $Date: 2005/08/07 04:11:03 $
 */

#endif
