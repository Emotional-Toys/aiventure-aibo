//-*-c++-*-
#ifndef INCLUDED_ToggleHeadLightBehavior_h_
#define INCLUDED_ToggleHeadLightBehavior_h_

#include "IPC/SharedObject.h"
#include "Motion/PostureMC.h"
#include "Shared/ERS220Info.h"

//! opens or closes the head light on an ERS-220
class ToggleHeadLightBehavior : public BehaviorBase {
public:
	//! constructor
	ToggleHeadLightBehavior() : BehaviorBase("ToggleHeadLightBehavior"), light_id(MotionManager::invalid_MC_ID) {}

	//! opens the head light
	virtual void DoStart() {
		BehaviorBase::DoStart();
		if(RobotName == ERS220Info::TargetName) {
			SharedObject<PostureMC> pose;
			pose->setOutputCmd(ERS220Info::RetractableHeadLEDOffset,true);
			light_id=motman->addPersistentMotion(pose);
		}
	}

	//! resets the head light
	virtual void DoStop() {
		motman->removeMotion(light_id);
	}

	static std::string getClassDescription() { return "Opens or closes the head light on an ERS-220"; }
	virtual std::string getDescription() const { return getClassDescription(); }

protected:
	MotionManager::MC_ID light_id; //!< id value of the PostureMC used to control the light
};

/*! @file
 * @brief Defines ToggleHeadLightBehavior, which will open or close the head light on an ERS-220
 * @author ejt (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.6 $
 * $State: Exp $
 * $Date: 2007/06/26 04:27:43 $
 */

#endif
