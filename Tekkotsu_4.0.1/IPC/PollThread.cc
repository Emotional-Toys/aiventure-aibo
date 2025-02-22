#ifndef PLATFORM_APERIOS
#include "PollThread.h"
#include <errno.h>
#include <signal.h>

//better to put this here instead of the header
using namespace std; 

void PollThread::start() {
	initialPoll=true;
	startTime.Set();
	Thread::start();
}

bool PollThread::poll() {
	unsigned int sleeptime=runloop();
	if(sleeptime==-1U)
		return false;
	period.Set(static_cast<long>(sleeptime));
	return true;
}

void PollThread::interrupted() {
	if(!initialPoll) {
		if(period-startTime.Age()<1L) {
			delay=0L;
		} else {
			while(!(period-startTime.Age()<1L))
				startTime-=period;
			startTime+=period;
			delay=period;
		}
	}
}

void * PollThread::run() {
	timespec sleepSpec,remainSpec;
	if(startTime.Age()<delay) {
		sleepSpec=delay-startTime.Age();
		while(nanosleep(&sleepSpec,&remainSpec)) {
			if(errno!=EINTR) {
				perror("PollThread::run(): initial nanosleep");
				break;
			}
			testCancel();
			//interrupted() may have changed delay to indicate remaining sleep time
			if(delay<1L || delay<startTime.Age())
				break;
			sleepSpec=delay-startTime.Age();
		}
	}
	testCancel();
	if(!poll())
		return returnValue;
	initialPoll=false;
	bool wasInterrupted=true; // need to add delay instead of period for the first time, same as if an interrupt occurred
	for(;;) {
		bool noSleep=false;
		if(TimeET(0,0)<period) {
			if(trackPollTime) {
				if(wasInterrupted) {
					if(delay<0L || startTime.Age()<delay) {}
					else
						startTime+=delay;
				} else {
					if(startTime.Age()<period) {}
					else
						startTime+=period;
				}
				//the idea with this part is that if we get behind, (because of poll() taking longer than period)
				// then we want to quick poll again as fast as we can to catch up, but we don't want to backlog
				// such that we'll be quick-calling multiple times once we do catch up
				if(period<startTime.Age()) {
					noSleep=true;
					while(period<startTime.Age())
						startTime+=period;
					startTime-=period; //back off one -- the amount we've overshot counts towards the next period
				}
				sleepSpec=period-startTime.Age();
			} else {
				sleepSpec=period;
				startTime.Set();
			}
			wasInterrupted=false;
			while(!noSleep && nanosleep(&sleepSpec,&remainSpec)) {
				wasInterrupted=true;
				if(errno!=EINTR) {
					perror("PollThread::run(): periodic nanosleep");
					break;
				}
				testCancel();
				//interrupted() should have changed delay and/or period to indicate remaining sleep time
				if(delay<1L || delay<startTime.Age())
					break;
				sleepSpec=delay-startTime.Age();
			}
		} else {
			startTime.Set();
		}
		testCancel();
		if(!poll())
			return returnValue;
	}
	// this return is just to satisfy warnings with silly compiler
	return returnValue; //never happens -- cancel or bad poll would exit
}

//void PollThread::cancelled() {
	//signal handlers are global, not per-thread, so if we reset the handler, then the next interrupt will cause the program to end :(
	//signal(SIGALRM,SIG_DFL);
//}



/*! @file
 * @brief 
 * @author Ethan Tira-Thompson (ejt) (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.14 $
 * $State: Exp $
 * $Date: 2007/10/12 16:55:04 $
 */
#endif
