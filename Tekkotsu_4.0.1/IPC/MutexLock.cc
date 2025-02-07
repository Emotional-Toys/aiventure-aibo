#include "MutexLock.h"

unsigned int MutexLockBase::usleep_granularity=5000;

#if !defined(PLATFORM_APERIOS) && TEKKOTSU_SHM_STYLE!=NO_SHM
SemaphoreManager MutexLockBase::preallocated;
SemaphoreManager* MutexLockBase::semgr=&preallocated;
#endif

/*! @file 
* @brief Defines MutexLock, a software only mutual exclusion lock.
* @author ejt (Creator), Edward A. Lycklama, Vassos Hadzilacos (paper from which this was based)
*
* $Author: ejt $
* $Name: tekkotsu-4_0-branch $
* $Revision: 1.5 $
* $State: Exp $
* $Date: 2007/06/14 06:22:29 $
*/
