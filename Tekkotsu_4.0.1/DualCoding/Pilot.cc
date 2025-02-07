#include "Behaviors/Transitions/CompletionTrans.h"
#include "Behaviors/Transitions/EventTrans.h"
#include "Behaviors/Transitions/TimeOutTrans.h"
#include "Events/EventRouter.h"
#include "Events/LocomotionEvent.h"

#include "MapBuilder.h"
#include "Lookout.h"
#include "VRmixin.h"

#include "ShapeTarget.h"

#include "Pilot.h"

using namespace std;

namespace DualCoding {

Pilot::Pilot() : 
  StateNode("Pilot"), requests(), curReq(NULL), idCounter(0), 
  waypointwalk_id(MotionManager::invalid_MC_ID), posture_id(MotionManager::invalid_MC_ID),
  lastDisplayParticleTime(0),
  dispatchNode(NULL)
{
	setRetain(false); // don't want VRmixin-based subnodes to persist when Pilot is stopped
}


void Pilot::DoStart() {
  StateNode::DoStart();
  SharedObject<WaypointWalkMC> waypointwalk_mc;
  waypointwalk_id = motman->addPersistentMotion(waypointwalk_mc,MotionManager::kIgnoredPriority);
  SharedObject<PostureMC> posture_mc;
  posture_id = motman->addPersistentMotion(posture_mc,MotionManager::kIgnoredPriority);
  if ( verbosity & PVstart ) 
    cout << "Pilot starting up: waypointwalk_id=" << waypointwalk_id 
	 << ", posture_id=" << posture_id << endl;
}

void Pilot::DoStop() {
  if ( verbosity & PVstart )
    cout << "Pilot is shutting down." << endl;
  abort();
  motman->removeMotion(waypointwalk_id);
  waypointwalk_id = MotionManager::invalid_MC_ID;
  motman->removeMotion(posture_id);
  posture_id = MotionManager::invalid_MC_ID;
  StateNode::DoStop();
}

void Pilot::abort() {
  if ( dispatchNode )
    dispatchNode->DoStop();
  curReq = NULL;
  while (!requests.empty()) {
    delete requests.front();
    requests.pop();
  }
  motman->setPriority(waypointwalk_id,MotionManager::kIgnoredPriority);
  motman->setPriority(posture_id,MotionManager::kIgnoredPriority);
}

void Pilot::setup() {
  StateNode::setup();
  dispatchNode = new Dispatch;
  addNode(dispatchNode);
}

unsigned int Pilot::executeRequest(const PilotRequest &req) {
  requests.push(new PilotRequest(req));
  const unsigned int id =  ++idCounter;
  requests.back()->requestID = id;
  executeRequest();
  return id;
}

void Pilot::executeRequest() {
  // check carefully because event from prior request completion could
  // have caused user code to already queue and start another request
  if ( curReq == NULL & !requests.empty() ) {
    curReq = requests.front();
    if ( verbosity & PVexecute )
      cout << "Pilot executing request " << curReq->requestID << endl;
    dispatchNode->DoStart();
  }
}

void Pilot::requestComplete(const bool result) {
  if ( curReq == NULL ) {
    cout << "Pilot::requestComplete had NULL curReq !!!!!!!!!!!!!!!!" << endl;
    return;
  }
  if ( curReq->trackRequest != NULL )
    VRmixin::lookout.stopTrack();
  const unsigned int reqID = curReq->requestID;
  delete curReq;
  curReq = NULL;
  requests.pop();
  motman->setPriority(waypointwalk_id,MotionManager::kIgnoredPriority);
  motman->setPriority(posture_id,MotionManager::kIgnoredPriority);
  if ( verbosity & PVcomplete )
    cout << "Pilot request " << reqID << " complete." << endl;
  erouter->postEvent(EventBase(EventBase::pilotEGID, reqID, EventBase::deactivateETID,0, "Pilot Completion",float(result)));
  executeRequest();
}

//================ Dispatch

void Pilot::Dispatch::setup() {
  VisualRoutinesStateNode::setup();
  successNode = new Success;
  addNode(successNode);
  failureNode = new Failure;
  addNode(failureNode);

  //================ Walk

  walkNode = new Walk;
  addNode(walkNode);
  walkNode->setMC(pilot.waypointwalk_id);
  walkNode->addTransition(new CompletionTrans(successNode));

  //================ Visual Search

  visualsearchNode = new VisualSearch;
  addNode(visualsearchNode);
  VisualSearchHaveMap *visualsearchhavemapNode = new VisualSearchHaveMap;
  addNode(visualsearchhavemapNode);
  VisualSearchWalk *visualsearchwalkNode = new VisualSearchWalk;
  visualsearchwalkNode->setMC(pilot.waypointwalk_id);
  addNode(visualsearchwalkNode);

  visualsearchNode->addTransition(new EventTrans(visualsearchhavemapNode, EventBase::mapbuilderEGID,
						       &(visualsearchNode->mapbuilder_id), EventBase::deactivateETID));
  visualsearchhavemapNode->addTransition(new CompletionTrans(visualsearchwalkNode));
  visualsearchwalkNode->addTransition(new CompletionTrans(visualsearchNode));

  //================ GotoShape

  gotoshapeNode = new GotoShape;
  gotoshapeNode->setMC(pilot.waypointwalk_id);
  addNode(gotoshapeNode);
  gotoshapeNode->addTransition(new CompletionTrans(successNode));

  //================ GotoTarget

  gotoTargetNode = new GotoTarget;
  gotoTargetNode->setMC(pilot.waypointwalk_id);
  addNode(gotoTargetNode);
  
  CreepToShapeStand *gotoTargetStandNode = new CreepToShapeStand;
  gotoTargetStandNode->setMC(pilot.posture_id);
  addNode(gotoTargetStandNode);

  CreepToShapeWait *gotoTargetWaitNode = new CreepToShapeWait;
  addNode(gotoTargetWaitNode);

  CreepToShapeBuildMap *gotoTargetBuildMapNode = new CreepToShapeBuildMap;
  addNode(gotoTargetBuildMapNode);
  
  gotoTargetNode->buildTargetNode = new BuildTarget;
  addNode(gotoTargetNode->buildTargetNode);

  gotoTargetNode->addTransition(new CompletionTrans(gotoTargetStandNode));
  gotoTargetStandNode->addTransition(new CompletionTrans(gotoTargetWaitNode));
  gotoTargetWaitNode->addTransition(new TimeOutTrans(gotoTargetBuildMapNode,2000));
  gotoTargetBuildMapNode->addTransition(new EventTrans(gotoTargetNode->buildTargetNode,
               EventBase::mapbuilderEGID,
               &CreepToShapeBuildMap::mapreq_id,
               EventBase::deactivateETID));
  gotoTargetNode->buildTargetNode->addTransition(new CompletionTrans(gotoTargetNode));
  
  //================ PushTarget
  
  pushTargetNode = new PushTarget;
  pushTargetNode->setMC(pilot.waypointwalk_id);
  addNode(pushTargetNode);
  
  CreepToShapeStand *pushTargetStandNode = new CreepToShapeStand;
  pushTargetStandNode->setMC(pilot.posture_id);
  addNode(pushTargetStandNode);
  
  CreepToShapeWait *pushTargetWaitNode = new CreepToShapeWait;
  addNode(pushTargetWaitNode);

  CreepToShapeBuildMap *pushTargetBuildMapNode = new CreepToShapeBuildMap;
  addNode(pushTargetBuildMapNode);
  
  pushTargetNode->buildTargetNode = new BuildTarget;
  addNode(pushTargetNode->buildTargetNode);

  pushTargetNode->addTransition(new CompletionTrans(pushTargetStandNode));
  pushTargetStandNode->addTransition(new CompletionTrans(pushTargetWaitNode));
  pushTargetWaitNode->addTransition(new TimeOutTrans(pushTargetBuildMapNode,2000));
  pushTargetBuildMapNode->addTransition(new EventTrans(pushTargetNode->buildTargetNode,
               EventBase::mapbuilderEGID,
               &CreepToShapeBuildMap::mapreq_id,
               EventBase::deactivateETID));
  pushTargetNode->buildTargetNode->addTransition(new CompletionTrans(pushTargetNode));
  
  //================ CreepToShape

  creeptoshapeNode = new CreepToShape;
  creeptoshapeNode->setMC(pilot.waypointwalk_id);
  addNode(creeptoshapeNode);

  CreepToShapeStand *creeptoshapestandNode = new CreepToShapeStand;
  creeptoshapestandNode->setMC(pilot.posture_id);
  addNode(creeptoshapestandNode);

  CreepToShapeWait *creeptoshapewaitNode = new CreepToShapeWait;
  addNode(creeptoshapewaitNode);

  CreepToShapeBuildMap *creeptoshapebuildmapNode = new CreepToShapeBuildMap;
  addNode(creeptoshapebuildmapNode);

  CreepToShapeLocalize *creeptoshapelocalizeNode = new CreepToShapeLocalize;
  addNode(creeptoshapelocalizeNode);

  creeptoshapeNode->addTransition(new CompletionTrans(creeptoshapestandNode));
  creeptoshapestandNode->addTransition(new CompletionTrans(creeptoshapewaitNode));
  creeptoshapewaitNode->addTransition(new TimeOutTrans(creeptoshapebuildmapNode,500));
  creeptoshapebuildmapNode->addTransition(new EventTrans(creeptoshapelocalizeNode,
							 EventBase::mapbuilderEGID,
							 &CreepToShapeBuildMap::mapreq_id,
							 EventBase::deactivateETID));
  creeptoshapelocalizeNode->addTransition(new CompletionTrans(creeptoshapeNode));

  //================ IRCliff and IRObstacle
#ifdef TGT_HAS_IR_DISTANCE
#ifdef TGT_ERS7
  ircliffNode = new IRCliff;
  addNode(ircliffNode);
#endif //TGT_ERS7
  irobstacleNode = new IRObstacle;
  addNode(irobstacleNode);
#endif // TGT_HAS_IR_DISTANCE

  //================ Localize

  localizeNode = new Localize;
  addNode(localizeNode);

}

void Pilot::Dispatch::DoStart() {
  VisualRoutinesStateNode::DoStart();
#ifdef TGT_HAS_IR_DISTANCE
#ifdef TGT_ERS7
  if ( pilot.curReq->avoidCliffs )
    ircliffNode->DoStart();
#endif // TGT_ERS7
  if ( pilot.curReq->avoidObstacles )
    irobstacleNode->DoStart();
#endif // TGT_HAS_IR_DISTANCE
  if ( pilot.curReq->trackRequest )
    if ( lookout.busy() )
      cout << "Error: Pilot cannot launch track request because the Lookout is busy!" << endl;
    else
      lookout.executeRequest(*pilot.curReq->trackRequest);
  if ( pilot.curReq->localizationInterval > 0 )
    localizeNode->DoStart();
  else if ( pilot.curReq->localizationDisplayInterval > 0 )
    cout << "Pilot warning: localizationDisplayInterval > 0 but no localization requested." << endl;
  switch ( pilot.curReq->getRequestType() ) {
  case PilotRequest::walk:
    walkNode->DoStart();
    break;
  case PilotRequest::visualSearch:
    visualsearchNode->DoStart();
    break;
  case PilotRequest::gotoShape:
    gotoshapeNode->DoStart();
    break;
  case PilotRequest::gotoTarget:
    gotoTargetNode->DoStart();
    break;
  case PilotRequest::pushTarget:
    pushTargetNode->DoStart();
    break;
  case PilotRequest::creepToShape:
    if ( pilot.curReq->mapBuilderRequest == NULL || 
	 pilot.curReq->mapBuilderRequest->worldTargets.size() == 0 ||
	 pilot.curReq->trackRequest != NULL ) {
      cout << "Pilot error: creeptoShape requires mapBuilderRequest with a worldTarget, and no trackRequest" << endl;
      VRmixin::pilot.failure();
    }
    else
      creeptoshapeNode->DoStart();
    break;
  default:
    cout << "Pilot request type " << pilot.curReq->getRequestType() << " not yet implemented." << endl;
    VRmixin::pilot.failure();
  }
}
    
//================ Success and Failure

void Pilot::Success::DoStart() {
  StateNode::DoStart();
  if ( Pilot::getVerbosity() & Pilot::PVsuccess )
    cout << "Pilot result: Success" << endl;
  VRmixin::pilot.dispatchNode->DoStop();
  VRmixin::pilot.requestComplete(true);
}

void Pilot::Failure::DoStart() {
  StateNode::DoStart();
  if ( Pilot::getVerbosity() & Pilot::PVfailure )
    cout << "Pilot result: Failure" << endl;
  VRmixin::pilot.dispatchNode->DoStop();
  VRmixin::pilot.requestComplete(false);
}

//================ Walk

void Pilot::Walk::DoStart() {
  WaypointWalkNode::DoStart();
  PilotRequest &req = *VRmixin::pilot.curReq;
  getMC()->getWaypointList().clear();
  getMC()->addEgocentricWaypoint(req.dx/1000, req.dy/1000, req.da, true, 0.1);
  getMC()->go();
}

//================ VisualSearch

void Pilot::VisualSearch::DoStart() {
  VisualRoutinesStateNode::DoStart();
  if ( pilot.curReq->mapBuilderRequest == NULL ) {
    cout << "Pilot received visualSearch request with invalid mapBuilderRequest field" << endl;
    pilot.failure();
  }
  else if ( pilot.curReq->exitTest == NULL &&  pilot.curReq->searchRotationAngle != 0 ) {
    cout << "Pilot received visualSearch request with no exitTest and nonzero searchRotationAngle" << endl;
    pilot.failure();
  }
  else
    mapbuilder_id = VRmixin::mapBuilder.executeRequest(*pilot.curReq->mapBuilderRequest);
}

void Pilot::VisualSearchHaveMap::DoStart() {
  VisualRoutinesStateNode::DoStart();
  if ( (*pilot.curReq->exitTest)() )
    pilot.success();
  else if ( pilot.curReq->searchRotationAngle == 0 )
    pilot.failure();
  else
    postCompletionEvent();  // go to VisualSearchWalk to turn body
}

void Pilot::VisualSearchWalk::DoStart() {
  WaypointWalkNode::DoStart();
  getMC()->getWaypointList().clear();
  getMC()->addEgocentricWaypoint(0,0,VRmixin::pilot.curReq->searchRotationAngle,true,0);
  getMC()->go();
}

//================ GotoShape

void Pilot::GotoShape::DoStart() {
  WaypointWalkNode::DoStart();
  const ShapeRoot &target = VRmixin::pilot.curReq->targetShape;
  Point const targetLoc(target->getCentroid());
  getMC()->getWaypointList().clear();
  switch ( target->getRefFrameType() ) {
  case unspecified:
  case camcentric:
    cout << "Pilot: GotoShape target " << target << " has suspect reference frame type; assuming egocentric." << endl;
    // fall through to next case
  case egocentric: {
    AngSignPi bearing = atan2(targetLoc.coordY(),targetLoc.coordX());
    float const distance = targetLoc.distanceFrom(Point(0,0));
    cout << "Pilot egocentric gotoShape: rotate by " << float(bearing)*180/M_PI << " deg., then forward "
	 << distance << " mm" << endl;
    getMC()->addEgocentricWaypoint(0,0,bearing,true,0);
    getMC()->addEgocentricWaypoint(distance/1000,0,0,true,0.1);  // convert distance from mm to meters
    break;
  }
  case allocentric:
    cout << "Pilot alloocentric gotoShape: " << targetLoc 
	 << "    current position: " << VRmixin::theAgent->getCentroid() << endl;
    getMC()->addAbsoluteWaypoint(targetLoc.coordX()/1000, targetLoc.coordY()/1000, 0, true, 0.1);
  }
  getMC()->go();
}

//================ GotoTarget

// Assumes the target has been set in the Pilot request
void Pilot::GotoTarget::DoStart() {
  WaypointWalkNode::DoStart();

  // Set the parameters for building the target
  bool buildFrontLeft = true, buildFrontRight = true, buildBackLeft = true, buildBackRight = true, lookAtCentroid = true;
  int maxRetries = 10;
  if (VRmixin::pilot.curReq->buildTargetParamsFn != NULL) {
    (*VRmixin::pilot.curReq->buildTargetParamsFn)(&buildFrontLeft, &buildFrontRight, &buildBackLeft, &buildBackRight, &lookAtCentroid, &maxRetries);
  }
  if (buildTargetNode != NULL)
    buildTargetNode->setParams(buildFrontLeft, buildFrontRight, buildBackLeft, buildBackRight, lookAtCentroid, maxRetries);

  Point waypoint;
  AngSignPi angle;
  int result = getNextWaypoint( &waypoint, &angle );
  if (result < 0) {
    VRmixin::pilot.failure();
    return;
  }
  else if (result > 0) {
    VRmixin::pilot.success();
    return;
  }
  
  if ( waypoint != Point(0, 0, 0) ) {
    if (abs(angle) <= M_PI / 6) {
      //cout << "Going to (" << waypoint.coordX() << ", " << waypoint.coordY() << ", " << waypoint.coordZ() << ") and turning " << (float)angle / M_PI * 180.0f << " degrees." << endl;
      float currentAngle = getMC()->getCurA() + angle;
      getMC()->getWaypointList().clear();
      getMC()->addEgocentricWaypoint(waypoint.coordX() / 1000, waypoint.coordY() / 1000, (float)currentAngle, false, 0.1f);
      getMC()->go();
    }
    else {
      // Turning angle too large, only turning in place
      angle = (angle >= 0) ? M_PI / 6 : -M_PI / 6;
      getMC()->getWaypointList().clear();
      getMC()->addEgocentricWaypoint(0, 0, (float)angle, true, 0.1f);
      getMC()->go();
    }
  }
  else {
    //cout << "At destination, only turning " << (float)angle / M_PI * 180.0f << " degrees." << endl;
    getMC()->getWaypointList().clear();
    getMC()->addEgocentricWaypoint(0, 0, (float)angle, true, 0.1f);
    getMC()->go();
  }
  
}

/** Calculates the next waypoint and angle to turn in local coordinates.
 *  \return 0 on success (got a new waypoint), 1 on success (arrived at destination), -1 on failure
 */
int Pilot::GotoTarget::getNextWaypoint(Point *point, AngSignPi *angle) {
  const Shape<TargetData>& target = ShapeRootTypeConst( VRmixin::pilot.curReq->targetShape, TargetData );
  if (!target.isValid()) {
    return -1;
  }
  if (target->getRefFrameType() != egocentric) {
    return -1;
  }
  
  bool pointOK = true;
  bool angleOK = false;
  
  const float orientation = target->getOrientation();
  
  // These points are relative to the target (in local coordinates) i.e. offset from target's centroid but not orientation
  Point desiredPoint( VRmixin::pilot.curReq->positionRelativeToTarget.coordX() * cos(orientation) - VRmixin::pilot.curReq->positionRelativeToTarget.coordY() * sin(orientation),
                      VRmixin::pilot.curReq->positionRelativeToTarget.coordX() * sin(orientation) + VRmixin::pilot.curReq->positionRelativeToTarget.coordY() * cos(orientation),
                      VRmixin::pilot.curReq->positionRelativeToTarget.coordZ() );
  Point presentPoint = target->getCentroid() * -1;
  
  // If we're within 100mm of the desired point, only need to check angle
  *point = Point(0, 0, 0);
  if (presentPoint.distanceFrom(desiredPoint) > 100) {
    pointOK = false;
    
    // Calculate present and desired angle around target
    AngSignPi desiredAngle = atan2(desiredPoint.coordY(), desiredPoint.coordX());
    AngSignPi presentAngle = atan2(presentPoint.coordY(), presentPoint.coordX());
    
    // Go to the desired point if present angle is close to desired angle
    if (abs((double)(presentAngle - desiredAngle)) <= VRmixin::pilot.curReq->approachAngle) {
      *point = desiredPoint + target->getCentroid();
    }
    // Go to the next safe point
    else {
      // decide whether to go clockwise or anti-clockwise
      AngSignPi diff = desiredAngle - presentAngle;
      if (abs((double)diff) < VRmixin::pilot.curReq->subtendAngle) {
        presentAngle = desiredAngle;
      }
      else if ((double)diff >= 0) {
        presentAngle += VRmixin::pilot.curReq->subtendAngle;
      }
      else {
        presentAngle -= VRmixin::pilot.curReq->subtendAngle;
      }
      *point = getPointAtAngle(presentAngle);
    }
    
    // Cap distance to a maximum of 200mm
    *point = Point( point->coordX(), point->coordY(), 0 );
    float distance = point->distanceFrom( Point( 0, 0, 0) );
    float scaleFactor = distance < 200 ? 1.0 : 200 / distance;
    *point = *point * scaleFactor;
  }
  
  Point delta = target->getCentroid() - *point;
  *angle = atan2(delta.coordY(), delta.coordX());
  
  // If the angle is within 15 degrees and no translation is needed, we're done
  if (pointOK && (abs( (float)*angle / M_PI * 180.0f ) <= 15)) {
    *angle = 0;
    angleOK = true;
  }
  
  return ( pointOK && angleOK ) ? 1 : 0;
}

/** Returns a point in local coordinates corresponding to an angle around the target at
 *  the distance given in the pilot request.
 */
Point Pilot::GotoTarget::getPointAtAngle(AngSignPi angle) {
  Point point(VRmixin::pilot.curReq->safeDistanceAroundTarget * cos(angle),
              VRmixin::pilot.curReq->safeDistanceAroundTarget * sin(angle),
              VRmixin::pilot.curReq->positionRelativeToTarget.coordZ());
  point += VRmixin::pilot.curReq->targetShape->getCentroid();
  return point;
}

//================ PushTarget

void Pilot::PushTarget::DoStart() {
  WaypointWalkNode::DoStart();
  
  // Set the parameters for building the target
  bool buildFrontLeft = true, buildFrontRight = true, buildBackLeft = true, buildBackRight = true, lookAtCentroid = true;
  int maxRetries = 10;
  if (VRmixin::pilot.curReq->buildTargetParamsFn != NULL) {
    (*VRmixin::pilot.curReq->buildTargetParamsFn)(&buildFrontLeft, &buildFrontRight, &buildBackLeft, &buildBackRight, &lookAtCentroid, &maxRetries);
  }
  if (buildTargetNode != NULL)
    buildTargetNode->setParams(buildFrontLeft, buildFrontRight, buildBackLeft, buildBackRight, lookAtCentroid, maxRetries);

  const Shape<TargetData>& target = ShapeRootTypeConst( VRmixin::pilot.curReq->targetShape, TargetData );
  
  double orientation = target->getOrientation();
  // This point is in local coordinates
  Point desiredPoint = Point( VRmixin::pilot.curReq->positionRelativeToTarget.coordX() * cos(orientation) - VRmixin::pilot.curReq->positionRelativeToTarget.coordY() * sin(orientation),
                              VRmixin::pilot.curReq->positionRelativeToTarget.coordX() * sin(orientation) + VRmixin::pilot.curReq->positionRelativeToTarget.coordY() * cos(orientation),
                              VRmixin::pilot.curReq->positionRelativeToTarget.coordZ() )
                     + target->getCentroid();
                      
  // Create push line
  AngSignPi pushAngle = (double)VRmixin::pilot.curReq->angleToPushTarget + orientation;
  LineData pushLine(VRmixin::localShS, desiredPoint, pushAngle);
  
  // Get present position relative to target
  Point origin(0, 0, 0);
  double totalDisplacement = 200.0;
  double xDisplacement = 200.0;
  double yDisplacement = pushLine.perpendicularDistanceFrom(origin);
  // figure out which side of the line the dog is on and change direction if needed
  Point leftPoint(pushLine.getCentroid().coordX(), pushLine.getCentroid().coordY() + 100, pushLine.getCentroid().coordZ());
  if (pushLine.pointsOnSameSide( leftPoint, origin )) {
    yDisplacement = -yDisplacement;
  }
  
  float scaleFactor = abs(yDisplacement) < totalDisplacement ? 1.0 : totalDisplacement / yDisplacement;
  yDisplacement = yDisplacement * scaleFactor;
  totalDisplacement -= abs(yDisplacement);
  if (totalDisplacement <= 0) {
    xDisplacement = 0;
  }
  else {
    scaleFactor = xDisplacement < totalDisplacement ? 1.0 : totalDisplacement / xDisplacement;
    xDisplacement = xDisplacement * scaleFactor;
  }
  
  if (abs((float)pushAngle) <= M_PI / 6) {
    //cout << "PushTarget: turning by " << (float)pushAngle / M_PI * 180.0f << " deg, y by " << yDisplacement << " mm, x by " << xDisplacement << " mm." << endl;
  
    float currentAngle = getMC()->getCurA() + pushAngle;
    
    getMC()->getWaypointList().clear();
    getMC()->addEgocentricWaypoint(0, 0, (float)pushAngle, true, 0.1f);
    if (abs(yDisplacement) > 10) {
      getMC()->addEgocentricWaypoint(0, yDisplacement / 1000, currentAngle, false, 0.1f);
    }
    if (abs(xDisplacement) > 10) {
      getMC()->addEgocentricWaypoint(xDisplacement / 1000, 0, currentAngle, false, 0.1f);
    }
    getMC()->go();
  }
  else {
    // Turn angle too large, only turning in place
    pushAngle = (pushAngle >= 0) ? M_PI / 6 : -M_PI / 6;
    getMC()->getWaypointList().clear();
    getMC()->addEgocentricWaypoint(0, 0, (float)pushAngle, true, 0.1f);
    getMC()->go();
  }
}

//================ BuildTarget
void Pilot::BuildTarget::DoStart() {
  VisualRoutinesStateNode::DoStart();

  std::cout << "BuildTarget: DoStart" << std::endl;
  
  erouter->addListener(this, EventBase::mapbuilderEGID);
  
  triesLeft = maxRetries;
  generateMapBuilderRequest();
}

void Pilot::BuildTarget::DoStop() {
  std::cout << "BuildTarget: DoStop" << std::endl;
  
  myState = none;
  VisualRoutinesStateNode::DoStop();
}

void Pilot::BuildTarget::processEvent(const EventBase& event) {
  switch ( event.getGeneratorID() ) {

  case EventBase::mapbuilderEGID: {
    if ((myState == build) && (event.getTypeID() == EventBase::deactivateETID)) {
      GET_SHAPE(target, TargetData, localShS);
      if (target.isValid()) {
        target->printParams();
      }
      
      generateMapBuilderRequest();
    }
    
    break;
  };
  default:
    std::cout << "BuildTarget: Unexpected event: " << event.getDescription() << std::endl;
  }
}

void Pilot::BuildTarget::setParams(bool _buildFrontLeft, bool _buildFrontRight, bool _buildBackLeft, bool _buildBackRight, bool _lookAtCentroid, int _maxRetries) {
  buildFrontLeft  = _buildFrontLeft;
  buildFrontRight = _buildFrontRight;
  buildBackLeft   = _buildBackLeft;
  buildBackRight  = _buildBackRight;
  lookAtCentroid  = _lookAtCentroid;
  maxRetries      = _maxRetries;
}

void Pilot::BuildTarget::generateMapBuilderRequest() {
  keepBestTarget();
  GET_SHAPE(target, TargetData, localShS);
  if (target.isValid()) {
    
    Point nextPt;
    if (getNextPoint(&nextPt)) {
      VRmixin::pilot.curReq->targetShape = target;
    
      //cout << "BuildTarget: no more points to build." << endl;
      myState = complete;
      postCompletionEvent();
      return;
    }
    
    myState = build;
    
    if ( VRmixin::pilot.curReq->buildTargetMapBuilderRequestFn != NULL ) {
      MapBuilderRequest* mapreq = (*VRmixin::pilot.curReq->buildTargetMapBuilderRequestFn)(nextPt);
      VRmixin::mapBuilder.executeRequest(*mapreq);
      delete mapreq;
    }
    else {
      cout << "BuildTarget: No MapBuilderRequest function provided." << endl;
      myState = error;
      VRmixin::pilot.failure();
      return;
    }
  }
  else {
    cout << "BuildTarget: Cannot find target in build phase." << endl;
    myState = error;
    VRmixin::pilot.failure();
    return;
  }
}

void Pilot::BuildTarget::keepBestTarget() {
  Shape<TargetData> best;
  int bestCount = 0;
  std::vector<Shape<TargetData> >target_vector = select_type<TargetData>(localShS);
  std::vector<Shape<TargetData> >::iterator targetIterator;
  
  // Iterates through all targets in local space, and keeps the one with the most number of valid endpoints, or the longest, in that order
  for (targetIterator = target_vector.begin(); targetIterator != target_vector.end(); targetIterator++) {
    Shape<TargetData> target = *(targetIterator);
    int targetCount = 0;
    if (target->getFrontLeftPt().isValid())
      targetCount++;
    if (target->getFrontRightPt().isValid())
      targetCount++;
    if (target->getBackLeftPt().isValid())
      targetCount++;
    if (target->getBackRightPt().isValid())
      targetCount++;
    if (!best.isValid() || (targetCount > bestCount) || ((targetCount == bestCount) && (target->getLength() > best->getLength()))) {
      best = target;
      bestCount = targetCount;
    }
  }
  if (best.isValid()) {
    best->setName("target");
    for (targetIterator = target_vector.begin(); targetIterator != target_vector.end(); targetIterator++) {
      Shape<TargetData> other_target = *(targetIterator);
      if (best != other_target) {
        localShS.deleteShape(other_target);
      }
    }
  }
}

bool Pilot::BuildTarget::getNextPoint(Point *nextPoint) {
  if (triesLeft <= 0) {
    return true;
  }
  triesLeft--;
  
  GET_SHAPE(target, TargetData, localShS);
  if (target.isValid()) {
    EndPoint &frontLeft  = target->getFrontLeftPt();
    EndPoint &frontRight = target->getFrontRightPt();
    EndPoint &backLeft   = target->getBackLeftPt();
    EndPoint &backRight  = target->getBackRightPt();
    
    Point zero(0, 0, 0);
    
    if (buildFrontLeft && target->isFrontValid() && !frontLeft.isValid() && ((frontLeftPt == zero) || (frontLeftPt != frontLeft))) {
      frontLeftPt = frontLeft;
      cout << "Looking at front left pt: " << frontLeft << endl;
      *nextPoint = frontLeft;
      return false;
    }
    else if (buildFrontRight && target->isFrontValid() && !frontRight.isValid() && ((frontRightPt == zero) || (frontRightPt != frontRight))) {
      frontRightPt = frontRight;
      cout << "Looking at front right pt: " << frontRight << endl;
      *nextPoint = frontRight;
      return false;
    }
    else if (buildBackLeft && target->isBackValid() && !backLeft.isValid() && ((backLeftPt == zero) || (backLeftPt != backLeft))) {
      backLeftPt = backLeft;
      cout << "Looking at back left pt: " << backLeft << endl;
      *nextPoint = backLeft;
      return false;
    }
    else if (buildBackRight && target->isBackValid() && !backRight.isValid() && ((backRightPt == zero) || (backRightPt != backRight))) {
      backRightPt = backRight;
      cout << "Looking at back right pt: " << backRight << endl;
      *nextPoint = backRight;
      return false;
    }
    else if (lookAtCentroid) {
      lookAtCentroid = false;
      if ((buildFrontLeft || buildFrontRight) && ((centroid == zero) || (centroid != target->getFrontCentroid()))) {
        centroid = target->getCentroid();
        cout << "Looking at centroid: " << centroid << endl;
        *nextPoint = centroid;
        return false;
      }
      else if (!buildFrontLeft && !buildFrontRight && (buildBackLeft || buildBackRight) && ((centroid == zero) || (centroid != target->getBackCentroid()))) {
        centroid = target->getBackCentroid();
        cout << "Looking at back centroid: " << centroid << endl;
        *nextPoint = centroid;
        return false;
      }
    }
  }
  return true;
}
  
//================ CreepToShape

void Pilot::CreepToShape::DoStart() {
  WaypointWalkNode::DoStart();
  cout << "  >>>>> CreepToShape" << endl;
  if ( VRmixin::lookout.busy() )
    cout << "Error: Pilot cannot launch track request because the Lookout is busy!" << endl;
  else
    VRmixin::lookout.executeRequest(LookoutTrackRequest(VRmixin::pilot.curReq->mapBuilderRequest->worldTargets.front()->targetShape));
  const ShapeRoot &target = VRmixin::pilot.curReq->targetShape;
  Point const targetLoc(target->getCentroid());
  getMC()->getWaypointList().clear();
  switch ( target->getRefFrameType() ) {
  case unspecified:
  case camcentric:
    cout << "Pilot: GotoShape target " << target << " has suspect reference frame type; assuming egocentric." << endl;
    // fall through to next case
  case egocentric: {
    // can't creep to egocentric shape because motion invalidates localShS, so try to get there in one trajectory
    float distance = targetLoc.xyNorm();
    AngSignPi bearing = atan2(targetLoc.coordY(),targetLoc.coordX());
    cout << "Pilot egocentric creepToShape: rotate by " << float(bearing)*(180.0/M_PI) << " deg., then forward "
	 << distance << " mm" << endl;
    if ( distance >= 200 ) {
      getMC()->addEgocentricWaypoint(0,0,bearing,true,0);
      getMC()->addEgocentricWaypoint(distance/1000,0,0,true,0.06);  // convert distance from mm to meters
      VRmixin::mapBuilder.moveAgent(0.0, 0.0, float(bearing));
      VRmixin::mapBuilder.moveAgent(distance, 0.0, 0.0);
    } else {
      getMC()->addEgocentricWaypoint(distance/1000, 0, VRmixin::theAgent->getOrientation(), false, 0.06);  // convert distance from mm to meters
      VRmixin::mapBuilder.moveAgent(distance, 0.0, 0.0);
    }      
    break;
  }
  case allocentric: // this is the normal case
    Point const agentLoc = VRmixin::theAgent->getCentroid();
    float const distance = targetLoc.xyDistanceFrom(agentLoc);
    if ( distance < 50 ) {
      VRmixin::pilot.success();
      return;
    }
    float const scaleFactor = distance < 200 ? 1.0 : 200/distance;
    Point const waypointLoc = agentLoc + (targetLoc-agentLoc)*scaleFactor;  // take at most 200 mm steps
    cout << "Pilot creepToShape: "
	 << "current position: " << agentLoc
	 << "  next waypoint: " << waypointLoc
	 << "  target loc: " << targetLoc << endl;
    if ( distance > 200 )
      getMC()->addAbsoluteWaypoint(waypointLoc.coordX()/1000, waypointLoc.coordY()/1000, 0, true, 0.06);
    else
      getMC()->addAbsoluteWaypoint(waypointLoc.coordX()/1000, waypointLoc.coordY()/1000, VRmixin::theAgent->getOrientation(), false, 0.06);
    float const heading = atan2(waypointLoc.coordY(),waypointLoc.coordX());
    VRmixin::mapBuilder.setAgent(Point(waypointLoc.coordX(),
				       waypointLoc.coordY(),
				       agentLoc.coordZ(),
				       allocentric),
				 heading);
    break;
  }
  getMC()->go();
}

void Pilot::CreepToShapeStand::DoStop() {
  PostureNode::DoStop(); // call parent first so we can override its effects
  motman->setPriority(VRmixin::pilot.posture_id,MotionManager::kBackgroundPriority);
}

void Pilot::CreepToShapeBuildMap::DoStart() {
  VisualRoutinesStateNode::DoStart();
  VRmixin::lookout.stopTrack();
  if ( VRmixin::pilot.curReq->mapBuilderRequest )
    VRmixin::mapBuilder.executeRequest(*VRmixin::pilot.curReq->mapBuilderRequest, &mapreq_id);
  else if ( VRmixin::pilot.curReq->mapBuilderRequestFn ) {
    MapBuilderRequest* mapreq = (*VRmixin::pilot.curReq->mapBuilderRequestFn)();
    VRmixin::mapBuilder.executeRequest(*mapreq, &mapreq_id);
    delete mapreq;
  }
}

void Pilot::CreepToShapeLocalize::DoStart() {
  StateNode::DoStart();
  cout << "  >>>>> CreepToShape Localize" << endl;
  if ( VRmixin::pilot.curReq->localizationTest != NULL &&
       (*VRmixin::pilot.curReq->localizationTest)() == false ) {
    VRmixin::pilot.failure();
    return;
  }
  VRmixin::particleFilter->setMinAcceptableWeight(-3);
  for ( int i=0; i<8; i++ )
    VRmixin::particleFilter->update();
  VRmixin::particleFilter->setAgent();
  VRmixin::particleFilter->displayParticles();
  Point const loc = VRmixin::theAgent->getCentroid();
  MMAccessor<WaypointWalkMC>(VRmixin::pilot.waypointwalk_id)->
    setCurPos(loc.coordX()/1000, loc.coordY()/1000, VRmixin::theAgent->getOrientation());
  // cout << "View world map; then send a text message to continue..." << endl;
  // erouter->addListener(this,EventBase::textmsgEGID);
  postCompletionEvent();
}

void Pilot::CreepToShapeLocalize::processEvent(const EventBase&) {
    postCompletionEvent();
}

//================ IRCliff
#ifdef TGT_HAS_IR_DISTANCE
#ifdef TGT_ERS7

void Pilot::IRCliff::DoStart() {
  StateNode::DoStart();
  erouter->addListener(this,EventBase::sensorEGID, SensorSrcID::UpdatedSID, EventBase::statusETID);
}

void Pilot::IRCliff::processEvent(const EventBase &) {
  if ( state->sensors[ChestIRDistOffset] > VRmixin::pilot.curReq->cliffThreshold ) {
    cout << "Pilot: cliff detected!" << endl;
    VRmixin::pilot.failure();
  }
}
#endif //TGT_ERS7
#endif //TGT_HAS_IR_DISTANCE

//================ IRObstacle
#ifdef TGT_HAS_IR_DISTANCE

inline float getIR() {
#ifdef TGT_ERS7 
  if ( state->sensors[FarIRDistOffset] > 400 )  // far IR goes 200-1500; near IR goes 50-500
    return state->sensors[FarIRDistOffset];
  else
    return state->sensors[NearIRDistOffset];
#else 
  return state->sensors[IRDistOffset];
#endif
}

void Pilot::IRObstacle::DoStart() {
  StateNode::DoStart();
  erouter->addListener(this,EventBase::sensorEGID, SensorSrcID::UpdatedSID, EventBase::statusETID);
}

void Pilot::IRObstacle::processEvent(const EventBase &) {
  if ( getIR() < VRmixin::pilot.curReq->obstacleThreshold ) {
    cout << "Pilot: obstacle detected!" << endl;
    VRmixin::pilot.failure();
  }
}
#endif // TGT_HAS_IR_DISTANCE


//================ Localize
void Pilot::Localize::DoStart() {
  StateNode::DoStart();
  erouter->addTimer(this, 1, VRmixin::pilot.curReq->localizationInterval, true);
  if ( VRmixin::pilot.curReq->mapBuilderRequest->immediateRequest() == false )  // see comment "For immediate requests..." below
    erouter->addListener(this,EventBase::mapbuilderEGID);
}

/* For immediate requests, MapBuilder will post a completion event before
executeRequest has returned the request id, so processEvent will not
recognize this as a relevant event.  Therefore we will process the map
when executeRequest returns; the completion event will already have been
posted and ignored. */

void Pilot::Localize::processEvent(const EventBase& event) {
  switch ( event.getGeneratorID() ) {
  case EventBase::timerEGID:
    mapbuilder_id = VRmixin::mapBuilder.executeRequest(*VRmixin::pilot.curReq->mapBuilderRequest);
    if ( VRmixin::pilot.curReq->mapBuilderRequest->immediateRequest() )
      processMap(event.getTimeStamp());
    break;
  case EventBase::mapbuilderEGID:
    if ( event.getSourceID() == mapbuilder_id && event.getTypeID() == EventBase::deactivateETID )
      processMap(event.getTimeStamp());
    break;
  default:
    break;
  }
}

void Pilot::Localize::processMap(unsigned int timestamp) {
  if ( VRmixin::pilot.curReq->localizationTest == NULL || (*VRmixin::pilot.curReq->localizationTest)() ) {
    VRmixin::particleFilter->update();
    VRmixin::particleFilter->setAgent();
    Point loc = VRmixin::theAgent->getCentroid();
    MMAccessor<WaypointWalkMC>(VRmixin::pilot.waypointwalk_id)->
      setCurPos(loc.coordX()/1000, loc.coordY()/1000, VRmixin::theAgent->getOrientation());
  int const dispInt = VRmixin::pilot.curReq->localizationDisplayInterval;
  if ( dispInt > 0 )
    if ( timestamp >= VRmixin::pilot.lastDisplayParticleTime+dispInt ) {
      VRmixin::particleFilter->displayParticles(VRmixin::pilot.curReq->localizationDisplayParticles);
      VRmixin::pilot.lastDisplayParticleTime = timestamp;
    }
  }
}

Pilot::PilotVerbosity_t  Pilot::verbosity = -1U;

unsigned int Pilot::CreepToShapeBuildMap::mapreq_id = 0;

} // namespace
