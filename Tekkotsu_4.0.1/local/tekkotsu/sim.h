//-*-c++-*-
#ifndef INCLUDED_sim_h_
#define INCLUDED_sim_h_

#include "Events/EventTranslator.h"
#include "IPC/MessageQueue.h"
#include "SharedGlobals.h"
#include "IPC/SharedObject.h"
#include "IPC/Thread.h"
#include "Wireless/Wireless.h"
#include "Shared/ProjectInterface.h"
#include <vector>
class SimConfig;

class sim {
	friend int main(int argc, const char* argv[]);
public:
	sim();
	bool processCommandLine(int argc, const char* argv[]);
	bool run();
	~sim();
	
	typedef MessageQueue<50> EventQueue_t;
	typedef MessageQueue<50> MotionCommandQueue_t;
	typedef MessageQueue<50> SoundPlayQueue_t;
	typedef MessageQueue<1> CameraQueue_t;
	typedef MessageQueue<1> SensorQueue_t;
	typedef MessageQueue<1> TimerWakeup_t;
	typedef MessageQueue<1> MotionWakeup_t;
	typedef MessageQueue<ProcessID::NumProcesses+1> StatusRequest_t;
	typedef MessageQueue<1> MotionOutput_t;
	
	static SimConfig config;
	static IPCEventTranslator * etrans;
	static std::vector<std::string> cmdlineArgs;
	
protected:
	static unsigned int measure_usleep_cost();
	static void wait_runlevel(SharedGlobals::runlevel_t level);
	template<class T> static void manage_process();
	template<class T> static bool fork_process();
	
	static ProcessID::ProcessID_t getProcessID(); //!< ProcessID hook to use the thread group as the process ID
	static void setProcessID(ProcessID::ProcessID_t id); //!< ProcessID hook to set the thread group as the process ID
	
	//! a wrapper for one of the 'major' processes, which would otherwise be forked as its own process if SimConfig::multiprocess was true
	template<typename T>
	class PrimaryThread : public Thread {
	public:
		PrimaryThread() : Thread() {}
		virtual void* run() { sim::manage_process<T>(); return NULL; }
	};
	static std::vector<Thread*> primaries;
	
	//! provides warning regarding configuration settings
	class ConfigErrorCheck : public plist::PrimitiveListener {
	public:
		ConfigErrorCheck();
		~ConfigErrorCheck();
		virtual void plistValueChanged(const plist::PrimitiveBase& pl);
		void holdMultiprocess();
	protected:
		bool holdMPValue;
	};
	static ConfigErrorCheck cfgCheck;

	static unsigned int sim_get_time() { return globals?globals->get_time():0; }
	static float sim_getTimeScale() { return globals?globals->getTimeScale():0; }
	
	static void handle_signal(int sig);
	static void handle_exit();

	SharedObject<SemaphoreManager> mutexSemMgr;
	SharedObject<SharedGlobals> glob;
	SharedObject<ipc_setup_t> subj;
	static bool original;
	static const char * const usage;
	static pid_t child;
};

class WirelessThread : public Thread {
public:
	//! constructor
	WirelessThread() : Thread() {}
	//! destructor -- stop thread
	virtual ~WirelessThread() {
		if(isStarted()) {
			stop();
			join();
		}
	}
	virtual unsigned int runloop() {
		// initial setup done by wireless's own constructor
		wireless->pollTest(NULL); // by passing NULL, we wait indefinitely, so no need to usleep in the Thread code
		wireless->pollProcess();
		wireless->pollSetup(); // reinitialize for next test
		return 0; //no sleep time because pollTest blocks
	}
	virtual void stop() {
		Thread::stop();
		wireless->wakeup();
	}
};


/*! @file
 * @brief 
 * @author ejt (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Date: 2007/10/12 16:55:05 $
 */

#endif
