//-*-c++-*-

#include <iostream>
#include <vector>
#include <fstream>

#include "ShapeRoot.h"
#include "ShapeLine.h"
#include "ShapeEllipse.h"
#include "ShapeBlob.h"
#include "ShapePoint.h"

#include "ParticleShapes.h"

using namespace std;

namespace DualCoding {

void PfRoot::loadLms(const std::vector<ShapeRoot> &lms, bool isWorld, std::vector<PfRoot*>& landmarks){
  int id;
  int type;
  rgb color;
  bool mobile;
  deleteLms(landmarks);
  landmarks.reserve(lms.size());
  for (unsigned int i = 0; i<lms.size(); i++){
    type = lms[i]->getType();
    color = lms[i]->getColor();
    mobile = lms[i]->getMobile();
    if (type == lineDataType) {
      const Shape<LineData> &line = ShapeRootTypeConst(lms[i],LineData);
      id = line->getId();
      const EndPoint &pt1 = line->end1Pt();
      const EndPoint &pt2 = line->end2Pt();
      PfLine *land = new PfLine(id, color, mobile, pt1.coordX(), pt1.coordY(), 
				pt2.coordX(), pt2.coordY(), pt1.isValid(), pt2.isValid());
      land->link = &lms[i];
      land->orientation = atan2(pt2.coordY()-pt1.coordY(), pt2.coordX()-pt1.coordX());
      land->length = sqrt((pt2.coordX()-pt1.coordX())*(pt2.coordX()-pt1.coordX()) +
			  (pt2.coordY()-pt1.coordY())*(pt2.coordY()-pt1.coordY()));
      landmarks.push_back(land);
      if ( isWorld ) {
	PfLine *land2 = new PfLine(id, color, mobile, pt2.coordX(), pt2.coordY(),
				   pt1.coordX(), pt1.coordY(), pt2.isValid(), pt1.isValid());
	land2->link = &lms[i];
	land2->orientation = land->orientation;
	land2->length = land->length;
	landmarks.push_back(land2);
      }
    }
    else if (type == ellipseDataType) {
      const Shape<EllipseData> &ellipse = ShapeRootTypeConst(lms[i],EllipseData);
      id = ellipse->getId();
      const Point &pt = ellipse->getCentroid();
      PfEllipse* land = new PfEllipse(id, color, mobile, pt.coordX(), pt.coordY());
      land->link = &lms[i];
      landmarks.push_back(land);
    }
    else if (type == pointDataType) {
      const Shape<PointData> &point = ShapeRootTypeConst(lms[i],PointData);
      id = point->getId();
      const Point &pt = point->getCentroid();
      PfPoint* land = new PfPoint(id, color, mobile, pt.coordX(), pt.coordY());
      land->link = &lms[i];
      landmarks.push_back(land);
    }
    else if (type == blobDataType) {
      const Shape<BlobData> &blob = ShapeRootTypeConst(lms[i],BlobData);
      id = blob->getId();
      Point pt = blob->getCentroid();
      PfBlob* land = new PfBlob(id, color, mobile, pt.coordX(), pt.coordY());
      land->link = &lms[i];
      landmarks.push_back(land);
    }
  }
}

void PfRoot::deleteLms(std::vector<PfRoot*>& vec) {
	for ( unsigned int i = 0; i < vec.size(); i++ )
		delete vec[i];
	vec.clear();
}

void PfRoot::findBounds(const std::vector<PfRoot*> &landmarks, 
			 coordinate_t &xmin, coordinate_t &ymin,
			 coordinate_t &xmax, coordinate_t &ymax) {
  if ( landmarks.size() == 0 ) {  // should never get here
    cout << "Error in PFRoot::findBounds -- empty landmark vector" << endl;
    return;
  }
  xmin = xmax = landmarks[0]->x;
  ymin = ymax = landmarks[0]->y;
  for ( size_t i = 1; i<landmarks.size(); i++ ) {
    if ( (*landmarks[i]).x < xmin )
      xmin = landmarks[i]->x;
    else if  ( landmarks[i]->x > xmax )
      xmax = landmarks[i]->x;
    if ( landmarks[i]->y < ymin )
      ymin = landmarks[i]->y;
    else if  ( landmarks[i]->y > ymax )
      ymax = landmarks[i]->y;
  }
}

void PfRoot::printLms(const std::vector<PfRoot*> &landmarks) {
  for (unsigned int i = 0; i<landmarks.size(); i++)
    cout << *landmarks[i] << endl;
}

void PfRoot::printRootInfo(std::ostream &os) const {
    os << "id=" << id
       << ", x=" << x
       << ", y=" << y
       << ", mobile=" << mobile;
  }

ostream& operator<<(std::ostream &os, const PfRoot &obj) {
  obj.print(os);
  return os;
}

void PfLine::print(std::ostream &os) const {
  os << "PfLine(";
  printRootInfo(os);
  os << ", x2=" << x2
     << ", y2=" << y2
     << ", length=" << length
     << ")";
}

void PfEllipse::print(std::ostream &os) const {
  os << "PfEllipse(";
  printRootInfo(os);
  os << ")";
}

void PfPoint::print(std::ostream &os) const {
  os << "PfPoint(";
  printRootInfo(os);
  os << ")";
}

void PfBlob::print(std::ostream &os) const {
  os << "PfBlob(";
  printRootInfo(os);
  os << ")";
}


} // namespace
