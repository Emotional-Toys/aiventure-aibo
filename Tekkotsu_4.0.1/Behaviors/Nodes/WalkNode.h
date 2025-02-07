//-*-c++-*-
#ifndef INCLUDED_WalkNode_h_
#define INCLUDED_WalkNode_h_

#include "MCNode.h"
#include "Motion/MotionManager.h"
#include "Motion/WalkMC.h"
#include "Motion/MMAccessor.h"
#include "Events/LocomotionEvent.h"
#include "Events/EventRouter.h"

//!default name for WalkEngineNode's (have to instantiate a variable in order to use as a template argument)
/*! instantiation will be placed in MCNode.cc to avoid file bloat */
extern const char defWalkNodeName[];
//!default description for WalkEngineNode's (have to instantiate a variable in order to use as a template argument)
/*! instantiation will be placed in MCNode.cc to avoid file bloat */
extern const char defWalkNodeDesc[];

//! A StateNode for walking in a direction, use the template parameter to specify a custom walk MC, or use the ::WalkNode typedef to accept the "default" walk
template<typename W, const char* mcName=defWalkNodeName, const char* mcDesc=defWalkNodeDesc>
class WalkEngineNode : public MCNode<W,mcName,mcDesc> {
public:
	//! lets us interpret values as either distances or velocities
  enum WalkMode_t {
		VelocityWalkMode, //!< #x, #y, #a will be interpreted as mm/s
		DistanceWalkMode //!< #x, #y, #a will be interpreted as millimeters
	};

public:
	//!constructor
	WalkEngineNode()
	  : MCNode<W,mcName,mcDesc>(), x(0), y(0), a(0), n(-1), walkMode()
	{}

	//!constructor, positive @a yvel is counter-clockwise from above (to match coordinate system), assumes velocity
	WalkEngineNode(float xvel, float yvel, float avel)
	  : MCNode<W,mcName,mcDesc>(), x(xvel), y(yvel), a(avel), n(-1), walkMode(VelocityWalkMode)
	{}

	//!constructor, positive @a yvel is counter-clockwise from above (to match coordinate system), assumes distance
	WalkEngineNode(float xdist, float ydist, float adist, int steps)
	  : MCNode<W,mcName,mcDesc>(), x(xdist), y(ydist), a(adist), n(steps), walkMode(DistanceWalkMode)
	{}

	//!constructor, positive @a yvel is counter-clockwise from above (to match coordinate system), assumes velocity
	WalkEngineNode(const std::string& name, float xvel, float yvel, float avel)
	  : MCNode<W,mcName,mcDesc>(name), x(xvel), y(yvel), a(avel), n(-1), walkMode(VelocityWalkMode)
	{}

	//!constructor, positive @a yvel is counter-clockwise from above (to match coordinate system), assumes distance
	WalkEngineNode(const std::string& name, float xdist, float ydist, float adist, int steps)
	  : MCNode<W,mcName,mcDesc>(name), x(xdist), y(ydist), a(adist), n(steps), walkMode(DistanceWalkMode)
	{}

	//!destructor
	~WalkEngineNode() {}

	//! sets the velocity of the walk
	/*! @param xdist x displacement (mm, positive is forward)
	 *  @param ydist y displacement (mm, positive is left)
	 *  @param adist angular displacement (rad, positive is counter-clockwise)
	 *  @param steps how many steps to take to achieve displacement (velocity depends on walk's cycle time parameter) */
	void setDisplacement(float xdist, float ydist, float adist, int steps = -1) {
		storeValues(xdist, ydist, adist, steps, DistanceWalkMode);
	}

	//! sets the velocity of the walk
	/*! @param xvel x velocity (mm/s, positive is forward)
	 *  @param yvel y velocity (mm/s, positive is left)
	 *  @param avel angular velocity (rad/s, positive is counter-clockwise)
	 *  @param np how many steps to take at specified velocity (resulting distance depends on walk's cycle time parameter) */
	void setVelocity(float xvel, float yvel, float avel, int np = -1) {
		storeValues(xvel, yvel, avel, np, VelocityWalkMode);
	}
	
	//! sets the velocity in x direction (positive is forward)
	void setXVelocity(float xvel) { x=xvel; storeValues(xvel, y, a, n, VelocityWalkMode); }

	//! returns the velocity in x direction (positive is forward)
	float getX() { return x; }
	
	//! sets the velocity in y direction (positive is left)
	void setYVelocity(float yvel) { y=yvel; storeValues(x, yvel, a, n, VelocityWalkMode); }
	
	//! returns the velocity in y direction (positive is left)
	float getY() { return y; }
	
	//! sets the velocity of the turn, positive is counter-clockwise from above (to match coordinate system)
	void setAVelocity(float avel) { a=avel; storeValues(x, y, avel, n, VelocityWalkMode); }
	
	//! returns the velocity of the turn (positive is counter-clockwise from above (to match coordinate system))
	float getA() { return a; }
	
	//! returns the number of steps being taken (not necessarily the number *remaining*)
	int getSteps() { return n; }

	virtual void DoStart() {
		MCNode<W,mcName,mcDesc>::DoStart();
		updateWMC();
	}

protected:
	//!constructor, positive @a yvel is counter-clockwise from above (to match coordinate system), assumes velocity
	WalkEngineNode(const std::string& className, const std::string& instanceName, float xvel, float yvel, float avel)
	: MCNode<W,mcName,mcDesc>(className,instanceName), x(xvel), y(yvel), a(avel), n(-1), walkMode(VelocityWalkMode)
	{}
	
	//!constructor, positive @a yvel is counter-clockwise from above (to match coordinate system), assumes distance
	WalkEngineNode(const std::string& className, const std::string& instanceName, float xdist, float ydist, float adist, int steps)
	: MCNode<W,mcName,mcDesc>(className,instanceName), x(xdist), y(ydist), a(adist), n(steps), walkMode(DistanceWalkMode)
	{}

	//! stores the values and if active, calls updateWMC()
	void storeValues(float xp, float yp, float ap, int np, WalkMode_t wmode) {
		x = xp;
		y = yp;
		a = ap;
		n = np;
		walkMode = wmode;

		if(MCNode<W,mcName,mcDesc>::isActive())
			updateWMC();
	}

	//! makes the appropriate calls on the WalkMC
	void updateWMC() {
		MMAccessor<W> walk = MCNode<W,mcName,mcDesc>::getMC();
		switch(walkMode) {
		case VelocityWalkMode:
			walk->setTargetVelocity(x,y,a,n);
			break;
		case DistanceWalkMode:
			walk->setTargetDisplacement(x,y,a,n); // WalkMC will calculate velocities.
			break;
		default:
			std::cout << "Unknown Walk Mode" << std::endl;
			break;
		}
	}

	float x; //!< velocity in x direction (positive is forward), or distance if #walkMode is DistanceWalkMode
	float y; //!< velocity in y direction (positive is dog's left), or distance if #walkMode is DistanceWalkMode
	float a; //!< velocity of the turn, positive is counter-clockwise from above (to match coordinate system), or distance if #walkMode is DistanceWalkMode
	int n; //!< number of steps (-1 means walk forever)
	WalkMode_t walkMode; //!< the current interpretation of #x, #y, and #a
};

//! the prototypical WalkNode, using a WalkMC
typedef WalkEngineNode<WalkMC> WalkNode;

/*! @file
 * @brief Describes WalkEngineNode,  a StateNode for walking in a direction; use the template parameter to specify a custom walk MC, or use the WalkNode typedef to accept the "default" walk
 * @author ejt (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.20 $
 * $State: Exp $
 * $Date: 2006/09/27 20:10:27 $
 */

#endif
