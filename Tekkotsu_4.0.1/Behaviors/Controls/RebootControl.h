//-*-c++-*-
#ifndef INCLUDED_RebootControl_h_
#define INCLUDED_RebootControl_h_

#include "NullControl.h"

//! when activated, this will cause the aibo to reboot
class RebootControl : public NullControl {
public:

	RebootControl() : NullControl("Reboot","Reboots the Aibo") {} //!< constructor
	RebootControl(const std::string& n) : NullControl(n,"Reboots the Aibo") {} //!< constructor
	RebootControl(const std::string& n, const std::string& d) : NullControl(n,d) {} //!< constructor

	virtual ControlBase * activate(MotionManager::MC_ID , Socket * ) { return doSelect(); } //!< calls doSelect()

	//! reboots
	virtual ControlBase * doSelect();
};

/*! @file
 * @brief Defines RebootControl, which causes the aibo to reboot
 * @author ejt (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.2 $
 * $State: Rel $
 * $Date: 2003/09/25 15:26:11 $
 */

#endif
