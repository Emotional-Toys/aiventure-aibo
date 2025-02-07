//-*-c++-*-
#ifndef INCLUDED_TailWagNode_h_
#define INCLUDED_TailWagNode_h_

#include "MCNode.h"
#include "Events/EventRouter.h"
#include "Motion/PostureMC.h"

//!default name for PostureNode's (have to instantiate a variable in order to use as a template argument)
/*! instantiation will be placed in MCNode.cc to avoid file bloat */
extern const char defPostureNodeName[];
//!default description for PostureNode's (have to instantiate a variable in order to use as a template argument)
/*! instantiation will be placed in MCNode.cc to avoid file bloat */
extern const char defPostureNodeDesc[];

//! A simple StateNode that executes a PostureMC motion command
/*! Caches the posture file in a private PostureEngine because the
 *  motion command might be shared with other functions that are
 *  using it for other purposes */
class PostureNode : public MCNode<PostureMC,defPostureNodeName,defPostureNodeDesc,true> {
public:
	
	//! Constructor: takes optional instance name and filename.
	/*! Caches the posture file in a private PostureEngine because the
	 motion command might be shared with other functions that are
	 using it for other purposes */
	PostureNode(const std::string &nodename=defPostureNodeName, const std::string &filename=std::string())
	: MCNode<PostureMC,defPostureNodeName,defPostureNodeDesc,true>(nodename), posture()
	{
		if ( filename.size() > 0 )
			posture.loadFile(filename.c_str());
	}
	
	virtual void DoStart() {
		getMC()->setAverage(posture,1);  // copy cached posture into the motion command
		MCNode<PostureMC,defPostureNodeName,defPostureNodeDesc,true>::DoStart();
	}
	
	//! loads the specified file into #posture, note this @em doesn't affect the current PostureMC, just the cached one which will be loaded into it on next activation.  See getPosture(), getMC_ID()
	virtual void loadFile(const std::string &filename) {
		posture.loadFile(filename.c_str());
		getMC()->setAverage(posture,1);
	}
	
	//! accessor for #posture, note this @em doesn't affect the current PostureMC, just the cached one which will be loaded into it on next activation.  See getMC_ID()
	virtual PostureEngine& getPosture() { return posture; }
	//! accessor for #posture, note this @em doesn't return the current PostureMC, just the cached one which will be loaded into it on next activation.  See getMC_ID()
	virtual const PostureEngine& getPosture() const { return posture; }
	
protected:
	//! The internal cache of joint positions, copied to the motion command when activated.
	/*! This allows the motion command to be shared by other nodes/behaviors, which might modify
	 *  the posture on an ongoing basis. */
	PostureEngine posture;
};

/*! @file
 * @brief Defines PostureNode, a simple StateNode that runs a PostureMC motion command
 * @author dst
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.5 $
 * $State: Exp $
 * $Date: 2007/11/11 23:57:19 $
 */

#endif
