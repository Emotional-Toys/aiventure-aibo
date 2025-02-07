#ifndef PLATFORM_APERIOS
#include "Thread.h"
#include "Shared/ReferenceCounter.h"
#include "ProcessID.h"

#include <pthread.h>
#include <string.h>
#include <iostream>
#include <signal.h>
#include <unistd.h>
#include <cassert>
#include <errno.h>
#ifdef __APPLE__
#	include <AvailabilityMacros.h>
#endif

using namespace std;

#define THREADCANCEL_SANITY_CHECKS

/*! @cond INTERNAL */
//! provides the system-dependent implementation of a thread
struct Threadstorage_t {
	//! constructor
	Threadstorage_t() : threadInfo() {}
	
	//! the main POSIX reference to the thread
	pthread_t threadInfo;
	
	//! storage which will be set up as a thread-specific memory value, so threads can tell themselves apart
	static pthread_key_t selfKey;
private:
	Threadstorage_t(const Threadstorage_t& r); //!< don't call
	Threadstorage_t& operator=(const Threadstorage_t& r); //!< don't call
};
pthread_key_t Threadstorage_t::selfKey=0;
/*! @endcond */

Thread::Thread()
	: pt(new Threadstorage_t), started(false), running(false), returnValue(NULL),
	noCancelDepth(0), cancelOrig(PTHREAD_CANCEL_ENABLE), group(NULL)
{
	Thread* cur=getCurrent();
	if(cur!=NULL)
		group=cur->getGroup();
}

Thread::~Thread() {
	//can only happen externally
	if(started) {
		stop();
		join();
	}
	/*if(pt==NULL) {
		std::cerr << "Thread storage already deleted!?!?!" << std::endl;
		*(int*)NULL=0xDEADDEAD;
	}*/
	assert(pt!=NULL);
	delete pt;
	pt=NULL;
}

void Thread::start() {
	if(started) {
		std::cerr << "Thread::start() -- thread is already started!" << std::endl;
		std::cerr << "   make another instance if you want to run another copy of this thread" << std::endl;
		return;
	}
	started=true;
	if(int err=pthread_create(&pt->threadInfo, NULL, launch, this))
		cerr << "Thread start(), pthread_create: " << strerror(err) << endl;
}

void * Thread::run() {
	for(;;) {
		unsigned int sleeptime=runloop();
		if(sleeptime==-1U)
			return returnValue;
		if(sleeptime>0)
			usleep(sleeptime);
		testCancel();
	}
	// this return is just to satisfy warnings with silly compiler
	return returnValue; //never happens -- cancel or max sleep time would exit
}

void Thread::interrupt() {
	if(!isRunning()) //can't interrupt before thread has been launched!
		return;
	if(signal(SIGALRM,handleInterrupt)==SIG_ERR)
		perror("PollThread::run(): initial signal()");
	sendSignal(SIGALRM);
}

void Thread::stop() {
	if(!started && !running) {
		std::cerr << "Thread::stop() -- thread has not been started!" << std::endl;
		return;
	}
	if(started && !running)
		usleep(50000);
	if(started && !running)
		std::cerr << "Thread::stop(): Waiting for thread launch to complete (stillborn thread?)" << std::endl;
	while(started && !running)
		usleep(100000);
	if(!running)
		return;
	if(int err=pthread_cancel(pt->threadInfo))
		cerr << "Thread cancel(), pthread_cancel("<<pt->threadInfo<<"): " << strerror(err) << endl;
#if defined(__APPLE__) && MAC_OS_X_VERSION_MAX_ALLOWED < MAC_OS_X_VERSION_10_5
	/* Mac OS X Tiger and earlier (v10.4 and prior) don't handle pthread_cancel in system calls,
	 * (namely sleep, read, listen, etc.) so we need to send a signal to wake it up.
	 * Mac OS X Leopard seems to fix this, although the thread usually gets canceled before we
	 * get to signal it, causing a warning in sendSignal()... hence the Mac OS version check.
	 * This is generally fine to do on other platforms too, but seems to cause a problem on Fedora Core 5
	 * where it causes the pthread_cleanup to skip Thread::handle_exit and cause mayhem.
	 * So we'll restrict the interrupt signal to only those platforms which won't break out of sleep on a cancel */
	interrupt(); // break thread out of any long sleep commands
#endif
}

void Thread::kill() {
	sendSignal(SIGUSR1);
}

void Thread::murder() {
	if(int err=pthread_detach(pt->threadInfo))
		cerr << "Thread kill(), thread_detach: " << strerror(err) << endl;
	sendSignal(SIGSTOP);
	started=running=false;
}

void Thread::sendSignal(int sig) {
	if(started && !running)
		usleep(50000);
	if(started && !running)
		std::cerr << "Thread::stop(): Waiting for thread launch to complete (stillborn thread?)" << std::endl;
	while(started && !running)
		usleep(100000);
	if(!isRunning())
		return;
	if(int err=pthread_kill(pt->threadInfo,sig))
		cerr << "Thread sendSignal(), pthread_kill("<<sig<<"): " << strerror(err) << endl;
}

void * Thread::join() {
	void * ans=NULL;
	if(int err=pthread_join(pt->threadInfo, &ans))
		cerr << "Thread join(), pthread_join: " << strerror(err) << endl;
	return ans;
}

Thread* Thread::getCurrent() {
	if(Threadstorage_t::selfKey==0) {
		static bool gaveError=false;
		if(!gaveError) {
			cerr << "ERROR: In Thread::getCurrent(), selfKey uninitialized; Thread::initMainThread was not called." << endl;
			cerr << "       (This error will only be displayed once)" << endl;
			gaveError=true;
		}
		return NULL;
	}
	return static_cast< Thread* >(pthread_getspecific(Threadstorage_t::selfKey));
}

void Thread::initMainThread() {
	if(int err=pthread_key_create(&Threadstorage_t::selfKey,warnSelfUndestructed))
		cerr << "WARNING: In Thread::initMainThread(), pthread_key_create(selfKey) returned " << strerror(err) << endl;
	if(int err=pthread_setspecific(Threadstorage_t::selfKey,NULL))
		cerr << "WARNING: In Thread::initMainThread(), pthread_setspecific(selfKey) returned " << strerror(err) << endl;
}

void Thread::releaseMainThread() {
	//handle_exit(NULL);
	if(int err=pthread_key_delete(Threadstorage_t::selfKey))
		cerr << "WARNING: In Thread::releaseMainThread, pthread_key_delete(selfKey) returned " << strerror(err) << endl;
}

void Thread::testCancel() {
#ifdef DEBUG
	if(noCancelDepth!=0) {
		cerr << "WARNING: Thread::testCancel called with noCancelDepth=="<<noCancelDepth<<" (process="<<ProcessID::getID()<<", thread="<<pthread_self()<<")"<<endl;
	}
#endif
	pthread_testcancel();
}

void * Thread::launch(void * msg) {
	//cout << "Spawn thread " << pthread_self() << " from process " << ProcessID::getID() << endl;
	Thread* cur=static_cast<Thread*>(msg);
	if(cur==NULL) {
		cerr << "ERROR: Thread::launch with NULL msg" << endl;
		return NULL;
	}

	if(int err=pthread_setspecific(Threadstorage_t::selfKey,msg))
		cerr << "WARNING: In ThreadNS::launch(), pthread_setspecific(selfKey) returned " << strerror(err) << endl;
	
	//disable cancel while calling launch()
	if(int err=pthread_setcancelstate(PTHREAD_CANCEL_DISABLE,NULL))
		cerr << "Thread launch(), pthread_setcanceltype: " << strerror(err) << endl;
	++(cur->noCancelDepth);
	if(signal(SIGUSR1,Thread::handle_launch_signal)==SIG_ERR)
		perror("Thread launch(), signal(SIGUSR1,handle_launch_signal)");
	cur->running=true;
	if(!cur->launched()) {
		//subclass's launch cancelled launch
		--(cur->noCancelDepth);
		handle_exit(NULL);
		return cur->returnValue;
	}
	--(cur->noCancelDepth);
	
	//These pthread functions actually define a scope between them (ugh)
	//I've added braces of my own to make this explicitly clear
	pthread_cleanup_push(Thread::handle_exit,msg); {
		
		if(signal(SIGUSR1,Thread::handle_signal)==SIG_ERR)
			perror("Thread launch(), signal(SIGUSR1,handle_signal)");
		
		if(cur->noCancelDepth==0) {
			//reset cancelability before run
			if(int err=pthread_setcancelstate(cur->cancelOrig,NULL))
				cerr << "Thread launch(), pthread_setcanceltype: " << strerror(err) << endl;
			if(int err=pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED,NULL))
				cerr << "Thread launch(), pthread_setcanceltype: " << strerror(err) << endl;
		}
		cur->returnValue=cur->run();
		
	} pthread_cleanup_pop(true);
	return cur->returnValue;
}

void Thread::handle_launch_signal(int /*sig*/) {
	handle_exit(NULL);
	pthread_exit(NULL);
}

void Thread::handle_signal(int /*sig*/) {
	pthread_exit(NULL);
}

void Thread::handle_exit(void * th) {
	//cout << "Cull thread " << pthread_self() << endl;
	Thread* cur=getCurrent();
	if(cur==NULL) {
		cerr << "ERROR: handle_exit called for a NULL thread" << endl;
		if(th!=NULL) {
			static_cast<Thread*>(th)->cancelled();
			static_cast<Thread*>(th)->started=static_cast<Thread*>(th)->running=false;
		}
		return;
	}
	
	if(th!=NULL && th!=cur)
		cerr << "WARNING: handle_exit argument does not match selfKey" << endl;
	if(cur->noCancelDepth!=0) {
		cerr << "WARNING: thread " << pthread_self() << " of ProcessID_t " << ProcessID::getID() << " exited while noCancelDepth>0 (was " << cur->noCancelDepth << ")" << endl;
		cerr << "         This may indicate a mutex was left locked." << endl;
	}
	if(int err=pthread_setspecific(Threadstorage_t::selfKey,NULL))
		cerr << "WARNING: In Thread::handle_exit(), pthread_setspecific(selfKey) returned " << err << endl;
	cur->cancelled();
	cur->started=cur->running=false;
}

void Thread::pushNoCancel() {
	Thread * cur=getCurrent();
	if(cur==NULL) {
		//cerr << "ERROR: Thread::pushNoCancel was given NULL thread by getCurrent, thread=" << pthread_self() << endl;
		//not so bad, indicates already canceled -- don't test cancel again, don't want to cancel-recurse
		if(int err=pthread_setcancelstate(PTHREAD_CANCEL_DISABLE,NULL))
			cerr << "ERROR: Thread pushNoCancel(), pthread_setcanceltype: " << strerror(err) << endl;
	} else {
		++(cur->noCancelDepth);
		int previous=-1;
		if(int err=pthread_setcancelstate(PTHREAD_CANCEL_DISABLE,&previous))
			cerr << "ERROR: Thread pushNoCancel(), pthread_setcanceltype: " << strerror(err) << endl;
#ifdef THREADCANCEL_SANITY_CHECKS
		if(cur->noCancelDepth==1 && previous!=cur->cancelOrig)
			cerr << "WARNING: In Thread::pushNoCancel, cancel state was wrong (was " << previous << ", expected " << cur->cancelOrig << ")" << endl;
		else if(cur->noCancelDepth!=1 && previous!=PTHREAD_CANCEL_DISABLE)
			cerr << "WARNING: In Thread::pushNoCancel, cancel state was somehow re-enabled" << endl;
#endif
	}
}
void Thread::popNoCancel() {
	Thread * cur=getCurrent();
	if(cur==NULL) {
		//cerr << "ERROR: Thread::popNoCancel was given NULL thread by getCurrent, thread=" << pthread_self() << endl;
		//not so bad, indicates already canceled -- don't test cancel again, don't want to cancel-recurse
		return; //no point in continuing
	} else if(cur->noCancelDepth==0) {
		cerr << "ERROR: Thread::popNoCancel underflow" << endl;
	} else
		--(cur->noCancelDepth);
	int previous=-1;
	if(cur->noCancelDepth==0) {
		if(int err=pthread_setcancelstate(cur->cancelOrig,&previous))
			cerr << "ERROR: Thread popNoCancel(), pthread_setcanceltype: " << strerror(err) << endl;
	}
#ifdef THREADCANCEL_SANITY_CHECKS
	else { //still disabled, double check it
		if(int err=pthread_setcancelstate(PTHREAD_CANCEL_DISABLE,&previous))
			cerr << "ERROR: Thread popNoCancel(), pthread_setcanceltype: " << strerror(err) << endl;
	}
	if(previous!=PTHREAD_CANCEL_DISABLE)
		cerr << "WARNING: In Thread::popNoCancel, cancel state was somehow re-enabled" << endl;
#endif
}

void Thread::handleInterrupt(int /*signal*/) {
	//if(signal(SIGALRM,SIG_DFL)==SIG_ERR)
	//	perror("PollThread::handleInterrupt(): could not re-enable signal");
	Thread * cur=Thread::getCurrent();
	if(cur==NULL) {
		std::cerr << "Thread::handleInterrupt called from non-Thread" << endl;
		return;
	}
	if(cur->noCancelDepth==0)
		cur->testCancel();
	cur->interrupted();
}

void Thread::warnSelfUndestructed(void* msg) {
	cerr << "ERROR: Thread local data (selfKey) not deleted by Thread::handle_exit" << endl;
	Thread* cur = getCurrent();
	if(cur!=NULL)
		cerr << "       Weird, key wasn't cleared... (" << cur << ") " << cur->noCancelDepth << " locks on stack? " << endl;;
	if(msg==NULL) {
		cerr << "       Message is null, warnCancelDepthUndestructed shouldn't have been called." << endl;
	} else {
		if(cur!=NULL && cur!=msg)
			cerr << "       and current thread does not match msg (" << cur << " vs " << msg << ")" << endl;
		cur = static_cast<Thread*>(msg);
	}
	if(cur!=NULL) {
		//try to recover
		if(cur->noCancelDepth==0) {
			cerr << "       But at least the depth is 0" << endl;
		} else {
			cerr << "       The depth indicates there may be " << cur->noCancelDepth << " locks left in place" << endl;
		}
		cur->cancelled();
		cur->started=cur->running=false;
		pthread_setspecific(Threadstorage_t::selfKey,NULL);
	}
}


namespace ThreadNS {
		
	/*! @cond INTERNAL */
	//! This handles the actual lock implementation, which allows Lock to provide an abstract interface
	class Lock::LockStorage : public ReferenceCounter {
		friend class Condition;
	public:
		//! constructor
		LockStorage() : ReferenceCounter(), locklevel(0), mutex(), attr(), threadkey() {
			AddReference();
			pthread_mutexattr_init(&attr);
			pthread_mutexattr_settype(&attr,PTHREAD_MUTEX_RECURSIVE);
			pthread_mutex_init(&mutex,&attr);
		}
		//! destructor, releases any pending locks (with warning
		~LockStorage() {
			pthread_mutexattr_destroy(&attr);
			pthread_mutex_destroy(&mutex);
			if(locklevel>1) //having one left is ok, perhaps even good (keeping the lock as it is destroyed)
				cerr << "WARNING: lockstorage destructed with " << locklevel << " locks still in effect" << endl;
			while(locklevel>0) {
				locklevel--;
				Thread::popNoCancel();
			}
		}
		//! copy constructor (functional!) -- both locks will wind up referencing the same system resource, so this is more of an alias than a clone
		LockStorage(const LockStorage& ls) : ReferenceCounter(ls), locklevel(ls.locklevel), mutex(ls.mutex), attr(ls.attr), threadkey(ls.threadkey) {}
		//! assignment (functional!) -- both locks will wind up referencing the same system resource, so this is more of an alias than a clone
		LockStorage& operator=(const LockStorage& ls) { ReferenceCounter::operator=(ls); locklevel=ls.locklevel; mutex=ls.mutex; attr=ls.attr; threadkey=ls.threadkey; return *this; }
		
		//! trigger and wait for a mutual exclusion lock, recursively
		void lock() {
			Thread::pushNoCancel();
			if(int err=pthread_mutex_lock(&mutex)) {
				cerr << "ERROR: ThreadNS::Lock::lock() failed: " << strerror(err) << endl;
				Thread::popNoCancel();
			} else
				locklevel++;
		}
		//! attempt to get a lock, but return false if it is not immediately available
		bool trylock() {
			Thread::pushNoCancel();
			if(!pthread_mutex_trylock(&mutex)) {
				locklevel++;
				return true;
			} else {
				Thread::popNoCancel();
				return false;
			}
		}
		//! release a lock (recursively, won't actually release the lock resource until all calls to lock() have been balanced)
		void unlock() {
			if(locklevel==0)
				cerr << "ERROR: ThreadNS::Lock::unlock() underflow" << endl;
			locklevel--;
			if(int err=pthread_mutex_unlock(&mutex))
				cerr << "ERROR: ThreadNS::Lock::unlock() failed: " << strerror(err) << endl;
			Thread::popNoCancel();
		}
		//! returns the depth of the lock recursion (#locklevel)
		unsigned int getLockLevel() { return locklevel; }
		
	protected:
		unsigned int locklevel; //!< depth of lock recursion (i.e. number of calls to lock() minus calls to unlock())
		pthread_mutex_t mutex; //!< system lock resource
		pthread_mutexattr_t attr; //!< system lock resource attributes (used to specify #mutex is recursive in the system as well)
		pthread_key_t threadkey; //!< not making use of the thread specific nature of these, but we are making use of the call to a destructor (emergencyUnlock) on cancel
	};

	Lock::LockStorage* Lock::glock=NULL;
	/*! @endcond */

	Lock::Lock() : mylock(new LockStorage), locklevel(0) {
		if(glock==NULL)
			setup();
	}
	/*Lock::Lock(const Lock& l)
		: mylock(l.mylock), locklevel(0)
	{
		glock->lock();
		mylock->AddReference();
		glock->unlock();
		lock();
	}
	Lock::Lock(const Lock& l, bool autolock)
		: mylock(l.mylock), locklevel(0)
	{
		glock->lock();
		mylock->AddReference();
		glock->unlock();
		if(autolock)
			lock();
	}
	Lock& Lock::operator=(const Lock& l) {
		glock->lock();
		lock();
		if(locklevel>2)
			cerr << "WARNING: ThreadNS::Lock overwritten with "<<locklevel<<" locks still in effect" << endl;
		if(!mylock->RemoveReference())
			while(locklevel>0)
				unlock();
		mylock=l.mylock;
		locklevel=0;
		glock->unlock();
		return *this;
	}*/
	Lock::~Lock() {
		glock->lock();
		if(locklevel>1)
			cerr << "WARNING: ThreadNS::Lock destructed with "<<locklevel<<" locks still in effect" << endl;
		if(!mylock->RemoveReference())
			while(locklevel>0)
				unlock();
		glock->unlock();
	}

	void Lock::lock() {
		mylock->lock();
		locklevel++;
	}
	bool Lock::trylock() {
		if(mylock->trylock()) {
			locklevel++;
			return true;
		} else {
			return false;
		}
	}
	void Lock::unlock() {
		locklevel--;
		mylock->unlock();
	}
	unsigned int Lock::getLockLevel() const {
		return mylock->getLockLevel();
	}
	void Lock::setup() {
		if(glock==NULL)
			glock=new LockStorage;
	}
	
	/*! @cond INTERNAL */
	//! Implement system-dependent portion of a thread condition, a signaling mechanism.
	/*! This is a very basic wrapper -- just adds a constructor and destructor to the POSIX pthread_cond_t. */
	class Condition::ConditionStorage {
	public:
		//! constructor
		ConditionStorage() : cond() {
			if(int err=pthread_cond_init(&cond,NULL)) {
				cerr << "ERROR: ThreadNS::Condition::ConditionStorage() failed: " << strerror(err) << endl;
			}
		}
		//! destructor
		~ConditionStorage() {
			if(int err=pthread_cond_destroy(&cond)) {
				cerr << "ERROR: ThreadNS::Condition::~ConditionStorage() failed: " << strerror(err) << endl;
			}
		}
		//! system resource storage
		pthread_cond_t cond;
	};
	/*! @endcond */
	
	Condition::Condition() : mycond(new ConditionStorage) {}
	Condition::~Condition() { delete mycond; mycond=NULL; }
	
	void Condition::broadcast() const {
		if(int err=pthread_cond_broadcast(&mycond->cond)) {
			cerr << "ERROR: ThreadNS::Condition::broadcast() failed: " << strerror(err) << endl;
		}
	}
	void Condition::signal() const {
		if(int err=pthread_cond_signal(&mycond->cond)) {
			cerr << "ERROR: ThreadNS::Condition::signal() failed: " << strerror(err) << endl;
		}
	}
	bool Condition::timedwait(Lock& l, const timespec* abstime) const {
		if(int err=pthread_cond_timedwait(&mycond->cond,&l.mylock->mutex,abstime)) {
			if(err!=ETIMEDOUT)
				cerr << "ERROR: ThreadNS::Condition::timedwait() failed: " << strerror(err) << endl;
			return false;
		}
		return true;
	}
	void Condition::wait(Lock& l) const {
		if(int err=pthread_cond_wait(&mycond->cond,&l.mylock->mutex)) {
			cerr << "ERROR: ThreadNS::Condition::wait() failed: " << strerror(err) << endl;
		}
	}
	
}

#endif // PLATFORM check

/*! @file
* @brief Describes the Thread class and its AutoThread templated subclass
* @author ejt (Creator)
*
* $Author: ejt $
* $Name: tekkotsu-4_0-branch $
* $Revision: 1.32 $
* $State: Exp $
* $Date: 2007/11/11 05:58:13 $
*/
