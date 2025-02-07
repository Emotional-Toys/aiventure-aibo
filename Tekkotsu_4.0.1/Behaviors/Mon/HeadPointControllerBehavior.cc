#include "HeadPointControllerBehavior.h"
#include "Behaviors/Controller.h"
#include "Motion/MMAccessor.h"
#include "Motion/HeadPointerMC.h"
#include "Shared/ERS7Info.h"
#include "Shared/ERS210Info.h"

HeadPointControllerBehavior* HeadPointControllerBehavior::theOne = NULL;

void HeadPointControllerBehavior::runCommand(unsigned char *command) {
	// Extract the command parameter
	float param;
	unsigned char *paramp = (unsigned char *) &param;

#if defined(BYTE_ORDER) && BYTE_ORDER==BIG_ENDIAN
	paramp[0] = command[4];
	paramp[1] = command[3];
	paramp[2] = command[2];
	paramp[3] = command[1];
#else
	paramp[0] = command[1];
	paramp[1] = command[2];
	paramp[2] = command[3];
	paramp[3] = command[4];
#endif
	
	// Find out what type of command this is
#ifdef TGT_IS_AIBO
	switch(command[0]) {
	case CMD_tilt:
		t = fabs(param)*outputRanges[HeadOffset+TiltOffset][param>0?MaxRange:MinRange];
		break;
	case CMD_pan:
		p = fabs(param)*outputRanges[HeadOffset+PanOffset][param>0?MaxRange:MinRange];
		break;
	case CMD_roll:
		r = fabs(param)*outputRanges[HeadOffset+RollOffset][param>0?MaxRange:MinRange];
		break;
	default:
		std::cout << "MECHA: unknown command " << command[0] << std::endl;
	}
#else
	switch(command[0]) {
		case CMD_tilt: {
			const char* n = ERS7Info::outputNames[ERS7Info::HeadOffset+ERS7Info::TiltOffset];
			unsigned int i = capabilities.findOutputOffset(n);
			if(i!=-1U)
				t = fabs(param)*outputRanges[i][param>0?MaxRange:MinRange];
		} break;
		case CMD_pan: {
			const char* n = ERS7Info::outputNames[ERS7Info::HeadOffset+ERS7Info::PanOffset];
			unsigned int i = capabilities.findOutputOffset(n);
			if(i!=-1U)
				p = fabs(param)*outputRanges[i][param>0?MaxRange:MinRange];
		} break;
		case CMD_roll: {
			const char* n = ERS7Info::outputNames[ERS7Info::HeadOffset+ERS7Info::NodOffset];
			unsigned int i = capabilities.findOutputOffset(n);
			if(i!=-1U)
				r = fabs(param)*outputRanges[i][param>0?MaxRange:MinRange];
			else {
				n = ERS210Info::outputNames[ERS210Info::HeadOffset+ERS210Info::RollOffset];
				i = capabilities.findOutputOffset(n);
				if(i!=-1U)
					r = fabs(param)*outputRanges[i][param>0?MaxRange:MinRange];
			}
		} break;
		default:
			std::cout << "MECHA: unknown command " << command[0] << std::endl;
	}
#endif

	// If the command was a new motion command, apply the
	// new motion parameters:
	switch(command[0]) {
	case CMD_tilt:
	case CMD_pan:
	case CMD_roll:
		{
			MMAccessor<HeadPointerMC> head(head_id);
			head->setJoints(t,p,r);
		}
	}
}

void HeadPointControllerBehavior::DoStart() {
	// Behavior startup
	BehaviorBase::DoStart();
	// Enable head control
	SharedObject<HeadPointerMC> head;
	head->setHold(false);
	head_id = motman->addPersistentMotion(head);
	// Turn on wireless
	theLastOne=theOne;
	theOne=this;
	cmdsock=wireless->socket(Socket::SOCK_STREAM, 2048, 2048);
	wireless->setReceiver(cmdsock->sock, mechacmd_callback);
	wireless->setDaemon(cmdsock,true);
	wireless->listen(cmdsock->sock, config->main.headControl_port);
	// Open the WalkGUI on the desktop
	Controller::loadGUI("org.tekkotsu.mon.HeadPointGUI","HeadPointGUI",config->main.headControl_port);
}

void HeadPointControllerBehavior::DoStop() {
	// Close the GUI
	Controller::closeGUI("HeadPointGUI");
	// Turn off timers
	erouter->removeListener(this);
	// Close socket; turn wireless off
	wireless->setDaemon(cmdsock,false);
	wireless->close(cmdsock);
	theOne=theLastOne;
	// Disable head pointer
	motman->removeMotion(head_id);
	// Total behavior stop
	BehaviorBase::DoStop();
}

// The command packet reassembly mechanism
int HeadPointControllerBehavior::mechacmd_callback(char *buf, int bytes) {
  static char cb_buf[5];
  static int cb_buf_filled;

  // If there's an incomplete command in the command buffer, fill
  // up as much of the command buffer as we can and then execute it
  // if possible
  if(cb_buf_filled) {
    while((cb_buf_filled < 5) && bytes) {
      cb_buf[cb_buf_filled++] = *buf++;	// copy incoming buffer byte
      --bytes;				// decrement remaining byte ct.
    }
    // did we fill it? if so, execute! and mark buffer empty.
    if(cb_buf_filled == 5) {
      if(HeadPointControllerBehavior::theOne) HeadPointControllerBehavior::theOne->runCommand((unsigned char*) cb_buf);
      cb_buf_filled = 0;
    }
  }

  // now execute all complete bytes in the incoming buffer
  while(bytes >= 5) {
    if(HeadPointControllerBehavior::theOne) HeadPointControllerBehavior::theOne->runCommand((unsigned char *) buf);
    bytes -= 5;
    buf += 5;
  }

  // finally, store all remaining bytes in the command buffer
  while(bytes) {
    cb_buf[cb_buf_filled++] = *buf++;
    --bytes;
  }

  return 0;
}

/*! @file
 * @brief Implements HeadPointControllerBehavior, listens to control commands coming in from the command port for remotely controlling the head
 * @author tss (Creator)
 * 
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.16 $
 * $State: Exp $
 * $Date: 2007/11/12 18:00:41 $
 */

