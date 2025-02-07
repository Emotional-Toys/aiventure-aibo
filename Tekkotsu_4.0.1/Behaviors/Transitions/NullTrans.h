//-*-c++-*-
#ifndef INCLUDED_NullTrans_h_
#define INCLUDED_NullTrans_h_

#include "Behaviors/Transition.h"
#include "Events/EventRouter.h"

//! a transition that occurs (via a 0 msec Timer event) as soon as the source node finishes starting up
class NullTrans : public Transition {
public:
  //! constructor
  NullTrans(StateNode* destination) : Transition("NullTrans",destination) {}
	
  //! constructor
  NullTrans(const std::string& name, StateNode* destination) : 
    Transition("NullTrans",name,destination) {}
	
  //!starts 0 msec timer, so transition will occur very soon
  virtual void DoStart() {
    Transition::DoStart();
    erouter->addTimer(this,0,0,false);
  }

  //!stops timer
  virtual void DoStop() { erouter->removeListener(this); Transition::DoStop(); }

  //!when timer event is received, fire() the transition
  virtual void processEvent(const EventBase&) { fire(); }

protected:
	//! constructor, only to be called by subclasses (which need to specify their own @a classname)
  NullTrans(const std::string &classname, const std::string &instancename, 
	    StateNode* destination) :
    Transition(classname,instancename,destination) {}

};

/*! @file
 * @brief Defines NullTrans, which causes a transition as soon as the source node finishes starting up
 * @author dst (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.8 $
 * $State: Exp $
 * $Date: 2004/12/23 01:47:07 $
 */

#endif
