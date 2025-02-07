//-*-c++-*-
#ifndef INCLUDED_GroupNode_h_
#define INCLUDED_GroupNode_h_

#include "Behaviors/StateNode.h"
#include <vector>

//! Allows a group of StateNodes to be activated together
/*! When a transition occurs into the group node, it will activate all
 *	of its subnodes.
 *
 *  @note  Another (probably better) way to do this is with a
 *  multi-destination transition - after the initialization of the
 *  transition (usually specifying the first transition) you can call
 *  the Transition::addDestination() function to add addition
 *  destinations, replicating the functionality of this class.
 *
 *  The group node doesn't deactivate its subnodes together.  In
 *  essence, this is a fork - one transition targeting the GroupNode
 *  can activate several states, which are then separate branches of
 *  execution.  (Although of course only one is actually being
 *  executed at a time, as per the cooperative multi-tasking of
 *  behaviors.)
 *
 */
class GroupNode : public StateNode {
public:
	//!constructor
	GroupNode() : StateNode("GroupNode","GroupNode") {}
	//!constructor
	explicit GroupNode(const std::string& nm) : StateNode("GroupNode",nm) {}

	//! activates all of the sub nodes
	virtual void DoStart() {
		StateNode::DoStart();
		for(std::vector<StateNode*>::iterator it=nodes.begin(); it!=nodes.end(); it++)
			if(!(*it)->isActive())
				(*it)->DoStart();
	}
	
private:
	GroupNode(const GroupNode& node); //!< don't call this
	GroupNode operator=(const GroupNode& node); //!< don't call this
};

/*! @file
 * @brief Defines GroupNode, which allows a group of StateNodes to be activated together
 * @author ejt (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.6 $
 * $State: Exp $
 * $Date: 2005/01/24 22:23:50 $
 */

#endif
