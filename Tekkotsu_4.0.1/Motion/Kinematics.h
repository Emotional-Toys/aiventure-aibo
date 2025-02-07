//-*-c++-*-
#ifndef INCLUDED_Kinematics_h_
#define INCLUDED_Kinematics_h_

#include "Shared/RobotInfo.h"
#include "Wireless/Socket.h"
#include "Shared/newmat/newmat.h"
#include <string>
#include <vector>
#include <ext/hash_map>

namespace ROBOOP {
	class Robot;
	class Config;
}

//! Provides access to the mathematical functionality of the ROBOOP package using Tekkotsu data structures
/*! 
 *  You should read the <a
 *  href="http://www.tekkotsu.org/Kinematics.html">Kinematics
 *  tutorial</a> to get a general understanding of the math involved
 *  and diagrams for usage with supported robots.
 *
 *  This class involves all aspects of the forward kinematics:
 *  calculations concerning locations and orientations in space given
 *  a known set of joint configurations.  There is a global
 *  instantiation of Kinematics named ::kine, which can be used to
 *  perform these calculations regarding the joint positions currently
 *  in ::state.
 *
 *  To perform kinematics on a hypothetical set of joint values,
 *  use PostureEngine or one of its subclasses.  PostureEngine also
 *  adds inverse kinematic functions, which will allow you to
 *  determine joint angles in order to reach a given point.
 *  
 *  The underlying ROBOOP library does not currently handle branching
 *  kinematic chains - in other words, each limb of the robot is a
 *  separate ROBOOP::Robot as far as ROBOOP is concerned.  This
 *  Kinematics class interfaces the Tekkotsu array index approach to
 *  referencing joints with ROBOOP's chain based hierarchy.
 *  
 *  Thus, wherever a reference frame index is requested, you can
 *  simply supply one of the output indexes in the usual manner:
 *  @code kine->jointToBase(HeadOffset+TiltOffset); @endcode
 *
 *  However, there are also a number of points on the body which are
 *  not joints, but should have their own reference frames, such as
 *  the base frame, or the camera.  These frames have their own
 *  indices, listed in the robot info file for the model in question
 *  (such as ERS7Info.h), with names ending in @c FrameOffset.
 *  @code kine->jointToBase(CameraFrameOffset); @endcode
 *  Note that for these non-joint-associated reference frames, the
 *  link and joint frames are always identical, so you can use either
 *  version of the corresponding functions.
 *
 *  Since newmat matrix library is used by ROBOOP, you will need to
 *  pass and receive information in newmat matrices.  Kinematics class
 *  provides static #pack and #unpack functions which can convert
 *  individual x,y,z variables into a NEWMAT::ColumnVector, and vice
 *  versa.  However, for readability of your code and long-term ease
 *  of use, we recommend embracing the newmat data structures directly
 *  when appropriate.
 *  @code
 *  // Find the ray from the camera to whatever the near-field IR is hitting:
 *  NEWMAT::Matrix T = kine->jointToJoint(NearIRFrameOffset,CameraFrameOffset);
 *  NEWMAT::ColumnVector camera_ray = T*Kinematics::pack(0,0,state->sensors[NearIRDistOffset]);
 *  float x,y; // x and y will be in the range -1 to 1 for resolution layer independence
 *  config->vision.computePixel(camera_ray(1),camera_ray(2),camera_ray(3),x,y);
 *  @endcode
 *  
 *  Finally, for each model we have created a database of "interest points" --
 *  locations of notable interest on the body of the robot.  These may be of
 *  use to people attempting to use the limbs to manipulate objects.
 *  To access these interest points, call either #getLinkInterestPoint
 *  or #getJointInterestPoint with the name of the interest point, obtained
 *  from the <a href="http://www.tekkotsu.org/Kinematics.html">diagrams</a>.
 *
 *  Note that you can pass a comma separated list of interest point names
 *  and the result will be the midpoint of those interest points:
 *  @code kine->getLinkInterestPoint(BaseFrameOffset,"LowerInnerFrontLFrShin,LowerOuterFrontLFrShin"); @endcode
 *  
 *  @see PostureEngine for inverse kinematics
 *
 *
 *
 *  <a name="config_file_format"></a><h2>Configuration File Format</h2>
 *
 *  The file is actually read by ROBOOP::Config, and thus the syntax
 *  of the file is defined by that class.  However, Tekkotsu will look
 *  for some additional sections beyond what is expected by ROBOOP.
 *
 *  In any give link section, a <tt>tekkotsu_output</tt> field may
 *  appear, which specifies the index (aka offset) of the
 *  corresponding joint in Tekkotsu, as defined by the model's Info.h
 *  file (e.g. ERS7Info.h).  Alternatively, <tt>tekkotsu_frame</tt>
 *  may be specified, which should specify the offset of an abstract
 *  reference frame, which does not correspond to any joint.
 *  Typically this is used for things such as the camera, or
 *  un-actuated joints, such as the spring-loaded ankles.
 *
 *  Additionally, Kinematics will look for an custom
 *  <tt>InterestPoints</tt> section, which should contain a
 *  <tt>Length</tt> field for specifying the number of interest
 *  points.  Kinematics will then attempt to read
 *  <tt>%InterestPoint</tt><i>N</i> for 1 through <i>Length</i>.
 * 
 *  Each <tt>%InterestPoint</tt><i>N</i> section should contain:
 *  - <tt>name</tt> - (string) name which will be passed to get*InterestPoint() to retrieve this IP
 *  - <tt>chain</tt> - (string) name of the chain the IP is in, must match one of the kinematic chains loaded from the file
 *  - <tt>link</tt> - (unsigned int) the index of the link the IP is connected to
 *  - <tt>x</tt> - (float) the x location of the point, in link-relative coordinates
 *  - <tt>y</tt> - (float) the y location of the point, in link-relative coordinates
 *  - <tt>z</tt> - (float) the z location of the point, in link-relative coordinates
 */
class Kinematics {
public:
	//!Constructor, pass the full path to the kinematics configuration file
	Kinematics()
#ifdef THREADSAFE_KINEMATICS
	: chains()
#endif
	{
		init();
	}
	//!Copy constructor, everything is either update-before-use or static, copy is normal init
	Kinematics(const Kinematics&)
#ifdef THREADSAFE_KINEMATICS
	: chains()
#endif
	{
		init();
	}
	//!Assignment operator, everything is either update-before-use or static, assignment is no-op
	Kinematics& operator=(const Kinematics&) { return *this; }

	//!Destructor
	virtual ~Kinematics();



	//! Returns a matrix for transforming from link frame @a j to base frame
	/*! @param[in]  link the output offset, see class notes for values */
	NEWMAT::ReturnMatrix linkToBase(unsigned int link);

	//! Returns a matrix for transforming from joint frame @a j to base frame
	/*! @param[in]  joint the output offset, see class notes for values */
	NEWMAT::ReturnMatrix jointToBase(unsigned int joint);

	//! Returns a matrix for transforming from the base frame to link @a j frame
	/*! @param[in]  link the output offset, see class notes for values */
	NEWMAT::ReturnMatrix baseToLink(unsigned int link);

	//! Returns a matrix for transforming from the base frame to joint @a j frame
	/*! @param[in]  joint the output offset, see class notes for values */
	NEWMAT::ReturnMatrix baseToJoint(unsigned int joint);

	//! Returns a matrix for transforming from link @a ij to link @a oj
	/*! @param[in]  iL the output offset to convert from, see class notes for values
	 *  @param[in]  oL the output offset to convert to, see class notes for values */
	NEWMAT::ReturnMatrix linkToLink(unsigned int iL, unsigned int oL);

	//! Returns a matrix for transforming from link frame @a ij to joint frame @a oj
	/*! @param[in]  iL the output offset to convert from, see class notes for values
	 *  @param[in]  oJ the output offset to convert to, see class notes for values */
	NEWMAT::ReturnMatrix linkToJoint(unsigned int iL, unsigned int oJ);

	//! Returns a matrix for transforming from joint frame @a ij to link frame @a oj
	/*! @param[in]  iJ the output offset to convert from, see class notes for values
	 *  @param[in]  oL the output offset to convert to, see class notes for values */
	NEWMAT::ReturnMatrix jointToLink(unsigned int iJ, unsigned int oL);

	//! Returns a matrix for transforming from joint @a ij to joint @a oj
	/*! @param[in]  iJ the output offset to convert from, see class notes for values
	 *  @param[in]  oJ the output offset to convert to, see class notes for values */
	NEWMAT::ReturnMatrix jointToJoint(unsigned int iJ, unsigned int oJ);



	//! Returns the location of a named point and the link it is attached to
	/*! @param[in]  name   the name of the interest point; varies by model, <a href="http://www.tekkotsu.org/Kinematics.html">see the diagrams</a> for your model.
	 *  @param[out] link   on exit, offset of the link, or -1U if not found
	 *  @param[out] ip     on exit, a homogeneous column vector of the requested point, relative to the link frame returned in @a j
	 *
	 *  If @a name is not found, j will be -1 and ip will be all 0's. */
	void getInterestPoint(const std::string& name, unsigned int& link, NEWMAT::Matrix& ip);

	//! Returns the location of a named point, relative to any desired joint reference frame
	/*! @param[in]  joint  the desired joint reference frame to give results in
	 *  @param[in]  name   the name of the interest point; varies by model, <a href="http://www.tekkotsu.org/Kinematics.html">see the diagrams</a> for your model.
	 *
	 *  You can pass a comma separated list of interest point names and the result will be the midpoint of those IPs */
	NEWMAT::ReturnMatrix getJointInterestPoint(unsigned int joint, const std::string& name);

	//! Returns the location of a named point, relative to any desired reference frame
	/*! @param[in]  link   the desired link reference frame to give results in
	 *  @param[in]  name   the name of the interest point; varies by model, <a href="http://www.tekkotsu.org/Kinematics.html">see the diagrams</a> for your model.
	 *
	 *  You can pass a comma separated list of interest point names and the result will be the midpoint of those IPs */
	NEWMAT::ReturnMatrix getLinkInterestPoint(unsigned int link, const std::string& name) {
		NEWMAT::ColumnVector p=jointToLink(link,link)*getJointInterestPoint(link,name);
		p.Release(); return p;
	}



#ifdef TGT_HAS_LEGS
	//! Calculate the leg heights along a given "down" vector (0 is level with base frame)
	/*! This can be based on either the gravity vector from accelerometer readings,
	 *  or if that may be unreliable due to being in motion, you could do some basic
	 *  balance modeling and pass a predicted vector.  This uses the interest point database
	 *  to find the lowest interest point for each leg
	 *  @note on Aibo platforms, if packing accelerometer readings, don't forget to negate the "left" accelerometer! */
	void calcLegHeights(const NEWMAT::ColumnVector& down, float heights[NumLegs]);
	
	//! Find the leg which is in least contact with ground
	/*! @see calcLegHeights()
	 *  @note on Aibo platforms, if packing accelerometer readings, don't forget to negate the "left" accelerometer!
	 *  @return index of leg which is highest in reference to gravity vector */
	LegOrder_t findUnusedLeg(const NEWMAT::ColumnVector& down);
#endif

	//! Find the ground plane by fitting a plane to the lowest 3 interest points
	/*! This function merely calls the other version of calculateGroundPlane with the current
	 *  gravity vector as the "down" vector.
	 *  @return vector of the form @f$p_1x + p_2y + p_3z = p_4@f$, relative to the base frame */
	NEWMAT::ReturnMatrix calculateGroundPlane();

	//! Find the ground plane by fitting a plane to the lowest 3 interest points
	/*! @note on Aibo platforms, if packing accelerometer readings, don't forget to negate the "left" accelerometer!
	 *  @return vector of the form @f$p_1x + p_2y + p_3z = p_4@f$, relative to the base frame */
	NEWMAT::ReturnMatrix calculateGroundPlane(const NEWMAT::ColumnVector& down);

	//! Find the point of intersection between a ray and a plane
	/*! @param j is the link number the ray is relative to
	 *  @param r_j is the line through the origin, in homogeneous coordinates
	 *  @param b is the link number the plane is relative to (probably BaseFrameOffset)
	 *  @param p_b represents the plane to be intersected
	 *  @param f is the link number the results should be relative to
	 *
	 *  @a p_b should be of the form @f$p_1x + p_2y + p_3z = p_4@f$
	 *  @return homogeneous coordinate of intersection (may be infinity) */
	NEWMAT::ReturnMatrix projectToPlane(unsigned int j, const NEWMAT::ColumnVector& r_j,
	                                    unsigned int b, const NEWMAT::ColumnVector& p_b,
	                                    unsigned int f);


	//! A simple utility function, converts x,y,z,h to a NEWMAT::ColumnVector
	/*! @param[in]  x the value for the first row
	 *  @param[in]  y the value for the second row
	 *  @param[in]  z the value for the third row
	 *  @param[in]  h the value for the fourth row (defaults to 1 if not specified)
	 *  @return @f$ \left[\begin{array}{c} x\\y\\z\\h\\ \end{array}\right] @f$ */
	static NEWMAT::ReturnMatrix pack(float x, float y, float z, float h=1) {
		NEWMAT::ColumnVector m(4);
		m(1)=x; m(2)=y; m(3)=z; m(4)=h;
		m.Release(); return m;
	}
	//! A simple utility function, pulls the first 3 rows of the first column, divides each by the fourth row, and stores into ox, oy, and oz
	/*! @param[in]  m  the matrix to unpack (only uses first column)
	 *  @param[out] ox set to the first row of the first column of @a m, divided by fourth row
	 *  @param[out] oy set to the second row of the first column of @a m, divided by fourth row
	 *  @param[out] oz set to the third row of the first column of @a m, divided by fourth row */
	static void unpack(NEWMAT::Matrix m, float& ox, float& oy, float& oz) {
		ox=m(1,1)/m(4,1); oy=m(2,1)/m(4,1); oz=m(3,1)/m(4,1);
	}
	//! A simple utility function, pulls the first 4 rows of the first column, stores into ox, oy, oz, oh
	/*! @param[in]  m  the matrix to unpack (only uses first column)
	 *  @param[out] ox set to the first row of the first column of @a m
	 *  @param[out] oy set to the second row of the first column of @a m
	 *  @param[out] oz set to the third row of the first column of @a m
	 *  @param[out] oh set to the fourth row of the first column of @a m */
	static void unpack(NEWMAT::Matrix m, float& ox, float& oy, float& oz, float& oh) {
		ox=m(1,1); oy=m(2,1); oz=m(3,1); oh=m(4,1);
	}
	
	//! returns the global ROBOOP::Config object which Kinematics classes initialize themselves from (#roconfig)
	static ROBOOP::Config * getConfig() { return roconfig; }
	
protected:
	//! Called by constructors to do basic setup - first call will read Config::motion_config::kinematics from disk, future initializes reuse static roconfig
	void init();
	
	//! initializes static variables -- only call if not #staticsInited
	static void initStatics();
	
	//! checks that statics have been initialized, and calls initStatics if they are missing
	static void checkStatics() { if(!staticsInited) initStatics(); }
	
	//! called by init to allocate/initialize each of #chains
	static ROBOOP::Robot* newChain(unsigned int chainIdx);
	
	//! Returns the location of a named point, relative to the link it is attached to
	/*! @param[in]  name   the name of the interest point; varies by model, <a href="http://www.tekkotsu.org/Kinematics.html">see the diagrams</a> for your model.
	 *  @param[out] c      on exit, chain index the IP is on
	 *  @param[out] l      on exit, link index the IP is on
	 *  @param[out] ip     on exit, a homogeneous column vector of the requested point
	 *
	 *  If @a name is not found, @a c and @a l will be -1 and @a ip will be all 0's. 
	 *  This internal version of the function allows us to use @a c and @a l, ourselves,
	 *  but users will probably want to use the getInterestPoint(name,j,ip) version */
	void getInterestPoint(const std::string& name, unsigned int& c, unsigned int& l, NEWMAT::Matrix& ip);
	
	//! Called at the beginning of each function which accesses ROBOOP computations - should make sure the ROBOOP structures are up to date with Tekkotsu structures
	/*! This class will pull current values from WorldState, but it is expected
	 *  that subclasses (i.e. PostureEngine) will want to provide their own joint values.
	 *  Updates from link 1 through link @a l.
	 *  @param[in] c the chain to update
	 *  @param[in] l the last link to update (later links in the chain are left untouched) */
	virtual void update(unsigned int c, unsigned int l);

	//! converts a Tekkotsu output index to a chain and link
	/*! @param[in]  tkout the tekkotsu index to lookup
	 *  @param[out] c set to the chain index that @a tkout is in
	 *  @param[out] l set to the link in @a c corresponding to @a tkout */
	bool lookup(unsigned int tkout, unsigned int& c, unsigned int& l) {
		if(tkout>=NumReferenceFrames) {
			serr->printf("ERROR Kinematics: illegal link %d\n",l);
			return false;
		}
		if(jointMaps[tkout].chain>=chains.size()) {
			serr->printf("ERROR Kinematics: no chain available for link %d\n",l);
			return false;
		}
		c=jointMaps[tkout].chain;
		l=jointMaps[tkout].link;
		return true;
	}

	//! initially false, set to true after first Kinematics is initialized
	static bool staticsInited;

#ifdef THREADSAFE_KINEMATICS
	//! A separate ROBOOP::Robot instantiation for each chain since ROBOOP package doesn't support branching chains (which would be rather difficult to implement well)
	std::vector<ROBOOP::Robot*> chains;
#else
	//! A separate ROBOOP::Robot instantiation for each chain since ROBOOP package doesn't support branching chains (which would be rather difficult to implement well)
	/*! static allocation solves problems with shared memory regions, but becomes thread-UNsafe... */
	static std::vector<ROBOOP::Robot*> chains;
#endif

	//! holds mapping for each chain's links back to the tekkotsu outputs and reference frames they represent
	static std::vector< std::vector<unsigned int> > chainMaps;
	
	//! Allows mapping from tekkotsu output index to chain and link indicies
	struct JointMap {
		JointMap() : chain(-1U), link(-1U) {} //!< constructor
		JointMap(unsigned int c, unsigned int l) : chain(c), link(l) {} //!< constructor
		unsigned int chain; //!< the chain index
		unsigned int link; //!< the link index
	};
	//! holds the position and attached link of a given interest point
	struct InterestPoint {
		InterestPoint() : x(), y(), z(), chain(), link() {} //!< constructor
		InterestPoint(float x_, float y_, float z_, unsigned int chain_, unsigned int link_)
			: x(x_), y(y_), z(z_), chain(chain_), link(link_) {} //!< constructor
		float x; //!< x value
		float y; //!< y value
		float z; //!< z value
		unsigned int chain; //!< chain containing @a link
		int link; //!< link in @a chain
	};

	//! holds mapping from tekkotsu output index to chain and link indicies
	static JointMap jointMaps[NumReferenceFrames];
	
	//! cache of the configuration of the robot for rapid initialization (so we don't have to re-read from disk)
	static ROBOOP::Config * roconfig;
	//! allows us to use the STL hash_map with strings
	struct hashstring {
		//! hashes a string by multiplying current total by 5, add new character
		/*! not my idea, this is what the STL library does for char*, i've just reimplemented it for strings */
		size_t operator()(const std::string& s) const {
			unsigned long h=0;
			for(std::string::size_type x=s.size(); x!=0; x--)
				h=h*5+s[x];
			return (size_t)h;
		}
	};
	//! we'll be using the hash_map to store named interest points
	typedef __gnu_cxx::hash_map<const std::string,InterestPoint,hashstring> InterestPointMap;
	//! these interest points are shared by all Kinematics classes (i.e. all PostureEngines)
	/*! this is to reduce initialization time, but does mean one robot can't do
	 *  kinematic calculations regarding a different model robot...  */
	static InterestPointMap * ips;
};

//! a global instance of Kinematics, joint values reference those of WorldState so users can easily query the current spatial locations of joints
extern Kinematics * kine;

/*! @file
 * @brief Describes Kinematics, which provides access to the mathematical functionality of the roboop package using Tekkotsu data structures
 * @author ejt (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.38 $
 * $State: Exp $
 * $Date: 2007/11/11 23:57:23 $
 */

#endif
