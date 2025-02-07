//-*-c++-*-
#ifndef INCLUDED_DeadReckoningBehavior_h_
#define INCLUDED_DeadReckoningBehavior_h_

#include "Behaviors/BehaviorBase.h"
#include "Events/EventRouter.h"
#include "Events/LocomotionEvent.h"
#include "Motion/HolonomicMotionModel.h"
#include "Shared/WorldState.h"

//! Subscribes to LocomotionEvents and attempts to track robot movement over time using a fairly generic HolonomicMotionModel
/*! Can be used as a ParticleFilter::MotionModel, or as a component of other behaviors.
 *
 *  If you want a regular report on position, the behavior will output current position on timer events */
template<class ParticleT>
class DeadReckoningBehavior : public BehaviorBase, public HolonomicMotionModel<ParticleT> {
public:
	//! constructor
	explicit DeadReckoningBehavior(const std::string& name="DeadReckoningBehavior") : BehaviorBase(name), HolonomicMotionModel<ParticleT>() {}
	
	//! constructor
	DeadReckoningBehavior(float xVariance, float yVariance, float aVariance)
	: BehaviorBase("DeadReckoningBehavior"), HolonomicMotionModel<ParticleT>(xVariance,yVariance,aVariance) {}
	
	virtual void DoStart() {
		BehaviorBase::DoStart(); // do this first (required)
		HolonomicMotionModel<ParticleT>::setVelocity(state->vel_x, state->vel_y, state->vel_a);
		erouter->addListener(this, EventBase::locomotionEGID );
		//erouter->addTimer(this, 0, 500);
	}
	
	virtual void processEvent(const EventBase& event) {
		if (event.getGeneratorID() == EventBase::locomotionEGID) {
			const LocomotionEvent &locoevt = dynamic_cast<const LocomotionEvent&>(event);
			HolonomicMotionModel<ParticleT>::setVelocity(locoevt.x,locoevt.y,locoevt.a,locoevt.getTimeStamp());
		} else if (event.getGeneratorID() == EventBase::timerEGID) {
			float tempx;
			float tempy;
			float tempa;
			HolonomicMotionModel<ParticleT>::getPosition(tempx, tempy, tempa);
			std::cout << "DEADPOS " << tempx << " " << tempy << " " << tempa << std::endl;
		}
	}
	
	static std::string getClassDescription() { return "Subscribes to LocomotionEvents and attempts to track robot movement over time"; }
	virtual std::string getDescription() const { return getClassDescription(); }
	
protected:
	//! constructor, accepts separate classname and instance name parameters from subclass
	DeadReckoningBehavior(const std::string& classname, const std::string& instancename) : BehaviorBase(classname, instancename), HolonomicMotionModel<ParticleT>() {}
};

/*! @file
* @brief Defines DeadReckoningBehavior, which subscribes to LocomotionEvents and attempts to track robot movement over time using a fairly generic HolonomicMotionModel
* @author Ethan Tira-Thompson (ejt) (Creator)
*
* $Author: ejt $
* $Name: tekkotsu-4_0-branch $
* $Revision: 1.2 $
* $State: Exp $
* $Date: 2007/11/11 23:57:20 $
*/

#endif
