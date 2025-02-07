//-*-c++-*-
#ifndef INCLUDED_SoundPlayThread_h_
#define INCLUDED_SoundPlayThread_h_

#include "IPC/PollThread.h"

class SoundPlayThread : public PollThread {
public:
	static const long BUFFER_TIME=32;
	static const unsigned int NUM_BUFFERS=2;
	SoundPlayThread() : PollThread(0L,BUFFER_TIME,true,false), buf(NULL), bufsize(0), buffersInFlight(0) {}
	virtual ~SoundPlayThread() { delete buf; buf=NULL; }
	
	void reset();
	
protected:
	virtual bool launched();
	virtual bool poll();
	
	char * buf;
	size_t bufsize;
	unsigned int buffersInFlight;
	
private:
	SoundPlayThread(const SoundPlayThread& l); //!< don't call
	SoundPlayThread& operator=(const SoundPlayThread& l); //!< don't call
};

/*! @file
 * @brief 
 * @author Ethan Tira-Thompson (ejt) (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Date: 2007/02/16 20:18:23 $
 */

#endif
