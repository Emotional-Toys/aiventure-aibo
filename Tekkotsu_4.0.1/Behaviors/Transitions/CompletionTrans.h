//-*-c++-*-
#ifndef INCLUDED_CompletionTrans_h_
#define INCLUDED_CompletionTrans_h_

#include "Behaviors/StateNode.h"
#include "Behaviors/Transition.h"
#include "Events/EventRouter.h"

//! causes a transition when at least @e n sources have signalled completion;  @e n = 0 means "all" (default)
class CompletionTrans : public Transition {
protected:
  int minsrcs; //!< the minimum number of sources which must signal completion before this transition will fire
  bool *completions;  //!< pointer to array for recording completion events for all sources

public:
	//! constructor, pass @a destination and the minimum number of sources which must signal completion before this transition will fire
  CompletionTrans(StateNode* destination, int n=0) :
    Transition("CompletionTrans",destination), minsrcs(n), completions(NULL) {};

	//! constructor, pass @a name, @a destination and the minimum number of times the source must signal completion beyond the first (@a n)
  CompletionTrans(const std::string& name, StateNode* destination, int n=0) :
    Transition("CompletionTrans",name,destination), minsrcs(n), completions(NULL) {};

  //! starts listening
  virtual void DoStart() {
    Transition::DoStart();
    unsigned int const numsrcs = getSources().size();
    completions = new bool[numsrcs];
    for (unsigned int i = 0; i < numsrcs; i++) {
      completions[i] = false;
      erouter->addListener(this,
			   EventBase::stateMachineEGID,
			   reinterpret_cast<size_t>(getSources()[i]),
			   EventBase::statusETID);
    };
  }

  //! stops listening
  virtual void DoStop() {
    erouter->removeListener(this);
    delete completions;
    completions = NULL;
    Transition::DoStop();
  }

  //! record completions, and fire the transition if all sources have completed
  virtual void processEvent(const EventBase &event) {
    int numcomplete = 0;
    for ( unsigned int i=0; i<getSources().size(); i++ ) {
      if ( event.getSourceID() == reinterpret_cast<size_t>(getSources()[i]) )
	completions[i] = true;
      if ( completions[i] ) ++numcomplete;
    };
    int const threshold = (minsrcs > 0 ? minsrcs : (int)getSources().size());
    if (numcomplete >= threshold) fire();
  }

protected:
	//!constructor, this version is only need by subclasses so they can pass their type name
  CompletionTrans(const std::string& classname, const std::string& instancename, StateNode* destination, int n=0) :
    Transition(classname,instancename,destination), minsrcs(n), completions(NULL) {};

  //!@name Dummy functions to satisfy the compiler
  CompletionTrans(const CompletionTrans&);  //!< don't call this
  CompletionTrans& operator=(const CompletionTrans&);  //!< don't call this
  //@}

};

/*! @file
 * @brief Defines Completiontrans, which causes a transition if all sources have signalled completion
 * @author dst (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.10 $
 * $State: Exp $
 * $Date: 2007/11/13 04:16:01 $
 */

#endif
