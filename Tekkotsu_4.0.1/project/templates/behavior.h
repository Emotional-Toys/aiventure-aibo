//-*-c++-*-
#ifndef INCLUDED_CLASSNAME_h_
#define INCLUDED_CLASSNAME_h_

// This is an empty Behavior template file.
// Replace YOURNAMEHERE, CLASSNAME, and DESCRIPTION as appropriate, and go to town!


#include "Behaviors/BehaviorBase.h"
#include "Events/EventRouter.h"

//! DESCRIPTION
class CLASSNAME : public BehaviorBase {
public:
	//! constructor
	CLASSNAME() : BehaviorBase("CLASSNAME") {}

	virtual void DoStart() {
		BehaviorBase::DoStart(); // do this first (required)

		// <your startup code here>
		// e.g. erouter->addListener(this, ... );
		// (Event IDs are found in Events/EventBase.h)
	}

	virtual void DoStop() {
		// <your shutdown code here>

		BehaviorBase::DoStop(); // do this last (required)
	}

	virtual void processEvent(const EventBase& event) {
		// <your event processing code here>
		// you can delete this function if you don't use any events...
		// (in which case, you may want to call DoStop() at the end of DoStart()
	}

	static std::string getClassDescription() { return "DESCRIPTION"; }
	virtual std::string getDescription() const { return getClassDescription(); }
	

protected:
	// <class members go here>

	
private:
	// Providing declarations for these functions will avoid a compiler warning if
	// you have any class members which are pointers.  However, as it is, an error
	// will result if you inadvertantly cause a call to either (which is probably
	// a good thing, unless you really intended to copy/assign a behavior, in
	// which case simply provide implementations for the functions)
	CLASSNAME(const CLASSNAME&); //!< don't call (copy constructor)
	CLASSNAME& operator=(const CLASSNAME&); //!< don't call (assignment operator)
};

/*! @file
 * @brief Defines CLASSNAME, which DESCRIPTION
 * @author YOURNAMEHERE (Creator)
 *
 * $Author$
 * $Name$
 * $Revision$
 * $State$
 * $Date$
 */

#endif
