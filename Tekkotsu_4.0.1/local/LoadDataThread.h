//-*-c++-*-
#ifndef INCLUDED_LoadDataThread_h_
#define INCLUDED_LoadDataThread_h_

#include "IPC/PollThread.h"
#include "Shared/plist.h"
#include "IPC/MessageQueue.h"
#include "Shared/get_time.h"
#include "local/DeviceDriver.h"
#include "local/DataSource.h"
#include <list>
#include <vector>

//! Provides resources for loading time-based data from disk
/*! Runs in a separate thread, preloads data into shared memory buffers, and
 *  then sends the buffer when the time is right.  Thread doesn't start until
 *  you set the source or call loadFileList(). */
class LoadDataThread : public virtual PollThread, public virtual plist::Dictionary, public virtual plist::PrimitiveListener, public virtual plist::CollectionListener, public virtual DeviceDriver::SourceListener {
public:
	//! member function pointer to call on DeviceDriver to get the data sources available for this thread
	typedef void (DeviceDriver::*getDataSources_t)(std::map<std::string,DataSource*>&);

	
	//! constructor
	/*! @param getDS a DeviceDriver member function callback, which will be used to query drivers for the data sources of they type being loaded by this thread
	 *  @param fps frames per second, see #framerate
	 *  @param messages the MessageQueue through which to send the data
	 *
	 *  For example, there are sensor sources and camera/image sources, each managed by a
	 *  different function of DeviceDriver (DeviceDriver::getSensorSources vs. DeviceDriver::getImageSources).
	 *  The LoadDataThread is initialized to call one of these functions to acquire data sources
	 *  based on which driver is referenced by #source.
	 */
	LoadDataThread(getDataSources_t getDS, float fps, MessageQueueBase& messages)
	: PollThread(), plist::Dictionary(),
		source(), framerate(fps), verbose(0), heartbeat(true), frozen(false),
		regions(), getDataSources(getDS), msgr(messages), lock(), lastSent(-1), frameSN(0), sourceSN(0), enabled(false), runloopState(INTERFRAME),
		curDS(NULL), dsDriver(NULL)
	{
		setLoadSavePolicy(FIXED,SYNC);
		addEntry("Source",source); getAvailableDataSources(); // getAvailableDataSources sets #source comment string
		addEntry("Framerate",framerate,"The rate at which images should be loaded.  This is passed as a hint to the source, which may be limited to multiples of its capture device frequency.");
		addEntry("Verbose",verbose,"Controls how much feedback to give on the console regarding progress\n  0 - none\n  1 - report when messages are dropped\n  2 - also report when a message is sent\n  3 - also report when heartbeat is sent/dropped, and when loop occurs\n  4 - also report when each message is preloaded");
		addEntry("Heartbeat",heartbeat,"If enabled, an empty \"heartbeat\" message is sent at the appropriate framerate, even if no data is being processed (i.e. frozen, no data loaded, or out of frames); this will cause an update event within the simulator, repeating processing on the previous data.");
		addEntry("Frozen",frozen,"If true, no frames will be sent, except via explicit 'advance' commands; if false, the thread will run and send messages at the requested times automatically");
		source.addPrimitiveListener(this); // call setDataSource when the source changes
		framerate.addPrimitiveListener(this); // update data source's framerate
		verbose.addPrimitiveListener(this); // set msgr report droppings and update data source's verbosity
		heartbeat.addPrimitiveListener(this); // start/stop thread if frozen
		frozen.addPrimitiveListener(this); // update current source, start/stop the thread
		DeviceDriver::getRegistry().addCollectionListener(this);
	}
	//! destructor
	~LoadDataThread();
	
	//! Names the device driver from which the DataSource will be taken
	/*! If you simply provide a device driver name, the first entry of the appropriate data queue will be used.
	 *  Alternatively, use drivername.entryname to specify a specific data source entry in case the driver
	 *  has multiple data sources for the queue type. */
	plist::Primitive<std::string> source;
	
	//! frames per second to send -- this is only a suggestion to hardware devices, which generally use their 'native' framerate, but may use this to limit data flow
	plist::Primitive<float> framerate;

	//! Controls how much feedback to give on the console regarding progress
	/*! 0 - none\n
	 *  1 - report when message is sent\n
	 *  2 - also report when message is dropped\n
	 *  3 - also report when heartbeat is sent/dropped, and when loop occurs\n
	 *  4 - also report when each message is preloaded */
	plist::Primitive<int> verbose; 

	//! if enabled, an empty "heartbeat" message is sent at the appropriate framerate, even if no data is being processed (i.e. no data loaded or out of frames); this will cause an update event within the simulator, repeating processing on the previous data.
	plist::Primitive<bool> heartbeat;
	
	//! if true, no frames will be sent, except via explicit (external) calls to advanceFrame(); if false, the thread will run and send messages at the requested times
	plist::Primitive<bool> frozen;
	
	//! sends the next frame as soon as possible, blocking until sent
	/*! @param forceQueue pass true to force a new data frame to be sent, otherwise will send a heartbeat if frozen
	 * @return true if there was a frame to send; will send heartbeat if configured, but returns false for heartbeats */
	virtual bool advanceFrame(bool forceQueue);

	virtual void plistValueChanged(const plist::PrimitiveBase& pl);
	virtual void plistCollectionEntryAdded(Collection& /*col*/, ObjectBase& /*primitive*/) { getAvailableDataSources(); }
	virtual void plistCollectionEntryRemoved(Collection& /*col*/, ObjectBase& /*primitive*/) { getAvailableDataSources(); }
	virtual void plistCollectionEntriesChanged(Collection& /*col*/) { getAvailableDataSources(); }
	virtual void start();
	virtual void stop();
		
	//! recovers data from serialized IPC message, returns beginning of payload, or NULL if there's an error
	/*! each parameter can be a pointer where to store the field, or NULL if you don't care */
	static char* deserializeHeader(char* buf, unsigned int size, unsigned int* verbose, unsigned int* sn, std::string* filename, bool* dataInQueue, unsigned int* payloadSize);

	virtual unsigned int nextTimestamp() { return (curDS!=NULL) ? curDS->nextTimestamp() : -1U; }
	virtual const std::string& nextName() { return (curDS!=NULL) ? curDS->nextName() : emptyStr(); }
	
	virtual DataSource* getDataSource() { return curDS; } //!< returns #curDS
	
	virtual void dataSourcesUpdated() { plistValueChanged(source); }
	
	//! called from constructor to build list of available data source names, resets the #source help text
	std::string getAvailableDataSources();
	
	virtual void loadXML(xmlNode* node) {
		plist::Dictionary::loadXML(node);
		getAvailableDataSources();
	}
	
protected:
	//! Makes a data source eligible for providing data
	/*! This is protected because you shouldn't be calling it directly.  Instead, set #source to the driver instance name, which will cause this to be called via plistValueChanged */
	virtual void setDataSource(DeviceDriver* dd, DataSource* ds);
	
	//! monitor #msgr, send new messages when their timestamp indicates they are due, then load upcoming messages
	virtual unsigned int runloop();

	//! removes and returns first region in #regions with only one reference, or NULL if none exists
	virtual RCRegion* firstUnusedRegion();
	
	//! sets up some header info in the specified RCRegion, reallocating if the suggested region is not large enough (or is NULL), returns pointer to end of header info in the region
	virtual char* setupRegion(RCRegion*& region, const std::string& file, unsigned int payload, bool hasMoreData);

	//! removes our reference to a region created by loadFile()
	virtual void freeRegion(RCRegion* rcr) { if(rcr!=NULL) rcr->RemoveReference(); }
	
	//! sends an empty heartbeat message indicating previous data should be reused
	virtual void sendHeartbeat() { sendHeartbeat(get_time()); }
	//! sends an empty heartbeat message indicating previous data should be reused, assuming that curt is the current time
	virtual void sendHeartbeat(unsigned int curt);
	
	//! returns time of next heartbeat
	unsigned int calcNextHeartbeat(unsigned int curt) const;
	
	unsigned int calcSleepTime() { unsigned int curt=get_time(); return static_cast<unsigned int>((calcNextHeartbeat(curt)-curt)/getTimeScale()); }
	
	typedef std::list<RCRegion* > msgbuf_t; //!< type of collection of shared data regions
	msgbuf_t regions; //!< for efficiency, reuse old buffers -- oldest at front, most recently used at back
	
	//! member function pointer to be called on DeviceDriver to get its available data sources
	getDataSources_t getDataSources;
	
	MessageQueueBase& msgr; //!< the MessageQueue through which to send the data
	Thread::Lock lock; //!< allows mutual exclusion over this object's member variables
	float lastSent; //!< timestamp of most recently sent message (or heartbeat); -1 if hasn't sent any from current source
	unsigned int frameSN; //!< serial number of next message to send
	unsigned int sourceSN; //!< serial number of the last data from #curDS; 0 if hasn't received any from current source
	bool enabled; //!< set to true by start(), false by stop(), but reset if we call start or stop ourselves based on changes to freeze or heartbeat
	volatile enum {
		INTERFRAME, //!< runloop() is waiting between frames (usually heartbeats)
		GET_FRAME, //!< runloop() is getting a frame, should not be interrupted
		GOT_FRAME, //!< runloop() has gotten a frame and is waiting until its timestamp has arrived
		ADVANCE_FRAME //!< runloop() was in GOT_FRAME, but advanceFrame() was called, frame should be sent *now*
	} runloopState;
	
	DataSource * curDS; //!< pointer to the current data source
	DeviceDriver * dsDriver; //!< pointer to the data source's driver
	
private:
	LoadDataThread(const LoadDataThread&); //!< do not call
	LoadDataThread& operator=(const LoadDataThread&); //!< do not call
};

/*! @file
 * @brief 
 * @author Ethan Tira-Thompson (ejt) (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.16 $
 * $State: Exp $
 * $Date: 2007/11/09 19:01:15 $
 */

#endif
