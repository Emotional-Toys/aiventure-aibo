#include "Shared/RobotInfo.h"
#ifdef TGT_HAS_IR_DISTANCE

#include "WallTestBehavior.h"
#include "Shared/newmat/newmat.h"
#include "Shared/newmat/newmatap.h"
#include "Events/EventRouter.h"
#include "Motion/MotionManager.h"
#include "Motion/MotionSequenceMC.h"
#include "Shared/Config.h"
#include "Shared/WorldState.h"
#include "Shared/newmat/newmatio.h"
#include "Shared/TimeET.h"
#include <fstream>

using namespace std;

void
WallTestBehavior::DoStart() {
	BehaviorBase::DoStart(); // do this first
	
	// We'll set these two variables to the time at which we should
	// start and stop recording ir values
	int startrec,stoprec;
	
	// This motion sequence will rotate the head left, then right, then back to the middle.
	SharedObject<SmallMotionSequenceMC> pan;
	// stand up, and move the head to the left, taking `reposTime` to do it
	pan->setTime(startrec=reposTime); // reposTime is defined in the header file
	pan->setPose(PostureEngine(config->motion.makePath("stand.pos").c_str()));
	pan->setOutputCmd(HeadOffset+TiltOffset,0);
	pan->setOutputCmd(HeadOffset+PanOffset,outputRanges[HeadOffset+PanOffset][MaxRange]);
	pan->setOutputCmd(HeadOffset+RollOffset,0);
	// now pan to the right, taking `panTime`
	stoprec=pan->advanceTime(panTime);
	pan->setOutputCmd(HeadOffset+TiltOffset,0);
	pan->setOutputCmd(HeadOffset+PanOffset,outputRanges[HeadOffset+PanOffset][MinRange]);
	pan->setOutputCmd(HeadOffset+RollOffset,0);
	// center the head
	pan->advanceTime(reposTime);
	pan->setOutputCmd(HeadOffset+TiltOffset,0);
	pan->setOutputCmd(HeadOffset+PanOffset,0);
	pan->setOutputCmd(HeadOffset+RollOffset,0);
	// this second repetition simply forces the head to stay still for a little longer
	pan->advanceTime(reposTime);
	pan->setOutputCmd(HeadOffset+TiltOffset,0);
	pan->setOutputCmd(HeadOffset+PanOffset,0);
	pan->setOutputCmd(HeadOffset+RollOffset,0);
	// now we add the motion sequence to the motion manager
	// this will cause it to start executing
	motman->addPrunableMotion(pan);
	
	// add timers to start and stop sensor recording at the
	// times we stored in startrec and stoprec
	erouter->addTimer(this,0,startrec+lagTime,false); // SID==0 start recording
	erouter->addTimer(this,1,stoprec+lagTime,false);  // SID==1 stop recording
}

void
WallTestBehavior::DoStop() {
	erouter->removeListener(this);
	BehaviorBase::DoStop(); // do this last
}

void
WallTestBehavior::processEvent(const EventBase& e) {
	if(e.getGeneratorID()==EventBase::sensorEGID) {
		// The ERS-7 has different IR sensors than the 2xx series
		// So we need to do a bit of different code depending on the target model
#ifdef TGT_ERS7
		float nd = state->sensors[NearIRDistOffset];
		/* This code tried to be smart by picking between near or far
		 * However, without additional calibration work, the two are
		 * inconsistent.  Current calibration handles direction of the
		 * beam, but we don't (yet) have any calibration of measurement
		 * error due to sensor reading long (or short).
		 * Instead, the following 'false' causes only the 'far' sensor
		 * to be used */
		if(false && nd<350)
			usedNear.push_back(true);
		else {
			nd=state->sensors[FarIRDistOffset];
			usedNear.push_back(false);
		}
#else //not TGT_ERS7
		float nd = state->sensors[IRDistOffset];
#endif //not TGT_ERS7
		float na = state->outputs[HeadOffset+PanOffset];
		//cout << nd << ' ' << na << endl;
		// Just store the measurements for later analysis
		d.push_back(nd);
		a.push_back(na);

	} else if(e.getSourceID()==0) {
		// must be a timer event -- SID 0 indicates start of recording
		// so start subscribing to sensor events
		erouter->addListener(this,EventBase::sensorEGID,SensorSrcID::UpdatedSID);

	} else if(e.getSourceID()==1) {
		// must be a timer event -- SID 1 indicates stop of recording
		// so stop subscribing to sensor events
		erouter->removeListener(this,EventBase::sensorEGID);
				
		PostureEngine pose;
		pose.clear();
		//float legheight=NEWMAT::ColumnVector(pose.getJointInterestPoint(BaseFrameOffset,"LFrPaw"))(3);

		/* Extremely simplistic estimation.
		 * Assumes all readings are relative to the center of head rotation */
		cout << "Logging Non-Kinematic calculations to /data/raw_xy.txt" << endl;
		{
			ofstream rawxy(config->portPath("data/raw_xy.txt").c_str());
			if(!rawxy) {
				cout << "Could not open file" << endl;
			} else {
				cout << "Columns are:\tx\ty" << endl;
				for(unsigned int i=0; i<d.size(); i++)
					rawxy << d[i]*cos(a[i]) << '\t' << d[i]*sin(a[i]) << '\n';
			}
		}

		/* Less simplistic estimation.
		 * Uses interest point information to determine location of
		 * IR sensor from center of rotation, but doesn't make use
		 * of information regarding direction of the beam */
		cout << "Logging uncalibrated kinematic calculations to /data/k_xyz.txt" << endl;
		{
			ofstream kxys(config->portPath("data/k_xyz.txt").c_str());
			if(!kxys) {
				cout << "Could not open file" << endl;
			} else {
				// `off` will store the offset of the sensor from center of rotation
#ifdef TGT_ERS7
				cout << "Columns are:\tx\ty\tz\tis_using_near" << endl;
				float off=NEWMAT::ColumnVector(pose.getJointInterestPoint(CameraFrameOffset,"NearIR"))(3);
#else //not TGT_ERS7
				cout << "Columns are:\tx\ty\tz" << endl;
				float off=NEWMAT::ColumnVector(pose.getJointInterestPoint(CameraFrameOffset,"IR"))(3);
#endif //TGT_ERS7 check
				for(unsigned int i=0; i<d.size(); i++) {
					pose(HeadOffset+PanOffset)=a[i];
					NEWMAT::ColumnVector hit=pose.jointToBase(CameraFrameOffset)*Kinematics::pack(0,0,d[i]+off);
#ifdef TGT_ERS7
					kxys << hit(1) << '\t' << hit(2) << '\t' << hit(3) << '\t' << usedNear[i] << '\n';
#else //not TGT_ERS7
					kxys << hit(1) << '\t' << hit(2) << '\t' << hit(3) << '\n';
#endif //TGT_ERS7 check
				}
			}
		}

		/* Decent estimation.
		 * Uses full kinematic information to determine 3D location
		 * of the point in space being measured.  These measurements
		 * can be then projected into the ground plane to get a more
		 * accurate estimation from the walls.
		 * However, still doesn't take into account any sensor error,
		 * such as offsets or scaling issues. (unless such calibration
		 * has been added into WorldState itself after the time of this
		 * writing...) */
		cout << "Logging calibrated kinematic calculations to /data/ck_xyz.txt" << endl;
		{
			ofstream ckxys(config->portPath("data/ck_xyz.txt").c_str());
			if(!ckxys) {
				cout << "Could not open file" << endl;
			} else {
#ifdef TGT_ERS7
				cout << "Columns are:\tx\ty\tz\tis_using_near" << endl;
#else //not TGT_ERS7
				cout << "Columns are:\tx\ty\tz" << endl;
#endif //not TGT_ERS7
				for(unsigned int i=0; i<d.size(); i++) {
					pose(HeadOffset+PanOffset)=a[i];
#ifdef TGT_ERS7
					unsigned int frame=usedNear[i]?NearIRFrameOffset:FarIRFrameOffset;
					NEWMAT::ColumnVector hit=pose.jointToBase(frame)*Kinematics::pack(0,0,d[i]);
					ckxys << hit(1) << '\t' << hit(2) << '\t' << hit(3) << '\t' << usedNear[i] << '\n';
#else //not TGT_ERS7
					NEWMAT::ColumnVector hit=pose.jointToBase(IRFrameOffset)*Kinematics::pack(0,0,d[i]);
					ckxys << hit(1) << '\t' << hit(2) << '\t' << hit(3) << '\n';
#endif //not TGT_ERS7
				}
			}
		}

		//find data regions
		// This sections the angle of the head into left, forward,
		// and right regions, which assumes the walls will be roughly
		// orthogonal
		unsigned int start[3];
		unsigned int stop[3];
		start[0]=stop[0]=0;
		while(a[stop[0]++]>60*M_PI/180 && stop[0]<a.size()) ;
		start[1]=stop[0];
		while(a[start[1]++]>30*M_PI/180 && start[1]<a.size()) ;
		stop[1]=start[1];
		while(a[stop[1]++]>-30*M_PI/180 && stop[1]<a.size()) ;
		start[2]=stop[1];
		while(a[start[2]++]>-60*M_PI/180 && start[2]<a.size()) ;
		stop[2]=a.size();
			
		cout << "Regions are: ";
		for(int i=0; i<3; i++)
			cout << start[i] << "-" << stop[i] << (i==2?"":", ");
		cout << endl;

		//now process each of those regions independently
		for(int w=0; w<3; w++) {
			cout << "Wall "<<w<<": ";
			//convert radial coordinates to x,y coordinates
			int n=stop[w]-start[w];
			if(n<3) {
				cout << "not enough data - did you forget to turn off estop?" << endl;
				continue;
			}
			vector<float> x,y;
			for(unsigned int i=start[w]; i<stop[w]; i++) {
#ifdef TGT_ERS7
				if(d[i]==200 || d[i]>1500) //limits for the far sensor, near sensor is [50,500]
					continue;
#else //not ers-7
				if(d[i]==100 || d[i]>700)
					continue;
#endif //not ers-7
				x.push_back(d[i]*cos(a[i]));
				y.push_back(d[i]*sin(a[i]));
			}
			cout << "(" << x.size() << " valid samples)" << endl;
			if(x.size()<3) {
				cout << "No wall" << endl;
				continue;
			}


			// Now that we've done some setup and sanity checking, do
			// the actual linear least squares computation to fit a line
			// There's two methods we'll test, one using QR factorization
			// and the other using SVD decomposition.
			float x0=0,x1=0;
			TimeET t;

			t.Set();
			solve1(x,y,x0,x1);
			cout << "   QR: 'y = "<<x0<<"x + "<<x1<<"'\tTime:"<<t.Age()<<endl;

			t.Set();
			solve2(x,y,x0,x1);
			cout << "  SVD: '"<<x0<<"x + "<<x1<<"y = 1'\tTime:"<<t.Age()<<endl;
		}
	}
}

void
WallTestBehavior::solve1(const std::vector<float>& x, const std::vector<float>& y, float& x0, float& x1) {
	NEWMAT::Matrix A(x.size(),2); //x values in first column, 1's in second column (homogenous coordinates)
	NEWMAT::ColumnVector b(x.size()); //vector of y values
	for(unsigned int i=0; i<x.size(); i++) {
		A(i+1,1)=x[i];
		A(i+1,2)=1;
		b(i+1)=y[i];
	}

	NEWMAT::UpperTriangularMatrix U;
	NEWMAT::Matrix M;

	NEWMAT::QRZ(A,U);
	NEWMAT::QRZ(A,b,M);
	NEWMAT::ColumnVector dq = U.i()*M;

	x0=dq(1);
	x1=dq(2);
}

void
WallTestBehavior::solve2(const std::vector<float>& x, const std::vector<float>& y, float& x0, float& x1) {
	NEWMAT::Matrix A(x.size(),3); //x values in first column, y's in the second, -1's in third column (homogenous coordinates)
	for(unsigned int i=0; i<x.size(); i++) {
		A(i+1,1)=x[i];
		A(i+1,2)=y[i];
		A(i+1,3)=-1;
	}
	
	NEWMAT::DiagonalMatrix Q;
	NEWMAT::Matrix U,V;
	
	NEWMAT::SVD(A,Q,U,V,false,true);
	
	x0=V(1,V.ncols())/V(3,V.ncols());
	x1=V(2,V.ncols())/V(3,V.ncols());
}

#endif
