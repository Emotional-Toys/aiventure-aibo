//-*-c++-*-
#ifndef INCLUDED_StartupBehavior_h_
#define INCLUDED_StartupBehavior_h_
#include "Behaviors/BehaviorBase.h"
#include "Motion/MotionManager.h"
#include <vector>
#include <stack>

class Controller; // defined in Controller.h, don't actually need header file yet
class ControlBase; // defined in ControlBase.h, don't actually need header file yet

//! This is the default init behavior - it launches daemons, builds the Controller menu structure, does some nice things to fade the joints in to full power (reduces twitch)

/*! This is done by assigning an instance of StartupBehavior to the
 *  global ProjectInterface::startupBehavior variable.  Note that
 *  there's nothing special about the type of this class, it's just
 *  another BehaviorBase - ProjectInterface::startupBehavior could be
 *  any behavior.
 *  
 *  If you want some other behavior to handle the root initialization,
 *  simply remove the assignment of ::theStartup to startupBehavior at
 *  the beginning of StartupBehavior.cc.  Then you can substitute your
 *  own behavior instead.  Probably a good way to distribute project
 *  code when you're all done with development and want to ship a
 *  single-purpose memory stick (assuming your users won't want to get
 *  access to things like the Controller anymore... up to you.)
 *
 *  If all you want to do is get your behavior to launch at startup,
 *  you don't need to replace this.  Just add a menu entry.  (in say,
 *  the Mode Switch menu, which is separated out into
 *  StartupBehavior_SetupModeSwitch.cc for reduced recompilation after
 *  header files are modified) If you use a BehaviorSwitchControl, you
 *  can call its BehaviorSwitchControl::start() method to start the
 *  behavior it controls as you add it to the menu.  There are several
 *  examples of this usage in the Background Behaviors menu
 *  (StartupBehavior_SetupBackgroundBehaviors.cc)
 *
 *  This behavior is similar in idea to the init process in
 *  unix/linux.
 */
class StartupBehavior : public BehaviorBase {
public:
	//! Constructor
	StartupBehavior();
	//! Destructor
	virtual ~StartupBehavior();

	//! @name Inherited
	virtual void DoStart();
	virtual void DoStop();
	virtual void processEvent(const EventBase&);
	static std::string getClassDescription() { return "The initial behavior, when run, sets up everything else"; }
	virtual std::string getDescription() const { return getClassDescription(); }
	//@}
protected:
	//! Initializes the Controller menu structure - calls each of the other Setup functions in turn
	/*! Also adds Shutdown and Help items at the end */
	virtual ControlBase* SetupMenus();

	virtual ControlBase* SetupModeSwitch();          //!< sets up Mode Switch menu
	virtual ControlBase* SetupBackgroundBehaviors(); //!< sets up Background Behaviors menu
	virtual ControlBase* SetupTekkotsuMon();         //!< sets up TekkotsuMon menu
	virtual ControlBase* SetupStatusReports();       //!< sets up Status Reports menu
	virtual ControlBase* SetupFileAccess();          //!< sets up File Access menu
	virtual ControlBase* SetupWalkEdit();            //!< sets up Walk Editing menus (contributed by PGSS 2003 team project - see StartupBehavior_SetupWalkEdit.cc )
	virtual ControlBase* SetupVision();              //!< sets up the Vision menu

	virtual void initVision(); //!< Sets up the vision pipelines (held in StartupBehavior_SetupVision.cc)

	virtual void startSubMenu(); //!< future calls to addItem() will be placed under the most recently added item
	virtual void addItem(ControlBase * control); //!< inserts a control at the end of the current menu
	virtual ControlBase* endSubMenu();  //!< closes out a submenu so that future calls to addItem() will be added to the enclosing menu, returns the menu being closed out

	std::vector<BehaviorBase*> spawned; //!< Holds pointers to all the behaviors spawned from DoStart, so they can automatically be stopped on DoStop (should only happen on shutdown, but ensures cleanliness)
	std::vector<MotionManager::MC_ID> spawnedMC; //!< Holds ids for all the motion commands spawned from DoStart, so they can automatically be removed on DoStop, if they weren't already autopruned (should only happen on shutdown, but ensures cleanliness)
	std::stack<ControlBase*> setup;     //!< only used during setup - easier than passing it around all the Setup*() functions
	MotionManager::MC_ID stop_id; //!< the main EmergencyStopMC
	MotionManager::MC_ID pid_id; //!< used to fade in the PIDs up to full strength (from initial zero) This is so the joints don't jerk on startup.
};

#endif
