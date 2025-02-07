//-*-c++-*-
#ifndef _LINEDATA_H_
#define _LINEDATA_H_

#include <vector>
#include <iostream>

#include "BaseData.h"    // superclass
#include "EndPoint.h"    // EndPoint data member
#include "ShapeFuns.h"
#include "ShapePoint.h"
#include "SketchTypes.h" // uint
#include "Shared/mathutils.h"   // deg2rad

namespace DualCoding {

class Region;

#define EXTRACT_LINE_MIN_AREA 20
#define DEFAULT_MIN_LENGTH 4.0

//! A line shape, with two endpoints, a length, orientation, etc.
class LineData : public BaseData {
private:
  EndPoint end1_pt;
  EndPoint end2_pt;
  float rho_norm;
  AngTwoPi theta_norm;
  AngPi orientation;
  float length;

  static const Point origin_pt;

  friend class Shape<LineData>;   // grant access to line endpoints
  friend class PolygonData;
  friend class BlobData;

public:
  
  //! Constructor
  LineData(ShapeSpace& _space, const EndPoint &p1, const EndPoint &p2) 
    : BaseData(_space,getStaticType()), 
      end1_pt(p1), end2_pt(p2), rho_norm(0), theta_norm(0), orientation(0), length(0)
  { update_derived_properties(); }
  
  //! Constructor
  LineData(ShapeSpace& _space, const Point &pt, orientation_t orient);

  //! Copy constructor
  LineData(const LineData& other)
    : BaseData(other),
      end1_pt(other.end1_pt), end2_pt(other.end2_pt),
      rho_norm(other.rho_norm), theta_norm(other.theta_norm),
      orientation(other.orientation), length(other.length) {}

  static ShapeType_t getStaticType() { return lineDataType; }
  
  DATASTUFF_H(LineData);
  
  //! Updates norm parameters (rho and theta)
  void update_derived_properties();
  
  //! Centroid. (Virtual in BaseData.)
  virtual Point getCentroid() const;	
  
  //! Makes endpoints inactive if value = true
  void setInfinite(bool value=true);

  BoundingBox getBoundingBox() const {
    return BoundingBox(std::min(end1_pt.coordX(),end2_pt.coordX()),
		       std::min(end1_pt.coordY(),end2_pt.coordY()),
		       std::max(end1_pt.coordX(),end2_pt.coordX()),
		       std::max(end1_pt.coordY(),end2_pt.coordY()));
  }

  //! Match lines based on their parameters.  (Virtual in BaseData.)
  virtual bool isMatchFor(const ShapeRoot& other) const;
  bool isMatchFor(const LineData& other) const;

  //! Lines are admissible to the local map if they're long enough to not be noise.
  virtual bool isAdmissible() const ;

  virtual bool updateParams(const ShapeRoot& other, bool force=false);
  static void updateLinePt(EndPoint& localPt, coordinate_t local_coord,
			   const EndPoint& groundPt, coordinate_t ground_coord,
			   int sign);
  virtual void mergeWith(const ShapeRoot& other);

  LineData& operator=(const LineData&);

  //checks if update of endpoints from (p1,p2) to (p3,p4) is acceptable
  bool isValidUpdate(coordinate_t p1, coordinate_t p2, coordinate_t p3, coordinate_t p4);

  //! Print information about this shape. (Virtual in BaseData.)
  virtual void printParams() const;
  
  //! Transformations. (Virtual in BaseData.)
  virtual void applyTransform(const NEWMAT::Matrix& Tmat, const ReferenceFrameType_t newref=unspecified);
  
  //! Project to ground
  virtual void projectToGround(const NEWMAT::Matrix& camToBase,
			       const NEWMAT::ColumnVector& groundplane);

  virtual unsigned short getDimension() const { return 1; }

  void printEnds() const;
  void setEndPts(const EndPoint& _end1_pt, const EndPoint& _end2_pt);
  
  //!@name Point access functions.
  /*! The first point of a line is the leftmost point if the line is horizontal, 
  else the topmost point.  With an optional Shape<LineData> argument, uses the current
  line's orientation to pick the appropriate point of the other line.
  */
  //@{
  EndPoint& end1Pt() { return end1_pt; }
  EndPoint& end2Pt() { return end2_pt; }
  const EndPoint& end1Pt() const { return end1_pt; }
  const EndPoint& end2Pt() const { return end2_pt; }

  EndPoint& leftPt();
  EndPoint& rightPt();
  EndPoint& topPt();
  EndPoint& bottomPt();

  Shape<PointData> leftPtShape();
  Shape<PointData> rightPtShape();
  Shape<PointData> topPtShape();
  Shape<PointData> bottomPtShape();

  EndPoint& firstPt();
  EndPoint& firstPt(const Shape<LineData> &otherline) const;
  EndPoint& secondPt();
  EndPoint& secondPt(const Shape<LineData> &otherline) const;

  Shape<PointData> firstPtShape();
  Shape<PointData> secondPtShape();

  coordinate_t firstPtCoord() const;
  coordinate_t firstPtCoord(const Shape<LineData> &otherline) const;
  coordinate_t secondPtCoord() const;
  coordinate_t secondPtCoord(const Shape<LineData> &otherline) const;
  //@}
  
  //!@name Properties functions
  //@{
  float getRhoNorm() const { return rho_norm; }
  AngTwoPi getThetaNorm() const { return theta_norm; }
  AngPi getOrientation() const { return orientation; }
  float getLength() const { return length; }
  std::pair<float,float> lineEquation_mb() const;
  std::vector<float> lineEquation_abc() const;
  std::vector<float> lineEquation_abc_xz() const;
  //@}
  
  //!@name Orientation functions
  //@{
  bool isNotVertical() const; //!< True if line orientation is far enough from vertical
  bool isOverlappedWith(const LineData& otherline, int amount=0) const;
  //  bool isRoughlyPerpendicularTo(Shape<LineData> &other);
  //  bool isExactlyPerpendicularTo(Shape<LineData> &other);
  //@}
  
  
  //!@name Predicates based on line length
  //@{
  bool isLongerThan(const Shape<LineData>& other) const;
  bool isLongerThan(float ref_length) const;
  bool isShorterThan(const Shape<LineData>& other) const;
  bool isShorterThan(float ref_length) const;
  //@}
  
  //! Check if point falls between the two lines
  bool isBetween(const Point &p, const LineData &other) const;

  //!@name Check line intersection
  //@{
  bool intersectsLine(const Shape<LineData>& other) const;
  bool intersectsLine(const LineData& other) const;

  Point intersectionWithLine(const Shape<LineData>& other) const
  { bool b; return intersectionWithLine(other,b,b); };
  Point intersectionWithLine(const Shape<LineData>& other,
			     bool& intersection_on_this,
			     bool& intersection_on_other) const;
  Point intersectionWithLine(const LineData& other) const 
  { bool b; return intersectionWithLine(other, b,b); };
  Point intersectionWithLine(const LineData& other,
			     bool& intersection_on_this,
			     bool& intersection_on_other) const;
  
  //@}

  bool pointsOnSameSide(const Point& p1, const Point& p2);
  bool pointOnLine(const Point& p);
  
  //! Distance.
  float perpendicularDistanceFrom(const Point& other) const;
  
  
  // ==================================================
  // BEGIN SKETCH MANIPULATION AND LINE EXTRACTION CODE
  // ==================================================
  
  //!@name Line extraction
  //@{

  //! Extracts most prominent line from a skeletonized image.
  static Shape<LineData> extractLine(Sketch<bool>& sketch);

  //! Extracts most prominent line from a skeletonized image.
  //! It's often useful to use the original sketch as an occluder
  static Shape<LineData> extractLine(Sketch<bool>& skelsketch, 
				     const Sketch<bool>& occlusions);
  //@}
  
  //! Helper functions used by extractLine().
  //@{
  static Shape<LineData> splitLine(ShapeSpace &ShS, Region &skelchunk,
				   Sketch<bool> &skeleton, const Sketch<bool> &occlusions);

  //! Clears a line from a sketch.
  void clearLine(Sketch<bool>& sketch);
  
  void scanHorizForEndPts(const Sketch<uint>& skelDist, const Sketch<bool>& occlusions,
			  float m, float b);
  void scanVertForEndPts(const Sketch<uint>& skelDist, const Sketch<bool>& occlusions,
			  float m, float b);

  void balanceEndPointHoriz(EndPoint &pt, Sketch<bool> const &occluders, float m, float b);
  void balanceEndPointVert(EndPoint &pt, Sketch<bool> const &occluders, float m, float b);

  static std::vector<Shape<LineData> > extractLines(Sketch<bool> const& sketch, int const num_lines=10);
  
  static std::vector<Shape<LineData> > extractLines(Sketch<bool> const& skel,
					       Sketch<bool> const& occluders,
					       int const num_lines=10);
  
  static std::vector<Shape<LineData> > houghTransform(const Sketch<bool>& fat, 
					       const Sketch<bool>& skinny, 
					       const Sketch<bool>& occlusions,
					       const size_t num_lines,
					       int minLength=DEFAULT_MIN_LENGTH); 

  static bool linesParallel(Shape<LineData> l1, Shape<LineData>l2);

  //@}
  
  //!@name Comparison predicates used by shape functions
  //@{

  //! True if line1 shorter than line2
  class LengthLessThan : public BinaryShapePred<LineData> {
  public:
    bool operator() (const Shape<LineData> &ln1, const Shape<LineData> &ln2) const;
  };

  //! True if difference in line orientations is <= tolerance (default 20 deg)
  class ParallelTest : public BinaryShapePred<LineData> {
  public:
    AngPi tolerance;
    ParallelTest(AngPi _tol=mathutils::deg2rad(20.0)) : tolerance(_tol) {}
    bool operator() (const Shape<LineData> &line1, const Shape<LineData> &line2) const;
  };


  //! True if difference in line orientations is 90 deg +/- tolerance (default 20 deg)
  class PerpendicularTest : public BinaryShapePred<LineData> {
  public:
    AngPi tolerance;
    PerpendicularTest(AngPi _tol=mathutils::deg2rad(20.0)) : tolerance(_tol) {}
    bool operator() (const Shape<LineData> &line1, const Shape<LineData> &line2) const;
  };


  //! True if line orientations are within @a ang_tol (default 20 deg) and normpoints are within @a dist_tol (default 10 units)
  class ColinearTest : public BinaryShapePred<LineData> {
  public:
    AngPi ang_tol;
    coordinate_t dist_tol;
    ColinearTest(AngPi _ang_tol=mathutils::deg2rad(20.0), coordinate_t _dist_tol=10) : 
      ang_tol(_ang_tol), dist_tol(_dist_tol) {}
    bool operator() (const Shape<LineData> &line1, const Shape<LineData> &ln2) const;
  };

  //! Predicate returns true if line orientation is within @a threshold of horizontal
  class IsHorizontal : public UnaryShapePred<LineData> {
  public:
    IsHorizontal(AngPi thresh=M_PI/6) : UnaryShapePred<LineData>(), threshold(thresh) {}
    bool operator() (const Shape<LineData> &line) const;
  private:
    AngPi threshold;
  };
      
  //! Predicate returns true if line orientation is within threshold of vertical
  class IsVertical : public UnaryShapePred<LineData> {
  public:
    IsVertical(AngPi thresh=M_PI/3) : UnaryShapePred<LineData>(), threshold(thresh) {}
    bool operator() (const Shape<LineData> &line) const;
  private:
    AngPi threshold;
  };

  //@}


  virtual Sketch<bool>& getRendering();

private:
  static const int extractorGapTolerance = 8;

  //!@name  Rendering.
  //@{
  
  //! Render into a sketch space and return reference.
  Sketch<bool>* render() const;
  
  //! returns a Sketch which is true where the specified line is
  //! end0_stop and end1_stop specify whether rendering should stop at endpoints
  //  Sketch<bool>& drawline2d(SketchSpace &renderspace, 
  //			   int x0, int y0, int x1, int y1) const;
  void setDrawCoords(float& x1,float& y1, float& x2, float& y2, const int width, const int height) const;
  static void drawline2d(Sketch<bool>& canvas, int x0, int y0, int x1, int y1);
  //@}
};

} // namespace

#endif

