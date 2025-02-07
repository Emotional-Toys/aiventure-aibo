//-*-c++-*-
#ifndef _LOCALIZATIONPARTICLEDATA_H_
#define _LOCALIZATIONPARTICLEDATA_H_

#include "Shared/Measures.h"    // coordinate_t; AngPi data member
#include "Shared/newmat/newmat.h"

#include "BaseData.h"    // superclass
#include "PFShapeLocalization.h"
#include "Point.h"       // Point data member
#include "ShapeTypes.h"  // localizationParticleDataType

namespace DualCoding {

class ShapeRoot;
class SketchSpace;
template<typename T> class Sketch;

class LocalizationParticleData : public BaseData {
private:
  Point location;
  AngTwoPi orientation;
  float weight;

public:

  //! Constructor
  LocalizationParticleData(ShapeSpace& _space, const Point &_location, const AngTwoPi _orientation, const float _weight=0);

  LocalizationParticleData(ShapeSpace &space, const PFShapeLocalization::particle_type& particle);

  //! Copy constructor
  LocalizationParticleData(const LocalizationParticleData &other);

  static ShapeType_t getStaticType() { return localizationParticleDataType; }

  DATASTUFF_H(LocalizationParticleData);
  
  //! Centroid. (Virtual in BaseData.)
  virtual Point getCentroid() const { return location; } 
  void setCentroid(const Point& other) { location.setCoords(other); }
  
  AngTwoPi getOrientation() const { return orientation; }
  void setOrientation(const AngTwoPi _orientation) {orientation = _orientation; }

  float getWeight() const { return weight;}
  void setWeight(float w) { weight=w; }

  virtual bool isMatchFor(const ShapeRoot& other) const;

  virtual bool updateParams(const ShapeRoot&, bool force=false);

  //! Print information about this shape. (Virtual in BaseData.)
  virtual void printParams() const;
  
  virtual void applyTransform(const NEWMAT::Matrix& Tmat, const ReferenceFrameType_t newref=unspecified);

  virtual void projectToGround(const NEWMAT::Matrix&,
			       const NEWMAT::ColumnVector&) {}

  virtual unsigned short getDimension() const { return 2; }

  LocalizationParticleData& operator=(const LocalizationParticleData &other);

private:
  //! Render into a sketch space and return pointer. (Private.)
  virtual Sketch<bool>* render() const;


};

} // namespace

#endif
