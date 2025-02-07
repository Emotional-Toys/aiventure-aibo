//-*-c++-*-
#ifndef _POINTDATA_H_
#define _POINTDATA_H_

#include <vector>
#include <iostream>
#include <string>

#include "Shared/newmat/newmat.h"

#include "BaseData.h"    // superclass
#include "Point.h"       // Point data member
#include "ShapeTypes.h"  // pointDataType

namespace DualCoding {

class ShapeRoot;
class SketchSpace;
template<typename T> class Sketch;

class PointData : public BaseData {
public:
  Point the_point;

  //! Constructor
  PointData(ShapeSpace& _space, const Point &c);

  static ShapeType_t getStaticType() { return pointDataType; }

  DATASTUFF_H(PointData);
  
  //! Centroid. (Virtual in BaseData.)
  virtual Point getCentroid() const { return the_point; }  
  
  BoundingBox getBoundingBox() const {
    return BoundingBox(the_point.coordX(),the_point.coordY(),the_point.coordX(),the_point.coordY());
  }

  //! Match points based on their parameters.  (Virtual in BaseData.)
  virtual bool isMatchFor(const ShapeRoot& other) const;

  virtual void mergeWith(const ShapeRoot& other);

  virtual bool updateParams(const ShapeRoot& other, bool force=false);

  //! Print information about this shape. (Virtual in BaseData.)
  virtual void printParams() const;
  
  //! Transformations. (Virtual in BaseData.)
  virtual void applyTransform(const NEWMAT::Matrix& Tmat, const ReferenceFrameType_t newref=unspecified);
  
  //! Project to ground
  virtual void projectToGround(const NEWMAT::Matrix& camToBase,
			       const NEWMAT::ColumnVector& groundplane);

  //! Extraction.
  static std::vector<ShapeRoot> extractPoints(const Sketch<bool>& sketch);

  virtual unsigned short getDimension() const { return 0; }

  PointData& operator=(const PointData&);
  operator Point&() { return the_point; };
  operator const Point&() const { return the_point; };
  
private:
  //! Render into a sketch space and return reference. (Private.)
  virtual Sketch<bool>* render() const;
  //@}

};

} // namespace

#endif
