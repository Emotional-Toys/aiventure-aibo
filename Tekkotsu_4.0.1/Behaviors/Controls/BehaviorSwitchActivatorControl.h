//-*-c++-*-
#ifndef INCLUDED_BehaviorSwitchActivatorControl_h
#define INCLUDED_BehaviorSwitchActivatorControl_h

#include "ControlBase.h"
#include "BehaviorSwitchControl.h"

//! Upon activation, will tell the specified BehaviorSwitchControl to start or stop the behavior
class BehaviorSwitchActivatorControl : public ControlBase {
public:
	//! lets you tell it what action to perform
	enum Mode_t {
		start, //!< Passed to constructor, indicates this control should start the behavior when activated
		stop,  //!< Passed to constructor, indicates this control should stop the behavior when activated
		toggle //!< Passed to constructor, indicates this control should toggle the behavior when activated
	};

	//!constructor
	BehaviorSwitchActivatorControl(const std::string& n, BehaviorSwitchControlBase* bscb, Mode_t m=toggle) : ControlBase(n), behswitch(bscb), mode(m) {}

	//!destructor
	virtual ~BehaviorSwitchActivatorControl() {}

	//! performs the action denoted by #mode
	virtual ControlBase * activate(MotionManager::MC_ID disp_id, Socket *) {
		switch(mode) {
		case start:
			behswitch->start();
			break;
		case stop:
			behswitch->stop();
			break;
		case toggle:
			behswitch->toggle();
			break;
		}
		if(disp_id!=MotionManager::invalid_MC_ID) {
			MMAccessor<LedMC> display(disp_id);
			display.mc()->flash(FaceLEDMask,100);
		}
		return NULL;
	}

	virtual std::string getName() const { return behswitch->getName(); }
	virtual std::string getDescription() const { return behswitch->getDescription(); }

protected:
	BehaviorSwitchControlBase* behswitch; //!< The behavior switch to activate/deactivate
	Mode_t mode; //!< the mode this control is in

private:
	BehaviorSwitchActivatorControl(const BehaviorSwitchActivatorControl&); //!< don't copy this class
	BehaviorSwitchActivatorControl operator=(const BehaviorSwitchActivatorControl&); //!< don't assign this class
};

/*! @file
 * @brief Defines BehaviorSwitchActivatorControl, which will tell the specified BehaviorSwitchControl to start or stop the behavior
 * @author ejt (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.4 $
 * $State: Exp $
 * $Date: 2006/09/16 06:28:06 $
 */

#endif
