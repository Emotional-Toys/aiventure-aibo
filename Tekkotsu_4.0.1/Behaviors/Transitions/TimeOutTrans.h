//-*-c++-*-
#ifndef INCLUDED_TimeOutTrans_h_
#define INCLUDED_TimeOutTrans_h_

#include "Behaviors/Transition.h"
#include "Events/EventRouter.h"

//! causes a transition after a specified amount of time has passed
/*! If any event parameters are specified, this transition will listen
 *  for matching events, and if any are received, it will reset the
 *  timer */
class TimeOutTrans : public Transition {
public:
  //! constructor, specify delay in milliseconds
  TimeOutTrans(StateNode* destination, unsigned int delay)
		: Transition("TimeOutTrans","TimeOutTrans",destination), d(delay),
			eventargcount(0), egid(EventBase::unknownEGID), esid(0), etid(EventBase::statusETID) {}

  //! constructor, specify delay in milliseconds, if any events matching given parameters are received, the timer will be reset
  TimeOutTrans(StateNode* destination, unsigned int delay, EventBase::EventGeneratorID_t gid)
		: Transition("TimeOutTrans","TimeOutTrans",destination), d(delay),
			eventargcount(1), egid(gid), esid(0), etid(EventBase::statusETID) {}

  //! constructor, specify delay in milliseconds, if any events matching given parameters are received, the timer will be reset
  TimeOutTrans(StateNode* destination, unsigned int delay, EventBase::EventGeneratorID_t gid, unsigned int sid)
		: Transition("TimeOutTrans","TimeOutTrans",destination), d(delay),
			eventargcount(2), egid(gid), esid(sid), etid(EventBase::statusETID) {}

  //! constructor, specify delay in milliseconds, if any events matching given parameters are received, the timer will be reset
  TimeOutTrans(StateNode* destination, unsigned int delay, EventBase::EventGeneratorID_t gid, unsigned int sid, EventBase::EventTypeID_t tid)
		: Transition("TimeOutTrans","TimeOutTrans",destination), d(delay),
			eventargcount(3), egid(gid), esid(sid), etid(tid) {}

  //!starts timer
  virtual void DoStart() {
    Transition::DoStart();
    switch (eventargcount) {
    case 1: erouter->addListener(this,egid); break;
    case 2: erouter->addListener(this,egid,esid); break;
    case 3: erouter->addListener(this,egid,esid,etid); break;
    };
    resetTimer(); 
  }

  //!stops timer
  virtual void DoStop() {
		erouter->removeListener(this);
		Transition::DoStop();
	}
  
  //!resets timer
  void resetTimer() {
    // std::cout << "Reset @ " << get_time() << " stop @ " << get_time()+d << ' ' << this << std::endl;
    erouter->addTimer(this,0,d,false);
  }

  //!if we receive the timer event, fire()
  virtual void processEvent(const EventBase& e) {
    // std::cout << "Timeout @ " << get_time() << " from " << event.getName() << ' ' << this << std::endl;
		if(e.getGeneratorID()==EventBase::timerEGID)
			fire();
		else
			resetTimer();
  }

protected:
  //! constructor, specify delay in milliseconds - use assignment in your subclass's constructor if you want set #egid,#esid,#etid (don't forget #eventargcount!)
  TimeOutTrans(const std::string& classname, const std::string& instancename, StateNode* destination, unsigned int delay)
		: Transition(classname,instancename,destination), d(delay),
			eventargcount(0), egid(EventBase::unknownEGID), esid(0), etid(EventBase::statusETID) {}

  //!amount to delay (in milliseconds) before transition
  unsigned int d;

	//!level of specificity of events to listen for
	unsigned int eventargcount;

  EventBase::EventGeneratorID_t egid; //!< the event generator to listen for
  unsigned int esid; //!< the source to listen for
  EventBase::EventTypeID_t etid; //!< the type to listen for

};

/*! @file
 * @brief Defines TimeOutTrans, which causes a transition after a specified amount of time has passed
 * @author ejt (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.15 $
 * $State: Exp $
 * $Date: 2004/12/04 00:11:19 $
 */

#endif
