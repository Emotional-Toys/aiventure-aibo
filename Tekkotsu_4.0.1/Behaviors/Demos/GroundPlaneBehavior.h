//-*-c++-*-

#ifndef INCLUDED_GroundPlaneBehavior_h_
#define INCLUDED_GroundPlaneBehavior_h_

#include "Behaviors/BehaviorBase.h"
#include "Motion/Kinematics.h"
#include "Motion/PIDMC.h"
#include "Motion/MotionManager.h"
#include "Shared/ERS7Info.h"
#include "Shared/ERS2xxInfo.h"
#include "Shared/WorldState.h"
#include "Events/VisionObjectEvent.h"
#include "Shared/ProjectInterface.h"

//! Reports the location of the center of the camera image on the ground plane
class GroundPlaneBehavior : public BehaviorBase {
public:
	//! constructor
	GroundPlaneBehavior()
		: BehaviorBase("GroundPlaneBehavior"),
		head_release(EventBase::buttonEGID,HeadButOffset,EventBase::activateETID,0),
		head_lock(EventBase::buttonEGID,HeadButOffset,EventBase::deactivateETID,0),
		target_toggle(EventBase::buttonEGID,ChinButOffset,EventBase::activateETID),
		targeting(false),
		visual_target(EventBase::visObjEGID,ProjectInterface::visYellowBallSID,EventBase::statusETID),
		last_seen(0),
		clock(EventBase::timerEGID,0,EventBase::statusETID,250)
	{ }

	virtual void DoStart() {
		BehaviorBase::DoStart(); // do this first
		erouter->addListener(this,head_release);
		erouter->addListener(this,head_lock);
		erouter->addListener(this,target_toggle);
		processEvent(clock);
	}
	
	virtual void processEvent(const EventBase& e) {
		using std::cout;
		using std::endl;
		if(e==clock) {
			//This is the direction gravity is pulling - probably a good way to find out
			//the attitude of the robot, assuming it is not moving.
			//** Note that the LAccel sensor needs to be negated to match the coordinate system **//
			NEWMAT::ColumnVector down=Kinematics::pack(state->sensors[BAccelOffset],
			                                           -state->sensors[LAccelOffset],
			                                           state->sensors[DAccelOffset]);
			
			//Just for kicks, lets report which leg is off the ground
			cout << "I think leg " << kine->findUnusedLeg(down) << " is least used" << endl;

			//First we determine the ground plane
			NEWMAT::ColumnVector p=kine->calculateGroundPlane(down);
			cout << "Ground plane: " << p(1)<<"x + " << p(2)<<"y + " << p(3)<<"z = 1" << endl;

			//Project to ground plane - we do it twice here, once for camera frame and once for base frame
			NEWMAT::ColumnVector ray(4); ray(1)=ray(2)=0; ray(3)=ray(4)=1;
			NEWMAT::ColumnVector hit;
			//cout <<"Current head:\n"<<state->outputs[HeadOffset] <<' '<< state->outputs[HeadOffset+1] <<' '<< state->outputs[HeadOffset+2] << endl <<kine->getTransform(CameraFrameOffset);
			hit=kine->projectToPlane(CameraFrameOffset,ray,BaseFrameOffset,p,CameraFrameOffset);
			cout << "Intersection_camera: (" << hit(1)<<','<<hit(2)<<','<<hit(3)<<')'<<endl;
			hit=kine->projectToPlane(CameraFrameOffset,ray,BaseFrameOffset,p,BaseFrameOffset);
			cout << "Intersection_base: (" << hit(1)<<','<<hit(2)<<','<<hit(3)<<')'<<endl;

		} else if(e==head_release) {
			motman->addPrunableMotion(SharedObject<PIDMC>(HeadOffset,HeadOffset+NumHeadJoints,0));
			erouter->addTimer(this,clock);
			processEvent(clock);
		} else if(e==head_lock) {
			motman->addPrunableMotion(SharedObject<PIDMC>(HeadOffset,HeadOffset+NumHeadJoints,1));
			erouter->removeTimer(this,clock);
		} else if(e==target_toggle) {
			if(targeting) {
				erouter->removeListener(this,visual_target);
				targeting=false;
			} else {
				erouter->addListener(this,visual_target);
				targeting=true;
			}
			sndman->playFile("yap.wav");
		} else if(e==visual_target) {
			cout << '.' << std::flush;
			if(get_time()<last_seen+1000)
				return;
			cout << endl;
			
			const VisionObjectEvent& visob = dynamic_cast<const VisionObjectEvent&>(e);
			
			cout << "Target position: " << visob.getCenterX()<< " " << visob.getCenterY() << " normalized: " << visob.getCenterX()/visob.getXrange()<< " " << visob.getCenterY()/visob.getYrange() << endl;
			
			//First we determine the ground plane
			NEWMAT::ColumnVector p=kine->calculateGroundPlane();
			cout << "Ground plane: " << p(1)<<"x + " << p(2)<<"y + " << p(3)<<"z = 1" << endl;
			
			//Project to ground plane - we do it twice here, once for camera frame and once for base frame
			NEWMAT::ColumnVector ray(4);
			config->vision.computeRay(visob.getCenterX()/visob.getXrange(),visob.getCenterY()/visob.getYrange(),ray(1),ray(2),ray(3));
			ray(4)=1;
			NEWMAT::ColumnVector hit;
			//cout <<"Current head:\n"<<state->outputs[HeadOffset] <<' '<< state->outputs[HeadOffset+1] <<' '<< state->outputs[HeadOffset+2] << endl <<kine->getTransform(CameraFrameOffset);
			hit=kine->projectToPlane(CameraFrameOffset,ray,BaseFrameOffset,p,CameraFrameOffset);
			cout << "Intersection_camera: (" << hit(1)<<','<<hit(2)<<','<<hit(3)<<')'<<endl;
			hit=kine->projectToPlane(CameraFrameOffset,ray,BaseFrameOffset,p,BaseFrameOffset);
			cout << "Intersection_base: (" << hit(1)<<','<<hit(2)<<','<<hit(3)<<')'<<endl;
			last_seen=get_time();
		} else {
			ASSERT(false,"unprocessed event " << e.getName() << endl);
		}
	}

	static std::string getClassDescription() { return "Reports the location of the center of the camera image on the ground plane"; }
	virtual std::string getDescription() const { return getClassDescription(); }
	
protected:
	EventBase head_release; //!< event template to match to signal the head's PID joints should be relaxed
	EventBase head_lock;    //!< event template to match to signal the head's PID joints should be powered up again
	EventBase target_toggle; //!< event template to indicate that the behavior should watch for visual targets are report their position
	bool targeting; //!< whether currently targeting
	EventBase visual_target; //!< event template for the object to track the position of
	unsigned int last_seen;  //!< time that the last report regarding visual_target's position was displayed
	EventBase clock;        //!< event template to match to signal a new round of calculations should be performed
};

/*! @file
 * @brief Defines GroundPlaneBehavior, which reports the location of the center of the camera image on the ground plane
 * @author ejt (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.15 $
 * $State: Exp $
 * $Date: 2007/11/12 18:00:38 $
 */

#endif
