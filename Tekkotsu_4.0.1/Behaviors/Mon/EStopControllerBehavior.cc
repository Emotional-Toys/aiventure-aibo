#include "EStopControllerBehavior.h"
#include "Motion/EmergencyStopMC.h"
#include "Motion/MMAccessor.h"

EStopControllerBehavior* EStopControllerBehavior::theOne = NULL;

void EStopControllerBehavior::DoStart() {
	// Behavior startup
	BehaviorBase::DoStart();
	// We listen to the estop
	erouter->addListener(this, EventBase::estopEGID);
	// Turn on wireless
	cmdsock=wireless->socket(Socket::SOCK_STREAM, 300, 300);
	wireless->setDaemon(cmdsock,true);
	wireless->setReceiver(cmdsock->sock, callback);
	wireless->listen(cmdsock->sock, config->main.estopControl_port);
}

void EStopControllerBehavior::DoStop() {
	// Turn off timers
	erouter->removeListener(this);
	// Close socket; turn wireless off
  wireless->setDaemon(cmdsock,false);
	wireless->close(cmdsock);
	// Total behavior stop
	BehaviorBase::DoStop();
}

void EStopControllerBehavior::runCommand(const std::string& s) {
	if(s==std::string("start")) {
		MMAccessor<EmergencyStopMC> estop(estop_id);
		estop->setStopped(false);
	} else if(s==std::string("stop")) {
		MMAccessor<EmergencyStopMC> estop(estop_id);
		estop->setStopped(true);
	} else if(s==std::string("refresh")) {
		MMAccessor<EmergencyStopMC> estop(estop_id);
		if(estop.checkin(estop->getStopped()))
			cmdsock->printf("on\n");
		else
			cmdsock->printf("off\n");
	} else {
		serr->printf("EStopControllerBehavior::runCommand() - bad message: '%s'",s.c_str());
	}
}

void EStopControllerBehavior::processEvent(const EventBase & e) {
	if(e.getTypeID()==EventBase::activateETID) {
		cmdsock->printf("on\n");
	} else if(e.getTypeID()==EventBase::deactivateETID) {
		cmdsock->printf("off\n");
	}
}

// The command packet reassembly mechanism
int EStopControllerBehavior::callback(char *buf, int bytes) {
	if(EStopControllerBehavior::theOne==NULL)
		return 0;
	static std::string cmd;
	for(int i=0; i<bytes; i++) {
		if(buf[i]=='\n') {
			EStopControllerBehavior::theOne->runCommand(cmd);
			cmd.clear();
		} else
			cmd+=buf[i];
	}
  return 0;
}

/*! @file
 * @brief Implements EStopControllerBehavior, listens to commands coming in from the command port for remotely controlling toggling the estop
 * @author tss (Creator)
 * 
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.5 $
 * $State: Exp $
 * $Date: 2007/05/21 20:51:20 $
 */

