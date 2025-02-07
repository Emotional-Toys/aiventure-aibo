//-*-c++-*-
#ifndef INCLUDED_BehaviorActivatorControl_h
#define INCLUDED_BehaviorActivatorControl_h

#include "NullControl.h"

//! Upon activation, will start, stop, or toggle a behavior
class BehaviorActivatorControl : public NullControl {
public:
	//! lets you tell it what action to perform
	enum Mode_t {
		start, //!< Passed to constructor, indicates this control should start the behavior when activated
		stop,  //!< Passed to constructor, indicates this control should stop the behavior when activated
		toggle //!< Passed to constructor, indicates this control should toggle the behavior when activated
	};

	//@{
	//!constructors
	BehaviorActivatorControl(BehaviorBase* behave, Mode_t m=toggle) : NullControl(m==toggle?"Toggle":(m==start?"Start":"Stop"),m==toggle?"Toggles the behavior's activation":(m==start?"Starts the behavior":"Stops the behavior")), target(behave), mode(m) {init();}
	BehaviorActivatorControl(const std::string& n, BehaviorBase* behave, Mode_t m=toggle) : NullControl(n,m==toggle?"Toggles the behavior's activation":(m==start?"Starts the behavior":"Stops the behavior")), target(behave), mode(m) {init();}
	BehaviorActivatorControl(const std::string& n, const std::string& d, BehaviorBase* behave, Mode_t m=toggle) : NullControl(n,d), target(behave), mode(m) {init();}
	//@}

	//!destructor
	virtual ~BehaviorActivatorControl() {target->RemoveReference();}

	//! performs the action denoted by #mode
	virtual ControlBase * activate(MotionManager::MC_ID disp_id, Socket * gui) {
		switch(mode) {
		case start:
			target->DoStart();
			break;
		case stop:
			target->DoStop();
			break;
		case toggle:
			if(target->isActive())
				target->DoStop();
			else
				target->DoStart();
			break;
		}
		/*if(disp_id!=MotionManager::invalid_MC_ID) {
			MMAccessor<LedMC> display(disp_id);
			display.mc()->flash(FaceLEDMask,100);
			}*/
		return NullControl::activate(disp_id,gui);
	}

protected:
	//! adds to target's reference counter
	void init() {
		target->AddReference();
	}

	BehaviorBase* target; //!< The behavior to activate/deactivate
	Mode_t mode;       //!< the mode this control is in

private:
	BehaviorActivatorControl(const BehaviorActivatorControl&); //!< don't copy this class
	BehaviorActivatorControl operator=(const BehaviorActivatorControl&); //!< don't assign this class
};

/*! @file
 * @brief Defines BehaviorActivatorControl, which can either start, stop, or toggle a behavior when activated
 * @author ejt (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.9 $
 * $State: Exp $
 * $Date: 2006/09/16 06:28:06 $
 */

#endif
