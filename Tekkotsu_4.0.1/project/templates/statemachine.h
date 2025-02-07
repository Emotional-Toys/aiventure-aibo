//-*-c++-*-
#ifndef INCLUDED_CLASSNAME_h_
#define INCLUDED_CLASSNAME_h_

// This is an empty StateNode template file.
//
// StateNodes are recursive data structures, can be used as either a leaf node
// or a machine.  This template is the suggested form for a state machine, which
// breaks a task down into one or more subnodes.
//
// You are of course welcome to combine the abilities of a leaf node (actual
// execution) and a state machine (which breaks the task down into subnodes) --
// simply add processEvent() and DoStop(), as seen in statenode.h
//
// Replace YOURNAMEHERE, CLASSNAME, and DESCRIPTION as appropriate, and go to town!


#include "Behaviors/StateNode.h"

//! DESCRIPTION
class CLASSNAME : public StateNode {

	// ****************************
	// ******* CONSTRUCTORS *******
	// ****************************
public:
	//! default constructor, use type name as instance name
	CLASSNAME()
		: StateNode("CLASSNAME","CLASSNAME"), start(NULL)
	{}

	//! constructor, take an instance name
	CLASSNAME(const std::string& nm)
		: StateNode("CLASSNAME",nm), start(NULL)
	{}

	//! destructor, check call to teardown -- only actually necessary if you override teardown()
	~CLASSNAME() {
		if(issetup) 
			teardown();
	}

protected:
	//! constructor for subclasses (which would need to provide a different class name)
	CLASSNAME(const std::string &class_name, const std::string &node_name)
		: StateNode(class_name,node_name), start(NULL)
	{}
	
	
	// ****************************
	// ********* METHODS **********
	// ****************************
public:
	//! This function should wire together any subnodes which you may desire
	virtual void setup() {
		StateNode::setup(); // call anytime(required)
		// <your setup code here>
		start=/*...*/;
	}

	//! You may not need this function if the only memory allocated in
	//! setup() was subnodes and transitions
	virtual void teardown() {
		// <your teardown code here>
		StateNode::teardown(); // may delete subnodes (required)
	}

	//! Just like a behavior, called when it's time to start doing your thing
	virtual void DoStart() {
		StateNode::DoStart(); // do this first (required)
		start->DoStart();
		// probably don't need to do anything else...
	}

	static std::string getClassDescription() { return "DESCRIPTION"; }
	virtual std::string getDescription() const { return getClassDescription(); }


	// ****************************
	// ********* MEMBERS **********
	// ****************************
protected:
	StateNode * start; //!< the subnode to begin within on DoStart()
	// <class members go here>


	// ****************************
	// ********** OTHER ***********
	// ****************************
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
