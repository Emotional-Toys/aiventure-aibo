//-*-c++-*-
#ifndef INCLUDED_CommonERSInfo_h
#define INCLUDED_CommonERSInfo_h

#include <math.h>
#ifndef PLATFORM_APERIOS
typedef unsigned short word; //!< otherwise defined in Types.h
#else
#include <Types.h>
#endif

#include "CommonInfo.h"
using namespace RobotInfo;

// see http://tekkotsu.org/porting.html#configuration for more information on TGT_HAS_* flags

#if defined(TGT_ERS210) || defined(TGT_ERS220) || defined(TGT_ERS2xx) || defined(TGT_ERS7)

//! Should only be used to specialize/optimize for the Aibo
#  define TGT_IS_AIBO

#	define TGT_HAS_LEGS 4
#	define TGT_HAS_LED_PANEL 1
#	define TGT_HAS_ACCELEROMETERS 3
#	ifndef TGT_ERS220
#		define TGT_HAS_MOUTH 1
#		define TGT_HAS_TAIL 1
#	endif
#	define TGT_HAS_POWER_STATUS
#	define TGT_HAS_CAMERA 1
#	define TGT_HAS_HEAD 1
#	define TGT_HAS_MICROPHONE 2
#	define TGT_HAS_SPEAKERS 1

#endif

/*! @file
 * @brief Defines some capabilities common to all Aibo robots
 * @author ejt (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.11 $
 * $State: Exp $
 * $Date: 2007/11/09 18:08:29 $
 */

#endif
