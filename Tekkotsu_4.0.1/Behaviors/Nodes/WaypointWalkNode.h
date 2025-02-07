//-*-c++-*-
#ifndef INCLUDED_WaypointWalkNode_h_
#define INCLUDED_WaypointWalkNode_h_

#include "MCNode.h"
#include "Motion/MotionManager.h"
#include "Motion/WaypointWalkMC.h"
#include "Events/EventRouter.h"
#include "Events/LocomotionEvent.h"

//!default name for WaypointEngineNode's (have to instantiate a variable in order to use as a template argument)
/*! instantiation will be placed in MCNode.cc to avoid file bloat */
extern const char defWaypointWalkNodeName[];
//!default description for WaypointWalkNode's (have to instantiate a variable in order to use as a template argument)
/*! instantiation will be placed in MCNode.cc to avoid file bloat */
extern const char defWaypointWalkNodeDesc[];

//! A StateNode for doing a waypoint walk, use the template parameter to specify a custom walk MC, or use the ::WaypointWalkNode typedef to accept the "default" walk
template<typename W, const char* mcName=defWaypointWalkNodeName, const char* mcDesc=defWaypointWalkNodeDesc>
class WaypointEngineNode : public MCNode<W,mcName,mcDesc> {
public:

  //! constructor
  WaypointEngineNode() : MCNode<W,mcName,mcDesc>() {}

  //! constructor
  WaypointEngineNode(const std::string& name) : MCNode<W,mcName,mcDesc>(name) {}

  //!destructor
  ~WaypointEngineNode() {}

  virtual void DoStart() {
    MCNode<W,mcName,mcDesc>::DoStart();
    erouter->addListener(this,EventBase::locomotionEGID,MCNode<W,mcName,mcDesc>::getMC_ID(),EventBase::statusETID);
  }

protected:

  //! constructor
  WaypointEngineNode(const std::string& className, const std::string& instanceName) : 
    MCNode<W,mcName,mcDesc>(className,instanceName) {}

  void processEvent(const EventBase &event) {
    if ( static_cast<const LocomotionEvent&>(event).isStop() )
      MCNode<W,mcName,mcDesc>::postCompletionEvent();
  }

};

//! the prototypical WaypointWalkNode, using a WaypointWalkMC
typedef WaypointEngineNode<WaypointWalkMC> WaypointWalkNode;

/*! @file
 * @brief Describes WaypointEngineNode,  a StateNode for doing a waypoint walk; use the template parameter to specify a custom waypoint walk MC, or use the WaypointWalkNode typedef to accept the "default" walk
 * @author dst (Creator)
 *
 * $Author: dst $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.1 $
 * $State: Exp $
 * $Date: 2007/02/26 08:42:51 $
 */

#endif
