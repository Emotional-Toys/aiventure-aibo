//-*-c++-*-
#ifndef _VRmixin_h_
#define _VRmixin_h_

#include <string>
#include <iostream>

#include "Behaviors/BehaviorBase.h"
#include "Shared/debuget.h" // needed for ASSERT macros
#include "Vision/RawCameraGenerator.h"
#include "Vision/SegmentedColorGenerator.h"
#include "Vision/cmv_types.h" // needed for 'run' type?
#include "Wireless/Wireless.h"
#include "Shared/Config.h"
#include "Shared/RobotInfo.h"
#include "Shared/get_time.h" // needed for time stamp, for serialization 

#include "ShapeAgent.h"
#include "BlobData.h"
#include "PFShapeSLAM.h"
#include "SketchRoot.h"

namespace DualCoding {

class Lookout;
class Pilot;
class SketchDataRoot;
class SketchSpace;
class MapBuilder;

typedef unsigned char cmap_t;

//! Mix-in for the BehaviorBase or StateNode class to give access to VisualRoutinesBehavior variables.
class VRmixin {
protected:
  static unsigned int instanceCount; //!< count of NewVRmixin instances -- when this hits zero, free sketch spaces
  static unsigned int crewCount; //!< count of "crew" (pilot, lookout, map builders) users -- stop these when no one is using them
	
public:
  //! returns reference to the global space instances, call there from global constructors instead of accessing #camSkS, which might not be initialized yet
  static SketchSpace& getCamSkS();
  static SketchSpace& getLocalSkS();
  static SketchSpace& getWorldSkS();
  static ShapeSpace& getGroundShS();

  static SketchSpace& camSkS;      //!< The camera sketch space
  static ShapeSpace& camShS;       //!< The camera shape space
  
  static ShapeSpace& groundShS;    //!< The ground shape space of MapBuilder (MapBuilder::groundShS)
  
  static SketchSpace& localSkS;    //!< The localmap sketch space (LocalMapBuilder::localSkS)
  static ShapeSpace& localShS;     //!< The localmap shape space (LocalMapBuilder::localShS)
  
  static SketchSpace& worldSkS;    //!< The worldmap sketch space (WorldMapBuilder::localSkS)
  static ShapeSpace& worldShS;     //!< The worldmap sketch space (WorldMapBuilder::localShS)
  static Shape<AgentData> theAgent; //!< The robot (usually lives in worldShS)
  
  //! returns reference to the global WorldMapBuilder instance, call this from global constructors instead of accessing #worldSkS/#localSkS/#groundShS/etc., which might not be initialized yet
  static MapBuilder& getMapBuilder();
  static MapBuilder& mapBuilder;   //!< the global world mapbuilder instance

  static Lookout& getLookout();
  static Lookout &lookout;         //!< the global Lookout instance
  
  static Pilot& getPilot();
  static Pilot& pilot;              //!< the global Pilot instance

  static PFShapeLocalization *particleFilter;   //!< the global particle filter instance

private:
  static Socket *camDialogSock;    //!< socket to talk with cam-space sketch viewer
  static Socket *camRleSock;       //!< socket for transmitting RLE images to cam-space sketch viewer
  static Socket *localDialogSock;  //!< socket to talk with local-space sketch viewer
  static Socket *localRleSock;     //!< socket for transmitting RLE images to local-space sketch viewer
  static Socket *worldDialogSock;  //!< socket to talk with world-space sketch viewer
  static Socket *worldRleSock;     //!< socket for transmitting RLE images to world-space sketch viewer
  
public:
  //! Constructor
  VRmixin();

  //! Destructor
  virtual ~VRmixin(void);
  
  static void startCrew(); //!< starts map builders, pilot, and lookout
  static void stopCrew(); //!< stops map builders, pilot, and lookout

  // serialize the specified Sketch; should use RLE encoding later 
  static bool rleEncodeSketch(const SketchDataRoot& image);
  
  //! Import the current color-segmented camera image as a Sketch<uchar>
  static Sketch<uchar> sketchFromSeg();
  
  //! Import channel n image as a Sketch<uchar>
  static Sketch<uchar> sketchFromChannel(const RawCameraGenerator::channel_id_t chan);
  
  //! Import the current y-channel camera image as a Sketch<uchar>
  static Sketch<uchar> sketchFromRawY();
  
  //! Import blobs from the current region list as a vector of Shape<BlobData>
  static std::vector<Shape<BlobData> >
  getBlobsFromRegionGenerator(const color_index color, int minarea=25,
			      const BlobData::BlobOrientation_t orient=BlobData::groundplane,
			      const coordinate_t height=0,
			      const int maxblobs=50);
  
  //! processes a single line of input for a Sketch request
  static void processSketchRequest(const std::string &line, 
				   SketchSpace &sketches, 
				   ShapeSpace &shapes);
  
  //! project shapes from cam space to ground space
#ifdef TGT_HAS_CAMERA
  static void projectToGround(const NEWMAT::Matrix& camToBase = kine->jointToBase(CameraFrameOffset),
			      const NEWMAT::ColumnVector& ground_plane = kine->calculateGroundPlane());
#else
	// no camera, require they provide the camToBase parameter...
  static void projectToGround(const NEWMAT::Matrix& camToBase,
			      const NEWMAT::ColumnVector& ground_plane = kine->calculateGroundPlane());
#endif

private:
  //! used so static member functions can access non-static members
  static VRmixin* theOne;
  
  // dummy functions to satisfy the compiler
  VRmixin (const VRmixin&);	 //!< never call this
  VRmixin& operator=(const VRmixin&); //!< never call this
  
  //! Called whenever data is received on camDialogSocket
  static int camDialogSockCallback(char *buf, int bytes);
  
  //! Called whenever data is received on localDialogSocket
  static int localDialogSockCallback(char *buf, int bytes);
  
  //! Called whenever data is received on worldDialogSocket
  static int worldDialogSockCallback(char *buf, int bytes);
  
  static void dialogCallback(char* buf, int bytes, std::string& incomplete,
			     SketchSpace &SkS, ShapeSpace &ShS);
};
  
} // namespace

#endif
