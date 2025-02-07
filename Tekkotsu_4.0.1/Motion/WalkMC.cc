//This class is ported from Carnegie Mellon's 2001 Robosoccer entry, and falls under their license:
/*=========================================================================
    CMPack'02 Source Code Release for OPEN-R SDK v1.0
    Copyright (C) 2002 Multirobot Lab [Project Head: Manuela Veloso]
    School of Computer Science, Carnegie Mellon University
  -------------------------------------------------------------------------
    This software is distributed under the GNU General Public License,
    version 2.  If you do not have a copy of this licence, visit
    www.gnu.org, or write: Free Software Foundation, 59 Temple Place,
    Suite 330 Boston, MA 02111-1307 USA.  This program is distributed
    in the hope that it will be useful, but WITHOUT ANY WARRANTY,
    including MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  -------------------------------------------------------------------------
    Additionally licensed to Sony Corporation under the following terms:

    This software is provided by the copyright holders AS IS and any
    express or implied warranties, including, but not limited to, the
    implied warranties of merchantability and fitness for a particular
    purpose are disclaimed.  In no event shall authors be liable for
    any direct, indirect, incidental, special, exemplary, or consequential
    damages (including, but not limited to, procurement of substitute
    goods or services; loss of use, data, or profits; or business
    interruption) however caused and on any theory of liability, whether
    in contract, strict liability, or tort (including negligence or
    otherwise) arising in any way out of the use of this software, even if
    advised of the possibility of such damage.
  =========================================================================
*/

#include "WalkMC.h"

#include "Shared/WorldState.h"
#include "Events/EventRouter.h"
#include "Events/LocomotionEvent.h"
#include "Wireless/Socket.h"
#include "Shared/Config.h"

#include "Motion/MotionManager.h"

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <fstream>
#include <cmath>

//REGIMP(WalkMC);

//#define BOUND_MOTION

const float WalkMC::MAX_DX   = 180;//225; // mm/sec
const float WalkMC::MAX_DY   = 140;//170; // mm/sec
const float WalkMC::MAX_DA   = 1.8;//2.1; // rad/sec
// tss "SmoothWalk" modification follows
// const vector3d WalkMC::max_accel_xya(MAX_DX*2,MAX_DY*2,MAX_DA*2);
const vector3d WalkMC::max_accel_xya(MAX_DX*2,MAX_DY*2,MAX_DA*2);

unsigned int checksum(const char *data,int num); //!< computes a file checksum

WalkMC::WalkMC(const char* pfile/*=NULL*/)
	: MotionCommand(), isPaused(false), wp(), cp(), body_loc(), body_angle(),
		acc_style(DEFAULT_ACCEL), step_count(-1), step_threshold(0), last_cycle(0),
		pos_delta(0,0,0), angle_delta(0), travelTime(get_time()),
		time(get_time()), TimeStep(FrameTime), slowmo(1.0f),
		CycleOffset(0), TimeOffset(0), NewCycleOffset(false),
		vel_xya(0,0,0), target_vel_xya(0,0,0), last_target_vel_xya(0,0,0)
{
	init(pfile);
}

WalkMC::CalibrationParam::CalibrationParam() {
	for(unsigned int r=0; r<3; r++) {
		for(unsigned int c=0; c<11; c++)
			f_calibration[r][c]=b_calibration[r][c]=0;
		f_calibration[r][r]=b_calibration[r][r]=1;
	}
	for(unsigned int d=0; d<NUM_DIM; d++)
		max_accel[d]=0;
	max_vel[0]=max_vel[1]=MAX_DX;
	max_vel[2]=MAX_DY;
	max_vel[3]=MAX_DA;
}


void WalkMC::DoStart() {
	MotionCommand::DoStart();
	LocomotionEvent e(EventBase::locomotionEGID,getID(),EventBase::activateETID,0);
	e.setXYA(target_vel_xya.x,target_vel_xya.y,target_vel_xya.z);
	postEvent(e);
	travelTime=get_time();
}

void WalkMC::DoStop() {
	unsigned int t=get_time();
	LocomotionEvent e(EventBase::locomotionEGID,getID(),EventBase::deactivateETID,t-travelTime);
	//leave values at 0!
	//e.setXYA(target_vel_xya.x,target_vel_xya.y,target_vel_xya.z);
	postEvent(e);
	travelTime=t;
	MotionCommand::DoStop();
}

void WalkMC::init(const char* pfile)
{
	//	RegInit();
  body_loc.init(vector3d(0,0,wp.body_height),vector3d(0,0,0));
  body_angle.init(vector3d(0,wp.body_angle,0),vector3d(0,0,0));

  for(unsigned int i=0; i<NumLegs; i++)
    legw[i].air = false;

	if(pfile!=NULL)
		loadFile(pfile);
	else
		loadFile(config->motion.walk.c_str());

	double zeros[JointsPerLeg];
	for(unsigned int i=0; i<JointsPerLeg; i++)
		zeros[i]=0;
	resetLegPos();

	//	cmds[HeadOffset+TiltOffset].set(.3333,1);
}

// tss "SmoothWalk" addition follows
int WalkMC::isDirty()
{
  if(isPaused)
		return 0;
	if(step_count==0)
		return 0;
  if((target_vel_xya.x == 0) && (target_vel_xya.y == 0) && (target_vel_xya.z == 0)) {
		// we may stopping, but not stopped yet...
		if((vel_xya.x == 0) && (vel_xya.y == 0) && (vel_xya.z == 0))
			return false;
  }
#ifdef TGT_HAS_LEGS
  return JointsPerLeg*NumLegs;
#elif defined(TGT_HAS_WHEELS)
  return NumWheels;
#else
	return 0;
#endif
}
// tss "SmoothWalk" addition ends

	
unsigned int WalkMC::getBinSize() const {
	unsigned int used=0;
	used+=creatorSize("WalkMC-2.2");
	used+=sizeof(wp);
	used+=sizeof(cp);
	return used;	
}

unsigned int WalkMC::loadBuffer(const char buf[], unsigned int len) {
	enum {
		VER1, // original CMPack version
		VER2, // added calibration parameters
		VER3 // added diff drive and sag parameter
	} version;
	
	unsigned int origlen=len;
	if(checkCreatorInc("WalkMC-2.2",buf,len,false)) {
		version=VER3;
	} else if(checkCreatorInc("WalkMC",buf,len,false)) {
		sout->printf("Pre-2.2 release walk parameter file\n");
		version=VER2;
	} else {
		// backwards compatability - if there's no creator code, just assume it's a straight WalkParam
		sout->printf("Assuming CMPack format walk parameter file\n");
		version=VER1;
	}
	
	// Leg parameters
	for(unsigned int i=0; i<4; ++i) {
		if(!decodeInc(wp.leg[i].neutral.x,buf,len)) return 0;
		if(!decodeInc(wp.leg[i].neutral.y,buf,len)) return 0;
		if(!decodeInc(wp.leg[i].neutral.z,buf,len)) return 0;
		if(!decodeInc(wp.leg[i].lift_vel.x,buf,len)) return 0;
		if(!decodeInc(wp.leg[i].lift_vel.y,buf,len)) return 0;
		if(!decodeInc(wp.leg[i].lift_vel.z,buf,len)) return 0;
		if(!decodeInc(wp.leg[i].down_vel.x,buf,len)) return 0;
		if(!decodeInc(wp.leg[i].down_vel.y,buf,len)) return 0;
		if(!decodeInc(wp.leg[i].down_vel.z,buf,len)) return 0;
		if(!decodeInc(wp.leg[i].lift_time,buf,len)) return 0;
		if(!decodeInc(wp.leg[i].down_time,buf,len)) return 0;
	}
	// Body parameters
	if(!decodeInc(wp.body_height,buf,len)) return 0;
	if(!decodeInc(wp.body_angle,buf,len)) return 0;
	if(!decodeInc(wp.hop,buf,len)) return 0;
	if(!decodeInc(wp.sway,buf,len)) return 0;
	if(!decodeInc(wp.period,buf,len)) return 0;
	if(version<VER3) wp.useDiffDrive=0; else if(!decodeInc(wp.useDiffDrive,buf,len)) return 0;
	if(version<VER3) wp.sag=0; else if(!decodeInc(wp.sag,buf,len)) return 0;
	if(!decodeInc(wp.reserved,buf,len)) return 0;
	// Calibration parameters
	if(version<VER2) {
		cp=CalibrationParam();
	} else {
		for(unsigned int i=0; i<3; ++i)
			for(unsigned int j=0; j<11; ++j)
				if(!decodeInc(cp.f_calibration[i][j],buf,len)) return 0;
		for(unsigned int i=0; i<3; ++i)
			for(unsigned int j=0; j<11; ++j)
				if(!decodeInc(cp.b_calibration[i][j],buf,len)) return 0;
		for(unsigned int i=0; i<CalibrationParam::NUM_DIM; ++i)
			if(!decodeInc(cp.max_accel[i],buf,len)) return 0;
		for(unsigned int i=0; i<CalibrationParam::NUM_DIM; ++i)
			if(!decodeInc(cp.max_vel[i],buf,len)) return 0;
	}
	return origlen-len;	
}

unsigned int WalkMC::saveBuffer(char buf[], unsigned int len) const {
	unsigned int origlen=len;
	// Leg parameters
	if(!saveCreatorInc("WalkMC-2.2",buf,len)) return 0;
	for(unsigned int i=0; i<4; ++i) {
		if(!encodeInc(wp.leg[i].neutral.x,buf,len)) return 0;
		if(!encodeInc(wp.leg[i].neutral.y,buf,len)) return 0;
		if(!encodeInc(wp.leg[i].neutral.z,buf,len)) return 0;
		if(!encodeInc(wp.leg[i].lift_vel.x,buf,len)) return 0;
		if(!encodeInc(wp.leg[i].lift_vel.y,buf,len)) return 0;
		if(!encodeInc(wp.leg[i].lift_vel.z,buf,len)) return 0;
		if(!encodeInc(wp.leg[i].down_vel.x,buf,len)) return 0;
		if(!encodeInc(wp.leg[i].down_vel.y,buf,len)) return 0;
		if(!encodeInc(wp.leg[i].down_vel.z,buf,len)) return 0;
		if(!encodeInc(wp.leg[i].lift_time,buf,len)) return 0;
		if(!encodeInc(wp.leg[i].down_time,buf,len)) return 0;
	}
	// Body parameters
	if(!encodeInc(wp.body_height,buf,len)) return 0;
	if(!encodeInc(wp.body_angle,buf,len)) return 0;
	if(!encodeInc(wp.hop,buf,len)) return 0;
	if(!encodeInc(wp.sway,buf,len)) return 0;
	if(!encodeInc(wp.period,buf,len)) return 0;
	if(!encodeInc(wp.useDiffDrive,buf,len)) return 0;
	if(!encodeInc(wp.sag,buf,len)) return 0;
	if(!encodeInc(wp.reserved,buf,len)) return 0;
	// Calibration parameters
	for(unsigned int i=0; i<3; ++i)
		for(unsigned int j=0; j<11; ++j)
			if(!encodeInc(cp.f_calibration[i][j],buf,len)) return 0;
	for(unsigned int i=0; i<3; ++i)
		for(unsigned int j=0; j<11; ++j)
			if(!encodeInc(cp.b_calibration[i][j],buf,len)) return 0;
	for(unsigned int i=0; i<CalibrationParam::NUM_DIM; ++i)
		if(!encodeInc(cp.max_accel[i],buf,len)) return 0;
	for(unsigned int i=0; i<CalibrationParam::NUM_DIM; ++i)
		if(!encodeInc(cp.max_vel[i],buf,len)) return 0;
	return origlen-len;	
}

unsigned int WalkMC::loadFile(const char* filename) {
	return LoadSave::loadFile(config->motion.makePath(filename).c_str());
}
unsigned int WalkMC::saveFile(const char* filename) const {
	return LoadSave::saveFile(config->motion.makePath(filename).c_str());
}

void WalkMC::setTargetVelocity(double dx,double dy,double da,int n)
{
#ifdef BOUND_MOTION
  da = bound(da, -cp.rotate_max_vel, cp.rotate_max_vel);
  double fa = 1.0 - fabs(da / cp.rotate_max_vel);

  vector2d v(dx>0?dx/cp.forward_max_vel:dx/cp.reverse_max_vel,dy/cp.strafe_max_vel);
  double l = v.length();
  if(l > 1) v /= l;

  dx = (dx>0?cp.forward_max_vel:cp.reverse_max_vel) * v.x * fa;
  dy = cp.strafe_max_vel * v.y * fa;
#endif

  target_vel_xya.set(dx,dy,da);
	//std::cout << "set " << dx << ' ' << dy << ' ' << da << std::endl;
	// we just modified the target velocity, but we'll hold off on generating
	// an event until the changes are actually picked up by the motion system

	step_count=n;
}

void WalkMC::setTargetDisplacement(double dx, double dy, double da, unsigned int n) {
	if(n==0) {
		setTargetVelocity(0,0,0,0);
	} else {
		//compute the point in cycle of the mid-step
		float midsteps[NumLegs];
		for(unsigned int i=0; i<NumLegs; i++)
			midsteps[i] = (wp.leg[i].down_time+wp.leg[i].lift_time)/2;
		//find the number of unique steps per cycle
		unsigned int steps_per_cycle=4;
		for(unsigned int i=0; i<NumLegs-1; i++)
			for(unsigned int j=i+1; j<NumLegs; j++)
				if(midsteps[i]==midsteps[j]) {
					steps_per_cycle--;
					break;
				}
		//compute velocity needed to move requested distance in the time it takes to do n steps
		double scale=steps_per_cycle/(n*wp.period/1000.0);
		double vx=dx*scale;
		double vy=dy*scale;
		double va=da*scale;
		setTargetVelocity(vx,vy,va,n); //apply new value
	}
}
	
int WalkMC::updateOutputs() {
	//	cout << "WalkMC,,," << flush;
	if(!isDirty())
		return 0;

	unsigned int curT=get_time();
	if(last_target_vel_xya!=target_vel_xya) {
		last_target_vel_xya=target_vel_xya;
		LocomotionEvent e(EventBase::locomotionEGID,getID(),EventBase::statusETID,getTravelTime());
		e.setXYA(target_vel_xya.x,target_vel_xya.y,target_vel_xya.z);
		//cout << e.getDescription(true) << endl;
		postEvent(e);
		travelTime=curT;
	}
	
#if defined(TGT_HAS_LEGS)
	
	if(vel_xya.sqlength()==0) {
		// we had been stopped - better check someone else didn't move the legs while we weren't using them...
		resetLegPos(); 
	}
	
	float tm = TimeStep * slowmo / 1000;

	vector3d cal_target_vel_xya(target_vel_xya);
	if(target_vel_xya.x<0)
		cal_target_vel_xya.x/=cp.max_vel[CalibrationParam::reverse];
	else
		cal_target_vel_xya.x/=cp.max_vel[CalibrationParam::forward];
	cal_target_vel_xya.y/=cp.max_vel[CalibrationParam::strafe];
	cal_target_vel_xya.z/=cp.max_vel[CalibrationParam::rotate];
	if(cal_target_vel_xya.sqlength()>.0025)
		if(target_vel_xya.x<0)
			applyCalibration(cp.b_calibration,target_vel_xya,cal_target_vel_xya);
		else
			applyCalibration(cp.f_calibration,target_vel_xya,cal_target_vel_xya);

	if(step_count<0 && (acc_style==CALIBRATION_ACCEL || acc_style==DEFAULT_ACCEL && !config->motion.inf_walk_accel)) {
		//software accel:
		vel_xya.x = bound(cal_target_vel_xya.x, vel_xya.x-max_accel_xya.x*tm, vel_xya.x+max_accel_xya.x*tm);
		vel_xya.y = bound(cal_target_vel_xya.y, vel_xya.y-max_accel_xya.y*tm, vel_xya.y+max_accel_xya.y*tm);
		vel_xya.z = bound(cal_target_vel_xya.z, vel_xya.z-max_accel_xya.z*tm, vel_xya.z+max_accel_xya.z*tm);
	} else {
		//no software accel:
		vel_xya=cal_target_vel_xya;
	}

  BodyPosition delta;
  delta.loc.set(vel_xya.x*tm,vel_xya.y*tm,0);
  delta.angle.set(0,0,vel_xya.z*tm);

	time=(int)(curT*slowmo);

// tss "SmoothWalk" addition follows
	// If necessary, we compute a new TimeOffset here.
	if(NewCycleOffset) {
		TimeOffset = CycleOffset - time % wp.period;
		NewCycleOffset = false;
	}

	// Adjusted time--time adjusted for cycle matching
	int AdjustedTime = time + TimeOffset;

	// If walking speeds have dwindled down to zero, save our time offset from the beginning of the current walk cycle. Once we start walking again, we start up at the same offset to prevent jerky starts.
	if((vel_xya.x == 0) && (vel_xya.y == 0) && (vel_xya.z == 0)) {
		CycleOffset = AdjustedTime % wp.period;
		NewCycleOffset = true;
	}
// tss "SmoothWalk" addition ends

	for(unsigned int frame=0; frame<NumFrames; frame++) {
		cal_target_vel_xya.rotate_z(-delta.angle.z);

		// incorporate movement delta
		angle_delta += delta.angle.z;
		pos_delta += delta.loc.rotate_z(angle_delta);

		//		cout << "setup,,," << flush;

// tss "SmoothWalk" modification follows
		// double cycle = (double)(time % wp.period) / wp.period;
		AdjustedTime = time + TimeOffset + (int)(frame*TimeStep*slowmo);
		double cycle = (double)(AdjustedTime % wp.period) / wp.period;

		if(step_count>0) {
			for(unsigned int i=0; i<NumLegs; i++){
				float midstep;
				if(step_threshold<=.5f)
					midstep=wp.leg[i].lift_time+(wp.leg[i].down_time-wp.leg[i].lift_time)*step_threshold*2;
				else
					midstep=wp.leg[i].down_time+(1-wp.leg[i].down_time+wp.leg[i].lift_time)*(step_threshold-.5)*2;
				midstep-=floorf(midstep);
				//cout << "leg "<<i<<": " <<AdjustedTime << ' ' << cycle << ' ' << last_cycle << ' ' << midstep << ' ' <<step_count ;
				bool above_last= (last_cycle<midstep);
				bool below_cur = (midstep<=cycle);
				bool wrap      = (cycle<last_cycle);
				//need any 2 of the conditions: above_last && below_cur || wrap && (above_last || below_cur)
				if(above_last+below_cur+wrap>1) { //we just completed a step
					step_count--;
					//cout << " -> " << step_count << endl;
					if(step_count==0) { //we're done, copy out any completed frames
						for(unsigned int f=0; f<frame; f++)
							for(unsigned int joint=LegOffset; joint<LegOffset+NumLegJoints; joint++)
								motman->setOutput(this,joint,cmds[joint][f],f);
						CycleOffset = AdjustedTime % wp.period;
						NewCycleOffset = true;
						last_cycle=cycle;
						target_vel_xya.set(0,0,0);
						last_target_vel_xya=target_vel_xya;
						LocomotionEvent e(EventBase::locomotionEGID,getID(),EventBase::statusETID,getTravelTime());
						e.setXYA(target_vel_xya.x,target_vel_xya.y,target_vel_xya.z);
						//cout << e.getDescription(true) << endl;
						postEvent(e);
						postEvent(EventBase(EventBase::motmanEGID,getID(),EventBase::statusETID,getTravelTime()));
						//		cout << "WalkMC-done" << endl;
						return frame==0?0:NumLegs*JointsPerLeg;
					}
					break; //don't count legs moving in sync as two steps, only ever one step at a time
				}
				//cout << endl;
			}
		}
		

		double sway   = wp.sway*cos(2*M_PI*cycle);
		double hop    = wp.hop*sin(4*M_PI*cycle);
		double height = wp.body_height;
		BodyPosition nextpos;
		nextpos.loc.set(0,-sway,height+hop);
		nextpos.angle.set(0,wp.body_angle,0);

		//		cout << "loop,,," << flush;
		for(unsigned int i=0; i<NumLegs; i++){

			//interpret a down time before a lift time as a request to move the leg in reverse (has its uses)
			float lift_time=wp.leg[i].lift_time;
			float down_time=wp.leg[i].down_time;
			float dir=1;
			if(down_time==lift_time)
				dir=0;
			else if(down_time<lift_time) {
				lift_time=wp.leg[i].down_time;
				down_time=wp.leg[i].lift_time;
				dir=-1;
			}

			bool air = (cycle >= lift_time) && (cycle < down_time);
			double air_f = down_time - lift_time;
			double nextlegangles[JointsPerLeg];

			if(air != legw[i].air){
				if(air){
					/*
						cout << i << ":   ";
						cout << legpos[i].x << ' ' << legpos[i].y << ' ' << legpos[i].z << "  ->  ";
						GetLegAngles(nextlegangles,legpos[i],nextpos,i);
						for(unsigned int j=0; j<JointsPerLeg; j++)
							cout << nextlegangles[j] << ' ';
						cout << endl;
					*/
					tm = wp.period/1000.0 * 0.75; //wtf is the 0.75 based on?  Don't ask me, i just work here! (ejt)
					vector3d vfp;
					double vfa;
					if(step_count<0 && (acc_style==CALIBRATION_ACCEL || acc_style==DEFAULT_ACCEL && !config->motion.inf_walk_accel)) {
						//software accel:
						vfp.x = bound(cal_target_vel_xya.x, vel_xya.x-max_accel_xya.x*tm, vel_xya.x+max_accel_xya.x*tm);
						vfp.y = bound(cal_target_vel_xya.y, vel_xya.y-max_accel_xya.y*tm, vel_xya.y+max_accel_xya.y*tm);
						vfa   = bound(cal_target_vel_xya.z, vel_xya.z-max_accel_xya.z*tm, vel_xya.z+max_accel_xya.z*tm);
					} else {
						//no software accel:
						vfp.x=cal_target_vel_xya.x;
						vfp.y=cal_target_vel_xya.y;
						vfa=cal_target_vel_xya.z;
					}
					
					vfp.z = 0.0;
					double b = (wp.period/1000.0) * (1.0 - air_f) / 2.0;
					vector3d target;
					if(wp.useDiffDrive) {
						float rot = vfa/cp.max_vel[CalibrationParam::rotate];
						if((i&1)==0)
							rot=-rot;
						vfp.x += cp.max_vel[CalibrationParam::forward]*rot;
						target = (wp.leg[i].neutral + vfp*b*dir);
					} else {
						target = (wp.leg[i].neutral + vfp*b*dir).rotate_z(vfa*b);
					}
					target.z+=wp.sag;
					liftPos[i]=legpos[i];
					downPos[i]=target;
					legw[i].airpath.create(legpos[i],target,wp.leg[i].lift_vel,wp.leg[i].down_vel);
				}else{
					legpos[i].z = wp.leg[i].neutral.z;
				}
				legw[i].air = air;
			}

			if(air){
				legw[i].cyc = (cycle - lift_time) / air_f;
				legpos[i] = legw[i].airpath.eval(legw[i].cyc);

// Core tss "SmoothWalk" addition follows
				// KLUDGY MOD. Goal: reduce the height of the
				// AIBO's steps as its velocity nears zero.
				// Since I don't know how most of this code 
				// works, I'll directly alter legpos[i].z in
				// the following code to change the z height
				// with velocity.
				double velfraction_x = fabs(vel_xya.x / MAX_DX);
				double velfraction_y = fabs(vel_xya.y / MAX_DY);
				double velfraction_a = fabs(vel_xya.z / MAX_DA * 2); //rotation seems more sensitive

				// Choose the biggest velfraction
				double velfraction =
				  (velfraction_x > velfraction_y) ?
				    velfraction_x : velfraction_y;
				if(velfraction_a > velfraction)
				  velfraction = velfraction_a;

				// Modify legpos[i].z with velfraction to
				// shrink it down
				//velfraction = atan(velfraction * M_PI);

				velfraction-=1;
				velfraction*=velfraction;
				velfraction*=velfraction;

				legpos[i].z *= 1-velfraction;
// Core tss "SmoothWalk" addition ends
			}else{
				if(dir==0)
					legpos[i]=wp.leg[i].neutral;
				else {
					if(wp.useDiffDrive) {
						tm = wp.period/1000.0 * 0.75; //wtf is the 0.75 based on?  Don't ask me, i just work here! (ejt)
						double vfa;
						if(step_count<0 && (acc_style==CALIBRATION_ACCEL || acc_style==DEFAULT_ACCEL && !config->motion.inf_walk_accel)) {
							vfa   = bound(cal_target_vel_xya.z, vel_xya.z-max_accel_xya.z*tm, vel_xya.z+max_accel_xya.z*tm);
						} else {
							vfa   = cal_target_vel_xya.z;
						}
						legpos[i] -= delta.loc*dir;
						float rot = vfa/cp.max_vel[CalibrationParam::rotate]*TimeStep * slowmo / 1000;
						if((i&1)==0)
							rot=-rot;
						legpos[i].x -= cp.max_vel[CalibrationParam::forward]*rot;
					} else {
						legpos[i] = (legpos[i] - delta.loc*dir).rotate_z(-delta.angle.z);
					}
				}
			}

			GetLegAngles(nextlegangles,legpos[i],nextpos,i);
			for(unsigned int j=0; j<JointsPerLeg; j++)
				cmds[LegOffset+i*JointsPerLeg+j][frame].set(nextlegangles[j]);

		}

		last_cycle=cycle;
	}
	
	for(unsigned int joint=LegOffset; joint<LegOffset+NumLegJoints; joint++)
		motman->setOutput(this,joint,cmds[joint]);
	
	//		cout << "WalkMC-done" << endl;
	return NumLegs*JointsPerLeg;
	
#elif defined(TGT_HAS_WHEELS)
	vel_xya=target_vel_xya;
	float wheelcircum = 2*M_PI*config->motion.wheelRadius;
	float baserpm = vel_xya.x / wheelcircum;
	float turnrpm = (vel_xya.z*config->motion.wheelBaseWidth/2) / wheelcircum;
	float left = baserpm - turnrpm;
	float right = baserpm + turnrpm;
	for(unsigned int frame=0; frame<NumFrames; frame++) {
		cmds[LWheelOffset][frame]=left;
		cmds[RWheelOffset][frame]=right;
	}
	//std::cout << "update " << vel_xya.x << ' ' << vel_xya.y << ' ' << vel_xya.z << " -> " << left << ' ' << right << std::endl;
	
	for(unsigned int w=WheelOffset; w<WheelOffset+NumWheels; w++)
		motman->setOutput(this,w,cmds[w]);
	
	//		cout << "WalkMC-done" << endl;
	return NumWheels;
	
#else
#  warning target has neither legs nor wheels -- WalkMC is a no-op
	return 0;
#endif
}

void WalkMC::resetLegPos() {
	BodyPosition nextpos;
	nextpos.loc.set(0,0,wp.body_height);
	nextpos.angle.set(0,wp.body_angle,0);
#ifdef TGT_HAS_LEGS
	for(unsigned int i=0; i<NumLegs; i++) {
		double tmp[JointsPerLeg];
		for(unsigned int j=0; j<JointsPerLeg; j++)
			tmp[j]=state->outputs[LegOffset+i*JointsPerLeg+j];
		GetLegPosition(legpos[i],tmp,nextpos,i);
		/*
			for(unsigned int j=0; j<JointsPerLeg; j++)
			cout << state->outputs[LegOffset+i*JointsPerLeg+j] << ' ';
			cout << "  ->  " << legpos[i].x << ' ' << legpos[i].y << ' ' << legpos[i].z << endl;
		*/
	}
#endif
	//cout << "----------------------" << endl;
}

unsigned int checksum(const char *data,int num)
{
  unsigned long c;
  int i;

  c = 0;
  for(i=0; i<num; i++){
    c = c ^ (data[i]*13 + 37);
    c = (c << 13) | (c >> 19);
  }

  return(c);
}

void WalkMC::applyCalibration(const float mat[3][11], const vector3d& in, vector3d& out) {
	float inmat[11];
	inmat[0]=in.x;
	inmat[1]=in.y;
	inmat[2]=in.z;
	inmat[3]=fabs(in.y);
	inmat[4]=fabs(in.z);
	inmat[5]=exp(-.5f*in.z*in.z)*sin(in.z*2.5f);
	inmat[6]=in.x*in.x+in.y*in.y;
	inmat[7]=in.x*in.z;
	inmat[8]=in.y*in.x;
	inmat[9]=in.z*in.y;
	inmat[10]=1;
	out.set(0,0,0);
	for(unsigned int c=0; c<11; c++)
		out.x+=mat[0][c]*inmat[c];
	for(unsigned int c=0; c<11; c++)
		out.y+=mat[1][c]*inmat[c];
	for(unsigned int c=0; c<11; c++)
		out.z+=mat[2][c]*inmat[c];
}

/*! @file
 * @brief Implements WalkMC, a MotionCommand for walking around
 * @author CMU RoboSoccer 2001-2002 (Creator)
 * @author ejt (ported)
 *
 * @verbinclude CMPack_license.txt
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.42 $
 * $State: Exp $
 * $Date: 2007/11/18 06:47:03 $
 */

