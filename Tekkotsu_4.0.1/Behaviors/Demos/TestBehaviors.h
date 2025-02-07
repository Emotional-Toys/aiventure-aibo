//-*-c++-*-
#ifndef INCLUDED_TestBehaviors_h_
#define INCLUDED_TestBehaviors_h_

#include "Behaviors/BehaviorBase.h"
#include "Events/EventRouter.h"
#include "Events/TextMsgEvent.h"
#include "Motion/MotionManager.h"
#include "Motion/PostureMC.h"
#include "Shared/ProjectInterface.h"
#include "Vision/RawCameraGenerator.h"
#include "Shared/Config.h"

//! Adds a MotionCommand and then immediately removes it again
class InstantMotionTestBehavior : public BehaviorBase {
public:
	InstantMotionTestBehavior() : BehaviorBase("InstantMotionTestBehavior") {} //!< constructor
	virtual void DoStart() { BehaviorBase::DoStart(); motman->removeMotion(motman->addPersistentMotion(SharedObject<PostureMC>())); DoStop(); }
	static std::string getClassDescription() { return "Adds a MotionCommand and then immediately removes it again"; }
	virtual std::string getDescription() const { return getClassDescription(); }
};

//! When started, busy loops for 3 seconds and then stops
class BusyLoopTestBehavior : public BehaviorBase {
public:
	BusyLoopTestBehavior() : BehaviorBase("BusyLoopTestBehavior") {} //!< constructor
	virtual void DoStart() { BehaviorBase::DoStart(); unsigned int t=get_time(); while(get_time()-t<3000) {} DoStop(); }
	static std::string getClassDescription() { return "When started, busy loops for 3 seconds and then stops"; }
	virtual std::string getDescription() const { return getClassDescription(); }
};

//! Adds a MotionCommand which will busy loop for 3 seconds on its first update, and then autoprune
class BusyMCTestBehavior : public BehaviorBase {
	//! on first updateOutputs, blocks for 3 seconds, then autoprunes on second update
	class BusyMC : public MotionCommand {
		bool hasRun;
	public:
		BusyMC() : MotionCommand(), hasRun(false) {} //!< constructor
		virtual int updateOutputs() {unsigned int t=get_time(); while(get_time()-t<3000) {} hasRun=true; return 0; }
		virtual int isDirty() { return true; }
		virtual int isAlive() { return !hasRun; }
	};
public:
	BusyMCTestBehavior() : BehaviorBase("BusyMCTestBehavior") {} //!< constructor
	virtual void DoStart() { BehaviorBase::DoStart(); motman->addPrunableMotion(SharedObject<BusyMC>()); DoStop(); }
	static std::string getClassDescription() { return "Adds a MotionCommand which will busy loop for 3 seconds on its first update, and then autoprune"; }
	virtual std::string getDescription() const { return getClassDescription(); }
};

//! Stops itself after a second via timer event
class SuicidalBehavior : public BehaviorBase {
public:
	SuicidalBehavior() : BehaviorBase("SuicidalBehavior") {} //!< constructor
	~SuicidalBehavior() { std::cout << getName() << " destructed" << std::endl; }  //!< destructor
	virtual void DoStart() { BehaviorBase::DoStart(); erouter->addTimer(this,0,1000); std::cout << "One second to live!" << std::endl; }
	virtual void processEvent(const EventBase& /*event*/) { std::cout << "I'm stopping -- refresh controller to see if it worked!" << std::endl; DoStop(); }
	static std::string getClassDescription() { return "Stops itself after a second via timer event"; }
	virtual std::string getDescription() const { return getClassDescription(); }
};

//! Echos any text message events received to the console (doesn't add a listener, relies on direct BehaviorSwitchControl passing)
class EchoTextBehavior : public BehaviorBase {
public:
	EchoTextBehavior() : BehaviorBase("EchoTextBehavior") {} //!< constructor
	virtual void processEvent(const EventBase& event) { if(const TextMsgEvent* tev=dynamic_cast<const TextMsgEvent*>(&event)) std::cout << tev->getText() << std::endl; else std::cout << getName() << " got a non-TextMsgEvent" << std::endl; }
	static std::string getClassDescription() { return "Echos any text message events received to the console (doesn't add a listener, relies on direct BehaviorSwitchControl passing)"; }
	virtual std::string getDescription() const { return getClassDescription(); }
};

//! Saves all images currently provided by the hardware to a series of PNG files on the memory stick
/*! Handy for examining the different channels/resolution levels provided by the system, all based on the same input */
class SaveImagePyramidBehavior : public BehaviorBase {
public:
	SaveImagePyramidBehavior() : BehaviorBase("SaveImagePyramidBehavior") {} //!< constructor
	virtual void DoStart() { BehaviorBase::DoStart(); erouter->addListener(this,EventBase::textmsgEGID); }
	virtual void processEvent(const EventBase& event) {
		if(const TextMsgEvent* tev=dynamic_cast<const TextMsgEvent*>(&event)) {
			std::string prefix=config->portPath(tev->getText().substr(0,4));
			saveImage(ProjectInterface::doubleLayer,RawCameraGenerator::CHAN_Y,prefix+"%dy.png");
			for(unsigned int layer=ProjectInterface::fullLayer; layer>=ProjectInterface::quarterLayer; layer--) {
				saveImage(layer,RawCameraGenerator::CHAN_Y,prefix+"%dy.png");
				saveImage(layer,RawCameraGenerator::CHAN_U,prefix+"%du.png");
				saveImage(layer,RawCameraGenerator::CHAN_V,prefix+"%dv.png");
				saveImage(layer,RawCameraGenerator::CHAN_Y_DX,prefix+"%ddx.png");
				saveImage(layer,RawCameraGenerator::CHAN_Y_DY,prefix+"%ddy.png");
				saveImage(layer,RawCameraGenerator::CHAN_Y_DXDY,prefix+"%ddxy.png");
			}
		} else std::cout << getName() << " got a non-TextMsgEvent" << std::endl;
	}
	static std::string getClassDescription() { return "Saves all images currently provided by the hardware to a series of PNG files on the memory stick"; }
	virtual std::string getDescription() const { return getClassDescription(); }
protected:
	void saveImage(unsigned int layer, unsigned int chan, const std::string& name);
};

/*! @file
 * @brief A collection of small test behaviors (some would call them unit tests)
 * @author Ethan Tira-Thompson (ejt) (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Date: 2007/11/11 23:57:18 $
 */

#endif
