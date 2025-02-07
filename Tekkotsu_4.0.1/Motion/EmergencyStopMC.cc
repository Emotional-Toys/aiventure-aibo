#include "EmergencyStopMC.h"
#include "Shared/WorldState.h"
#include "Shared/get_time.h"
#include "Motion/MotionManager.h"
#include "Sound/SoundManager.h"
#include "Shared/Config.h"
#include "Events/EventRouter.h"
#include "Shared/ERS210Info.h"
#include "Shared/ERS220Info.h"
#include "Shared/ERS7Info.h"
#include "Wireless/Wireless.h"

EmergencyStopMC::EmergencyStopMC()
	: PostureMC(), paused(false), stilldown(false), active(true), period(2000),
		timeoflastbtn(0), timeofthisbtn(0), timeoflastfreeze(0), timeoflastrelease(0), duration(600),
		pidcutoff(0.2), ledengine()
{
	for(unsigned int i=0; i<NumPIDJoints; i++)
		piddutyavgs[i]=0;
#ifdef TGT_HAS_LEDS
	if(RobotName == ERS210Info::TargetName) {
		int red=capabilities.getOutputOffset(ERS210Info::outputNames[ERS210Info::TlRedLEDOffset]) - LEDOffset;
		int blue=capabilities.getOutputOffset(ERS210Info::outputNames[ERS210Info::TlBluLEDOffset]) - LEDOffset;
		ledengine.cycle((1<<red),period,1,0,period/2);
		ledengine.cycle((1<<blue),period,1);
	} else if(RobotName == ERS220Info::TargetName) {
		int tl=capabilities.getOutputOffset(ERS220Info::outputNames[ERS220Info::TailLeftLEDOffset]) - LEDOffset;
		int tc=capabilities.getOutputOffset(ERS220Info::outputNames[ERS220Info::TailCenterLEDOffset]) - LEDOffset;
		int tr=capabilities.getOutputOffset(ERS220Info::outputNames[ERS220Info::TailRightLEDOffset]) - LEDOffset;
		int bl1=capabilities.getOutputOffset(ERS220Info::outputNames[ERS220Info::BackLeft1LEDOffset]) - LEDOffset;
		int bl2=capabilities.getOutputOffset(ERS220Info::outputNames[ERS220Info::BackLeft2LEDOffset]) - LEDOffset;
		int bl3=capabilities.getOutputOffset(ERS220Info::outputNames[ERS220Info::BackLeft3LEDOffset]) - LEDOffset;
		int br1=capabilities.getOutputOffset(ERS220Info::outputNames[ERS220Info::BackRight1LEDOffset]) - LEDOffset;
		int br2=capabilities.getOutputOffset(ERS220Info::outputNames[ERS220Info::BackRight2LEDOffset]) - LEDOffset;
		int br3=capabilities.getOutputOffset(ERS220Info::outputNames[ERS220Info::BackRight3LEDOffset]) - LEDOffset;
		ledengine.cycle((1<<tc), period, 2.0f, -.5f, (int)(period * 0/5.5));
		ledengine.cycle((1<<tl)|(1<<tr),   period, 2.0f, -.5f, (int)(period * 1/5.5));
		ledengine.cycle((1<<bl3)|(1<<br1), period, 2.0f, -.5f, (int)(period * 2/5.5));
		ledengine.cycle((1<<bl2)|(1<<br2), period, 2.0f, -.5f, (int)(period * 3/5.5));
		ledengine.cycle((1<<bl1)|(1<<br3), period, 2.0f, -.5f, (int)(period * 4/5.5));
	} else if(RobotName == ERS7Info::TargetName) {
		int mb=capabilities.getOutputOffset(ERS7Info::outputNames[ERS7Info::MdBackColorLEDOffset]) - LEDOffset;
		ledengine.cycle((1<<mb),2*period/3,.15,.15/2-.5,0);
	} else {
		serr->printf("Emergency Stop: unknown model, no LED special effects\n");
		ledengine.cycle(1<<(NumLEDs-1),period,1,0,period/2);
		ledengine.cycle(1<<(NumLEDs-2),period,1);
	}
#endif
	defaultMaxSpeed(.15);
	takeSnapshot();
}


int EmergencyStopMC::updateOutputs() {
	if(trigger()) {
		if(!stilldown) {
			stilldown=true;
			timeoflastbtn=timeofthisbtn;
			timeofthisbtn=get_time();
			//			cout << "Press " << timeofthisbtn << ' ' << timeoflastbtn << endl;
		}
		//		cout << "Down" << endl;
	} else if(stilldown) {
		//		cout << "Release " << get_time() << endl;
		stilldown=false;
		if((get_time()-timeoflastbtn)<duration)
			setStopped(!paused);
	}
	unsigned int curt=get_time();
	dirty=dirty || (curt<timeoflastrelease);
	if(!paused) {
		if(!dirty)
			return 0;
		if(curt>=timeoflastrelease) {
#ifdef TGT_HAS_LEDS
			for(unsigned int i=LEDOffset; i<LEDOffset+NumLEDs; i++)
				motman->setOutput(this,i,0.f); //blank out LEDs to avoid residual background display
#endif
			dirty=false;
			return 0;
		}
		float w = (curt>=timeoflastrelease) ? 0 : (static_cast<float>(timeoflastrelease-curt)/FADE_OUT_TIME);
		for(unsigned int i=0; i<NumOutputs; i++)
			cmds[i].weight=w;
	} else {
		//immediately following a pause, just hold current position at first to prevent twitching if we were in motion
		if(curt-timeoflastfreeze>FrameTime*NumFrames*5) { 
			//once joints have come to rest, respond to outside forces
			for(unsigned int i=0; i<NumPIDJoints; i++) {
				//exponential average of duty cycles to filter out noise
				piddutyavgs[i]=piddutyavgs[i]*.9f+state->pidduties[i]*.1f;
				//reset if there's something significantly out of place (perhaps we're being overridden)
				if(fabsf(state->outputs[PIDJointOffset+i]-cmds[PIDJointOffset+i].value)>.15f) {
					//if(PIDJointOffset+i==LFrLegOffset+RotatorOffset)
					//cout << "resetting from " << cmds[PIDJointOffset+i].value << " to " << state->outputs[PIDJointOffset+i] << endl;
					curPositions[PIDJointOffset+i]=cmds[PIDJointOffset+i].value=state->outputs[PIDJointOffset+i];
					dirty=true;
					targetReached=false;
				}
				//give if there's a force...
				if(fabsf(piddutyavgs[i])>pidcutoff) {
					cmds[PIDJointOffset+i].value-=piddutyavgs[PIDJointOffset+i]; //move in the direction of the force
					dirty=true;
					targetReached=false;
				}
			}
		}
	}
#ifdef TGT_HAS_LEDS
	ledengine.updateLEDs(&cmds[LEDOffset]);
#endif
	if(RobotName == ERS7Info::TargetName) {
		//a special Battlestar Galactica inspired effect for the ERS-7
		static float acts[5];
		static bool wasPaused=false;
		if(!wasPaused && paused) {
			for(int i=0; i<5; i++)
				acts[i]=0;
			wasPaused=paused;
		}
		float t=curt;
		t/=period;
		t=(((int)t)&1)?(int)t+1-t:(t-(int)t);
		t*=8;
		const float invsigma=-6;
		const float gamma=.83;
		const float amp=.5;
		float imp[5];
		// w is used to fade out LEDs when releasing estop
		float w = (paused || curt>=timeoflastrelease) ? 1 : (static_cast<float>(timeoflastrelease-curt)/FADE_OUT_TIME);
		for(int i=0; i<5; i++) {
			float p=invsigma*(t-i-2)*(t-i-2);
			if(p>-10) { // only bother with impulse if big enough
				// (in particular, saw exp returning -inf instead of 0 for <-85... bug in libm?)
				imp[i]=expf(p)*w;
				acts[i]+=amp*imp[i];
			} else {
				imp[i]=0;
			}
			acts[i]*=gamma*w;
		}
		cmds[ERS7Info::FaceLEDPanelOffset+ 6]=acts[0]/2+imp[0];
		cmds[ERS7Info::FaceLEDPanelOffset+ 8]=acts[1]/2+imp[1];
		cmds[ERS7Info::FaceLEDPanelOffset+10]=acts[2]/2+imp[2];
		cmds[ERS7Info::FaceLEDPanelOffset+ 9]=acts[3]/2+imp[3];
		cmds[ERS7Info::FaceLEDPanelOffset+ 7]=acts[4]/2+imp[4];
	}
	int changed=PostureMC::updateOutputs();
	dirty=(curt<timeoflastrelease);
	return changed;
}

void EmergencyStopMC::setActive(bool a) {
	if(paused) {
		if(!a && active)
			releaseJoints();
		else if(a && !active)
			freezeJoints();
	}
	active=a;
}


void EmergencyStopMC::setStopped(bool p, bool sound) {
	if(p!=paused) {
		paused=p;
		if(active) {
			if(paused) {
				freezeJoints();
				if(sound)
					sndman->playFile(config->motion.estop_on_snd);
				std::cout << "*** PAUSED ***" << std::endl;
			} else {
				releaseJoints();
				if(sound)
					sndman->playFile(config->motion.estop_off_snd);
				std::cout << "*** UNPAUSED ***" << std::endl;
			}
		}
	}
}

void EmergencyStopMC::freezeJoints() {
	dirty=true;
	targetReached=false;
	for(unsigned int i=0; i<NumOutputs; i++) {
		OutputCmd c=motman->getOutputCmd(i);
		curPositions[i]=cmds[i].value = (c.weight==0) ? state->outputs[i] : c.value;
	}
	for(unsigned int i=0; i<NumPIDJoints; i++)
		piddutyavgs[i]=0; //or: state->pidduties[i];
#ifdef TGT_HAS_WHEELS
	//Wheels need to be set to 0 in e-stop mode
	for(unsigned int i=WheelOffset; i<WheelOffset+NumWheels; i++) {
	  cmds[i].value = 0.0;
	  curPositions[i] = 0;
	}
#endif
#ifndef TGT_HAS_LEDS
	// no LEDs, just go all the way through in one pass...
	for(unsigned int i=0; i<NumOutputs; i++)
		cmds[i].weight=1;
#else
	for(unsigned int i=0; i<LEDOffset; i++)
		cmds[i].weight=1;
	for(unsigned int i=LEDOffset; i<LEDOffset+NumLEDs; i++)
		cmds[i].unset(); // let other commands' LEDs "show through"
	for(unsigned int i=LEDOffset+NumLEDs; i<NumOutputs; i++)
		cmds[i].weight=1;
	if(RobotName == ERS210Info::TargetName) {
		int red=capabilities.getOutputOffset(ERS210Info::outputNames[ERS210Info::TlRedLEDOffset]);
		int blue=capabilities.getOutputOffset(ERS210Info::outputNames[ERS210Info::TlBluLEDOffset]);
		cmds[red].set(0,.5);
		cmds[blue].set(0,.5);
	} else if(RobotName == ERS220Info::TargetName) {
		int tl=capabilities.getOutputOffset(ERS220Info::outputNames[ERS220Info::TailLeftLEDOffset]);
		int tc=capabilities.getOutputOffset(ERS220Info::outputNames[ERS220Info::TailCenterLEDOffset]);
		int tr=capabilities.getOutputOffset(ERS220Info::outputNames[ERS220Info::TailRightLEDOffset]);
		int bl1=capabilities.getOutputOffset(ERS220Info::outputNames[ERS220Info::BackLeft1LEDOffset]);
		int bl2=capabilities.getOutputOffset(ERS220Info::outputNames[ERS220Info::BackLeft2LEDOffset]);
		int bl3=capabilities.getOutputOffset(ERS220Info::outputNames[ERS220Info::BackLeft3LEDOffset]);
		int br1=capabilities.getOutputOffset(ERS220Info::outputNames[ERS220Info::BackRight1LEDOffset]);
		int br2=capabilities.getOutputOffset(ERS220Info::outputNames[ERS220Info::BackRight2LEDOffset]);
		int br3=capabilities.getOutputOffset(ERS220Info::outputNames[ERS220Info::BackRight3LEDOffset]);
		cmds[tl].set(0, .5); cmds[tc].set(0, .5); cmds[tr].set(0, .5);
		cmds[bl1].set(0, .5); cmds[bl2].set(0, .5); cmds[bl3].set(0, .5);
		cmds[br1].set(0, .5); cmds[br2].set(0, .5); cmds[br3].set(0, .5);
	} else if(RobotName == ERS7Info::TargetName) {
		cmds[ERS7Info::MdBackColorLEDOffset].set(0,.5);
		for(int i=6; i<6+5; i++)
			cmds[ERS7Info::FaceLEDPanelOffset+i].set(0,0.5);
	} else {
		cmds[LEDOffset+NumLEDs-1].set(0,.5);
		cmds[LEDOffset+NumLEDs-2].set(0,.5);
	}
#endif
	postEvent(EventBase(EventBase::estopEGID,getID(),EventBase::activateETID,0));
	timeoflastfreeze=get_time();
}

void EmergencyStopMC::releaseJoints() {
	dirty=true;
	targetReached=false;
	unsigned int curt=get_time();
	timeoflastrelease=curt+FADE_OUT_TIME;
	postEvent(EventBase(EventBase::estopEGID,getID(),EventBase::deactivateETID,curt-timeoflastfreeze));
}

bool EmergencyStopMC::trigger() {
	WorldState * st=WorldState::getCurrent(); // this is need because trigger is a static, so it doesn't have access to the MC 'state' instance
	if(RobotName == ERS210Info::TargetName)
		return st->button_times[capabilities.getButtonOffset(ERS210Info::buttonNames[ERS210Info::BackButOffset])];
	if(RobotName == ERS220Info::TargetName)
		return st->button_times[capabilities.getButtonOffset(ERS220Info::buttonNames[ERS220Info::BackButOffset])];
	if(RobotName == ERS7Info::TargetName)
		return st->button_times[ERS7Info::FrontBackButOffset]+st->button_times[ERS7Info::MiddleBackButOffset]+st->button_times[ERS7Info::RearBackButOffset];
#ifdef TGT_HAS_BUTTONS
	// depending on platform, RobotName may be either char* or string
	std::string tmpName = RobotName; // convert to consistent form so we can printf it...
	serr->printf("EmergencyStopMC: %s unsupported model!\n",tmpName.c_str());
#endif
	return false;
}

/*! @file
 * @brief Implements EmergencyStopMC, overrides all joints, allows modelling, blinks tail
 * @author ejt (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.40 $
 * $State: Exp $
 * $Date: 2007/11/18 06:47:03 $
 */

