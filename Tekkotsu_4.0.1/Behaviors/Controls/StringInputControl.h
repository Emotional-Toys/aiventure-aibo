//-*-c++-*-
#ifndef INCLUDED_StringInputControl_h_
#define INCLUDED_StringInputControl_h_

#include "ControlBase.h"
#include "Motion/PostureEngine.h"
#include <string>

//! Upon activation, prompts the user for a string and stores it
class StringInputControl : public ControlBase {
public:
	//! Constructor
	StringInputControl(const std::string& n, const std::string& prompt) : ControlBase(n,prompt), lastInput(), userPrompt(prompt) {}
	//! Constructor
	StringInputControl(const std::string& n, const std::string& desc, const std::string& prompt) : ControlBase(n,desc), lastInput(), userPrompt(prompt) {}

	//	virtual ControlBase* activate(MotionManager::MC_ID disp_id, Socket* gui);
	
	virtual void refresh();
	
	virtual ControlBase* doReadStdIn(const std::string& prompt/*=std::string()*/);
	
	virtual ControlBase * takeInput(const std::string& msg) {
		lastInput=msg;
		return NULL;
	}	
	
	//! returns last call to takeInput()
	virtual const std::string& getLastInput() const { return lastInput; }

	//! clears the last input (i.e. so you can easily tell later if new input is entered)
	virtual void clearLastInput() { takeInput(""); }

	//! sets the prompt to give to the user
	virtual void setPrompt(const std::string& prompt) { userPrompt=prompt; }

protected:
	std::string lastInput;  //!< stores the last input to takeInput()
	std::string userPrompt; //!< stores the prompt to send out
};

/*! @file
 * @brief Defines StringInputControl, which prompts for and stores a string from the user
 * @author ejt (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.6 $
 * $State: Exp $
 * $Date: 2007/01/30 22:56:18 $
 */

#endif
