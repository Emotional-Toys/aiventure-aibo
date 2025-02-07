#define TK_ENABLE_CONFIG 
#define TK_ENABLE_SOUTSERR
#define TK_ENABLE_KINEMATICS
#include "local/minisim.h"

#include "Motion/PostureEngine.h"
#include "Shared/newmat/newmatio.h"
#include <iostream>
#include <stdlib.h>
#include <unistd.h>

namespace ROBOOP {
void serrprintf(const char* msg, int x, int y, int z) {
	printf(msg,x,y,z);
}
};

using namespace std;
using namespace NEWMAT;
using namespace minisim;

int main(int /*argc*/, char** /*argv*/) {
	if(chdir("../../../project")!=0) {
		perror("could not cd to project directory ../../../project (need config files)");
		return 1;
	}
	initialize();

	
	PostureEngine pose;
	for(unsigned int i=0; i<NumOutputs; i++)
		pose(i).weight=0;
	for(unsigned int i=PIDJointOffset; i<PIDJointOffset+NumPIDJoints; i++)
		pose(i).weight=1;
	
	ColumnVector Pobj(4);
	//Pobj(1)=100;
	//Pobj(2)=90;
	//Pobj(3)=-110;
	Pobj(1)=100;
	Pobj(2)=90;
	Pobj(3)=-110;
	Pobj(4)=1;
	
	ColumnVector Plink(4);
	Plink(1)=10;
	Plink(2)=10;
	Plink(3)=10;
	Plink(4)=1;

#ifdef TGT_HAS_LEGS

	cout << pose.getLinkInterestPoint(BaseFrameOffset,"LFrPaw");
	
	/*for(unsigned int i=LFrLegOffset; i<LFrLegOffset+JointsPerLeg; i++)
		cout << outputNames[i] << ' ' << pose(i).value << endl;
	cout << pose.getTransformFrames(LFrLegOffset+KneeOffset,LFrLegOffset+KneeOffset) << endl<<endl;
	cout << pose.getTransformFrames(LFrLegOffset+KneeOffset,LFrLegOffset+ElevatorOffset) << endl;
	cout << pose.getTransformFrames(LFrLegOffset+ElevatorOffset,LFrLegOffset+KneeOffset) << endl<<endl;
	cout << pose.getTransformFrames(LFrLegOffset+KneeOffset,LFrLegOffset+RotatorOffset) << endl;
	cout << pose.getTransformFrames(LFrLegOffset+RotatorOffset,LFrLegOffset+KneeOffset) << endl<<endl;
	cout << pose.getTransformFrames(LFrLegOffset+KneeOffset,BaseFrameOffset) << endl;
	cout << pose.getTransformFrames(BaseFrameOffset,LFrLegOffset+KneeOffset) << endl;
	*/

//	pose(LFrLegOffset+RotatorOffset)=.5;
//	pose(LFrLegOffset+ElevatorOffset)=.5;
//	pose(LFrLegOffset+KneeOffset)=.5;
	bool converged;
	converged=pose.solveLinkPosition(Pobj,PawFrameOffset+LFrLegOrder,Plink);
	cout << "LFr Converged=="<<converged<<endl;
	Pobj(2)=-Pobj(2);
	Plink(3)=-Plink(3);
	converged=pose.solveLinkPosition(Pobj,PawFrameOffset+RFrLegOrder,Plink);
	cout << "RFr Converged=="<<converged<<endl;
	Pobj(2)=-Pobj(2);
	Plink(3)=-Plink(3);
	Pobj(1)=-Pobj(1);
	converged=pose.solveLinkPosition(Pobj,PawFrameOffset+LBkLegOrder,Plink);
	cout << "LBk Converged=="<<converged<<endl;
	Pobj(2)=-Pobj(2);
	Plink(3)=-Plink(3);
	converged=pose.solveLinkPosition(Pobj,PawFrameOffset+RBkLegOrder,Plink);
	cout << "RBk Converged=="<<converged<<endl;
	Pobj(2)=-Pobj(2);
	Plink(3)=-Plink(3);
	Pobj(1)=-Pobj(1);

	Pobj(1)=300;
	Pobj(2)=-50;
	Pobj(3)=190;
	Plink=0; Plink(3)=1;
	converged=pose.solveLinkVector(Pobj,CameraFrameOffset,Plink);
	cout << "Camera Converged=="<<converged<<endl;

	for(unsigned int i=PIDJointOffset; i<PIDJointOffset+NumPIDJoints; i++)
		cout << outputNames[i] << ' ' << pose(i).value << endl;
		
/*	cout << pose.getTransformFrames(BaseFrameOffset,BaseFrameOffset) << endl;
	cout << pose.getTransformFrames(LFrLegOffset+RotatorOffset,BaseFrameOffset) << endl;
	cout << pose.getTransformFrames(LFrLegOffset+ElevatorOffset,BaseFrameOffset) << endl;
	cout << pose.getTransformFrames(LFrLegOffset+KneeOffset,BaseFrameOffset) << endl;
	cout << pose.getTransformFrames(PawFrameOffset+LFrLegOrder,BaseFrameOffset) << endl;
	
	cout << pose.getTransformLinks(BaseFrameOffset,BaseFrameOffset) << endl;
	cout << pose.getTransformLinks(LFrLegOffset+RotatorOffset,BaseFrameOffset) << endl;
	cout << pose.getTransformLinks(LFrLegOffset+ElevatorOffset,BaseFrameOffset) << endl;
	cout << pose.getTransformLinks(LFrLegOffset+KneeOffset,BaseFrameOffset) << endl;
	cout << pose.getTransformLinks(PawFrameOffset+LFrLegOrder,BaseFrameOffset) << endl; */
	
#endif
	
	destruct();
	return 0;
}

