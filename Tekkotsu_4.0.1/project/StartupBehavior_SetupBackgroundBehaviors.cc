#include "StartupBehavior.h"
#include "Shared/RobotInfo.h"

#include "Behaviors/Controls/ControlBase.h"
#include "Behaviors/Controls/BehaviorSwitchControl.h"

#ifdef TGT_HAS_ACCELEROMETERS
#  include "Behaviors/Services/AutoGetupBehavior.h"
#endif
#ifdef TGT_HAS_LEDS
#  include "Behaviors/Services/FlashIPAddrBehavior.h"
#endif
#include "Behaviors/Demos/SimpleChaseBallBehavior.h"
#include "Behaviors/Demos/StareAtBallBehavior.h"
#ifdef TGT_HAS_POWER_STATUS
#  include "Behaviors/Services/BatteryMonitorBehavior.h"
#endif
#include "Behaviors/Demos/ToggleHeadLightBehavior.h"
#include "Behaviors/Demos/CrashTestBehavior.h"
#include "Behaviors/Demos/FreezeTestBehavior.h"
#include "Behaviors/Demos/RelaxBehavior.h"
#include "Behaviors/Services/WorldStateVelDaemon.h"
#include "Behaviors/Demos/CameraBehavior.h"
#ifdef TGT_HAS_LEGS
#  include "Behaviors/Demos/MotionStressTestBehavior.h"
#endif
#include "Behaviors/Demos/ASCIIVisionBehavior.h"
#ifdef TGT_HAS_TAIL
#  include "Behaviors/Nodes/TailWagNode.h"
#endif
#include "Sound/PitchDetector.h"

#include "Behaviors/Demos/TestBehaviors.h"

ControlBase*
StartupBehavior::SetupBackgroundBehaviors() {
	addItem(new ControlBase("Background Behaviors","Background daemons and monitors"));
	startSubMenu();
	{ 
		addItem(new BehaviorSwitchControl<SimpleChaseBallBehavior>("Simple Chase Ball",false));
		addItem(new BehaviorSwitchControl<StareAtBallBehavior>("Stare at Ball",false));
		if(RobotName==ERS220Info::TargetName)
			addItem(new BehaviorSwitchControl<ToggleHeadLightBehavior>("Toggle Head Light",false));
#ifdef TGT_HAS_TAIL
		addItem(new BehaviorSwitchControl<TailWagNode>("Wag Tail",false));
#endif
		addItem(new BehaviorSwitchControl<RelaxBehavior>("Relax",false));
		addItem(new BehaviorSwitchControl<CameraBehavior>("Camera",false));
		addItem((new BehaviorSwitchControl<ASCIIVisionBehavior>("ASCIIVision",false)));
		addItem(new ControlBase("Debugging Tests","Stress tests"));
		startSubMenu();
		{
#ifdef TGT_HAS_LEGS
			addItem(new BehaviorSwitchControl<MotionStressTestBehavior>("Motion Stress Test",false));
#endif
			addItem(new BehaviorSwitchControl<CrashTestBehavior>("Crash Test",false));
			addItem(new BehaviorSwitchControl<FreezeTestBehavior>("Freeze Test",false));
			// these following behaviors are all found in TestBehaviors.h, too small to warrant individual files
			addItem(new BehaviorSwitchControl<InstantMotionTestBehavior>("Instant MC Add/Remove",false));
			addItem(new BehaviorSwitchControl<BusyLoopTestBehavior>("Busy Loop Behavior",false));
			addItem(new BehaviorSwitchControl<BusyMCTestBehavior>("Busy Loop MC",false));
			addItem(new BehaviorSwitchControl<SuicidalBehavior>("Suicidal Behavior",false));
			addItem(new BehaviorSwitchControl<EchoTextBehavior>("Echo Text",false));
			addItem(new BehaviorSwitchControl<SaveImagePyramidBehavior>("Save Image Pyramid",false));
		}
		endSubMenu();
		addItem(new ControlBase("System Daemons","Provide some common sensor or event processing"));
		startSubMenu();
		{
#ifdef TGT_HAS_ACCELEROMETERS
			addItem((new BehaviorSwitchControl<AutoGetupBehavior>("Auto Getup",false)));
#endif
#ifdef TGT_HAS_LEDS
			addItem((new BehaviorSwitchControl<FlashIPAddrBehavior>("Flash IP Address",false))->start());
#endif
			addItem((new BehaviorSwitchControl<WorldStateVelDaemon>("World State Vel Daemon",false))->start());
#ifdef TGT_HAS_POWER_STATUS
			addItem((new BehaviorSwitchControl<BatteryMonitorBehavior>("Battery Monitor",false))->start());
#endif
			addItem((new BehaviorSwitchControl<PitchDetector>("Pitch Detection",false))->start());
		}
		endSubMenu();
	}
	return endSubMenu();
}
