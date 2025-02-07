//-*-c++-*-
#ifndef INCLUDED_RoverControllerBehavior_h_
#define INCLUDED_RoverControllerBehavior_h_

#include "Shared/RobotInfo.h"
#if !defined(TGT_HAS_ARMS) && !defined(TGT_HAS_LEGS) || !defined(TGT_HAS_HEAD)
// no manipulator, but we can still compile, so it's only a warning
#  warning RoverControllerBehavior can't find a manipulator, won't run
#endif

#include "Behaviors/BehaviorBase.h"
#include "Behaviors/Controller.h"
#include "Events/EventRouter.h"
#include "Motion/PostureMC.h"
#include "Motion/HeadPointerMC.h"

//! DESCRIPTION
class RoverControllerBehavior : public BehaviorBase {
public:
	static const int PORT=10056; //!< port number to open up
	
	//! returns #theOne, initializing it if necessary
	static RoverControllerBehavior& getInstance() {
		if(theOne==NULL)
			theOne = new RoverControllerBehavior;
		return *theOne;
	}
	
	//! destructor
	virtual ~RoverControllerBehavior() {
		if(isActive())
			DoStop();
		theOne=NULL;
	}
	
	virtual void DoStart();
	virtual void DoStop();

	static std::string getClassDescription() { return "DESCRIPTION"; }
	virtual std::string getDescription() const { return getClassDescription(); }
	

protected:
	//! constructor
	RoverControllerBehavior()
		: BehaviorBase("Rover Remote Control"),
		tgtScale(1), tgtXOff(0), tgtYOff(0), heightScale(1), heightOff(0), distanceScale(1),
		tgtX(0),tgtY(0),tgtZ(0), lookX(0),lookY(0),lookD(0),
		gripperAngle(0),perspective(0),
		autoGripperAngle(true),autoPerspective(true),autoTrackGripper(true),maxDist(true),
		cmdsock(NULL), head(MotionManager::invalid_MC_ID), arm(MotionManager::invalid_MC_ID)
	{
		initScale();
	}
	
	void initScale();
	
	//! called by wireless when there's new data, forwards call to theOne->update
	static int callback(char *buf, int bytes) { return theOne->update(buf,bytes); }
	//! called by wireless (via callback()) when there's new data
	int update(char *buf, int bytes);
	
	void updateArm();
	void updateHead();
	
	static RoverControllerBehavior * theOne;
	
	float tgtScale;
	float tgtXOff;
	float tgtYOff;
	float heightScale;
	float heightOff;
	float distanceScale;
	
	float tgtX;
	float tgtY;
	float tgtZ;
	float lookX;
	float lookY;
	float lookD;
	
	float gripperAngle;
	float perspective;
	
	bool autoGripperAngle;
	bool autoPerspective;
	bool autoTrackGripper;
	bool maxDist;
	
	Socket *cmdsock; //!< The input command stream socket
	
	MotionManager::MC_ID head;
	MotionManager::MC_ID arm;
	
private:
	// Providing declarations for these functions will avoid a compiler warning if
	// you have any class members which are pointers.  However, as it is, an error
	// will result if you inadvertantly cause a call to either (which is probably
	// a good thing, unless you really intended to copy/assign a behavior, in
	// which case simply provide implementations for the functions)
	RoverControllerBehavior(const RoverControllerBehavior&); //!< don't call (copy constructor)
	RoverControllerBehavior& operator=(const RoverControllerBehavior&); //!< don't call (assignment operator)
};

/*! @file
 * @brief Defines RoverControllerBehavior, which DESCRIPTION
 * @author Ethan Tira-Thompson (ejt) (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.4 $
 * $State: Exp $
 * $Date: 2007/11/13 16:41:14 $
 */

#endif
