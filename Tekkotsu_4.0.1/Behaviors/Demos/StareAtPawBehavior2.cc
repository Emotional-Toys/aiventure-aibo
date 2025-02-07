#include "Shared/RobotInfo.h"
#if defined(TGT_HAS_LEGS)

#include "StareAtPawBehavior2.h"

#include "Events/EventRouter.h"
#include "Shared/WorldState.h"
#include "Motion/MMAccessor.h"
#include "Motion/PIDMC.h"
#include "Motion/HeadPointerMC.h"
#include "Motion/PostureMC.h"
#include "IPC/SharedObject.h"
#include "Motion/roboop/robot.h"
#include "Shared/Config.h"
#include "Motion/Kinematics.h"
#include "Shared/ERS7Info.h"

// these are for drawing into the camera frame
#include "Shared/ProjectInterface.h"
#include "Vision/Graphics.h"
#include "Events/FilterBankEvent.h"
#include "Vision/RawCameraGenerator.h"
#include "Behaviors/Mon/RawCamBehavior.h"

//better to put this here instead of the header
using namespace std; 

void StareAtPawBehavior2::DoStart() {
	BehaviorBase::DoStart(); // do this first

	pointID=motman->addPersistentMotion(SharedObject<HeadPointerMC>());

	//sensorEGID to update head pointer whenever we get new info about joint positions
	erouter->addListener(this,EventBase::sensorEGID);
	//buttonEGID to switch the target when the LFr or RFr button is pressed (ignores the others)
	erouter->addListener(this,EventBase::buttonEGID);
	//visRawCameraEGID to draw a dot on the camera image where we think the toe is
	erouter->addListener(this,EventBase::visRawCameraEGID,ProjectInterface::visRawCameraSID,EventBase::statusETID);
		
	if(RobotName == ERS7Info::TargetName)
		sout->printf("NOTICE: The ERS-7 has a rather \"sticky\" nod joint\n"
								 "(the upper tilt joint).  This can cause it to hesitate\n"
								 "or altogether fail to precisely center the target position\n"
								 "vertically in the center of the image...\n");
}

void StareAtPawBehavior2::DoStop() {
	motman->removeMotion(pointID);
	pointID=MotionManager::invalid_MC_ID;
	erouter->removeListener(this);
	BehaviorBase::DoStop(); // do this last
}
	
void StareAtPawBehavior2::processEvent(const EventBase& e) {
	if(e.getGeneratorID()==EventBase::buttonEGID) {
		//*******************************//
		//*** Switch the "active" toe ***//
		//*******************************//

		if(e.getSourceID()==LFrPawOffset) {
			lastLeg=LFrLegOrder;
		} else if(e.getSourceID()==RFrPawOffset) {
			lastLeg=RFrLegOrder;
		} else
			return;
		if(e.getTypeID()==EventBase::activateETID) {
			unsigned int lastlegoff=LegOffset+lastLeg*JointsPerLeg;
			SharedObject<PIDMC> relaxLeg(lastlegoff,lastlegoff+JointsPerLeg,0);
			motman->addPrunableMotion(relaxLeg);
		} else if(e.getTypeID()==EventBase::deactivateETID) {
			unsigned int lastlegoff=LegOffset+lastLeg*JointsPerLeg;
			SharedObject<PIDMC> tightLeg(lastlegoff,lastlegoff+JointsPerLeg,1);
			motman->addPrunableMotion(tightLeg);
		}


	} else if(e.getGeneratorID()==EventBase::sensorEGID) {
		//***************************************************************************//
		//*** Update the position of the head based on new information on the toe ***//
		//***************************************************************************//

		//Which paw do we want?
		const char * ipname= (lastLeg==LFrLegOrder) ? "ToeLFrPaw" : "ToeRFrPaw";

		//Ask kinematics for current location of that paw (this is the "objective" aka target)
		NEWMAT::ColumnVector Pobj=kine->getJointInterestPoint(BaseFrameOffset,ipname);

		//Now point the head there
		MMAccessor<HeadPointerMC>(pointID)->lookAtPoint(Pobj(1),Pobj(2),Pobj(3)); //keep head as far away as possible
		//Alternative method:
		// MMAccessor<HeadPointerMC>(pointID)->lookAtPoint(Pobj(1),Pobj(2),Pobj(3),80); //keep head 80mm away


	} else if(e.getGeneratorID()==EventBase::visRawCameraEGID) {
		//**************************************************************//
		//*** Draw into the camera frame to put a box around the toe ***//
		//**************************************************************//
			
		//same as above, but get the toe position relative to the camera
		const char * ipname= (lastLeg==LFrLegOrder) ? "ToeLFrPaw" : "ToeRFrPaw";
		NEWMAT::ColumnVector Pobj=kine->getJointInterestPoint(CameraFrameOffset,ipname);
		float x,y;
		config->vision.computePixel(Pobj(1),Pobj(2),Pobj(3),x,y);
		y*=config->vision.aspectRatio;
		float r=atan2(10.f,sqrt(Pobj.SumSquare()))/CameraHorizFOV/2;
			
		//draw into the layer which will be sent to the gui
		//not doing any error checking on the type of the event or its source... cross your fingers ;)
		const FilterBankEvent& fbe=dynamic_cast<const FilterBankEvent&>(e);
		unsigned chan=RawCameraGenerator::CHAN_Y;
		unsigned int layer=RawCamBehavior::getSourceLayer(chan,fbe.getNumLayers());
		Graphics g(*fbe.getSource(), layer, chan);
		g.setColor(255);
		g.drawRect(x-r,y-r,r*2,r*2);
			
		/*
		//this method of drawing gets the pixel coordinates without applying
		// camera calibration via config->vision.computePixel
		g.setColor(192);
		r=atan2(5.f,sqrt(Pobj.SumSquare()))/CameraHorizFOV/2; //draws a half-size rectangle
		x=Pobj(1)/(Pobj(3)*tan(CameraHorizFOV/2));
		y=Pobj(2)/(Pobj(3)*tan(CameraVertFOV/2));
		y*=config->vision.aspectRatio;
		g.drawRect(x-r,y-r,r*2,r*2);
		*/

	} else {
		serr->printf("StareAtPawBehavior2: Unhandled event %s\n",e.getName().c_str());
	}
}

#endif // has legs

/*! @file
 * @brief Implements StareAtPawBehavior2, which uses new-style ROBOOP kinematics to track the paw which last received a button press with the camera
 * @author ejt (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.6 $
 * $State: Exp $
 * $Date: 2007/06/28 04:36:20 $
 */
