//-*-c++-*-
#ifndef _POINT_H_
#define _POINT_H_

#include <iostream>
#include <vector>

#include "Shared/newmat/newmat.h"
#include "Shared/newmat/newmatio.h"

#include "Shared/Measures.h"   // coordinate_t, AngPi
#include "ShapeTypes.h" // ReferenceFrameType_t

namespace DualCoding {

class LineData;
template<typename T> class Shape;

/*! We define Point as a separate lightweight class because it is used
 * as a component of all the xxxData classes, and we don't want to nest
 * these structures.
 */

class Point {

public:
  NEWMAT::ColumnVector coords;
  ReferenceFrameType_t refFrameType;

  //! Constructors
  //@{
  Point(void);
  Point(coordinate_t const &xp, coordinate_t const &yp, coordinate_t zp=0, ReferenceFrameType_t ref=unspecified);
  Point(const NEWMAT::ColumnVector& c, ReferenceFrameType_t ref=unspecified);
  //@}

  //! Copy constructor
  Point(const Point& otherPt) : coords(otherPt.coords), refFrameType(otherPt.refFrameType) {}

  //! Destructor
  virtual ~Point() { }

  NEWMAT::ColumnVector& getCoords() const { return const_cast<Point*>(this)->coords; }
  coordinate_t coordX() const { return coords(1); }
  coordinate_t coordY() const { return coords(2); }
  coordinate_t coordZ() const { return coords(3); }
  ReferenceFrameType_t getRefFrameType() const { return refFrameType; }

  //! Set position.
  //@{
  void setCoords(const Point& otherPt);
  void setCoords(coordinate_t _x, coordinate_t _y, coordinate_t z=0);
  //@}

  //! Set reference frame type
  void setRefFrameType(const ReferenceFrameType_t ref) { refFrameType = ref; }

  //! Euclidean distance from another point to this one
  float distanceFrom(const Point& other) const;
  float xyDistanceFrom(const Point& other) const;
  float xyNorm() const;

  //! These functions need a ShapeSpace argument because left/right depends on reference frame type.
  //@{
  bool isLeftOf(const Point& other, float distance=0) const;
  bool isRightOf(const Point& other, float distance=0) const;
  bool isAbove(const Point& other, float distance=0) const;
  bool isBelow(const Point& other, float distance=0) const;
//@}

  //! These functions return true based on relative positions, assuming points line in a 2D plane (z coordinate is ignored).
  //@{
  bool isBetween(const Point& other1, const Point& other2) const;
  bool isBetween(const Shape<LineData>& line1, const Shape<LineData>& line2) const;
  bool isBetween(const LineData& line1, const LineData& line2) const;
  bool isInside(const std::vector<LineData>& bound) const;
//@}

  float getHeightAbovePoint(const Point& groundPoint, const NEWMAT::ColumnVector& groundplane);
  

  void applyTransform(const NEWMAT::Matrix& T, const ReferenceFrameType_t newref=unspecified);
  
  //! projects this to ground plane according to camToBase matrix
  void projectToGround(const NEWMAT::Matrix& camToBase,
		       const NEWMAT::ColumnVector& groundPlane);

  void projectToGround(int xres, int yres,
		       const NEWMAT::ColumnVector& ground_plane);

  Point operator+(const Point& b) const;
  Point& operator+=(const Point& b);

  Point operator-(const Point& b) const;
  Point& operator-=(const Point& b);

  Point operator*(float b) const;
  Point& operator*=(float b);

  Point operator/(float b) const;
  Point& operator/=(float b);

  bool operator==(const Point& b) const;
  bool operator!=(const Point& b) const { return !operator==(b); }

  Point& operator=(const Point& b) {
    if (&b==this) return *this;
    setCoords(b);
    refFrameType = b.refFrameType;
    return *this;
  }

  void printData();

  friend class EndPoint;

private:
  void makeRefFrameCompatible(const Point &other);

};

inline Point& leftMost(Point &pt1, Point &pt2) { return pt1.isLeftOf(pt2) ? pt1 : pt2; }
inline Point& rightMost(Point &pt1, Point &pt2) { return pt1.isLeftOf(pt2) ? pt2 : pt1; }
inline Point& topMost(Point &pt1, Point &pt2) { return pt1.isAbove(pt2) ? pt1 : pt2; }
inline Point& bottomMost(Point &pt1, Point &pt2) { return pt1.isAbove(pt2) ? pt2 : pt1; }

std::ostream& operator<<(std::ostream &os, const Point &p);

} // namespace

#endif
