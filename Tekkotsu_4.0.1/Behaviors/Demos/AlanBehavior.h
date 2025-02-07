//-*-c++-*-
#ifndef INCLUDED_AlanBehavior_h_
#define INCLUDED_AlanBehavior_h_

#include "Behaviors/BehaviorBase.h"
#include "Motion/MotionManager.h"
#include "Motion/MMAccessor.h"
#include "IPC/SharedObject.h"
#include "Shared/WorldState.h"
#include "Events/EventRouter.h"
#include "Motion/PostureMC.h"
#include "Shared/ERS210Info.h"
#include "Shared/ERS7Info.h"

//! just a little demo behavior which lifts a leg higher as more pressure is put on a head button
/*! Based on an idea from Alan Chun-ho Ho for a basic demo program */
class AlanBehavior : public BehaviorBase {
public:
	AlanBehavior()
		: BehaviorBase("AlanBehavior"), pose_id(MotionManager::invalid_MC_ID)
	{}
	
	virtual void DoStart() {
		//call superclass first for housekeeping:
		BehaviorBase::DoStart();

		//now do your code:
		
		// creates a PostureMC class to move the joint(s) and adds it to global MotionManager
		pose_id=motman->addPersistentMotion(SharedObject<PostureMC>());
		// subscribe to sensor updated events through the global EventRouter
		erouter->addListener(this,EventBase::sensorEGID,SensorSrcID::UpdatedSID);
	}
	
	virtual void DoStop() {
		//do your code first:
		motman->removeMotion(pose_id);  // removes your posture controller
		erouter->removeListener(this); // stops getting events (and timers, if we had any)

		//but don't forget to call superclass at the end:
		BehaviorBase::DoStop();
	}
	
	virtual void processEvent(const EventBase& event) {
		// to be more general, let's check that it's the right event:
		if(event.getGeneratorID()==EventBase::sensorEGID) {
			//we'll need to specify the ERS210Info namespace below when
			//referencing the button offsets so that this will compile for
			//the ERS-7 as well (which lacks front and back head buttons),
			//but for your own code, you could leave it off if you weren't
			//worried about compiling for other models.

			// "checks out" the posture motion command from MotionManager
			// (this is the PostureMC we created in DoStart())
			MMAccessor<PostureMC> pose_mc(pose_id);
			
			//Joint offsets are defined in ERS210Info.h, ERS220Info.h, ERS2xxInfo.h, and ERS7Info.h
			unsigned int joint=LFrLegOffset+RotatorOffset;
			
			//state is a global instantiation of WorldState, kept up to date by framework;
			//pressure is in range 0 to 1 - we use the pressure on the front head button here
			float pressure=0;
			if(RobotName == ERS210Info::TargetName) {
				pressure=state->buttons[capabilities.getButtonOffset("HeadFrBut")];
				std::cout << "HeadFrBut Pressure: " << pressure << std::endl;
			} else if(RobotName == ERS7Info::TargetName) {
				pressure=state->buttons[ERS7Info::HeadButOffset];
				std::cout << "HeadBut Pressure: " << pressure << std::endl;
			} else {
				//only really works on the ERS-210 or ERS-7 models - the others don't have a proper pressure sensor
				//(the 220's antenna-thing is close, but doesn't give a continuous range)
				std::cout << "Unsupported/unknown model" << std::endl;
				erouter->removeListener(this); // stops getting events (and timers, if we had any)
				return;
			}
			
			//outputRanges is a constant table, also defined in ERS210Info.h or ERS220Info.h
			float angle=outputRanges[joint][MaxRange]*pressure;

			// now send the joint angle to the posture motion command
			pose_mc->setOutputCmd(joint,angle);

			//let's do the whole thing again with the other head button for the other leg:
			// (cutting out a some of the intermediary steps this time)
			joint=RFrLegOffset+RotatorOffset;
			if(RobotName == ERS210Info::TargetName)
				pose_mc->setOutputCmd(joint,outputRanges[joint][MaxRange]*state->buttons[capabilities.getButtonOffset(ERS210Info::buttonNames[ERS210Info::HeadBkButOffset])]);
			else if(RobotName == ERS7Info::TargetName) //ERS7 doesn't have another head button, we'll use one of its back buttons
				pose_mc->setOutputCmd(joint,outputRanges[joint][MaxRange]*state->buttons[ERS7Info::FrontBackButOffset]);

			// notice that there's no "check in" for pose_mc
			// MMAccessor's destructor does this automatically

		} else {
			//should never happen
			std::cout << "Unhandled Event:" << event.getName() << std::endl;
		}
	}
	
	static std::string getClassDescription() {
		// This string will be shown by the HelpControl or by the tooltips of the Controller GUI
		return "Lifts the left/right front legs higher as more pressure is applied to the front/back head buttons";
	}
	virtual std::string getDescription() const {
		// We override this function to return the string we supplied above (not required, but nice)
		return getClassDescription();
	}
	
protected:
	MotionManager::MC_ID pose_id; //!< ID of PostureMC, set up in DoStart() and used in processEvent()
};

/*! @file
 * @brief Defines AlanBehavior, a little demo behavior which lifts a leg higher as more pressure is put on the back head button
 * @author ejt (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.17 $
 * $State: Exp $
 * $Date: 2007/11/12 18:00:38 $
 */

#endif
