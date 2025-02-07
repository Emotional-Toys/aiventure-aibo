// These have to go in the .cc file because SketchSpace.h circularly depends
// on SketchData.h, so we can't reference members of SketchSpace in the
// SketchDataRoot.h file.

#include "SketchDataRoot.h"
#include "SketchSpace.h"  // can't include this in SketchDataRoot.h due to circularities

namespace DualCoding {

const size_t SketchDataRoot::getWidth() const { return space->getWidth(); }
const size_t SketchDataRoot::getHeight() const { return space->getHeight(); }
const size_t SketchDataRoot::getNumPixels() const { return space->getNumPixels(); }

void SketchDataRoot::inheritFrom(const SketchDataRoot &parent) {
  setParentId(parent.getViewableId());
  setColor(parent.getColor());
  if ( getType() == sketchBool )
    setColorMap(segMap);
  else
    setColorMap(parent.getColorMap());
}

void SketchDataRoot::inheritFrom(const ShapeRoot &parent) {
  setParentId(parent->getViewableId());
  setColor(parent->getColor());
}

void SketchDataRoot::inheritFrom(const BaseData &parent) {
  setParentId(parent.getViewableId());
  setColor(parent.getColor());
}

void SketchDataRoot::V(std::string const &_name) {
  setViewable(true);
  if ( !_name.empty() ) setName(_name);
}

void SketchDataRoot::N(std::string const &_name) {
  setViewable(false);
  if ( !_name.empty() ) setName(_name);
}

void SketchDataRoot:: setColor(const std::string &colorname) {
  setColor(ProjectInterface::getColorRGB(colorname));
}

void SketchDataRoot::setColor(const color_index cindex) {
  setColor(ProjectInterface::getColorRGB(cindex));
}


} // namespace
