//-*-c++-*-
#ifndef INCLUDED_MCRepeater_h_
#define INCLUDED_MCRepeater_h_

#include "Behaviors/BehaviorBase.h"
#include "IPC/SharedObject.h"
#include "Motion/MotionManager.h"
#include "Events/EventRouter.h"

//! Sends a given MotionCommand to MotionManager, waits until it autoprunes, and then sends it again.
class MCRepeater : public BehaviorBase {
public:
	//! constructor
	MCRepeater(const SharedObjectBase* sharedmc) : BehaviorBase("MCRepeater"), mc(sharedmc) {}

	virtual void DoStart() {
		BehaviorBase::DoStart(); // do this first
		MotionManager::MC_ID id=motman->addPrunableMotion(*mc);
		erouter->addListener(this,EventBase::motmanEGID,id,EventBase::deactivateETID);
	}

	virtual void DoStop() {
		erouter->removeListener(this);
		BehaviorBase::DoStop(); // do this last
	}

	virtual void processEvent(const EventBase& /*e*/) {
		erouter->removeListener(this);
		MotionManager::MC_ID id=motman->addPrunableMotion(*mc);
		erouter->addListener(this,EventBase::motmanEGID,id,EventBase::deactivateETID);
	}

	static std::string getClassDescription() { return "Sends a given MotionCommand to MotionManager, waits until it autoprunes, and then sends it again."; }
	virtual std::string getDescription() const { return getClassDescription(); }
	
protected:
	const SharedObjectBase* mc; //!< the motion command being repeated

private:
	MCRepeater(const MCRepeater&); //!< don't call
	MCRepeater operator=(const MCRepeater&); //!< don't call
};

/*! @file
 * @brief Defines MCRepeater, which sends a given MotionCommand to MotionManager, waits until it autoprunes, and then sends it again.
 * @author ejt (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.6 $
 * $State: Exp $
 * $Date: 2005/06/01 05:47:45 $
 */

#endif
