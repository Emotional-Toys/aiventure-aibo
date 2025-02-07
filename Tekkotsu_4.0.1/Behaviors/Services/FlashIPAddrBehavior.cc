#include "Shared/RobotInfo.h"
#ifdef TGT_HAS_LEDS

#include "Shared/ERS210Info.h"
#include "Shared/ERS220Info.h"
#include "Shared/ERS7Info.h"

#include "FlashIPAddrBehavior.h"
#include "Events/EventRouter.h"
#include "Motion/MMAccessor.h"
#include "Motion/LedEngine.h"
#include "Shared/WorldState.h"
#include "Shared/Config.h"
#include "Sound/SoundManager.h"
#include "Wireless/Wireless.h"

void FlashIPAddrBehavior::DoStart() {
	BehaviorBase::DoStart(); // do this first
	if(config->behaviors.flash_on_start) {
		setupSequence();
		loadSounds();
		ms_id = motman->addPrunableMotion(ms,MotionManager::kEmergencyPriority+1);
		erouter->addListener(this,EventBase::motmanEGID,ms_id,EventBase::deactivateETID);
	}
#ifdef TGT_HAS_BUTTONS
	erouter->addListener(this,EventBase::buttonEGID,button1);
	erouter->addListener(this,EventBase::buttonEGID,button2);
#endif
}

void FlashIPAddrBehavior::DoStop() {
	erouter->removeListener(this);
	motman->removeMotion(ms_id);
	ms_id=MotionManager::invalid_MC_ID;
	releaseSounds();
	BehaviorBase::DoStop(); // do this last
}

void FlashIPAddrBehavior::processEvent(const EventBase& e) {
	if(e.getGeneratorID()==EventBase::timerEGID) {

		if(e.getSourceID()==ACTIVATE_TIMER) {
			//buttons have been held down long enough, time to run display
			if(ms_id!=MotionManager::invalid_MC_ID) {
				//there's already one running, have to check it out to clear it
				MMAccessor<MSMC_t> ms_acc(ms_id);
				setupSequence();
			} else
				setupSequence();
			loadSounds();
			ms_id = motman->addPrunableMotion(ms);
			erouter->addListener(this,EventBase::motmanEGID,ms_id,EventBase::deactivateETID);
				
		} else { //its time to play a digit sound file
			//the source id was set to correspond to an element of the sounds vector
			if(e.getSourceID()>=sounds.size())
				serr->printf("ERROR: %s received invalid timer event %s\n",getName().c_str(),e.getName().c_str());
			else {
				sndman->playFile(sounds[e.getSourceID()]);
				if(e.getSourceID()==sounds.size()-1)
					releaseSounds();
			}
				
		}

#ifdef TGT_HAS_BUTTONS
	} else if(e.getGeneratorID()==EventBase::buttonEGID) {
		//if it's an activate, start a timer to expire in a few seconds
		//if it's a deactivate, cancel that timer
		if(e.getTypeID()==EventBase::activateETID) {
			if(state->buttons[button1] && state->buttons[button2])
				erouter->addTimer(this,ACTIVATE_TIMER,2000,false);
		} else if(e.getTypeID()==EventBase::deactivateETID)
			erouter->removeTimer(this,ACTIVATE_TIMER);
#endif

	} else if(e.getGeneratorID()==EventBase::motmanEGID) {
		// display has completed, mark it as such
		if(e.getSourceID()!=ms_id)
			serr->printf("WARNING: %s received event %s, doesn't match ms_id (%d)\n",getName().c_str(),e.getName().c_str(),ms_id);
		ms_id=MotionManager::invalid_MC_ID;
		erouter->removeListener(this,EventBase::motmanEGID);
			
	}
}

void FlashIPAddrBehavior::loadSounds() {
	for(unsigned int i=0; i<sounds.size(); i++)
		sndman->loadFile(sounds[i]);
}

void FlashIPAddrBehavior::releaseSounds() {
	for(unsigned int i=0; i<sounds.size(); i++)
		sndman->releaseFile(sounds[i]);
	sounds.clear();
}

void FlashIPAddrBehavior::setupSequence() {
	const unsigned int DISP_TIME=600;
	const unsigned int GROUP_TIME=500;
	const unsigned int DOT_TIME=400;
	const unsigned int FADE_TIME=1;
	const unsigned int BLANK_TIME=100-FADE_TIME*2;
	erouter->removeTimer(this);
	ms->clear();
	releaseSounds();
	unsigned int a=wireless->getIPAddress();
	unsigned int n=config->behaviors.flash_bytes;
	if(n>4)
		n=4;
	LedEngine disp;
	for(unsigned int i=n-1; i!=-1U; i--) {
		unsigned int byte=(a>>(i*8))&0xFF;
		unsigned int digits=1;
		if(byte>=10)
			digits++;
		if(byte>=100)
			digits++;
		//cout << "byte " << i << " is " << byte << " -- " << digits << " digits" << endl;
		//cout << "Setting LEDs: ";
		for(unsigned int d=0; d<digits; d++) {
			unsigned int digit=byte;
			for(unsigned int j=d;j<digits-1;j++)
				digit/=10;
			digit-=(digit/10)*10;
			disp.displayNumber(digit,LedEngine::onedigit);
			std::string soundfile="numbers/";
			soundfile+=(digit+'0');
			soundfile+=".wav";
			erouter->addTimer(this,sounds.size(),ms->getTime()+delay,false);
			sounds.push_back(soundfile);
			for(unsigned int j=0; j<NumLEDs; j++)
				if(FaceLEDMask&(1<<j)) {
					//if(disp.getValue(static_cast<LEDOffset_t>(LEDOffset+j)))
					//cout << j << ' ';
					ms->setOutputCmd(LEDOffset+j,disp.getValue(static_cast<LEDOffset_t>(LEDOffset+j)));
				}
			ms->advanceTime(DISP_TIME);
			for(unsigned int j=0; j<NumLEDs; j++)
				if(FaceLEDMask&(1<<j))
					ms->setOutputCmd(LEDOffset+j,disp.getValue(static_cast<LEDOffset_t>(LEDOffset+j)));
			ms->advanceTime(FADE_TIME);
			for(unsigned int j=0; j<NumLEDs; j++)
				if(FaceLEDMask&(1<<j))
					ms->setOutputCmd(LEDOffset+j,0);
			ms->advanceTime(BLANK_TIME);
			if(d==digits-1)
				ms->advanceTime(GROUP_TIME);
			for(unsigned int j=0; j<NumLEDs; j++)
				if(FaceLEDMask&(1<<j))
					ms->setOutputCmd(LEDOffset+j,0);
			ms->advanceTime(FADE_TIME);
		}
		//cout << endl;
		if(i!=0) {
			LEDBitMask_t dot=LedEngine::defaultCountNumMasks[10]; //default in case we don't recognize model
#ifdef TGT_HAS_LED_PANEL
			if(RobotName == ERS210Info::TargetName) {
				dot=LedEngine::defaultMimicNumMasks[10];
			} else if(RobotName == ERS220Info::TargetName) {
				dot=LedEngine::ERS220numMasks[10];
			} else if(RobotName == ERS7Info::TargetName) {
				dot=LedEngine::ERS7numMasks[10];
			}
#endif
			erouter->addTimer(this,sounds.size(),ms->getTime()+delay,false);
			sounds.push_back("numbers/dot.wav");
			for(unsigned int j=0; j<NumLEDs; j++)
				if(FaceLEDMask&(1<<j))
					ms->setOutputCmd(LEDOffset+j,(dot>>j)&1);
			ms->advanceTime(DOT_TIME);
			for(unsigned int j=0; j<NumLEDs; j++)
				if(FaceLEDMask&(1<<j))
					ms->setOutputCmd(LEDOffset+j,(dot>>j)&1);
			ms->advanceTime(FADE_TIME);
			for(unsigned int j=0; j<NumLEDs; j++)
				if(FaceLEDMask&(1<<j))
					ms->setOutputCmd(LEDOffset+j,0);
			ms->advanceTime(BLANK_TIME);
			for(unsigned int j=0; j<NumLEDs; j++)
				if(FaceLEDMask&(1<<j))
					ms->setOutputCmd(LEDOffset+j,0);
			ms->advanceTime(FADE_TIME);
		}
	}
	ms->play();
}

#endif

/*! @file
 * @brief Implements FlashIPAddrBehavior, which displays IP address by flashing a series of numbers on the LED face panel
 * @author ejt (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.5 $
 * $State: Exp $
 * $Date: 2007/08/05 16:16:04 $
 */
