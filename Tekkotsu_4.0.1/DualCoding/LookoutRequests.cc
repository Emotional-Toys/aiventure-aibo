#include "LookoutRequests.h"

namespace DualCoding {

const float LookoutScanRequest::defSpd = 0.0015;

LookoutScanRequest::~LookoutScanRequest() {
  for (std::vector<Task*>::const_iterator it = tasks.begin();
       it != tasks.end(); it++)
    delete *it;
}

const char* const LookoutRequest::headMotionTypeNames[numHeadMotionTypes] = {
  "noMotion",
  "pointAt",
  "scan",
  "track",
  "search"
};


} // namespace
