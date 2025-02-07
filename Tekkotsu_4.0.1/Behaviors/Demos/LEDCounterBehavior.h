//-*-c++-*-
#ifndef INCLUDED_LEDCounterBehavior_h_
#define INCLUDED_LEDCounterBehavior_h_

#include "Behaviors/BehaviorBase.h"
#include "Events/EventRouter.h"
#include "Motion/LedMC.h"
#include "Motion/PostureMC.h"
#include "Motion/MMAccessor.h"

//! Counts with LEDs on back button presses
class LEDCounterBehavior : public BehaviorBase {
public:
	LEDCounterBehavior()
		: BehaviorBase("LEDCounterBehavior"),
			inc(EventBase::buttonEGID,FrontBackButOffset,EventBase::activateETID),
			dec(EventBase::buttonEGID,RearBackButOffset,EventBase::activateETID),
			sw(EventBase::buttonEGID,MiddleBackButOffset,EventBase::activateETID),
			style(LedEngine::onedigit), cnt(0), led_id(MotionManager::invalid_MC_ID) {}
	virtual void DoStart() {
		BehaviorBase::DoStart();
		erouter->addListener(this,inc);
		erouter->addListener(this,dec);
		erouter->addListener(this,sw);
		led_id=motman->addPersistentMotion(SharedObject<LedMC>());
		motman->addPrunableMotion(SharedObject<PostureMC>("stand.pos"));
	}
	virtual void DoStop() {
		motman->removeMotion(led_id);
		erouter->removeListener(this);
		BehaviorBase::DoStop();
	}
	virtual void processEvent(const EventBase& e) {
		if(e==inc) {
			MMAccessor<LedMC>(led_id)->displayNumber(++cnt,style);
		} else if(e==dec) {
			MMAccessor<LedMC>(led_id)->displayNumber(--cnt,style);
		} else if(e==sw) {
			style = (style==LedEngine::onedigit ? LedEngine::twodigit : LedEngine::onedigit);
			MMAccessor<LedMC>(led_id)->displayNumber(cnt,style);
		}			
	}

	static std::string getClassDescription() { return "Counts with LEDs on back button presses"; }
	virtual std::string getDescription() const { return getClassDescription(); }

protected:
	EventBase inc,dec,sw;
	LedEngine::numStyle_t style;
	int cnt;
	MotionManager::MC_ID led_id;
};

/*! @file
 * @brief Defines LEDCounterBehavior, which counts with LEDs on back button presses
 * @author ejt (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.1 $
 * $State: Exp $
 * $Date: 2006/02/02 20:08:12 $
 */

#endif
