//-*-c++-*-
#ifndef INCLUDED_WorldStatePool_h_
#define INCLUDED_WorldStatePool_h_

#include "WorldState.h"
#include "IPC/MutexLock.h"
#include "IPC/ListMemBuf.h"
#include "Shared/Resource.h"
#include "Motion/PostureEngine.h"
#include <stdexcept>

class RCRegion;

#ifndef WORLDSTATEPOOL_NUM_STATES
//! provides default value for WorldStatePool::NUM_STATES, allows you to override from build settings, without touching source
#define WORLDSTATEPOOL_NUM_STATES 3
#endif

//! holds multiple instances of WorldState, allows one process to be updating while another is reading
/*! 
Use the AutoGetReadState and AutoGetWriteState to access individual WorldState entries... their
constructors and destructors allow WorldStatePool to keep track of which entries are in use.

When a state wants to write, it is given the oldest unused entry to write into.  During writing,
other accessors can read the newest (complete) entry, or concurrently write into a different
entry (in case they don't want to wait for the other process to finish).

A global lock (#lock) is used while choosing an entry, and individual locks (#writeLocks) are
used while writing into entries (to easily allow readers to block on the lock until writing is done)

One point of trickiness is that entries being written are moved to the end of the list when
the writing begins, not when it is complete.  Readers can always scan backwards in the list
to find the newest entries, but writers must check the end to see if newer (or equivalent)
frames are already in progress, as well as the beginning to find the oldest unreferenced.

When a writer tries to access an entry, and another writer is already processing that frame,
if blocking is set then the writer will given that entry once the original is done with it (so
check your frame index when you receive it so you can tell if it was already processed).
If blocking is *not* set, then you will get a separate entry with no indication another process
is also working on the same frame.
*/
class WorldStatePool : public Resource {
public:
	//! common base class for ReadRequest or WriteRequest
	class Request : public Resource::Data {
	protected:
		//! constructor, sets the WorldState point to be assigned, whether to block, and whether is an instance of ReadRequest
		/*! @a wantRead is because we can't trust RTTI (i.e. dynamic_cast) to work correctly on Aperios :( */
		Request(WorldState*& target, bool block, bool wantRead) : Resource::Data(),
			bufUsed(-1U), tgt(target), prev(NULL), bl(block), depth(0), isRead(wantRead)
		{}
		//! shallow copy constructor supported
		Request(const Request& r) : Resource::Data(r), bufUsed(r.bufUsed), tgt(r.tgt), prev(r.prev), bl(r.bl), depth(r.depth), isRead(r.isRead) {}
		//! shallow assignment supported
		Request& operator=(const Request& r) { bufUsed=r.bufUsed; tgt=r.tgt; prev=r.prev; bl=r.bl; depth=r.depth; Resource::Data::operator=(r); return *this; }
		
	public:
		unsigned int bufUsed; //!< the entry index used
		WorldState*& tgt; //!< reference to pointer, which is set to an element of the pool when the request goes through
		WorldState* prev; //!< stores previous value at #tgt so it can be restored upon release (needed to support recursive usage)
		bool bl; //!< whether to block if a write is in progress, or use most recent "complete" entry
		unsigned int depth; //!< supports recursive read requests
		bool isRead; //!< true if instance is a read request
	};
	//! retrieves the current WorldState entry, and through it's destructor, marks the entry available again when it goes out of scope
	class ReadRequest : public Request {
	public:
		//! stores the current completed WorldState into #tgt, optionally blocking if an update is in progress (otherwise it returns the previous completed entry)
		ReadRequest(WorldState*& target, bool block) : Request(target,block,true) {}
		//! shallow copy constructor supported
		ReadRequest(const ReadRequest& r) : Request(r) {}
		//! shallow assignment supported
		ReadRequest& operator=(const ReadRequest& r) { Request::operator=(r); return *this; }
	};
	//! retrieves the current WorldState entry, and through it's destructor, marks the entry available again when it goes out of scope
	/*! By default, when the release occurs, the entry is marked "complete" unless you have called setComplete(false) before then */
	class WriteRequest : public Request {
	public:
		//! stores the oldest unreferenced WorldState into #tgt, optionally blocking if an update is currently in progress for the same frame
		WriteRequest(WorldState*& target, bool block, unsigned int frame_number) : Request(target,block,false),
			src(NULL), srcRequest(src,false), frame(frame_number), statusCache(0), completeCache(false)
		{}
		//! shallow copy constructor supported
		WriteRequest(const WriteRequest& r) : Request(r), src(r.src), srcRequest(r.srcRequest), frame(r.frame), statusCache(r.statusCache), completeCache(r.completeCache) {}
		//! shallow assignment supported
		WriteRequest& operator=(const WriteRequest& r) { src=r.src; srcRequest=r.srcRequest; frame=r.frame; statusCache=r.statusCache; completeCache=r.completeCache; Request::operator=(r); return *this; }
		
		WorldState* src; //!< will be set to the previously written element, so you can copy over unmodified values
		ReadRequest srcRequest; //!< used to get #src
		unsigned int frame; //!< should be initialized to the frame number about to be written
		
		unsigned int getStatus() const { return statusCache; } //!< returns the WorldStatePool::status value for the target WorldState entry (see documentation for WorldStatePool::status)
		void setStatus(unsigned int status) { statusCache=status; } //!< sets the WorldStatePool::status value for the target WorldState entry (see documentation for WorldStatePool::status)
		bool getComplete() const { return completeCache; } //!< returns the WorldStatePool::complete value for the target WorldState entry (see documentation for WorldStatePool::complete)
		void setComplete(bool complete) { completeCache=complete; } //!< returns the WorldStatePool::complete value for the target WorldState entry (see documentation for WorldStatePool::complete)
		
	protected:
		unsigned int statusCache; //!< when using resource, this field is set to the status field for that entry, and when released, this value is stored back
		bool completeCache; //!< when using resource, this field is set to the complete flag for that entry, and when released, this value is stored back
	};
	
	//! constructor
	WorldStatePool();
	
#ifdef PLATFORM_APERIOS
	//! returned by isUnread containing information parsed from the incoming message
	class UpdateInfo {
	public:
		//! constructor, sets #msg to NULL, #frameNumber to -1U
		UpdateInfo()
#ifdef DEBUG
		: msg(NULL), frameNumber(-1U), intendedBuf(-1U) {}
#else
		: msg(NULL), frameNumber(-1U) {}
#endif
		OSensorFrameVectorData* msg; //!< incoming data
		unsigned int frameNumber; //!< serial number of the message
#ifdef DEBUG
		unsigned int intendedBuf; //!< the write buffer read() is expected to use;  This is for debugging, if this isn't the buffer selected, display warning
#endif
	private:
		UpdateInfo(const UpdateInfo&); //!< not implemented
		UpdateInfo operator=(const UpdateInfo&); //!< not implemented
	};
	
	//! returns true if the process should call WorldState::read (i.e. @a msg has new or unprocessed data (such as motion needs to supply feedback))
	/*! only one call to this can be made at a time per process (not threadsafe, but is multi-process safe)
		*  @param msg the incoming sensor data from the system -- should be const, but accessor functions from Sony aren't marked const themselves :-/
		*  @param[out] lastFrameNumber if the incoming frame is already complete (no need to read), then the frame's number will be assigned here
		*  @return returns a static UpdateInfo structure (to be passed to read()) if the message is unread, otherwise returns NULL. The structure is static -- DO NOT DELETE IT */
	WorldStatePool::UpdateInfo* isUnread(OSensorFrameVectorData& msg, unsigned int& lastFrameNumber);
		
#else //PLATFORM_LOCAL
	//! flags for the status field on each WriteRequest -- tracks partial completion when multiple writers are involved
	enum status_t {
		SENSORS_APPLIED=1<<0, //!< bit flag signals sensor data has been applied to the write target
		FEEDBACK_APPLIED=1<<1 //!< bit flag signals motion feedback has been applied to the write target (only required if feedback is being generated)
	};
	
	//! returned by isUnread containing information parsed from the incoming message
	class UpdateInfo {
	public:
		//! constructor
		UpdateInfo()
#ifdef DEBUG
			: verbose(0), frameNumber(-1U), filename(), dataInQueue(false), payload(NULL), payloadSize(0), intendedBuf(-1U) {}
#else
			: verbose(0), frameNumber(-1U), filename(), dataInQueue(false), payload(NULL), payloadSize(0) {}
#endif
		unsigned int verbose; //!< status of processing the message should be displayed
		unsigned int frameNumber; //!< serial number of the message
		std::string filename; //!< source of the data in the message
		bool dataInQueue; //!< sender indicates data is in the queue (if this is heartbeat, treat it as data)
		char* payload; //!< pointer to beginning of the data (NULL if no data available, i.e. heartbeat message)
		unsigned int payloadSize; //!< size of data (0 if no data available, i.e. heartbeat message)
#ifdef DEBUG
		unsigned int intendedBuf; //!< the write buffer read() is expected to use;  This is for debugging, if this isn't the buffer selected, display warning
#endif
	private:
		UpdateInfo(const UpdateInfo&); //!< not implemented
		UpdateInfo operator=(const UpdateInfo&); //!< not implemented
	};
	
	//! returns true if the process should call read (i.e. @a msg has new or unprocessed data (such as motion needs to supply feedback))
	/*! only one call to this can be made at a time per process (not threadsafe, but is multi-process safe)
	 *  @param msg incoming message to test
	 *  @param curFrameNumber the most recent frame number sent, i.e. SharedGlobals::MotionSimConfig::frameNumber
	 *  @param[out] lastFrameNumber if the incoming frame is already complete (no need to read), then the frame's number will be assigned here
	 *  @param haveFeedback you should pass true if motion feedback <em>can be</em> provided to read() (note the feedback doesn't necessarily need to be available right now, just if the call to read is necessary)
	 *  @param motionOverride true if motion feedback overrides sensor data (i.e SharedGlobals::MotionSimConfig::override)
	 *  @return returns a static UpdateInfo structure (to be passed to read()) if the message is unread, otherwise returns NULL. The structure is static -- DO NOT DELETE IT */
	WorldStatePool::UpdateInfo* isUnread(const RCRegion& msg, unsigned int curFrameNumber, unsigned int& lastFrameNumber, bool haveFeedback, bool motionOverride);
	//! takes a sensor update from the simulator/system, updates an entry in the pool with the new information
	/*! If isUnread() returns non-NULL, you should acquire a WriteRequest, and check that it succeeds, is current, and is still incomplete, before calling read()
	 *  @param info the info structure returned by isUnread
	 *  @param wsw the WriteRequest aquired before calling read (yes, you should submit a WriteRequest for @a info.frameNumber between isUnread() and read())
	 *  @param feedbackGenerated pass true if feedback will be generated by a caller to read() -- doesn't need to be <em>this</em> process, but <em>a</em> process.
	 *  @param zeroPIDFeedback pass true if sensor data should override motion feedback for joints with 0's for PID control (i.e. SharedGlobals::MotionSimConfig::zeroPIDFeedback)
	 *  @param feedback pointer to actual motion feedback, or NULL if not available in this process (weight values of feedback are ignored, assumes all values are valid) */
	bool read(const UpdateInfo& info, WriteRequest& wsw, bool feedbackGenerated, bool zeroPIDFeedback, const PostureEngine* feedback=NULL);
#endif

	//! processes a request, passed as either a ReadRequest or WriteRequest, to access an entry in the pool
	virtual void useResource(Data& d) { doUseResource(d); }
	//! completes access to an entry in the pool, you must pass the same request instance here which you originally passed to useResource()
	virtual void releaseResource(Data& d) { doReleaseResource(d); }
	
	//! does the actual work of useResource()
	/*! this is split off as a non-virtual function to avoid some process
	 *  identity issues that occur with virtual functions under Aperios */
	void doUseResource(Data& d);
	//! does the actual work of releaseResource()
	/*! this is split off as a non-virtual function to avoid some process
	 *  identity issues that occur with virtual functions under Aperios */
	void doReleaseResource(Data& d);

#ifdef PLATFORM_APERIOS
	//! registers #stateLookupMap with WorldState::setWorldStateLookup()
	void InitAccess() { WorldState::setWorldStateLookup(stateLookupMap); }
#endif
	
protected:
	//! number of buffers to set up
	static const unsigned int NUM_STATES=WORLDSTATEPOOL_NUM_STATES;
	
	//! shorthand for the type of #order
	typedef ListMemBuf<unsigned int, NUM_STATES> order_t;
	//! indicies of entries, in the order they were written (i.e. corresponding value in #frames should be monotonically increasing)
	order_t order;
	
	//! shorthand to test if all three P, I, and D values are 0 for the specified joint index (relative to 0, not PIDJointOffset)
	static bool isZeroPID(WorldState* s, unsigned int i) { return i>=PIDJointOffset && i<PIDJointOffset+NumPIDJoints && s->pids[i][0]==0 && s->pids[i][1]==0 && s->pids[i][2]==0; }
	
	//! called when access to an entry for reading is requested
	unsigned int getCurrentReadState(WorldState*& tgt, bool block);
	//! called when an read access to an entry is complete
	void doneReadingState(unsigned int i);
	
	//! returns true if the specified element of #states has been marked completed
	bool isComplete(unsigned int idx) const;
	//! returns index of buffer in #states to use for write request
	unsigned int selectWriteState(unsigned int frame, bool block) const { order_t::index_t idx; return selectWriteState(frame,block,idx); }
	//! returns index of buffer in #states to use for write request, stores index of corresponding entry of #order in @a idx
	unsigned int selectWriteState(unsigned int frame, bool block, order_t::index_t& idx) const;
	//! called when access to an entry for writing is requested
	unsigned int getCurrentWriteState(WorldState*& tgt, unsigned int frame, bool block);
	//! called when an write access to an entry is complete
	void doneWritingState(unsigned int i, bool completed);
	
	//! entries to hand out
	WorldState states[NUM_STATES];
	//! serial numbers of corresponding entries in #states, set when writing begins, should be monotonically increasing relative to #order (i.e. if you run through #order and look at corresponding values in #frames, should be monotonically increasing serial numbers)
	unsigned int frames[NUM_STATES];
	//! flag set when a reader is blocking for writing to finish, until read is satisified
	unsigned int reading[NUM_STATES];
	//! count of writers in line for access (occurs when a writer is blocking on another writer of the same frame, or no other frames are free)
	unsigned int writing[NUM_STATES];
	//! the status is intended as a bitfield to support communication between writers if they need to cooperatively fill out an entry
	/*! The value is set to 0 before handing out to a writer with a new frame number */
	unsigned int status[NUM_STATES];
#ifdef PLATFORM_APERIOS
	//! this lock indicates/controls whether the state is available for reading
	/*! The lock is set before handing out to a writer with a new frame number, and released
	 *  when a writer has marked the entry complete (via the WriteRequest upon releaseResource()) */
	MutexLock<ProcessID::NumProcesses> complete[NUM_STATES];
#else
	//! this semaphore is set to positive value when writing begins, and then lowered to zero when complete
	/*! A semaphore is used on "normal" systems because the MutexLock also implies a
	 *  thread lock, which we actually don't want in this case because a different thread
	 *  may complete the entry than the one which started it.  Since Aperios does not allow
	 *  multithreading, we don't have to worry about it there.
	 *
	 *  As an aside, we @e could use this to store #writing, but that would only be feasible if
	 *  Aperios gave us semaphores.  Bah. */
	SemaphoreManager::semid_t complete[NUM_STATES];
#endif
	
	//! locks to be acquired before handing out corresponding #states entry for writing
	MutexLock<ProcessID::NumProcesses> writeLocks[NUM_STATES];
	//! lock on WorldStatePool's own members
	MutexLock<ProcessID::NumProcesses> lock;

#ifdef PLATFORM_APERIOS
	//! the current state in use by each process
	WorldState* stateLookupMap[ProcessID::NumProcesses];
#endif

private:
	WorldStatePool(const WorldStatePool&); //!< this shouldn't be called...
	WorldStatePool& operator=(const WorldStatePool&); //!< this shouldn't be called...
};

/*! @file
 * @brief 
 * @author Ethan Tira-Thompson (ejt) (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.19 $
 * $State: Exp $
 * $Date: 2007/11/11 23:57:26 $
 */

#endif
