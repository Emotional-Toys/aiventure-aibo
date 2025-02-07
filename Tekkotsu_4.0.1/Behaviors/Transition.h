//-*-c++-*-
#ifndef INCLUDED_Transition_h_
#define INCLUDED_Transition_h_

#include "BehaviorBase.h"
#include <vector>

class StateNode;

//! Represents a transition between StateNodes.
/*! This is an abstract class - you'll want to subclass it to put
 *  conditions on the transitions.  Transitions are "smart" - they are
 *  behaviors in and of themselves and can listen for events and use
 *  the standard DoStart/DoStop interface.  Based on the events they
 *  receive, they can fire() themselves and cause a state
 *  transition.
 *
 *  DoStart() will be called when this transition is 'active' - it
 *  should listen/monitor for the transition it represents until
 *  DoStop() is called.
 *  
 *  If the conditions are satisified for a transition, you should call
 *  fire() to do the appropriate notifications.
 *
 *  Also note that a transition can have multiple sources and
 *  destinations.  See fire().
 *
 *  When setting up, the flow of additions follows the flow of
 *  control.  In other words, you add a transition to a source, and
 *  you add a destination to a transition.  This makes the code
 *  simpler because it doesn't have to worry about recursive looping
 *  depending whether the source was added to the transition or the
 *  transition was added to the source.  Confusing?  Exactly.
 *
 *  A template file is available at <a href="http://cvs.tekkotsu.org/cgi/viewcvs.cgi/Tekkotsu/project/templates/transition.h?rev=HEAD&content-type=text/vnd.viewcvs-markup"><i>project</i><tt>/templates/transition.h</tt></a>,
 *  which will help you get moving faster.
 */
class Transition : public BehaviorBase {
	friend class StateNode;
public:
	//!destructor
	virtual ~Transition() {}

	//!call this when the transition should be made, base class version simply calls StateNode::DoStop() on each active of #srcs and StateNode::DoStart() on each inactive of #dsts, but you can override.
	virtual void fire();

	virtual std::vector<StateNode*>& getSources() { return srcs; }  //!< returns a user-modifiable reference to the current source list
	virtual const std::vector<StateNode*>& getSources() const { return srcs; } //!< returns a const reference to the current source list

	virtual void addDestination(StateNode* destination) { if(destination!=NULL) dsts.push_back(destination); } //!< if @a destination is non-null, add it to the destination list
	virtual std::vector<StateNode*>& getDestinations() { return dsts; } //!< returns a user-modifiable reference to the current destination list
	virtual const std::vector<StateNode*>& getDestinations() const { return dsts; } //!< returns a const reference to the current destination list

	virtual void setSound(const std::string& snd) { sound=snd; } //!< set a sound file to be played upon activation; you might want to preload this in the parent node; empty string to turn off
	virtual std::string getSound() { return sound; } //!< returns the current sound file
	
	//! If #instanceName == #className, will autogenerate a name incorporating source and destination names
	virtual std::string getName() const;

protected:
	//!constructor, pass your subclass type name as a string for the default name
	explicit Transition(const std::string& classname) : BehaviorBase(classname), srcs(), dsts(), sound() {}
	//!constructor, specify destination StateNode (ignores NULL)
	Transition(const std::string& classname, StateNode* destination) : BehaviorBase(classname), srcs(), dsts(), sound() {
		addDestination(destination);
	}
	//!constructor, pass your subclass type name as a string for the default name, and a separate instance name
	Transition(const std::string& classname, const std::string& instancename) : BehaviorBase(classname,instancename), srcs(), dsts(), sound() {}
	//!constructor, specify names and destination StateNode (ignores NULL)
	Transition(const std::string& classname, const std::string& instancename, StateNode* destination) : BehaviorBase(classname,instancename), srcs(), dsts(), sound() {
		addDestination(destination);
	}
	//!copy constructor, just in case you need it
	Transition(const Transition& t) : BehaviorBase(t), srcs(t.srcs), dsts(t.dsts), sound(t.sound) {}

	//!assignment operator (only does shallow copy)
	Transition& operator=(const Transition& t) { BehaviorBase::operator=(t); srcs=t.srcs; dsts=t.dsts; sound=t.sound; return *this; }

	//! if @a source is non-null, add it to the source list
	/*! Only StateNodes should be calling this - you add a transition to a source, not a source to a transition.
	 *  @see StateNode::addTransition() */
	virtual void addSource(StateNode* source) { if(source!=NULL) srcs.push_back(source); } 

	std::vector<StateNode*> srcs; //!< the node being transitioned from
	std::vector<StateNode*> dsts; //!< the node being transitioned to
	std::string sound;            //!< sound to play on transitioning
};

/*! @file
 * @brief Describes Transition, represents a transition between StateNodes.
 * @author ejt (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.13 $
 * $State: Exp $
 * $Date: 2006/10/03 21:08:20 $
 */

#endif
