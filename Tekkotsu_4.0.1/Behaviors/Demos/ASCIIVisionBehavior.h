//-*-c++-*-

// This is an empty Behavior template file.
// Replace ASCIIVisionBehavior and streams low-resolution ASCII-art of the camera image to sout as appropriate, and go to town!

#ifndef INCLUDED_ASCIIVisionBehavior_h_
#define INCLUDED_ASCIIVisionBehavior_h_

#include "Behaviors/BehaviorBase.h"
#include "Events/EventRouter.h"

//! streams low-resolution ASCII-art of the camera image to sout
class ASCIIVisionBehavior : public BehaviorBase {
public:
	//! constructor
	ASCIIVisionBehavior() : BehaviorBase("ASCIIVisionBehavior") {}

	static const unsigned int charMapSize=18;  //!< the number of available characters for levels of "gray"
	static const char charMap[charMapSize+1]; //!< the included characters sorted in order of darkness - could be improved... (less is more sometimes)

	virtual void DoStart() {
		BehaviorBase::DoStart(); // do this first
		erouter->addListener(this,EventBase::visRawCameraEGID);
	}

	virtual void DoStop() {
		erouter->removeListener(this);
		BehaviorBase::DoStop(); // do this last
	}

	virtual void processEvent(const EventBase& e);

	static std::string getClassDescription() { return "streams low-resolution ASCII-art of the camera image to sout"; }
	virtual std::string getDescription() const { return getClassDescription(); }
	
protected:
	
};

/*! @file
 * @brief Describes ASCIIVisionBehavior, which streams low-resolution ASCII-art of the camera image to sout
 * @author ejt (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.7 $
 * $State: Exp $
 * $Date: 2005/06/01 05:47:45 $
 */

#endif
