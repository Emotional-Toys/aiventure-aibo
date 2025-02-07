//-*-c++-*-
#ifndef INCLUDED_IPCMotionHook_h_
#define INCLUDED_IPCMotionHook_h_

#include "local/MotionHook.h"
#include <list>

class MessageQueueBase;
class RCRegion;

//! description of IPCMotionHook
class IPCMotionHook : public MotionHook {
public:
	IPCMotionHook(MessageQueueBase& q) : MotionHook(), mq(q), regions() {}
	virtual ~IPCMotionHook();
	
	virtual void motionCheck(const float outputs[][NumOutputs]);
	
protected:
	MessageQueueBase& mq;

	RCRegion* getRegion();
	typedef std::list<RCRegion* > msgbuf_t; //!< type of collection of shared data regions
	msgbuf_t regions; //!< for efficiency, reuse old buffers -- oldest at front, most recently used at back
};

/*! @file
 * @brief 
 * @author Ethan Tira-Thompson (ejt) (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.1 $
 * $State: Exp $
 * $Date: 2007/06/03 17:03:26 $
 */

#endif
