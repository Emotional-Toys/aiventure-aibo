#include "VisualRoutinesStateNode.h"

namespace DualCoding {

//----------------------------------------------------------------

void VisualRoutinesStateNode::DoStart() {
  if ( !started ) {
    StateNode::DoStart();
    startCrew();
  }
}

void VisualRoutinesStateNode::DoStop() {
  if ( started ) {
    stopCrew();
    StateNode::DoStop();
  }
}

} // namespace
