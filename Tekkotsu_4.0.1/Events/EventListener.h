#ifndef INCLUDED_EventListener_h
#define INCLUDED_EventListener_h

#include "EventBase.h"

//! An interface to allow a standard method of passing events
class EventListener {
 public:
	//! destructor
	virtual ~EventListener() {}

	//! for receiving events - you must override this to inherit
	/*! @see EventRouter
	 *  @param event the event being received */
	virtual void processEvent(const EventBase& event)=0;
};

/*! @file
 * @brief Defines EventListener class, an interface for anything that wants to receive events
 * @author ejt (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.2 $
 * $State: Rel $
 * $Date: 2003/09/25 15:27:10 $
 */

#endif
