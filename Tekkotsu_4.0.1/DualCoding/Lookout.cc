//-*-c++-*-

#include "Events/EventRouter.h"
#include "Events/LocomotionEvent.h"
#include "Events/LookoutEvents.h"
#include "Events/VisionObjectEvent.h"
#include "Shared/ProjectInterface.h"
#include "Shared/mathutils.h"
#include "Shared/WorldState.h"
#include "Vision/RegionGenerator.h"

#include "VRmixin.h"
#include "LookoutRequests.h"
#include "Lookout.h"
#include "MapBuilder.h"
#include "ShapeBlob.h"
#include "ShapeLine.h"
#include "ShapePolygon.h"

using namespace mathutils;
using namespace std;

namespace DualCoding {

Lookout::Lookout()
  : BehaviorBase("Lookout"),
    pixelHistograms(VRmixin::camSkS.getNumPixels()), distanceSamples(),
    pointer_id(MotionManager::invalid_MC_ID),
    posture_id(MotionManager::invalid_MC_ID),
    sequence_id(MotionManager::invalid_MC_ID),
    requests(), curReq(NULL), curPAR(NULL), successSave(false),
    trackerState(inactive), idCounter(0)
{}

void Lookout::DoStart() {
  BehaviorBase::DoStart();
  SharedObject<HeadPointerMC> head_mc;
  SharedObject<PostureMC> posture_mc;
  SharedObject<MediumMotionSequenceMC> mseq_mc;
  pointer_id = motman->addPersistentMotion(head_mc,MotionManager::kIgnoredPriority);
  posture_id = motman->addPersistentMotion(posture_mc,MotionManager::kIgnoredPriority);
  sequence_id = motman->addPersistentMotion(mseq_mc,MotionManager::kIgnoredPriority);
  cout << "Lookout starting up: pointer_id=" << pointer_id
       << " posture_id=" << posture_id
       << " sequence_id=" << sequence_id << endl;
  erouter->addListener(this,EventBase::motmanEGID,pointer_id,EventBase::statusETID);
  erouter->addListener(this,EventBase::motmanEGID,posture_id,EventBase::statusETID);
  erouter->addListener(this,EventBase::motmanEGID,sequence_id,EventBase::statusETID);
}

void Lookout::DoStop() {
  motman->removeMotion(pointer_id);
  pointer_id = MotionManager::invalid_MC_ID;
  motman->removeMotion(posture_id);
  posture_id = MotionManager::invalid_MC_ID;
  motman->removeMotion(sequence_id);
  sequence_id = MotionManager::invalid_MC_ID;
  curReq = NULL;
  curPAR = NULL;
  while (!requests.empty()) {
    delete requests.front();
    requests.pop();
  }
  BehaviorBase::DoStop();
}

vector<DualCoding::Point> Lookout::groundSearchPoints() {
  vector<Point> gazePts;
  gazePts.push_back(Point( 200, -250, -100, egocentric));
  gazePts.push_back(Point( 800,-1000, -100, egocentric));
  gazePts.push_back(Point(1200,    0, -100, egocentric));
  gazePts.push_back(Point( 800, 1000, -100, egocentric));
  gazePts.push_back(Point( 200,  250, -100, egocentric));
  gazePts.push_back(Point( 200,    0, -100, egocentric));
  gazePts.push_back(Point( 400,    0, -100, egocentric));
  gazePts.push_back(Point( 800,    0, -100, egocentric));
  return gazePts;
}

unsigned int Lookout::executeRequest(const LookoutRequest &req) {
  switch (req.getHeadMotionType()) {
  case LookoutRequest::noMotion:
  case LookoutRequest::pointAt:
    pushRequest<LookoutPointRequest>(req);
    break;
  case LookoutRequest::scan:
    pushRequest<LookoutScanRequest>(req);
    break;
  case LookoutRequest::track:
    pushRequest<LookoutTrackRequest>(req); 
    break;
  case LookoutRequest::search:
    pushRequest<LookoutSearchRequest>(req); 
    break;
  default:
    cout << "Lookout::executeRequest: unknown request type " << req.getHeadMotionType() << endl;
  };
  const unsigned int reqid = requests.back()->requestID = ++idCounter;  
  executeRequest();
  return reqid;
}

void Lookout::executeRequest() {
  if ( curReq != NULL || requests.empty() )
    return;
  curReq = requests.front();
  cout << "Lookout executing " << LookoutRequest::headMotionTypeNames[curReq->getHeadMotionType()] 
       << " request, id=" << curReq->requestID << endl;
  curPAR = dynamic_cast<LookoutPointRequest*>(curReq);

  // If we're going to be computing pixel or distance modes, clear the bins first
  if ( curPAR != NULL && curPAR->numSamples > 1 )
    switch ( curPAR->getResultType() ) {
    case LookoutRequest::imageResult:
      for ( unsigned int i=0; i<pixelHistograms.size(); i++ )
	pixelHistograms[i].clear();
      break;
#ifdef TGT_HAS_IR_DISTANCE
    case LookoutRequest::distanceResult:
      while ( distanceSamples.size() > 0) distanceSamples.pop();
      break;
#endif
    default:
      break;
    }

  // now dispatch on the head motion type
  trackerState = inactive;
  switch (curReq->getHeadMotionType()) {
  case LookoutRequest::noMotion:
    erouter->addTimer(this, settle_timer, curPAR->motionSettleTime, false);
    break;
  case LookoutRequest::pointAt:
    moveHeadToPoint();
    break;
  case LookoutRequest::scan:
    setupScan();
    break;
  case LookoutRequest::track:
    setupTrack();
    break;
  case LookoutRequest::search:
    setupSearch();
    break;
  default:
    cout << "Lookout::executeRequest(): unknown request " << curReq->getHeadMotionType() << endl;
    break;
  };
}

void Lookout::relax() {
  motman->setPriority(pointer_id,MotionManager::kIgnoredPriority);
  motman->setPriority(posture_id,MotionManager::kIgnoredPriority);
  motman->setPriority(sequence_id,MotionManager::kIgnoredPriority);
}

void Lookout::moveHeadToPoint() {
  Point pt = curPAR->gazePt;
  switch ( pt.getRefFrameType() ) {
  case unspecified:
  case camcentric:
    cout << "Warning: Lookout gaze point " << curPAR->gazePt << " reference frame must be egocentric or allocentric" << endl;
    pt.setRefFrameType(egocentric);
  case egocentric:
    break;
  case allocentric:
    pt.applyTransform(VRmixin::mapBuilder.worldToLocalTranslateMatrix,egocentric);
    pt.applyTransform(VRmixin::mapBuilder.worldToLocalRotateMatrix,egocentric);
  }
  // point head based on Camera or IR reference frame
  switch ( curPAR->getResultType() ) {
  case LookoutRequest::noResult:
  case LookoutRequest::imageResult: {
    successSave = MMAccessor<HeadPointerMC>(pointer_id)->lookAtPoint(pt.coordX(),pt.coordY(),pt.coordZ());
    motman->setPriority(pointer_id,MotionManager::kStdPriority);
    motman->setPriority(posture_id,MotionManager::kIgnoredPriority);
    motman->setPriority(sequence_id,MotionManager::kIgnoredPriority);
  }
    break;
#ifdef TGT_HAS_IR_DISTANCE
  case LookoutRequest::distanceResult: {
#ifdef TGT_ERS7
    successSave = MMAccessor<PostureMC>(posture_id)->solveLinkVector(pt.coords,NearIRFrameOffset,Kinematics::pack(0,0,1));
#else
    successSave = MMAccessor<PostureMC>(posture_id)->solveLinkVector(pt.coords,IRFrameOffset,Kinematics::pack(0,0,1));
#endif
    motman->setPriority(posture_id,MotionManager::kStdPriority);
    motman->setPriority(pointer_id,MotionManager::kIgnoredPriority);
    motman->setPriority(sequence_id,MotionManager::kIgnoredPriority);
    }
    break;
#endif
  case LookoutRequest::interestPoints:
    cout << "Lookout error: this scan type cannot return interest points." << endl;
    return;
  }
}

void Lookout::processEvent(const EventBase& event) {
  if ( curReq == NULL ) {
    if ( event.getGeneratorID() == EventBase::motmanEGID &&
	 event.getTypeID() == EventBase::statusETID )
      return; // harmless motman status event from startup
    else {
      cout << "Error:  Lookout received an event when not executing a request:\n";
      cout << "    " << event.getDescription(true,3) << endl;
      return;
    }
  }

  switch (curReq->getHeadMotionType()) {
  case LookoutRequest::noMotion:
  case LookoutRequest::pointAt:
    processPointAtEvent(event);
    break;
  case LookoutRequest::scan:
    processScanEvent(event);
    break;
  case LookoutRequest::track:
    processTrackEvent(event);
    break;
  case LookoutRequest::search:
    processSearchEvent(event);
    break;
  default:
    cout << "Lookout::processEvent: unknown head motion request type: "
	 << curReq->getHeadMotionType() << ", event: " << event.getDescription() << endl;
    break;
  };
}

void Lookout::processPointAtEvent(const EventBase& event) {
  switch (event.getGeneratorID()) {
  case EventBase::motmanEGID:
    if ( event.getSourceID() == pointer_id || event.getSourceID() == posture_id ) {
      // head motion complete, now wait for head to settle
      motman->setPriority(event.getSourceID(), MotionManager::kBackgroundPriority);
      erouter->addTimer(this, settle_timer, curPAR->motionSettleTime, false);
    }
    break;

  case EventBase::timerEGID:
    if (event.getSourceID() == settle_timer || event.getSourceID() == sample_timer) {
      switch (curReq->getResultType()) {
      case LookoutRequest::imageResult:
	erouter->addListener(this, EventBase::visRegionEGID,
			     ProjectInterface::visRegionSID,EventBase::statusETID);
	break;
#ifdef TGT_HAS_IR_DISTANCE
      case LookoutRequest::distanceResult:
	erouter->addListener(this, EventBase::sensorEGID, SensorSrcID::UpdatedSID);
	break;
#endif
      case LookoutRequest::noResult:
      default:
	requestComplete(successSave);
	break;
      };
    }
    break;

  case EventBase::visRegionEGID:
    erouter->removeListener(this, EventBase::visRegionEGID);
    VRmixin::camSkS.clear();
    VRmixin::camShS.clear();
    curPAR->image.bind((curPAR->sketchFunc)());
    ++curPAR->sampleCounter;
    if ( curPAR->numSamples == 1 || findPixelModes() == true ) {
      curPAR->toBaseMatrix = kine->jointToBase(curPAR->joint);
      requestComplete(successSave);
    }
    else
      erouter->addTimer(this, sample_timer, curPAR->sampleInterval, false);
    break;

  case EventBase::sensorEGID:
    erouter->removeListener(this, EventBase::sensorEGID);
#ifdef TGT_HAS_IR_DISTANCE
    if ( findDistanceMode() == true ) {
      curPAR->toBaseMatrix = kine->jointToBase(curPAR->joint);
      requestComplete(successSave);
    } else
#endif
      erouter->addTimer(this, sample_timer, curPAR->sampleInterval, false);
    break;

  default:
    cout << "Lookout::processPointAtEvent: unknown event " << event.getDescription() << endl;
    break;
  };
}

bool Lookout::findPixelModes() {
  // update our counts using the new sammple
  for (size_t i = 0; i<pixelHistograms.size(); i++)
    pixelHistograms[i][curPAR->image[i]]++;
  if ( curPAR->sampleCounter < curPAR->numSamples )
    return false;
  // we have enough samples; compute the mode
  for (size_t i = 0; i<pixelHistograms.size(); i++) {
    unsigned int maxCount = 0;
    uchar maxChar = 0;
    for (map<uchar, unsigned int>::const_iterator it = pixelHistograms[i].begin();
	 it != pixelHistograms[i].end(); it++)
      if (it->second > maxCount) {
	maxCount = it->second;
	maxChar = it->first;
      }
    curPAR->image[i] = maxChar;
  }
  return true;
}

#ifdef TGT_HAS_IR_DISTANCE
float Lookout::getDistanceModeValue() {
  int const npops = distanceSamples.size() / 2;
  for ( int i=0; i<npops; i++ ) distanceSamples.pop();
  return distanceSamples.top();
}

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

bool Lookout::findDistanceMode() {
  distanceSamples.push(getIR());
  return (int)distanceSamples.size() < curPAR->numSamples;
}
#endif // TGT_HAS_IR_DISTANCE

void Lookout::requestComplete(bool success) {
  cout << "Lookout request " << curReq->requestID << " complete." << endl;
  erouter->removeTimer(this);
  erouter->removeListener(this,EventBase::sensorEGID);
  erouter->removeListener(this,EventBase::visSegmentEGID);
  erouter->removeListener(this,EventBase::visRegionEGID);
  erouter->removeListener(this,EventBase::visObjEGID);
  LookoutRequest *saveReq = curReq;
  curReq = NULL;
  curPAR = NULL;
  requests.pop();
  switch ( saveReq->getHeadMotionType() ) {
  case LookoutRequest::noMotion:
  case LookoutRequest::pointAt:
    switch ( saveReq->getResultType() ) {
    case LookoutRequest::noResult:
      erouter->postEvent(LookoutPointAtEvent(success,static_cast<LookoutPointRequest*>(saveReq)->toBaseMatrix,
					     EventBase::lookoutEGID, saveReq->requestID, EventBase::deactivateETID));
      break;
    case LookoutRequest::imageResult:
      erouter->postEvent(LookoutSketchEvent(success,static_cast<LookoutPointRequest*>(saveReq)->image,
					    static_cast<LookoutPointRequest*>(saveReq)->toBaseMatrix,
					    EventBase::lookoutEGID, saveReq->requestID, EventBase::deactivateETID));
      break;
#ifdef TGT_HAS_IR_DISTANCE
    case LookoutRequest::distanceResult:
      erouter->postEvent(LookoutIREvent(success,getDistanceModeValue(), static_cast<LookoutPointRequest*>(saveReq)->toBaseMatrix,
					EventBase::lookoutEGID, saveReq->requestID, EventBase::deactivateETID));
      break;
#endif
    default:
      cout << "Lookout::requestComplete(): Unknown type returned by getResultType()\n";
    }
    break;

  case LookoutRequest::scan:
    erouter->postEvent(LookoutScanEvent(static_cast<LookoutScanRequest*>(saveReq)->tasks,
					EventBase::lookoutEGID,saveReq->requestID, EventBase::deactivateETID));
    break;

  case LookoutRequest::track:
    erouter->postEvent(EventBase(EventBase::lookoutEGID,saveReq->requestID, EventBase::deactivateETID));
    break;

  case LookoutRequest::search: {
    Sketch<uchar> image(VRmixin::sketchFromSeg());
#ifdef TGT_HAS_CAMERA
    const NEWMAT::Matrix camToBase = kine->jointToBase(CameraFrameOffset);
#else
    NEWMAT::Matrix camToBase(4,4);
    camToBase << ROBOOP::fourbyfourident;
#endif
    erouter->postEvent(LookoutSketchEvent(success, image, camToBase,
					  EventBase::lookoutEGID, saveReq->requestID, EventBase::deactivateETID));
    break;
  }

  default:
    cout << "Lookout::requestComplete(): Unknown head motion type\n";
  }

  // The postEvent above may have led to a series of functions
  // returning and eventually shutting down the Lookout or starting
  // another request, so make sure we're still in the same state
  // before proceeding.
  delete saveReq;
  if ( curReq == NULL && !requests.empty() )
    executeRequest();
}

typedef SegmentedColorGenerator::color_class_state color_class_state;

void Lookout::storeVisionRegionDataTo(vector<Point>& data, const set<color_index>& colors, int minArea) {
  const unsigned char *img = 
    ProjectInterface::defRegionGenerator->getImage(ProjectInterface::fullLayer,0);
  const color_class_state *regions = reinterpret_cast<const color_class_state*> (img);
  for (set<color_index>::const_iterator it = colors.begin();
       it != colors.end(); it++)
    for (int i = 0; i < regions[*it].num; i++)
      if ((regions[*it].list+i)->area > minArea) {
	data.push_back(findLocationFor(regions[*it].list));
	cout << regions[*it].name  << " at " << data.back() << endl;
      }
      else break;
}
  
#ifdef TGT_HAS_IR_DISTANCE
void Lookout::storeIRDataTo(vector<Point>& data) {
  NEWMAT::ColumnVector ray = Kinematics::pack(0,0,getIR());
  cout << "dist= " << ray(3) << ", in base frame= ";
#ifdef TGT_ERS7
  NEWMAT::ColumnVector baseCoords = kine->jointToBase(NearIRFrameOffset)*ray;
#else //not ERS7
  NEWMAT::ColumnVector baseCoords = kine->jointToBase(IRFrameOffset)*ray;
#endif
  data.push_back(Point(baseCoords(1),baseCoords(2),baseCoords(3)));
  cout << data.back() << endl;
}
#endif


//================ Scan Request ================

void Lookout::processScanEvent(const EventBase& event) {
  //  cout << "Lookout::processScan: " << event.getName() << endl;
  static bool listeningObjEGID = false;
  const LookoutScanRequest* curScanReq = dynamic_cast<LookoutScanRequest*>(curReq);

  switch (event.getGeneratorID()) {
  case EventBase::motmanEGID:
    // head arrived at the start of motion sequence, add listeners and start scan sequence
    if (event.getSourceID() == pointer_id) {
      erouter->addTimer(this,settle_timer,curScanReq->motionSettleTime,false);
    }
    else if (event.getSourceID() == sequence_id) {
      motman->setPriority(sequence_id,MotionManager::kBackgroundPriority);
      requestComplete();
    }
    break;

  case EventBase::timerEGID: // time to take some measurements
    if ( event.getSourceID() == settle_timer )
      triggerScanMotionSequence();
    else if ( event.getSourceID() >= scan_timer && event.getSourceID()-scan_timer < curScanReq->tasks.size() ) {
      LookoutRequest::Task* task = curScanReq->tasks[event.getSourceID()-scan_timer];
      if (task->getTaskType() == LookoutRequest::Task::visRegTask) {
	LookoutRequest::VisionRegionTask* vrt = dynamic_cast<LookoutRequest::VisionRegionTask*>(task);
	storeVisionRegionDataTo(vrt->data,vrt->index,vrt->minArea);
      }
      else if (task->getTaskType() == LookoutRequest::Task::visObjTask) {
	erouter->addListener(this, EventBase::visSegmentEGID,
			     ProjectInterface::visSegmentSID,EventBase::statusETID);
	listeningObjEGID = true;
      }
#ifdef TGT_HAS_IR_DISTANCE
      else if (task->getTaskType() == LookoutRequest::Task::irTask) 
	storeIRDataTo(task->data);
#endif
    }
    break;

  case EventBase::visSegmentEGID:
    if (listeningObjEGID)
      listeningObjEGID = false;
    else
      erouter->removeListener(this, EventBase::visSegmentEGID);
    break;

  case EventBase::visObjEGID:
    if (listeningObjEGID) {
      const VisionObjectEvent &voe = static_cast<const VisionObjectEvent&>(event);    
      for (vector<LookoutRequest::Task*>::const_iterator it = curScanReq->tasks.begin();
	   it != curScanReq->tasks.end(); it++)
	if ((*it)->getTaskType() == LookoutRequest::Task::visObjTask) {
	  LookoutRequest::VisionTask& vTask = *dynamic_cast<LookoutRequest::VisionTask*>(*it);
	  if (vTask.index.find(event.getSourceID()) != vTask.index.end()) {
	    vTask.data.push_back(findLocationFor(voe));
	    cout << "VisionObject at " << vTask.data.back() << endl;
	    break;
	  }
	}
    }
    break;
  default:
    cout << "Lookout::processScan: unknown event " << event.getName() << endl;
    break;
  };
}

void Lookout::setupScan() {
  const LookoutScanRequest *curScan = dynamic_cast<const LookoutScanRequest*>(curReq);
  cout << "scan speed: " << curScan->scanSpeed 
       << "  (rad / millisec)\n";
  if ( !curScan->searchArea.isValid() ) {
    cout << "Invalid search area in LookoutScanRequest" << endl;
    return;
  }
  switch ( curScan->searchArea->getType() ) {
  case lineDataType: {
    const Shape<LineData> &line = ShapeRootTypeConst(curScan->searchArea,LineData);
    scanAlongLine(line->firstPt(), line->secondPt());
    break;
  }
  case polygonDataType:{
    const Shape<PolygonData> &poly = ShapeRootTypeConst(curScan->searchArea,PolygonData);
    scanAlongPolygon(poly->getVertices());
    break;
  }
  default:
    cout << "Invalid shape type for LookoutRequest searchArea: must be line or polygon" << endl;
  }
}

void Lookout::triggerScanMotionSequence() {
  const LookoutScanRequest* curScanReq = dynamic_cast<LookoutScanRequest*>(curReq);
  for (unsigned int i = 0; i < curScanReq->tasks.size(); i++) {
    const LookoutRequest::Task* task = curScanReq->tasks[i];
    if (task->getTaskType() == LookoutRequest::Task::visObjTask) {
      const LookoutRequest::VisionTask& vTask = *dynamic_cast<const LookoutRequest::VisionTask*>(task);
      for(set<color_index>::const_iterator color_it = vTask.index.begin();
	  color_it != vTask.index.end(); color_it++)
	erouter->addListener(this,EventBase::visObjEGID, *color_it);
    }
    erouter->addTimer(this,scan_timer+i,0,false); // for initial measurements
    int snap_interval = (int)((float)task->dTheta / (float)curScanReq->scanSpeed);
    cout << "Lookout scan snap_interval = " << snap_interval << endl;
    erouter->addTimer(this, scan_timer+i, snap_interval, true);
  }
  motman->setPriority(pointer_id,MotionManager::kIgnoredPriority);
  motman->setPriority(posture_id,MotionManager::kIgnoredPriority);
  motman->setPriority(sequence_id,MotionManager::kStdPriority);
  MMAccessor<MediumMotionSequenceMC>(sequence_id)->play();
}

void Lookout::scanAlongLine(const Point& startPt, const Point& endPt) {
  motman->setPriority(pointer_id,MotionManager::kIgnoredPriority); // just to be safe
  MMAccessor<HeadPointerMC> pointer_acc(pointer_id);

  // first compute joint angles for final point
  pointer_acc->lookAtPoint(endPt.coordX(),endPt.coordY(),endPt.coordZ());
	std::vector<float> anglesA(NumHeadJoints);
	for(unsigned int i=0; i<NumHeadJoints; ++i)
		anglesA[i]=pointer_acc->getJointValue(i);
  
  // now compute angles for initial point, so when the motion command is executed we'll start there
  pointer_acc->lookAtPoint(startPt.coordX(),startPt.coordY(),startPt.coordZ());
	std::vector<float> anglesB(NumHeadJoints);
	for(unsigned int i=0; i<NumHeadJoints; ++i)
		anglesB[i]=pointer_acc->getJointValue(i);
	
	float total_joint_distance=0;
	for(unsigned int i=0; i<NumHeadJoints; ++i) {
		float diff = anglesA[i] - anglesB[i];
		total_joint_distance += diff*diff;
	}
  const unsigned int movement_time = (unsigned int)(sqrt(total_joint_distance) / dynamic_cast<const LookoutScanRequest*>(curReq)->scanSpeed);
  
  // begin at start point (the HeadPointerMC will have brought us there) and move smoothly to end point
  motman->setPriority(sequence_id,MotionManager::kIgnoredPriority); // just to be safe
  MMAccessor<MediumMotionSequenceMC> mseq_acc(sequence_id);
  mseq_acc->clear();
  mseq_acc->pause();
#ifdef TGT_HAS_HEAD
	for(unsigned int i=0; i<NumHeadJoints; ++i)
		mseq_acc->setOutputCmd(HeadOffset+i,anglesA[i]);
#endif
  mseq_acc->advanceTime(movement_time);
#ifdef TGT_HAS_HEAD
	for(unsigned int i=0; i<NumHeadJoints; ++i)
		mseq_acc->setOutputCmd(HeadOffset+i,anglesB[i]);
#endif
  motman->setPriority(posture_id,MotionManager::kIgnoredPriority); // just to be safe
  motman->setPriority(pointer_id,MotionManager::kStdPriority); // start head moving to where scan begins; completion will trigger the scan
}

void Lookout::scanAlongPolygon(vector<Point> const &vertices, const bool closed) {
	if ( vertices.size() == 0 )
		requestComplete();
	vector<Point> vertices_copy(vertices);
	const Point startPt = vertices_copy[0];
	if ( closed )
		vertices_copy.push_back(startPt);
	motman->setPriority(pointer_id,MotionManager::kBackgroundPriority); // just to be safe
	motman->setPriority(sequence_id,MotionManager::kBackgroundPriority); // just to be safe
	MMAccessor<HeadPointerMC> pointer_acc(pointer_id);
	MMAccessor<MediumMotionSequenceMC> mseq_acc(sequence_id);
	mseq_acc->pause();
	mseq_acc->clear();
	float const speed = dynamic_cast<const LookoutScanRequest*>(curReq)->scanSpeed;
	std::vector<float> anglesA(NumHeadJoints,0), anglesB(NumHeadJoints,0);
	for ( vector<Point>::const_iterator it = vertices_copy.begin(); it != vertices_copy.end(); ++it ) {
		pointer_acc->lookAtPoint((*it).coordX(),(*it).coordY(),(*it).coordZ());
		for(unsigned int i=0; i<NumHeadJoints; ++i)
			anglesB[i]=pointer_acc->getJointValue(i);
		if ( it != vertices_copy.begin() ) {
			float total_joint_distance=0;
			for(unsigned int i=0; i<NumHeadJoints; ++i) {
				float diff = anglesA[i] - anglesB[i];
				total_joint_distance += diff*diff;
			}
			const unsigned int movement_time = (unsigned int)(sqrt(total_joint_distance) /speed + 200);
			// cout << "Movement time " << movement_time << "msec to " << *it << endl;
			mseq_acc->advanceTime(movement_time);
		}
#ifdef TGT_HAS_HEAD
		for(unsigned int i=0; i<NumHeadJoints; ++i)
			mseq_acc->setOutputCmd(HeadOffset+i,anglesB[i]);
		mseq_acc->advanceTime(200);
		for(unsigned int i=0; i<NumHeadJoints; ++i)
			mseq_acc->setOutputCmd(HeadOffset+i,anglesB[i]);
#endif
		anglesA.swap(anglesB);
	}
	
	pointer_acc->lookAtPoint(startPt.coordX(),startPt.coordY(),startPt.coordZ());
	motman->setPriority(pointer_id,MotionManager::kStdPriority); // start head moving to where scan begins; completion will trigger the scan
}
	
//================ Track Requests ================

void Lookout::setupTrack() {
  // point the head at the object, then start tracking it and posting status events
  LookoutTrackRequest *curTR = static_cast<const LookoutTrackRequest*>(curReq);
  const ShapeRoot &target = curTR->targetShape;
  curTR->cindex = ProjectInterface::getColorIndex(target->getColor());
  Point egoLoc = target->getCentroid();
  if ( target->getRefFrameType() == camcentric ) {
#ifndef TGT_HAS_CAMERA
    std::cerr << "Lookout::setupTrack target has camcentric reference frame, but target model doesn't have a camera" << std::endl;
#else
    const NEWMAT::Matrix camToBase = kine->jointToBase(CameraFrameOffset);
    egoLoc.projectToGround(camToBase,kine->calculateGroundPlane()); // assume head hasn't moved, or we're screwed
#endif
  }
  trackerState = moveToAcquire;
  erouter->addListener(this, EventBase::visRegionEGID, ProjectInterface::visRegionSID, EventBase::deactivateETID);
  motman->setPriority(posture_id,MotionManager::kIgnoredPriority);
  motman->setPriority(sequence_id,MotionManager::kIgnoredPriority);
  motman->setPriority(pointer_id,MotionManager::kStdPriority);
  successSave = MMAccessor<HeadPointerMC>(pointer_id)->lookAtPoint(egoLoc.coordX(), egoLoc.coordY(), egoLoc.coordZ());
}

void Lookout::processTrackEvent(const EventBase &event) {
  const LookoutTrackRequest *curTR = static_cast<const LookoutTrackRequest*>(curReq);
  switch ( event.getGeneratorID() ) {

  case EventBase::visRegionEGID: {
    if ( trackerState == moveToAcquire ) return; // ignore vision events while slewing head to initial position
    const color_class_state *ccs = reinterpret_cast<const CMVision::color_class_state*>
      (ProjectInterface::defRegionGenerator->getImage(ProjectInterface::fullLayer,0));
    const color_class_state &col = ccs[curTR->cindex];
    if ( col.list == NULL || col.list->area <= curTR->minBlobArea ) {
      if ( trackerState != lost ) {
	trackerState = lost;
	erouter->addTimer(this,lost_timer,2000,false);  // wait for object to reappear
      }
      return;
    }
    // test succeeded
    erouter->removeTimer(this,lost_timer);
    trackerState = tracking;
    Point target = findLocationFor(col.list);
    MMAccessor<HeadPointerMC>(pointer_id)->lookAtPoint(target.coordX(),target.coordY(),target.coordZ());
  }
    break;


  case EventBase::motmanEGID:
    switch ( trackerState ) {
    case inactive:
      break;
    case moveToAcquire:
      trackerState = tracking;
      break;
    case tracking:
    case searching:
    case centering:
    case lost:
      break;
    }
    break;
    
  case EventBase::timerEGID:
    if ( event.getSourceID() == lost_timer )   // object out of sight for too long: give up? Or search?
      stopTrack();
    break;

  default:
    break;
  }
}

void Lookout::stopTrack() {
  if ( curReq != NULL && curReq->getHeadMotionType() == LookoutRequest::track )
    requestComplete(false);
}

Point Lookout::findLocationFor(const float normX, const float normY) {
  // NEWMAT::ColumnVector ground_plane = kine->calculateGroundPlane();
  NEWMAT::ColumnVector cameraPt(4);
  config->vision.computeRay(normX, normY, cameraPt(1),cameraPt(2),cameraPt(3));
#ifdef TGT_HAS_IR_DISTANCE
  cameraPt *= getIR();
#else
  cameraPt *= 400;
#endif
  cameraPt(4) = 1;
  // groundPt = kine->projectToPlane(CameraFrameOffset, cameraPt, 
  //				  BaseFrameOffset, ground_plane, BaseFrameOffset);
#ifndef TGT_HAS_CAMERA
  const NEWMAT::ColumnVector groundPt = cameraPt;
#else
  const NEWMAT::Matrix camToBase(kine->jointToBase(CameraFrameOffset));
  const NEWMAT::ColumnVector groundPt = camToBase * cameraPt;
#endif
  return Point(groundPt(1),groundPt(2),groundPt(3),egocentric);
}

  /*
void Lookout::processTrackVision(float normX, float normY, bool isCurrentlyVisible) {
  if (!isCurrentlyVisible && landmarkInView) { // landmark just lost
    cout << "landmark just lost" << endl;
    erouter->addTimer(this,start_pan,500,false); // wait 0.5 sec before starting to look for landmark
    erouter->removeTimer(this,reset_pan);
  }
  else if (!landmarkInView && isCurrentlyVisible) { // landmark just found
    cout << "found landmark" << endl;
    erouter->removeTimer(this,start_pan);
    erouter->addTimer(this,reset_pan,1000,false);
  }
  else if (isCurrentlyVisible) { // continue tracking landmark
    trackObjectAt(normX,normY);
  }
  landmarkInView = isCurrentlyVisible;
  lm_location = findLocationFor(normX,normY);
  erouter->postEvent(EventBase::lookoutEGID, curReq->getRequestID(), EventBase::statusETID,0);
}

void Lookout::processTrackEvent(const EventBase& event) {
  switch (event.getGeneratorID()) {
  case EventBase::visObjEGID:
    if (event.getTypeID()==EventBase::statusETID) {
      const VisionObjectEvent *voe = (static_cast<const VisionObjectEvent*>(&event));
      const float area = voe->getBoundaryArea();
      processTrackVision(voe->getCenterX(), voe->getCenterY(), area > 0.03);
    }
    break;
  case EventBase::visSegmentEGID:
    if (event.getTypeID() == EventBase::statusETID) {
      vector<Point> pts = getRegionData();
      if (pts.empty()) processTrackVision(0,0,false);
      else processTrackVision(pts.front().coordX(),pts.front().coordY(),true);
    }
    break;
  case EventBase::timerEGID:
    switch (event.getSourceID()) {
    case start_pan: //! enough time passed after landmark lost
      cout << "landmark not seen for 0.5 sec\n";
      if (curReq->isSticky())
	lookForLandmark();
      else
	requestComplete();
      break;
    case reset_pan:
      cout << "reset pan motion" << endl;
      { MMAccessor<SmallMotionSequenceMC> (sequence_id)->setTime(0); }
      break;
    default:
      cout << "unknown source timer evenet" << endl;
      break;
    }
  case EventBase::motmanEGID:
    if (event.getSourceID() == sequence_id && curReq->isSticky()) { // lookForLandmark() failed
      cout << "Lookout::processTrack: track failed\n";
      requestComplete();
    }
    break;
  default:
    cout << "Lookout::processTrack: unknown event" << endl;
    break;
  };
}
  */


  /*
void Lookout::trackObjectAt(float horiz, float vert) {
  setPanPrior(false);
//  findLandmarkLocation(voe);
//  erouter->postEvent(EventBase::lookoutEGID, curReq->getRequestID(), EventBase::statusETID,0);
  double tilt=state->outputs[HeadOffset+TiltOffset]-vert*M_PI/7.5;
  double pan=state->outputs[HeadOffset+PanOffset]-horiz*M_PI/6;
  if(tilt<mathutils::deg2rad(-20.0))
    tilt=mathutils::deg2rad(-20.0);
  if(tilt>mathutils::deg2rad(40.0))
    tilt=mathutils::deg2rad(40.0);
  if(pan>mathutils::deg2rad(80.0))
    pan=mathutils::deg2rad(80.0);
  if(pan<mathutils::deg2rad(-80.0))
    pan=mathutils::deg2rad(-80.0);

#ifdef TGT_ERS7
  //  cout << "tilt: " << state->outputs[HeadOffset+TiltOffset] << ", nod: " << state->outputs[HeadOffset+NodOffset] << endl;
  if (tilt < -0.5)
    MMAccessor<HeadPointerMC> (pointer_id)->setJoints(tilt,pan,outputRanges[HeadOffset+NodOffset][MinRange]);
  else {
    const float act_tilt = state->outputs[HeadOffset+TiltOffset];
    const float nod_fact = act_tilt*act_tilt*4.0;
    MMAccessor<HeadPointerMC> (pointer_id)->setJoints(tilt,pan,outputRanges[HeadOffset+NodOffset][MinRange]*nod_fact);
  }
#else
  MMAccessor<HeadPointerMC> (pointer_id)->setJoints(tilt,pan,0);
#endif
}
  */

// ================ Search Requests ================

/* Move head to the target shape.  If we encounter the desired color
region along the way, stop moving.  Otherwise, upon reaching the
target, begin a spiral search sequence.  If the sequence completes
without finding the color region, return failure.  Else find the
center of the color region, move the head there, wait for the camera
to settle, take a picture, and return success. */

void Lookout::setupSearch() {
  // point the head at expected object location, then start a spiral search
  LookoutSearchRequest *curSR = static_cast<const LookoutSearchRequest*>(curReq);
  const ShapeRoot &target = curSR->targetShape;
  curSR->cindex = ProjectInterface::getColorIndex(target->getColor());
  Point egoLoc = target->getCentroid();
  if ( target->getRefFrameType() == allocentric ) {
    egoLoc.applyTransform(VRmixin::mapBuilder.worldToLocalTranslateMatrix,egocentric);
    egoLoc.applyTransform(VRmixin::mapBuilder.worldToLocalRotateMatrix,egocentric);
  }
  cout << "Lookout: search target estimated at " << egoLoc << ", color=" << curSR->cindex << endl;
  erouter->addListener(this, EventBase::visRegionEGID,ProjectInterface::visRegionSID,EventBase::deactivateETID);
  MMAccessor<HeadPointerMC>(pointer_id)->lookAtPoint(egoLoc.coordX(), egoLoc.coordY(), egoLoc.coordZ());
  motman->setPriority(pointer_id,MotionManager::kStdPriority);
  trackerState = moveToAcquire;
}

void Lookout::processSearchEvent(const EventBase &event) {
  //  cout << "Lookout::processSearchEvent: " << event.getDescription() << endl;
  const LookoutSearchRequest *curSR = static_cast<const LookoutSearchRequest*>(curReq);
  switch ( event.getGeneratorID() ) {

  case EventBase::motmanEGID:
    if ( event.getSourceID() == pointer_id || event.getSourceID() == sequence_id )
      erouter->addTimer(this, settle_timer, 500, false);
    else
      return;
    break;

  case EventBase::visRegionEGID: {
    const color_class_state *ccs = reinterpret_cast<const CMVision::color_class_state*>
      (ProjectInterface::defRegionGenerator->getImage(ProjectInterface::fullLayer,0));
    const color_class_state &col = ccs[curSR->cindex];
    // insert test to see if desired object has been seen:
    if ( col.list != NULL && col.list->area >= curSR->minBlobArea ) 
      erouter->removeListener(this,EventBase::visRegionEGID);
    else
      return;
    // test succeeded
    const Point center = findLocationFor(col.list);
    cout << "Lookout search: center = " << center << "   area = " << col.list->area << "  color = " << curSR->cindex << endl;
    // MMAccessor<HeadPointerMC>(pointer_id)->lookAtPoint(center.coordX(),center.coordY(),center.coordZ());
    // motman->setPriority(pointer_id,MotionManager::kStdPriority);
    motman->setPriority(pointer_id,MotionManager::kIgnoredPriority);
    motman->setPriority(posture_id,MotionManager::kIgnoredPriority);
    motman->setPriority(sequence_id,MotionManager::kIgnoredPriority);
    trackerState = centering;
    erouter->addTimer(this, settle_timer, 500, false);
    break;
  }

  case EventBase::timerEGID:
    switch ( trackerState ) {
    case moveToAcquire:  // camera has settled, so start the search
      triggerSearchMotionSequence();
      break;
    case searching:
    case centering:      // camera has settled, so grab a picture and return
      requestComplete();
      break;
    default:
      break;
    }

  default:
    break;
  }
}

void Lookout::triggerSearchMotionSequence() {
  cout << "Lookout: beginning search motion sequence" << endl;
  const LookoutSearchRequest *curSR = static_cast<const LookoutSearchRequest*>(curReq);
  const ShapeRoot &target = curSR->targetShape;
  Point egoLoc = target->getCentroid();
  if ( target->getRefFrameType() == allocentric ) {
    egoLoc.applyTransform(VRmixin::mapBuilder.worldToLocalTranslateMatrix,egocentric);
    egoLoc.applyTransform(VRmixin::mapBuilder.worldToLocalRotateMatrix,egocentric);
  }
  int const nspirals = 5;
  float const xstep=50, ystep=100;
  HeadPointerMC hpmc_temp;
  std::vector<float> jointvals(0);
  jointvals.reserve(nspirals*4*NumHeadJoints);
  for (int i=1; i<=nspirals; i++) {
    searchAt(hpmc_temp,jointvals,egoLoc+Point(-i*xstep,        0));
    searchAt(hpmc_temp,jointvals,egoLoc+Point(      0,   i*ystep));
    searchAt(hpmc_temp,jointvals,egoLoc+Point( i*xstep,        0));
    searchAt(hpmc_temp,jointvals,egoLoc+Point(       0, -i*ystep));
  }
  // Calculate all the joint positions first (above), then lock the
  // motionsequence command and quickly copy them over.
  MMAccessor<MediumMotionSequenceMC> mseq_acc(sequence_id);
  mseq_acc->clear();
#ifdef TGT_HAS_HEAD
  for ( std::vector<float>::const_iterator it = jointvals.begin(); it != jointvals.end(); ) {
		mseq_acc->advanceTime(800);
		for(unsigned int i=0; i<NumHeadJoints; ++i)
			mseq_acc->setOutputCmd(HeadOffset+i,*(it++));
  }
  mseq_acc->advanceTime(800);
#endif
  mseq_acc->play();
  motman->setPriority(sequence_id,MotionManager::kStdPriority);
  trackerState = searching;
}

void Lookout::searchAt(HeadPointerMC &hpmc_temp,
		       std::vector<float> &jointvals,
		       const Point &target) {
	hpmc_temp.lookAtPoint(target.coordX() ,target.coordY() ,target.coordZ());
	for(unsigned int i=0; i<NumHeadJoints; ++i)
		jointvals.push_back(hpmc_temp.getJointValue(i));
}

} // namespace
