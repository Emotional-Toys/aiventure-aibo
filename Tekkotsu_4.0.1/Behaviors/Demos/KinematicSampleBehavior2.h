//-*-c++-*-
#ifndef INCLUDED_KinematicSampleBehavior2_h_
#define INCLUDED_KinematicSampleBehavior2_h_

#include "Behaviors/BehaviorBase.h"
#include "Motion/PIDMC.h"
#include "Motion/PostureMC.h"
#include "Motion/MotionManager.h"
#include "IPC/SharedObject.h"
#include "Motion/roboop/robot.h"
#include "Shared/Config.h"
#include "Motion/Kinematics.h"


//! Uses kinematics to make the back toe (Toe{LR}BkPaw) touch the lower thigh (Lower{LeftBackL,RightBackR}FrThigh)
class KinematicSampleBehavior2 : public BehaviorBase {
public:
	//! constructor
	KinematicSampleBehavior2()
		: BehaviorBase("KinematicSampleBehavior2"), lastLeg(LFrLegOrder), poseID(MotionManager::invalid_MC_ID)
	{ }

	virtual void DoStart() {
		BehaviorBase::DoStart(); // do this first
		poseID=motman->addPersistentMotion(SharedObject<PostureMC>());
		erouter->addListener(this,EventBase::sensorEGID);
		erouter->addListener(this,EventBase::buttonEGID);
	}

	virtual void DoStop() {
		motman->removeMotion(poseID);
		poseID=MotionManager::invalid_MC_ID;
		erouter->removeListener(this);
		BehaviorBase::DoStop(); // do this last
	}

	virtual void processEvent(const EventBase& e) {
		using std::cout;
		using std::endl;
		if(e.getGeneratorID()==EventBase::buttonEGID) {
			switch(e.getSourceID()) {
			case LFrPawOffset:
				lastLeg=LFrLegOrder; break;
			case RFrPawOffset:
				lastLeg=RFrLegOrder; break;
			case LBkPawOffset:
				lastLeg=LBkLegOrder; break;
			case RBkPawOffset:
				lastLeg=RBkLegOrder; break;
			default:
				return;
			}
			if(e.getTypeID()==EventBase::activateETID) {
				unsigned int lastlegoff=LegOffset+lastLeg*JointsPerLeg;
				SharedObject<PIDMC> relaxLeg(lastlegoff,lastlegoff+JointsPerLeg,0);
				motman->addPrunableMotion(relaxLeg);
				MMAccessor<PostureMC> pose_acc(poseID);
				for(unsigned int i=0; i<JointsPerLeg; i++)
					pose_acc->setOutputCmd(lastlegoff+i,OutputCmd::unused);
			} else if(e.getTypeID()==EventBase::deactivateETID) {
				unsigned int lastlegoff=LegOffset+lastLeg*JointsPerLeg;
				SharedObject<PIDMC> tightLeg(lastlegoff,lastlegoff+JointsPerLeg,1);
				motman->addPrunableMotion(tightLeg);
			}

		} else if(e.getGeneratorID()==EventBase::sensorEGID) {
			//Plan A:
			NEWMAT::ColumnVector obj(4);
			switch(lastLeg) {
			case LFrLegOrder:
				obj=kine->getJointInterestPoint(BaseFrameOffset,"LowerInnerBackLFrThigh,LowerOuterBackLFrThigh"); break;
			case RFrLegOrder:
				obj=kine->getJointInterestPoint(BaseFrameOffset,"LowerInnerBackRFrThigh,LowerOuterBackRFrThigh"); break;
			case LBkLegOrder:
				obj=kine->getJointInterestPoint(BaseFrameOffset,"LowerInnerFrontLBkThigh"); break;
			case RBkLegOrder:
				obj=kine->getJointInterestPoint(BaseFrameOffset,"LowerInnerFrontRBkThigh"); break;
			}
			if(obj(4)!=1)
				return;
			
			unsigned int solveLink=PawFrameOffset+((lastLeg+2)%NumLegs); //swap front/back
			NEWMAT::ColumnVector link(4);
			switch(lastLeg) {
			case LFrLegOrder:
				link=kine->getLinkInterestPoint(solveLink,"ToeLBkPaw"); break;
			case RFrLegOrder:
				link=kine->getLinkInterestPoint(solveLink,"ToeRBkPaw"); break;
			case LBkLegOrder:
				link=kine->getLinkInterestPoint(solveLink,"LowerInnerBackLFrShin"); break;
			case RBkLegOrder:
				link=kine->getLinkInterestPoint(solveLink,"LowerInnerBackRFrShin"); break;
			}
			if(link(4)!=1)
				return;
			
			//use the knee angle to assign distance from the solution point
			float dist=state->outputs[LegOffset+lastLeg*JointsPerLeg+KneeOffset];
			dist*=30/outputRanges[LegOffset+lastLeg*JointsPerLeg+KneeOffset][MaxRange]; //scale to go up to 3 cm away
			cout << "Distance is " << dist/10 << "cm" << endl;
			float curlen=sqrt(link.SubMatrix(1,3,1,1).SumSquare());
			//Two ways to do the same thing:
			if(lastLeg==LFrLegOrder || lastLeg==LBkLegOrder)
				link.SubMatrix(1,3,1,1)*=(dist+curlen)/curlen; //scale the vector components individually
			else
				link(4)=curlen/(dist+curlen); //scale along the vector using the homogenous coordinate

			//Plan B:
			/*
			NEWMAT::ColumnVector obj(4);
			switch(lastLeg) {
			case LFrLegOrder:
				obj=kine->getJointInterestPoint(BaseFrameOffset,"LowerLeftBackLFrShin,LowerRightBackLFrShin"); break;
			case RFrLegOrder:
				obj=kine->getJointInterestPoint(BaseFrameOffset,"LowerLeftBackRFrShin,LowerRightBackRFrShin"); break;
			case LBkLegOrder:
				obj=kine->getJointInterestPoint(BaseFrameOffset,"ToeLBkPaw"); break;
			case RBkLegOrder:
				obj=kine->getJointInterestPoint(BaseFrameOffset,"ToeRBkPaw"); break;
			}
			if(obj(4)!=1)
				return;
			
			unsigned int solveLink=PawFrameOffset+((lastLeg+2)%NumLegs); //swap front/back
			NEWMAT::ColumnVector link(4);
			switch(lastLeg) {
			case LFrLegOrder:
				link=kine->getLinkInterestPoint(solveLink,"ToeLBkPaw"); break;
			case RFrLegOrder:
				link=kine->getLinkInterestPoint(solveLink,"ToeRBkPaw"); break;
			case LBkLegOrder:
				link=kine->getLinkInterestPoint(solveLink,"LowerLeftBackLFrShin,LowerRightBackLFrShin"); break;
			case RBkLegOrder:
				link=kine->getLinkInterestPoint(solveLink,"LowerLeftBackRFrShin,LowerRightBackRFrShin"); break;
			}
			if(link(4)!=1)
				return;
			*/

			MMAccessor<PostureMC> pose_acc(poseID);
			pose_acc->solveLinkPosition(obj,solveLink,link);

			//If you would like to verify the positiions of the back toes... (relative to body center)
			//cout << "L: " << kine->getJointInterestPoint(BaseFrameOffset,"ToeLBkPaw").t();
			//cout << "R: " << kine->getJointInterestPoint(BaseFrameOffset,"ToeRBkPaw").t();
			//cout << "Toe: " << pose_acc->getJointInterestPoint(BaseFrameOffset,"ToeLBkPaw").t();
			//cout << "PawA: " << pose_acc->getJointInterestPoint(BaseFrameOffset,"LBkPaw").t();
			//cout << "PawB: " << (pose_acc->jointToJoint(PawFrameOffset+LBkLegOrder,BaseFrameOffset)*Kinematics::pack(0,0,0)).t();
			
		} else {
			serr->printf("KinematicSampleBehavior2: Unhandled event %s\n",e.getName().c_str());
		}
	}

	static std::string getClassDescription() { return "Uses kinematics to make the back toe (Toe{LR}BkPaw) touch the lower thigh (Lower{LeftBackL,RightBackR}FrThigh)"; }
	virtual std::string getDescription() const { return getClassDescription(); }
	
protected:
	LegOrder_t lastLeg; //!< the last leg to have its button pressed, i.e. the "source"
	MotionManager::MC_ID poseID; //!< the PostureMC which does all the computation
};

/*! @file
 * @brief Defines KinematicSampleBehavior2, which uses kinematics to make the back toe (Toe{LR}BkPaw) touch the lower thigh (Lower{LeftBackL,RightBackR}FrThigh)
 * @author ejt (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.14 $
 * $State: Exp $
 * $Date: 2007/11/12 18:00:38 $
 */

#endif
