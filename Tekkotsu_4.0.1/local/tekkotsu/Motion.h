//-*-c++-*-
#ifndef INCLUDED_Motion_h_
#define INCLUDED_Motion_h_

#include "Process.h"
#include "sim.h"
#include "IPC/ProcessID.h"
#include "IPC/SharedObject.h"
#include "SharedGlobals.h"
#include "Motion/MotionManager.h"
#include "Sound/SoundManager.h"
#include "Shared/WorldStatePool.h"
#include "Shared/Profiler.h"
#include "local/EntryPoint.h"
#include <list>

class Motion : public Process {
public:
	//! constructor
	Motion();
	//! destructor
	~Motion();

	virtual void DoStart();
	virtual void run();
	virtual void DoStop();

	static const char * getClassName() { return "Motion"; }
	static ProcessID::ProcessID_t getID() { return ProcessID::MotionProcess; }
	
	static const char * getMotionCommandID() { return "MotionCommands"; }
	static const char * getMotionOutputID() { return "MotionOutput"; }
	static const char * getMotionManagerID() { return "MotionManager"; }
	static const char * getMotionProfilerID() { return "MotionProfiler"; }

protected:
	SharedObject<sim::SoundPlayQueue_t> sounds;
	SharedObject<sim::MotionCommandQueue_t> motions;
	SharedObject<sim::MotionOutput_t> motionout;
	SharedObject<sim::EventQueue_t> events;
	SharedObject<sim::SensorQueue_t> sensorFrames;
	SharedObject<sim::StatusRequest_t> statusRequest;
	SharedObject<MotionManager> motionmanager;
	SharedObject<SoundManager> soundmanager;
	SharedObject<WorldStatePool> worldstatepool;
	SharedObject<sim::MotionWakeup_t> motionWakeup;
	SharedObject<motionProfiler_t> motionProf;
	
	IPCEventTranslator * etrans;
	class MessageReceiver * sensrecv;
	class MessageReceiver * statusrecv;
	class MessageReceiver * wakeuprecv;
	class MotionExecThread * motionExec;
	class IPCMotionHook * motionfwd;
	WirelessThread wireless_thread;

	EntryPoint motionLock;
	
	static bool gotSensors(RCRegion* msg);
	static bool gotWakeup(RCRegion* msg);
	unsigned int lastSensorSN;
	
private:
	Motion(const Motion&); //!< don't call (copy constructor)
	Motion& operator=(const Motion&); //!< don't call (assignment operator)
};

/*! @file
 * @brief Defines Motion, which DESCRIPTION
 * @author ejt (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.1 $
 * $State: Exp $
 * $Date: 2007/06/14 15:37:41 $
 */

#endif
