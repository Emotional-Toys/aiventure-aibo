#include "PilotRequest.h"

#include "Pilot.h"

namespace DualCoding {

PilotRequest::PilotRequest(PilotRequestType_t _type) :
  requestType(_type), 
  dx(0), dy(0), da(0),
  mapBuilderRequest(NULL), mapBuilderRequestFn(NULL),searchRotationAngle(0), exitTest(NULL),
  targetShape(),
  avoidCliffs(false), cliffThreshold(200), avoidObstacles(false), obstacleThreshold(100),
  trackRequest(NULL),
  localizationInterval(0), localizationTest(NULL), localizationDisplayParticles(100), localizationDisplayInterval(0),
  safeDistanceAroundTarget(300), subtendAngle(0.2f), approachAngle(0.1f),
  positionRelativeToTarget(200, 0, 0), angleToPushTarget(M_PI),
  buildTargetParamsFn(NULL), buildTargetMapBuilderRequestFn(NULL),
  requestID(Pilot::invalid_Pilot_ID) {}

PilotRequest::PilotRequest(const PilotRequest &req) :
  requestType(req.requestType),
  dx(req.dx), dy(req.dy), da(req.da),
  mapBuilderRequest(req.mapBuilderRequest), mapBuilderRequestFn(req.mapBuilderRequestFn),
  searchRotationAngle(req.searchRotationAngle), exitTest(req.exitTest),
  targetShape(req.targetShape),
  avoidCliffs(req.avoidCliffs), cliffThreshold(req.cliffThreshold),
  avoidObstacles(req.avoidObstacles), obstacleThreshold(req.obstacleThreshold),
  trackRequest(req.trackRequest),
  localizationInterval(req.localizationInterval), localizationTest(req.localizationTest), 
  localizationDisplayParticles(req.localizationDisplayParticles), localizationDisplayInterval(req.localizationDisplayInterval),
  safeDistanceAroundTarget(req.safeDistanceAroundTarget), subtendAngle(req.subtendAngle), approachAngle(req.approachAngle),
  positionRelativeToTarget(req.positionRelativeToTarget), angleToPushTarget(req.angleToPushTarget),
  buildTargetParamsFn(req.buildTargetParamsFn), buildTargetMapBuilderRequestFn(req.buildTargetMapBuilderRequestFn),
  requestID(req.requestID) {}

}
