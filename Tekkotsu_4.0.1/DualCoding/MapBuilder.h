//-*-c++-*-
#ifndef _MapBuilder_h_
#define _MapBuilder_h_

#include <queue>

#include "Behaviors/BehaviorBase.h"
#include "Shared/newmat/newmat.h"

#include "Point.h"

#include "BlobData.h"
#include "EllipseData.h"
#include "LineData.h"
#include "SphereData.h"
#include "TargetData.h"

#include "VRmixin.h"
#include "MapBuilderRequest.h"
#include "SketchTypes.h"
#include "ShapeSpace.h"
#include "PolygonData.h"

class LookoutSketchEvent;  // note: this is NOT in the DualCoding namespace

namespace DualCoding {

class SketchSpace;

class MapBuilder : public BehaviorBase {
protected:
  SketchSpace &camSkS;
  ShapeSpace &camShS, &groundShS;
  SketchSpace &localSkS;
  ShapeSpace &localShS;
  SketchSpace &worldSkS;
  ShapeSpace &worldShS;

  const int xres, yres; //!< width and height of camera frame

  NEWMAT::ColumnVector ground_plane; //!< ground plane to which shapes are projected

  static bool retain; //!< if true, VRmixin::stopCrew will not clear MapBuilder structures

public:
  //! Control whether static structures (mapbuilder, sketchGUI sockets, etc.) are retained
  static void setRetain(bool r) { retain = r; }
  static bool isRetained() { return retain; }

  typedef unsigned int MapBuilderVerbosity_t;
  static const MapBuilderVerbosity_t MBVstart = 1<<0;
  static const MapBuilderVerbosity_t MBVevents = 1<<1;
  static const MapBuilderVerbosity_t MBVexecute = 1<<2;
  static const MapBuilderVerbosity_t MBVcomplete = 1<<3;
  static const MapBuilderVerbosity_t MBVdefineGazePoints = 1<<4;
  static const MapBuilderVerbosity_t MBVnextGazePoint = 1<<5;
  static const MapBuilderVerbosity_t MBVshapeSearch = 1<<6;
  static const MapBuilderVerbosity_t MBVshapesFound = 1<<7;
  static const MapBuilderVerbosity_t MBVgroundPlane = 1<<8;
  static const MapBuilderVerbosity_t MBVprojectionFailed = 1<<9;
  static const MapBuilderVerbosity_t MBVimportShapes = 1<<10;
  static const MapBuilderVerbosity_t MBVnotAdmissible = 1<<11;
  static const MapBuilderVerbosity_t MBVshapeMatch = 1<<12;
  static const MapBuilderVerbosity_t MBVshapesMerge = 1<<13;
  static const MapBuilderVerbosity_t MBVshouldSee = 1<<14;
  static const MapBuilderVerbosity_t MBVdeleteShape = 1<<15;
  static const MapBuilderVerbosity_t MBVsetAgent = 1<<16;
  static const MapBuilderVerbosity_t MBVbadGazePoint = 1<<17;

private:
  static MapBuilderVerbosity_t verbosity;
public:
  static void setVerbosity(MapBuilderVerbosity_t v) { verbosity = v; }

protected:
  friend class Lookout;

  Shape<AgentData> &theAgent; //!< Agent in the world frame
   //!@name Transformation matrices between local and world frames
  //@{
  NEWMAT::Matrix localToWorldMatrix, worldToLocalTranslateMatrix, worldToLocalRotateMatrix;
  //@}

  std::vector<Point> badGazePoints; //!<  gaze points for which HeadPointerMC.lookAtPoint() returned false

  std::queue<MapBuilderRequest*> requests;
  MapBuilderRequest *curReq;
  unsigned int idCounter;
  
  float maxDistSq; //!< square of current request's max distance parameter
  unsigned int pointAtID, scanID; //!< ID's for lookout requests
  Point nextGazePoint;
  //! Triggers action to execute the request at the front of the queue
  void executeRequest();
  //! calls exitTest of current request if there is one and returns the result
  bool requestExitTest();
  //! posts completion event and deletes current request, executes next request if there is one
  void requestComplete(); 

public:
  MapBuilder(); //!< Constructor
  virtual ~MapBuilder() {}   //!< Destructor
  virtual void DoStart();
  virtual void DoStop(); 
  virtual std::string getDescription() const { return "MapBuilder"; }
  void printShS(ShapeSpace&) const;
  unsigned int executeRequest(const MapBuilderRequest&, unsigned int *req_id=NULL); // execute a MapBuilder request, and optionally store the request id in a variable; the id will be returned in any case

  virtual void processEvent(const EventBase&);
  void processImage(const LookoutSketchEvent&);

  // Returns true if a ground shape should be seen in the current camera frame
  static bool isPointVisible(const Point &pt, const NEWMAT::Matrix& baseToCam, float maxDistanceSq) ;
  static bool isLineVisible(const LineData& ln, const NEWMAT::Matrix& baseToCam);
  static bool isShapeVisible(const ShapeRoot &ground_shape, const NEWMAT::Matrix& baseToCam, float maxDistanceSq);
  
  
  //! utility functions which may be used by MapBuilderRequest's exit condition and others
  //@{
  const Shape<AgentData>& getAgent() const { return theAgent; }

  // sets the agent location and heading
  void setAgent(const Point &worldLocation, const AngTwoPi worldHeading);
  
  // updates the agent location and heading
  void moveAgent(coordinate_t const local_dx, coordinate_t const local_dy, AngTwoPi dtheta);
  //@}
  
  std::vector<ShapeRoot> getShapes(const ShapeSpace& ShS, int minConf=2) const {
    const std::vector<ShapeRoot> allShapes = ShS.allShapes();
    if (&ShS == &camShS || &ShS == &groundShS || minConf <= 0) 
      return allShapes;
    std::vector<ShapeRoot> nonNoiseShapes;
    for (std::vector<ShapeRoot>::const_iterator it = allShapes.begin();
	 it != allShapes.end(); it++)
      if ((*it)->getConfidence() >= minConf)
	nonNoiseShapes.push_back(*it);
    return nonNoiseShapes;
  }

  void importLocalToWorld();

  ShapeRoot importWorldToLocal(const ShapeRoot &worldShape);
  template<class T> Shape<T> importWorldToLocal(const Shape<T> &worldShape);

protected:
  //!@name Shape extraction functions
  //@{
  void getCameraShapes(const Sketch<uchar>& camFrame);

  std::vector<Shape<LineData> > 
  getCamLines(const Sketch<uchar>&, const std::set<color_index>& objectColors, 
	      const std::set<color_index>& occluderColors) const;

  std::vector<Shape<EllipseData> > 
  getCamEllipses(const Sketch<uchar>&, const std::set<color_index>& objectColors, 
		 const std::set<color_index>& occluderColors) const;

  std::vector<Shape<PolygonData> > 
  getCamPolygons(const Sketch<uchar>&, const std::set<color_index>& objectColors, 
		 const std::set<color_index>& occluderColors) const;

  std::vector<Shape<LineData> >  
  getCamWalls(const Sketch<uchar>&, unsigned int) const;

  std::vector<Shape<SphereData> >  
  getCamSpheres(const Sketch<uchar>&, const std::set<color_index>& objectColors, 
		const std::set<color_index>& occluderColors) const;

  void getCamBlobs(const Sketch<uchar>& camFrame,
		   const std::set<color_index>& colors,
		   const std::map<color_index,int>& minBlobAreas,
		   const std::map<color_index, BlobData::BlobOrientation_t>& blobOrientations,
		   const std::map<color_index,coordinate_t>& assumedBlobHeights);
  void getCamBlobs();
  
  std::vector<Shape<TargetData> > 
  getCamTargets(const Sketch<uchar> &camFrame, const std::set<color_index>& objectColors,
    const std::set<color_index>& occluderColors) const;
  //@}

  // matching shapes between two spaces.
  static void matchSrcToDst(ShapeSpace &src, ShapeSpace &dst, std::set<color_index> polygonEdgeColors=std::set<color_index>(),
			    bool mergeSrc=true, bool mergeDst=true);

  //!@name Functions to make requests to the Lookout
  //@{
  void storeImage() { storeImage(Point(),false); }
  void storeImage(const Point& pt, bool havepoint=true);
  void grabCameraImageAndGo();
  void scanForGazePts();
  //@}

  //! define gazePts either virtually or by scan
  void defineGazePts();
  
  void extendLocal(const NEWMAT::Matrix& baseToCam);
  void extendWorld(const NEWMAT::Matrix& baseToCam);

  //! decrement confidence of shapes which should have been seen according to the baseToCam matrix
  void removeNoise(ShapeSpace&, const NEWMAT::Matrix& baseToCam);
  //! erase gaze points which should have been seen according to the baseToCam matrix
  void removeGazePts(std::vector<Point>&, const NEWMAT::Matrix& baseToCam);
  
  //! Returns true if it has set up a valid next gaze point in nextGazePoint
  bool determineNextGazePoint();
  //! Returns true if there is a shape which needs be looked at again and is reachable; sets it up as nextGazePoint
  bool determineNextGazePoint(const std::vector<ShapeRoot>&);
  // Returns true if an element of gazePts can be looked at; sets it up as nextGazePoint
  bool determineNextGazePoint(std::vector<Point> &gazePts);
  //! Starts robot moving to the next gaze point
  void moveToNextGazePoint(const bool manualOverride=false);
  void doNextSearch();
  void doNextSearch2();

  // operations in ground shape space 
  bool isBadGazePoint(const Point&) const ;
  void projectToGround(const NEWMAT::Matrix& camToBase);
  void filterGroundShapes(const NEWMAT::Matrix& baseToCam);

  // calculates ground plane based on ground plane assumption type
  void calculateGroundPlane();

private:
  MapBuilder(const MapBuilder&); //!< never call this
  MapBuilder& operator=(const MapBuilder&);  //!< never call this
};

template<class T> Shape<T> MapBuilder::importWorldToLocal(const Shape<T> &worldShape) {
  ShapeRoot temp(localShS.importShape(worldShape));
  Shape<T> localShape(ShapeRootType(temp,T));
  localShape->applyTransform(worldToLocalTranslateMatrix);
  localShape->applyTransform(worldToLocalRotateMatrix);
  return localShape;
}

//! Utility function for deleting queues of pointers to Lookout or MapBuilder requests
template<typename T> void deleteAll(std::queue<T*> &q) {
  while ( ! q.empty() ) {
    delete q.front();
    q.pop();
  }
}

} // namespace

#endif

