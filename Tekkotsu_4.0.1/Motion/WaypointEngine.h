//-*-c++-*-
#ifndef INCLUDED_WaypointEngine_h_
#define INCLUDED_WaypointEngine_h_

#include "IPC/ListMemBuf.h"
#include "Shared/LoadSave.h"
#include "Shared/Config.h"

//! Provides computation and management of a desired path through a series of waypoints
/*! This is a generalized set of data structures and management code -
 *  it doesn't actually say anything about @e how you get from one
 *  waypoint to the other, it will just tell you where you should be
 *  going at any given time.
 *
 *  So, for instance, you may be interested in WaypointWalk, which
 *  will use a WalkMC to traverse the waypoints.  Future development
 *  may include a WaypointPush, to push an object along a path instead
 *  of just moving the body along a path.
 *
 *  Although general curves between waypoints are not supported, you
 *  can use either circular arcs or straight lines.
 *
 *  The Waypoint class holds the actual data about each waypoint.  You
 *  can specify waypoints in 3 ways: egocentric, offset, and absolute.
 *
 *  <table cellspacing=0 cellpadding=0 width="750" class="figures" align="center" border="0"><tr>
 *  <td class="figure"><img src="Waypoint_Ego.png"><div style="padding:10px;">
 *    @b Egocentric: the x and y parameters are relative to the body
 *    itself; x is always forward and y is always left.  Handy for
 *    turtle/logo style specification of instructions
 *  </div></td><td class="figure"><img src="Waypoint_Off.png"><div style="padding:10px;">
 *    @b Offset: the x and y parameters are relative to the current body
 *    position, but not its heading.
 *  </div></td><td class="figure"><img src="Waypoint_Abs.png"><div style="padding:10px;">
 *    @b Absolute: the x and y parameters are direct coordinates
 *  </div></td></tr></table>
 *
 *  These specify the @e position of the next waypoint, but we also need
 *  to be able to specify the @e orientation (heading) of the robot.  This
 *  is done by specifying an angle and a mode which controls how that
 *  angle is interpreted: Waypoint::angleIsRelative, which can be @c true or @c false.
 *
 *  <table cellspacing=0 cellpadding=0 width="500" class="figures" align="center" border="0"><tr>
 *  <td class="figure"><img src="Waypoint_AngleRel.png"><div style="padding:10px;">
 *  <code>angleIsRelative==</code>@b true: The angle is relative to the path, so that @c 0 will keep
 *  the robot pointed in the direction of travel, even when arcing.  Similarly, @f$\pi/2\ (90^\circ)@f$
 *  would cause the robot to walk sideways.
 *  </div></td><td class="figure"><img src="Waypoint_AngleAbs.png"><div style="padding:10px;">
 *  <code>angleIsRelative==</code>@b false: The angle is relative to the world coordinate system, so a 
 *  constant heading is maintained throughout execution of the path.
 *  </div></td></tr></table>
 *
 *  The final orientation of the robot is simply the heading it was
 *  facing when it reaches the end point.  To turn in place, you can
 *  use a (0,0) egocentric or offset waypoint with an angle parameter.
 *
 *  In order to execute curves, you can supply an arc value:
 *
 *  <table cellspacing=0 cellpadding=0 width="417" class="figures" align="center" border="0"><tr>
 *  <td class="figure"><img src="Waypoint_Arc.png"><div style="padding:10px;">
 *  Here you see the results of 3 different arc values @f$(60^\circ,120^\circ,180^\circ)@f$.  Note how
 *  the arc parameter corresponds to the angle of the circle which is swept.<br>
 *  </div></td></tr></table>
 *
 *  There are two ways to specify arcs.  The @c add*Waypoint functions
 *  use the position arguments to specify the <em>end point</em> of
 *  the arc, and the arc parameter serves to "bow" the path.  The @c
 *  add*Arc functions specify the <em>center of the circle</em> as the
 *  position, and the end point is inferred from the amount of the arc
 *  to sweep.
 *
 *  Beware that arcs greater than @f$180^\circ@f$ are entirely
 *  possible, but will create larger and larger circles which may
 *  cause the robot to initially start moving @e away from the
 *  destination.  This isn't necessarily a bad thing, but may be
 *  unanticipated.  Values approaching @f$2\pi\ (360^\circ)@f$ may
 *  cause numerical instability yielding infinitely large circles.
 *  Values larger than @f$2\pi\ (360^\circ)@f$ will be normalized to
 *  the range @f$[0,2\pi)@f$.
 *
 *  Dead reckoning is very prone to accruing error.  It is highly
 *  recommended that you calibrate the locomotion mechanism carefully
 *  (see WalkCalibration, available under the "Walk Edit" menu with a
 *  run-time help menu) and implement some form of localization to
 *  handle the inevitable drift.
 *
 *  If you have a localization module in place, you can use the
 *  setCurPos() function to update the position of the robot within
 *  the world.  WaypointEngine provides two ways to handle this
 *  ensuing discrepency from the path the robot had been tracing:
 *
 *  <table cellspacing=0 cellpadding=0 width="325" class="figures" align="center" border="0"><tr>
 *  <td class="figure"><img src="Waypoint_Error.png"><div style="padding:10px;">
 *  The effect of the Waypoint::trackPath flag.  When @c true, the robot will attempt to catch up
 *  to its "ideal" location after a perturbation.  When @c false, the robot will ignore the "ideal"
 *  path, and just go straight to the destination from wherever perturbations may push it.
 *  </div></td></tr></table>
 *
 *  trackPath is a per-waypoint setting, setTracking() sets the
 *  default value for any new waypoints which are thereafter created
 *  (the default default is false ;)
 *
 *  Waypoint list files are a fairly straightforward plain text format.
 *  The extension .wyp is suggested.
 *
 *  The waypoint file format is:
 *  - '<tt>\#WyP</tt>' - header to verify file type
 *  - A series of entries:
 *    - '<tt>max_turn_speed </tt><i>num</i>' - sets the maximum error-correction turning speed used for all following waypoints
 *    - '<tt>track_path </tt><i>bool</i>' - sets trackpath mode on or off for all following waypoints (see Waypoint::trackPath)
 *    - '<tt>add_point </tt>{<tt>ego</tt>|<tt>off</tt>|<tt>abs</tt>}<tt> </tt><i>x_val</i><tt> </tt><i>y_val</i><tt> </tt>{<tt>hold</tt>|<tt>follow</tt>}<tt> </tt><i>angle_val</i><tt> </tt><i>speed_val</i><tt> </tt><i>arc_val</i>' - adds the waypoint with the parameters given, see Waypoint, similar to add*Waypoint functions
 *    - '<tt>add_arc </tt>{<tt>ego</tt>|<tt>off</tt>|<tt>abs</tt>}<tt> </tt><i>x_val</i><tt> </tt><i>y_val</i><tt> </tt>{<tt>hold</tt>|<tt>follow</tt>}<tt> </tt><i>angle_val</i><tt> </tt><i>speed_val</i><tt> </tt><i>arc_val</i>' - adds the waypoint with the parameters given, see Waypoint, similar to add*Arc functions
 *  - '<tt>\#END</tt>' - footer to verify ending of file
 */
template<unsigned int MAX_WAY>
class WaypointEngine : public LoadSave {
public:
	static const unsigned int MAX_WAYPOINTS=MAX_WAY; //!< for external access to maximum waypoints

	//! Holds information about each waypoint, see WaypointEngine for overview
	struct Waypoint {
		//! defines different ways to interpret the position values
		enum posType_t {
			POSTYPE_EGOCENTRIC, //!< #x and #y are relative to current heading - so x is forward and y is strafe
			POSTYPE_OFFSET, //!< #x and #y are oriented with the coordinates, but relative to current location (delta x and delta y)
			POSTYPE_ABSOLUTE //!< #x and #y are a specific coordinate location
		};
		Waypoint()
			: x(0), y(0), angle(0), arc(), speed(), turnSpeed(), posType(), angleIsRelative(), trackPath()
		{} //!< constructor
		Waypoint(float xc, float yc, Waypoint::posType_t pos_rel, float ac, bool ang_rel, float spd, bool track, float turn)
			: x(xc), y(yc), angle(ac), arc(0), speed(spd), turnSpeed(turn), posType(pos_rel), angleIsRelative(ang_rel), trackPath(track)
		{} //!< constructor
		float x; //!< the displacement along x (meters), subject to #posType
		float y; //!< the displacement along y (meters), subject to #posType
		float angle; //!< either the angle relative to path to maintain, or the heading to maintain, see #angleIsRelative
		float arc; //!< angle of sector of arc to use to get to waypoint (0 means straight line)
		float speed; //!< speed (in meters per second)
		float turnSpeed; //!< maximum speed to correct heading (in radians per second)
		posType_t posType; //!< lets us know how to interpret the #x and #y values
		bool angleIsRelative; //!< if true, #angle is interpreted as relative to the path; otherwise, interpreted as an absolute heading to maintain
		bool trackPath; //!< if true, if off course, will attempt to get back on path at the ideal location; if false, simply heads directly for waypoint from whereever it is
	};

	typedef ListMemBuf<Waypoint,MAX_WAYPOINTS> WaypointList_t; //!< convenient shorthand
	typedef typename ListMemBuf<Waypoint,MAX_WAYPOINTS>::index_t WaypointListIter_t; //!< convenient shorthand

	//! constructor
	WaypointEngine()
		: LoadSave(), waypoints(), isRunning(false), isLooping(false), isTracking(false),
			curWaypoint(waypoints.end()), waypointTime(0), waypointDistance(0), pathLength(0), arcRadius(0),
			lastUpdateTime(0), Pcorr(.5), turnSpeed(.65)
	{init();}
	//! constructor
	WaypointEngine(char * f)
		: LoadSave(), waypoints(), isRunning(false), isLooping(false), isTracking(false),
			curWaypoint(waypoints.end()), waypointTime(0), waypointDistance(0), pathLength(0), arcRadius(0),
			lastUpdateTime(0), Pcorr(.5), turnSpeed(.65)
	{init(); loadFile(f); }

	//! returns a rough overestimate of the size needed
	/*! pretends we need to switch max_turn_speed and track_path on
		every point, and the longest options are given for every point */
	virtual unsigned int getBinSize() const;
	virtual unsigned int loadBuffer(const char buf[], unsigned int len);
	virtual unsigned int saveBuffer(char buf[], unsigned int len) const;
	virtual unsigned int loadFile(const char * filename) { return LoadSave::loadFile(config->motion.makePath(filename).c_str()); }
	virtual unsigned int saveFile(const char * filename) const { return LoadSave::saveFile(config->motion.makePath(filename).c_str()); }

	virtual void go();      //!< starts walking towards the first waypoint
	virtual void pause();   //!< halts execution of waypoint list
	virtual void unpause(); //!< resumes execution of waypoint list from last paused location

	virtual void setIsLooping(bool isl) { isLooping=isl; } //!< sets #isLooping
	virtual bool getIsLooping() const { return isLooping; } //!< returns #isLooping

	virtual WaypointList_t& getWaypointList() { return waypoints; } //!< returns a reference to #waypoints
	virtual const WaypointList_t& getWaypointList() const { return waypoints; } //!< returns a const reference to #waypoints

	virtual WaypointListIter_t getCurWaypointID() const { return curWaypoint; } //!< returns id value of current waypoint (#curWaypoint)

	virtual float getCurX() const { return curPos[0]; } //!< returns current x position
	virtual float getCurY() const { return curPos[1]; } //!< returns current y position
	virtual float getCurA() const { return curPos[2]; } //!< returns current heading
	//! sets the current position (for instance your localization module has an update)
	virtual void setCurPos(float x, float y, float a) {
		curPos[0]=x; curPos[1]=y; curPos[2]=a;
	}

	virtual void setTracking(bool b) { isTracking=b; } //!< sets the #isTracking flag, only affects future waypoints which are added, not currently listed waypoints (use getWaypointList() to modify existing waypoints)
	virtual bool getTracking() const { return isTracking; } //!< returns #isTracking

	//! call this on each opportunity to check current location and correct velocities
	/*! @return #isRunning for convenience of checking if anything is happening */
	virtual bool cycle(); 

	//!these are for convenience - can also directly edit the waypoint list using access from getWaypointList()
	//!@name Adding Waypoints

	//! adds a waypoint to the end of the list, allows you to specify turtle-style instructions
	/*! <img src="Waypoint_Ego.png">
	 *  @param forward distance forward to move (negative to move backward of course)
	 *  @param left distance to the left to move (negative to move right of course)
	 *  @param angle angle of attack to use on the path
	 *  @param angleIsRelative controls interpretation of @a angle; true means angle specifies an offset from the bearing of the target waypoint, false means maintain an absolute heading
	 *  @param speed is the speed to move at; meters per second */
	virtual void addEgocentricWaypoint(float forward, float left, float angle, bool angleIsRelative, float speed) {
		waypoints.push_back(Waypoint(forward,left,Waypoint::POSTYPE_EGOCENTRIC,angle,angleIsRelative,speed,isTracking,turnSpeed));
	}
	//! adds a waypoint to the end of the list, allows you to set locations relative to the location of the previous waypoint (or starting position)
	/*! <img src="Waypoint_Off.png">
	 *  @param x distance delta along x axis of the waypoint
	 *  @param y distance delta along y axis of the waypoint
	 *  @param angle angle of attack to use on the path
	 *  @param angleIsRelative controls interpretation of @a angle; true means angle specifies an offset from the bearing of the target waypoint, false means maintain an absolute heading
	 *  @param speed is the speed to move at; meters per second */
	virtual void addOffsetWaypoint(float x, float y, float angle, bool angleIsRelative, float speed) {
		waypoints.push_back(Waypoint(x,y,Waypoint::POSTYPE_OFFSET,angle,angleIsRelative,speed,isTracking,turnSpeed));
	}
	//! adds a waypoint to the end of the list, allows you to set locations relative to the world coordinate frame
	/*! <img src="Waypoint_Abs.png">
	 *  @param x position along x axis of the waypoint
	 *  @param y position along y axis of the waypoint
	 *  @param angle angle of attack to use on the path
	 *  @param angleIsRelative controls interpretation of @a angle; true means angle specifies an offset from the bearing of the target waypoint, false means maintain an absolute heading
	 *  @param speed is the speed to move at; meters per second */
	virtual void addAbsoluteWaypoint(float x, float y, float angle, bool angleIsRelative, float speed) {
		waypoints.push_back(Waypoint(x,y,Waypoint::POSTYPE_ABSOLUTE,angle,angleIsRelative,speed,isTracking,turnSpeed));
	}

	//! adds a waypoint to the end of the list, using an arcing path to get there, allows you to specify turtle-style instructions to specify the focus of the arc
	/*! <img src="Waypoint_Ego.png">
	 *  If you would rather specify the ending point and then "bow" the path, try addEgocentricWaypoint() followed by setting the Waypoint::arc field directly
	 *  @param forward distance in front of the center of the circle of the arc
	 *  @param left distance to the left of the center of the circle of the arc
	 *  @param angle angle of attack to use on the path
	 *  @param angleIsRelative controls interpretation of @a angle; true means angle specifies an offset from the bearing of the target waypoint, false means maintain an absolute heading
	 *  @param speed is the speed to move at; meters per second
	 *  @param arc is the number of radians the arc fills; arcs near 0 (or multiples of 360) may cause numeric instability */
	virtual void addEgocentricArc(float forward, float left, float angle, bool angleIsRelative, float speed, float arc) {
		addEgocentricWaypoint(forward,left,angle,angleIsRelative,speed);
		fixArc(arc);
	}
	//! adds a waypoint to the end of the list, using an arcing path to get there, allows you to specify locations relative to previous waypoint to specify the focus of the arc
	/*! <img src="Waypoint_Off.png">
	 *  If you would rather specify the ending point and then "bow" the path, try addOffsetWaypoint() followed by setting the Waypoint::arc field directly
	 *  @param x distance delta along x of the center of the circle of the arc
	 *  @param y distance delta along y of the center of the circle of the arc
	 *  @param angle angle of attack to use on the path
	 *  @param angleIsRelative controls interpretation of @a angle; true means angle specifies an offset from the bearing of the target waypoint, false means maintain an absolute heading
	 *  @param speed is the speed to move at; meters per second
	 *  @param arc is the number of radians the arc fills; arcs near 0 (or multiples of 360) may cause numeric instability */
	virtual void addOffsetArc(float x, float y, float angle, bool angleIsRelative, float speed, float arc) {
		addOffsetWaypoint(x,y,angle,angleIsRelative,speed);
		fixArc(arc);
	}
	//! adds a waypoint to the end of the list, using an arcing path to get there, allows you to specify absolute locations to specify the focus of the arc
	/*! <img src="Waypoint_Abs.png">
	 *  If you would rather specify the ending point and then "bow" the path, try addAbsoluteWaypoint() followed by setting the Waypoint::arc field directly
	 *  @param x position along x of the center of the circle of the arc
	 *  @param y position along y of the center of the circle of the arc
	 *  @param angle angle of attack to use on the path
	 *  @param angleIsRelative controls interpretation of @a angle; true means angle specifies an offset from the bearing of the target waypoint, false means maintain an absolute heading
	 *  @param speed is the speed to move at; meters per second
	 *  @param arc is the number of radians the arc fills; arcs near 0 (or multiples of 360) may cause numeric instability */
	virtual void addAbsoluteArc(float x, float y, float angle, bool angleIsRelative, float speed, float arc) {
		addAbsoluteWaypoint(x,y,angle,angleIsRelative,speed);
		fixArc(arc);
	}

	//@}
	
	//! will set the currently active waypoint to another waypoint; correctly calculates location of intermediate waypoints so target location will be the same as if the intervening waypoints had actually been executed
	virtual void setTargetWaypoint(WaypointListIter_t iter) {
		//cout << "Moving to waypoint " << iter << endl;
		bool isLoop=false;
		if(iter==waypoints.end()) {
			if(isLooping && waypoints.size()>0) { //restart at beginning
				iter=waypoints.begin();
				for(unsigned int i=0; i<3; i++)
					pathStartPos[i]=curPos[i];
				isLoop=true;
			} else { //not looping, halt
				isRunning=false;
				curWaypoint=iter;
				for(unsigned int i=0; i<3; i++) {
					sourcePos[i]=targetPos[i];
					targetPos[i]=curPos[i];
					curVel[i]=0;
				}				
				return;
			}
		}
		if(iter==waypoints.next(curWaypoint) || isLoop)
			for(unsigned int i=0; i<3; i++)
				sourcePos[i]=targetPos[i];
		else
			for(unsigned int i=0; i<3; i++)
				sourcePos[i]=curPos[i];

		Waypoint target;
		if(isLoop)
			target=calcAbsoluteCoords(iter,pathStartPos[0],pathStartPos[1],pathStartPos[2]);
		else
			target=calcAbsoluteCoords(iter);
		targetPos[0]=target.x;
		targetPos[1]=target.y;
		targetPos[2]=target.angle;

		float dx=targetPos[0]-sourcePos[0];
		float dy=targetPos[1]-sourcePos[1];
		waypointDistance=sqrt(dx*dx+dy*dy);
		waypointTime=get_time();
		curWaypoint=iter;

		float radiusRatio=sin(waypoints[iter].arc/2);
		arcRadius = (radiusRatio==0) ? 0 : (waypointDistance/2)/radiusRatio;
		pathLength = arcRadius!=0 ? arcRadius*waypoints[iter].arc : waypointDistance;

		std::cout << "Target is now: ("<<targetPos[0]<<','<<targetPos[1]<<','<<targetPos[2]<<")" << std::endl;
	}

	//!if @a it follows the current waypoint, applies all the waypoints between #curWaypoint and @a it and returns result as an absolute position (angle field stores heading); otherwise calls the other calcAbsoluteCoords(WaypointListIter_t, float, float, float)
	Waypoint calcAbsoluteCoords(WaypointListIter_t it) {
		//find out if 'it' is coming up, or already passed
		bool isAhead=false;
		for(WaypointListIter_t c=curWaypoint; c!=waypoints.end(); c=waypoints.next(c))
			if(c==it) {
				isAhead=true;
				break;
			}
		if(!isAhead)
			return calcAbsoluteCoords(it,pathStartPos[0],pathStartPos[1],pathStartPos[2]);
		Waypoint cur(targetPos[0],targetPos[1],Waypoint::POSTYPE_ABSOLUTE,targetPos[2],false,0,isTracking,turnSpeed);
		if(it==curWaypoint)
			return cur;
		for(WaypointListIter_t c=waypoints.next(curWaypoint); c!=waypoints.end(); c=waypoints.next(c)) {
			applyWaypoint(cur,waypoints[c]);
			if(c==it)
				break;
		}
		return cur;
	}

	//!starts at (@a sx, @a sy, heading=@a sa) and then applies all the waypoints up through @a it and returns result as an absolute position (angle field stores heading)
	Waypoint calcAbsoluteCoords(WaypointListIter_t it,float sx, float sy, float sa) {
		Waypoint cur(sx,sy,Waypoint::POSTYPE_ABSOLUTE,sa,false,0,isTracking,turnSpeed);
		for(WaypointListIter_t c=waypoints.begin(); c!=waypoints.end(); c=waypoints.next(c)) {
			applyWaypoint(cur,waypoints[c]);
			if(c==it)
				break;
		}
		return cur;
	}


protected:
	void init(); //!< basic memory initialization

	//!< if @a next is a relative waypoint (offset or egocentric), it is added to the location held in @a cur; otherwise if @a next is absolute, @a cur is set to @a next
	/*! the Waypoint::angle field is used to store the headings */
	void applyWaypoint(Waypoint& cur, const Waypoint& next); 

	//! assumes the last waypoint is actually center of circle, adjusts it to be the endpoint of following @a arc radians around that circle instead
	void fixArc(float arc);
	
	//! based on current velocity and time since last call, dead reckons current location in #curPos
	/*! doesn't take acceleration into account, but should... :( */
	void computeCurrentPosition(unsigned int t);
	void checkNextWaypoint(unsigned int t);  //!< checks to see if #curPos is within #eps of #targetPos; if so, setTargetWaypoint() to next waypoint
	void computeIdeal(unsigned int t);       //!< computes the ideal location (#idealPos) if we were following the intended path at the intended speed
	void computeNewVelocity(unsigned int t); //!< computes the velocity which should be used given the current position (#curPos) relative to the ideal position (#idealPos)

	//! will set a to be between (-pi,pi) (inclusive), just like atan2()
	static float normalizeAngle(float a) {
		while(a>M_PI)
			a-=M_PI*2;
		while(a<-M_PI)
			a+=M_PI*2;
		return a;
	}

	//! returns @a x such that it is at most @a max and at minimum @a min
	static float clipRange(float x, float min, float max) {
		if(x<min)
			return min;
		else if(x>max)
			return max;
		else
			return x;
	}

	WaypointList_t waypoints; //!< storage for the waypoints

	bool isRunning;  //!< true if we're currently executing the path
	bool isLooping;  //!< true if we should loop when done
	bool isTracking; //!< new waypoints will use trackPath mode
	unsigned int curWaypoint;  //!< index of current waypoint
	unsigned int waypointTime; //!< time we started working on current waypoint
	float waypointDistance;    //!< distance from #sourcePos to #targetPos
	float pathLength;          //!< distance to be traveled from #sourcePos to #targetPos (may differ from #waypointDistance due to arcing)
	float arcRadius;           //!< radius of current arc, may be inf or NaN if using a straight line; can also be negative depending on direction!
	unsigned int lastUpdateTime; //!< time we last updated curPos
	float pathStartPos[3]; //!< position when started execution of current path (aka origin offset for relative positions which preceed an absolute waypoint)
	float sourcePos[3]; //!< source position of the robot relative to the origin, aka absolute position of previous waypoint
	float targetPos[3]; //!< target position of the robot relative to the origin, aka absolute position of next waypoint
	float idealPos[4];  //!< ideal position of the robot relative to the origin, (x, y, heading, last element is desired direction of motion)
	float curPos[3];    //!< current position of the robot relative to the origin
	float curVel[3];    //!< current velocity
	float eps[3];       //!< epsilon - "close enough" to register a hit on the waypoint
	float Pcorr;        //!< proportional correction factor for tracking path
	float turnSpeed;    //!< maximum turning speed for new waypoints
};

template<unsigned int MAX_WAY>
void WaypointEngine<MAX_WAY>::go() {
	isRunning=true;
	for(unsigned int i=0; i<3; i++) {
		curVel[i]=0;
		pathStartPos[i]=sourcePos[i]=curPos[i];
	}
	Waypoint target(curPos[0],curPos[1],Waypoint::POSTYPE_ABSOLUTE,curPos[2],false,0,isTracking,turnSpeed);
	applyWaypoint(target,waypoints.front());
	targetPos[0]=target.x;
	targetPos[1]=target.y;
	targetPos[2]=target.angle;
	lastUpdateTime=get_time();
	curWaypoint=waypoints.begin();
	setTargetWaypoint(curWaypoint);
}

template<unsigned int MAX_WAY>
void WaypointEngine<MAX_WAY>::pause() {
	isRunning=false;
}

template<unsigned int MAX_WAY>
void WaypointEngine<MAX_WAY>::unpause() {
	if(curWaypoint==waypoints.end())
		go();
	isRunning=true;
	for(unsigned int i=0; i<3; i++)
		curVel[i]=0;
	lastUpdateTime=get_time();
}

template<unsigned int MAX_WAY>
bool WaypointEngine<MAX_WAY>::cycle() {
	if(!isRunning)
		return false;
	
	unsigned int curtime=get_time();
	if(curWaypoint!=waypoints.end()) {
		computeCurrentPosition(curtime);
		checkNextWaypoint(curtime);
	}
	if(curWaypoint!=waypoints.end()) {
		computeIdeal(curtime);
		computeNewVelocity(curtime);
	}
	
	return true;
}

template<unsigned int MAX_WAY>
unsigned int WaypointEngine<MAX_WAY>::getBinSize() const {
	unsigned int numPrecision=9;
	unsigned int wpSize=0;
	unsigned int boilerplateSize=0;
	boilerplateSize+=strlen("#WyP\n");
	boilerplateSize+=strlen("#add_{point|arc} {ego|off|abs} x_val y_val {hold|follow} angle_val speed_val arc_val\n");
	wpSize+=strlen("max_turn_speed ")+numPrecision+1;
	wpSize+=strlen("track_path false\n");
	wpSize+=strlen("add_point ")+4+numPrecision*5+1*5+strlen("follow");
	boilerplateSize+=strlen("#END\n");
	return wpSize*waypoints.size()+boilerplateSize;
}

template<unsigned int MAX_WAY>
unsigned int WaypointEngine<MAX_WAY>::loadBuffer(const char buf[], unsigned int len) {
	unsigned int origlen=len;
	waypoints.clear();
	if(strncmp("#WyP\n",buf,5)!=0 && strncmp("#WyP\r",buf,5)!=0) {
		return 0;
	}

	float turn=turnSpeed; //init to current mode, in case file doesn't specify
	bool track=isTracking;
	char cmd[40];
	char posType[40];
	float x_val=0;
	float y_val=0;
	char angType[40];
	bool ang_val=0;
	float angle_val=0;
	float speed_val=0;
	float arc_val=0;
	unsigned int linenum=1;
	while(len<=origlen && len>0) {
		//		printf("%d %.9s\n",linenum+1,buf);
		if(buf[0]=='\r') {
			buf++; len--;
			if(buf[0]=='\n') {
				buf++; len--;
			}
			linenum++;
			continue;
		}
		if(buf[0]=='\n') {
			buf++; len--;
			linenum++;
			continue;
		}
		if(buf[0]=='#') {
			if(strncmp("#END\n",buf,5)==0 || strncmp("#END\r",buf,5)==0) {
				return origlen-len+5;
			} else if(strncmp("#END\r\n",buf,6)==0) {
				return origlen-len+6;
			} else {
				while(len>0 && *buf!='\n' && *buf!='\r') {len--;buf++;}
				if(*buf=='\n') { //in case of \r\n
					buf++;
					len--;
				}
				linenum++;
				continue;
			}
		}
		int used=-1U;
		sscanf(buf,"%40s%n",cmd,&used);
		if(!checkInc(used,buf,len,"*** ERROR Waypoint list load corrupted - ran out of room line %d\n",linenum)) return 0;
		if(strncasecmp(cmd,"add_point",9)==0 || strncasecmp(cmd,"add_arc",7)==0) {
			sscanf(buf,"%40s %g %g %40s %g %g %g%n",posType,&x_val,&y_val,angType,&angle_val,&speed_val,&arc_val,&used);
			if(!checkInc(used,buf,len,"*** ERROR Waypoint list load corrupted - bad read on add at line %d\n",linenum)) return 0;
			if(strncasecmp(angType,"hold",4)==0)
				ang_val=false;
			else if(strncasecmp(angType,"follow",6)==0)
				ang_val=true;
			else {
				printf("*** ERROR WaypointEngine: Invalid angle value type %s\n",angType);
				return 0;
			}
			if(strncasecmp(cmd,"add_point",9)==0) {
				if(strncasecmp(posType,"ego",3)==0)
					addEgocentricWaypoint(x_val,y_val,angle_val,ang_val,speed_val);
				else if(strncasecmp(posType,"off",3)==0)
					addOffsetWaypoint(x_val,y_val,angle_val,ang_val,speed_val);
				else if(strncasecmp(posType,"abs",3)==0)
					addAbsoluteWaypoint(x_val,y_val,angle_val,ang_val,speed_val);
				else {
					printf("*** ERROR WaypointEngine: Invalid position type %s\n",posType);
					return 0;
				}						
				waypoints.back().arc=arc_val;
			} else {
				if(strncasecmp(posType,"ego",3)==0)
					addEgocentricArc(x_val,y_val,angle_val,ang_val,speed_val,arc_val);
				else if(strncasecmp(posType,"off",3)==0)
					addOffsetArc(x_val,y_val,angle_val,ang_val,speed_val,arc_val);
				else if(strncasecmp(posType,"abs",3)==0)
					addAbsoluteArc(x_val,y_val,angle_val,ang_val,speed_val,arc_val);
				else {
					printf("*** ERROR WaypointEngine: Invalid position type %s\n",posType);
					return 0;
				}
			}
			waypoints.back().trackPath=track;
			waypoints.back().turnSpeed=turn;
		} else if(strncasecmp(cmd,"track_path",10)==0) {
			int track_tmp;
			sscanf(buf,"%d%n",&track_tmp,&used);
			track=track_tmp;
			if(!checkInc(used,buf,len,"*** ERROR Waypoint load corrupted - bad read on track_path line %d\n",linenum)) return 0;
		} else if(strncasecmp(cmd,"max_turn_speed",14)==0) {
			sscanf(buf,"%g%n",&turn,&used);
			if(!checkInc(used,buf,len,"*** ERROR Waypoint load corrupted - bad read on max_turn_speed line %d\n",linenum)) return 0;
		} else {
			printf("*** ERROR WaypointEngine: Invalid command %s\n",cmd);
			return 0;
		}
				
		linenum++;
	}
	std::cout << "*** WARNING WaypointEngine: load missing #END" << std::endl;
	return origlen-len;
}

template<unsigned int MAX_WAY>
unsigned int WaypointEngine<MAX_WAY>::saveBuffer(char buf[], unsigned int len) const {
	unsigned int origLen=len;
	unsigned int used;
	unsigned int cnt=0;
		
	used=snprintf(buf,len,"#WyP\n");
	if(!checkInc(used,buf,len,"*** ERROR Waypoint list save failed on header\n")) return 0;

	used=snprintf(buf,len,"#add_{point|arc} {ego|off|abs} x_val y_val {hold|follow} angle_val speed_val arc_val\n");
	if(!checkInc(used,buf,len,"*** ERROR Waypoint list save failed on header\n")) return 0;

	//set our state variables so we'll be forced to output the first set
	float turn=waypoints.front().turnSpeed-1;
	bool track=!waypoints.front().trackPath;

	for(WaypointListIter_t it=waypoints.begin(); it!=waypoints.end(); it=waypoints.next(it)) {
		if(waypoints[it].turnSpeed!=turn) {
			turn=waypoints[it].turnSpeed;
			used=snprintf(buf,len,"max_turn_speed %g\n",turn);
			if(!checkInc(used,buf,len,"*** ERROR Waypoint list save failed on waypoint %d turnSpeed\n",cnt)) return 0;
		}
		if(waypoints[it].trackPath!=track) {
			track=waypoints[it].trackPath;
			used=snprintf(buf,len,"track_path %d\n",track);
			if(!checkInc(used,buf,len,"*** ERROR Waypoint list save failed on waypoint %d\n trackPath",cnt)) return 0;
		}
		const char * posType=NULL;
		switch(waypoints[it].posType) {
		case Waypoint::POSTYPE_EGOCENTRIC:
			posType="EGO"; break;
		case Waypoint::POSTYPE_OFFSET:
			posType="OFF"; break;
		case Waypoint::POSTYPE_ABSOLUTE:
			posType="ABS"; break;
		}
		if(waypoints[it].arc!=0)
			used=snprintf(buf,len,"add_point %s %g %g %s %g %g %g\n",posType,waypoints[it].x,waypoints[it].y,(waypoints[it].angleIsRelative?"FOLLOW":"HOLD"),waypoints[it].angle,waypoints[it].speed,waypoints[it].arc);
		else //todo - store center of circle
			used=snprintf(buf,len,"add_point %s %g %g %s %g %g %g\n",posType,waypoints[it].x,waypoints[it].y,(waypoints[it].angleIsRelative?"FOLLOW":"HOLD"),waypoints[it].angle,waypoints[it].speed,waypoints[it].arc);
		if(!checkInc(used,buf,len,"*** ERROR Waypoint list save failed on waypoint %d\n",cnt)) return 0;
		cnt++;
	}

	used=snprintf(buf,len,"#END\n");
	if(!checkInc(used,buf,len,"*** ERROR Waypoint list save failed on footer\n")) return 0;

	return origLen-len;
}

template<unsigned int MAX_WAY>
void WaypointEngine<MAX_WAY>::init() {
	eps[0]=eps[1]=.01; //1 centimeter
	eps[2]=0.0175; //1 degree
	for(unsigned int i=0; i<3; i++)
		pathStartPos[i]=targetPos[i]=sourcePos[i]=curPos[i]=curVel[i]=0;
	for(unsigned int i=0; i<4; i++)
		idealPos[i]=0;
}

template<unsigned int MAX_WAY>
void WaypointEngine<MAX_WAY>::applyWaypoint(Waypoint& cur, const Waypoint& next) {
	float origx=cur.x;
	float origy=cur.y;
	switch(next.posType) {
	case Waypoint::POSTYPE_EGOCENTRIC: {
		cur.x+=next.x*cos(cur.angle)-next.y*sin(cur.angle);
		cur.y+=next.x*sin(cur.angle)+next.y*cos(cur.angle);
		break;
	}
	case Waypoint::POSTYPE_OFFSET:
		cur.x+=next.x;
		cur.y+=next.y;
		break;
	case Waypoint::POSTYPE_ABSOLUTE:
		cur.x=next.x;
		cur.y=next.y;
		break;
	}
	float dx=cur.x-origx;
	float dy=cur.y-origy;
	if(fabs(dx)<eps[0] && fabs(dy)<eps[1]) { //turn in place
		if(next.angleIsRelative)
			cur.angle+=next.angle;
		else
			cur.angle=next.angle;
	} else { //move at heading
		cur.angle=next.angle;
		if(next.angleIsRelative)
			cur.angle+=atan2(dy,dx);
	}
	cur.angle+=next.arc/2;
	cur.angle=normalizeAngle(cur.angle);
}

template<unsigned int MAX_WAY>
void WaypointEngine<MAX_WAY>::fixArc(float arc) {
	Waypoint& center=waypoints.back();
	float cdx=center.x; //center delta
	float cdy=center.y; //center delta
	if(center.posType==Waypoint::POSTYPE_ABSOLUTE) {
		//have to find location of waypoint before last one
		WaypointListIter_t start_it=waypoints.prev(waypoints.prev(waypoints.end()));
		if(start_it!=waypoints.end()) {
			Waypoint start=calcAbsoluteCoords(waypoints.prev(waypoints.prev(waypoints.end())));
			cdx-=start.x;
			cdy-=start.y;
		}
	}
	float r=sqrt(cdx*cdx+cdy*cdy); //radius of circle
	float ca=atan2(cdy,cdx); //angle to center of circle
	center.x-=r*cos(ca-arc); //now x is the endpoint
	center.y-=r*sin(ca-arc); //now y is the endpoint
	center.arc=arc;
}
	
template<unsigned int MAX_WAY>
void WaypointEngine<MAX_WAY>::computeCurrentPosition(unsigned int t) {
	float dt=(t-lastUpdateTime)/1000.f;
	float df=dt*curVel[0];
	float ds=dt*curVel[1];
	float da=dt*curVel[2];

	float avgAngle=curPos[2]+da/2;
	float ca=cos(avgAngle);
	float sa=sin(avgAngle);

	curPos[0]+=df*ca-ds*sa;
	curPos[1]+=df*sa+ds*ca;
	curPos[2]+=da;
	curPos[2]=normalizeAngle(curPos[2]);

	lastUpdateTime=t;
}

template<unsigned int MAX_WAY>
void WaypointEngine<MAX_WAY>::checkNextWaypoint(unsigned int /*t*/) {
	float rx=targetPos[0]-curPos[0];
	float ry=targetPos[1]-curPos[1];
	float ra=targetPos[2]-curPos[2];
	if(fabs(rx)<eps[0] && fabs(ry)<eps[1] && fabs(ra)<eps[2]) {
		//cout << "Check succeeds: " << rx << ' '<<ry<<' ' << ra << endl;
		setTargetWaypoint(waypoints.next(curWaypoint));
	}
}

template<unsigned int MAX_WAY>
void WaypointEngine<MAX_WAY>::computeIdeal(unsigned int t) {
	Waypoint& cur=waypoints[curWaypoint];
	if(cur.trackPath) {
		float dx=targetPos[0]-sourcePos[0];
		float dy=targetPos[1]-sourcePos[1];
		float dt=(t-waypointTime)/1000.f; //time we've been traveling towards current waypoint
		float ideal_travel=dt*cur.speed; //distance we should have covered
		float p=1; //will be set to percentage of path length to waypoint we have covered
		if(pathLength!=0) {
			p=ideal_travel/pathLength;
			if(p>1)
				p=1;
		}
		if(arcRadius==0) { //radius is "infinite" - straight line solution
			idealPos[0]=sourcePos[0]+dx*p;
			idealPos[1]=sourcePos[1]+dy*p;
			idealPos[2]=targetPos[2];
			idealPos[3]=atan2(dy,dx);
		} else {
			// have to find center of arc's circle
			float bearing=atan2(dy,dx); //bearing of target from source
			float center_bearing=bearing+(M_PI-cur.arc)/2; //bearing of center from source
			float cx=sourcePos[0]+arcRadius*cos(center_bearing); //x pos of center
			float cy=sourcePos[1]+arcRadius*sin(center_bearing); //y pos of center
			float arc_bearing=center_bearing-M_PI+cur.arc*p; //bearing from center to current ideal location
			//sout->printf("%g %g (%g,%g) %g\n",center_bearing,arcRadius,cx,cy,arc_bearing);
			idealPos[0]=cx+arcRadius*cos(arc_bearing);
			idealPos[1]=cy+arcRadius*sin(arc_bearing);
			idealPos[3]=arc_bearing+M_PI/2;
			idealPos[2]=cur.angle;
			if(cur.angleIsRelative)
				idealPos[2]+=idealPos[3];
			idealPos[2]=normalizeAngle(idealPos[2]);
			idealPos[3]=normalizeAngle(idealPos[3]);
		}
	} else {
		idealPos[0]=curPos[0];
		idealPos[1]=curPos[1];
		float rx=targetPos[0]-curPos[0];
		float ry=targetPos[1]-curPos[1];
		if(fabs(rx)<eps[0] && fabs(ry)<eps[1]) {
			idealPos[2]=targetPos[2];
		} else {
			idealPos[2]=cur.angle;
			if(cur.angleIsRelative) {
				float dx=targetPos[0]-curPos[0];
				float dy=targetPos[1]-curPos[1];
				idealPos[2]+=atan2(dy,dx);
			}
			idealPos[2]=normalizeAngle(idealPos[2]);
		}
		idealPos[3]=atan2(ry,rx);
		if(arcRadius!=0) {
			float dt=(t-waypointTime)/1000.f; //time we've been traveling towards current waypoint
			float ideal_travel=dt*cur.speed; //distance we should have covered
			float p=1; //will be set to percentage of path length to waypoint we have covered
			if(pathLength!=0) {
				p=ideal_travel/pathLength;
				if(p>1)
					p=1;
			}
			float arc=cur.arc*(1-p)/2;
			idealPos[2]=normalizeAngle(idealPos[2]-arc);
			idealPos[3]=normalizeAngle(idealPos[3]-arc);
		}
	}
}

template<unsigned int MAX_WAY>
void WaypointEngine<MAX_WAY>::computeNewVelocity(unsigned int /*t*/) {
	Waypoint& cur=waypoints[curWaypoint];

	//first we'll start with the velocity we would use if we were on path
	//determine distance remaining (only an approximation for arcing paths)
	float dx=targetPos[0]-idealPos[0];
	float dy=targetPos[1]-idealPos[1];
	float spd=sqrt(dx*dx+dy*dy)/(FrameTime*NumFrames)*1000;
	if(spd>cur.speed) {
		//we're far away - go at full speed
		curVel[0]=cur.speed*cos(idealPos[3]-curPos[2]);
		curVel[1]=cur.speed*sin(idealPos[3]-curPos[2]);
	} else {
		//we're about to overshoot... just go fast enough to get on target
		curVel[0]=spd*cos(idealPos[3]-curPos[2]);
		curVel[1]=spd*sin(idealPos[3]-curPos[2]);
	}
	if(arcRadius==0)
		curVel[2]=0;
	else
		curVel[2]=cur.speed/arcRadius;

	//sout->printf("ideal vel: %g %g %g\n",curVel[0],curVel[1],curVel[2]);
		
	// then we'll add the errors
	float ex=idealPos[0]-curPos[0];
	float ey=idealPos[1]-curPos[1];
	float ed=sqrt(ex*ex+ey*ey);
	float ehead=atan2(ey,ex)-curPos[2];
	float ea=normalizeAngle(idealPos[2]-curPos[2]);
	float easpd=ea/(FrameTime*NumFrames)*1000;
	easpd=clipRange(easpd,-cur.turnSpeed,cur.turnSpeed);
	curVel[0]+=Pcorr*ed*cos(ehead);
	curVel[1]+=Pcorr*ed*sin(ehead);
	curVel[2]+=easpd;
}

/*! @file
 * @brief Defines WaypointEngine, which provides computation and management of a desired path through a series of waypoints
 * @author ejt (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.14 $
 * $State: Exp $
 * $Date: 2007/03/05 01:19:11 $
 */

#endif
