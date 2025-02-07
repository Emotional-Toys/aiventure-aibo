//-*-c++-*-
#ifndef INCLUDED_StateNode_h_
#define INCLUDED_StateNode_h_

#include "Transition.h"
#include "Behaviors/BehaviorBase.h"
#include <vector>
#include <string>

//! Recursive data structure - both a state machine controller as well as a node within a state machine itself
/*!
 *  Override setup() to build your own Transition and StateNode network if you want
 *  this node to contain a state machine.
 *
 *  Override DoStart() / DoStop() as you would a normal BehaviorBase subclass to
 *  have this node add some functionality of its own.
 *  
 *  You can override setup to create a sub-network, as well as overriding DoStart and DoStop, in the same class.
 *  
 *  See also the <a href="http://www.cs.cmu.edu/~dst/Tekkotsu/Tutorial/state.shtml">tutorial page on State Machines</a>.
 *  
 *  There are two StateNode templates in <a href="http://cvs.tekkotsu.org/cgi/viewcvs.cgi/Tekkotsu/project/templates/"><i>project</i><tt>/templates/</tt></a>:
 *  - <a href="http://cvs.tekkotsu.org/cgi/viewcvs.cgi/Tekkotsu/project/templates/statenode.h?rev=HEAD&content-type=text/vnd.viewcvs-markup">statenode.h</a>
 *    is intended for leaf nodes, which directly implement the execution of a task.
 *  - <a href="http://cvs.tekkotsu.org/cgi/viewcvs.cgi/Tekkotsu/project/templates/statemachine.h?rev=HEAD&content-type=text/vnd.viewcvs-markup">statemachine.h</a>
 *    is intended for nodes which contain a network of transitions and subnodes, which together solve the task.
 */
class StateNode  : public BehaviorBase {
public:
	//!constructor, pass a name to use
	StateNode(const std::string& name) : BehaviorBase("StateNode",name), parent(NULL), transitions(), issetup(false), retain(true), startedTime(0), nodes() {}

	//!destructor, removes references to its outgoing transitions (be careful of incoming ones - they're still around!), and calls RemoveReference() on subnodes
	virtual ~StateNode();

	//!Adds the specified StateTransition to the transition table
	virtual Transition* addTransition(Transition* trans);

	//!Returns the std::vector of transitions so you can modify them yourself if need be
	std::vector<Transition*>& getTransitions() { return transitions; }

	//!Returns the const std::vector of transitions so you can read through them if need be
	const std::vector<Transition*>& getTransitions() const { return transitions; }

	//!Adds a StateNode to #nodes so it can be automatically dereferenced later, returns what it's passed (for convenience), calls AddReference() on @a node.  Also sets the node's parent to @c this if it is null.
	virtual StateNode* addNode(StateNode* node);

	//!Returns the std::vector of sub-nodes so you can modify them yourself if need be
	std::vector<StateNode*>& getNodes() { return nodes; }

	//!Returns the const std::vector of sub-nodes so you can read through them if need be
	const std::vector<StateNode*>& getNodes() const { return nodes; }

	//!Sets the retain flag - if not retained, will RemoveReference() subnodes upon DoStop() and recreate them on DoStart (by calling setup()) - may be too expensive to be worth saving memory...
	void setRetain(bool f) { retain=f; }

	//!Transitions should call this when you are entering the state, so it can enable its transitions
	virtual void DoStart();

	//!This is called by DoStart() when you should setup the network of subnodes (if any)
	virtual void setup() {issetup=true;}

	//!Transitions should call this when you are leaving the state, so it can disable its transitions
	virtual void DoStop();
	
	//!This is called by DoStop() when you should destruct subnodes
	/*!Default implementation will take care of the subnodes and their
	 * transitions, you only need to worry about any *other* memory
	 * which may have been allocated.  If none, you may not need
	 * implement this function at all. */
	virtual void teardown();

	//!returns #parent
	virtual StateNode* getParent() const { return parent; }

protected:
	//!constructor, pass the class name and instance name to use 
	StateNode(const std::string& classname, const std::string& name) : BehaviorBase(classname,name), parent(NULL), transitions(), issetup(false), retain(true), startedTime(0), nodes() {}

	//!will throw an activation event through stateMachineEGID, used when DoStart() is called
	virtual void postStartEvent();

	//!will throw a status event through stateMachineEGID to signal "completion" of the node
	/*! "completion" is defined by your subclass - will mean different things to different
	 *  nodes depending on the actions they are performing.  So call this yourself if there
	 *  is a natural ending point for your state.
	 * @param magnitude the value to use for EventBase::magnitude -- generally is 1 for status events, but since this is to signal completion, 0 (the default) may be more appropriate; if your node is doing something repetitive however, 1 (or the loop count) may be better */
	virtual void postCompletionEvent(float magnitude=0);

	//!will throw an deactivation event through stateMachineEGID, used when DoStop() is called
	/* @param duration the value to use for EventBase::duration -- nice but not generally used */
	virtual void postStopEvent();

	//Node Stuff:
	//! pointer to the machine that contains this node
	StateNode* parent;
	//! a vector of outgoing transitions
	std::vector<Transition*> transitions;
	
	//Machine Stuff:
	//! this is set to true if the network has been setup but not destroyed (i.e. don't need to call setupSubNodes again)
	bool issetup;
	//! this is set to true if the network should be retained between activations.  Otherwise it's dereferenced upon DoStop(). (or at least RemoveReference() is called on subnodes)
	bool retain;
	//! the timestamp of last call to DoStart()
	unsigned int startedTime;
	//! vector of StateNodes, just so they can be dereferenced again on DoStop() (unless retained) or ~StateNode()
	std::vector<StateNode*> nodes;

private:
	StateNode(const StateNode& node); //!< don't call this
	StateNode operator=(const StateNode& node); //!< don't call this
};

/*! @file
 * @brief Describes StateNode, which is both a state machine controller as well as a node within a state machine itself
 * @author ejt (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.23 $
 * $State: Exp $
 * $Date: 2007/08/14 18:23:27 $
 */

#endif
