//-*-c++-*-
#ifndef INCLUDED_LedNode_h_
#define INCLUDED_LedNode_h_

#include "MCNode.h"
#include "Motion/LedMC.h"

//! A simple StateNode that executes a LedMC motion command and throws a status event upon completion
/*! Extends MCNode slightly so that each time the LedMC is accessed, any flash commands are reset.
 *  This allows a flash to be triggered each time the node starts */
class LedNode : public MCNode<LedMC> {
public:
	//! default constructor, use type name as instance name
	LedNode() : MCNode<LedMC>("LedNode","LedNode"), lastAccess(0) {}
	
	//! constructor, take an instance name
	LedNode(const std::string& nm) : MCNode<LedMC>("LedNode",nm), lastAccess(0) {}
	
	static std::string getClassDescription() { return "Displays a pattern on the LEDs for as long as the state is active"; }
	virtual std::string getDescription() const { return getClassDescription(); }
	
protected:
	//! constructor for subclasses (which would need to provide a different class name)
	LedNode(const std::string &class_name, const std::string &node_name) : MCNode<LedMC>(class_name,node_name), lastAccess(0) {}
	
	//! extends MCNode implementation so that each time the LedMC is accessed, any flash commands are reset.
	virtual SharedObject<LedMC>& getPrivateMC() {
		unsigned int curtime=get_time();
		bool isFirstCreation=(mc==NULL);
		SharedObject<LedMC>& l=MCNode<LedMC>::getPrivateMC();
		if(!isFirstCreation)
			l->extendFlash(curtime-lastAccess);
		lastAccess=curtime;
		return l;
	}
	
	unsigned int lastAccess; //!< stores time of last call to getPrivateMC()
};

/*! @file
 * @brief Defines LedNode, a simple StateNode that runs a LedMC motion command and throws a status event upon completion
 * @author dst (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.8 $
 * $State: Exp $
 * $Date: 2006/09/16 17:32:38 $
 */

#endif
