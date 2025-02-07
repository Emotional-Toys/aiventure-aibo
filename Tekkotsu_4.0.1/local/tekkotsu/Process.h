//-*-c++-*-
#ifndef INCLUDED_Process_h_
#define INCLUDED_Process_h_

#include "IPC/ProcessID.h"
#include <string>

class RCRegion;

//! Represents a common interface for each process being run
class Process {
public:
	Process(ProcessID::ProcessID_t pid, const std::string& pname);
	virtual ~Process();
	virtual void DoStart() {}
	virtual void DoStop() {}
	virtual void run();

	static const char* getName() { return ProcessID::getIDStr(); }
	static Process * getCurrent() { return procs[ProcessID::getID()]; }
	
	virtual void statusReport(std::ostream& os);
	static bool statusReport(RCRegion* msg);

protected:
	static Process* procs[ProcessID::NumProcesses];
	
private:
	Process(const Process&);            //!< don't call
	Process& operator=(const Process&); //!< don't call
};

/*! @file
 * @brief 
 * @author ejt (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.1 $
 * $State: Exp $
 * $Date: 2007/06/14 15:37:42 $
 */

#endif
