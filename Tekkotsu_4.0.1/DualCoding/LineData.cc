//-*-c++-*-
#include <iostream>
#include <math.h>
#include <vector>
#include <list>

#include "Macrodefs.h"

#include "SketchSpace.h"
#include "Sketch.h"
#include "Region.h"
#include "visops.h"

#include "ShapeSpace.h"
#include "ShapeRoot.h"

#include "PointData.h"
#include "LineData.h"
#include "ShapeLine.h"

using namespace std;

namespace DualCoding {

DATASTUFF_CC(LineData);

const Point LineData::origin_pt = Point(0,0);

LineData::LineData(ShapeSpace& _space, const Point &p1, orientation_t orient)
  : BaseData(_space,getStaticType()), end1_pt(p1), end2_pt(), 
    rho_norm(0), theta_norm(0), orientation(0), length(0) {
  int const width = space->getDualSpace().getWidth();
  int const height = space->getDualSpace().getHeight();
  // Use a large offset from p1 to p2 because SketchGUI must calculate
  // the line slope from p1/p2 coords transmitted as strings; we don't
  // transmit the orientation.  But don't use an offset so large that the line
  // goes off-screen.
  float p2x=0, p2y=0;
  if ( fabs(orient-M_PI/2) < 0.001 ) {
    p2x = p1.coordX();
    p2y = p1.coordY() > height/2 ? 0 : height-1;
  } else {
    float slope = tan(orient);
    float intcpt = p1.coordY() - p1.coordX()*slope;
    p2x = p1.coordX() >= width/2 ? 0.0 : width-1;
    p2y = p2x * slope + intcpt;
    if ( p2y > height-1 ) {
      p2x = (height - intcpt) / slope;
      p2y = height;
    } else if ( p2y < 0 ) {
      p2x = -intcpt / slope;
      p2y = 0;
    }
  }
  end2_pt = Point(p2x,p2y);
  end1_pt.setValid(false);
  end1_pt.setActive(false);
  end2_pt.setValid(false);
  end2_pt.setActive(false);
  update_derived_properties();
}

Point LineData::getCentroid() const { return (end1Pt()+end2Pt())*0.5; }

void LineData::setInfinite(bool value) {
  end1_pt.setActive(!value);
  end2_pt.setActive(!value);
}

#define NORMPOINT_MATCH_DISTSQ 3500
#define LINE_MATCH_OVERLAP -10
#define LINE_ORIENTATION_MATCH M_PI/6

bool LineData::isMatchFor(const ShapeRoot& other) const {
  if (!(isSameTypeAs(other) && isSameColorAs(other)))
    return false;
  else {
    const Shape<LineData>& other_ln = ShapeRootTypeConst(other,LineData);
    return isMatchFor(other_ln.getData());
  }
}

bool LineData::isMatchFor(const LineData& other_line) const {
  float const px = rho_norm*cos(theta_norm), 
    py = rho_norm*sin(theta_norm);
  float const qx = other_line.rho_norm*cos(other_line.theta_norm),
    qy = other_line.rho_norm*sin(other_line.theta_norm);
  AngPi theta_diff = float(theta_norm) - float(other_line.theta_norm);
  if ( theta_diff > M_PI/2 )
    theta_diff = M_PI - theta_diff;
  float normpointdistsq = (px-qx)*(px-qx) + (py-qy)*(py-qy);
  // cout << "px=" << px << "  py=" << py << "  qx=" << qx << "  qy=" << qy << "  normpointdistsq=" << normpointdistsq 
  //  << "  theta_diff=" << theta_diff
  //   << "  perpdist=" << perpendicularDistanceFrom(other_line.getCentroid())
  //   << "  isoverlapped=" << isOverlappedWith(other_line,LINE_MATCH_OVERLAP) << endl;
  return normpointdistsq < NORMPOINT_MATCH_DISTSQ  // *** DST hack
    && theta_diff < LINE_ORIENTATION_MATCH
    && perpendicularDistanceFrom(other_line.getCentroid()) < 100
    && isOverlappedWith(other_line,LINE_MATCH_OVERLAP);
}

  bool LineData::isOverlappedWith(const LineData& otherline, int amount) const {
  if ( firstPtCoord() <= otherline.firstPtCoord() )
    return secondPtCoord()-amount >= otherline.firstPtCoord();
  else
    return firstPtCoord()+amount <= otherline.secondPtCoord();
}


void LineData::mergeWith(const ShapeRoot& other) {
  const Shape<LineData>& other_line = ShapeRootTypeConst(other,LineData);
  if (other_line->confidence <= 0)
    return;
  const int other_conf = other_line->confidence;
  confidence += other_conf;
  end1_pt = (end1_pt*confidence + other_line->end1Pt()*other_conf) / (confidence+other_conf);
  end2_pt = (end2_pt*confidence + other_line->end2Pt()*other_conf) / (confidence+other_conf);
  update_derived_properties();
}


bool LineData::isValidUpdate(coordinate_t c1_cur, coordinate_t c2_cur, coordinate_t c1_new, coordinate_t c2_new) {
  const float c1_noise = 10.0 + fabs(c1_cur+c1_new) / 20.0; // allow larger error for shapes far from the robot
  const float c2_noise = 10.0 + fabs(c2_cur+c2_new) / 20.0;
  return (c1_new-c1_noise < c1_cur && c2_cur < c2_new+c2_noise);
}


//! Update a line in the local map with info from a matching line in the ground space.
bool LineData::updateParams(const ShapeRoot& ground_root, bool force) {
  const Shape<LineData>& ground_line = ShapeRootTypeConst(ground_root,LineData);
  //  cout << "updating local Line " << getId() << " with data from ground line " << ground_line->getId() << ":" << endl;
  //  ground_line->printEnds();
  //  cout << "Update from " << endl;
  //  printEnds();
  
  const coordinate_t c1_cur = firstPtCoord();
  const coordinate_t c2_cur = secondPtCoord();

  Point _end1_pt = firstPt();
  Point _end2_pt = secondPt();
  
  updateLinePt(firstPt(), firstPtCoord(), firstPt(ground_line), firstPtCoord(ground_line), -1);
  updateLinePt(secondPt(), secondPtCoord(), secondPt(ground_line), secondPtCoord(ground_line), +1);
  //  cout << "to" << endl;
  //  printEnds();
  
  const coordinate_t c1_new = firstPtCoord();
  const coordinate_t c2_new = secondPtCoord();
  
  if (isValidUpdate(c1_cur, c2_cur, c1_new, c2_new) || force){
    //    cout << "was accepted, line updated" << endl;
    //    ++nsamples;
    update_derived_properties();
    return true;
  }
  
  //  cout << "was denied, line not updated" << endl;
  setEndPts(_end1_pt, _end2_pt);
  return false;
}

void LineData::updateLinePt(EndPoint& localPt, coordinate_t local_coord,
			    const EndPoint& groundPt, coordinate_t ground_coord,
			    int sign) {
  if ( groundPt.isValid() ) {
    if ( localPt.isValid() )
      localPt.updateParams(groundPt);
    else
      localPt = groundPt;
  }
  else if ( (ground_coord - local_coord)*sign > 0 )
    localPt = groundPt;
}

bool LineData::isAdmissible() const {
  if (end1Pt().isValid() && end2Pt().isValid())
    return length >= 70.0;  // *** DST hack: lines should be at least 70 mm long
  else
    return length >= 40.0;
}

//! Print information about this shape. (Virtual in BaseData.)
void LineData::printParams() const {
  cout << "Type = " << getTypeName() << "  ID=" << getId() << "  ParentID=" << getParentId() << endl;
  cout << "  end1{" << end1Pt().coordX() << ", " << end1Pt().coordY()  << "}"
       << " active=" << end1Pt().isActive()
       << " valid=" << end1Pt().isValid() << endl;
  
  cout << "  end2{" << end2Pt().coordX() << ", " << end2Pt().coordY() <<  "}"
       << " active=" << end2Pt().isActive()
       << " valid=" << end2Pt().isValid() << std::endl;
  
  cout << "  rho_norm=" << rho_norm
       << ", theta_norm=" << theta_norm
       << ", orientation=" << getOrientation() 
       << ", length=" << getLength() << endl;
  
  printf("  color = %d %d %d\n",getColor().red,getColor().green,getColor().blue);
  
  cout << "  mobile=" << getMobile()
       << ", viewable=" << isViewable() << endl;
  
  vector<float> abc = lineEquation_abc();
  printf("  equ = %f %f %f\n",abc[0],abc[1],abc[2]);
}

void LineData::printEnds() const {
  cout << "  end1{" << end1Pt().coordX() << ", " << end1Pt().coordY()  << "}";
  cout << "  active=" << end1Pt().isActive() << ", valid=" << end1Pt().isValid() << endl;
  cout << "  end2{" << end2Pt().coordX() << ", " << end2Pt().coordY() <<  "}";
  cout << "  active=" << end2Pt().isActive() << ", valid=" << end2Pt().isValid() << endl;
  // cout << endl;
}



// *** Transformations. *** //

//! Apply a transformation to this shape.
void LineData::applyTransform(const NEWMAT::Matrix& Tmat, const ReferenceFrameType_t newref) {
  end1Pt().applyTransform(Tmat,newref);
  end2Pt().applyTransform(Tmat,newref);
  update_derived_properties();
}

void LineData::projectToGround(const NEWMAT::Matrix& camToBase,
			       const NEWMAT::ColumnVector& groundplane) {
  end1Pt().projectToGround(camToBase,groundplane);
  end2Pt().projectToGround(camToBase,groundplane);
  update_derived_properties();
}


// *** Logical endpoints *** //

EndPoint& LineData::leftPt() { return end1Pt().isLeftOf(end2Pt()) ? end1_pt : end2_pt; }
EndPoint& LineData::rightPt() { return end1Pt().isLeftOf(end2Pt()) ? end2_pt : end1_pt; }
EndPoint& LineData::topPt() { return end1Pt().isAbove(end2Pt()) ? end1_pt : end2_pt; }
EndPoint& LineData::bottomPt() { return end1Pt().isAbove(end2Pt()) ? end2_pt : end1_pt; }

Shape<PointData> LineData::leftPtShape() {
  Shape<PointData> result(new PointData(*space, leftPt()));
  result->setName("leftPt");
  result->inheritFrom(*this);
  result->setViewable(false);
  return result;
}

Shape<PointData> LineData::rightPtShape() {
  Shape<PointData> result(new PointData(*space, leftPt()));
  result->setName("rightPt");
  result->inheritFrom(*this);
  result->setViewable(false);
  return result;
}

Shape<PointData> LineData::topPtShape() {
  Shape<PointData> result(new PointData(*space, leftPt()));
  result->setName("topPt");
  result->inheritFrom(*this);
  result->setViewable(false);
  return result;
}

Shape<PointData> LineData::bottomPtShape() {
  Shape<PointData> result(new PointData(*space, leftPt()));
  result->setName("bottomPt");
  result->inheritFrom(*this);
  result->setViewable(false);
  return result;
}

EndPoint& LineData::firstPt() {
  if ( isNotVertical() )
    if ( end1Pt().coordX() < end2Pt().coordX() )
      return end1Pt();
    else return end2Pt();
  else
    if ( end1Pt().coordY() < end2Pt().coordY() )
      return end1Pt();
    else return end2Pt();
}
    
EndPoint& LineData::firstPt(const Shape<LineData> &otherline) const {
  if ( isNotVertical() )
    if ( otherline->end1Pt().coordX() < otherline->end2Pt().coordX() )
      return otherline->end1Pt();
    else return otherline->end2Pt();
  else
    if ( otherline->end1Pt().coordY() < otherline->end2Pt().coordY() )
      return otherline->end1Pt();
    else return otherline->end2Pt();
}
    
EndPoint& LineData::secondPt() {
  if ( isNotVertical() )
    if ( end1Pt().coordX() > end2Pt().coordX() )
      return end1Pt();
    else return end2Pt();
  else
    if ( end1Pt().coordY() > end2Pt().coordY() )
      return end1Pt();
    else return end2Pt();
}
    
EndPoint& LineData::secondPt(const Shape<LineData> &otherline) const {
  if ( isNotVertical() )
    if ( otherline->end1Pt().coordX() > otherline->end2Pt().coordX() )
      return otherline->end1Pt();
    else return otherline->end2Pt();
  else
    if ( otherline->end1Pt().coordY() > otherline->end2Pt().coordY() )
      return otherline->end1Pt();
    else return otherline->end2Pt();
}
    
Shape<PointData> LineData::firstPtShape() {
  Shape<PointData> result(new PointData(*space, firstPt()));
  result->setName("firstPt");
  result->inheritFrom(*this);
  result->setViewable(false);
  return result;
}

Shape<PointData> LineData::secondPtShape() {
  Shape<PointData> result(new PointData(*space, secondPt()));
  result->setName("secondPt");
  result->inheritFrom(*this);
  result->setViewable(false);
  return result;
}

coordinate_t LineData::firstPtCoord() const {
  return  isNotVertical() ?
    const_cast<LineData*>(this)->firstPt().coordX() :
    const_cast<LineData*>(this)->firstPt().coordY();
}

coordinate_t LineData::firstPtCoord(const Shape<LineData> &otherline) const {
  return  isNotVertical() ?
    firstPt(otherline).coordX() :
    firstPt(otherline).coordY();
}

coordinate_t LineData::secondPtCoord() const {
  return  isNotVertical() ?
    const_cast<LineData*>(this)->secondPt().coordX() :
    const_cast<LineData*>(this)->secondPt().coordY();
}

coordinate_t LineData::secondPtCoord(const Shape<LineData> &otherline) const {
  return  isNotVertical() ?
    secondPt(otherline).coordX() :
    secondPt(otherline).coordY();
}


// *** Functions to set endpoints. *** //

void LineData::setEndPts(const EndPoint& _end1_pt, const EndPoint& _end2_pt) {
  end1_pt.setCoords(_end1_pt);
  end1_pt.setActive(_end1_pt.isActive());
  end1_pt.setValid(_end1_pt.isValid());
  end1_pt.setNumUpdates(_end1_pt.numUpdates());

  end2_pt.setCoords(_end2_pt);
  end2_pt.setActive(_end2_pt.isActive());
  end2_pt.setValid(_end2_pt.isValid());
  end2_pt.setNumUpdates(_end2_pt.numUpdates());

  update_derived_properties();
}


// *** Properties functions. *** //

std::pair<float,float> LineData::lineEquation_mb() const {
  float m;
  if ((fabs(end2Pt().coordX() - end1Pt().coordX()) * BIG_SLOPE)
      <= fabs(end2Pt().coordY() - end2Pt().coordY()))
    m = BIG_SLOPE;
  else 
    m = (end2Pt().coordY() - end1Pt().coordY())/(end2Pt().coordX() - end1Pt().coordX());
  float b = end1Pt().coordY() - m * end1Pt().coordX();
  return pair<float,float>(m,b);
}


//! Determine parameters a, b, c, d satisfying the equation ax + bz = c.
std::vector<float> LineData::lineEquation_abc_xz() const {
  float dx = end2Pt().coordX() - end1Pt().coordX();
  float dz = end2Pt().coordZ() - end1Pt().coordZ();

  std::vector<float> abc;
  abc.resize(3, 1.0);
  float& a = abc[0];
  float& b = abc[1];
  float& c = abc[2];

  // If vertical...b = 0
  if((dx == 0.0)
     || (dz/dx > BIG_SLOPE)) {
    a = 1.0;
    b = 0.0;
    c = end1Pt().coordX();
  }
  
  // If horizontal...a = 0
  else if((dz == 0.0)
	  || (dx/dz > BIG_SLOPE)) {
    a = 0.0;
    b = 1.0;
    c = end1Pt().coordZ();
  }
  
  // If not horizontal or vertical...a = 1.0
  else {
    a = 1.0;
    b = (end1Pt().coordX() - end2Pt().coordX()) 
      / (end2Pt().coordZ() - end1Pt().coordZ());
    c = end1Pt().coordX() + b*end1Pt().coordZ();
  }
  
  return(abc);

}

//! Determine parameters a, b, c satisfying the equation ax + by = c.
std::vector<float> LineData::lineEquation_abc() const {
  float dx = end2Pt().coordX() - end1Pt().coordX();
  float dy = end2Pt().coordY() - end1Pt().coordY();
  
  std::vector<float> abc;
  abc.resize(3, 1.0);
  float& a = abc[0];
  float& b = abc[1];
  float& c = abc[2];

  // If vertical...b = 0
  if( fabs(dx) < 1.0e-6 || dy/dx > BIG_SLOPE) {
    a = 1.0;
    b = 0.0;
    c = end1Pt().coordX();
  }
  
  // If horizontal...a = 0
 else if ( fabs(dy) < 1.0e-6 || dx/dy > BIG_SLOPE ) {
    a = 0.0;
    b = 1.0;
    c = end1Pt().coordY();
  }
  
  // If not horizontal or vertical...a = 1.0
  else {
    a = 1.0;
    //    b = (end1Pt().coordX() - end2Pt().coordX()) 
    // / (end2Pt().coordY() - end1Pt().coordY());
    b = -dx / dy;
    c = end1Pt().coordX() + b*end1Pt().coordY();
  }
  
  return(abc);
}


// *** Functions to set values dealing with orientation. *** //

void LineData::update_derived_properties() {
  rho_norm = perpendicularDistanceFrom(origin_pt);
  const vector<float> abc = lineEquation_abc();
  const float& a1 = abc[0];
  const float& b1 = abc[1];
  const float& c1 = abc[2];
  const float c1sign = (c1 >= 0) ? 1.0 : -1.0;
  theta_norm = atan2(b1*c1sign, a1*c1sign);
  orientation = theta_norm + AngPi(M_PI/2);
  length = end1Pt().distanceFrom(end2Pt());
  const ReferenceFrameType_t ref = getRefFrameType();
  end1_pt.setRefFrameType(ref);
  end2_pt.setRefFrameType(ref);
  deleteRendering();
}

bool LineData::isNotVertical() const {
  const AngPi threshold = M_PI / 3;
  const AngPi orient = getOrientation();
  return (orient <= threshold) || (orient >= M_PI - threshold);
}

/*
bool LineData::isRoughlyPerpendicularTo(Shape<LineData>& other) {
  AngPi threshold = M_PI_4;
  AngPi orientation_diff = getOrientation() - other->getOrientation();
  if((orientation_diff >= threshold) && (orientation_diff < (M_PI-threshold)))
    return true;
  else
    return false;
}

bool LineData::isExactlyPerpendicularTo(Shape<LineData>& other) {
  AngPi orientation_diff = getOrientation() - other->getOrientation();
  return (orientation_diff == M_PI_2);
}

*/


// *** These functions are true based on line length. *** //

bool LineData::isLongerThan(const Shape<LineData>& other) const {
  return length > other->length; }

bool LineData::isLongerThan(float ref_length) const {
  return length > ref_length; }

bool LineData::isShorterThan(const Shape<LineData>& other) const {
  return length < other->length; }

bool LineData::isShorterThan(float ref_length) const {
  return length < ref_length; }

bool LineData::isBetween(const Point &p, const LineData &other) const {
  if (getOrientation() == other.getOrientation()) { // parallel lines
    float dl = perpendicularDistanceFrom(other.end1Pt()); // distance between the lines
    return (perpendicularDistanceFrom(p) <= dl && other.perpendicularDistanceFrom(p) <= dl);
  }
  else {
    bool b;
    const LineData p_line (*space,  p,  // line from intersection of this and other to p
			   intersectionWithLine(other, b, b));
    const AngPi theta_pline = p_line.getOrientation();
    const AngPi theta_greater = 
      (getOrientation() > other.getOrientation()) ? getOrientation() : other.getOrientation();
    const AngPi theta_smaller = 
      (getOrientation() < other.getOrientation()) ? getOrientation() : other.getOrientation();
    if (theta_greater - theta_smaller > M_PI/2)
      return (theta_pline >= theta_greater || theta_pline <= theta_smaller);
    else
      return (theta_pline <= theta_greater && theta_pline >= theta_smaller);
  }
}

	
// ***  Check intersection. *** //

bool
LineData::intersectsLine(const Shape<LineData>& other) const {
  return intersectsLine(other.getData());
}

bool
LineData::intersectsLine(const LineData& other) const {
  // Calculate F(x,y) = 0 for this line (x1,y1) to (x2,y2).
  pair<float,float> F = lineEquation_mb();
  
  // Calculate G(x,y) = 0 for L (x3,y3) to (x4,y4).
  pair<float,float> G = other.lineEquation_mb();
  
  // NOTE: These tests are assumed to be taking place in the space of
  // "this" line.  Therefore, the limits of line extent (for lines
  // with inactive endpoints) are calculated in the space of "this"
  // line.
  
  // JJW *** YOU NEED TO TAKE ACCOUNT OF END POINTS BEING TURNED OFF.
  
  // 	float end1x, end1y, end2x, end2y, other_end1x, other_end1y, other_end2x, other_end2y;
  // 	if(end1Pt().isActive()) {
  // 		end1x = end1Pt().coordX();
  // 		end1y = end1Pt().coordY();
  // 	} else {
  // 		end1x = 
  
  
  // TEST 1
  
  // Calculate r3 = F(x3,y3)
  float r3 = F.first * other.end1Pt().coordX() + F.second - other.end1Pt().coordY();
  
  // Calculate r4 = F(x4,y4)
  float r4 = F.first * other.end2Pt().coordX() + F.second - other.end2Pt().coordY();
  
  // If r3 != 0...
  // ...AND r4 != 0...
  // ...AND SGN(r3) == SGN(r4)...
  // ...THEN the lines do not intersect.
  
  if((r3 != 0)
     && (r4 != 0)
     && (SGN(r3) == SGN(r4))
     )
    return false;
  
  
  // TEST 2
  
  // Calculate r1 = G(x1,y1)
  float r1 = G.first * end1Pt().coordX() + G.second - end1Pt().coordY();
  
  // Calculate r2 = G(x2,y2)
  float r2 = G.first * end2Pt().coordX() + G.second - end2Pt().coordY(); 
  
  // If r1 != 0...
  // ...AND r2 != 0...
  // ...AND SGN(r1) == SGN(r2)...
  // ...THEN the lines do not intersect.
  
  if((r1 != 0)
     && (r2 != 0)
     && (SGN(r1) == SGN(r2))
     )
    return false;
  
  // Otherwise, the lines DO intersect.
  return true;
}


Point LineData::intersectionWithLine(const Shape<LineData>& other,
				     bool& intersection_on_this,
				     bool& intersection_on_other) const { 
  return intersectionWithLine(other.getData(), intersection_on_this,intersection_on_other); 
}

Point
LineData::intersectionWithLine(const LineData& other, 
			       bool& intersection_on_this, bool& intersection_on_other) const {

  // Based upon algorithm written by Paul Bourke, April 1989.
  // http://astronomy.swin.edu/~pbourke/geometry/lineline2d/  
  // Accessed July 20th 2004
  
  // Points 1 and 2 are on "this" line. Points 3 and 4 define the "other" line.
  //  float x1, x2, x3, x4, y1, y2, y3, y4;
  float x1, x2, x3, x4, y1, y2, y3, y4;
  x1 = end1Pt().coordX();
  x2 = end2Pt().coordX();
  x3 = other.end1Pt().coordX();
  x4 = other.end2Pt().coordX();
  // x3 = other->end1Pt().coordX();
  // x4 = other->end2Pt().coordX();
  y1 = end1Pt().coordY();
  y2 = end2Pt().coordY();
  y3 = other.end1Pt().coordY();
  y4 = other.end2Pt().coordY();
  // y3 = other->end1Pt().coordY();
  // y4 = other->end2Pt().coordY();
  
  // x1 + u_a(x2-x1) = x3 + u_b(x4-x3)
  // y1 + u_a(y2-y1) = y3 + u_b(y4-y3)
  
  // u_a = (x4-x3)(y1-y3) - (y4-y3)(x1-x3)
  //       -------------------------------
  //       (y4-y3)(x2-x1) - (x4-x3)(y2-y1)
  
  // u_b = (x2-x1)(y1-y3) - (y2-y1)(x1-x3)
  //       -------------------------------
  //       (y4-y3)(x2-x1) - (x4-x3)(y2-y1)
  
  float denom = ((y4-y3)*(x2-x1) - (x4-x3)*(y2-y1));
  float u_a_numerator = ((x4-x3)*(y1-y3) - (y4-y3)*(x1-x3));
  float u_b_numerator = ((x2-x1)*(y1-y3) - (y2-y1)*(x1-x3));
  
  // If denominators of u_a and u_b are zero, then lines are parallel.
  if(denom == 0.0) {
    if (u_a_numerator == 0.0 && u_b_numerator == 0.0) {
      PRINTF("intersectionWithLine: lines are coincident!\n");
      return(end1Pt());
    }
    else {
      cout << x1 << " " << x2 << " " << x3 << " " << x4 << " "
	   << y1 << " " << y2 << " " << y3 << " " << y4 << endl;
      cout << "this theta; " << getOrientation() << ", other theta: " << other.getOrientation() << endl;
      PRINTF("ERROR in intersectionWithLine: lines are parallel!\n");
      return(Point(-9999.0,-99999.0));
    }
  }
  
  else {
    float u_a = u_a_numerator / denom;
    float u_b = u_b_numerator / denom;
    
    // If 0 <= u_a <=1  then intersection point is on segment a.
    if(0.0<=u_a && u_a<=1.0)
      intersection_on_this = true;
    else
      intersection_on_this = false;
    
    // If 0 <= u_b <=1  then intersection point is on segment b.
    if(0.0<=u_b && u_b<=1.0)
      intersection_on_other = true;
    else
      intersection_on_other = false;
    
    return(Point((x1+u_a*(x2-x1)),
		 (y1+u_a*(y2-y1))));
  }
}


// *** Distance. *** //

float LineData::perpendicularDistanceFrom(const Point& otherPt) const {
  // NOTE that this formula is rather slow, as it involves the
  // calculation of the line equation in addition to a square root here.
  
  // Using the formula from http://www.tpub.com/math2/8.htm
  vector<float> abc = lineEquation_abc();
  float& a = abc[0];
  float& b = abc[1];
  float& c = abc[2];
  
  // Distance...
  //    (x*A + y*B - C  )
  // abs(-------------  )
  //    (sqrt(a^2 + b^2)) 
  float d = fabs((a * otherPt.coordX() + b * otherPt.coordY() - c)/sqrt(a*a + b*b));
  //  cout << "abcd: " << a << ", " << b << ", " << c << ", " << d << endl;
  return(d);
}



// ==================================================
// BEGIN LINE EXTRACTION CODE
// ==================================================

#define BEG_DIST_THRESH 2
#define END_DIST_THRESH 2

Shape<LineData> LineData::extractLine(Sketch<bool>& sketch) {
  NEW_SKETCH_N(fatmask,bool,visops::fillin(sketch,1,2,8));
  NEW_SKETCH_N(skel,bool,visops::skel(fatmask));
  return extractLine(skel, sketch);
}

Shape<LineData> LineData::extractLine(Sketch<bool>& skeleton, 
				      const Sketch<bool>& occlusions) {
  const int width = skeleton->getWidth(), height = skeleton->getHeight();
  SketchSpace &SkS = skeleton->getSpace();
  ShapeSpace &ShS = SkS.getDualSpace();

  // approximate largest skel region with line
  NEW_SKETCH_N(labels,uint,visops::oldlabelcc(skeleton,visops::EightWayConnect));
  list<Region> regions = Region::extractRegions(labels,EXTRACT_LINE_MIN_AREA);
  if ( regions.empty() ) {
    ShapeRoot invalid; // need to define a named variable to avoid warning on next line
    return ShapeRootType(invalid,LineData);
  }
  Region skelchunk = regions.front();  // regions not empty, so use the largest....
  AngPi orientation(skelchunk.findPrincipalAxisOrientation());
  Point centroid(skelchunk.findCentroid());
  // cout << " orientation=" << orientation << "  centroid=" << centroid.getCoords() << endl;
  if (! skelchunk.isContained(centroid, 3) ) {   //  region does not look like a straight line
    Shape<LineData> result(splitLine(ShS, skelchunk, skeleton, occlusions));
    if ( result.isValid() )
      return result;
  }
  
  // Create a symbolic line object.
  Shape<LineData> extracted_line(ShS, centroid, orientation);
  extracted_line->setColor(skeleton->getColor());
  extracted_line->setParentId(skeleton->getViewableId());

  // rho and theta describe the normal line.
  // normpoint is where the normal line intersects with this line.
  // normpoint may actually be off-image.
  float x_normpoint = extracted_line->rho_norm*cos(extracted_line->theta_norm);
  float y_normpoint = extracted_line->rho_norm*sin(extracted_line->theta_norm);
  
  // m is slope of this line (not the normal line)
  // float m = (y_normpoint != 0) ? -(x_normpoint/y_normpoint) : BIG_SLOPE;
  float m = max(min(tan(extracted_line->theta_norm+AngPi(M_PI/2)), (float)BIG_SLOPE), (float)(-BIG_SLOPE));
  float b = y_normpoint - m*x_normpoint;
  // cout << "  x_normpoint=" << x_normpoint << "  y_normpoint=" << y_normpoint
  // << "  m=" << m << "  b=" << b <<std::endl;
  
  NEW_SKETCH_N(skelDist,uint,visops::edist(skeleton));
  if ( abs(m) <= 1 )  // when |slope| <= 1, scan along x, else scan along y
    extracted_line->scanHorizForEndPts(skelDist,occlusions,m,b);
  else
    extracted_line->scanVertForEndPts(skelDist,occlusions,m,b);

  // Check to see if any endpoints are near any edge of the screen.
  // If they are, invalidate them, assuming that line continues beyond screen.
  extracted_line->end1_pt.checkValidity(width,height,BEG_DIST_THRESH);
  extracted_line->end2_pt.checkValidity(width,height,BEG_DIST_THRESH);

  // Transform from SketchSpace coordinates to ShapeSpace coordinates
  SkS.applyTmatinv(extracted_line->end1_pt.getCoords());
  SkS.applyTmatinv(extracted_line->end2_pt.getCoords());
  extracted_line->update_derived_properties();
  return extracted_line;
}


  // hack to split a non-straight region by kei
Shape<LineData> LineData::splitLine(ShapeSpace &ShS, Region &skelchunk, 
			  Sketch<bool> &skeleton, const Sketch<bool> &occlusions) {
  //  cout << "this region is not straight, needs be split into two regions" << endl;
  Point bounds[4] = {skelchunk.findTopBoundPoint(), skelchunk.findLeftBoundPoint(),
		     skelchunk.findRightBoundPoint(), skelchunk.findBotBoundPoint()};
  //  cout << "top(0): " << bounds[0] << endl;
  //  cout << "left(1): " << bounds[1] << endl;
  //  cout << "right(2): " << bounds[2] << endl;
  //  cout << "bottom(3): " << bounds[3] << endl;
  for (int i = 0; i < 4; i++) {
    for (int j = i+1; j < 4; j++) {
      if (bounds[i].distanceFrom(bounds[j]) > 20 && ! skelchunk.isContained((bounds[i]+bounds[j])/2.0, 3)) {
	//	cout << "[" << i << "," << j << "] form a line from which most distant point is where the region should split " << endl;
	LineData ln(ShS,bounds[i],bounds[j]);
	PointData most_distant_pt(ShS, skelchunk.mostDistantPtFrom(ln));
	//	cout << "Point of split: " << most_distant_pt.getCentroid() << endl;
	const Sketch<bool>& point_rendering = most_distant_pt.getRendering();
	uint const clear_dist = 10;
	NEW_SKETCH_N(not_too_close, bool, visops::edist(point_rendering) >= clear_dist);
	skeleton &= not_too_close;
	return extractLine(skeleton, occlusions);
      }
    }
  }
  ShapeRoot invalid; // need to define a named variable to avoid warning on next line
  return ShapeRootType(invalid,LineData);
}

void LineData::scanHorizForEndPts(const Sketch<uint>& skelDist, 
				  const Sketch<bool>& occlusions, 
				  float m, float b) {
  // Scan along the infinite line, looking for segments in the image.
  bool on_line = false;			   // true if tracing a line skeleton
  int beg_dist_thresh = BEG_DIST_THRESH;   // skel has to be <= this close to start segment
  int end_dist_thresh = END_DIST_THRESH;   // how far line should extend from skel
  int curxstart = -1;    // start of current segment we're examining
  int possxstop = -1;    // end of current segment unless we bridge a gap
  int bestxstart = -1;   // start of best (longest) segment seen so far
  int bestxstop = -1;    // end of best segment seen so far
  int bestlength = -1;   // length of best segment seen so far
  for (int x = 0, y, dist=0; x < skelDist.width; x++) {
    y = int(m*x+b);
    
    // If y is on-screen...
    if (y >= 0 && y < skelDist.height) {
      dist = skelDist(x,y);

      if (on_line == false) {   // not currently on a line segment
	if (dist <= beg_dist_thresh) {
	  // start a new segment
	  on_line = true;
	  curxstart = x - dist;
	  // if new segment begins at an occluder, back up over the occluder
	  int curystart;
	  bool backupflag = false;
	  while ( curxstart >= 0 && (curystart=int(m*curxstart+b)) >= 0 && curystart < skelDist.height )
	    if ( occlusions(curxstart,curystart) || skelDist(curxstart,curystart) == 0 ) {
	      --curxstart;
	      backupflag = true;
	    } else { // if we backed up past the occluder, go one step forward
	      curxstart += backupflag;
	      break;
	    }
	  if ( curxstart < 0) // occluder extended to left edge of image
	    curxstart = 0;
	}
      }
      else {   // on_line == true:  currently on a line segment
	const bool occ = occlusions(x,y);
	// cout << "x=" << x << "  dist=" << dist <<"  occ=" << occ;
	if ( dist <= end_dist_thresh || occ ) {
	  if ( occ )
	    possxstop = x;
	  else
	    possxstop = x - dist;
	  // cout << "  possxstop=" << possxstop;
	  if ( possxstop-curxstart > bestlength ) {
	    bestxstart = curxstart;
	    bestxstop = possxstop;
	    bestlength = bestxstop-bestxstart;
	    // cout << "  bestxstop=" << bestxstop << "  bestlength=" << bestlength;
	  }
	}
	else if ( dist > extractorGapTolerance ) {
	  // we're traversing a gap, and it just got too long
	  on_line = false;
	  // cout << "  on_line=" << on_line << "  dist=" << dist;
	};	
	// cout << endl;
      }
    }
  }

  // cout << "xstart:" << bestxstart << "xstop:" << bestxstop << endl;
  float y1 = m*bestxstart + b;
  float y2 = m*bestxstop + b;
  setEndPts(Point(bestxstart,y1), Point(bestxstop,y2));
  // cout << "m = " << m << "   b = " << b << endl;
  balanceEndPointHoriz(end1_pt,occlusions,m,b);
  balanceEndPointHoriz(end2_pt,occlusions,m,b);
}

void LineData::scanVertForEndPts(const Sketch<uint>& skelDist, 
				  const Sketch<bool>& occlusions, 
				  float m, float b) {
  // Scan along the infinite line, looking for segments in the image.
  bool on_line = false;			   // true if tracing a line skeleton
  int beg_dist_thresh = BEG_DIST_THRESH;   // skel has to be <= this close to start segment
  int end_dist_thresh = END_DIST_THRESH;   // how far line should extend from skel
  int curystart = -1;    // start of current segment we're examining
  int possystop = -1;    // end of current segment unless we bridge a gap
  int bestystart = -1;   // start of best (longest) segment seen so far
  int bestystop = -1;    // end of best segment seen so far
  int bestlength = -1;   // length of best segment seen so far
  for (int x, y = 0, dist=0; y < skelDist.height; y++) {
    x = int((y-b)/m);
    
    // If x is on-screen...
    if (x >= 0 && x < skelDist.width) {
      dist = int(skelDist(x,y));

      if (on_line == false) {   // not currently on a line segment
	if (dist <= beg_dist_thresh) {
	  // start a new segment
	  on_line = true;
	  curystart = y - dist;
	  // if new segment begins at an occluder, back up over the occluder
	  int curxstart;
	  bool backupflag = false;
	  while ( curystart >= 0 && (curxstart=int((curystart-b)/m)) >= 0 && curxstart < skelDist.width )
	    if ( occlusions(curxstart,curystart) || skelDist(curxstart,curystart) == 0 ){
	      --curystart;
	      backupflag = true;
	    } else { // if we backed up past the occluder, go one step forward
	      curystart += backupflag;
	      break;
	    }
	  if ( curystart < 0) // occluder extended to top edge of image
	    curystart = 0;
	}
      }
      else {   // on_line == true:  currently on a line segment
	const bool occ = occlusions(x,y);
	// cout << "y=" << y << "  dist=" << dist <<"  occ=" << occ;
	if ( dist <= end_dist_thresh || occ ) {
	  if ( occ )
	    possystop = y;
	  else
	    possystop = y - dist;
	  // cout << "  possystop=" << possystop;
	  if ( possystop-curystart > bestlength ) {
	    bestystart = curystart;
	    bestystop = possystop;
	    bestlength = bestystop-bestystart;
	    // cout << "  bestystop=" << bestystop << "  bestlength=" << bestlength;
	  }
	}
	else if ( dist > extractorGapTolerance ) {
	  // we're traversing a gap, and it just got too long
	  on_line = false;
	  // cout << "  on_line=" << on_line;
	};	
	// cout << endl;
      }
    }
  }

  float x1 = (bestystart-b)/m;
  float x2 = (bestystop-b)/m;
  // cout << "x1=" << x1 << ", ystart=" << bestystart << "   x2=" << x2 << ", ystop=" << bestystop << endl;
  setEndPts(Point(x1,bestystart), Point(x2,bestystop));
  // cout << "m = " << m << "   b = " << b << endl;
  balanceEndPointVert(end1_pt,occlusions,m,b);
  balanceEndPointVert(end2_pt,occlusions,m,b);
}

void LineData::balanceEndPointHoriz(EndPoint &, Sketch<bool> const &, float, float) {
  /*
  int xstep = ( pt.coordX() < max(end1_pt.coordX(),end2_pt.coordX()) ? +1 : -1 );
  int toppix = 0, bottompix = 0;
  int const balance_rows = 5;    // check this many rows from the line endpoint inward
  int const row_samples = 10; // for each row, check this many pixels each side of the line
  */
  return;
}

void LineData::balanceEndPointVert(EndPoint &pt, Sketch<bool> const &occluders, float m, float b) {
  int ystep = ( pt.coordY() < max(end1_pt.coordY(),end2_pt.coordY()) ? +1 : -1 );
  int leftpix = 0, rightpix = 0;
  int const balance_rows = 8;    // check this many rows from the line endpoint inward
  int const row_samples = 10; // for each row, check this many pixels each side of the line
  // cout << endl << " ystep=" << ystep << " coords= ( " << pt.coordX() << " , " << pt.coordY() << ")"  << endl;
  for ( int y = (int)pt.coordY(), ycnt=balance_rows ;
	y>= 0 && y<occluders.height && ycnt-- > 0 ; y+=ystep ) {
    int const xstart = (int) ((y-b)/m);
    for ( int x = xstart-row_samples; x < xstart; x++ )
      if ( x >= 0 )
	leftpix += occluders(x,y);
    for ( int x = xstart+row_samples; x > xstart; x-- )
      if ( x < occluders.width )
	rightpix += occluders(x,y);
    // cout << "   " << xstart << "/" << y << "  =  " << leftpix << " " << rightpix << endl;;
  }
   float const new_x = pt.coordX() + (rightpix-leftpix)/(2*balance_rows);
   // cout << "    leftpix=" << leftpix <<"  rightpix=" << rightpix << endl;
   pt.setCoords(new_x, pt.coordY());
}

vector<Shape<LineData> > LineData::extractLines(Sketch<bool> const& sketch,
						const int num_lines) {
  NEW_SKETCH_N(fatmask,bool,visops::fillin(sketch,1,2,8));
  NEW_SKETCH_N(skel,bool,visops::skel(fatmask));
  return extractLines(skel, sketch, num_lines);
}

vector<Shape<LineData> > LineData::extractLines(Sketch<bool> const& skel,
						Sketch<bool> const& occluders,
						const int num_lines) {
  vector<Shape<LineData> > lines_vec;
  NEW_SKETCH_N(temp,bool,visops::copy(skel))
  for (int gloop = 0; gloop<num_lines; gloop++) {  // <---- HACK TO LIMIT NUMBER OF LINES RETURNED
    Shape<LineData> newline(extractLine(temp,occluders));
    if ( !newline.isValid() ) break;
    newline->clearLine(temp);
    if (newline->isLongerThan(DEFAULT_MIN_LENGTH)) {
      lines_vec.push_back(newline);
    }
  }
  // std::cout << "extractLines returning " << lines_vec.size() << " lines." << std::endl;
  
  return lines_vec;
}

//! Clear out pixels that are on or close to this line.
void LineData::clearLine(Sketch<bool>& sketch) {
  const Sketch<bool>& line_rendering = getRendering();
  uint const clear_dist = 5;
  Sketch<bool> not_too_close = (visops::edist(line_rendering) >= clear_dist);
  sketch &= not_too_close;
} 

  

// ==================================================
// BEGIN LINE RENDERING CODE
// ==================================================

Sketch<bool>& LineData::getRendering() {
  if ( ! end1Pt().rendering_valid || ! end2Pt().rendering_valid )
    deleteRendering();
  if ( rendering_sketch == NULL )
    rendering_sketch = render();
  return *rendering_sketch;
}

//! Render line to SketchSpace and return reference.
//! This function does not link the Sketch<bool>* in the shape to the sketch returned.
Sketch<bool>* LineData::render() const {
  SketchSpace &renderspace = space->getDualSpace();
  int const width = renderspace.getWidth();
  int const height = renderspace.getHeight();
  float x1,y1,x2,y2;
  setDrawCoords(x1, y1, x2, y2, width, height);
  Sketch<bool>* draw_result = 
    new Sketch<bool>(renderspace, "render("+getName()+")");
  (*draw_result)->setParentId(getViewableId());
  (*draw_result)->setColor(getColor());
  *draw_result = 0;
  drawline2d(*draw_result, (int)x1, (int)y1, (int)x2, (int)y2);
  const_cast<LineData*>(this)->end1Pt().rendering_valid = true;
  const_cast<LineData*>(this)->end2Pt().rendering_valid = true;
  return draw_result;
}  
 
  
// This function will be called by both LineData and PolygonData renderers
void LineData::setDrawCoords(float& x1,float& y1, float& x2, float& y2,
			     const int width, const int height) const {
  EndPoint e1(end1Pt());
  EndPoint e2(end2Pt());
  space->getDualSpace().applyTmat(e1.getCoords());
  space->getDualSpace().applyTmat(e2.getCoords());
  const EndPoint &left_point = e1.coordX() <= e2.coordX() ? e1 : e2;
  const EndPoint &right_point = e1.coordX() <= e2.coordX() ? e2 : e1;

  // Check if horizontal
  if((int)left_point.coordY() == (int)right_point.coordY()) { 
    if (!left_point.isActive())
      x1 = 0;
    else x1 = max(0,(int)left_point.coordX());
    
    if (!right_point.isActive())
      x2 = width-1;
    else x2 = min(width-1, (int)right_point.coordX());
    
    y1 = (int)left_point.coordY();
    y2 = y1;
  } 
  else // Check if vertical...
    if ((int)left_point.coordX() == (int)right_point.coordX()) { 
    
    x1 = (int)left_point.coordX();
    x2 = x1;
    
    const EndPoint &top_point = end1Pt().coordY() <= end2Pt().coordY() ? end1Pt() : end2Pt();
    const EndPoint &bottom_point = end1Pt().coordY() <= end2Pt().coordY() ? end2Pt() : end1Pt();

    if(!top_point.isActive())
      y1 = 0;
    else y1 = max(0,(int)top_point.coordY());
    
    if (!bottom_point.isActive())
      y2 = height-1;
    else y2 = min(height-1,(int)bottom_point.coordY());
  } 

  else {   // Neither horizontal nor vertical...
    float m = (right_point.coordY()-left_point.coordY())/(right_point.coordX()-left_point.coordX());
    float b = left_point.coordY() - m*left_point.coordX();
    
    // find image edge intersections
    int i0x = (int)((0-b)/m);
    int ihx = (int)(((height-1)-b)/m);
    int i0y = (int)(m*0+b);
    int iwy = (int)(m*(width-1)+b);
    
    // If left point is active, set starting x and y accordingly.
    if(left_point.isActive()) {
      x1 = (int)left_point.coordX();
      y1 = (int)left_point.coordY();
    } 
    
    // If endpoint 1 extends past image edge...
    else { 
      
      // intersects left edge
      if(i0y >= 0 && i0y < height) { 
	x1 = 0;
	y1 = i0y;
      } 
      
      // intersects top or bottom edge
      else { 
	
	// intersects top first
	if (i0x < ihx) { 
	  x1 = i0x;
	  y1 = 0;
	} 
	
	// intersects bottom first
	else { 
	  x1 = ihx;
	  y1 = height-1;
	}
      }
    }
    
    // If right point is active, set starting x and y accordingly.
    if(right_point.isActive()) {
      x2 = (int)right_point.coordX();
      y2 = (int)right_point.coordY();
    } 
    else { // endpoint extends to image edge
      if(iwy >= 0 && iwy < height) { // intersects right edge
	x2 = width-1;
	y2 = iwy;
      } 
      else { // intersects top or bottom edge
	if (i0x > ihx) { // intersects top last 
	  x2 = i0x;
	  y2 = 0;
	} 
	else { // intersects bottom last
	  x2 = ihx;
	  y2 = height-1;
	}
      }
    }
  }
}

void LineData::drawline2d(Sketch<bool>& canvas, int x0, int y0, int x1, int y1) {
  int width = canvas->getWidth();
  int height = canvas->getHeight();
  
  // code below from free Graphics Gems repository, graphicsgems.org
  int d, x, y, ax, ay, sx, sy, dx, dy;
  dx = x1-x0;  ax = abs(dx)<<1;  sx = SGN(dx);
  dy = y1-y0;  ay = abs(dy)<<1;  sy = SGN(dy);
  
  x = x0;
  y = y0;
  if ( ax > ay ) {		/* x dominant */
    d = ay-(ax>>1);
    for (;;) {
      if (x >= 0 && y >= 0 && x < width && y < height)
	canvas(x,y) = true;
      
      if (x==x1)
	return;
      
      if ( d >= 0 ) {
	y += sy;
	d -= ax;
      }
      
      x += sx;
      d += ay;
    }
  }
  else {			/* y dominant */
    d = ax-(ay>>1);
    for (;;) {
      if (x >= 0 && y >= 0 && x < width && y < height)
	canvas(x,y) = true;
      
      if ( y == y1 )
	return;
      
      if ( d >= 0 ) {
	x += sx;
	d -= ay;
      }
      
      y += sy;
      d += ax;
    }
  }
}
//}


std::vector<Shape<LineData> > LineData::houghTransform(const Sketch<bool>& fat, 
						const Sketch<bool>& skinny,
						const Sketch<bool>& occlusions,
						const size_t num_lines, int minLength)
{
  std::vector<Shape<LineData> > result;
  ShapeSpace& ShS = fat->getSpace().getDualSpace();

  const int width = fat->getWidth(), height = fat->getHeight();
  const int numR = 120, numTheta = 120;
  const int numRf = 40, numThetaf = 40;
  int hsize = numR*numTheta, hsizef = numRf * numThetaf;
  int htab[hsize], hfine[hsizef]; // Hough accumulator table
  float minTheta = 0.0, maxTheta = 2*M_PI; //0.9999*M_PI;
  float minR = 0.000, maxR = sqrt((float)width*width+(float)height*height); //240.0;
  float thetaSpread = maxTheta - minTheta;
  float rSpread = maxR - minR;
  //float bestR = 0, bestTheta = 0;

  for (int i = 0; i < hsize; i++)
    htab[i] = 0; // zero accumulator table

  // build accumulator table
  float theta, r;
  int ridx;
  for (int x = 0; x < width; x++) {
    for (int y = 0; y < height; y++) {
      if (fat(x,y) == true) {
	for (int tidx = 0; tidx < numTheta ; tidx++) {
	  theta = minTheta + tidx*thetaSpread/numTheta;
	  r = (float)x*cos(theta)+(float)y*sin(theta);
	  ridx = (int)((r-minR)*(float)numR/rSpread); // Is this right calc?
	  if (ridx >= 0 && ridx < numR) // check ridx bounds
	    htab[ridx+tidx*numR]++;	
	}
      }
    }
  }


  /*float lineLen = 500;
  for (int i = 0; i < numR*numTheta; i++) {
    if (htab[i] >= minCount)
      {
	const float curR = (i%numR)*rSpread/numR + minR;
	const float curTheta = (i/numR)*thetaSpread/numTheta + minTheta;
	const float x1 = curR*cos(curTheta), y1 = curR*sin(curTheta);
	const float x2 = x1+lineLen*cos(curTheta+M_PI/2), y2 = y1+lineLen*sin(curTheta+M_PI/2);
	const float x3 = x1-lineLen*cos(curTheta+M_PI/2), y3 = y1-lineLen*sin(curTheta+M_PI/2);

	Point e1(x3,y3), e2(x2,y2);
	Shape<LineData> line(ShS,e1,e2);
	line->end1Pt().setValid(false);
	line->end2Pt().setValid(false);
	result.push_back(line);
      }
      }*/

  // Finely explore the first n best lines in the rough table
  int max_val = -1, max_i = 0;
  while(result.size() < num_lines) {

    // find maximum value in accumulator table
    max_val = -1;
    max_i = 0;
    for (int i = 0; i < numR*numTheta; i++) {
      if (max_val < htab[i]) {
	max_val = htab[i];
	max_i = i;
      }
    }

    // Give up if the lines start getting too small
    if (max_val < minLength)
      break;
    

    float bestR = (max_i%numR)*rSpread/numR + minR;
    float bestTheta = (max_i/numR)*thetaSpread/numTheta + minTheta;

    // determine parameters for the fine hough iteration
    const float fthetaSpread = M_PI/40.0; 
    const float frSpread = maxR/40.0;
    float fminTheta = bestTheta-fthetaSpread/2.0;
    float fminR = bestR-frSpread/2.0;

    for (int i = 0; i < hsizef; i++)
      hfine[i] = 0; // zero fine table
    
    // build fine table
    float thetaf, rf;
    int ridxf;
    for (int x = 0; x < width; x++) {
      for (int y = 0; y < height; y++) {
	if (skinny(x,y) == true) {
	  for (int tidx = 0; tidx < numThetaf ; tidx++) {
	    thetaf = fminTheta + tidx*fthetaSpread/numThetaf;
	    rf = (float)x*cos(thetaf)+(float)y*sin(thetaf);
	    ridxf = (int)((rf-fminR)*(float)numRf/frSpread); // Is this right calc?
	    if (ridxf >= 0 && ridxf < numRf) // check ridx bounds
	      hfine[ridxf+tidx*numRf]++;	
	  }
	}
      }
    }

    
    // Pull the best line out of the fine table
    int max_val_f = -1, max_i_f = 0;
    for (int i = 0; i < numRf*numThetaf; i++) {
      if (max_val_f < hfine[i]) {
	max_val_f = hfine[i];
	max_i_f = i;
      }
    }
    float bestRf = (max_i_f%numRf)*frSpread/numRf + fminR;
    float bestThetaf = (max_i_f/numRf)*fthetaSpread/numThetaf + fminTheta;

    // Add viable segments to the vector
    // Step along the line, looking for segments. 
    float lineLen = 500;
    // Can be simpler
    const float x1 = bestRf*cos(bestThetaf), y1 = bestRf*sin(bestThetaf);
    const float x2 = x1+lineLen*cos(bestThetaf+M_PI/2), y2 = y1+lineLen*sin(bestThetaf+M_PI/2);
    const float x3 = x1-lineLen*cos(bestThetaf+M_PI/2), y3 = y1-lineLen*sin(bestThetaf+M_PI/2);
    
    Point e1(x3,y3), e2(x2,y2);
    Shape<LineData> line(ShS,e1,e2);


    line->setColor(skinny->getColor());
    line->setParentId(skinny->getViewableId());

    // m is slope of this line (not the normal line)
    float m = (y1 != 0) ? -(x1/y1) : BIG_SLOPE;
    float b = y1 - m*x1;
    
    NEW_SKETCH_N(skelDist,uint,visops::edist(skinny));
    if ( abs(m) <= 1 )  // when |slope| <= 1, scan along x, else scan along y
      line->scanHorizForEndPts(skelDist,occlusions,m,b);
    else
      line->scanVertForEndPts(skelDist,occlusions,m,b);
    
    //! Check to see if any endpoints are near any edge of the screen.
    //! If they are, invalidate them, assuming that line continues beyond screen.
    line->end1_pt.checkValidity(width,height,BEG_DIST_THRESH);
    line->end2_pt.checkValidity(width,height,BEG_DIST_THRESH);


    // Experimental: check how much of the line actually lies along edge pixels
    // only include lines that are at least 50% above edge pixels. 
    
    float len = line->getLength();
    EndPoint start = line->end1_pt;
    EndPoint dx = (line->end2_pt - start)/len;
    float onCount = 0;
    for (float i=0; i<len; i++) {
      EndPoint cur = start+dx*i;
      if (skinny((int)(cur.coordX()), (int)(cur.coordY()))) {
	onCount++;
      }
    }


    std::cout<<"Line with "<<onCount<<" / "<<len<<" pixels";
    if (line->isLongerThan(minLength) && (onCount / len) > .5 )
      {
	std::cout<<" accepted";
	result.push_back(line);
      }
    std::cout<<std::endl;


    // Empty the corresponding bins in the rough hough table
    //std::cout<<max_i%numR<<","<<max_i/numR<<std::endl;
    for(int row = -5; row <= 5; row++)
      {
	for (int col = -(5-abs(row)); col <=5-abs(row); col++)
	  {
	    //int index = (max_i - (max_i%numR) + ((max_i+col)%numR)+row*numR + numR*numTheta)%(numR*numTheta);
	    //std::cout<<index%numR<<","<<index/numR<<" ";
	    htab[(max_i - (max_i%numR) + ((max_i+col)%numR)+row*numR + numR*numTheta)%(numR*numTheta)] = 0;
	  }
	//std::cout<<std::endl;
      }
    //std::cout<<std::endl;
  }
	
  return result;
}

bool LineData::linesParallel(Shape<LineData> l1, Shape<LineData>l2)
{
  const float maxDTheta = .15, minDThetaR = 10.0;
  float dTheta = l1->getOrientation() - l2->getOrientation(); 
  if (dTheta > M_PI_2)
    dTheta = dTheta - M_PI;
  if (abs(dTheta) < maxDTheta)
    {
      if (abs (100*dTheta + (l1->rho_norm - l2->rho_norm)) > minDThetaR)
	return true;
    }
  return false;
}

LineData& LineData::operator=(const LineData& other) {
  if (&other == this)
    return *this;
  BaseData::operator=(other);
  end1_pt = other.end1_pt;
  end2_pt = other.end2_pt;
  rho_norm = other.rho_norm;
  theta_norm = other.theta_norm;
  orientation = other.orientation;
  length = other.length;
  return *this;
}


// Compute if two points are on the same side of the line
bool LineData::pointsOnSameSide(const Point& p1, const Point& p2)
{
  float dx = end2_pt.coordX() - end1_pt.coordX();
  float dy = end2_pt.coordY() - end1_pt.coordY();
  
  float p1val = (p1.coordY() - end1_pt.coordY())*dx - (p1.coordX() - end1_pt.coordX())*dy;
  float p2val = (p2.coordY() - end1_pt.coordY())*dx - (p2.coordX() - end1_pt.coordX())*dy;

  return (p1val>0) == (p2val>0);
}

// Checks if the distance from the line to the point is less than 1 pixel,
// and checks that the point is within the end points of the line.
bool LineData::pointOnLine(const Point& p)
{
  const float BOUNDS_EXTEND = 1.0;
  float dx = end2_pt.coordX() - end1_pt.coordX();
  float dy = end2_pt.coordY() - end1_pt.coordY();
  float val = (p.coordY() - end1_pt.coordY())*dx - (p.coordX() - end1_pt.coordX())*dy;
  val /= length;
  bool inBounds = (p.coordX() >= (leftPt().coordX() - BOUNDS_EXTEND)) &&
    (p.coordX() <= (rightPt().coordX() + BOUNDS_EXTEND)) &&
    (p.coordY() >= (topPt().coordY() - BOUNDS_EXTEND)) &&
    (p.coordY() <= (bottomPt().coordY() + BOUNDS_EXTEND));
  return (val > -1) && (val < 1) && inBounds;
}


// Comparison predicates

bool LineData::LengthLessThan::operator() (const Shape<LineData> &line1, const Shape<LineData> &line2) const {
      return line1->getLength() < line2->getLength();
}

bool LineData::ParallelTest::operator() (const Shape<LineData> &line1, const Shape<LineData> &line2) const {
  return angdist(line1->getOrientation(),line2->getOrientation()) <= tolerance;
}

bool LineData::PerpendicularTest::operator() (const Shape<LineData> &line1, const Shape<LineData> &line2) const {
  return angdist(angdist(line1->getOrientation(),line2->getOrientation()), M_PI/2) <= tolerance;
}

bool LineData::ColinearTest::operator() (const Shape<LineData> &line1, const Shape<LineData> &line2) const {
  return ParallelTest(ang_tol)(line1,line2) && 
    abs( line1->getRhoNorm() - line2->getRhoNorm() ) <= dist_tol;
}

bool LineData::IsHorizontal::operator() (const Shape<LineData> &line) const {
      const AngPi orient = line->getOrientation();
      return  (orient <= threshold) || (orient >= M_PI - threshold);
    }

bool LineData::IsVertical::operator() (const Shape<LineData> &line) const {
      const AngPi orient = line->getOrientation();
      return  (orient >= threshold) && (orient <= M_PI - threshold);
    }

} // namespace
