//-*-c++-*-
#ifndef INCLUDED_DrawSkeletonBehavior_h_
#define INCLUDED_DrawSkeletonBehavior_h_

#include "Behaviors/BehaviorBase.h"
#include "Events/EventRouter.h"

#include "Shared/ProjectInterface.h"
#include "Events/VisionObjectEvent.h"
#include "Events/FilterBankEvent.h"
#include "Vision/Graphics.h"
#include "Shared/Config.h"
#include "Vision/RawCameraGenerator.h"
#include "Behaviors/Mon/RawCamBehavior.h"
#include "Motion/Kinematics.h"
#include "Shared/newmat/newmatio.h"

//! Draws the kinematics "skeleton" on the camera frame
class DrawSkeletonBehavior : public BehaviorBase {
public:
	//! constructor
	DrawSkeletonBehavior() : BehaviorBase("DrawSkeletonBehavior") {}

	virtual void DoStart() {
		BehaviorBase::DoStart(); // do this first (required)
		erouter->addListener(this, EventBase::visRawCameraEGID,ProjectInterface::visRawCameraSID,EventBase::statusETID);
	}

	virtual void DoStop() {
		erouter->removeListener(this); //generally a good idea, unsubscribe all
		BehaviorBase::DoStop(); // do this last (required)
	}

	virtual void processEvent(const EventBase& e) {
		//We're assuming e is a filter bank event and that its source is not null...
		//You could add error checking code for that.
		const FilterBankEvent& fbe=dynamic_cast<const FilterBankEvent&>(e);

		unsigned int chan=RawCameraGenerator::CHAN_U;
		unsigned int layer=RawCamBehavior::getSourceLayer(chan,fbe.getNumLayers());
		unsigned char color=48;
		float originBoxSize=.1;
		
		Graphics g(*fbe.getSource(),layer,chan);
		g.setColor(color);

		NEWMAT::ColumnVector p=Kinematics::pack(0,0,0);
		for(unsigned int leg=0; leg<NumLegs; leg++) {
			float lastx,lasty,curx,cury;
			bool lastFront=false;
			bool front=getCameraPoint(LegOffset+leg*JointsPerLeg+ElevatorOffset,p,lastx,lasty);
			if(front) {
				g.setColor(-color);
				g.drawRect(lastx-originBoxSize/2,lasty-originBoxSize/2,originBoxSize,originBoxSize);
			}
			for(unsigned int j=KneeOffset; j<JointsPerLeg; j++) {
				//actually, there's only one more joint for this loop to process, but just for generality...
				lastFront=front;
				front=getCameraPoint(LegOffset+leg*JointsPerLeg+j,p,curx,cury);
				if(lastFront) {
					g.setColor(front?color:-color);
					g.drawLine(lastx,lasty,curx,cury);
				}
				if(front) {
					g.setColor(-color);
					g.drawRect(curx-originBoxSize/2,cury-originBoxSize/2,originBoxSize,originBoxSize);
				}
				lastx=curx;
				lasty=cury;
			}
			lastFront=front;
			front=getCameraPoint(PawFrameOffset+leg,p,curx,cury);
			if(lastFront) {
				g.setColor(front?color:-color);
				g.drawLine(lastx,lasty,curx,cury);
			}
			if(front) {
				g.setColor(-color);
				g.drawRect(curx-originBoxSize/2,cury-originBoxSize/2,originBoxSize,originBoxSize);
			}
			lastx=curx;
			lasty=cury;
		}
	}

	static std::string getClassDescription() { return "Draws the kinematics \"skeleton\" on the camera frame"; }
	virtual std::string getDescription() const { return getClassDescription(); }
	

protected:
	//! returns the @a x and @a y coordinates with the camera image corresponding to a point (@a p) in 3d space (relative to @a jointOffset)
	bool getCameraPoint(unsigned int jointOffset, NEWMAT::ColumnVector& p, float& x, float& y) {
		NEWMAT::Matrix T = kine->linkToJoint(jointOffset,CameraFrameOffset);
		NEWMAT::ColumnVector o=T*p; //o is now the position of point p on the link, relative to the camera
		bool front=o(3)>=0;
		config->vision.computePixel(o(1),o(2),o(3),x,y);
		//cout << jointOffset << ' ' << o.t() << " -> " << x << ' ' << y << endl;
		return front;
	}

	
private:
	// Providing declarations for these functions will avoid a compiler warning if
	// you have any class members which are pointers.  However, as it is, an error
	// will result if you inadvertantly cause a call to either (which is probably
	// a good thing, unless you really intended to copy/assign a behavior, in
	// which case simply provide implementations for the functions)
	DrawSkeletonBehavior(const DrawSkeletonBehavior&); //!< don't call (copy constructor)
	DrawSkeletonBehavior& operator=(const DrawSkeletonBehavior&); //!< don't call (assignment operator)
};

/*! @file
 * @brief Defines DrawSkeletonBehavior, which draws the kinematics "skeleton" on the camera frame
 * @author ejt (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.3 $
 * $State: Exp $
 * $Date: 2005/08/07 04:11:03 $
 */

#endif
