//-*-c++-*-
#ifndef INCLUDED_KinematicSampleBehavior_h_
#define INCLUDED_KinematicSampleBehavior_h_

#include "Behaviors/BehaviorBase.h"
#include "Motion/PIDMC.h"
#include "Motion/PostureMC.h"
#include "Motion/MotionManager.h"
#include "IPC/SharedObject.h"
#include "Motion/roboop/robot.h"
#include "Shared/Config.h"
#include "Motion/Kinematics.h"


//! Uses kinematics to mirror leg positions (note that it mirrors the paw position - not necessarily the joint angles used to get there!)
class KinematicSampleBehavior : public BehaviorBase {
public:
	//! constructor
	KinematicSampleBehavior()
		: BehaviorBase("KinematicSampleBehavior"), lastLeg(LFrLegOrder), poseID(MotionManager::invalid_MC_ID)
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
			//I'll use the pack and unpack functions here just to illustrate how to avoid 
			//needing to use NEWMAT data structures - but if you're going to be doing
			//any significant math, you'll eventually want to get comfortable with NEWMAT...

			// (Actually, I think the NEWMAT version would be more readable...)

			/******** Determine location of target position ********/
			float link_x=60,link_y=0,link_z=0; // 6cm along x axis of selected joint
			float obj_x=0, obj_y=0, obj_z=0;   // these will hold the objective position
			//this next line computes the link position, and stores result into obj_*
			Kinematics::unpack(kine->linkToBase(getIndex(lastLeg))*Kinematics::pack(link_x,link_y,link_z),
			                   obj_x,obj_y,obj_z);

			/******** Solve each leg for the point ********/
			MMAccessor<PostureMC> pose_acc(poseID); //
			for(unsigned int i=0; i<NumLegs; i++)
				if(i!=(unsigned int)lastLeg) {
					float m_x=((i<2)==((unsigned int)lastLeg<2))?obj_x:-obj_x;
					float m_y=((i%2)==((unsigned int)lastLeg%2))?obj_y:-obj_y;
					pose_acc->solveLinkPosition(Kinematics::pack(m_x,m_y,obj_z),
					                            getIndex((LegOrder_t)i),
					                            Kinematics::pack(link_x,link_y,link_z));
				}
			
			//If you would like to verify the positiions of the back toes... (relative to body center)
			//cout << "L: " << kine->getJointInterestPoint(BaseFrameOffset,"ToeLBkPaw").t();
			//cout << "R: " << kine->getJointInterestPoint(BaseFrameOffset,"ToeRBkPaw").t();
			
		} else {
			serr->printf("KinematicSampleBehavior: Unhandled event %s\n",e.getName().c_str());
		}
	}

	static std::string getClassDescription() { return "Uses kinematics to mirror leg positions (note that it mirrors the paw position - not necessarily the joint angles used to get there!)"; }
	virtual std::string getDescription() const { return getClassDescription(); }
	
protected:
	//! returns the index of the knee for the requested @a leg
	unsigned int getIndex(LegOrder_t leg) {
		//or try: return PawFrameOffset+leg;
		return LegOffset+leg*JointsPerLeg+KneeOffset;
	}
	LegOrder_t lastLeg; //!< the last leg to have its button pressed, i.e. the "source"
	MotionManager::MC_ID poseID; //!< the PostureMC which does all the computation
};

/*! @file
 * @brief Defines KinematicSampleBehavior, which uses kinematics to mirror leg positions (note that it mirrors the paw position - not necessarily the joint angles used to get there!)
 * @author ejt (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.10 $
 * $State: Exp $
 * $Date: 2005/06/01 05:47:45 $
 */

#endif
