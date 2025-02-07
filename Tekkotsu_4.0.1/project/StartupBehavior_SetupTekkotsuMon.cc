#include "StartupBehavior.h"
#include "Shared/RobotInfo.h"

#include "Behaviors/Controls/ControlBase.h"
#include "Behaviors/Controls/BehaviorSwitchControl.h"

#ifdef TGT_HAS_HEAD
#  include "Behaviors/Mon/HeadPointControllerBehavior.h"
#endif

#if defined(TGT_HAS_LEGS) || defined(TGT_HAS_WHEELS)
#  include "Behaviors/Mon/WalkControllerBehavior.h"
#endif

#ifdef TGT_HAS_LEGS
#  include "Behaviors/Mon/UPennWalkControllerBehavior.h"
#  include "Behaviors/Mon/StewartPlatformBehavior.h"
#endif

#ifdef TGT_HAS_CAMERA
#  include "Behaviors/Mon/RawCamBehavior.h"
#  include "Behaviors/Mon/SegCamBehavior.h"
#  include "Behaviors/Mon/RegionCamBehavior.h"
#endif

#ifdef TGT_HAS_MICROPHONE
#  include "Behaviors/Mon/MicrophoneServer.h"
#endif

#ifdef TGT_HAS_SPEAKERS
#  include "Behaviors/Mon/SpeakerServer.h"
#endif

#if ( defined(TGT_HAS_ARMS) || defined(TGT_HAS_LEGS) ) && defined(TGT_HAS_HEAD)
#  include "Behaviors/Mon/RoverControllerBehavior.h"
#endif

#include "Behaviors/Mon/ViewWMVarsBehavior.h"
#include "Behaviors/Mon/WMMonitorBehavior.h"
#include "Behaviors/Mon/EStopControllerBehavior.h"
#include "Behaviors/Mon/Aibo3DControllerBehavior.h"
#include "Behaviors/Mon/WorldStateSerializerBehavior.h"
#include "Behaviors/Mon/EchoBehavior.h"

template<class T>
class SingletonFactory : public Factory0Arg<T> {
public:
	virtual T* operator()() { return &T::getInstance(); }
};

ControlBase*
StartupBehavior::SetupTekkotsuMon() {
	addItem(new ControlBase("TekkotsuMon","Servers for GUIs"));
	startSubMenu();
	{ 
#ifdef TGT_HAS_HEAD
		addItem((new BehaviorSwitchControl<HeadPointControllerBehavior>("Head Remote Control",false)));
#endif
#if defined(TGT_HAS_LEGS) || defined(TGT_HAS_WHEELS)
		addItem((new BehaviorSwitchControl<WalkControllerBehavior>("Walk Remote Control",false)));
#endif
#ifdef TGT_HAS_LEGS
		addItem((new BehaviorSwitchControl<UPennWalkControllerBehavior>("UPenn Walk Remote Control",false)));
		addItem((new BehaviorSwitchControl<StewartPlatformBehavior>("Stewart Platform",false)));
#endif
		addItem((new BehaviorSwitchControl<ViewWMVarsBehavior>("View WMVars",false)));
		addItem((new BehaviorSwitchControl<WMMonitorBehavior>("Watchable Memory Monitor",false))->start());
		addItem((new BehaviorSwitchControlBase(new EStopControllerBehavior(stop_id)))->start());
		BehaviorSwitchControlBase * wss=NULL;
#if defined(TGT_ERS7) || defined(TGT_ERS210) || defined(TGT_ERS2xx)
		// do this *before* WorldStateSerializer to avoid crash on shutdown (ugly, sorry)
		addItem((new BehaviorSwitchControl<Aibo3DControllerBehavior>("Aibo 3D",false)));
#endif
		addItem((wss=new BehaviorSwitchControl<WorldStateSerializerBehavior>("World State Serializer",false)));
		Aibo3DControllerBehavior::setSerializerControl(wss);
#ifdef TGT_HAS_CAMERA
		addItem((new BehaviorSwitchControl<RawCamBehavior>("Raw Cam Server",false)));
		addItem((new BehaviorSwitchControl<SegCamBehavior>("Seg Cam Server",false)));
		addItem((new BehaviorSwitchControl<RegionCamBehavior>("Region Cam Server",false)));
#endif
#ifdef TGT_HAS_MICROPHONE
		addItem((new BehaviorSwitchControl<MicrophoneServer,SingletonFactory<MicrophoneServer> >("Microphone Server",false)));
#endif
#ifdef TGT_HAS_SPEAKERS
		addItem((new BehaviorSwitchControl<SpeakerServer,SingletonFactory<SpeakerServer> >("Speaker Server",false)));
#endif
#if ( defined(TGT_HAS_ARMS) || defined(TGT_HAS_LEGS) ) && defined(TGT_HAS_HEAD)
		addItem((new BehaviorSwitchControl<RoverControllerBehavior,SingletonFactory<RoverControllerBehavior> >("Rover Control",false)));
#endif
		addItem((new BehaviorSwitchControl<EchoBehavior>("Echo Client/Server",false)));
	}
	return endSubMenu();
}
