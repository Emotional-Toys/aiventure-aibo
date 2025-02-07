//-*-c++-*-
#ifndef INCLUDED_ViewWMVarsBehavior_h_
#define INCLUDED_ViewWMVarsBehavior_h_

#include "Behaviors/BehaviorBase.h"
#include "Behaviors/Controller.h"
#include "Shared/Config.h"
#include <stdio.h>

//! simply launches the Watchable Memory GUI, which should connect to the already-running WMMonitorBehavior
class ViewWMVarsBehavior : public BehaviorBase {
public:
	//! constructor
	ViewWMVarsBehavior() : BehaviorBase("ViewWMVarsBehavior") {}

	virtual void DoStart() {
		BehaviorBase::DoStart();
		// Open the WalkGUI on the desktop
		Controller::loadGUI("org.tekkotsu.mon.WatchableMemory","WatchableMemory",config->main.wmmonitor_port);
	}

	virtual void DoStop() {
		// Close the GUI
		Controller::closeGUI("WatchableMemory");
		BehaviorBase::DoStop();
	}

	static std::string getClassDescription() {
		char tmp[20];
		sprintf(tmp,"%d",*config->main.wmmonitor_port);
		return std::string("Brings up the WatchableMemory GUI on port ")+tmp+std::string(" (connects to WMMonitorBehavior, this just launches the GUI)");
	}
	virtual std::string getDescription() const { return getClassDescription(); }
};

/*! @file
 * @brief Defines ViewWMVarsBehavior, simply launches the Watchable Memory GUI, which should connect to the already-running WMMonitorBehavior
 * @author ejt (Creator)
 * 
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.4 $
 * $State: Exp $
 * $Date: 2007/01/30 22:56:19 $
 */

#endif 
