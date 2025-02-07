#include "StartupBehavior.h"
#include "Shared/RobotInfo.h"

#include "Behaviors/Controls/ControlBase.h"

#ifdef TGT_HAS_POWER_STATUS
#  include "Behaviors/Controls/BatteryCheckControl.h"
#endif
#include "Behaviors/Controls/FreeMemReportControl.h"
#include "Behaviors/Controls/ProfilerCheckControl.h"
#include "Behaviors/Controls/EventLogger.h"
#include "Behaviors/Controls/SensorObserverControl.h"
#include "Behaviors/Controls/BehaviorReportControl.h"
#include "Behaviors/Controls/NetworkStatusControl.h"

ControlBase*
StartupBehavior::SetupStatusReports() {
	addItem(new ControlBase("Status Reports","Displays information about the runtime environment on the console"));
	startSubMenu();
	{
		addItem(new BehaviorReportControl());
#ifdef TGT_HAS_POWER_STATUS
		addItem(new BatteryCheckControl());
#endif
		addItem(new ProfilerCheckControl());
		addItem(new EventLogger());
		addItem(new SensorObserverControl());
		FreeMemReportControl * tmp=new FreeMemReportControl();
		tmp->DoStart();
		addItem(tmp);
		addItem(new NetworkStatusControl());
	}
	return endSubMenu();
}
