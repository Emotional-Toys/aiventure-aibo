//-*-c++-*-

#ifndef INCLUDED_SketchRoot_h
#define INCLUDED_SketchRoot_h

#include "SketchSpace.h"
#include "SketchDataRoot.h"

namespace DualCoding {

//! Parent class for all Sketch<T>
class SketchRoot {
 public:

  SketchRoot() {}
  virtual ~SketchRoot() {}

  SketchSpace& rootGetSpace() const;
  const SketchDataRoot* rootGetData() const;

  int getNewId() { return ++idCounter; }

private:
  static int idCounter;

  friend class SketchRootReset;
  friend std::ostream& operator<<(std::ostream &os, const SketchRoot &r);
};

//! Dummy class used by SketchSpace to reset static SketchRoot::idCounter
class SketchRootReset {
public:
  SketchRootReset() { SketchRoot::idCounter = 0; } //!< constructor, set idCounter to 0
  virtual ~SketchRootReset() {} //!< virtual destructor to satisfy compiler warning
};

std::ostream& operator<<(std::ostream &os, const SketchRoot &r);

} // namespace

#endif
