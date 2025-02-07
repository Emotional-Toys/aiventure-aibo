//-*-c++-*-
#ifndef _ELLIPSEDATA_H_
#define _ELLIPSEDATA_H_

#include <vector>
#include <iostream>

#include "Shared/newmat/newmat.h"

#include "BaseData.h"    // superclass
#include "Point.h"       // Point data member
#include "Shared/Measures.h"    // coordinate_t; AngPi data member

namespace DualCoding {

class ShapeRoot;
class SketchSpace;
template<typename T> class Sketch;

#define ELLIPSE_DATA_MOBILE false

class EllipseData : public BaseData {
private:
  Point center_pt;
  float semimajor;
  float semiminor;
  AngPi orientation;

public:

  //! Constructor
  EllipseData(ShapeSpace& _space, const Point &c, const float _semimajor=25, const float _semiminor=15, const float _orientation=0);

  static ShapeType_t getStaticType() { return ellipseDataType; }

  DATASTUFF_H(EllipseData);
  
  //! Centroid. (Virtual in BaseData.)
  Point getCentroid() const { return center_pt; } 
  void setCentroidPt(const Point& other) { center_pt.setCoords(other); }
  
  //! finds points where semiminor or semimajor axis touchs the circumference of ellipse
  std::pair<Point,Point> findFeaturePoints() const;

  //! updates major/minor axis and orientation from feature points
  void updateProperties(const Point& minorPt, const Point& majorPt);

  BoundingBox getBoundingBox() const;

  //! Match ellipses based on their parameters.  (Virtual in BaseData.)
  virtual bool isMatchFor(const ShapeRoot& other) const;

  virtual bool isAdmissible() const {
    return (semimajor >= 9.0 );  // **DST Hack** minimum size for an ellipse to be added to local map
  }

  virtual bool updateParams(const ShapeRoot& other, bool force=false);

  //! Print information about this shape. (Virtual in BaseData.)
  virtual void printParams() const;
  
  //! Transformations. (Virtual in BaseData.)
  void applyTransform(const NEWMAT::Matrix& Tmat, const ReferenceFrameType_t newref=unspecified);
  
  //! Project to ground
  virtual void projectToGround(const NEWMAT::Matrix& camToBase,
			       const NEWMAT::ColumnVector& groundplane);

  //! Center point access function.
  const Point& centerPt() const { return center_pt; }
  
  virtual unsigned short getDimension() const { return 2; }
  
  //! Properties functions.
  //@{
  AngPi getOrientation() const { return orientation; }
  float getSemimajor() const { return semimajor; }
  float getSemiminor() const { return semiminor; }
  //@}
  
  
  //! Set properties.
  //@{
  void setOrientation(const AngPi _orientation);
  void setSemimajor(float _semimajor);
  void setSemiminor(float _semiminor);
  //@}

  //! Extraction.
  static std::vector<Shape<EllipseData> > extractEllipses(const Sketch<bool>& sketch);
  
  
private:
  //! Render into a sketch space and return reference. (Private.)
  virtual Sketch<bool>* render() const;
  //@}

  EllipseData& operator=(const EllipseData&); //!< don't call

};


} // namespace

#endif
