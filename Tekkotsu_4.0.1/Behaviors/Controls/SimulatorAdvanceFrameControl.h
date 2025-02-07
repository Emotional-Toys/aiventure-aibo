//-*-c++-*-
#ifndef INCLUDED_SimulatorAdvanceFrameControl_h
#define INCLUDED_SimulatorAdvanceFrameControl_h

#include "Behaviors/Controls/NullControl.h"
#ifdef PLATFORM_APERIOS
#  warning SimulatorAdvanceFrameControl is available on Aperios!
#else
#  include "Shared/ProjectInterface.h"
#endif

//! Requests the next camera frame and sensor data, for use when running in simulation
/*! Note that this won't increment the simulator time if triggered while paused... */
class SimulatorAdvanceFrameControl : public NullControl {

public:
	//! default constructor
	SimulatorAdvanceFrameControl()
		: NullControl("SimulatorAdvanceFrameControl","Requests the next camera frame and sensor data, for use when running in simulation")
	{}
	//! constructor which allows a custom name
	SimulatorAdvanceFrameControl(const std::string& n)
		: NullControl(n,"Requests the next camera frame and sensor data, for use when running in simulation")
	{}

	//can't do anything if we're running on aperios
#ifndef PLATFORM_APERIOS

	virtual ControlBase * activate(MotionManager::MC_ID disp_id, Socket * gui) {
		ProjectInterface::sendCommand("advance");
		return NullControl::activate(disp_id,gui);
	}

	virtual std::string getName() const {
		if(canManuallyAdvance())
			return NullControl::getName();
		return "[Auto-Advancing]";
	}

	virtual std::string getDescription() const {
		if(canManuallyAdvance())
			return NullControl::getDescription();
		return "Cannot manually advance when in realtime mode, or when AdvanceOnAccess is enabled";
	}
	
protected:
	//! ideally, this should return true only when the simulator is paused or the data source is frozen...
	bool canManuallyAdvance() const { return true; }
	
#endif

};

/*! @file
 * @brief Defines SimulatorAdvanceFrameControl, which requests the next camera frame and sensor data, for use when running in simulation
 * @author ejt (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.4 $
 * $State: Exp $
 * $Date: 2007/11/10 22:58:05 $
 */
#endif
