//-*-c++-*-
#ifndef INCLUDED_LookAtPointBehavior_h_
#define INCLUDED_LookAtPointBehavior_h_

#include "Behaviors/BehaviorBase.h"
#include "Motion/HeadPointerMC.h"
#include "Motion/Kinematics.h"
#include "Motion/MotionManager.h"
#include "Shared/newmat/newmat.h"

//! A simple behavior to see how well LookAtPoint (inverse kinematics) is working
//! You may want to uncomment cout's in HeadPointer::LooAtPoint before running Behavior
class LookAtPointBehavior : public BehaviorBase {
 public:
  
  LookAtPointBehavior() : 
    BehaviorBase("LookAtPointBehavior"), 
    pointer_id(MotionManager::invalid_MC_ID), gazePt(500,0,0) { srand(clock()); }

  virtual void DoStart() {
    cout << "LookAtPointBehavior:DoStart()\n";
    BehaviorBase::DoStart();
    pointer_id = motman->addPersistentMotion(SharedObject<HeadPointerMC>());
    erouter->addListener(this, EventBase::motmanEGID, pointer_id, EventBase::statusETID);
    erouter->addTimer(this, 0, 1000, false);
  }
  virtual void DoStop() {
    motman->removeMotion(pointer_id);
    erouter->removeListener(this);
    BehaviorBase::DoStop();
  }

  virtual void processEvent(const EventBase& e) {
    switch (e.getGeneratorID()) {
    case EventBase::motmanEGID:
      if (e.getSourceID() == pointer_id) {
	erouter->addTimer(this, 0, 1000, false); // wait 1 sec for joints to stablize
      }
      break;
    case EventBase::timerEGID:
      if (e.getSourceID() == 0) {
	NEWMAT::ColumnVector down=Kinematics::pack(state->sensors[BAccelOffset],
						   -state->sensors[LAccelOffset],
						   state->sensors[DAccelOffset]);
	NEWMAT::ColumnVector p=kine->calculateGroundPlane(down);
	
	NEWMAT::ColumnVector ray(4); ray(1)=ray(2)=0; ray(3)=ray(4)=1;
	NEWMAT::ColumnVector hit;
	hit=kine->projectToPlane(CameraFrameOffset,ray,BaseFrameOffset,p,BaseFrameOffset);

	NEWMAT::ColumnVector groundPt = Kinematics::pack(gazePt.coordX(),gazePt.coordY(),gazePt.coordZ());
	NEWMAT::ColumnVector camPt = (kine->baseToJoint(CameraFrameOffset))*groundPt;
	groundPt = kine->projectToPlane(CameraFrameOffset,camPt,BaseFrameOffset,p,BaseFrameOffset);
	float theta = acos(camPt(3)/sqrt(camPt(1)*camPt(1)+camPt(2)*camPt(2)+camPt(3)*camPt(3)));
	cout << "Result:\n Cam Center projected to GroundPlane: " << Point(hit(1),hit(2),hit(3)) << endl;
	cout << " Gaze Point projected to GroundPlane: " << Point(groundPt(1),groundPt(2),groundPt(3)) << endl;
	cout << " Gaze Point in CameraPlane: " << Point(camPt(1),camPt(2),camPt(3)) << endl;
	cout << " theta: " << mathutils::rad2deg(theta) << " degrees\n";

	gazePt.setCoords(rand()%1000-200, rand()%1000-500, rand()%400-200); //set next gaze point
	erouter->addTimer(this, 1, 4000, false);
      }
      else {
	cout << "\n\n\nLookAtPoint: " << gazePt << endl;
	bool b = MMAccessor<HeadPointerMC>(pointer_id)->lookAtPoint(gazePt.coordX(), gazePt.coordY(), gazePt.coordZ());
	cout << " => " << (b ? "Reachable" : "Unreachable") << endl;
      }
      break;
    default:
      cout << "LookAtPointBehavior::ProcessEvent(): unknown event\n";
      break;
    };
  }
  
protected:
  MotionManager::MC_ID pointer_id;
  Point gazePt;
};

/*! @file
* @brief Defines LookAtPointBehavior, moves the head through a series of gaze points and reports if each is reachable to test inverse kinematic's lookAtPoint
* @author dst (Creator)
*
* $Author: ejt $
* $Name: tekkotsu-4_0-branch $
* $Revision: 1.3 $
* $State: Exp $
* $Date: 2006/09/22 16:59:27 $
*/

#endif
