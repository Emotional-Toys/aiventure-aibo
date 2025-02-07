//-*-c++-*-
#ifndef INCLUDED_CLASSNAME_h_
#define INCLUDED_CLASSNAME_h_

// This is an empty StateNode template file.
//
// StateNodes are recursive data structures, can be used as either a leaf node
// or a machine.  This template is the suggested form for a leaf node, which
// does the actual implementation of a task without help from other subnodes.
// Note the extreme similarity to a Behavior... the only significant difference
// for implementation is a few extra suggested constructors.
//
// You are of course welcome to combine the abilities of a leaf node (actual
// execution) and a state machine (which breaks the task down into subnodes),
// but statemachine.h might provide an easier starting point for such usage.
//
// Replace YOURNAMEHERE, CLASSNAME, and DESCRIPTION as appropriate, and go to town!


#include "Behaviors/StateNode.h"

//! DESCRIPTION
class CLASSNAME : public StateNode {

	// **************************** //
	// ******* CONSTRUCTORS ******* //
	// **************************** //
public:
	//! default constructor, use type name as instance name
	CLASSNAME()
		: StateNode("CLASSNAME","CLASSNAME")
	{}

	//! constructor, take an instance name
	CLASSNAME(const std::string& nm)
		: StateNode("CLASSNAME",nm)
	{}

protected:
	//! constructor for subclasses (which would need to provide a different class name)
	CLASSNAME(const std::string &class_name, const std::string &node_name)
		: StateNode(class_name,node_name)
	{}
	
	
	// **************************** //
	// ********* METHODS ********** //
	// **************************** //
public:
	//! Just like a behavior, called when it's time to start doing your thing
	virtual void DoStart() {
		StateNode::DoStart(); // do this first (required)
		// <your startup code here>
		// can also erouter->addListener(this, ... );
		// (Event IDs are found in Events/EventBase.h)

		// Don't call DoStop() yourself (as behaviors can do) -- let a transition do
		// it when deemed appropriate
	}

	virtual void processEvent(const EventBase& /*event*/) {
		// <your event processing code here>
		// you can delete this function if you don't use any events...
	}

	//! Just like a behavior, called when it's time to stop doing your thing
	virtual void DoStop() {
		// <your shutdown code here>

		erouter->removeListener(this); //generally a good idea, unsubscribe all
		StateNode::DoStop(); // do this last (required)
	}

	static std::string getClassDescription() { return "DESCRIPTION"; }
	virtual std::string getDescription() const { return getClassDescription(); }


	// **************************** //
	// ********* MEMBERS ********** //
	// **************************** //
protected:
	// <class members go here>


	// **************************** //
	// ********** OTHER *********** //
	// **************************** //
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
