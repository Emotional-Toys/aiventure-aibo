//-*-c++-*-
#ifndef INCLUDED_EntryPoint_h_
#define INCLUDED_EntryPoint_h_

#include "Shared/Resource.h"
#include "Shared/WorldStatePool.h"
#include "Shared/debuget.h"
#include <list>
#include <typeinfo>

//! manages a thread lock to serialize behavior computation and mark whether ::state is being read or written
class EntryPoint : public Resource {
public:
	//! pass this to MarkScope when using an EntryPoint over a section which will read ::state
	class WorldStateRead : public WorldStatePool::ReadRequest {
	public:
		//! constructor, hardcoded to use global ::state and non-blocking pool access
		WorldStateRead() : WorldStatePool::ReadRequest(state,false) {}
	};
	
	//! pass this to MarkScope when using an EntryPoint over a section which will update ::state
	class WorldStateWrite : public WorldStatePool::WriteRequest {
	public:
		//! constructor, hardcoded to use global ::state and blocking pool access
		explicit WorldStateWrite(unsigned int frame_number) : WorldStatePool::WriteRequest(state,true,frame_number) {}
	};
	
	//! constructor, need to specify the WorldStatePool (presumably it's in a shared memory region...)
	explicit EntryPoint() : Resource(), pool(NULL), epFrame(NULL), data() {}
	
	//! sets the WorldStatePool to use
	void setPool(WorldStatePool* wspool);
	
	//! an EmptyData implies a WorldStateRead should be passed on to the pool, requesting a write requires a WorldStateWrite to be passed
	virtual void useResource(Data& d);
	//! an EmptyData implies a WorldStateRead should be passed on to the pool, requesting a write requires a WorldStateWrite to be passed
	virtual void releaseResource(Data& d);
	
protected:
	WorldStatePool* pool; //!< pool which manages which WorldStates are being updated while old copies can still be read
	stacktrace::StackFrame * epFrame; //!< stores root stack frame so ProcessID::getID can tell processes apart through virtual calls on shared memory regions
	std::list<Data*> data; //!< data supplied when resource was marked used
	
private:
	EntryPoint(const EntryPoint&); //!< not implemented
	EntryPoint operator=(const EntryPoint&); //!< not implemented
};


/*! @file
* @brief 
* @author Ethan Tira-Thompson (ejt) (Creator)
*
* $Author: ejt $
* $Name: tekkotsu-4_0-branch $
* $Revision: 1.1 $
* $State: Exp $
* $Date: 2006/08/22 22:23:05 $
*/

#endif
