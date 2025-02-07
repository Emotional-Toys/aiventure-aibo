//-*-c++-*-
#ifndef INCLUDED_StewartPlatformBehavior_h_
#define INCLUDED_StewartPlatformBehavior_h_

#include <iostream>
#include "Behaviors/BehaviorBase.h"
#include "Motion/MotionManager.h"
#include "Shared/Config.h"

class Socket;

//*********** NOT DONE **********//

//! moves the legs in synchrony to emulate the capabilities of a stewart platform
class StewartPlatformBehavior : public BehaviorBase {

public:	
	//! Points to the one StewartPlatformBehavior object that the input command stream is talking to.
	static StewartPlatformBehavior * theOne;
	static int callback(char *buf, int bytes); //!< called by wireless when there's new data

public:
	//! constructor
	StewartPlatformBehavior()
		: BehaviorBase("StewartPlatformBehavior"),
			cmdsock(NULL), sock(-1), ctxt(NULL), docClosed(false),
			pose_id(MotionManager::invalid_MC_ID)
	{
		theOne=this;
	}
	//! destructor
	virtual ~StewartPlatformBehavior() {
		theOne=NULL;
	}

	virtual void DoStart();

	virtual void DoStop();

	virtual void processEvent(const EventBase &);

	static std::string getClassDescription() {
		char tmp[20];
		sprintf(tmp,"%d",*config->main.stewart_port);
		return std::string("moves the legs in synchrony to emulate the capabilities of a stewart platform, base on commands from port ")+tmp;
	}
	virtual std::string getDescription() const { return getClassDescription(); }

	virtual void setParam(const std::string& cmd, float value); //!< processes a string received from wireless

protected:
	//! The input command stream socket
	Socket *cmdsock;
	int sock; //!< the socket number so we can test if #cmdsock is still valid

	//! contains parsing context regarding communication socket
	void* ctxt;
	//! set to true by sax routines if the socket should be closed
	bool docClosed;

	//! The estop to control
	MotionManager::MC_ID pose_id;

	//! fills out (and returns) a xmlSAXHandler structure to control parsing of the communication stream
	static void* getSAXHandler();

	//!@name SAX functions
	//! handles parsing of communications stream
	static void sax_start_element(StewartPlatformBehavior * inst, const char * name, const char ** attrs);
	static void sax_end_element(StewartPlatformBehavior * inst, const char * name);
	static void sax_end_doc(StewartPlatformBehavior * inst);
	static void sax_warning(StewartPlatformBehavior *inst, const char *msg, ...);
	static void sax_error(StewartPlatformBehavior *inst, const char *msg, ...);
	static void sax_fatal_error(StewartPlatformBehavior *inst, const char *msg, ...);
	//@}

	static bool isConnected(); //!< returns true if #theOne is connected
	void closeComm(); //!< closes communications socket

private:
	StewartPlatformBehavior(const StewartPlatformBehavior&); //!< don't call
	StewartPlatformBehavior operator=(const StewartPlatformBehavior&); //!< don't call

};

/*! @file
 * @brief Describes StewartPlatformBehavior, moves the legs in synchrony to emulate the capabilities of a stewart platform
 * @author tss (Creator)
 * 
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.4 $
 * $State: Exp $
 * $Date: 2007/01/30 22:56:19 $
 */

#endif 
