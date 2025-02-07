#ifndef INCLUDED_MCValueEditControl_h
#define INCLUDED_MCValueEditControl_h

#include "ValueEditControl.h"

//!allows you to modify a value in memory, much like ValueEditControl, but will check out a MotionCommand first to maintain proper mutual exclusion.
template<class T>
class MCValueEditControl : public ValueEditControl<T> {
 public:
	//!constructor
	MCValueEditControl(const std::string& n, T* t, MotionManager::MC_ID id)
		: ValueEditControl<T>(n,t), mcid(id) {}
	//!destructor
	virtual ~MCValueEditControl() {}

	virtual ControlBase* doSelect() {
		motman->checkoutMotion(mcid);
		ControlBase * ans = ValueEditControl<T>::doSelect();
		motman->checkinMotion(mcid);
		return ans;
	}

 protected:
	MotionManager::MC_ID mcid; //!< the id of the MotionCommand which should be checked out
};

/*! @file
 * @brief Defines MCValueEditControl, which allows you to modify a value in memory, much like ValueEditControl, but will check out a MotionCommand first to maintain proper mutual exclusion.
 * @author ejt (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.3 $
 * $State: Rel $
 * $Date: 2003/09/25 15:26:11 $
 */

#endif
