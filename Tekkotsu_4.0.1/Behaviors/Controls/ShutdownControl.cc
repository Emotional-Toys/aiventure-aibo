#include "ShutdownControl.h"
#ifdef PLATFORM_APERIOS
#  include <OPENR/OPENRAPI.h>
#else
#  include "Shared/ProjectInterface.h"
#endif

ControlBase * ShutdownControl::doSelect()    {
#ifdef PLATFORM_APERIOS
	OBootCondition bc(0);
	OPENR::Shutdown(bc);
#else
	ProjectInterface::sendCommand("quit");
#endif
	return NULL;
}

/*! @file
 * @brief Implements ShutdownControl, which causes the aibo to shutdown (very short - one function separated out to limit recompile of the OPENR headers)
 * @author ejt (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.6 $
 * $State: Exp $
 * $Date: 2007/06/13 18:14:12 $
 */

