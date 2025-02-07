//-*-c++-*-
#ifndef INCLUDED_MapBuilderRequest_h_
#define INCLUDED_MapBuilderRequest_h_

#include <queue>

#include "ShapeTypes.h"

namespace DualCoding {

class LookoutSearchRequest;

class MapBuilderRequest {
  friend class MapBuilder;

public:
  enum MapBuilderRequestType_t { 
    cameraMap, 
    groundMap,
    localMap, 
    worldMap
  };

  MapBuilderRequestType_t requestType;  //!< Type of map to build
  std::map<ShapeType_t, std::set<color_index> > objectColors;		//!< For each object type, a set of object color indices
  std::map<ShapeType_t, std::set<color_index> > occluderColors;		//!< For each object type, a set of occluder color indices
  std::map<color_index, int> minBlobAreas;	//!< Minimum acceptable areas for blobs of specified colors, e.g., req.minBlobAreas[pink_index]=50
  std::map<color_index, BlobData::BlobOrientation_t> blobOrientations; //!< Default orientation for blobs of specified colors
  std::map<color_index, coordinate_t> assumedBlobHeights; //!< Fixed heights for pillar or poster  blobs of specified colors
  unsigned int floorColor;
  unsigned int motionSettleTime;  //!< Time in msec to wait before taking measurements or throwing completion event after head reaches gazePt.
  int numSamples; //!< Number of camera images to combine, for noise reduction
  int sampleInterval; //!< Interval in msec between successive samples
  float maxDist; //!< Ignore objects farther than this distance
  bool clearShapes; //!< If true, clear the shape space at start of request
  bool pursueShapes; //!< If true, generate new gaze points as shapes are recognized
  bool manualHeadMotion; //!< If true, waits for !msg MoveHead before moving to next gaze point (for debugging)
  bool rawY; //!< If true, leave an intensity (Y-channel) image in camera space for debugging
  bool removePts; //!< If true, remove pending gaze points if they're visible in current image
  bool doScan; //!< If true, do a continuous scan of the area to find interest points to be examined
  AngPi dTheta; //!< Angular step for scanning
  ShapeRoot searchArea; //!< The area to search, in egocentric coords
  std::queue<LookoutSearchRequest*> worldTargets; //!< Queue of search requests for world targets
  void (*userCamProcessing)(); //!< User routine to call after cam space processing
  void (*userGroundProcessing)(); //!< User routine to call after ground space processing
  void (*userLocalProcessing)(); //!< User routine to call after local space processing
  void (*userWorldProcessing)(); //!< User routine to call after world space processing
  bool (*exitTest)(); //!< If true, terminate map building and post completion event
  enum GroundPlaneAssumption_t { onStand, onLegs, custom } groundPlaneAssumption;
  NEWMAT::ColumnVector customGroundPlane; //!< User-supplied ground plane
private:
  std::vector<Point> gazePts;
  std::vector<NEWMAT::Matrix> baseToCamMats;
  unsigned int requestID; //!< Set by mapbuilder when the request is added to its request queue

public:
  //! Constructor
  MapBuilderRequest(MapBuilderRequestType_t reqtype=cameraMap)
    : requestType(reqtype), objectColors(), occluderColors(), 
      minBlobAreas(), blobOrientations(), assumedBlobHeights(), floorColor(0), 
      motionSettleTime(1000), numSamples(1), sampleInterval(0), maxDist(1e10), 
      clearShapes(true), pursueShapes(false), manualHeadMotion(false), rawY(true), removePts(true), 
      doScan(false), dTheta(M_PI/18),
      searchArea(), worldTargets(),
      userCamProcessing(NULL), userGroundProcessing(NULL), 
      userLocalProcessing(NULL), userWorldProcessing(NULL),
      exitTest(NULL),
      groundPlaneAssumption(onLegs), customGroundPlane(4),
      gazePts(), baseToCamMats(),
      requestID(0)
  {}

  //! Copy constructor
  MapBuilderRequest(const MapBuilderRequest &req)
    : requestType(req.requestType),
      objectColors(req.objectColors), occluderColors(req.occluderColors), 
      minBlobAreas(req.minBlobAreas), blobOrientations(req.blobOrientations), assumedBlobHeights(req.assumedBlobHeights),
      floorColor(req.floorColor), motionSettleTime(req.motionSettleTime), 
      numSamples(req.numSamples), sampleInterval(req.sampleInterval),
      maxDist(req.maxDist), clearShapes(req.clearShapes), pursueShapes(req.pursueShapes),
      manualHeadMotion(req.manualHeadMotion), rawY(req.rawY), removePts(req.removePts), 
      doScan(req.doScan), dTheta(req.dTheta),
      searchArea(req.searchArea), worldTargets(req.worldTargets),
      userCamProcessing(req.userCamProcessing), 
      userGroundProcessing(req.userGroundProcessing), 
      userLocalProcessing(req.userLocalProcessing), 
      userWorldProcessing(req.userWorldProcessing),
      exitTest(req.exitTest),
      groundPlaneAssumption(req.groundPlaneAssumption),
      customGroundPlane(req.customGroundPlane),
      gazePts(req.gazePts), baseToCamMats(req.baseToCamMats),
      requestID(req.requestID)
      {}

  virtual ~MapBuilderRequest() {} //!< Destructor

  MapBuilderRequestType_t getRequestType() const { return requestType; }

  void setCustomGroundPlane(float const angle, float const height) {
    float const s = std::sin(-angle);
    float const c = std::cos(-angle);
    customGroundPlane = Kinematics::pack(s, 0, c, -height * c);
    groundPlaneAssumption = custom;
  }

  //! Returns true if the request can complete immediately (no head motion required and numSamples <= 1)
  bool immediateRequest() const {
    return !searchArea.isValid() && worldTargets.size() == 0 && !pursueShapes && !doScan && numSamples <= 1;
  }

private:
  MapBuilderRequest& operator=(const MapBuilderRequest& req);

};

} // namespace

#endif
