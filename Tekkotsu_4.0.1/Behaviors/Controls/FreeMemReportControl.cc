#include "FreeMemReportControl.h"
#ifdef PLATFORM_APERIOS
#  include <AnalyzerAPI.h>
#  include <AnalyzerError.h>
#endif

void FreeMemReportControl::processEvent(const EventBase& e) {
	ASSERTRET(e.getGeneratorID()==EventBase::timerEGID,"Unexpected event");
	if(e.getSourceID()==0) {
		report();
	} else if(e.getSourceID()==1) {
		size_t freemem=freeMem();
		if(freemem<low_mem) {
			if(!isWarning) {
				isWarning=true;
				serr->printf("WARNING: Low memory: %lu\n",(unsigned long)freemem);
			}
		} else {
			if(isWarning) {
				isWarning=false;
				report();
			}
		}
	} else
		ASSERT(false,"Unexpected timer event");
}
	
void FreeMemReportControl::refresh() {
	char tmp[256];
	sprintf(tmp,"Free Mem: %lu",(unsigned long)freeMem());
	options[1]->setName(tmp);
	ControlBase::refresh();
	report();
}


//! reports size of free memory - if this is below low_mem, also generates a warning
void FreeMemReportControl::report() {
	size_t freemem=freeMem();
	sout->printf("%lu bytes free (%+ld)\n",(unsigned long)freemem,(long)(freemem-lastReport));
	lastReport=freemem;
	if(freemem<low_mem)
		if(isWarning)
			serr->printf("WARNING: Low memory: %lu\n",(unsigned long)freemem);
	resetTimerFreq();
}

//! returns the size of the free memory
size_t FreeMemReportControl::freeMem() {
	size_t freemem;
#ifdef PLATFORM_APERIOS
	if (AnalyzerGetSizeOfFreeMemory(&freemem)!=azrSUCCESS)
		sout->printf("Aperios error: getsizeoffreememory failed\n");
#else
	freemem=-1U;
#endif
	return freemem;
}

void FreeMemReportControl::resetTimerFreq() {
	if(report_freq<0)
		erouter->removeTimer(this,0);
	else
		erouter->addTimer(this,0,report_freq,true);
	erouter->addTimer(this,1,monitor_freq,true);
}

/*! @file
 * @brief Implements FreeMemReportControl, which gives reports on free memory size at various (configurable) rates
 * @author ejt (object), alokl (core function)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.7 $
 * $State: Exp $
 * $Date: 2006/09/25 23:26:56 $
 */

