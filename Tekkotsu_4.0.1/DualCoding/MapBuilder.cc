//-*-c++-*-
#include "Events/EventRouter.h"
#include "Events/TextMsgEvent.h"
#include "Events/LookoutEvents.h"
#include "Motion/HeadPointerMC.h"
#include "Shared/mathutils.h"
#include "Shared/newmat/newmat.h"

#include "Shared/Measures.h"
#include "EllipseData.h"    // for extractEllipses
#include "SphereData.h"     // for extractSpheres
#include "BlobData.h"       // for extractSpheres
#include "PolygonData.h"   // for extractPolygons
#include "SphereData.h"
#include "TargetData.h"     // for extractTarget
#include "ShapeRoot.h"
#include "ShapeLine.h"
#include "ShapeEllipse.h"
#include "ShapeBlob.h"
#include "ShapePolygon.h"
#include "ShapeSphere.h"
#include "ShapeTarget.h"
#include "Sketch.h"    // for NEW_SKETCH
#include "visops.h"
#include "VRmixin.h"

#include "LookoutRequests.h"
#include "Lookout.h"
#include "MapBuilder.h"

using namespace std;

namespace DualCoding {

inline float distSq(const NEWMAT::ColumnVector& vec) {
  return vec(1)*vec(1) + vec(2)*vec(2) + vec(3)*vec(3);
}

MapBuilder::MapBuilder() : 
  BehaviorBase("MapBuilder"),
  camSkS(VRmixin::getCamSkS()), camShS(camSkS.getDualSpace()),
  groundShS(VRmixin::getGroundShS()),
  localSkS(VRmixin::getLocalSkS()), localShS(localSkS.getDualSpace()),
  worldSkS(VRmixin::getWorldSkS()), worldShS(worldSkS.getDualSpace()),
  xres(camSkS.getWidth()), yres(camSkS.getHeight()), 
  ground_plane(4),
  theAgent(VRmixin::theAgent),
  localToWorldMatrix(NEWMAT::IdentityMatrix(4)),
  worldToLocalTranslateMatrix(NEWMAT::IdentityMatrix(4)), 
  worldToLocalRotateMatrix(NEWMAT::IdentityMatrix(4)),
  badGazePoints(), 
  requests(), curReq(NULL), idCounter(0), maxDistSq(0), 
  pointAtID(Lookout::invalid_LO_ID), scanID(Lookout::invalid_LO_ID),
  nextGazePoint() {}

void MapBuilder::DoStart() {
  if ( verbosity & MBVstart )
    cout << "MapBuilder::DoStart()\n";
  BehaviorBase::DoStart();

  camSkS.clear(); camShS.clear();
  groundShS.clear();
  localSkS.clear(); localShS.clear();
  worldSkS.clear(); worldShS.clear();
  badGazePoints.clear();
  setAgent(Point(0,0,0),0);

  erouter->addListener(this,EventBase::textmsgEGID);  // listen for commands to move to next gaze point
  erouter->addListener(this,EventBase::lookoutEGID);  // listen for Lookout completion events
}

bool MapBuilder::retain = true;
MapBuilder::MapBuilderVerbosity_t MapBuilder::verbosity = -1U;

/*
  Since MapBuilder is constructed as static from VRmixin, the
  destructor doesn't get called until the robot shuts down.  We have
  to do everything assumed to be done in destructor in DoStop, such as
  clearing the request queue and setting parameters to the initial
  values as set in the constructor.
*/
void MapBuilder::DoStop() {
  cout << "MapBuilder::DoStop()\n";
  while(!requests.empty()) {
    delete requests.front();
    requests.pop();
  }
  curReq = NULL;
  BehaviorBase::DoStop();
}

unsigned int MapBuilder::executeRequest(const MapBuilderRequest& req, unsigned int *req_id) {
  MapBuilderRequest *newreq = new MapBuilderRequest(req);
  const unsigned int this_request  = ++idCounter;
  newreq->requestID = this_request;
  if ( req_id != NULL ) *req_id = this_request;
  requests.push(newreq);
  executeRequest();
  return this_request;
}

void MapBuilder::executeRequest() {
  if ( curReq != NULL || requests.empty() ) return;
  curReq = requests.front();
  if ( verbosity & MBVexecute )
    cout << "MapBuilder::executeRequest: execute " << curReq->requestID << endl;
  erouter->postEvent(EventBase::mapbuilderEGID, curReq->requestID, EventBase::activateETID,0);  
  if ( (curReq->pursueShapes || curReq->doScan || curReq->worldTargets.size()>0) &&
       curReq->getRequestType() == MapBuilderRequest::cameraMap ) {
    cout << "Warning: switching MapBuilderRequest type from cameraMap to localMap because request parameters require head movement." << endl;
    curReq->requestType = MapBuilderRequest::localMap;
  }

  calculateGroundPlane();
  maxDistSq = curReq->maxDist*curReq->maxDist;
 
 if ( curReq->clearShapes ) {
   curReq->gazePts.clear();
   curReq->baseToCamMats.clear();
   switch ( curReq->getRequestType() ) {
   case MapBuilderRequest::localMap:
     localShS.clear();
     localSkS.clear();
     break;
   case MapBuilderRequest::worldMap:
     worldShS.clear();
     worldSkS.clear();
     break;
   default:
     break;
   }
 }

 if ( curReq->immediateRequest() )
   grabCameraImageAndGo();
 else if ( !curReq->searchArea.isValid() & curReq->worldTargets.size() == 0 )
   storeImage();
 else {
   defineGazePts();
   if ( curReq->doScan == true )
     return; // wait for Lookout to finish scanning
   else if ( curReq->worldTargets.size() > 0 )
     doNextSearch();
   else if ( determineNextGazePoint() )
     moveToNextGazePoint();
   else
     requestComplete();
 }
}
  
/*
There are three kinds of events for MapBuilder::processEvent to handle:

1. TextMsg event saying to move to the next gaze point; only needed
when we've asserted manual control of gaze point sequencing for
debugging.

2. Lookout event announcing that a "point at" request is complete and
an image has been stored.  We can now go ahead and process that image.

3. Lookout event announcing that a "scan" request is complete and gaze
points have been stored.  We can now start examining those gaze points.
*/


void MapBuilder::processEvent(const EventBase &e) {
  if ( curReq == NULL) return;
  if ( verbosity & MBVevents )
    cout << "MapBuilder got event " << e.getName() << ";   current pointAtID=" << pointAtID
	 << " scanID=" << scanID << endl;

  switch ( e.getGeneratorID() ) {
  case EventBase::textmsgEGID:
    if ( strcmp(dynamic_cast<const TextMsgEvent&>(e).getText().c_str(),"MoveHead") == 0 )
      moveToNextGazePoint(true);
    break;

  case EventBase::lookoutEGID:
    if ( e.getSourceID() == pointAtID )
      processImage(dynamic_cast<const LookoutSketchEvent&>(e));
    else if ( e.getSourceID() == scanID ) {
      const vector<Point>& pts = dynamic_cast<const LookoutScanEvent*>(&e)->getTasks().front()->data;
      cout << " doScan found " << pts.size() << " interest points." << endl;
      curReq->gazePts.insert(curReq->gazePts.end(),pts.begin(), pts.end());
    }
    else {
      cout << "MapBuilder: unexpected Lookout event " << e.getDescription()
	   << ",   current pointAtID=" << pointAtID << ", scanID=" << scanID << endl;
      return;
    }
    // we've dealt with the event (processed an image or did a scan); now see what we should do next
    if ( requestExitTest() )
      requestComplete();
    else if ( curReq->worldTargets.size() > 0 )
      doNextSearch();
    else if ( determineNextGazePoint() )
      moveToNextGazePoint();
    else
      requestComplete();
    break;

  default:
    cout << "MapBuilder received unexpected event: " << e.getDescription() << endl;
  }
}

void MapBuilder::processImage(const LookoutSketchEvent &e) {
  camSkS.clear();
  if ( curReq->rawY ) {
    NEW_SKETCH(rawY, uchar, VRmixin::sketchFromRawY());
  }
  NEW_SKETCH(camFrame, uchar, e.getSketch());
  const NEWMAT::Matrix& camToBase = e.toBaseMatrix;
  const NEWMAT::Matrix baseToCam = camToBase.i();
  getCameraShapes(camFrame);
  if (curReq->userCamProcessing != NULL) (*curReq->userCamProcessing)();
  if (curReq->getRequestType() > MapBuilderRequest::cameraMap) {
    projectToGround(camToBase);
    if (curReq->userGroundProcessing != NULL) (*curReq->userGroundProcessing)();
    filterGroundShapes(baseToCam);
  }
  switch ( curReq->getRequestType() ) {
  case MapBuilderRequest::cameraMap:
  case MapBuilderRequest::groundMap:
    break;
  case MapBuilderRequest::localMap:
    extendLocal(baseToCam);
    if (curReq->userLocalProcessing != NULL) (*curReq->userLocalProcessing)();
    break;
  case MapBuilderRequest::worldMap:
    extendWorld(baseToCam);
    if (curReq->userWorldProcessing != NULL) (*curReq->userWorldProcessing)();
  }
}

bool MapBuilder::determineNextGazePoint() {
  if (curReq->getRequestType() == MapBuilderRequest::worldMap) {
    worldShS.applyTransform(worldToLocalTranslateMatrix,egocentric);
    worldShS.applyTransform(worldToLocalRotateMatrix,egocentric);
    bool b = determineNextGazePoint(worldShS.allShapes()) || determineNextGazePoint(curReq->gazePts);
    worldShS.applyTransform(localToWorldMatrix,allocentric); // transform back
    return b;
  }
  else
    return determineNextGazePoint(localShS.allShapes()) || determineNextGazePoint(curReq->gazePts);
}

  
bool MapBuilder::determineNextGazePoint(const vector<ShapeRoot>& shapes) {
  if ( ! curReq->pursueShapes ) return false;
  HeadPointerMC headpointer_mc;
  for (vector<ShapeRoot>::const_iterator it = shapes.begin();
       it != shapes.end(); it++) {
    // look for invalid endpoints of lines / polygons
    if ((*it)->isType(lineDataType) || (*it)->isType(polygonDataType)) {
      const Shape<LineData>& ld = ShapeRootTypeConst(*it,LineData);
      const Shape<PolygonData>& pd = ShapeRootTypeConst(*it,PolygonData);
      bool isLine = (*it)->isType(lineDataType);
      EndPoint p[2] = { isLine ? ld->end1Pt(): pd->end1Pt(), isLine ? ld->end2Pt() : pd->end2Pt()};
      for (int i = 0; i < 2; i++) {
	if ( !p[i].isValid() && !isBadGazePoint(p[i] )
	    && badGazePoints.end() == find(badGazePoints.begin(), badGazePoints.end(), (Point) p[i])) {
	  cout << "Take image at endpoint" << (i+1) << " of shape id " 
	       << (*it)->getId() << " at " << p[i] << endl;
	  if ( !headpointer_mc.lookAtPoint(p[i].coordX(),p[i].coordY(),p[i].coordZ()) ) {
	    if ( verbosity & MBVbadGazePoint )
	      cout << "MapBuilder noting unreachable gaze point " << (Point)p[i] << endl;
	    badGazePoints.push_back((Point)p[i]);
	  }
	  nextGazePoint = p[i];
	  return true;
	}
      }
    }
    // look for shapes w/ <2 confidence
    if ((!(*it)->isType(agentDataType)) &&
	(*it)->getLastMatchId() != 0 &&
	(*it)->getConfidence() <= 1 &&
	! isBadGazePoint((*it)->getCentroid()) &&
	badGazePoints.end() == find(badGazePoints.begin(), badGazePoints.end(), (*it)->getCentroid()))  {
      const Point pt = (*it)->getCentroid();
      cout << "take image at shape " << (*it)
	   << " (confidence level: " << (*it)->getConfidence() << ")" << endl;      
      cout << " at " << pt << endl;  
      if (! headpointer_mc.lookAtPoint(pt.coordX(),pt.coordY(),pt.coordZ()))
	badGazePoints.push_back(pt);
      nextGazePoint = pt;
      return true;
    }
    // cout << "Skipping shape " << (*it)->getId()
    //      << " (confidence level: " << (*it)->getConfidence() << ")" << endl;      
  }
  return false;
}


bool MapBuilder::determineNextGazePoint(vector<Point>& gazePts) {
  for (vector<Point>::iterator it = gazePts.begin();
       it != gazePts.end(); it++) {
    nextGazePoint = *it;
    gazePts.erase(it);
    if ( ! isBadGazePoint(nextGazePoint) )
	 return true;
  }
  return false;
}

void MapBuilder::moveToNextGazePoint(const bool manualOverride) {
  if ( curReq == NULL ) {
    cout << "curReq == NULL in moveToNextGazePoint!" << endl;
    return;
  }
  if ( (verbosity & MBVnextGazePoint) || (curReq->manualHeadMotion && manualOverride==false) )
    cout << "moveToNextGazePoint " << nextGazePoint << endl;
  if ( curReq->manualHeadMotion && manualOverride==false ) {
    cout << "To proceed to this gaze point:  !msg MoveHead" << endl;
    return;
  }
  else
    storeImage(nextGazePoint,true);
}


void MapBuilder::doNextSearch() {
  LookoutSearchRequest *curLSR = curReq->worldTargets.front();
  curReq->worldTargets.pop();
  pointAtID = VRmixin::lookout.executeRequest(*curLSR);
}

bool MapBuilder::isBadGazePoint(const Point& Pt) const {
  const coordinate_t x = Pt.coordX();
  const coordinate_t y = Pt.coordY();
  return ( x*x + y*y > maxDistSq || x < -30.0);
}

void MapBuilder::storeImage(const Point& pt, bool havepoint) { 
  LookoutPointRequest lreq;
  if ( havepoint )
    lreq.setTarget(pt);
  else
    lreq.setHeadMotionType(LookoutRequest::noMotion);
  lreq.motionSettleTime = curReq->motionSettleTime;
  lreq.numSamples = curReq->numSamples;
  lreq.sampleInterval = curReq->sampleInterval;
  pointAtID = VRmixin::lookout.executeRequest(lreq);
}

void MapBuilder::grabCameraImageAndGo() {
  // This is a performance hack to avoid calling the Lookout or event
  // router, so the MapBuilder can produce results very quickly when
  // we need real-time performance, e.g., for particle filtering where
  // we take multiple snapshots.
  pointAtID = 0;
  Sketch<uchar> camFrame(VRmixin::sketchFromSeg());
#ifdef TGT_HAS_CAMERA
	const NEWMAT::Matrix camToBase = kine->jointToBase(CameraFrameOffset);
#else
	NEWMAT::Matrix camToBase(4,4);
	camToBase << ROBOOP::fourbyfourident;
#endif
  LookoutSketchEvent dummy(true, camFrame, camToBase,
			   EventBase::lookoutEGID, pointAtID, EventBase::deactivateETID);
  processImage(dummy);
  requestComplete();
}

void MapBuilder::scanForGazePts() {
  LookoutScanRequest lreq;
  lreq.searchArea = curReq->searchArea;
  lreq.motionSettleTime = curReq->motionSettleTime;
  set<color_index> colors;  // colors of all the shape types we're looking for
  for (map<ShapeType_t,set<color_index> >::const_iterator it1 = curReq->objectColors.begin();
       it1 != curReq->objectColors.end(); it1++)
    for (set<color_index>::const_iterator it2 = it1->second.begin();
	 it2 != it1->second.end(); it2++)
      colors.insert(*it2);
  lreq.addTask(LookoutScanRequest::VisionRegionTask(colors,curReq->dTheta));
  scanID = VRmixin::lookout.executeRequest(lreq);
}

void MapBuilder::extendLocal(const NEWMAT::Matrix& baseToCam) {
  vector<ShapeRoot> all = localShS.allShapes();
  removeNoise(localShS, baseToCam);
  matchSrcToDst(groundShS,localShS,curReq->objectColors[polygonDataType]);
  removeGazePts(curReq->gazePts, baseToCam);
  curReq->baseToCamMats.push_back(baseToCam);
}

void MapBuilder::extendWorld(const NEWMAT::Matrix& baseToCam) {
  worldShS.applyTransform(worldToLocalTranslateMatrix,egocentric);
  worldShS.applyTransform(worldToLocalRotateMatrix,egocentric);
  removeNoise(worldShS, baseToCam);
  matchSrcToDst(groundShS,worldShS,curReq->objectColors[polygonDataType]);
  worldShS.applyTransform(localToWorldMatrix,allocentric);
  removeGazePts(curReq->gazePts,baseToCam);
  curReq->baseToCamMats.push_back(baseToCam);
}

bool MapBuilder::requestExitTest() {
  if ( curReq->exitTest == NULL )
    return false;
  else
    return (*curReq->exitTest)();
}

void MapBuilder::requestComplete() {
  const unsigned int reqID = curReq->requestID;
  if ( verbosity & MBVcomplete )
    cout << "MapBuilderRequest " << reqID << " complete\n";
  delete curReq;
  curReq = NULL;
  requests.pop();
  erouter->postEvent(EventBase::mapbuilderEGID, reqID, EventBase::deactivateETID,0);
  if ( requests.empty() )
    VRmixin::lookout.relax();
  else
    executeRequest(); // execute next request AFTER deactivate event has finished processing
}

void MapBuilder::setAgent(const Point &worldLocation, const AngTwoPi worldHeading) {
  if ( verbosity & MBVsetAgent )
    cout << "Agent now at " << worldLocation << " hdg " << worldHeading 
	 << " (= " << float(worldHeading)*180/M_PI << " deg.)" << endl;
  theAgent->setCentroidPt(worldLocation);
  theAgent->setOrientation(worldHeading);
  const coordinate_t dx = worldLocation.coordX();
  const coordinate_t dy = worldLocation.coordY();
  const coordinate_t dz = worldLocation.coordZ();
  float const c = cos(worldHeading);
  float const s = sin(worldHeading);
  float localToWorld[] = {c, -s, 0, dx,
			  s,  c, 0, dy, 
			  0,  0, 1, dz,
			  0 , 0, 0, 1};
  float worldToLocalTrans[] = {1, 0, 0, -dx,
			       0, 1, 0, -dy, 
			       0, 0, 1, -dz,
			       0 ,0, 0,  1};
  float worldToLocalRotate[] = { c, s, 0, 0,
				-s, c, 0, 0, 
				 0, 0, 1, 0,
				 0, 0, 0, 1};
  localToWorldMatrix << localToWorld;
  worldToLocalTranslateMatrix << worldToLocalTrans;
  worldToLocalRotateMatrix << worldToLocalRotate;
}

void MapBuilder::moveAgent(coordinate_t const local_dx, coordinate_t const local_dy, AngTwoPi dtheta) {
  Point const agentLoc = theAgent->getCentroid();
  AngTwoPi const heading = theAgent->getOrientation();
  float const c = cos(heading);
  float const s = sin(heading);
  float const dx = local_dx*c + local_dy*-s;
  float const dy = local_dx*s + local_dy*c;
  setAgent(agentLoc + Point(dx,dy,agentLoc.coordZ(),allocentric),
	   heading+dtheta);
}

void MapBuilder::importLocalToWorld() {
  worldShS.applyTransform(worldToLocalTranslateMatrix,egocentric);
  worldShS.applyTransform(worldToLocalRotateMatrix,egocentric);
  matchSrcToDst(localShS,worldShS);
  worldShS.applyTransform(localToWorldMatrix,allocentric);
}


ShapeRoot MapBuilder::importWorldToLocal(const ShapeRoot &worldShape) {
  ShapeRoot localShape(localShS.importShape(worldShape));
  localShape->applyTransform(worldToLocalTranslateMatrix,egocentric);
  localShape->applyTransform(worldToLocalRotateMatrix,egocentric);
  return localShape;
}

bool MapBuilder::isPointVisible(const Point &pt, const NEWMAT::Matrix& baseToCam, float maxDistanceSq) {
  NEWMAT::ColumnVector camCoords = baseToCam*pt.coords;
  //  if (camCoords(3) <=0 || distSq(camCoords) >= maxDistanceSq) return false;
  if ( camCoords(1)*camCoords(1)+camCoords(2)*camCoords(2) >= maxDistanceSq )
    return false;
  float normX,normY; // normalized coordinates in cam frame
  config->vision.computePixel(camCoords(1),camCoords(2),camCoords(3),normX,normY);
  return (fabs(normX) < 0.9 && fabs(normY) < 0.9); //normX and normY range from -1 to 1. Giving 10% offset here
}

bool MapBuilder::isLineVisible(const LineData& ln, const NEWMAT::Matrix& baseToCam) {
  float normX1,normX2,normY1,normY2;
  NEWMAT::ColumnVector camCoords(4);
  camCoords = baseToCam*ln.end1Pt().coords;
  config->vision.computePixel(camCoords(1),camCoords(2),camCoords(3),normX1,normY1);
  camCoords = baseToCam*ln.end2Pt().coords;
  config->vision.computePixel(camCoords(1),camCoords(2),camCoords(3),normX2,normY2);
  const bool end1Pt_visible = fabs(normX1) < 0.9 && fabs(normY1) < 0.9;
  const bool end2Pt_visible = fabs(normX2) < 0.9 && fabs(normY2) < 0.9;
  if (end1Pt_visible && end2Pt_visible)
    return true;
  LineData lnCam(VRmixin::groundShS, Point(normX1,normY1), Point(normX2,normY2));
  // define bounding box of camera frame in terms of normalized coordinates with 10% offset
  LineData camBounds[] = {LineData(VRmixin::groundShS, Point(0.9,0.9),Point(0.9,-0.9)),
			  LineData(VRmixin::groundShS, Point(0.9,-0.9),Point(-0.9,-0.9)),
			  LineData(VRmixin::groundShS, Point(-0.9,-0.9),Point(-0.9,0.9)),
			  LineData(VRmixin::groundShS, Point(-0.9,0.9),Point(0.9,0.9))};
  unsigned int ptCount = 0;
  Point p[2];
  // find if a portion of the line shows up in cam
  if (end1Pt_visible) p[ptCount++].setCoords(normX1,normY1,0); // end1Pt in frame
  else if (end2Pt_visible) p[ptCount++].setCoords(normX2,normY2,0); // end2Pt in frame
  for (int i = 0; i < 4; i++)
    if (camBounds[i].intersectsLine(lnCam)) {
      p[ptCount++].setCoords(lnCam.intersectionWithLine(camBounds[i]));
      // Let's say portion of line seen in cam should be longer than .1 normalized
      if (ptCount > 1)
	return p[0].distanceFrom(p[1]) > 0.1; 
    }
  return false;
}

bool MapBuilder::isShapeVisible(const ShapeRoot &ground_shape, const NEWMAT::Matrix& baseToCam,
				   float maxDistanceSq) {
  if (ground_shape->isType(lineDataType))
    return isLineVisible(ShapeRootTypeConst(ground_shape,LineData).getData(), baseToCam);
  else if (ground_shape->isType(polygonDataType)) {
    const Shape<PolygonData>& polygon = ShapeRootTypeConst(ground_shape,PolygonData);
    for (vector<LineData>::const_iterator edge_it = polygon->getEdges().begin();
	 edge_it != polygon->getEdges().end(); edge_it++)
      if (isLineVisible(*edge_it,baseToCam))
	return true;
    return false;
  }
  else 
    return isPointVisible(ground_shape->getCentroid(), baseToCam, maxDistanceSq);
}


// filter "bad" ground shapes before importing to dst shape space.
// 1. ignore shapes too far from dog or projected to the other side of cam plane
// 2. chop off line at max distance if it is extending beyond the distance and leave the endpoint invalid
void MapBuilder::filterGroundShapes(const NEWMAT::Matrix& baseToCam) {
  //  cout << "MapBuilder::filterGroundShapes()" << endl;
  vector<ShapeRoot> ground_shapes = groundShS.allShapes();

  for (vector<ShapeRoot>::iterator ground_it = ground_shapes.begin();
       ground_it != ground_shapes.end(); ground_it++ ) {
    const coordinate_t cenX = (*ground_it)->getCentroid().coordX();
    const coordinate_t cenY = (*ground_it)->getCentroid().coordY();
    if (cenX*cenX + cenY*cenY > maxDistSq) { // too far
      if ( verbosity & MBVnotAdmissible )
	cout << "ground shape " << (*ground_it)->getId() << " (lastMatch " 
	     << (*ground_it)->getLastMatchId() << ") too far, delete\n";
      ground_it->deleteShape();
    }
    NEWMAT::ColumnVector coords = Kinematics::pack(cenX,cenY,(*ground_it)->getCentroid().coordZ());
    coords = baseToCam*coords;
    if (coords(3) < 0) { // negative z-coordinate in camera frame indicates projection failed
      if ( verbosity & MBVprojectionFailed )
	cout << "Projection failed for ground shape " << (*ground_it)->getId() 
	     << " (lastMatch " << (*ground_it)->getLastMatchId() << "): deleting\n";
      ground_it->deleteShape();
    }
    // if a line is extending to maxDistance, chop it off at maxdistance and mark the endpoint invalid
    else if ((*ground_it)->isType(lineDataType)) {
      const Shape<LineData>& line = ShapeRootTypeConst(*ground_it,LineData);
      const coordinate_t e1x = line->end1Pt().coordX();
      const coordinate_t e1y = line->end1Pt().coordY();
      const coordinate_t e2x = line->end2Pt().coordX();
      const coordinate_t e2y = line->end2Pt().coordY();
      if (e1x*e1x + e1y*e1y > maxDistSq && e2x*e2x + e2y*e2y > maxDistSq)
	ground_it->deleteShape();
      else if (e1x*e1x + e1y*e1y > maxDistSq || e2x*e2x + e2y*e2y > maxDistSq) {
	//	cout << (*ground_it)->getId() << "(lastMatch " << (*ground_it)->getLastMatchId() 
	//	     << ")  extends beyond maximum distance we want. Chop off the line" << endl;
	vector<float> line_abc = line->lineEquation_abc();
	Point pt;
	const EndPoint* far_ept = (e1x*e1x + e1y*e1y > maxDistSq) ? &line->end1Pt() : &line->end2Pt(); 
      	if (line_abc[1] == 0.0) {
	  const coordinate_t y_abs = sqrt(maxDistSq - line_abc[2]*line_abc[2]);
	  if (fabs(far_ept->coordY()-y_abs) < fabs(far_ept->coordY()+y_abs))
	    pt.setCoords(e1x, y_abs, far_ept->coordZ());
	  else
	    pt.setCoords(e1x, -y_abs, far_ept->coordZ());
	}
	else {
	  const float a = - line_abc[0]/line_abc[1];
	  const float b = line_abc[2]/line_abc[1];
	  const coordinate_t x1 = (sqrt((a*a+1.0)*maxDistSq-b*b)-a*b)/(a*a+1.0);
	  const coordinate_t x2 = (-sqrt((a*a+1.0)*maxDistSq-b*b)-a*b)/(a*a+1.0);
	  if (fabs(far_ept->coordX()-x1) < fabs(far_ept->coordX()-x2))
	    pt.setCoords(x1, a*x1+b, far_ept->coordZ());
	  else
	    pt.setCoords(x2, a*x2+b, far_ept->coordZ());
	}
	EndPoint temp_endPt(pt);
	temp_endPt.setValid(false);
	//	cout << " (" << far_ept->coordX() << "," << far_ept->coordY() << ") => ("
	//	     << pt.coordX() << "," << pt.coordY() << ")" << endl;
	if (e1x*e1x + e1y*e1y > maxDistSq)
	  line->setEndPts(temp_endPt, line->end2Pt());
	else
	  line->setEndPts(line->end1Pt(), temp_endPt);
	badGazePoints.push_back(pt);
      }
    }
  }
}

void MapBuilder::calculateGroundPlane() {
  switch(curReq->groundPlaneAssumption) {
  case MapBuilderRequest::onLegs:
    ground_plane << kine->calculateGroundPlane(); 
    if ( verbosity & MBVgroundPlane ) 
      cout << "Calculated ground plane: " << NEWMAT::printmat(ground_plane) << endl;
    break;
  case MapBuilderRequest::onStand:
#ifdef TGT_ERS210
    ground_plane << 0 << 0 << (float)(-1/200.0) << 1;
#else
    ground_plane << 0 << 0 << (float)(-1/170.0) << 1; //for the order-made stands for ERS7 in the lab
#endif
    // cout << "Hard-coded ground plane: " << NEWMAT::printmat(ground_plane) << endl;
    break;
  case MapBuilderRequest::custom:
    ground_plane = curReq->customGroundPlane;
  }
}

void MapBuilder::projectToGround(const NEWMAT::Matrix& camToBase) {
  VRmixin::projectToGround(camToBase, ground_plane);
}
    

void MapBuilder::matchSrcToDst(ShapeSpace &srcShS, ShapeSpace &dstShS,
				  set<color_index> polCols, bool mergeSrc, bool mergeDst) {
  vector<ShapeRoot> src_shapes = srcShS.allShapes();
  vector<ShapeRoot> dst_shapes = dstShS.allShapes();
  vector<LineData> polygon_edges;
  
  // clean up src_shapes before messing with dst space
  for (vector<ShapeRoot>::iterator src_it = src_shapes.begin();
       src_it != src_shapes.end(); src_it++ ) {
    if (!(*src_it)->isAdmissible()) {
      if ( verbosity & MBVnotAdmissible )
	cout << "shape " << (*src_it)->getId() << "(lastMatch " 
	     << (*src_it)->getLastMatchId() << ") is not admissible:" << endl;
      (*src_it)->printParams();
      src_shapes.erase(src_it--);
    }
    else if ((*src_it)->isType(polygonDataType)) { 
      const vector<LineData>& lines = ShapeRootTypeConst(*src_it, PolygonData)->getEdges();
      polygon_edges.insert(polygon_edges.end(), lines.begin(), lines.end());
      src_shapes.erase(src_it--);
    }
    else if ((*src_it)->isType(lineDataType)) {
      const color_index colorIndex = ProjectInterface::getColorIndex((*src_it)->getColor());
      if ( polCols.end() != find(polCols.begin(), polCols.end(), colorIndex)) {
	polygon_edges.push_back(ShapeRootTypeConst(*src_it, LineData).getData());
	src_shapes.erase(src_it--);
      }
    }
  }

  // merge shapes inside srcShS
  if (mergeSrc)
    for ( vector<ShapeRoot>::iterator it = src_shapes.begin();
	  it != src_shapes.end(); it++ )
      for ( vector<ShapeRoot>::iterator it2 = it+1;
	  it2 != src_shapes.end(); it2++)
	if ((*it2)->isMatchFor(*it) && (*it)->updateParams(*it2)) {
	  if ( verbosity & MBVshapesMerge )
	    cout << "merging shape " << (*it)->getId() << " and shape " << (*it2)->getId() << endl;
	  src_shapes.erase(it2--);
	}

  vector<Shape<PolygonData> > existingPolygons;
  // update dst shapes from src shapes
  for (vector<ShapeRoot>::iterator dst_it = dst_shapes.begin();
       dst_it != dst_shapes.end(); dst_it++ )
    if ((*dst_it)->isType(polygonDataType))
      existingPolygons.push_back(ShapeRootType(*dst_it,PolygonData));
    else {
      for (vector<ShapeRoot>::iterator src_it = src_shapes.begin();
	   src_it != src_shapes.end(); src_it++)
	if ((*dst_it)->isMatchFor(*src_it) && (*dst_it)->updateParams((*src_it))) {
	  (*dst_it)->increaseConfidence(*src_it);
	  if ( verbosity & MBVshapeMatch )
	    cout << "Matched src shape " << (*src_it)->getId() << " (lastMatch " << (*src_it)->getLastMatchId()
		 << ") to dst shape " << (*dst_it)->getId() << endl;
	  src_shapes.erase(src_it--);
	}
    }
  
  // form polygons from lines and import unmatched src shapes into dstShS
  vector<ShapeRoot> deletedPolygons;
  //  cout << existingPolygons.size() << " polygons\n";
  vector<ShapeRoot> newPolygons = PolygonData::formPolygons(polygon_edges,existingPolygons,deletedPolygons);
  //  cout << existingPolygons.size()+deletedPolygons.size() << " polygons\n";
  for (vector<ShapeRoot>::iterator delete_it = deletedPolygons.begin();
       delete_it != deletedPolygons.end(); delete_it++)
    delete_it->deleteShape();
  dstShS.importShapes(newPolygons);
  dstShS.importShapes(src_shapes);
  if ( verbosity & MBVimportShapes )
    cout << "Imported " << (src_shapes.size()+newPolygons.size()) << " new shape(s) from "
	 << srcShS.name << "ShS to " << dstShS.name << "ShS\n";
  
  // match shapes inside dstShS
  if (mergeDst) {
    dst_shapes = dstShS.allShapes();
    for ( vector<ShapeRoot>::iterator it = dst_shapes.begin();
	  it < dst_shapes.end(); it++ )
      for ( vector<ShapeRoot>::iterator it2 = it+1;
	    it2 < dst_shapes.end(); it2++)
	if ((*it2)->isMatchFor(*it) && (*it)->updateParams(*it2,true)) {
	  // cout << "Matched src shape " << (*it)->getId() << " (lastMatch " 
	  //  << (*it)->getLastMatchId()<< ") is a match for " 
	  //   << (*it2)->getId() << " (lastMatch " 
	  //   << (*it2)->getLastMatchId() << "), delete\n";
	  it2->deleteShape();
	  dst_shapes.erase(it2--);
	}
  }
}

// decrease confidence of those shapes that should have been seen in the last snap,
// remove shapes from ShapeSpace if confidence becomes < 0
void MapBuilder::removeNoise(ShapeSpace& ShS, const NEWMAT::Matrix& baseToCam) {
  //  cout << "MapBuilder::removeNoise()\n";
  vector<ShapeRoot> shapes = ShS.allShapes();
  for (vector<ShapeRoot>::iterator it = shapes.begin();
       it != shapes.end(); it++ ) {
    // was not looking for this object in the last snap, 
    // and it's not fair to decrease this guy's confidence
    if (curReq->objectColors[(*it)->getType()].find(ProjectInterface::getColorIndex((*it)->getColor())) ==
	curReq->objectColors[(*it)->getType()].end())
      continue; 
    if ((*it)->isType(polygonDataType)) {
      Shape<PolygonData>& polygon = ShapeRootType(*it,PolygonData);
      vector<LineData>& edges = polygon->getEdgesRW();
      unsigned int edge_index = 0;
      for (vector<LineData>::iterator edge_it = edges.begin();
	   edge_it != edges.end(); edge_it++, edge_index++) {
	if (isLineVisible(*edge_it, baseToCam)) {
	  if ( verbosity & MBVshouldSee )
	    cout << "edge " << edge_index << " of polygon " << (*it)->getId() << "(confidence: " 
		 << edge_it->getConfidence() << ") should be visible in this frame" << endl;
	  edge_it->decreaseConfidence();
	}
      }
      vector<ShapeRoot> brokenPolygons = polygon->updateState();
      ShS.importShapes(brokenPolygons);
      if (!polygon->isAdmissible()) {
	if ( verbosity & MBVdeleteShape )
	  cout << "delete polygon " << (*it)->getId() << " from map" << endl;
	it->deleteShape();
      }
    }
    else if ((!(*it)->isType(agentDataType)) && isShapeVisible(*it, baseToCam, maxDistSq)){
      (*it)->decreaseConfidence(); // decrease confidence by 1 for every visible shape
      if ((*it)->getConfidence() < 0 ) {
	if ( verbosity & MBVshouldSee )
	  cout << "shape " << (*it)->getId() << "(confidence: " << (*it)->getConfidence() 
	       << ") should be visible in this frame; deleted" << endl;
	it->deleteShape();
      }
    }
  }
}

//================ Gaze points ================

void MapBuilder::defineGazePts() {
  const ShapeRoot &sArea = curReq->searchArea;
  if ( !sArea.isValid() )
    return;
  else if ( curReq->doScan == true )
    scanForGazePts();
  else
    switch ( sArea->getType() ) {

    case pointDataType:
      curReq->gazePts.push_back(sArea->getCentroid());
      break;

    case lineDataType: {
      static const float meshSize=50;
      const Shape<LineData>& line = ShapeRootTypeConst(sArea,LineData);
      if ( curReq->doScan == true )
	scanForGazePts();
      else {
	int numIntervals = (int) (line->getLength()/meshSize);
	const EndPoint& ep1 = line->end1Pt();
	const EndPoint& ep2 = line->end2Pt();
	curReq->gazePts.push_back(ep1);
	for (int i = 1; i < numIntervals; i++)
	  curReq->gazePts.push_back((ep1*i + ep2*(numIntervals-i))/numIntervals);
	curReq->gazePts.push_back(ep2);
      }
    }
      break;

    case polygonDataType:
      if ( curReq->doScan == true )
	scanForGazePts();
      else {
	const Shape<PolygonData> &poly = ShapeRootTypeConst(sArea,PolygonData);
	const vector<Point> &verts = poly->getVertices();
	curReq->gazePts.insert(curReq->gazePts.end(),verts.begin(),verts.end());
      }
      break;

      default:
	cerr << "ERROR MapBuilder::defineGazePts: Supported searchArea shapes are Point, Line, and Polygon\n";
	requestComplete();
	break;
    }
}


void MapBuilder::removeGazePts(vector<Point> &gazePts, const NEWMAT::Matrix& baseToCam) {
  if (curReq->removePts) {
    int num_points_seen = 0;
    for ( vector<Point>::iterator it = gazePts.begin();
	  it != gazePts.end(); it++ ) {
      if ( isPointVisible(*it,baseToCam,maxDistSq) ) {
	cout << "Removing already-visible gaze point " << *it << endl;
	num_points_seen++;
	gazePts.erase(it--);
      }
    }
    // cout << num_points_seen << " pre-defined gaze points seen in last image, "
    //      << gazePts.size() << " left\n";
  }
}


//================================================================

//Prints shapespace in the format to be used for particle filter on simulator
void MapBuilder::printShS(ShapeSpace &ShS) const {
  cout << "MapBuilder::printShS()" << endl;
  unsigned int line_count = 0;
  vector<ShapeRoot> shapes = ShS.allShapes();
  for (vector<ShapeRoot>::const_iterator it = shapes.begin();
       it != shapes.end(); it++) {
    if ((*it)->isType(lineDataType)) {
      const Shape<LineData>& ld = ShapeRootTypeConst(*it,LineData);
      cout << (*it)->getId() << " " << lineDataType << " " 
	   << ProjectInterface::getColorIndex((*it)->getColor()) 
	   << " " << ld->end1Pt().coordX()  << " " << ld->end1Pt().coordY()
	   << " " << ++line_count << " " << ld->getLength() << " " << ld->end1Pt().isValid() << endl; 
      cout << (*it)->getId() << " " << lineDataType << " " 
	   << ProjectInterface::getColorIndex((*it)->getColor()) 
	   << " " << ld->end2Pt().coordX()  << " " << ld->end2Pt().coordY()
	   << " " << line_count << " " << ld->getLength() << " " << ld->end2Pt().isValid() << endl;
    }
    else {
      cout << (*it)->getId() << " " << (*it)->getType() << " " 
	   << ProjectInterface::getColorIndex((*it)->getColor()) 
	   << " " << (*it)->getCentroid().coordX()  << " " << (*it)->getCentroid().coordY() << endl;
    }
  }
}


//================ Shape extraction ================

void MapBuilder::getCameraShapes(const Sketch<uchar>& camFrame) { 
  camShS.clear();
  getCamLines(camFrame, curReq->objectColors[lineDataType], curReq->occluderColors[lineDataType]);
  getCamEllipses(camFrame, curReq->objectColors[ellipseDataType], curReq->occluderColors[ellipseDataType]);
  getCamPolygons(camFrame, curReq->objectColors[polygonDataType], curReq->occluderColors[polygonDataType]);
  getCamSpheres(camFrame, curReq->objectColors[sphereDataType], curReq->occluderColors[sphereDataType]);
  getCamWalls(camFrame, curReq->floorColor);
  if ( curReq->numSamples == 1 && !curReq->searchArea.isValid() )  // for fast blob extraction from current camera frame
    getCamBlobs();
  else
    getCamBlobs(camFrame, curReq->objectColors[blobDataType], curReq->minBlobAreas, curReq->blobOrientations, curReq->assumedBlobHeights);
  getCamTargets(camFrame, curReq->objectColors[targetDataType], curReq->occluderColors[targetDataType]);
}

vector<Shape<LineData> > MapBuilder::getCamLines(const Sketch<uchar> &camFrame, const set<color_index>& objectColors, 
						 const set<color_index>& occluderColors) const {
  vector<Shape<LineData> > linesReturned;
  if ( objectColors.empty() ) 
    return linesReturned;
  if ( verbosity & MBVshapeSearch )
    cout << "*** Find the lines ***" << endl;
  NEW_SKETCH_N(occluders,bool,visops::zeros(camFrame));
  for ( set<color_index>::const_iterator it = occluderColors.begin();
	it != occluderColors.end(); it++ )
    occluders |= visops::minArea(visops::colormask(camFrame,*it));

  for (set<color_index>::const_iterator it = objectColors.begin();
       it != objectColors.end(); it++) {
    NEW_SKETCH_N(colormask,bool,visops::colormask(camFrame,*it));
    NEW_SKETCH_N(cleancolor,bool,visops::minArea(colormask));
    NEW_SKETCH_N(fatmask,bool,visops::fillin(cleancolor,1,2,8));
    NEW_SKETCH_N(skel,bool,visops::skel(fatmask));
    vector<Shape<LineData> > line_shapes(LineData::extractLines(skel,cleancolor|occluders));
    linesReturned.insert(linesReturned.end(), line_shapes.begin(), line_shapes.end());
    if ( verbosity & MBVshapesFound )
      cout << "Found " << line_shapes.size() << " " 
	   << ProjectInterface::getColorName(*it) << " lines." << endl;
  }
  return linesReturned;
}

vector<Shape<EllipseData> > 
MapBuilder::getCamEllipses(const Sketch<uchar> &camFrame,
			   const set<color_index>& objectColors, const set<color_index>& ) const {
  vector<Shape<EllipseData> > ellipses;
  if (objectColors.empty())
    return ellipses;
  if ( verbosity & MBVshapeSearch )
    cout << "*** Find the ellipses ***" << endl;
  for (set<color_index>::const_iterator it = objectColors.begin();
       it !=objectColors.end(); it++) {
    NEW_SKETCH_N(colormask,bool,visops::colormask(camFrame,*it));
    vector<Shape<EllipseData> > ellipse_shapes = EllipseData::extractEllipses(colormask);
    ellipses.insert(ellipses.end(), ellipse_shapes.begin(),ellipse_shapes.begin());
    if ( verbosity & MBVshapesFound )
      cout << "Found " << ellipse_shapes.size() << " "
	   << ProjectInterface::getColorName(*it) << " ellipses." << endl;
  }
  return ellipses;
}

vector<Shape<PolygonData> > 
MapBuilder::getCamPolygons(const Sketch<uchar> &camFrame,
			      const set<color_index>& objectColors, const set<color_index>& occluderColors) const {
  vector<Shape<PolygonData> > polygons;
  if ( objectColors.empty() ) 
    return polygons;
  if ( verbosity & MBVshapeSearch )
    cout << "*** Find the polygons ***" << endl;
  NEW_SKETCH_N(occluders,bool,visops::zeros(camFrame));
  for ( set<color_index>::const_iterator it = occluderColors.begin();
	it !=occluderColors.end(); it++ )
    occluders |= visops::colormask(camFrame,*it);
  
  for (set<color_index>::const_iterator it = objectColors.begin();
       it !=objectColors.end(); it++) {
    NEW_SKETCH_N(colormask,bool,visops::colormask(camFrame,*it));
    NEW_SKETCH_N(fatmask,bool,visops::fillin(colormask,1,2,8));
    NEW_SKETCH_N(skel,bool,visops::skel(fatmask));
    NEW_SKETCH_N(fatskel,bool,visops::fillin(skel,1,2,8));
    
    vector<Shape<LineData> > polygon_lines = PolygonData::extractPolygonEdges(fatskel,fatmask|occluders);
    polygons.insert(polygons.end(), polygon_lines.begin(), polygon_lines.end());
    if ( verbosity & MBVshapesFound )
      cout << "Found " << polygon_lines.size() << " lines." << endl;
  }
  return polygons;
}

vector<Shape<SphereData> > 
MapBuilder::getCamSpheres(const Sketch<uchar> &camFrame,
			  const set<color_index>& objectColors, const set<color_index>& ) const {
  vector<Shape<SphereData> > spheres;
  if ( objectColors.empty() )
    return spheres;
  if ( verbosity & MBVshapeSearch )
    cout << "*** Find the spheres ***" << endl;
  for (set<color_index>::const_iterator it = objectColors.begin();
       it !=objectColors.end(); it++) {
    NEW_SKETCH_N(colormask,bool,visops::colormask(camFrame,*it));
    vector<Shape<SphereData> > sphere_shapes = SphereData::extractSpheres(colormask);
    spheres.insert(spheres.end(), spheres.begin(), spheres.end());
    if ( verbosity & MBVshapesFound )
      cout << "Found " << sphere_shapes.size() << " spheres." << endl;
  }
  return spheres;
}

vector<Shape<LineData> > 
MapBuilder::getCamWalls(const Sketch<uchar> &camFrame, unsigned int floorColor) const {
  if (floorColor == 0)
    return vector<Shape<LineData> >();
  if ( verbosity & MBVshapeSearch )
    cout << "*** Find the walls ***" << endl;
  const int camFrame_offset = 8;

  NEW_SKETCH_N(colormask,bool,visops::colormask(camFrame,floorColor));
  NEW_SKETCH_N(fillinmask ,bool,visops::fillin(colormask, 1, 6, 8)); //remove pixels w/ connectivity<6 (noise)
  NEW_SKETCH_N(fillinmask2 ,bool,visops::fillin(fillinmask, 2, 3, 8)); //remove pixels w/ connectivity<3 and fill in the others
  NEW_SKETCH_N(edgemask ,bool,visops::fillin(fillinmask2, 1, 5, 7)); //remove pixels w/ connectivity=8 (non-edge pixels)
  NEW_SKETCH_N(edgemask2 ,bool,visops::non_bounds(edgemask, camFrame_offset)); //remove pixels close to cam_bound

  NEW_SKETCH_N(occluders_floor, bool, camFrame != uchar(0) & camFrame != uchar(floorColor));
  NEW_SKETCH_N(occ_mask ,bool,visops::fillin(occluders_floor, 1, 8, 8)); //remove pixels w/ connectivity<7 (noises)
  usint const clear_dist = 15;
  Sketch<bool> not_too_close = (visops::edist(occ_mask) >= clear_dist); 
  edgemask2 &= not_too_close; //remove pixels around occuluders
  
  NEW_SKETCH_N(fatmask ,bool,visops::fillin(edgemask2,2,2,8)); //make the remaining pixels fat
  NEW_SKETCH_N(skel,bool,visops::skel(fatmask));
  NEW_SKETCH_N(fatskel,bool,visops::fillin(skel,1,2,8));
  
  vector<Shape<LineData> > wall_bounds = PolygonData::extractPolygonEdges(fatskel,fatmask|occluders_floor);

  // larger offset from the cam frame should be applied to these lines
  // since all pixels near cam frame bounds are removed before extracting these lines.
  for (vector<Shape<LineData> >::iterator it = wall_bounds.begin();
       it != wall_bounds.end(); it++) {
    if (((*it)->end1Pt().coordX() < camFrame_offset*2.0 || (*it)->end1Pt().coordX() > xres - camFrame_offset*2.0
	 || (*it)->end1Pt().coordY() < camFrame_offset*2.0 || (*it)->end1Pt().coordY() > yres - camFrame_offset*2.0)
	&& (*it)->end1Pt().isValid())
      (*it)->end1Pt().setValid(false);
    if (((*it)->end2Pt().coordX() < camFrame_offset*2.0 || (*it)->end2Pt().coordX() > xres - camFrame_offset*2.0
	 || (*it)->end2Pt().coordY() < camFrame_offset*2.0 || (*it)->end2Pt().coordY() > yres - camFrame_offset*2.0)
	&& (*it)->end2Pt().isValid())
      (*it)->end2Pt().setValid(false);
  }
  
  if ( verbosity & MBVshapesFound )
    cout << "Found " << wall_bounds.size() << " wall boundary lines" << endl;
  return wall_bounds;
}

void MapBuilder::getCamBlobs(const Sketch<uchar>& camFrame,
			     const set<color_index>& colors,
			     const map<color_index,int>& minBlobAreas,
			     const map<color_index, BlobData::BlobOrientation_t>& blobOrientations,
			     const map<color_index,coordinate_t>& assumedBlobHeights) {
  if ( colors.empty() )
    return;
  int const maxblobs = 50;
  vector<Shape<BlobData> > result(BlobData::extractBlobs(camFrame, colors, minBlobAreas, blobOrientations, assumedBlobHeights, maxblobs));
  if ( verbosity & MBVshapesFound )
    cout << "Found " << result.size() << " blobs." << endl;
}

// The code below grabs blobs directly from the region generator stream, instead of
// calling CMVision to do region extraction on camFrame 
void MapBuilder::getCamBlobs() {
  if (  curReq->objectColors[blobDataType].empty() )
    return;
  if ( verbosity & MBVshapeSearch )
    cout << "*** Find the blobs ***" << endl;
  const set<color_index>& blobColors = curReq->objectColors[blobDataType];
  for (set<color_index>::const_iterator it = blobColors.begin();
       it != blobColors.end(); it++) {
    int const minarea = (curReq->minBlobAreas.find(*it)==curReq->minBlobAreas.end()) ? 
      0 : curReq->minBlobAreas[*it];
    BlobData::BlobOrientation_t const orient = (curReq->blobOrientations.find(*it)==curReq->blobOrientations.end()) ? 
      BlobData::groundplane : curReq->blobOrientations[*it];
    coordinate_t const height = (curReq->assumedBlobHeights.find(*it)==curReq->assumedBlobHeights.end()) ? 
      0 : curReq->assumedBlobHeights[*it];
    vector<Shape<BlobData> > blob_shapes(VRmixin::getBlobsFromRegionGenerator(*it,minarea,orient,height));
    if ( verbosity & MBVshapesFound )
      cout << "Found " << blob_shapes.size() << " " << ProjectInterface::getColorName(*it) << " blobs." << endl;
  }
}

vector<Shape<TargetData> > 
MapBuilder::getCamTargets(const Sketch<uchar> &camFrame, const set<color_index>& objectColors, const set<color_index>& occluderColors) const {
  vector<Shape<TargetData> > targets;
  if (objectColors.empty())
    return targets;
  if ( verbosity & MBVshapeSearch )
    cout << "*** Find the targets ***" << endl;
  
  NEW_SKETCH_N(occluders,bool,visops::zeros(camFrame));
  for (set<color_index>::const_iterator it = occluderColors.begin();
       it != occluderColors.end(); it++)
    occluders |= visops::minArea(visops::colormask(camFrame,*it));
  
  // assumes multiples of 3 for objectColors (stays on the last color otherwise)
  for (set<color_index>::const_iterator it = objectColors.begin();
       it != objectColors.end(); it++) {
    NEW_SKETCH_N(front_colormask, bool, visops::colormask(camFrame,*it));
    it++;
    if (it == objectColors.end()) {
      it--;
    }
    NEW_SKETCH_N(back_colormask, bool, visops::colormask(camFrame,*it));
    it++;
    if (it == objectColors.end()) {
      it--;
    }
    NEW_SKETCH_N(right_colormask, bool, visops::colormask(camFrame,*it));
    Shape<TargetData> target = TargetData::extractLineTarget(front_colormask, back_colormask, right_colormask, occluders);
    if (target.isValid()) {
      targets.insert(targets.end(), target);
    }
    if ( verbosity & MBVshapesFound )
      cout << "Found " << (target.isValid() ? 1 : 0) << " targets." << endl;
  }
  
  return targets;
}

} // namespace
