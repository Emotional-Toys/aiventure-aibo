//-*-c++-*-
#ifndef INCLUDED_DataSource_h_
#define INCLUDED_DataSource_h_

#include <string>
#include <iostream>

/* Usage cases:

Pulled (on demand):
	block with timeout: save bandwidth, accept latency -- load only after last was read and framerate delay
	sleep: minimize latency, waste bandwidth -- send requests at framerate
Pushed (sent by source):
	block (no timeout): when new data arrives, insert into queue (might overwrite last queue slot)
	sleep: can't block, have to poll at framerate
*/

class LoadDataThread;

//! abstract base class for simulator data sources
/*! Each subclass will implement loading data from some piece of hardware or network protocol.  This class
 *  is expected to be tied to a LoadDataThread (or subclass) which holds configuration settings which might
 *  be shared by multiple data sources, in particular LoadDataThread::src.
 *
 *  Ideally, getData() should return the most recent unprocessed data -- meaning that if you are pulling
 *  from a realtime source like a network stream, if multiple frames have arrived since the last call to
 *  getData(), you should return the most recent frame of the bunch.
 *
 *  If your data source provides sensor data including current output values, you should call
 *  providingOutput() for those outputs when you start being used by a LoadDataThread
 *  (e.g. setDataSourceThread() is called), and ignoringOutput() when you are no longer active
 *  (e.g. setDataSourceThread() is called with NULL).  This prevents the Motion process from
 *  clobbering your readings with its own feedback.
 */
class DataSource {
public:
	DataSource() : frozen(true), framerate(-1), verbose(0), thread(NULL) {} //!< constructor
	DataSource(const DataSource& ds) : frozen(ds.frozen), framerate(ds.framerate), verbose(ds.verbose), thread(NULL) {} //!< copy constructor, just in case your subclass wants it
	DataSource& operator=(const DataSource&) { return *this; } //!< assignment operator, just in case your subclass wants it
	virtual ~DataSource() {} //!< destructor

	//! returns the simulator time of the next data segment
	/*! should be in the future if nothing new since last data segment, otherwise should be the 
	 *  timestamp of the most recent data segment (older segments are skipped), return -1U if there is no more data
	 *  @see timestamp argument of getData() */
	virtual unsigned int nextTimestamp()=0;
	
	//! returns a descriptive name of the next data segment for user feedback
	/*! @see name argument of getData() */
	virtual const std::string& nextName()=0;
	
	//! called to retrieve the most recent data segment, or blocking until new data is available
	/*! @param[out] payload on return, should point to beginning of data segment, or NULL if none available
	 *  @param[out] payloadSize on return, should indicate size in bytes of data segment as @a payload
	 *  @param[in] timestamp the suggested return time; if multiple samples may be taken in the interval, they should be skipped until this time
	 *  @param[out] timestamp on return, should contain the time at which the data arrived (real time stream) or was scheduled to be sent (log on disk)
	 *  @param[out] name on return, a human-readable name for the frame -- e.g. filename for a data file loaded from disk
	 *  @return frame serial number, used to tell when frames from the data source have been dropped (indicated by the return value incrementing by more than one)
	 *
	 *  If no more data is available, set payload to NULL, and return the current frame (i.e. don't increment serial number).
	 *
	 * This call <b>should block</b> until data is available.  Other functions may be called
	 * asynchronously from other threads while in this function, see ThreadNS::Lock to implement
	 * mutual exclusion locks if needed.
	 *
	 * The input value of @a timestamp is a suggestion from the user's requested framerate -- try
	 * to return the frame closest to it.  If it is already past (e.g. 0 on 'advance'), return the current data!
	 * If you return a timestamp in the future, the LoadDataThread will sleep until the appropriate time.
	 * 
	 * Note that this can be called when the source is frozen, which means you should unfreeze,
	 * get the current (unread) data or block until the next data, freeze again, and return the data.  */
	virtual unsigned int getData(const char *& payload, unsigned int& payloadSize, unsigned int& timestamp, std::string& name)=0;
	
	//! called by simulator when the data source's activity changes; calls doFreeze() or doUnfreeze() as appropriate
	/*! You probably don't want to override this function -- that's what doFreeze() doUnfreeze are for! */
	virtual void setFrozen(bool fr) { if(fr==frozen) return; if((frozen=fr)) doFreeze(); else doUnfreeze(); }
	virtual bool getFrozen() const { return frozen; } //!< returns #frozen status
	
	//! if called, indicates a request to restart/reinitialize the data source
	/*! For example, a FileSystemDataSource would go back to the beginning of its list,
	 *  and a network-based source would close and reopen the connection */
	virtual void reset() {}
	
	//! called by the LoadDataThread subclass, allows you to register for properties which your subclass may care about
	/*! a pointer to the LoadDataThread is passed when this is becoming the current data source;
	 *  NULL will be passed when the data source is no longer being used */
	virtual void setDataSourceThread(LoadDataThread* th) { thread=th; }
	//! returns the LoadDataThread using this data source
	virtual LoadDataThread* getDataSourceThread() const { return thread; }
	
	virtual void setDataSourceFramerate(float fr) { framerate=fr; } //!< called by LoadDataThread whenever the expected framerate changes (LoadDataThread::framerate)
	virtual void setDataSourceVerbose(int v) { verbose=v; } //!< called by LoadDataThread whenever the requested verbosity level changes (LoadDataThread::verbosity)
	
	//! Called by simulator during initialization to tell DataSources where the array of output reference counts are stored (see #providedOutputs)
	/*! This would need to point into a shared memory region if using multi-process model, hence we can't just
	 *  use process-local static allocation. */
	static void setOutputTracker(unsigned int outputs[]) { providedOutputs=outputs; }
	
	//! will be called by initialization code prior to first getData() if client code is going to block on getting the first sensor reading
	static void setNeedsSensor(bool waiting) { requiresFirstSensor=waiting; }
	
protected:
	//! subclasses should call this if they provide sensor updates which will contain a measurement of the current position of output @a i.
	/* A DataSource should consider itself providing an output if it will be sending some kind of measurement of
	 *  the current value of an output, and has been assigned to a LoadDataThread
	 *  (i.e. setDataSourceThread() has been called and #thread is non-NULL).\n
	 *  This prevents the motion process from clobbering your readings with its own feedback.  */
	static void providingOutput(unsigned int i) {
		if(providedOutputs==NULL) {
			std::cerr << "Warning: unable to access DataSource::providedOutputs (NULL) +" << i << std::endl;
			return;
		}
		providedOutputs[i]++;
	}
	//! subclasses should call this if they used to, but will no longer, provide sensor updates containing the current position of output @a i.
	/*! You don't need to call this if you didn't previously call providingOutput(). */
	static void ignoringOutput(unsigned int i) {
		if(providedOutputs==NULL) {
			std::cerr << "Warning: unable to access DataSource::providedOutputs (NULL) -" << i << std::endl;
			return;
		}
		if(providedOutputs[i]==0) {
			std::cerr << "ERROR: DataSource output tracking underflow (" << __FILE__ << ":" << __LINE__ << ")" << std::endl;
			return;
		}
		providedOutputs[i]--;
	}

	virtual void doFreeze() {} //!< user hook for when #frozen is set to true
	virtual void doUnfreeze() {} //!< user hook for when #frozen is set to false
	
	bool frozen;	//!< indicates that data is going to be requested "sparsely"; logged data sources should not increment with time
	float framerate;
	int verbose;
	LoadDataThread* thread; //!< stores last call to setParent()

	//! if true, indicates that client code is going to wait for sensor readings returned by getData before executing
	/*! Allocation defined in LoadDataThread.cc */
	static bool requiresFirstSensor;

private:
	//! The counts of which outputs are being provided are used so the motion process can tell if it needs to provide feedback.
	/*! Subclasses should call providingOutput() and ignoringOutput() when they start and stop providing an output.
	 *  A DataSource should consider itself providing an output if it will be sending some kind of measurement of
	 *  the current value of an output, and has been assigned to a LoadDataThread
	 *  (i.e. setDataSourceThread() has been called and #thread is non-NULL).
	 *
	 *  Allocation is found in LoadDataThread.cc to avoid file clutter (no other need for a DataSource.cc) */
	static unsigned int * providedOutputs;
};

/*! @file
 * @brief Defines DataSource, an abstract base class for simulator data sources
 * @author Ethan Tira-Thompson (ejt) (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.8 $
 * $State: Exp $
 * $Date: 2007/07/24 03:46:35 $
 */

#endif
