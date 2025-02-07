//-*-c++-*-
#ifndef INCLUDED_EntryPoint_h_
#define INCLUDED_EntryPoint_h_

#include "IPC/Thread.h"
#include "Shared/Resource.h"
#include "Shared/WorldStatePool.h"

//! manages a thread lock to serialize behavior computation and mark whether ::state is being read or written
class EntryPoint : public Resource {
public:
	//! pass this to MarkScope when using an EntryPoint over a section which will read ::state
	class WorldStateRead : public WorldStatePool::ReadRequest {
	public:
		//! constructor, hardcoded to use global ::state and non-blocking pool access
		WorldStateRead() : WorldStatePool::ReadRequest(state,false) {}
	};
	WorldStateRead defaultRead; //!< this instance will be used if an empty Data is passed to useResource (only safe to do because we get #lock first, so two threads won't be using the same data at the same time)
	
	//! pass this to MarkScope when using an EntryPoint over a section which will update ::state
	class WorldStateWrite : public WorldStatePool::WriteRequest {
	public:
		//! constructor, hardcoded to use global ::state and blocking pool access
		explicit WorldStateWrite(unsigned int frame_number) : WorldStatePool::WriteRequest(state,true,frame_number) {} 
	};
	
	//! constructor, need to specify the WorldStatePool (presumably it's in a shared memory region...)
	explicit EntryPoint(WorldStatePool& wspool) : Resource(), defaultRead(), pool(wspool), lock() {}
	
	//! an EmptyData implies a WorldStateRead should be passed on to the pool, requesting a write requires a WorldStateWrite to be passed
	virtual void useResource(Data& d) {
		static_cast<Resource&>(lock).useResource(emptyData); //important to get lock first to make sure using shared defaultRead is safe in multi-threaded env.
		pool.useResource(typeid(d)==typeid(Data) ? defaultRead : d);
	}
	//! an EmptyData implies a WorldStateRead should be passed on to the pool, requesting a write requires a WorldStateWrite to be passed
	virtual void releaseResource(Data& d) {
		if(WorldStateWrite * wsw=dynamic_cast<WorldStateWrite*>(&d)) {
			if(wsw->getComplete() && state!=wsw->src) {
				/*for(unsigned int i=0; i<NumPIDJoints; i++) {
				pids[i][0]=lastState->pids[i][0];
				pids[i][1]=lastState->pids[i][1];
				pids[i][2]=lastState->pids[i][2];
				}*/
				memcpy(state->pids,wsw->src->pids,sizeof(state->pids)); //pids is probably big enough it's better to use memcpy (?)
				state->vel_x=wsw->src->vel_x;
				state->vel_y=wsw->src->vel_y;
				state->vel_a=wsw->src->vel_a;
				state->vel_time=wsw->src->vel_time;
			}
		}
		pool.releaseResource(typeid(d)==typeid(Data) ? defaultRead : d);
		static_cast<Resource&>(lock).releaseResource(emptyData); //important to release lock last to make sure using shared defaultRead is safe in multi-threaded env.
	}
	
	//! this can be useful when planning to write, get the threadlock to do some initial setup before grabbing an entry from the pool
	ThreadNS::Lock& getLock() { return lock; }
	
protected:
	WorldStatePool& pool; //!< pool which manages which WorldStates are being updated while old copies can still be read
	ThreadNS::Lock lock; //!< only one behavior runs at a time
};


/*! @file
 * @brief 
 * @author Ethan Tira-Thompson (ejt) (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.1 $
 * $State: Exp $
 * $Date: 2006/09/28 20:42:51 $
 */

#endif
