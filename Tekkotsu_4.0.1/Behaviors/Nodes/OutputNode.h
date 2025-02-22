//-*-c++-*-
#ifndef INCLUDED_OutputNode_h_
#define INCLUDED_OutputNode_h_

#include "Behaviors/StateNode.h"
#include <string>
#include <ostream>

//! A very simple StateNode that outputs its name to a given ostream upon activation, handy for debugging
/*! The Event Logger controller item (Status Reports menu) is a much better tool for debugging */
class OutputNode : public StateNode {
public:
	//!constructor, sets name and ostream to use for output
	OutputNode(const char* nm, std::ostream& output) : StateNode("OutputNode",nm), next(NULL), out(output), msg(nm) {}
	//!constructor, sets name and another state which will immediately be transitioned to upon activation
	OutputNode(const char* nm, std::ostream& output, StateNode * nextstate) : StateNode("OutputNode",nm), next(nextstate), out(output), msg(nm) {}
	//!constructor, sets name, message, and another state which will immediately be transitioned to upon activation
	OutputNode(const char* nm, const std::string& mg, std::ostream& output, StateNode * nextstate) : StateNode("OutputNode",nm), next(nextstate), out(output), msg(mg) {}

	//!outputs this state's name, will transition to #next if non-NULL
	/*!if #next is NULL, the state will simply stay active until some other transition causes it to leave*/
	virtual void DoStart() {
		StateNode::DoStart();
		out << msg << std::endl;
		if(next!=NULL) {
			DoStop();
			next->DoStart();
		}
	}
	
protected:
	StateNode* next; //!< the state to transition to upon entering, can be NULL
	std::ostream& out;    //!< the stream to use for output - if not specified (default constructor) cout will be used
	std::string msg;      //!< message to show on activation

private:
	OutputNode(const OutputNode& node); //!< don't call this
	OutputNode operator=(const OutputNode& node); //!< don't call this
};

/*! @file
 * @brief Defines OutputNode, a very simple StateNode that outputs its name to a given ostream upon activation, handy for debugging
 * @author ejt (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.8 $
 * $State: Exp $
 * $Date: 2005/02/02 18:20:27 $
 */

#endif
