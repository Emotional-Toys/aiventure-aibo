//-*-c++-*-
#ifndef INCLUDED_SignalTrans_h_
#define INCLUDED_SignalTrans_h_

#include "Behaviors/Transition.h"
#include "Events/DataEvent.h"
#include "Events/EventRouter.h"

//! causes a transition if a DataEvent from stateSignalEGID occurs with a specific value
/*! This allows a state node to signal a transition to another state
 *  in a clean symbolic way.  Only the transition itself needs to
 *  know the address of the destination node.
 */

template<class T>
class SignalTrans : public Transition {
public:
  //! Constructor
  SignalTrans(StateNode *destination, const T &value) :
    Transition("SignalTrans",destination), val(value)
  { }

  //! Constructor
  SignalTrans(const std::string &name, StateNode *destination, const T &value) :
    Transition("SignalTrans",name,destination), val(value)
  { }

  virtual void DoStart() {
    Transition::DoStart();
    for ( std::vector<StateNode*>::const_iterator it = srcs.begin(); it != srcs.end(); it++ )
      erouter->addListener(this,EventBase::stateSignalEGID,(unsigned int)*it);
  }

  virtual void processEvent(const EventBase &event) {
    const DataEvent<T> &d_event = dynamic_cast<const DataEvent<T>&>(event);
    if ( d_event.getData() == val )
      fire();
  }

protected:
  //! Constructor
  SignalTrans(const std::string &classname, const std::string &instancename,
	      StateNode *destination, const T &value) :
    Transition(classname,instancename,destination,value)
  { }

  T val; //!< value to compare against

};

#endif
