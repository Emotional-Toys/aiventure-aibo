//-*-c++-*-

#include <math.h>

#include "Shared/Config.h"
#include "Motion/Kinematics.h"

#include "Shared/Measures.h"
#include "Point.h"
#include "LineData.h"
#include "ShapeTypes.h"  // for ReferenceFrameType_t
#include "VRmixin.h"

using namespace std;

namespace DualCoding {

  Point::Point(void) : coords(4), refFrameType(unspecified) {
  coords << 0 << 0 << 0 << 1;
  }

Point::Point(coordinate_t const &xp, coordinate_t const &yp, coordinate_t zp, 
	     ReferenceFrameType_t ref) : coords(4), refFrameType(ref) {
  coords << xp << yp << zp << 1;
};

Point::Point(const NEWMAT::ColumnVector& c, ReferenceFrameType_t ref) : 
  coords(4), refFrameType(ref) { coords = c; }

//!Set Position
//{
void Point::setCoords(const Point& otherPt) {
  coords = otherPt.coords;
}

void Point::setCoords(coordinate_t x, coordinate_t y, coordinate_t z) {
  coords << x << y << z << 1;
}
//}

float Point::distanceFrom(const Point& otherPt) const {
  return (coords - otherPt.coords).NormFrobenius();
}

float Point::xyDistanceFrom(const Point& other) const {
  float dx = coords(1)-other.coords(1);
  float dy = coords(2)-other.coords(2);
  return sqrt(dx*dx+dy*dy);
}

float Point::xyNorm() const { return sqrt(coords(1)*coords(1) + coords(2)*coords(2)); }

bool Point::isLeftOf(const Point& other, float distance) const {
  switch ( refFrameType ) {
  case camcentric:
    return coordX()+distance < other.coordX();
  case egocentric:  
    return coordY()-distance > other.coordY();
  case allocentric:
  default:
    cout << "Allocentric Point::isLeftOf fudged for now" << endl;
    return coordY()-distance > other.coordY();
    // Should really calculate bearings to both points from current
    // agent location and check sign of the difference
  }
}

bool Point::isRightOf(const Point& other, float distance) const {
  return other.isLeftOf(*this,distance); }

bool Point::isAbove(const Point& other,float distance) const {
  switch ( refFrameType ) {
  case camcentric:
    return coordY()+distance < other.coordY();
  case egocentric:  
    return coordX()-distance > other.coordX();
  case allocentric:
  default:
    cout << "Allocentric Point::isLeftOf fudged for now" << endl;
    return coordX()-distance > other.coordX();
    // Should really calculate bearings to both points from current
    // agent location and check sign of the difference
  }
}

bool Point::isBelow(const Point& other, float distance) const {
  return other.isAbove(*this,distance); }


//! Apply a transformation matrix to translate and/or rotate  the point.
void Point::applyTransform(const NEWMAT::Matrix& Tmat, const ReferenceFrameType_t newref) {
  coords = Tmat*coords;
  if ( newref != unspecified )
    refFrameType = newref;
 }

void Point::projectToGround(const NEWMAT::Matrix& camToBase,
			    const NEWMAT::ColumnVector& groundPlane) {
  NEWMAT::Matrix T2 = camToBase.i();
  T2.SubMatrix(4,4,1,3)=T2.SubMatrix(1,3,4,4).t()*T2.SubMatrix(1,3,1,3);
  T2.SubMatrix(1,3,4,4)=0;
  //cout << "Transform plane b->j:\n"<<T2;

  const float normX = 2*(coordX()/VRmixin::camSkS.getWidth()) - 1;
  const float normY = 2*(coordY()/VRmixin::camSkS.getHeight()) - 1;
  NEWMAT::ColumnVector camera_point(4), camPlane(4);
  config->vision.computeRay(normX, normY,
			    camera_point(1),camera_point(2),camera_point(3));
  camera_point(4) = 1;
  camPlane = T2*groundPlane;
  
  float denom=0;
  for(unsigned int i=1; i<=3; i++)
    denom+=camera_point(i)*camPlane(i);
  NEWMAT::ColumnVector intersect=camera_point;
  if(denom==0)
    intersect(4)=0;
  else {
    float s=camPlane(4)/denom;
    for(unsigned int i=1; i<=3; i++)
      intersect(i)*=s;
    intersect(4)=1;
  }
  coords = camToBase*intersect;
  refFrameType = egocentric;
}

void Point::projectToGround(int xres, int yres,
			    const NEWMAT::ColumnVector& ground_plane) {
#ifdef TGT_HAS_CAMERA
  // Normalize coordinate system to [-1,+1]
  const float normX = 2*(coordX()/xres) - 1;
  const float normY = 2*(coordY()/yres) - 1;
  NEWMAT::ColumnVector camera_point(4), ground_point(4);
  config->vision.computeRay(normX, normY,
			    camera_point(1),camera_point(2),camera_point(3));
  camera_point(4) = 1;
  ground_point = 
    kine->projectToPlane(CameraFrameOffset, camera_point,
			 BaseFrameOffset, ground_plane, BaseFrameOffset);
  //  std::cout << "Ground plane: " << NEWMAT::printmat(ground_plane) << 
  //    "  camera_point: " << NEWMAT::printmat(camera_point) << 
  //    "  ground_point: " << NEWMAT::printmat(ground_point) << std::endl;
  coords(1) = ground_point(1) / ground_point(4);
  coords(2) = ground_point(2) / ground_point(4);
  coords(3) = ground_point(3) / ground_point(4);
#endif
}
  

// Calling point and groundPoint must both have been projectToGround'ed already
float Point::getHeightAbovePoint(const Point& groundPoint, const NEWMAT::ColumnVector& groundplane) {
#ifndef TGT_HAS_CAMERA
	return groundPoint.coordZ();
#else
  float camX, camY, camZ;
  NEWMAT::Matrix baseToCam = kine->jointToBase(CameraFrameOffset);
  NEWMAT::ColumnVector camOffset = baseToCam.Column(4);
  //std::cout<<std::endl;
  //std::cout<<"cam offset = "<<NEWMAT::printmat(camOffset)<<std::endl;
  //std::cout<<"groundPlane = "<<NEWMAT::printmat(groundplane)<<std::endl;
  camOffset = camOffset - groundplane;
  camOffset(4) = 1;
  
  Kinematics::unpack(camOffset, camX, camY, camZ);
  //std::cout<<"top pt coords = "<<NEWMAT::printmat(coords)<<std::endl;
  float dx = camX - coords(1);
  float dy = camY - coords(2);
  float dz = camZ - coords(3);
  //std::cout<<"Cam to point = "<<dx<<" "<<dy<<" "<<dz<<std::endl;
  float dHorizCam = sqrt(dx*dx + dy*dy);

  Point dP = groundPoint - *this;
  float dHorizPoint = sqrt(dP.coordX()*dP.coordX() + dP.coordY()*dP.coordY());
  
  return dz*dHorizPoint/dHorizCam;
#endif
}


Point Point::operator+ (const Point& b) const { return Point(*this) += b; }

Point& Point::operator+= (const Point& b) {
  coords += b.coords;
  makeRefFrameCompatible(b);
  return *this;
}

Point Point::operator- (const Point& b) const { return Point(*this) -= b; }

Point& Point::operator-= (const Point& b) {
  coords -= b.coords;
  makeRefFrameCompatible(b);
  return *this;
}

void Point::makeRefFrameCompatible(const Point &other) {
  refFrameType = ( refFrameType == unspecified || refFrameType == other.refFrameType ) ?
    other.refFrameType : unspecified;
}

Point Point::operator* (float const b) const { return Point(coords*b,refFrameType); }

Point& Point::operator *= (float const b) {
  coords *= b;
  return *this;
}


Point Point::operator/ (float const b) const { return Point(coords/b,refFrameType); }

Point& Point::operator /= (float const b) {
  coords /= b;
  return *this;
}

bool Point::operator ==(const Point& b) const {
  return (coordX() == b.coordX() 
	  && coordY() == b.coordY() 
	  && coordZ() == b.coordZ());
}

std::ostream& operator<< (std::ostream& out, const Point &p) {
  switch ( p.refFrameType ) {
  case unspecified:
    out << "u:";
    break;
  case camcentric:
    out << "c:";
    break;
  case egocentric:
    out << "e:";
    break;
  case allocentric:
    out << "a:";
    break;
  }
  out << "[" << p.coordX() << ", " << p.coordY() 
      << ", " << p.coordZ() << "]";
  return out;
}

void Point::printData()
{
  cout<<"{"<<coordX()<<","<<coordY()<<","<<coordZ()<<"}";
}

} // namespace
