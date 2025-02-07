#include "StartupBehavior.h"
#include "Shared/RobotInfo.h"

#include "Behaviors/Controls/ControlBase.h"
#include "Behaviors/Controls/BehaviorSwitchControl.h"

#include "Behaviors/Demos/ChaseBallBehavior.h"
#include "Behaviors/Nodes/WalkToTargetNode.h"
#include "Behaviors/Demos/HelloWorldBehavior.h"
#include "Behaviors/Demos/SimpleChaseBallBehavior.h"
#ifdef TGT_HAS_HEAD
#  include "Behaviors/Demos/StareAtBallBehavior.h"
#  include "Behaviors/Demos/LookForSoundBehavior.h"
#endif

#ifdef TGT_HAS_BUTTONS
#  ifdef TGT_HAS_HEAD
#    include "Behaviors/Demos/FollowHeadBehavior.h"
#  endif
#  include "Behaviors/Demos/SoundTestBehavior.h"
#  include "Behaviors/Demos/GroundPlaneBehavior.h"
#endif

#ifdef TGT_HAS_IR_DISTANCE
#  include "Behaviors/Demos/ExploreMachine.h"
#  include "Behaviors/Demos/PaceTargetsMachine.h"
#  include "Behaviors/Demos/WallTestBehavior.h"
#endif

#ifdef TGT_HAS_LEGS
#  include "Behaviors/Demos/AlanBehavior.h"
#  include "Behaviors/Demos/StareAtPawBehavior2.h"
#  include "Behaviors/Demos/KinematicSampleBehavior.h"
#  include "Behaviors/Demos/KinematicSampleBehavior2.h"
#  include "Behaviors/Demos/BanditMachine.h"
#endif

#include "Shared/ProjectInterface.h"

ControlBase*
StartupBehavior::SetupModeSwitch() {
	addItem(new ControlBase("Mode Switch","Contains the \"major\" applications - mutually exclusive selection"));
	startSubMenu();
	{
		//this group allows the behaviors to turn each other off when you start a new one so
		//only one is running at a time - like radio buttons
		BehaviorSwitchControlBase::BehaviorGroup * bg = new BehaviorSwitchControlBase::BehaviorGroup();

		//put behaviors here:
		addItem(new BehaviorSwitchControl<HelloWorldBehavior>("Hello World",false));
#ifdef TGT_HAS_LEGS
		addItem(new BehaviorSwitchControl<AlanBehavior>("Alan's Behavior",bg,false));
#endif
#ifdef TGT_HAS_HEAD
		addItem(new BehaviorSwitchControl<StareAtBallBehavior>("Stare at Pink Ball",bg,false));
		addItem(new BehaviorSwitchControl<LookForSoundBehavior>("Look at Sound",bg,false));
#endif
		addItem(new BehaviorSwitchControl<SimpleChaseBallBehavior>("Simple Chase Ball",bg,false));
		addItem(new BehaviorSwitchControl<ChaseBallBehavior>("Chase Ball",bg,false));
#ifdef TGT_HAS_BUTTONS
#  ifdef TGT_HAS_HEAD
		addItem(new BehaviorSwitchControl<FollowHeadBehavior>("Follow Head",bg,false));
#  endif
		addItem(new BehaviorSwitchControl<SoundTestBehavior>("Sound Test",bg,false));
#endif
		addItem(new ControlBase("State Machine Demos","More fully developed demo applications"));
		startSubMenu();
		{
			addItem(new BehaviorSwitchControlBase(new WalkToTargetNode("Walk To Target (ball)",ProjectInterface::visPinkBallSID),bg));
#ifdef TGT_HAS_LEGS
			addItem(new BehaviorSwitchControl<BanditMachine>("Bandit State Machine",bg,false));
#endif
#ifdef TGT_HAS_IR_DISTANCE
			addItem(new BehaviorSwitchControl<ExploreMachine>("Explore State Machine",bg,false));
			addItem(new BehaviorSwitchControl<PaceTargetsMachine>("Pace Targets State Machine",bg,false));
#endif
		}
		endSubMenu();
		addItem(new ControlBase("Kinematics Demos","Showcases some of the newly developed kinematics code"));
		startSubMenu();
		{
#ifdef TGT_HAS_LEGS
			addItem(new BehaviorSwitchControl<KinematicSampleBehavior>("Kinematic Sample 1",bg,false));
			addItem(new BehaviorSwitchControl<KinematicSampleBehavior2>("Kinematic Sample 2",bg,false));
			addItem(new BehaviorSwitchControl<StareAtPawBehavior2>("Stare at Paw",bg,false));
#endif
#ifdef TGT_HAS_BUTTONS
			addItem(new BehaviorSwitchControl<GroundPlaneBehavior>("Ground Plane Test",bg,false));
#endif
#ifdef TGT_HAS_IR_DISTANCE
			addItem(new BehaviorSwitchControl<WallTestBehavior>("Wall Test",bg,false));
#endif
		}
		endSubMenu();
	}
	return endSubMenu();
}
