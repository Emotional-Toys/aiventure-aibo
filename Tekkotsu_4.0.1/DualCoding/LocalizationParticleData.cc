//-*-c++-*-

#include <math.h>

#include "Sketch.h"
#include "LocalizationParticleData.h"
#include "ShapeLocalizationParticle.h"

using namespace std;

namespace DualCoding {

LocalizationParticleData::LocalizationParticleData(ShapeSpace& _space, 
						   const Point &_location, 
						   const AngTwoPi _orientation,
						   const float _weight) :
  BaseData(_space, getStaticType()), location(_location), orientation(_orientation), weight(_weight) {}

LocalizationParticleData::LocalizationParticleData(ShapeSpace &_space, 
						   const PFShapeLocalization::particle_type& particle) :
  BaseData(_space, getStaticType()), 
  location(Point(particle.x,particle.y,0,_space.getRefFrameType())),
  orientation(particle.theta),
  weight(particle.weight)
{}

LocalizationParticleData::LocalizationParticleData(const LocalizationParticleData &other) :
  BaseData(other), location(other.location), orientation(other.orientation), weight(other.weight) {}


DATASTUFF_CC(LocalizationParticleData);

bool LocalizationParticleData::updateParams(const ShapeRoot&, bool) { return true; }

void LocalizationParticleData::applyTransform(const NEWMAT::Matrix& Tmat, const ReferenceFrameType_t newref) {
  location.applyTransform(Tmat,newref);
  orientation += atan2(Tmat(1,2),Tmat(1,1));
}

void LocalizationParticleData::printParams() const {
  cout << "Type = " << getTypeName() << "  ID=" << getId() << "  ParentID=" << getParentId() << endl;
  cout << "  location{" << location.coordX() << ", " << location.coordY() << "}"
       << ", orientation = " << orientation << endl;
}

bool LocalizationParticleData::isMatchFor(const ShapeRoot& other) const {
  if (!(isSameTypeAs(other) && isSameColorAs(other)))
    return false;
  const Shape<LocalizationParticleData>& other_particle = ShapeRootTypeConst(other,LocalizationParticleData);
  float dist = location.distanceFrom(other_particle->getCentroid());
  return dist < 20; // *** DST hack
}

LocalizationParticleData& LocalizationParticleData::operator=(const LocalizationParticleData &other) {
  if ( this != &other ) {
    BaseData::operator=(other);
    location = other.location;
    orientation = other.orientation;
    weight = other.weight;
  }
  return *this;
}

//! Render into a sketch space and return a pointer. (Private.)
Sketch<bool>* LocalizationParticleData::render() const {
  SketchSpace &SkS = space->getDualSpace();
  NEWMAT::ColumnVector ctr(getCentroid().getCoords());
  SkS.applyTmat(ctr);
  int const cx = int(ctr(1));
  int const cy = int(ctr(2));
  Sketch<bool>& draw_result = 
    *new Sketch<bool>(SkS, "render("+getName()+")");
  draw_result = false;
  draw_result(cx,cy) = true;  
  return &draw_result;
}


} // namespace

