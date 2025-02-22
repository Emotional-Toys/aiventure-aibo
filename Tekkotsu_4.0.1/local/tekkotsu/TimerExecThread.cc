#include "TimerExecThread.h"
#include "SharedGlobals.h"
#include "Shared/ProjectInterface.h"
#include "Shared/MarkScope.h"
#include "Shared/get_time.h"
#include "Events/EventRouter.h"

//better to put this here instead of the header
using namespace std; 


void TimerExecThread::reset() {
	if((globals->getNextTimer()==-1U || globals->timeScale<=0) && isStarted())
		stop();
	else if(globals->timeScale>0 && !isStarted())
		start();
	else if(isStarted()) {
		interrupt();
	}
}

long TimerExecThread::calcSleepTime() {
	startTime.Set();
	return static_cast<long>((globals->getNextTimer()-get_time())/globals->timeScale);
}

bool TimerExecThread::launched() {
	if(globals->timeScale<=0)
		return false;
	delay=calcSleepTime();
	return PollThread::launched();
}

bool TimerExecThread::poll() {
	MarkScope bl(behaviorLock);
	//cout << "Poll at " << get_time() << " next timer " << globals->getNextTimer() << " (vs. " << erouter->getNextTimer() << ")" << endl;
	//this happens normally:
	//ASSERT(get_time()>=globals->getNextTimer(),"TimerExecThread::poll() early (time="<<get_time()<< " vs. nextTimer=" <<globals->getNextTimer()<<")");
	try {
		erouter->processTimers();
	} catch(const std::exception& ex) {
		if(!ProjectInterface::uncaughtException(__FILE__,__LINE__,"Occurred during timer processing",&ex))
			throw;
	} catch(...) {
		if(!ProjectInterface::uncaughtException(__FILE__,__LINE__,"Occurred during timer processing",NULL))
			throw;
	}
	globals->setNextTimer(erouter->getNextTimer());
	if(globals->getNextTimer()==-1U)
		return false;
	period=calcSleepTime();
	return true;
}

void TimerExecThread::interrupted() {
	delay=calcSleepTime();
}


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
