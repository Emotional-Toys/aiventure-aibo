//-*-c++-*-
#ifndef INCLUDED_MotionExecThread_h_
#define INCLUDED_MotionExecThread_h_

#include "IPC/PollThread.h"
#include "Shared/RobotInfo.h"
#include "IPC/MessageQueue.h"
#include "Shared/get_time.h"
#include "Motion/PostureEngine.h"
#include "SharedGlobals.h"
#include <list>
#include <fcntl.h>

#include "local/MotionHook.h"

class RCRegion;
class EntryPoint;

//! description of MotionExecThread
class MotionExecThread : public PollThread {
public:
	//! constructor, enables trackPollTime, but not auto-start (call reset() when you're ready to start it)
	/*! @arg bl a process lock to ensure mutual exclusion between MotionExecThread::poll() and other threads in the process */
	MotionExecThread(EntryPoint& bl)
		: PollThread(0L, FrameTime*NumFrames/globals->timeScale/1000, true,false), motionLock(bl),
		motionBuffers(), motionBufferPos(), curPose(), lastPoll(-1U)
	{
		motionBuffers.push_front(new float[NumFrames][NumOutputs]);
		for(unsigned int f=0; f<NumFrames; ++f)
			for(unsigned int o=0; o<NumOutputs; ++o)
				motionBuffers.front()[f][o]=0;
		motionBufferPos=motionBuffers.begin();
		curPose.setWeights(1);
	}
	virtual ~MotionExecThread() {
		if(isStarted()) {
			stop();
			join();
		}
		while(motionBuffers.size()>0) {
			delete [] motionBuffers.front();
			motionBuffers.pop_front();
		}
	}
	
	virtual void reset(); //!< starts and stops thread as needed, or interrupts thread to reset sleep time if already running
	
	//virtual void start();
	virtual bool poll();

	//! returns time (in milliseconds) of next motion frame -- multiples of FrameTime*NumFrames
	static unsigned int getNextMotion() {
		unsigned int pd=FrameTime*NumFrames;
		return (get_time()/pd+1)*pd;
	}

	//! returns a posture storing the "current" output values, delayed by SharedGlobals::MotionSimConfig::feedbackDelay
	const PostureEngine& getPostureFeedback();
	
protected:
	virtual bool launched();
	//! resets PollThread::delay and PollThread::period to appropriate values for current SharedGlobals::timeScale value
	virtual void interrupted();
	
	EntryPoint& motionLock; //!< a lock on the motions which should be obtained before updating

	std::list<float(*)[NumOutputs]> motionBuffers;
	std::list<float(*)[NumOutputs]>::iterator motionBufferPos;
	PostureEngine curPose;
	
	unsigned int lastPoll;
};

/*! @file
 * @brief 
 * @author Ethan Tira-Thompson (ejt) (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Date: 2007/10/12 16:55:05 $
 */

#endif
