//-*-c++-*-
#ifndef INCLUDED_Thread_h_
#define INCLUDED_Thread_h_

#ifdef PLATFORM_APERIOS
#  warning Thread class is not Aperios compatable
#else

#include "Shared/Resource.h"
#include <stddef.h>

struct timespec;

//! provides Thread related data structures
namespace ThreadNS {
	//! an inter-thread lock -- doesn't work across processes, only threads within a process.  (see MutexLock for inter-process locks)
	class Lock : public Resource {
	public:
		Lock(); //!< constructor
		//explicit Lock(const Lock& l); //!< copy constructor -- shallow copy, share a lock, is handy for locking over a scope!!! (lock is automatically obtained on copy -- to avoid autolock, pass false to the two-argument constructor: Lock(const Lock& l, bool autolock) )
		//Lock(const Lock& l, bool autolock); //!< copy constructor -- shallow copy, share a lock, is handy for locking over a scope!!!
		//Lock& operator=(const Lock& l); //!< assignment -- dereference (and release) any previous lock, take on the new storage (shallow copy!)
		~Lock(); //!< destructor -- dereference and release (if any references remain)
		void lock(); //!< block until lock is obtained
		bool trylock(); //!< see if lock is available
		void unlock(); //!< release lock, if held
		unsigned int getInstanceLockLevel() const { return locklevel; } //!< returns the lock level of the local instance of Lock (as opposed to the lock storage structure, which might be shared with other Lock instances)
		unsigned int getLockLevel() const; //!< returns the lock level of the lock storage itself, the sum of all instance's lock levels
	protected:
		friend class MarkScope;
		friend class Condition;
		virtual void useResource(Resource::Data&) { lock(); }
		virtual void releaseResource(Resource::Data&) { unlock(); }
		
		class LockStorage; //!< this internal class will hold the system-dependent lock information
		static LockStorage* glock; //!< The global lock to protect Locks sharing #mylock's
		LockStorage* mylock; //!< This lock's implementation
		static void setup(); //!< creates a new #glock if it is currently NULL (should be called by the Lock() constructor)
		unsigned int locklevel; //!< the current lock level from this Lock, may differ from #mylock's lock level if several Locks are sharing a storage!
	private:
		Lock(const Lock& l); //!< don't call
		Lock& operator=(const Lock& l); //!< don't call
	};
	
	//! Provides an inter-thread signaling and synchronization mechanism
	class Condition {
	public:
		Condition(); //!< constructor
		~Condition(); //!< destructor
		
		void broadcast() const; //!< wake up all threads waiting on the condition
		void signal() const; //!< wake up a single thread waiting on the condition (which thread is unspecified)
		bool timedwait(Lock& l, const timespec* abstime) const; //!< wait for at most @a abstime for the condition before giving up (return true if condition found)
		void wait(Lock& l) const; //!< wait for condition
	protected:
		class ConditionStorage; //!< internal class to hold system-dependent information
		ConditionStorage* mycond; //!< the condition's implementation storage
	private:
		Condition(const Condition& l); //!< don't call
		Condition& operator=(const Condition& l); //!< don't call
	};
}	

//! Provides a nice wrapping of pthreads library
/*! If you need to provide cleanup functions on stop(), cancelled(), etc., you
 *  should override the destructor to stop and join so that you can be assured
 *  that your cleanup will be called if the thread is auto-destructed by going out of scope */
class Thread {
public:
	typedef ThreadNS::Lock Lock; //!< shorthand for pthread lock wrapper

	Thread(); //!< constructor, does not start thread by itself (although subclasses may)
	virtual ~Thread()=0; //!< destructor, will stop and join the thread, but you should override it to do the same if you provide any cleanup functions
	
	//! requests that the thread be started, if not already running (you need to create a separate instances if you want to run multiple copies)
	virtual void start();
	
	//! sends a signal to the thread which will interrupt any sleep calls (and trigger interrupted() to be called within the thread)
	virtual void interrupt();
	
	//! requests that the thread be stopped gracefully, if running.
	/*! A cancel flag is sent, and the thread will be stopped at next cancel point, defined
	 *  by whenever testCancel(), or a set of other system functions, are called.
	 *  See your system's pthread_testcancel() manual page for a list of cancel points.
	 *
	 *  This function may imply a call to interrupt() on systems which have extremely limited
	 *  system cancel points.  Currently, this consists of only Mac OS X.  There is hope that
	 *  additional cancellation points will be enabled on this system:
	 *  http://lists.apple.com/archives/darwin-kernel/2004/Jan/msg00032.html
	 *
	 *  @see pushNoCancel(), popNoCancel() */
	virtual void stop();
	
	//! sends a SIGUSR1 to the thread, breaking its execution, but still allowing handle_exit (and thus cancelled()) to be called.
	/*! Beware if your thread uses mutual exclusion locks, this can cause the thread to terminate while still holding locks */
	virtual void kill();
	
	//! detaches thread and sends SIGSTOP, which immediately halts the thread without any chance for cleanup
	/*! Beware if your thread uses mutual exclusion locks, this @b will cause the thread to terminate while still holding locks. */
	virtual void murder();
	
	//! sends a signal to the thread
	virtual void sendSignal(int sig);
	
	//! blocks calling thread until this Thread has terminated, via one means or another; return value is final return value by the thread
	virtual void * join();
	
	//! indicates whether start() has been called (but may be some delay before isRunning() is true...)
	virtual bool isStarted() const { return started; }
	
	//! indicates whether the thread is currently alive and running, implies isStarted()
	virtual bool isRunning() const { return running; }
	
	//! returns the Thread object for the current thread (or NULL for the main thread)
	static Thread* getCurrent() ;
	
	//! should be called before any threads are created to allow some global thread-specific data to be set up
	static void initMainThread();
	//! should be called if you no longer expect to have any threads in use
	static void releaseMainThread();

	//! should be called whenever a critical section has been entered (i.e. mutex obtained) -- prevents cancel from occurring until popNoCancel() is called
	static void pushNoCancel();
	//! should be called whenever a critical section is left (i.e. mutex released) -- if it was the last one, tests cancellability as well
	static void popNoCancel();
	
	//! returns #group
	void* getGroup() const { return group; }
	//! assigns #group, which will then be inherited by any threads instantiated by this one (the constructor call queries the current thread, no the start() or launch())
	void setGroup(void* g) { group=g; }
	
protected:
	//! called by launch() when thread is first entered, return false to cancel launch (set #returnValue as well if you care)
	virtual bool launched() { return true; }
	//! called by launch() once the thread has been set up; when this returns, the thread ends, see runloop()
	/*! Default implementation repeatedly calls runloop(), usleep(), and testCancel().
	 *  If you override, you should also be sure to call testCancel occasionally in order to support stop()
	 *  If function returns a value, that value overrides #returnValue.  If cancel occurs, #returnValue is used. */
	virtual void * run();
	//! override this as a convenient way to define your thread -- return the number of *micro*seconds to sleep before the next call; return -1U to indicate end of processing
	virtual unsigned int runloop() { return -1U; }
	//! called when handle_exit() is triggered, either by the thread being cancelled, or when run() has returned voluntarily
	virtual void cancelled() {}
	
	//! checks to see if stop() has been called, and if so, will exit the thread (passing through handle_exit() first)
	virtual void testCancel();
	//! thread entry point -- calls launched() on the thread (as indicated by @a msg), and then run()
	static void * launch(void * msg);
	//! indicates kill() has been called (or SIGUSR1 was sent from some other source) while launch() was still running
	static void handle_launch_signal(int sig);
	//! indicates kill() has been called (or SIGUSR1 was sent from some other source)
	static void handle_signal(int sig);
	//! indicates the thread is exiting, either voluntary (run() returned), stop(), or kill() -- calls cancelled() for the thread as indicated by @a th
	static void handle_exit(void * th);

	//! called by handleInterrupt() in target thread following call to interrupt(), assuming thread has not been cancelled (which can intercept the interrupt)
	virtual void interrupted() {}
	
	//! called by SIGALRM signal handler installed by interrupt() just before it posts the corresponding SIGALRM
	/*! tests for thread cancel condition before calling on to interrupted() */
	static void handleInterrupt(int signal);
	
	//! emit a warning that the last thread exited while the self-pointer thread-specific key still exists (need to call releaseMainThread() or handle_exit())
	static void warnSelfUndestructed(void* msg);

	//! stores the actual pthread data fields
	struct Threadstorage_t * pt;
	//! set to true once start() has been called, set back to false by handle_exit(), or by murder() itself
	bool started;
	//! set to true once launch() has been called, set back to false by handle_exit(), or by murder() itself
	bool running;
	//! indicates the value to be returned by the thread entry point (and thus passed back to join()) -- set this in runloop() or launched(), overridden by run()'s return value
	void * returnValue;
	//! depth of the pushNoCancel() stack
	unsigned int noCancelDepth;
	//! cancel status at root of no-cancel stack (may be no-cancel through and through)
	int cancelOrig;
	
	//! indicates a common group of threads, inherited from the thread which created this one, default NULL if created from main thread
	void* group;

private:
	Thread(const Thread& r); //!< don't call, not a well defined operation
	Thread& operator=(const Thread& r); //!< don't call, not a well defined operation
};

#endif //Aperios check

#endif

/*! @file
* @brief Describes the Thread class and its AutoThread templated subclass
* @author ejt (Creator)
*
* $Author: ejt $
* $Name: tekkotsu-4_0-branch $
* $Revision: 1.15 $
* $State: Exp $
* $Date: 2007/10/12 16:55:04 $
*/

