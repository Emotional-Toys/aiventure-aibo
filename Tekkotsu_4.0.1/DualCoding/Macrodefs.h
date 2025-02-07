//-*-c++-*-
#ifndef _MACRODEFS_H_
#define _MACRODEFS_H_

#define DEBUG_INFORMATION_VERBOSE 1  // <--- change this to control debug info

#if DEBUG_INFORMATION_VERBOSE == 1
# include "Wireless/Wireless.h"
# define PRINTF(args, ...) serr->printf(args, ## __VA_ARGS__)
#else
# define PRINTF(args, ...)
#endif

#define SGN(a)		(((a)<0) ? -1 : 1)

#endif
