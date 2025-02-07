#include "StringInputControl.h"
#include "Wireless/Wireless.h"
#include "Motion/MMAccessor.h"
#include "Motion/LedMC.h"
#include "Motion/MotionManager.h"
#include <sstream>

/*ControlBase* StringInputControl::activate(MotionManager::MC_ID disp_id, Socket* gui) {
	display_id=disp_id;
	gui_comm=gui;
	return doReadStdIn(userPrompt);
	}*/

void StringInputControl::refresh() {
	ControlBase::refresh();
	doReadStdIn(userPrompt);
}

ControlBase* StringInputControl::doReadStdIn(const std::string& prompt/*=std::string()*/) {
	if(gui_comm!=NULL && wireless->isConnected(gui_comm->sock)) {
		//		cout << "REFRESHING " << getName() << endl;
		//try to get it all in one packet for better performance
		std::stringstream ss;
		ss << "refresh\n"
			 << getName() << '\n'
			 << "1\n"
			 << "0\n"
			 << "0\n"
			 << "Waiting for input...\n"
			 << std::count(userPrompt.begin(),userPrompt.end(),'\n') << '\n'
			 << userPrompt << "\n";
		gui_comm->write((const byte*)ss.str().c_str(),ss.str().size());
	}
	
	if(prompt.size()==0)
		return ControlBase::doReadStdIn(userPrompt);
	else
		return ControlBase::doReadStdIn(prompt);
}

/*! @file
 * @brief Implements StringInputControl, which prompts for and stores a string from the user
 * @author ejt (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.6 $
 * $State: Exp $
 * $Date: 2007/01/30 22:56:18 $
 */

