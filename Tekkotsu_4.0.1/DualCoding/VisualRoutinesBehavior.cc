#include "VisualRoutinesBehavior.h"

namespace DualCoding {

//----------------------------------------------------------------

void VisualRoutinesBehavior::DoStart() {
  if ( !started ) {
    BehaviorBase::DoStart();
    startCrew();
  }
}

void VisualRoutinesBehavior::DoStop() {
  if ( started ) {
    stopCrew();
    BehaviorBase::DoStop();
  }
}

} // namespace
