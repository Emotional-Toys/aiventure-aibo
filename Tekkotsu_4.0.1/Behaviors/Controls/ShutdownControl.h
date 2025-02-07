//-*-c++-*-
#ifndef INCLUDED_ShutdownControl_h_
#define INCLUDED_ShutdownControl_h_

#include "NullControl.h"

//! when activated, this will cause the aibo to shut down
class ShutdownControl : public NullControl {
public:

	ShutdownControl() : NullControl("Shutdown","Turns the Aibo off") {} //!< constructor
	ShutdownControl(const std::string& n) : NullControl(n,"Turns the Aibo off") {} //!< constructor
	ShutdownControl(const std::string& n, const std::string& d) : NullControl(n,d) {} //!< constructor

	virtual ControlBase * activate(MotionManager::MC_ID , Socket * ) { return doSelect(); } //!< calls doSelect()

	//! shuts down
	virtual ControlBase * doSelect();
};

/*! @file
 * @brief Describes ShutdownControl, which initiates the shutdown sequence
 * @author ejt (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.3 $
 * $State: Rel $
 * $Date: 2003/09/25 15:26:11 $
 */

#endif
