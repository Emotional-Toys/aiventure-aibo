//-*-c++-*-
#ifndef INCLUDED_MCNode_h_
#define INCLUDED_MCNode_h_

#include "Behaviors/StateNode.h"
#include "Motion/MotionManager.h"
#include "Motion/MMAccessor.h"

//! Common parent class for all the templated MCNode, which is what you want to instantiate.
class MCNodeBase : public StateNode {
public:
	static const char defName[]; //!< the default name for MCNodes -- can be overridden via MCNode's template arguments
	static const char defDesc[]; //!< the default description for MCNodes -- can be overridden via MCNode's template arguments

	//! destructor, free #mc
	virtual ~MCNodeBase() { delete mc; }
	
	//! Adds the motion command to the motion manager, add a listener for the motion's completion event
	virtual void DoStart();
	
	//! Assumes the event is a completion event from the motion, throws a corresponding state node completion event
	virtual void processEvent(const EventBase& /*e*/);
	
	//! Removes the motion command from the motion manager if it was our own creation
	virtual void DoStop();	
	
	//! Allows you to assign a previously created motion, which might be shared among several MCNodes
	/*! If this node already has an #mc, then it will be freed, removing from MotionManager if necessary */
	virtual void setMC(MotionManager::MC_ID mcid);
	
	//! reveal the MC_ID; if the motion isn't currently active, returns MotionManager::invalid_MC_ID
	virtual MotionManager::MC_ID getMC_ID() { return mc_id; }
	
	static std::string getClassDescription() { return defName; }
	virtual std::string getDescription() const { return getClassDescription(); }
	
protected:
	//! constructor for subclasses (which would need to provide a different class name)
	MCNodeBase(const std::string &class_name, const std::string &node_name, bool expectCompletion=true)
		: StateNode(class_name,node_name), mc(NULL), mc_id(MotionManager::invalid_MC_ID), mcCompletes(expectCompletion)
	{}
	
	//! returns reference to #mc or a new SharedObject<T> if #mc is currently NULL (so it will always return a valid value)
	/*! if a particular motion command needs some initial setup besides the default constructor,
	 *  overriding this function is a good opportunity to do so */
	virtual SharedObjectBase& getPrivateMC()=0;

	//! returns true if the motion command being used was created internally via getPrivateMC()
	virtual bool hasPrivateMC() { return mc!=NULL; }

	SharedObjectBase* mc;    //!< MotionCommand used by this node (may be NULL if sharing the MC with other nodes)
	MotionManager::MC_ID mc_id;  //!< id number for the MotionCommand
	bool mcCompletes; //!< if true, will post a completion when the underlying MotionCommand posts a status

private:
	MCNodeBase(const MCNodeBase&); //!< don't call (copy constructor)
	MCNodeBase& operator=(const MCNodeBase&); //!< don't call (assignment operator)
};

//! A generic wrapper for any MotionCommand, note that some functions are provided by the MCNodeBase common base class, namely MCNodeBase::setMC() and MCNodeBase::getMC_ID()
template<class T, const char* mcName=MCNodeBase::defName, const char* mcDesc=MCNodeBase::defDesc, bool completes=true>
class MCNode : public MCNodeBase {
public:
	//! default constructor, use type name as instance name
	MCNode()
		: MCNodeBase(mcName,mcName,completes)
	{}
	
	//! constructor, take an instance name
	MCNode(const std::string& nm)
		: MCNodeBase(mcName,nm,completes)
	{}
	
	//! destructor
	virtual ~MCNode() {}
	
	//! reveal the MotionCommand through an MMAccessor
	/*! This is a no-op if the motion command hasn't been added to motion manager yet, and enforces mutual exclusion if it has */
	virtual MMAccessor<T> getMC();
	
	static std::string getClassDescription() { return mcDesc; }
	virtual std::string getDescription() const { return getClassDescription(); }

protected:
	//! constructor for subclasses (which would need to provide a different class name)
	MCNode(const std::string &class_name, const std::string &node_name)
		: MCNodeBase(class_name,node_name)
	{}
	
	virtual SharedObject<T>& getPrivateMC();
};


// ****************************
// ******* IMPLEMENTATION *******
// ****************************

template<class T, const char* mcName, const char* mcDesc, bool completes>
MMAccessor<T> MCNode<T,mcName,mcDesc,completes>::getMC() {
	if(mc_id==MotionManager::invalid_MC_ID) {
		// motion hasn't been added to motion manager yet
		return MMAccessor<T>(*getPrivateMC());
	} else {
		// motion has been added to motion manager, check it out
		return MMAccessor<T>(mc_id);
	}
}

template<class T, const char* mcName, const char* mcDesc, bool completes>
SharedObject<T>& MCNode<T,mcName,mcDesc,completes>::getPrivateMC() {
	if(mc==NULL)
		mc=new SharedObject<T>;
	return dynamic_cast<SharedObject<T>&>(*mc);
}


/*! @file
 * @brief Defines MCNode, which provides generic wrapper for any MotionCommand
 * @author Ethan Tira-Thompson (ejt) (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.6 $
 * $State: Exp $
 * $Date: 2006/09/27 20:12:37 $
 */

#endif
