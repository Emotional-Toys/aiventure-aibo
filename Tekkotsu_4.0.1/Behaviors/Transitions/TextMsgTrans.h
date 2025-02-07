//-*-c++-*-
#ifndef INCLUDED_TextMsgTrans_h_
#define INCLUDED_TextMsgTrans_h_

#include "Behaviors/Transition.h"
#include "Events/TextMsgEvent.h"

//! Fires when a matching string is received
class TextMsgTrans : public Transition {

	// **************************** //
	// ******* CONSTRUCTORS ******* //
	// **************************** //
public:
	//! default constructor, use type name as instance name
	TextMsgTrans(StateNode* destination, const std::string& trigger )
		: Transition("TextMsgTrans",destination), msg(trigger)
	{}

protected:
  //! constructor for subclasses (which would need to provide a different class name)
  TextMsgTrans(const std::string &class_name, const std::string &instance_name, StateNode* destination, const std::string& trigger )
    : Transition(class_name,instance_name,destination), msg(trigger)
	{}
	
	
	// **************************** //
	// ********* METHODS ********** //
	// **************************** //
public:
	//! Just like a behavior, called when it's time to start doing your thing
	virtual void DoStart() {
		Transition::DoStart(); // do this first (required)
		erouter->addListener(this, EventBase::textmsgEGID );
	}

	virtual void processEvent(const EventBase& event) {
		const TextMsgEvent *e = dynamic_cast<const TextMsgEvent*>(&event);
		if(e==NULL)
			return;
		if(e->getText()==msg)
			fire();
	}

	//! Just like a behavior, called when it's time to stop doing your thing
	virtual void DoStop() {
		erouter->removeListener(this); //generally a good idea, unsubscribe all
		Transition::DoStop(); // do this last (required)
	}

	static std::string getClassDescription() { return "Fires when a matching string is received"; }
	virtual std::string getDescription() const { return getClassDescription(); }


	// **************************** //
	// ********* MEMBERS ********** //
	// **************************** //
protected:
	std::string msg; //!< the trigger to match messages against


	// **************************** //
	// ********** OTHER *********** //
	// **************************** //
private:
	// Providing declarations for these functions will avoid a compiler warning if
	// you have any class members which are pointers.  However, as it is, an error
	// will result if you inadvertantly cause a call to either (which is probably
	// a good thing, unless you really intended to copy/assign a behavior, in
	// which case simply provide implementations for the functions)
	TextMsgTrans(const TextMsgTrans&); //!< don't call (copy constructor)
	TextMsgTrans& operator=(const TextMsgTrans&); //!< don't call (assignment operator)
};

/*! @file
 * @brief Defines TextMsgTrans, which fires when a matching string is received
 * @author ejt (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.3 $
 * $State: Exp $
 * $Date: 2005/08/07 04:11:03 $
 */

#endif
