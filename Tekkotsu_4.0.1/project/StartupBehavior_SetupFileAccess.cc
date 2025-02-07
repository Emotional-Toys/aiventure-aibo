#include "StartupBehavior.h"
#include "Shared/RobotInfo.h"

#include "Behaviors/Controls/ControlBase.h"

#include "Behaviors/Controls/PostureEditor.h"
#include "Behaviors/Controls/RunSequenceControl.h"
#include "Behaviors/Controls/PlaySoundControl.h"
#include "Behaviors/Controls/DumpFileControl.h"
#include "Behaviors/Controls/WaypointWalkControl.h"
#include "Behaviors/Controls/LoadPostureControl.h"
#include "Behaviors/Controls/SavePostureControl.h"
#include "Behaviors/Controls/ConfigurationEditor.h"

ControlBase*
StartupBehavior::SetupFileAccess() {
	addItem(new ControlBase("File Access","Access/load files on the memory stick"));
	startSubMenu();
	{
		addItem(new ConfigurationEditor("Tekkotsu Configuration"));
		addItem(new LoadPostureControl("Load Posture",stop_id));
		addItem(new SavePostureControl("Save Posture"));
		addItem(new PostureEditor(stop_id));
		addItem(new RunSequenceControl<XLargeMotionSequenceMC::CAPACITY>("Run Motion Sequence",stop_id));
		addItem(new PlaySoundControl("Play Sound"));
		addItem(new WaypointWalkControl());
#ifdef TGT_HAS_LEGS
		SetupWalkEdit();
#endif
		addItem(new DumpFileControl("Files",config->portPath("")));
	}
	return endSubMenu();
}
