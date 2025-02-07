//-*-c++-*-
#ifndef INCLUDED_EStopControllerBehavior_h_
#define INCLUDED_EStopControllerBehavior_h_

#include <iostream>
#include "Wireless/Wireless.h"
#include "Behaviors/BehaviorBase.h"
#include "Motion/MotionManager.h"
#include "Events/EventRouter.h"
#include "Events/EventBase.h"
#include "Shared/Config.h"

//! Listens to control commands coming in from the command port for remotely controlling the head
class EStopControllerBehavior : public BehaviorBase {

public:	
	//! Points to the one EStopControllerBehavior object that the input command stream is talking to.
	/*! A kludge. Dunno how you're gonna make sure you're not using this uninitialized. */
	static EStopControllerBehavior * theOne;
	static int callback(char *buf, int bytes); //!< called by wireless when there's new data

public:
	//! constructor
	EStopControllerBehavior(MotionManager::MC_ID estop)
		: BehaviorBase("EStopControllerBehavior"),
			cmdsock(NULL),
			estop_id(estop)
	{
		theOne=this;
	}
	//! destructor
	virtual ~EStopControllerBehavior() {
		theOne=NULL;
	}

	virtual void DoStart();

	virtual void DoStop();

	virtual void processEvent(const EventBase &);

	static std::string getClassDescription() {
		char tmp[20];
		sprintf(tmp,"%d",*config->main.estopControl_port);
		return std::string("Listens to estop commands coming in from port ")+tmp;
	}
	virtual std::string getDescription() const { return getClassDescription(); }

	virtual void runCommand(const std::string& s); //!< processes a string received from wireless

protected:
	//! The input command stream socket
	Socket *cmdsock;

	//! The estop to control
	MotionManager::MC_ID estop_id;

private:
	EStopControllerBehavior(const EStopControllerBehavior&); //!< don't call
	EStopControllerBehavior operator=(const EStopControllerBehavior&); //!< don't call

};

/*! @file
 * @brief Describes EStopControllerBehavior, listens to control commands coming in from the command port for remotely toggling the estop
 * @author tss (Creator)
 * 
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.6 $
 * $State: Exp $
 * $Date: 2007/01/30 22:56:19 $
 */

#endif 
