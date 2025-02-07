#include "HeadPointerMC.h"
#include "Kinematics.h"
#include "Shared/debuget.h"
#include "Shared/WorldState.h"
#include "MotionManager.h"
#include "Shared/Config.h"
#include "Wireless/Socket.h"
#include "Shared/ERS7Info.h"
#include "Shared/ERS210Info.h"
#include <math.h>

HeadPointerMC::HeadPointerMC()
	: MotionCommand(), dirty(true), hold(true), tolerance(.05), // if change default tolerance, update documentation in header
		targetReached(true), targetTimestamp(0), timeout(2000), 
	  headkin(::config->motion.makePath(::config->motion.kinematics),"Camera")
{
	setWeight(1);
	defaultMaxSpeed();
	takeSnapshot();
}

void HeadPointerMC::freezeMotion() {
#ifdef TGT_HAS_HEAD
	for(unsigned int i=0; i<NumHeadJoints; i++)
		headTargets[i]=headCmds[i].value;
	dirty=false;
#endif
}

void HeadPointerMC::takeSnapshot() {
#ifdef TGT_HAS_HEAD
	for(unsigned int i=0; i<NumHeadJoints; i++)
		headTargets[i]=headCmds[i].value=state->outputs[HeadOffset+i];
	dirty=true;
#endif
}

void HeadPointerMC::defaultMaxSpeed(float x/*=1*/) {
#ifdef TGT_HAS_HEAD
	const char* n = ERS7Info::outputNames[ERS7Info::HeadOffset+ERS7Info::TiltOffset];
	unsigned int i = capabilities.findOutputOffset(n);
	if(i!=-1U)
		maxSpeed[i-HeadOffset]=config->motion.max_head_tilt_speed*FrameTime*x/1000;
	n = ERS7Info::outputNames[ERS7Info::HeadOffset+ERS7Info::PanOffset];
	i = capabilities.findOutputOffset(n);
	if(i!=-1U)
		maxSpeed[i-HeadOffset]=config->motion.max_head_pan_speed*FrameTime*x/1000;
	n = ERS7Info::outputNames[ERS7Info::HeadOffset+ERS7Info::NodOffset];
	i = capabilities.findOutputOffset(n);
	if(i!=-1U)
		maxSpeed[i-HeadOffset]=config->motion.max_head_roll_speed*FrameTime*x/1000;
	n = ERS210Info::outputNames[ERS210Info::HeadOffset+ERS210Info::RollOffset];
	i = capabilities.findOutputOffset(n);
	if(i!=-1U)
		maxSpeed[i-HeadOffset]=config->motion.max_head_roll_speed*FrameTime*x/1000;
#endif
}

void HeadPointerMC::setWeight(float w) {
#ifdef TGT_HAS_HEAD
	for(unsigned int x=0; x<NumHeadJoints; x++)
		headCmds[x].weight=w;
	markDirty();
#endif
}

void HeadPointerMC::setJoints(float tilt1, float pan, float tilt2) {
#ifdef TGT_HAS_HEAD
#ifdef TGT_IS_AIBO
	headTargets[TiltOffset]=clipAngularRange(HeadOffset+TiltOffset,tilt1);
	headTargets[PanOffset]=clipAngularRange(HeadOffset+PanOffset,pan);
	headTargets[NodOffset]=clipAngularRange(HeadOffset+NodOffset,tilt2);
#else
	const char* n = ERS7Info::outputNames[ERS7Info::HeadOffset+ERS7Info::TiltOffset];
	unsigned int i = capabilities.findOutputOffset(n);
	if(i!=-1U)
		headTargets[i-HeadOffset]=clipAngularRange(i,tilt1);
	n = ERS7Info::outputNames[ERS7Info::HeadOffset+ERS7Info::PanOffset];
	i = capabilities.findOutputOffset(n);
	if(i!=-1U)
		headTargets[i-HeadOffset]=clipAngularRange(i,pan);
	n = ERS7Info::outputNames[ERS7Info::HeadOffset+ERS7Info::NodOffset];
	i = capabilities.findOutputOffset(n);
	if(i!=-1U)
		headTargets[i-HeadOffset]=clipAngularRange(i,tilt2);
#endif
	markDirty();
#endif
}

bool HeadPointerMC::lookAtPoint(float x, float y, float z) {
#ifndef TGT_HAS_HEAD
	return false;
#else
	NEWMAT::ColumnVector Pobj(4),Plink(4);
	Pobj(1)=x; Pobj(2)=y; Pobj(3)=z; Pobj(4)=1;
	Plink=0; Plink(3)=1;
	bool conv=false;
	NEWMAT::ColumnVector q=headkin.inv_kin_pos(Pobj,0,headkin.get_dof(),Plink,conv);

	NEWMAT::ColumnVector poE=headkin.convertLink(0,headkin.get_dof())*Pobj;
	poE=poE.SubMatrix(1,3,1,1);
	//	float theta = acos(poE(3)/sqrt(poE(1)*poE(1)+poE(2)*poE(2)+poE(3)*poE(3)));
	//	cout << "Computed:\n theta1: " << mathutils::rad2deg(theta) << " degrees\n";
	NEWMAT::ColumnVector plE=Plink.SubMatrix(1,3,1,1);
	float plE2=plE.SumSquare();
	float plE_len=sqrt(plE2);
	float obj_comp_link=NEWMAT::DotProduct(plE,poE)/plE_len;
	if(obj_comp_link<plE_len)
		obj_comp_link=obj_comp_link*.975; //.975 is a bit of fudge - accounts for joints moving Plink when adjusting
	else
		obj_comp_link=obj_comp_link/.975; //.975 is a bit of fudge - accounts for joints moving Plink when adjusting
	NEWMAT::ColumnVector obj_proj_link(4);
	obj_proj_link.SubMatrix(1,3,1,1)=obj_comp_link*plE/plE_len;
	obj_proj_link(4)=1;
	q=headkin.inv_kin_pos(Pobj,0,headkin.get_dof(),obj_proj_link,conv);

	for(unsigned int i=0; i<NumHeadJoints; i++)
		setJointValue(i,headkin.get_q(2+i));

	//	NEWMAT::ColumnVector poE2=headkin.convertLink(0,headkin.get_dof())*Pobj;
	//	float theta2 = acos(poE2(3)/sqrt(poE2(1)*poE2(1)+poE2(2)*poE2(2)+poE2(3)*poE2(3)));
	//	cout << " theta2: " << mathutils::rad2deg(theta2) << " degrees\n";
	return isReachable(Pobj);
#endif
}
	
bool HeadPointerMC::lookAtPoint(float x, float y, float z, float d) {
#ifndef TGT_HAS_HEAD
	return false;
#else
	NEWMAT::ColumnVector Pobj(4),Plink(4);
	Pobj(1)=x; Pobj(2)=y; Pobj(3)=z; Pobj(4)=1;
	Plink=0; Plink(3)=d; Plink(4)=1;
	bool conv=false;
	NEWMAT::ColumnVector q=headkin.inv_kin_pos(Pobj,0,headkin.get_dof(),Plink,conv);
	for(unsigned int i=0; i<NumHeadJoints; i++)
		setJointValue(i,headkin.get_q(2+i));
	//	return conv;
	return isReachable(Pobj);
#endif
}
	
bool HeadPointerMC::lookInDirection(float x, float y, float z) {
#ifndef TGT_HAS_HEAD
	return false;
#else
	NEWMAT::ColumnVector Pobj(4),Plink(4);
	Pobj(1)=x; Pobj(2)=y; Pobj(3)=z; Pobj(4)=0;
	Plink=0; Plink(3)=1;
	bool conv=false;
	NEWMAT::ColumnVector q=headkin.inv_kin_pos(Pobj,0,headkin.get_dof(),Plink,conv);
	for(unsigned int i=0; i<NumHeadJoints; i++)
		setJointValue(i,headkin.get_q(2+i));
	//	return conv;
	return isReachable(Pobj);
#endif
}


int HeadPointerMC::updateOutputs() {
	int tmp=isDirty();
	if(tmp || hold) {
		dirty=false;
#ifdef TGT_HAS_HEAD
		for(unsigned int i=0; i<NumHeadJoints; i++) {
			if(maxSpeed[i]<=0) {
				headCmds[i].value=headTargets[i];
				motman->setOutput(this,i+HeadOffset,headCmds[i]);
			} else { // we may be trying to exceeded maxSpeed
				unsigned int f=0;
				while(headTargets[i]>headCmds[i].value+maxSpeed[i] && f<NumFrames) {
					headCmds[i].value+=maxSpeed[i];
					motman->setOutput(this,i+HeadOffset,headCmds[i],f);
					f++;
				}
				while(headTargets[i]<headCmds[i].value-maxSpeed[i] && f<NumFrames) {
					headCmds[i].value-=maxSpeed[i];
					motman->setOutput(this,i+HeadOffset,headCmds[i],f);
					f++;
				}
				if(f<NumFrames) { //we reached target value, fill in rest of frames
					headCmds[i].value=headTargets[i];
					for(;f<NumFrames;f++)
						motman->setOutput(this,i+HeadOffset,headCmds[i],f);
				} else // we didn't reach target value, still dirty
					dirty=true;
			}
		}
#endif
		if(!dirty && !targetReached) {
			postEvent(EventBase(EventBase::motmanEGID,getID(),EventBase::statusETID));
			targetReached=true;
			targetTimestamp=get_time();
		}
	}
	return tmp;
}

int HeadPointerMC::isAlive() {
#ifndef TGT_HAS_HEAD
	return false;
#else
	if(dirty || !targetReached)
		return true;
	if(targetReached && (!hold || get_time()-targetTimestamp>timeout)) { //prevents a conflicted HeadPointerMC's from fighting forever
		if(get_time()-targetTimestamp>timeout && getAutoPrune())
			serr->printf("WARNING: HeadPointerMC (mcid %d) timed out - possible joint conflict or out-of-range target\n",getID());
		return false;
	}
	float maxdiff=0;
	for(unsigned int i=0; i<NumHeadJoints; i++) {
		float diff=fabsf(state->outputs[HeadOffset+i]-headTargets[i]);
		if(diff>maxdiff)
			maxdiff=diff;
	}
	return (maxdiff>tolerance);
#endif
}

void HeadPointerMC::markDirty() {
	dirty=true;
	targetReached=false;
#ifdef TGT_HAS_HEAD
	for(unsigned int i=0; i<NumHeadJoints; i++)
		headCmds[i].value=motman->getOutputCmd(HeadOffset+i).value; //not state->outputs[HeadOffset+i]; - see function documentation
#endif
}

bool HeadPointerMC::ensureValidJoint(unsigned int& i) {
#ifndef TGT_HAS_HEAD
	serr->printf("ERROR: HeadPointerMC received a joint index of %d on headless target.\n",i);
#else
	if(i<NumHeadJoints)
		return true;
	if(i>=HeadOffset && i<HeadOffset+NumHeadJoints) {
		i-=HeadOffset;
		serr->printf("WARNING: HeadPointerMC received a joint index of %d (HeadOffset+%d).\n",i+HeadOffset,i);
		serr->printf("         Since all parameters are assumed to be relative to HeadOffset,\n");
		serr->printf("         you should just pass %d directly.\n",i);
		serr->printf("WARNING: Assuming you meant %d...\n",i);
		return true;
	}
	serr->printf("ERROR: HeadPointerMC received a joint index of %d (HeadOffset%+d).\n",i,i-HeadOffset);
	serr->printf("ERROR: This does not appear to be a head joint.  HeadPointerMC only controls\n");
	serr->printf("       head joints, and assumes its arguments are relative to HeadOffset\n");
#endif
	return false;
}

/*! @file
 * @brief Implements HeadPointerMC, a class for various ways to control where the head is looking
 * @author ejt (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.29 $
 * $State: Exp $
 * $Date: 2007/11/18 06:47:03 $
 */
