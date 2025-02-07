#include "MotionCommand.h"
#include "MotionManager.h"
#include "Events/EventTranslator.h"
#include "Events/EventRouter.h"
#include <iostream>

using namespace std;

void MotionCommand::postEvent(const EventBase& event) {
	if(queue==NULL) {
		erouter->postEvent(event);
	} else {
		queue->encodeEvent(event);
	}
}

void MotionCommand::resetWheels() {
#ifdef TGT_HAS_WHEELS
  //cout << "Resetting " << NumWheels << " wheels" << endl;
  for (unsigned int i = WheelOffset; i < WheelOffset + NumWheels; i++) {
    motman->setOutput(this, i, 0.0);
  }
#endif
}



/*! @file
 * @brief Empty
 * @author ejt (Creator)
 *
 * $Author: kcomer $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.6 $
 * $State: Exp $
 * $Date: 2007/11/15 21:31:18 $
 */

