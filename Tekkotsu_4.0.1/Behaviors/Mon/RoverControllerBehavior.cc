#include "Shared/RobotInfo.h"
#if ( defined(TGT_HAS_ARMS) || defined(TGT_HAS_LEGS) ) && defined(TGT_HAS_HEAD)

#include "RoverControllerBehavior.h"

#include "Motion/MMAccessor.h"
#include <sstream>

using namespace std; 

void RoverControllerBehavior::DoStart() {
	BehaviorBase::DoStart(); // do this first (required)
	// open server socket
	cmdsock=wireless->socket(Socket::SOCK_STREAM, 2048, 2048);
	wireless->setReceiver(cmdsock, callback);
	wireless->setDaemon(cmdsock,true);
	wireless->listen(cmdsock, PORT);
	// Open the gui on the desktop
	Controller::loadGUI("org.tekkotsu.mon.RoverGUI","RoverGUI",PORT);
	// set up motion commands
	head = motman->addPersistentMotion(SharedObject<HeadPointerMC>());
	arm = motman->addPersistentMotion(SharedObject<PostureMC>());
}

void RoverControllerBehavior::DoStop() {
	// remove motion commands
	motman->removeMotion(head);
	head=MotionManager::invalid_MC_ID;
	motman->removeMotion(arm);
	arm=MotionManager::invalid_MC_ID;
	// Close the GUI
	Controller::closeGUI("RoverGUI");
	// close server socket
	wireless->setDaemon(cmdsock,false);
	wireless->close(cmdsock);
	BehaviorBase::DoStop(); // do this last (required)
}

void RoverControllerBehavior::initScale() {
	PostureEngine e;
	unsigned int gripperFrame=-1U, armBaseFrame=-1U;
#ifdef TGT_HAS_ARMS
	gripperFrame=ArmOffset+NumArmJoints-1;
	armBaseFrame=ArmOffset;
#elif defined(TGT_HAS_LEGS)
	gripperFrame=PawFrameOffset;
	armBaseFrame=LFrLegOffset;
#else
	// RoverControllerBehavior can't find manipulator, won't run
	// (user should've seen the compiler warning when including the header...)
	return;
#endif
	
	// find something resembling range of motion
	const float BIGNUM=1000000;
	e.solveLinkPosition(BIGNUM,0,0, gripperFrame, 0,0,0);
	NEWMAT::ColumnVector x = e.jointToBase(gripperFrame).SubMatrix(1,4,4,4);
	float max=ceil(x(1)/x(4));
	x = e.jointToBase(armBaseFrame).SubMatrix(1,4,4,4);
	tgtXOff=x(1)/x(4);
	tgtScale=(max-tgtXOff)/2;
	tgtXOff+=tgtScale;
	x = e.jointToBase(armBaseFrame+1).SubMatrix(1,4,4,4);
	tgtYOff=x(2)/x(4);
	std::cout << "Gripper XY reach (-1 to 1): " << tgtXOff-tgtScale << " to " << tgtXOff+tgtScale << std::endl;
	std::cout << "Gripper XY center: (" << tgtXOff << ',' << tgtYOff <<')' << std::endl;
	
#ifdef TGT_HAS_ARMS
	// arm just goes up
	e.solveLinkPosition(0,0,BIGNUM, gripperFrame, 0,0,0);
	x = e.jointToBase(gripperFrame).SubMatrix(1,4,4,4);
	max=ceil(x(3)/x(4));
	x = e.jointToBase(armBaseFrame).SubMatrix(1,4,4,4);
	heightOff=x(3)/x(4);
	heightScale=max-heightOff;
#elif defined(TGT_HAS_LEGS)
	// legs start at bottom
	e.solveLinkPosition(0,0,-BIGNUM, gripperFrame, 0,0,0);
	x = e.jointToBase(gripperFrame).SubMatrix(1,4,4,4);
	heightOff=floor(x(3)/x(4));
	x = e.jointToBase(armBaseFrame).SubMatrix(1,4,4,4);
	heightScale=(x(3)/x(4)-heightOff)*2;
#endif
	std::cout << "Gripper Z reach (0 to 1):" << heightOff << " to " << heightOff+heightScale << std::endl;
	
	e.solveLinkPosition(0,0,-BIGNUM, gripperFrame, 0,0,0);
	e.solveLinkPosition(0,0,BIGNUM, CameraFrameOffset, 0,0,0);
	float ydist=e.jointToJoint(gripperFrame,CameraFrameOffset).SubMatrix(1,3,4,4).NormFrobenius();
	e.solveLinkPosition(BIGNUM,0,0, gripperFrame, 0,0,0);
	e.solveLinkPosition(-BIGNUM,0,0, CameraFrameOffset, 0,0,0);
	float xdist=e.jointToJoint(gripperFrame,CameraFrameOffset).SubMatrix(1,3,4,4).NormFrobenius();
	distanceScale = ceil(xdist>ydist ? xdist : ydist);
	std::cout << "Max camera distance is " << distanceScale << std::endl;
}

int RoverControllerBehavior::update(char *buf, int bytes) {
	stringstream cmd(string(buf,bytes));
	
	bool armDirty=false, headDirty=false;
	
	while(cmd) {
		string t;
		cmd >> t;
		bool gotArm=false, gotHead=false;
		if(gotHead=(t=="autoPerspective"))
			cmd >> autoPerspective;
		else if(gotArm=(t=="autoAngle"))
			cmd >> autoGripperAngle;
		else if(gotHead=(t=="autoTrack"))
			cmd >> autoTrackGripper;
		else if(gotHead=(t=="maxDist"))
			cmd >> maxDist;
		
		else if(t=="gripper") {
			float v;
			cmd >> v;
			unsigned int idx=-1U;
#ifdef TGT_HAS_ARMS
			idx=ArmOffset+JointsPerArm-1;
#elif defined(TGT_HAS_MOUTH)
			if(NumMouthJoints>0)
				idx=MouthOffset;
#endif
			if(idx!=-1U) {
				float range = outputRanges[idx][MaxRange] - outputRanges[idx][MinRange];
				v = v*range + outputRanges[idx][MinRange];
				MMAccessor<PostureMC>(arm)->setOutputCmd(idx,v);
			}
		}
		else if(gotArm=(t=="gripperAngle"))
			cmd >> gripperAngle;
		else if(gotHead=(t=="perspective"))
			cmd >> perspective;
		else if(gotArm=(t=="gripperHeight")) {
			cmd >> tgtZ;
			tgtZ*=heightScale;
			tgtZ+=heightOff;
		}
		else if(gotHead=(t=="cameraDistance")) {
			cmd >> lookD;
			lookD*=distanceScale;
		}

		else if(gotArm=(t=="tgt")) {
			cmd >> tgtY >> tgtX;
			tgtY=-tgtY;
			tgtX*=tgtScale;
			tgtX+=tgtXOff;
			tgtY*=tgtScale;
			tgtY+=tgtYOff;
		}
		else if(gotHead=(t=="look")) {
			cmd >> lookY >> lookX;
			lookY=-lookY;
			lookX*=tgtScale;
			lookX+=tgtXOff;
			lookY*=tgtScale;
			lookY+=tgtYOff;
		}
		
		armDirty = armDirty || gotArm;
		headDirty = headDirty || gotHead;
	}
	if(autoTrackGripper) {
		lookX=tgtX;
		lookY=tgtY;
		headDirty = headDirty || armDirty;
	}
	
	if(armDirty)
		updateArm();
	
	if(headDirty)
		updateHead();
	
	return 0;
}

RoverControllerBehavior * RoverControllerBehavior::theOne=NULL;

void RoverControllerBehavior::updateArm() {
#ifdef TGT_HAS_ARMS
	MMAccessor<PostureMC>(arm)->solveLinkPosition(tgtX,tgtY,tgtZ,ArmOffset+NumArmJoints-1,0,0,0);
#elif defined(TGT_HAS_LEGS)
	MMAccessor<PostureMC>(arm)->solveLinkPosition(tgtX,tgtY,tgtZ,PawFrameOffset,0,0,0);
#endif
}

void RoverControllerBehavior::updateHead() {
	if(maxDist)
		MMAccessor<HeadPointerMC>(head)->lookAtPoint(lookX,lookY,tgtZ);
	else
		MMAccessor<HeadPointerMC>(head)->lookAtPoint(lookX,lookY,tgtZ,lookD);
}


/*! @file
 * @brief 
 * @author Ethan Tira-Thompson (ejt) (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.5 $
 * $State: Exp $
 * $Date: 2007/11/18 06:47:01 $
 */

#endif // check for (arms or hands) and legs
