#include "Aibo3DControllerBehavior.h"
#include "Behaviors/Controls/BehaviorSwitchControl.h"

BehaviorSwitchControlBase* Aibo3DControllerBehavior::stateSerializerControl=NULL;

void Aibo3DControllerBehavior::DoStart() {
	// Behavior startup
	BehaviorBase::DoStart();
	
	launchedSerializer=false;
	if(stateSerializerControl!=NULL) {
		if(!stateSerializerControl->isRunning()) {
			stateSerializerControl->start();
			launchedSerializer=true;
		}
		// open gui
		/*		std::vector<std::string> tmp;
		tmp.push_back("Aibo3D Load Instructions");
		tmp.push_back("To load Aibo3D, you will need to install java3d\nand then run Tekkotsu/tools/aibo3d/");
		tmp.back()+=getGUIType();
		Controller::loadGUI("ControllerMsg","LoadAibo3d",getPort(),tmp);*/
	}
	
	Controller::loadGUI(getGUIType(),getGUIType(),getPort());
}

void Aibo3DControllerBehavior::DoStop() {
	Controller::closeGUI(getGUIType());
	if(launchedSerializer && stateSerializerControl!=NULL) {
		stateSerializerControl->stop();
	}
	// Total behavior stop
	BehaviorBase::DoStop();
}

