//-*-c++-*-
#ifndef INCLUDED_Pilot_h_
#define INCLUDED_Pilot_h_

#include <queue>

#include "Behaviors/StateNode.h"
#include "Behaviors/Nodes/PostureNode.h"
#include "Behaviors/Nodes/WaypointWalkNode.h"
#include "Motion/MotionManager.h"

#include "PilotRequest.h"
#include "VisualRoutinesStateNode.h"

namespace DualCoding {

class Pilot : public StateNode {
public:

  typedef unsigned int PilotVerbosity_t;
  static const PilotVerbosity_t PVstart = 1<<0;
  static const PilotVerbosity_t PVevents = 1<<1;
  static const PilotVerbosity_t PVexecute = 1<<2;
  static const PilotVerbosity_t PVsuccess = 1<<3;
  static const PilotVerbosity_t PVfailure = 1<<4;
  static const PilotVerbosity_t PVcomplete = 1<<5;

private:
  static PilotVerbosity_t verbosity;
public:
  static void setVerbosity(PilotVerbosity_t v) { verbosity = v; }
  static PilotVerbosity_t getVerbosity() { return verbosity; }

  class Success : public StateNode {
  public:
    Success() : StateNode("Success") {}
    virtual void DoStart();
  };

  class Failure : public StateNode {
  public:
    Failure() : StateNode("Failure") {}
    virtual void DoStart();
  };

  class Walk : public WaypointWalkNode {
  public:
    Walk() : WaypointWalkNode("Walk") {}
    virtual void DoStart();
  };

  class VisualSearch : public VisualRoutinesStateNode {
  public:
    VisualSearch() : VisualRoutinesStateNode("VisualSearch"), mapbuilder_id(0) {}
    virtual void DoStart();
    unsigned int mapbuilder_id;
  };

  class VisualSearchHaveMap : public VisualRoutinesStateNode {
  public:
    VisualSearchHaveMap() : VisualRoutinesStateNode("VisualSearchHaveMap") {}
    virtual void DoStart();
  };

  class VisualSearchWalk : public WaypointWalkNode {
  public:
    VisualSearchWalk() : WaypointWalkNode("VisualSearchWalk"), walk_id(MotionManager::invalid_MC_ID) {}
    virtual void DoStart();
    MotionManager::MC_ID walk_id;
  };

  class GotoShape : public WaypointWalkNode {
  public:
    GotoShape() : WaypointWalkNode("GotoShape") {}
    virtual void DoStart();
  };

  class BuildTarget : public VisualRoutinesStateNode {
  public:
    BuildTarget(bool _buildFrontLeft = true, bool _buildFrontRight = true, bool _buildBackLeft = true, bool _buildBackRight = true, bool _lookAtCentroid = true, int _maxRetries = 10) :
               VisualRoutinesStateNode("BuildTarget"),
               buildFrontLeft(_buildFrontLeft), buildFrontRight(_buildFrontRight), buildBackLeft(_buildBackLeft), buildBackRight(_buildBackRight), lookAtCentroid(_lookAtCentroid),
               frontLeftPt(0, 0, 0), frontRightPt(0, 0, 0), backLeftPt(0, 0, 0), backRightPt(0, 0, 0), centroid(0, 0, 0),
               maxRetries(_maxRetries), triesLeft(0),
               myState(none) {}
    virtual void DoStart();
    virtual void DoStop();
    virtual void processEvent(const EventBase& event);
    
    void setParams(bool _buildFrontLeft = true, bool _buildFrontRight = true, bool _buildBackLeft = true, bool _buildBackRight = true, bool _lookAtCentroid = true, int _maxRetries = 10);
  
  protected:
    void generateMapBuilderRequest();
    void keepBestTarget();
    bool getNextPoint(Point *point);
    
    bool buildFrontLeft, buildFrontRight, buildBackLeft, buildBackRight, lookAtCentroid;
    Point frontLeftPt, frontRightPt, backLeftPt, backRightPt, centroid;
    int maxRetries, triesLeft;
    enum State {none, build, complete, error} myState;
  };
  
  class GotoTarget : public WaypointWalkNode {
  public:
    GotoTarget() : WaypointWalkNode("GotoTarget"), buildTargetNode(NULL) {}
    virtual void DoStart();
    
    BuildTarget *buildTargetNode;
    
  protected:
    int getNextWaypoint(Point *point, AngSignPi *angle);
    void setNextWaypoint();
    Point getPointAtAngle(AngSignPi angle);
  
  
  private:
    GotoTarget(const GotoTarget&); //!< do not call
    GotoTarget& operator=(const GotoTarget&); //!< do not call
};
  
  class PushTarget : public WaypointWalkNode {
  public:
    PushTarget() : WaypointWalkNode("PushTarget"), buildTargetNode(NULL) {}
    virtual void DoStart();
    
    BuildTarget *buildTargetNode;
  
  private:
    PushTarget(const PushTarget&); //!< do not call
    PushTarget& operator=(const PushTarget&); //!< do not call
  };

  class CreepToShape : public WaypointWalkNode {
  public:
    CreepToShape() : WaypointWalkNode("CreepToShape") {}
    virtual void DoStart();
  };

  class CreepToShapeStand : public PostureNode {
  public:
    CreepToShapeStand() : PostureNode("CreepToShapeStand","walk.pos") {}
    virtual void DoStop();
  };

  class CreepToShapeWait : public StateNode {
  public:
    CreepToShapeWait() : StateNode("CreepToShapeWait") {}
  };

  class CreepToShapeBuildMap : public VisualRoutinesStateNode {
  public:
    CreepToShapeBuildMap() :  VisualRoutinesStateNode("CreepToShapeBuildMap") {}
    virtual void DoStart();
    static unsigned int mapreq_id;
  };

  class CreepToShapeLocalize : public StateNode {
  public:
    CreepToShapeLocalize() : StateNode("CreepToShapeLocalize") {}
    virtual void DoStart();
    virtual void processEvent(const EventBase&);
  };

#ifdef TGT_HAS_IR_DISTANCE
#ifdef TGT_ERS7	
  class IRCliff : public StateNode {
  public:
    IRCliff() : StateNode("IRCliff") {}
    virtual void DoStart();
    virtual void processEvent(const EventBase&);
  };
#endif // TGT_ERS7

  class IRObstacle : public StateNode {
  public:
    IRObstacle() : StateNode("IRObstacle") {}
    virtual void DoStart();
    virtual void processEvent(const EventBase&);
  };
#endif // TGT_HAS_IR_DISTANCE

  class Localize : public StateNode {
  public:
    Localize() : StateNode("Localize"), mapbuilder_id(0) {}
    virtual void DoStart();
    virtual void processEvent(const EventBase&);
    void processMap(unsigned int timestamp);
    unsigned int mapbuilder_id;
  };

  class Dispatch : public VisualRoutinesStateNode {
  public:
    Dispatch() : 
      VisualRoutinesStateNode("Dispatch"), successNode(NULL), failureNode(NULL),
      walkNode(NULL), visualsearchNode(NULL), gotoshapeNode(NULL),
      gotoTargetNode(NULL), pushTargetNode(NULL),
      creeptoshapeNode(NULL),
#ifdef TGT_HAS_IR_DISTANCE
#ifdef TGT_ERS7
      ircliffNode(NULL),
#endif
      irobstacleNode(NULL),
#endif
	  localizeNode(NULL)
    {}
    virtual void setup();
    virtual void DoStart();
    Success *successNode;
    Failure *failureNode;
    Walk *walkNode;
    VisualSearch *visualsearchNode;
    GotoShape *gotoshapeNode;
    GotoTarget *gotoTargetNode;
    PushTarget *pushTargetNode;
    CreepToShape *creeptoshapeNode;
#ifdef TGT_HAS_IR_DISTANCE
#ifdef TGT_ERS7
    IRCliff *ircliffNode;
#endif // TGT_ERS7
    IRObstacle *irobstacleNode;
#endif // TGT_HAS_IR_DISTANCE
    Localize *localizeNode;
  private:
    Dispatch(const Dispatch&);
    Dispatch& operator=(const Dispatch&);
  };

  //! Constructor
  Pilot();

  virtual void DoStart();
  virtual void DoStop();
  virtual void setup();
  unsigned int executeRequest(const PilotRequest& req);
  void abort(); //!< Abort current request and empty the request queue
  MotionManager::MC_ID getWaypointWalk_MC_ID() { return waypointwalk_id; }

  static const unsigned int invalid_Pilot_ID = (unsigned int)-1;

protected:
  void executeRequest();
  void requestComplete(const bool result=true);
  void success() { dispatchNode->successNode->DoStart(); }
  void failure() { dispatchNode->failureNode->DoStart(); }

  std::queue<PilotRequest*> requests;
  PilotRequest *curReq;
  unsigned int idCounter;
  MotionManager::MC_ID waypointwalk_id;
  MotionManager::MC_ID posture_id;
  unsigned int lastDisplayParticleTime;

  Dispatch *dispatchNode;

private:
  Pilot(const Pilot&); //!< do not call
  Pilot& operator=(const Pilot&); //!< do not call

};

} // namespace

#endif
