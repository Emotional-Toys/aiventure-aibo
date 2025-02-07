//-*-c++-*-
#ifndef INCLUDED_PilotRequest_h_
#define INCLUDED_PilotRequest_h_

#include "ShapeRoot.h"

namespace DualCoding {

class MapBuilderRequest;
class LookoutTrackRequest;

//! Request to the @a Pilot for motion or navigation.

class PilotRequest {
  friend class Pilot;

 public:
  enum PilotRequestType_t {
    walk,
    waypointWalk,
    visualSearch,
    gotoShape,
    gotoTarget,
    pushTarget,
    creepToShape
  };

  //! Constructor
  PilotRequest(PilotRequestType_t _type);

  PilotRequest(const PilotRequest &req);

  PilotRequestType_t getRequestType() const { return requestType; }

  PilotRequestType_t requestType; //!< Type of pilot request
  float dx; //!< Forward walk distance in mm
  float dy; //!< Walking strafe distance in mm
  float da; //!< Walking rotation distance in radians
  MapBuilderRequest *mapBuilderRequest; //!< MapBuilderRequest to be used for visual search
  MapBuilderRequest* (*mapBuilderRequestFn)(); //!< function to return a dynamically-constructed MapBuilderRequest which Pilot will delete
  AngSignPi searchRotationAngle; //!< Angle to rotate body to continue a visual search
  bool (*exitTest)(); //!< If true, terminate search and post a completion event
  ShapeRoot targetShape; //!< Shape to walk to
  bool avoidCliffs; //!< If true, use chest IR to avoid walking off a cliff
  int cliffThreshold; //!< Maximum tolerable distance to the ground (millimeters)
  bool avoidObstacles; //!< If true, use head IR sensors to avoid obstacles
  int obstacleThreshold; //!< Minimum tolerable distance to an obstacle (millimeters)
  LookoutTrackRequest *trackRequest; //!< Lookout request for tracking objects while walking
  int localizationInterval; //!< Time in msec between localization attempts; will execute the mapBuilderRequest, then if localizeTest is true, will call particle filter
  bool (*localizationTest)(); //!< If true, local map contains good enough data to try to localize
  float localizationDisplayParticles; //!< How many particles to display (number or percentage)
  int localizationDisplayInterval; //!< Time in msec between calls to displayParticles()

  float safeDistanceAroundTarget; //!< The distance to stay away from the target while circling
  AngSignPi subtendAngle; //!< The angle in which to subtend the target while circling
  AngSignPi approachAngle; //!< The angle in which to approach the desired position around the target
  Point positionRelativeToTarget; //!< The desired position around the target, relative to the target
  AngSignPi angleToPushTarget; //!< The angle in which to push the target
  void (*buildTargetParamsFn)(bool *buildFrontLeft, bool *buildFrontRight, bool *buildBackLeft, bool *buildBackRight, bool *lookAtCentroid, int *maxRetries); //!< function to return the parameters to build the target
  MapBuilderRequest* (*buildTargetMapBuilderRequestFn)(Point point); //!< function to return a dynamically-constructed MapBuilderRequest, given a point to look at, which BuildTarget will use to build the target
  
private:
  unsigned int requestID;

  PilotRequest& operator=(const PilotRequest &req); //!< don't call this
};

} // namespace

#endif
