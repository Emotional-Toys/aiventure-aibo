//-*-c++-*-
#ifndef INCLUDED_SoundTestBehavior_h_
#define INCLUDED_SoundTestBehavior_h_

#include "Behaviors/BehaviorBase.h"
#include "Sound/SoundManager.h"
#include "Shared/ERS210Info.h"
#include "Shared/ERS220Info.h"
#include "Shared/ERS7Info.h"

//! allows you to experiment with playing sounds different ways.
/*! A different sound will be played for each of the buttons, except the head buttons.
 *  When the chin button is held down, any sounds (from this behavior) will be queued
 *  up and then played successively once the chin button is released.
 *
 *  Notice that this doesn't preload all needed sounds:\n
 *  - @c barkmed.wav is listed in ms/config/tekkotsu.xml as a preloaded system sound
 *  - @c growl.wav will be loaded before being played automatically - notice the
 *    hiccup this can cause.
 */
class SoundTestBehavior : public BehaviorBase {
public:
	//! Constructor
	SoundTestBehavior()
		: BehaviorBase("SoundTestBehavior"), curplay(SoundManager::invalid_Play_ID), endtime(0),
			LFr(EventBase::buttonEGID,LFrPawOffset,EventBase::activateETID),
			RFr(EventBase::buttonEGID,RFrPawOffset,EventBase::activateETID),
			LBk(EventBase::buttonEGID,LBkPawOffset,EventBase::activateETID),
			RBk(EventBase::buttonEGID,RBkPawOffset,EventBase::activateETID),
			Back(EventBase::buttonEGID,NumButtons-1,EventBase::activateETID)
	{
		if(capabilities.getButtonOffset("BackBut")!=-1U)
			Back.setSourceID(capabilities.getButtonOffset("BackBut"));
		// otherwise unknown model, just use last button (NumButtons-1, as set in constructor)
	}
	
	
	//! Load some sounds, listen for button events
	virtual void DoStart() {
		BehaviorBase::DoStart();
		erouter->addListener(this,EventBase::buttonEGID);
		sndman->loadFile("yap.wav");
		sndman->loadFile("howl.wav");
		sndman->loadFile("whimper.wav");
	}

	//! Release sounds we loaded in DoStart()
	virtual void DoStop() {
		BehaviorBase::DoStop();
		erouter->removeListener(this);
		sndman->releaseFile("howl.wav");
		sndman->releaseFile("yap.wav");
		sndman->releaseFile("whimper.wav");
	}

	//! Play the sound corresponding to the button
	virtual void processEvent(const EventBase& event) {
		if(event==LFr)
			play("howl.wav");
		else if(event==RFr)
			play("yap.wav");
		else if(event==LBk)
			play("whimper.wav");
		else if(event==RBk)
			play("growl.wav");
		else if(event==Back)
			play("barkmed.wav");
		else if(event.getSourceID()==ChinButOffset)
			if(event.getTypeID()==EventBase::activateETID) {
				//start a new chain
				curplay=SoundManager::invalid_Play_ID;
				endtime=0;
			} else if(pauseWhileChin)
				sndman->resumePlay(curplay);
	}

	//! returns name to system
	static std::string getClassDescription() { return "Plays different sounds when buttons are pressed.  Holding the chin button queues the sounds."; }
	virtual std::string getDescription() const { return getClassDescription(); }

protected:
	//! called when a button is pressed - checks if it should enqueue or just play
	void play(const char* name) {
		if(!state->buttons[ChinButOffset]) {

			// Just play the sound
			// This is probably all you need how to do unless you want to get fancy
			sndman->playFile(name);

		} else {

			// Enqueue the sound - mainly useful if you have a set of sounds and want to play a song with them
			if(curplay==SoundManager::invalid_Play_ID || !pauseWhileChin && get_time()>=endtime) {
				//start a new chain, either this is the first or we already finished playing the chain
				curplay=sndman->playFile(name);
				if(pauseWhileChin)
					sndman->pausePlay(curplay);
			} else //add to existing chain
				sndman->chainFile(curplay,name);
			endtime=sndman->getRemainTime(curplay)+get_time()-SoundBufferTime;
			//-SoundBufferTime to guarrantee ID validity, see SoundManager::getRemainTime() documentation

		}
	}
	static const bool pauseWhileChin=true; //!< if this is true, won't start playing chain until you release the chin button
	SoundManager::Play_ID curplay; //!< current chain (may not be valid if chin button not down or time is past #endtime)
	unsigned int endtime; //!< the expected end of play time for the current chain
	
	//!@name Event Templates
	//!Used to match against the different buttons that have sounds mapped to them
	EventBase LFr,RFr,LBk,RBk,Back;
	//@}
};

	/*! @file
 * @brief Defines the SoundTestBehavior demo, which allows you to experiment with playing sounds different ways.
 * @author ejt (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.17 $
 * $State: Exp $
 * $Date: 2007/06/28 04:36:20 $
 */

#endif
