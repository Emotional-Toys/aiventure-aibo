#include "sim.h"
#include "Main.h"
#include "Motion.h"
#include "SoundPlay.h"
#include "Simulator.h"
#include "SharedGlobals.h"
#include "SimConfig.h"
#include "local/DataSource.h"
#include "IPC/SharedObject.h"
#include "IPC/RegionRegistry.h"
#include "Shared/Config.h"
#include "Shared/string_util.h"
#include "Shared/StackTrace.h"
#include "Shared/zignor.h"
#include "Shared/RobotInfo.h"
#include "IPC/SemaphoreManager.h"
#include "Events/EventRouter.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <sys/ipc.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <errno.h>
#include <pwd.h>

#ifndef DISABLE_READLINE
#  include <readline/readline.h>
#endif

#include "Events/EventBase.h"
#include "Events/LocomotionEvent.h"
#include "Events/TextMsgEvent.h"
#include "Events/VisionObjectEvent.h"

#ifndef TEKKOTSU_SHM_STYLE
#  error TEKKOTSU_SHM_STYLE unset!
#endif

using namespace std;

//declare explicit instances of the NamedEnumerations we're using
//(cuts down on some pretty significant binary size / debugging symbol bloat)
template<> std::map<std::string,SharedGlobals::runlevel_t> plist::NamedEnumeration<SharedGlobals::runlevel_t>::namesToVals = std::map<std::string,SharedGlobals::runlevel_t>();
template<> std::map<SharedGlobals::runlevel_t,std::string> plist::NamedEnumeration<SharedGlobals::runlevel_t>::valsToNames = std::map<SharedGlobals::runlevel_t,std::string>();

const char * const sim::usage=
"[-h|--help] [-c|--config config-file] [cmd1 cmd2 ...]\n"
"Commands passed as arguments are commonly of the form var=val, but can\n"
"also be any valid simulator command, such as 'freeze'.  If you wish to\n"
"pass a multi-word command, encase the command in quotes.";
	
//only true for the first process -- children should set this to false when they fork
bool sim::original=true;
	
SimConfig sim::config;
vector<string> sim::cmdlineArgs;
pid_t sim::child=static_cast<pid_t>(-1);
vector<Thread*> sim::primaries;
sim::ConfigErrorCheck sim::cfgCheck;
	
/* Although I generally dislike the "can't have main function without a class declaration" style of java,
 * sometimes it does come in handy.  See the class notes for sim for more information. */
int main(int argc, const char* argv[]) {
	
	//This should match the ID of the process sent to manage_process by sim::run()
	// *must* be done before we create any shared regions to provide proper reference counting
	ProcessID::setID(ProcessID::SimulatorProcess);
			
	//initialize some threading globals
	Thread::initMainThread();
	
#if TEKKOTSU_SHM_STYLE==POSIX_SHM
#ifndef USE_UNBACKED_SHM
	//append username to shared memory root avoid permission denied
	struct passwd * pw=getpwuid(getuid());
	if(pw!=NULL) {
		if(RCRegion::shmRoot[RCRegion::shmRoot.size()-1]=='/') {
			RCRegion::shmRoot[RCRegion::shmRoot.size()-1]='-';
			RCRegion::shmRoot+=pw->pw_name;
			RCRegion::shmRoot+='/';
		} else {
			RCRegion::shmRoot+=pw->pw_name;
		}
	}
#endif
#elif TEKKOTSU_SHM_STYLE==NO_SHM
	sim::cfgCheck.holdMultiprocess();
#endif

	{
		sim s;
		if(!s.processCommandLine(argc,argv))
			return 2;
		//now the real meat begins
		if(!s.run()) //when this eventually returns, we're done (and all our children forks as well)
			return 1;
		return 0;
	}
}
	
sim::sim() :
	mutexSemMgr(),
	glob(),
	subj()
{
	//what's the granularity of usleep on this platform?
	MutexLockBase::usleep_granularity=measure_usleep_cost();
	//use our own custom get_time routine
	project_get_time::get_time_callback=&sim_get_time;
	project_get_time::get_timeScale_callback=&sim_getTimeScale;
		
	ProjectInterface::sendCommand=Simulator::sendCommand;
		
	//setup signal handlers
	signal(SIGHUP, handle_signal);
	signal(SIGINT, handle_signal);
	signal(SIGQUIT, handle_signal);
	signal(SIGILL, handle_signal);
	signal(SIGABRT, handle_signal);
	signal(SIGFPE, handle_signal);
	signal(SIGBUS, handle_signal);
	signal(SIGSEGV, handle_signal);
	signal(SIGSYS, handle_signal);
	signal(SIGPIPE, handle_signal);
	signal(SIGTERM, handle_signal);
	atexit(handle_exit);
		
#if TEKKOTSU_SHM_STYLE!=NO_SHM
	//Set up mutex's semaphore manager
	MutexLockBase::setSemaphoreManager(&(*mutexSemMgr));
#endif
	//Set up MessageQueue's semaphore manager
	MessageQueueBase::setSemaphoreManager(&(*mutexSemMgr));
	
	//Set up shared global parameters -- e.g. clock and runlevel info
	globals = &(*glob);
	sim::config.addEntry("WaitForSensors",globals->waitForSensors,"If true, wait for initial sensor readings before triggering the startup behavior or starting the motion polling thread.  On some platforms, sensed output values can be used to initialize output positions.  On others, you may be unable to get any feedback, or can only expect feedback if the robot was left running and the executable is reconnecting.");
	sim::config.addEntry("Speed",globals->timeScale,"The speed at which to run the simulation, as a multiple of \"real-time\".\nFor example, '1' is normal, '0.5' is half-speed, '0' is paused.\nAny negative value requests non-realtime mode, where the clock is moved as fast as processing (or manual control) allows.");
	globals->simulatorTime=sim::config.initSimTime;
	sim::config.addEntry("Motion",globals->motion,"Parameters for motion simulation");
	DataSource::setOutputTracker(globals->motion.providedOutputs);
		
	//Set up the subject registration area
	ipc_setup = &(*subj);
		
	//everyone uses config and erouter, might as well set it up here
	::config = new Config();
	::config->setFileSystemRoot("ms");
	if(::config->loadFile("config/tekkotsu.xml")==0) {
		if(::config->loadFile("config/tekkotsu.cfg")==0)
			std::cerr << std::endl << " *** ERROR: Could not load configuration file config/tekkotsu.xml *** " << std::endl << std::endl;
		else
			std::cerr << "Successfully imported settings from old-format tekkotsu.cfg" << std::endl;
	}
	if(::config->loadFile("config/sim_ovrd.xml")==0)
		if(::config->loadFile("config/sim_ovrd.cfg")!=0)
			std::cerr << "Successfully imported settings from old-format simulator.cfg" << std::endl;
	::erouter = new EventRouter;
	
	//we won't have sensor values yet, but the system clock is good enough
	if(::config->main.seed_rng) {
		struct timeval tp;
		gettimeofday(&tp,NULL);
		tp.tv_sec+=tp.tv_usec;
		RanNormalSetSeedZig32((int*)&tp.tv_sec,tp.tv_usec);
		
		double t=TimeET().Value(); //current time with nanosecond resolution
		unsigned int * tm=reinterpret_cast<unsigned int*>(&t);
		unsigned int seed=tm[0]+tm[1];
		cout << "RNG seed=" << seed << ", zignor seeds: " << tp.tv_sec << ',' << tp.tv_usec << endl;;
		srand(seed);
	} else {
		int t=12345;
		int u=54321;
		RanNormalSetSeedZig32(&t,u);
	}
}
	
bool sim::processCommandLine(int argc, const char* argv[]) {
	int i=0;
	
	//try to load the configuration file
	string halconfigfile;
	halconfigfile.append("hal-").append(RobotName).append(".plist");
	struct stat sb;
	if(stat(halconfigfile.c_str(),&sb)==0) {
		if(!sim::config.loadFile(halconfigfile.c_str())) {
			cerr << "Error loading default configuration file '" << halconfigfile << "', may be malformed." << endl;
			return false;
		}
	} else {
		// no user settings available, load the defaults
		string defaultfile;
		defaultfile.assign("defaults/hal-common.plist");
		if(stat(defaultfile.c_str(),&sb)==0) {
			if(!sim::config.loadFile(defaultfile.c_str())) {
				cerr << "Error loading defaults file '" << defaultfile << "', may be malformed." << endl;
				return false;
			}
		}
		defaultfile.assign("defaults/hal-").append(RobotName).append(".plist");
		if(stat(defaultfile.c_str(),&sb)==0) {
			if(!sim::config.loadFile(defaultfile.c_str())) {
				cerr << "Error loading defaults file '" << defaultfile << "', may be malformed." << endl;
				return false;
			}
		}
		sim::config.setLastFile(halconfigfile); // don't want to save over the defaults file
	}
	
	//set the prompt from the binary name
	/*config.cmdPrompt=argv[i];
	if(config.cmdPrompt.rfind('/')!=string::npos)
		config.cmdPrompt=config.cmdPrompt.substr(config.cmdPrompt.rfind('/')+1);
	config.cmdPrompt+="> ";*/
	
	//set the prompt from the robot name
	config.cmdPrompt="HAL:";
	config.cmdPrompt.append(RobotName).append("> ");
	
	//now run through the rest of the arguments
	for(i++; i<argc; i++) {
		if(!strcmp(argv[i],"-h") || !strcmp(argv[i],"--help")) {
			cerr << argv[0] << ": " << usage << endl;
			return false;
		} else if(!strcmp(argv[i],"-c") || !strcmp(argv[i],"--config")) {
			if(!sim::config.loadFile(argv[++i]))
				return false;
		} else if(strchr(argv[i],'=')!=NULL) {
			string value=string_util::trim(strchr(argv[i],'=')+1);
			string key=string_util::trim(string(argv[i],strchr(argv[i],'=')-argv[i]));
			plist::ObjectBase* ob=sim::config.resolveEntry(key);
			if(ob==NULL)
				cmdlineArgs.push_back(argv[i]); //might be a key which is added by Simulator, we'll come back to it once Simulator has been launched
			else if(plist::PrimitiveBase* pbp=dynamic_cast<plist::PrimitiveBase*>(ob)) {
				try {
					pbp->set(value);
				} catch(const XMLLoadSave::bad_format& bf) {
					cout << "'" << value << "' is a bad value for '" << key << "'" << endl;
					cout << bf.what() << endl;
					return false;
				} catch(const std::exception& e) {
					cout << "An exception occured: " << e.what() << endl;
					return false;
				}
			} else {
				cout << "Cannot assign to a dictionary ("<<key<<")" << endl;
				return false;
			}
		} else {
			cmdlineArgs.push_back(argv[i]); //command to run in simulator
		}
	}
	
	return true;
}
	
bool sim::run() {
	// point of no return for setting multiprocess mode
	cfgCheck.holdMultiprocess();
	RCRegion::setMultiprocess(config.multiprocess);
	if(!config.multiprocess) {
		ProcessID::setIDHooks(getProcessID,setProcessID);
		
		//Setup wireless
		if(wireless==NULL) { // if running single-process, may already be set up
			wireless = new Wireless();
			sout=wireless->socket(Socket::SOCK_STREAM,Wireless::WIRELESS_DEF_RECV_SIZE,Wireless::WIRELESS_DEF_SEND_SIZE*12);
			serr=wireless->socket(Socket::SOCK_STREAM,Wireless::WIRELESS_DEF_RECV_SIZE,Wireless::WIRELESS_DEF_SEND_SIZE*4);
			wireless->setDaemon(sout);
			wireless->setDaemon(serr);
			serr->setFlushType(Socket::FLUSH_BLOCKING);
			sout->setTextForward();
			serr->setForward(sout);
		}
		
		//Setup Kinematics
		if(kine==NULL)
			kine=new Kinematics();
	}
	
	//this will force all of the processes to wait at the end of construction
	//until we're done spawning all of them
	globals->level_count[SharedGlobals::CREATED]++;
		
	cout << "Spawning processes..." << endl;
	cout.setf(ios::left);
	cout << "  Initializing runlevel " << setw(12) << SharedGlobals::runlevel_names[SharedGlobals::CONSTRUCTING] << endl;
	cout.unsetf(ios::left);
	if(fork_process<Main>()) ;
	else if(fork_process<Motion>()) ;
	else if(fork_process<SoundPlay>()) ;
	else if(config.multiprocess)
		manage_process<Simulator>();
	else {
		fork_process<Simulator>();
		globals->level_count[SharedGlobals::CREATED]--;
		while(primaries.size()>0) {
			primaries.back()->join();
			delete primaries.back();
			primaries.pop_back();
		}
	}	
	
	//every process is going to pass through here on their way out
	globals->level_count[SharedGlobals::DESTRUCTED]++;
	
	return true;
}
	
sim::~sim() {
#if TEKKOTSU_SHM_STYLE!=NO_SHM
	MutexLockBase::setSemaphoreManager(NULL);
#endif
	MessageQueueBase::setSemaphoreManager(NULL);
	globals=NULL;
	ipc_setup=NULL;
	
	if(child==static_cast<pid_t>(-1)) // never got to the fork (or had an error at the fork)
		return;
	
	if(original)
		cout << ProcessID::getIDStr() << ": Waiting for children to exit..." << endl;
	if(child!=0) {
		int status;
		int res=waitpid(child,&status,0);
		if(res<0 && errno!=ECHILD)
			perror("wait");
		if(!WIFEXITED(status))
			cout << ProcessID::getIDStr() << ": waiting for " << child << "..." << endl;
		while(!WIFEXITED(status)) {
			sleep(1);
			res=waitpid(child,&status,0);
			if(res<0 && errno!=ECHILD)
				perror("wait");
		}
	}
}
	
void sim::wait_runlevel(SharedGlobals::runlevel_t level) {
	globals->lock.lock(ProcessID::getID());
	globals->level_count[level]++;
	if(globals->level_count[level]==1) {
		cout.setf(ios::left);
		cout << "Collecting for runlevel " << setw(12) << SharedGlobals::runlevel_names[level] << "  |=" << flush;
		cout.unsetf(ios::left);
	}
	string nm=Process::getName();
	cout << nm << '=' << flush;
	if(globals->level_count[level]==globals->level_count[SharedGlobals::CREATED])
		cout << "|  done" << endl;
	globals->lock.unlock();
	while(globals->level_count[level]!=globals->level_count[SharedGlobals::CREATED])
		usleep(150*1000);
	globals->lock.lock(ProcessID::getID());
	globals->lock.unlock();
}
	
template<class T>
void sim::manage_process() {
	//initialize the first runlevel
	globals->lock.lock(T::getID());
	globals->level_count[SharedGlobals::CONSTRUCTING]++;
	cout << setw(35) << T::getClassName() << ":  ProcessID::getID()=" << T::getID() << "   pid=" << getpid() << endl;

	T t;
	ASSERT(T::getID()==ProcessID::getID(),"Process ID set incorrectly!");
	
	globals->lock.unlock();
	while(globals->level_count[SharedGlobals::CONSTRUCTING]!=globals->level_count[SharedGlobals::CREATED])
		usleep(150*1000);
	globals->lock.lock(ProcessID::getID());
	globals->lock.unlock();
	
	//now just walk through each of the other steps
	wait_runlevel(SharedGlobals::STARTING);
	t.DoStart();
	wait_runlevel(SharedGlobals::RUNNING);
	t.run(); //should return if/when SharedGlobals::shutdown flag is set
	wait_runlevel(SharedGlobals::STOPPING);
	t.DoStop();
	wait_runlevel(SharedGlobals::DESTRUCTING);
}
	
template<class T>
bool sim::fork_process() {
	if(config.multiprocess) {
		RCRegion::aboutToFork(T::getID());
		MutexLockBase::aboutToFork();
	}
	//increment this *before* the fork to guarantee everyone knows to wait for the new process
	globals->level_count[SharedGlobals::CREATED]++;
	if(config.multiprocess) {
		child=fork();
		if(child==static_cast<pid_t>(-1)) {
			cerr << "Unable to spawn simulator process!" << endl;
			exit(1);
		}
		if(child!=0) {
			manage_process<T>();
			return true;
		}
		original=false;
	} else {
		primaries.push_back(new PrimaryThread<T>());
		primaries.back()->start();
	}
	return false;
}

ProcessID::ProcessID_t sim::getProcessID() {
	Thread* th=Thread::getCurrent();
	if(th==NULL)
		return ProcessID::SimulatorProcess; // the main thread will fall into the simulator process
	return static_cast<ProcessID::ProcessID_t>(reinterpret_cast<size_t>(th->getGroup()));
}
void sim::setProcessID(ProcessID::ProcessID_t id) {
	Thread* th=Thread::getCurrent();
	ASSERTRET(th!=NULL,"Unable to set process ID for main thread");
	th->setGroup(reinterpret_cast<void*>(id));
}

void sim::handle_signal(int sig) {
	//	sigset_t mask_set;  /* used to set a signal masking set. */
	//	sigset_t old_set;   /* used to store the old mask set.   */
		
	/* reset the signal handler for next time */
	//	signal(sig, handle_signal);
	/* mask any further signals while we're inside the handler. */
	//	sigfillset(&mask_set);
	//	sigprocmask(SIG_SETMASK, &mask_set, &old_set);
		
	const char * name=NULL;
	char defBuf[30];
	switch(sig) {
	case SIGINT: name="SIGINT"; break;
	case SIGQUIT: name="SIGQUIT"; break;
	case SIGBUS: name="SIGBUS"; break;
	case SIGSEGV: name="SIGSEGV"; break;
	case SIGTERM: name="SIGTERM"; break;
	case SIGABRT: name="SIGABRT"; break;
	case SIGFPE: name="SIGFPE"; break;
	case SIGPIPE: name="SIGPIPE"; break;
	case SIGHUP: name="SIGHUP"; break;
	default:
		name=defBuf;
		snprintf(defBuf,30,"signal %d",sig);
		break;
	}
	cout << "*** ERROR " << Process::getName() << ": Received " << name << endl;
	
	static bool firstCall=true;
	if(!firstCall) {
		cerr << "Signal handler was recursively called, may be leaked IPC resources :(" << endl;
		cerr << "The 'ipcs' tool can be used to manually free these, if it becomes a problem. " << endl;
		cerr << "However, simply re-running will generally reclaim the previous buffers for you." << endl;
		exit(EXIT_FAILURE);
		return;
	}
	firstCall=false;
	
	stacktrace::displayCurrentStackTrace(15);
	
	cout << "*** ERROR " << Process::getName() << ": Engaging fault shutdown..." << endl;
	if(globals!=NULL && !globals->hadFault()) {
		if(!MessageQueueBase::getSemaphoreManager()->hadFault())
			globals->lock.lock(ProcessID::getID());
		if(globals->level_count[SharedGlobals::CREATED]>0)
			globals->level_count[SharedGlobals::CREATED]--;
		else
			cout << "*** ERROR " << Process::getName() << ": level_count[CREATED] underflow" << endl;
		globals->signalShutdown();
		if(!MessageQueueBase::getSemaphoreManager()->hadFault())
			globals->lock.unlock();
		globals->faultShutdown();
	}
	if(MessageQueueBase::getSemaphoreManager()!=NULL && !MessageQueueBase::getSemaphoreManager()->hadFault()) {
		cout << "*** ERROR " << Process::getName() << ": Dereferencing mutex SemaphoreManager" << endl;
		MessageQueueBase::setSemaphoreManager(NULL); //reset to preallocated instance
		MessageQueueBase::getSemaphoreManager()->faultShutdown();
	}
	cout << "*** ERROR " << Process::getName() << ": Dereferencing shared memory regions" << endl;
	RCRegion::faultShutdown();
	cout << "*** ERROR " << Process::getName() << ": Exiting..." << endl;
	exit(EXIT_FAILURE);
}
	
void sim::handle_exit() {
	static bool firstCall=true;
	if(!firstCall) {
		cerr << "handle_exit was recursively called" << endl;
		return;
	}
	firstCall=false;
	cout << Process::getName() << ": Exiting..." << endl;
#ifndef DISABLE_READLINE
	rl_reset_terminal(NULL);
#endif
	if(RCRegion::NumberOfAttach()==0) {
		if(original)
			cout << "Clean shutdown complete.  Have a nice day." << endl;
		return;
	}
	cout << "*** ERROR " << Process::getName() << ": Exit with attached memory regions, engaging fault shutdown..." << endl;
	if(globals!=NULL && !globals->hadFault()) {
		if(!MessageQueueBase::getSemaphoreManager()->hadFault())
			globals->lock.lock(ProcessID::getID());
		if(globals->level_count[SharedGlobals::CREATED]>0)
			globals->level_count[SharedGlobals::CREATED]--;
		else
			cout << "*** ERROR " << Process::getName() << ": level_count[CREATED] underflow" << endl;
		globals->signalShutdown();
		if(!MessageQueueBase::getSemaphoreManager()->hadFault())
			globals->lock.unlock();
		globals->faultShutdown();
	} else {
		cerr << "*** ERROR " << Process::getName() << ": exit with previous global fault" << endl;
	}
	if(MessageQueueBase::getSemaphoreManager()!=NULL && !MessageQueueBase::getSemaphoreManager()->hadFault()) {
		cout << "*** ERROR " << Process::getName() << ": Dereferencing mutex SemaphoreManager" << endl;
		MessageQueueBase::setSemaphoreManager(NULL); //reset to preallocated instance
		MessageQueueBase::getSemaphoreManager()->faultShutdown();
	} else {
		cerr << "*** ERROR " << Process::getName() << ": exit with previous mutex fault" << endl;
	}
	cout << "*** ERROR " << Process::getName() << ": Dereferencing shared memory regions" << endl;
	RCRegion::faultShutdown();
	cout << "*** ERROR " << Process::getName() << ": Exiting..." << endl;
}
	
unsigned int sim::measure_usleep_cost() {
	usleep(50000); //to hopefully clear out the scheduler for the duration of our test
	const unsigned int TRIALS=50;
	TimeET mintime(1.0); //should definitely be less than a second
	for(unsigned int i=0; i<TRIALS; i++) {
		//measure usleep (plus overhead)
		TimeET cur;
		usleep(1); // at least 1 to avoid being a no-op
		TimeET elapsed(cur.Age());
		if(elapsed<mintime)
			mintime=elapsed;
	}
	usleep(50000); //to hopefully clear out the scheduler for the duration of our test
	TimeET minover(1.0); //should definitely be less than a second
	for(unsigned int i=0; i<TRIALS; i++) {
		//measure overhead
		TimeET cur;
		TimeET elapsed(cur.Age());
		if(elapsed<minover)
			minover=elapsed;
	}
	if(mintime<minover) { // something went wrong -- overhead is greater than total
		mintime=0L;
		cout << "usleep granularity couldn't be measured, default to 10ms" << endl;
	} else {
		//subtract overhead
		mintime-=minover;
		cout << "usleep granularity is " << mintime.Value()*1.0e6 << "us";
		if(mintime<2L) {
			mintime=2L;
			cout << ", reset to 2ms";
		}
		cout << endl;
	}
	return static_cast<unsigned>(mintime.Value()*1.0e6);
}

sim::ConfigErrorCheck::ConfigErrorCheck() : PrimitiveListener(), holdMPValue(config.multiprocess) {}

sim::ConfigErrorCheck::~ConfigErrorCheck() {
	sim::config.multiprocess.removePrimitiveListener(this);
}

void sim::ConfigErrorCheck::plistValueChanged(const plist::PrimitiveBase& pl) {
	if(&pl==&sim::config.multiprocess) {
#if TEKKOTSU_SHM_STYLE==NO_SHM
		if(sim::config.multiprocess) {
			cerr << "ERROR: TEKKOTSU_SHM_STYLE was set to NO_SHM during compile, Muliprocess cannot be set to 'true'" << endl;
			sim::config.multiprocess=false;
		}
#else
#  ifdef DEBUG
		cerr << "sim::config.Multiprocess set to " << sim::config.multiprocess << endl;
#  endif
		if(holdMPValue!=sim::config.multiprocess) {
			cerr << "ERROR: Cannot change sim::config.Multiprocess during execution, must set from command line or load from settings file" << endl;
			sim::config.multiprocess=holdMPValue;
		}
#endif
	}
}

void sim::ConfigErrorCheck::holdMultiprocess() {
	holdMPValue=sim::config.multiprocess;
	sim::config.multiprocess.addPrimitiveListener(this);
}
