//-*-c++-*-
#ifndef INCLUDED_WaypointWalk_h_
#define INCLUDED_WaypointWalk_h_

#include "IPC/ListMemBuf.h"
#include "WalkMC.h"
#include "WaypointEngine.h"

//! Combines a WaypointEngine with a WalkMC so you can walk between a set of waypoints
/*! Note the use of a template so we can have dedicate more or less
 *  space without modifying the class.
 *  
 *  But for everyday use, you can just use the ::WaypointWalkMC typedef
 *  which will default to a maximum of 100 waypoints */
template<unsigned int MAX_WAY>
class WaypointWalk : public WalkMC, public WaypointEngine<MAX_WAY> {
public:
	static const unsigned int MAX_WAYPOINTS=MAX_WAY; //!< for external access to maximum waypoints
	
	//!constructor
	WaypointWalk()
		: WalkMC(), WaypointEngine<MAX_WAYPOINTS>()
	{}

	//!constructor
	WaypointWalk(char * f)
		: WalkMC(), WaypointEngine<MAX_WAYPOINTS>(f)
	{}
	
	//! so we can get our hooks in to modify the target velocity
	virtual int updateOutputs() {
		WaypointEngine<MAX_WAY>::cycle();
		WalkMC::setTargetVelocity(WaypointEngine<MAX_WAY>::curVel[0]*1000,WaypointEngine<MAX_WAY>::curVel[1]*1000,WaypointEngine<MAX_WAY>::curVel[2]);
		//cout << get_time()-waypointTime << " Cur: ("<<curPos[0]<<','<<curPos[1]<<','<<curPos[2]<<")  Ideal: ("<<idealPos[0]<<','<<idealPos[1]<<','<<idealPos[2]<<','<<idealPos[3]<<")  Vel: ("<<curVel[0]<<','<<curVel[1]<<','<<curVel[2]<<")" << endl;
		return WalkMC::updateOutputs();
	}

	virtual int LoadWaypointFile(const char * f) { return WaypointEngine<MAX_WAYPOINTS>::loadFile(f); } //!< allows loading a waypoint file
	virtual int SaveWaypointFile(const char * f) const { return WaypointEngine<MAX_WAYPOINTS>::saveFile(f); } //!< allows saving a waypoint file
	virtual int LoadWalkMCFile(const char * f) { return WalkMC::loadFile(f); } //!< allows loading a WalkMC parameter file
	virtual int SaveWalkMCFile(const char * f) const { return WalkMC::saveFile(f); } //!< allows saving a WalkMC parameter file

};

typedef WaypointWalk<100> WaypointWalkMC; //!< unless you need more/less waypoints, just use this type


/*! @file
 * @brief Defines WaypointWalk, which combines a WaypointEngine with a WalkMC so you can walk between a set of waypoints
 * @author ejt (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.5 $
 * $State: Exp $
 * $Date: 2006/09/09 04:32:58 $
 */

#endif
