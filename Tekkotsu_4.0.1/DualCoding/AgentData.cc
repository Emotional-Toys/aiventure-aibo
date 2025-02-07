//-*-c++-*-

#include <vector>
#include <iostream>
#include <math.h>

#include "BaseData.h"    // superclass
#include "Point.h"       // Point data member
#include "Shared/Measures.h"    // coordinate_t; AngPi data member
#include "ShapeTypes.h"  // agentDataType

#include "SketchSpace.h"
#include "Sketch.h"
#include "ShapeSpace.h"  // required by DATASTUFF_CC
#include "ShapeRoot.h"   // required by DATASTUFF_CC

#include "AgentData.h"
#include "ShapeAgent.h"

using namespace std;

namespace DualCoding {

DATASTUFF_CC(AgentData);

AgentData::AgentData(ShapeSpace& _space, const Point &c) 
  : BaseData(_space,agentDataType), center_pt(c), orientation_pt(c), orientation(0)
{ mobile = true; }
  
AgentData::AgentData(const AgentData& otherData)
  : BaseData(otherData), 
    center_pt(otherData.center_pt), 
    orientation_pt(otherData.orientation_pt),
    orientation(otherData.orientation)
{ mobile = true; }

bool AgentData::isMatchFor(const ShapeRoot&) const {
  return false;
}

//! Print information about this shape. (Virtual in BaseData.)
void
AgentData::printParams() const {
	cout << "Type = " << getTypeName();
	cout << "Shape ID = " << getId() << endl;
	cout << "Parent ID = " << getParentId() << endl;

	// Print critical points.
	cout << endl;
	cout << "center{" << getCentroid().coords(1) << ", " << getCentroid().coords(2) << "}" << endl;
		
	cout << "orientation = " << orientation << endl;
	//cout << "color = " << getColor() << endl;
	printf("color = %d %d %d\n",getColor().red,getColor().green,getColor().blue);

	cout << "mobile = " << getMobile() << endl;
	cout << "viewable = " << isViewable() << endl;
}


//! Transformations. (Virtual in BaseData.)
void AgentData::applyTransform(const NEWMAT::Matrix& Tmat, const ReferenceFrameType_t newref) {
  //  cout << "AgentData::applyTransform: " << getId() << endl;
  //  cout << Tmat << endl;
  center_pt.applyTransform(Tmat,newref);
  orientation_pt.applyTransform(Tmat,newref);
  orientation = orientation - (AngTwoPi)atan2(Tmat(1,2),Tmat(1,1));
  //  updateOrientation();
}

//! Functions to set properties.
//{
void
AgentData::setOrientation(AngTwoPi _orientation) {
  orientation = _orientation;
  orientation_pt.setCoords(getCentroid().coords(1) + 0.5*cos(orientation),
			   getCentroid().coords(2) + 0.5*sin(orientation));
  deleteRendering();
}
//}

void AgentData::updateOrientation() {
  Point heading = orientation_pt-center_pt;
  orientation = atan2(heading.coordY(),heading.coordX());
}

void AgentData::projectToGround(const NEWMAT::Matrix& camToBase,
				const NEWMAT::ColumnVector& groundplane) {
  center_pt.projectToGround(camToBase,groundplane);
  orientation_pt.projectToGround(camToBase,groundplane);
  updateOrientation();
}

bool AgentData::updateParams(const ShapeRoot& other, bool force) {
  if (isMatchFor(other) || force) {
    const AgentData& other_agent = ShapeRootTypeConst(other,AgentData).getData();
    int other_conf = other_agent.getConfidence();
    if (other_conf <= 0)
      return true;
    center_pt = (center_pt*confidence + other_agent.getCentroid()*other_conf) / (confidence+other_conf);
    orientation = orientation*((orientation_t)confidence/(confidence+other_conf))
      + other_agent.getOrientation()*((orientation_t)confidence/(confidence+other_conf));
    return true;
  }
  return false;
}

//! Render into a sketch space and return reference. (Private.)
Sketch<bool>* AgentData::render() const {
	Sketch<bool>* draw_result =
	  new Sketch<bool>(space->getDualSpace(), "render("+getName()+")");
	draw_result = 0;

	// JJW This does not take orientation into account.
	// should probably take principal axis into account later on
	int cx = int(getCentroid().coords(1));
	int cy = int(getCentroid().coords(2));
	
	// Sure the agent rendering is terribly inefficient, but it works
	const float a = 2.0;
	const float b = 2.0;
	const float x_skip = atan(1/(0.5*a)); // minimum x-diff w/o gaps 
	for(float x = (cx-a); x<(cx+a); x+=x_skip) {
		float y_y0_sq = (b*b) * (1 - (x-cx)*(x-cx)/(a*a));
		if(y_y0_sq > 0) {
			int y_bot = cy + (int)(sqrt(y_y0_sq));
			int y_top = cy - (int)(sqrt(y_y0_sq));
			(*draw_result)((int)x,y_bot) = true;
			(*draw_result)((int)x,y_top) = true;
		}
	}
	(*draw_result)(cx-(int)a, cy) = true; // fill in "holes" at ends
	(*draw_result)(cx+(int)a, cy) = true;
	return draw_result;
}

} // namespace
