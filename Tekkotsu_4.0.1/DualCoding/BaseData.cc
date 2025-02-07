#include "Macrodefs.h"
#include "Shared/Measures.h"

#include "Sketch.h"   // this must precede references to Sketch
#include "BaseData.h"
#include "ShapeRoot.h"
#include "ShapePoint.h"
#include "SketchDataRoot.h"
#include "ShapeSpace.h"

using namespace std;

namespace DualCoding {

BoundingBox::BoundingBox(const BoundingBox &b1, const BoundingBox &b2) :
  xmin(std::min(b1.xmin,b2.xmin)), ymin(std::min(b1.ymin,b2.ymin)),
  xmax(std::max(b1.xmax,b2.xmax)), ymax(std::max(b1.ymax,b2.ymax)) {}

BoundingBox::BoundingBox(const std::vector<ShapeRoot> &vec) :
  xmin(0), ymin(0), xmax(0), ymax(0) {
  if ( vec.size() > 0 ) {
    *this = vec[0]->getBoundingBox();
    for ( size_t i = 1; i<vec.size(); i++ )
      *this = BoundingBox(*this,vec[i]->getBoundingBox());
  }
}

std::ostream& operator<< (std::ostream& out, const BoundingBox &b) {
  out << "BoundingBox(" << b.xmin << "," << b.ymin << ","
      << b.xmax << "," << b.ymax << ")";
  return out;
}


BaseData::BaseData(ShapeSpace& _space, ShapeType_t _type, int _parentId) :
  space(&_space), name(data_name(_type)), type(_type), 
  id(0), parentId(_parentId), lastMatchId(0),
  refcount(0), viewable(true),
  color_rgb((ProjectInterface::getNumColors() != -1U) ? ProjectInterface::getColorRGB(1) : rgb(0,0,255)), // color 0 is invalid, so use color 1 as default, or blue if colors aren't loaded yet
  confidence(1),
  mobile(false),
  rendering_sketch(NULL)
{};


  /*
BaseData::BaseData(ShapeType_t _type, int _parentId) :
  space(NULL), name(data_name(_type)), type(_type), 
  id(0), parentId(_parentId), lastMatchId(0),
  refcount(0), viewable(true),
  color_rgb(ProjectInterface::defSegmentedColorGenerator?ProjectInterface::getColorRGB(1):rgb(0,0,255)), // color 0 is invalid, so use color 1 as default, or blue if colors aren't loaded yet
  confidence(1),
  mobile(false),
  rendering_sketch(NULL)
{};
  */
    
BaseData::BaseData(const BaseData& other)
  : space(other.space), name(other.name), type(other.type),
    id(0), parentId(other.parentId), lastMatchId(other.lastMatchId),
    refcount(0), viewable(other.viewable),
    color_rgb(other.color_rgb),
    confidence(other.confidence),
    mobile(other.mobile),
    rendering_sketch(NULL)
{
  //  cout << "copied BaseData: parentID " << parentId << " <-> " << other.parentId << endl;
};


BaseData::~BaseData(void) { 
  if ( rendering_sketch != NULL )
    delete rendering_sketch;
}

Shape<PointData> BaseData::getCentroidPtShape() const {
  PointData *pt = new PointData(*space,getCentroid());
  pt->inheritFrom(*this);
  return Shape<PointData>(pt);
}

BaseData& BaseData::operator=(const BaseData& other) {
  // assumes &other =? this check is done by the sub class using BaseData::operator=
  //  if (&other == this)
  //    return *this;

  space = other.space ? &(*other.space) : NULL;
  name = other.name;
  type = other.type;	
  id = other.id;
  parentId = other.parentId;
  lastMatchId = other.lastMatchId;
  refcount = other.refcount;
  viewable = other.viewable;
  color_rgb = other.color_rgb;
  confidence = other.confidence;
  mobile = other.mobile;  
  rendering_sketch = other.rendering_sketch ? &(*rendering_sketch) : NULL;
  return *this;
}

void BaseData::inheritFrom(const BaseData &parent) {   // used by leftPtShape, etc.
  setParentId(parent.getViewableId());
  setColor(parent.getColor());
}

void BaseData::inheritFrom(const ShapeRoot &parent) {
  setParentId(parent->getViewableId());
  setColor(parent->getColor());
}

void BaseData::inheritFrom(const SketchDataRoot &parent) {
  setParentId(parent.getViewableId());
  setColor(parent.getColor());
}

void BaseData::V(std::string const &_name) {
  setViewable(true);
  if ( !_name.empty() ) setName(_name);
}

void BaseData::N(std::string const &_name) {
  setViewable(false);
  if ( !_name.empty() ) setName(_name);
}

ReferenceFrameType_t BaseData::getRefFrameType() const {
  return space->getRefFrameType(); }


//!Type.
//{
//! Get shape type name.
const char* BaseData::getTypeName() const { return data_name(type); }

//! Test the shape type.
bool BaseData::isType(ShapeType_t this_type) const { return this_type == type; }

//! Test that two shapes are of same type.
bool BaseData::isSameTypeAs(const ShapeRoot& other) const {
	return((bool)(isType(other->type))); }


bool BaseData::isSameColorAs(const ShapeRoot& other) const {
  return getColor() == other->getColor(); }

void BaseData::setColor(const std::string &color_name) {
  setColor(ProjectInterface::getColorRGB(color_name));
}

void BaseData::setColor(const rgb &new_color) {
  color_rgb = new_color;
  if ( rendering_sketch != NULL )
    (*rendering_sketch)->setColor(new_color);
}

void BaseData::setColor(const unsigned int color_index) {
  setColor(ProjectInterface::getColorRGB(color_index));
}


bool BaseData::getMobile() const { return mobile; }

void BaseData::setMobile(bool _mobile) { mobile = _mobile; }

void BaseData::deleteRendering() {
  delete rendering_sketch;
  rendering_sketch = NULL;
}

Sketch<bool>& BaseData::getRendering() {
  if ( rendering_sketch != NULL )
    return *rendering_sketch;
  rendering_sketch = render();
  (*rendering_sketch)->setColor(getColor());
  (*rendering_sketch)->setParentId(id);
  (*rendering_sketch)->setName("render("+getName()+")");
  return *rendering_sketch;
}


void BaseData::increaseConfidence(int n, int maxConfidence) {
  confidence += n;
  if ( maxConfidence > 0 )
    confidence = std::min(confidence, maxConfidence);
}
  
void BaseData::increaseConfidence(const BaseData& other, int maxConfidence) {
  increaseConfidence(other.getConfidence() > 0 ? other.getConfidence()+1 : 2, maxConfidence);
}
  
void BaseData::increaseConfidence(const ShapeRoot& other, int maxConfidence) { 
  increaseConfidence(other.getData(), maxConfidence);
}

} // namespace
