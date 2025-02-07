//-*-c++-*-
#include <iostream>
#include <vector>
#include <list>
#include <math.h>

#include "BaseData.h"    // superclass
#include "Point.h"       // Point data member
#include "Shared/Measures.h"    // coordinate_t; AngPi data member
#include "ShapeTypes.h"  // ellipseDataType

#include "SketchSpace.h"
#include "Sketch.h"
#include "Region.h"
#include "visops.h"

#include "ShapeSpace.h"  // required by DATASTUFF_CC
#include "ShapeRoot.h"   // required by DATASTUFF_CC

#include "EllipseData.h"
#include "ShapeEllipse.h"

using namespace std;

namespace DualCoding {

inline int round(float x) { return (int) ceil((double)x-0.5f); }

  EllipseData::EllipseData(ShapeSpace& _space, const Point &c, 
			   const float _semimajor, const float _semiminor, const float _orientation) 
  : BaseData(_space, getStaticType()),
    center_pt(c), semimajor(_semimajor), semiminor(_semiminor), orientation(_orientation)
{ center_pt.setRefFrameType(getRefFrameType());
  mobile = ELLIPSE_DATA_MOBILE; }
  
DATASTUFF_CC(EllipseData);

BoundingBox EllipseData::getBoundingBox() const {
  float o_sin = sin(orientation);
  float o_cos = cos(orientation);
  Point major_tip(semimajor*o_cos, semimajor*o_sin);
  Point minor_tip(-semiminor*o_sin, semiminor*o_cos);
  return BoundingBox(BoundingBox(BoundingBox(center_pt+major_tip),
				 BoundingBox(center_pt-major_tip)),
		     BoundingBox(BoundingBox(center_pt+minor_tip),
				 BoundingBox(center_pt-minor_tip)));
}

bool EllipseData::isMatchFor(const ShapeRoot& other) const {
  if (!(isSameTypeAs(other) && isSameColorAs(other)))
    return false;
  const Shape<EllipseData>& other_ellipse = ShapeRootTypeConst(other,EllipseData);
  float dist = center_pt.distanceFrom(other_ellipse->centerPt());
  return dist < 2*max(semimajor,other_ellipse->semimajor); // *** DST hack
}

bool EllipseData::updateParams(const ShapeRoot& other, bool) {
  const Shape<EllipseData>& other_ellipse = ShapeRootTypeConst(other,EllipseData);
  if (other_ellipse->confidence <= 0)
    return false;
  const int other_conf = other_ellipse->confidence;
  center_pt = (center_pt*confidence + other_ellipse->centerPt()*other_conf) / (confidence+other_conf);
  semimajor = (semimajor*confidence + other_ellipse->getSemimajor()*other_conf) / (confidence+other_conf);
  semiminor = (semiminor*confidence + other_ellipse->getSemiminor()*other_conf) / (confidence+other_conf);
  orientation = orientation*((orientation_t)confidence/(confidence+other_conf))
    + other_ellipse->getOrientation()*((orientation_t)confidence/(confidence+other_conf));
  return true;
}

//! Print information about this shape. (Virtual in BaseData.)
void EllipseData::printParams() const {
  cout << "Type = " << getTypeName() << "  ID=" << getId() << "  ParentID=" << getParentId() << endl;
  printf("  color = %d %d %d\n",getColor().red,getColor().green,getColor().blue);
  cout << "  center = " << centerPt() << endl;
  cout << "  semimajor = " << getSemimajor()
       << ", semiminor = " << getSemiminor()
       << ", orientation = " << getOrientation() << endl;
  cout << "  mobile = " << getMobile() << ", viewable = " << isViewable() << endl;
}

pair<Point,Point> EllipseData::findFeaturePoints() const {
  AngPi theta = getOrientation();
  NEWMAT::ColumnVector from_center(4);

  float dl = getSemimajor();
  from_center << dl*cos(theta) << dl*sin(theta) << 0 << 0;
  Point majorPt(from_center);
  majorPt += center_pt;

  dl = getSemiminor();
  theta = theta + (AngPi) (M_PI/2);
  from_center << dl*sin(theta) << dl*cos(theta) << 0 << 0;
  Point minorPt(from_center);
  minorPt += center_pt;
  return pair<Point,Point>(majorPt,minorPt);
}

//! Transformations. (Virtual in BaseData.)
void EllipseData::applyTransform(const NEWMAT::Matrix& Tmat, const ReferenceFrameType_t newref) {
  pair<Point,Point> featurePts = findFeaturePoints();
  center_pt.applyTransform(Tmat,newref);
  //  orientation = orientation - (AngTwoPi)atan2(Tmat(1,2),Tmat(1,1));
  featurePts.first.applyTransform(Tmat,newref);
  featurePts.second.applyTransform(Tmat,newref);
  updateProperties(featurePts.first, featurePts.second);
}

void EllipseData::updateProperties(const Point& majorPt, const Point& minorPt) {
  setSemiminor(minorPt.xyDistanceFrom(center_pt));
  setSemimajor(majorPt.xyDistanceFrom(center_pt));
  setOrientation(atan2(majorPt.coords(2)-center_pt.coords(2),majorPt.coords(1)-center_pt.coords(1)));
}

void EllipseData::projectToGround(const NEWMAT::Matrix& camToBase,
				  const NEWMAT::ColumnVector& groundplane) {
  pair<Point,Point> featurePts = findFeaturePoints();
  center_pt.projectToGround(camToBase,groundplane);
  featurePts.first.projectToGround(camToBase,groundplane);  
  featurePts.second.projectToGround(camToBase,groundplane);
  updateProperties(featurePts.first, featurePts.second);
}

//! Functions to set properties.
//{
void EllipseData::setOrientation(const AngPi _orientation) {
  orientation = AngPi(_orientation);
  deleteRendering();
}

void EllipseData::setSemimajor(float _semimajor) {
  semimajor = _semimajor;
  deleteRendering();
}

void EllipseData::setSemiminor(float _semiminor) {
  semiminor = _semiminor;
  deleteRendering();
}
//}


// ==================================================
// BEGIN SKETCH MANIPULATION AND LINE EXTRACTION CODE
// ==================================================


//! Ellipse extraction.

std::vector<Shape<EllipseData> > EllipseData::extractEllipses(const Sketch<bool>& sketch)
{
  const float AREA_TOLERANCE = 0.5;
  const int REGION_THRESH = 25;
  NEW_SKETCH_N(labels,uint,visops::oldlabelcc(sketch,visops::EightWayConnect));
  list<Region> regionlist = Region::extractRegions(labels,REGION_THRESH);
  std::vector<Shape<EllipseData> > ellipses;
  
  if(regionlist.empty())
    return ellipses;
  
  typedef list<Region>::iterator R_IT;
  for (R_IT it = regionlist.begin(); it != regionlist.end(); ++it) {
    float ratio = it->findSemiMajorAxisLength()/(float)(it->findSemiMinorAxisLength());
    if((ratio < 2.0) && (ratio > 1.0/(float)2.0)
       && (it->findArea() > M_PI*2.0*(it->findSemiMajorAxisLength())
	   *2.0*(it->findSemiMinorAxisLength())*AREA_TOLERANCE/4.0)) {
      Shape<EllipseData> temp_ellipse(*it);
      temp_ellipse->setParentId(sketch->getViewableId());
      temp_ellipse->setColor(sketch->getColor());
      ellipses.push_back(Shape<EllipseData>(temp_ellipse));
    };
  }
  return ellipses;
}


//! Render into a sketch space and return reference. (Private.)
Sketch<bool>* EllipseData::render() const {
  SketchSpace &SkS = space->getDualSpace();
  NEWMAT::ColumnVector ctr(centerPt().getCoords());
  SkS.applyTmat(ctr);
  const float &cx = ctr(1);
  const float &cy = ctr(2);
  const NEWMAT::Matrix &Tmat = SkS.getTmat();
  NEWMAT::ColumnVector ori(2);
  ori << cos(orientation) << sin(orientation);
  NEWMAT::Matrix rot(2,2);
  rot(1,1) = Tmat(1,1);
  rot(1,2) = Tmat(1,2);
  rot(2,1) = Tmat(2,1);
  rot(2,2) = Tmat(2,2);
  ori = rot * ori;
  const float &cosT = ori(1);
  const float &sinT = ori(2);
  const float xRange = semimajor / Tmat(4,4);
  const float majorSq = xRange*xRange;
  const float mnrDevMjr = semiminor/semimajor;
  Sketch<bool> result(SkS, "render("+getName()+")");
  result = 0;
  for (float xDist = -xRange; xDist <= xRange; xDist+=0.2) {
    const float yRange = sqrt(max((float)0, majorSq - xDist*xDist)) * mnrDevMjr;
    for (float yDist = -yRange; yDist <= yRange; yDist+=0.2) {
      int const px = round(cx+xDist*cosT-yDist*sinT);
      int const py = round(cy+yDist*cosT+xDist*sinT);
      if ( px >= 0 && px < result.width &&
	   py >= 0 && py < result.height )
	result(px,py) = true;
    }
  }
  return new Sketch<bool>(result);
}

} // namespace
