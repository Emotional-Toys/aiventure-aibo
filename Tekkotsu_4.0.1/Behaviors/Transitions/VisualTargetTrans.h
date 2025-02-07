//-*-c++-*-
#ifndef INCLUDED_VisualTargetTrans_h_
#define INCLUDED_VisualTargetTrans_h_

#include "Events/EventRouter.h"
#include "Events/VisionObjectEvent.h"
#include "Shared/debuget.h"
#include "Shared/WorldState.h"

//! causes a transition when a visual object has been seen for at least 6 camera frames
class VisualTargetTrans : public Transition {
public:
	//!constructor
	VisualTargetTrans(StateNode* destination, unsigned int source_id)
		: Transition("VisualTargetTrans",destination), sid(source_id), count(0) {}

	//!constructor
	VisualTargetTrans(const std::string& name, StateNode* destination, unsigned int source_id)
		: Transition("VisualTargetTrans",name,destination), sid(source_id), count(0) {}

	//!starts listening for the object specified by the source id in the constructor
	virtual void DoStart() {
		//serr->printf("VisualTargetTrans::DoStart() - enter\n");
		Transition::DoStart();
		count=0;
		erouter->addListener(this,EventBase::visObjEGID,sid);
		//serr->printf("VisualTargetTrans::DoStart() - leave\n");
	}

	//!called by StateNode when it becomes inactive - undo whatever you did in Enable()
	virtual void DoStop() {
		//serr->printf("VisualTargetTrans::DoStop() - enter\n");
		erouter->removeListener(this);
		count=0;
		Transition::DoStop();
		//serr->printf("VisualTargetTrans::DoStop() - leave\n");
	}

	//!if the object is "close", calls fire()
	virtual void processEvent(const EventBase& e) {
		//serr->printf("VisualTargetTrans::processEvent() - enter %d\n",get_time());

		if(e.getTypeID()==EventBase::deactivateETID)
			count=0;
		else
			count++;
		if(count>5)
			fire();

		//serr->printf("VisualTargetTrans::processEvent() - leave %d\n",get_time());
	}

protected:
	//!Source ID of object to track
	unsigned int sid;
	//! number of frames for which we've seen the object
	unsigned int count;
};

/*! @file
 * @brief Defines VisualTargetTrans, which causes a transition when a visual object has been seen for at least 6 camera frames
 * @author ejt (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.14 $
 * $State: Exp $
 * $Date: 2005/06/13 21:23:01 $
 */

#endif
