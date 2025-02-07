#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <float.h>
#include <cmath>

#include "Shared/newmat/newmat.h"
#include "Shared/newmat/newmatio.h"

#include "ShapeRoot.h"
#include "ShapeAgent.h"
#include "ShapeLine.h"
#include "ShapeEllipse.h"
#include "ShapePoint.h"
#include "ShapeSphere.h"
#include "ShapeBlob.h"
#include "ShapePolygon.h"
#include "ShapeBrick.h"
#include "ShapePyramid.h"
#include "ShapeLocalizationParticle.h"
#include "BaseData.h"
#include "SketchSpace.h"
#include "ShapeSpace.h"
#include "VRmixin.h"

using namespace std;

namespace DualCoding {

ShapeSpace::ShapeSpace(SketchSpace* dualSkS, int init_id, std::string const _name, 
		       ReferenceFrameType_t _refFrameType) : 
  name(_name.length() == 0 ? dualSkS->name : _name),
  dualSpace(dualSkS), id_counter(init_id), shapeCache(),
  refFrameType(_refFrameType)
{
  shapeCache = std::vector<ShapeRoot>(30);
  shapeCache.clear();
}

ShapeSpace::~ShapeSpace(void) {
  //cout << "Deleting ShapeSpace " << name << " at " << this << endl;
  // do not call clear() here; VRmixin::theAgent's reference count (in AgentData) may be wrong if the static Shape was already deleted
};

ShapeRoot& ShapeSpace::addShape(BaseData *p) {
  p->id = ++id_counter;
  shapeCache.push_back(ShapeRoot(p));
  return shapeCache[shapeCache.size()-1];
};

void ShapeSpace::importShapes(std::vector<ShapeRoot>& foreign_shapes) {
  for (std::vector<ShapeRoot>::const_iterator it = foreign_shapes.begin();
       it != foreign_shapes.end();
       ++it) importShape(*it);
}

BaseData* ShapeSpace::importShape(const ShapeRoot& foreign_shape) {
  BaseData *own = foreign_shape->clone();
  own->space = this;
  own->parentId = 0;
  own->lastMatchId = foreign_shape.id;
  own->refcount = 0;
  addShape(own);
  return own;  // return value is used by MapBuilder::importLocalToWorld
}

void ShapeSpace::deleteShape(ShapeRoot &b) {
  if ( b.isValid() )
    for ( std::vector<ShapeRoot>::iterator it = shapeCache.begin();
	  it != shapeCache.end(); ++it ) {
      if ( it->id == b.id ) {
	shapeCache.erase(it);
	break;
      }
    }
  else
    std::cerr << "ERROR: Attempt to delete an invalid " << b << std::endl;
}

void ShapeSpace::deleteShapes(std::vector<ShapeRoot>& shapes_vec) {
  for (size_t i=0; i < shapes_vec.size(); i++)
    deleteShape(shapes_vec[i]);
}

void ShapeSpace::clear() {
  shapeCache.clear();
  if ( this == &VRmixin::worldShS && VRmixin::theAgent.isValid() )
    shapeCache.push_back(VRmixin::theAgent);
}

void ShapeSpace::applyTransform(const NEWMAT::Matrix& Tmat, const ReferenceFrameType_t newref) {
  std::vector<ShapeRoot> allShapes_vec = allShapes();
  const size_t nshapes = allShapes_vec.size();
  for(size_t i = 0; i < nshapes; i++)
    allShapes_vec[i]->applyTransform(Tmat,newref);
}

Point ShapeSpace::getCentroid() {
  std::vector<ShapeRoot> allShapes_vec = allShapes();
  return(getCentroidOfSubset(allShapes_vec));
}

Point ShapeSpace::getCentroidOfSubset(const std::vector<ShapeRoot>& subsetShapes_vec) {
  const size_t nshapes = subsetShapes_vec.size();
  Point subset_centroid_pt = Point(0,0);	
  for(size_t cur_shape = 0; cur_shape < nshapes; cur_shape++)
    subset_centroid_pt += subsetShapes_vec[cur_shape]->getCentroid();
  return(subset_centroid_pt/(float)nshapes);	
}

std::vector<ShapeRoot> ShapeSpace::allShapes(ShapeType_t type) {
  allShapes();  // make sure cache is valid;
  std::vector<ShapeRoot> result;
  for ( std::vector<ShapeRoot>::const_iterator it = shapeCache.begin();
	it != shapeCache.end(); it++ )
    if ( (*it)->getType() == type )
      result.push_back(*it);
  return result;
}

std::vector<ShapeRoot> ShapeSpace::allShapes(rgb color) {
  std::vector<ShapeRoot> result(shapeCache.size());
  result.clear();
  for ( std::vector<ShapeRoot>::const_iterator it = shapeCache.begin();
	it != shapeCache.end(); ++it )
    if ( (*it)->getColor() == color )
      result.push_back(*it);
  return result;
}

std::string ShapeSpace::getShapeListForGUI(void) {
  std::vector<ShapeRoot> &allShapes_vec = allShapes();
  std::ostringstream liststream;
#define writept(x) x.coordX() << " " << x.coordY() << " " << x.coordZ()
  for(unsigned int i = 0; i < allShapes_vec.size(); i++) {
    if ( allShapes_vec[i]->isViewable() ) {
      liststream << "shape" << endl;
      liststream << "id: " << allShapes_vec[i]->getId() << endl;
      liststream << "parentId: " << allShapes_vec[i]->getParentId() << endl;
      liststream << "name: " << allShapes_vec[i]->getName() << endl;
      liststream << "shapetype: " << allShapes_vec[i]->getType() << endl;
      liststream << "color: " << toString(allShapes_vec[i]->getColor()) << endl;
      liststream << "cxyz: " << writept(allShapes_vec[i]->getCentroid()) << endl;
		
      if(allShapes_vec[i]->isType(lineDataType)) { // lineshape
	const Shape<LineData>& current = ShapeRootTypeConst(allShapes_vec[i],LineData);
	liststream << "e1xyz: " << writept(current->end1Pt()) << endl; 
	liststream << "e2xyz: " << writept(current->end2Pt()) << endl; 
	liststream << "r:" << current->getRhoNorm() << endl;
	liststream << "theta: " << current->getThetaNorm() << endl;
	liststream << "end1: " << (current->end1Pt().isValid()) << " " 
		   << (current->end1Pt().isActive()) << endl;
	liststream << "end2: " << (current->end2Pt().isValid()) << " "
		   << (current->end2Pt().isActive()) << endl;
      } 
		
      else if (allShapes_vec[i]->isType(ellipseDataType)) { // ellipseshape
	const Shape<EllipseData>& current = ShapeRootTypeConst(allShapes_vec[i],EllipseData);
	liststream << "axes: " << current->getSemimajor()
		   << " " << current->getSemiminor() << endl;
	liststream << "orientation: " << current->getOrientation() 
		   << endl;
      } 
		
      else if (allShapes_vec[i]->isType(pointDataType)) { // pointshape
	;
      }
		
      else if (allShapes_vec[i]->isType(agentDataType)) { // agentshape
	const Shape<AgentData>& current = ShapeRootTypeConst(allShapes_vec[i],AgentData);
	liststream << "orientation: " << current->getOrientation() << endl;
      }
		
      else if (allShapes_vec[i]->isType(sphereDataType)) { // sphereshape
	const Shape<SphereData>& current = ShapeRootTypeConst(allShapes_vec[i],SphereData);
	liststream << "radius: " << current->getRadius() << endl;
      }
		
      else if (allShapes_vec[i]->isType(polygonDataType)) { // polygonshape
	const Shape<PolygonData>& current = ShapeRootTypeConst(allShapes_vec[i],PolygonData);
	liststream << "num_vtx: " << current->getVertices().size() << endl;
	for (std::vector<Point>::const_iterator vtx_it = current->getVertices().begin();
	     vtx_it != current->getVertices().end(); vtx_it++)
	  liststream << "vertex: " << writept((*vtx_it)) << endl;
	liststream << "end1_valid: " << (current->end1Ln().end1Pt().isValid()) << endl;
	liststream << "end2_valid: " << (current->end2Ln().end2Pt().isValid()) << endl;
      }
		
      else if (allShapes_vec[i]->isType(blobDataType)) { // blobshape
	const Shape<BlobData>& current = ShapeRootTypeConst(allShapes_vec[i],BlobData);
	liststream << "area: " << current->getArea() << endl;
	liststream << "orient: " << current->orientation << endl;
	liststream << "topLeft: " << writept(current->topLeft) << endl;
	liststream << "topRight: " << writept(current->topRight) << endl;
	liststream << "bottomLeft: " << writept(current->bottomLeft) << endl;
	liststream << "bottomRight :" << writept(current->bottomRight) << endl;
      }
      else if (allShapes_vec[i]->isType(brickDataType)) { // brickshape
	const Shape<BrickData>& current = ShapeRootTypeConst(allShapes_vec[i],BrickData);
	liststream << "GFL: " << writept(current->getGFL()) << endl;
	liststream << "GBL: " << writept(current->getGBL()) << endl;
	liststream << "GFR: " << writept(current->getGFR()) << endl;
	liststream << "GBR: " << writept(current->getGBR()) << endl;
	liststream << "TFL: " << writept(current->getTFL()) << endl;
	liststream << "TBL: " << writept(current->getTBL()) << endl;
	liststream << "TFR: " << writept(current->getTFR()) << endl;
	liststream << "TBR: " << writept(current->getTBR()) << endl;
      }
      else if (allShapes_vec[i]->isType(pyramidDataType)) { // pyramidshape
	const Shape<PyramidData>& current = ShapeRootTypeConst(allShapes_vec[i],PyramidData);
	liststream << "FL: " << writept(current->getFL()) << endl;
	liststream << "BL: " << writept(current->getBL()) << endl;
	liststream << "FR: " << writept(current->getFR()) << endl;
	liststream << "BR: " << writept(current->getBR()) << endl;
	liststream << "Top: " << writept(current->getTop()) << endl;
      }
      else if (allShapes_vec[i]->isType(localizationParticleDataType)) { // localizationparticleshape
	const Shape<LocalizationParticleData>& current = ShapeRootTypeConst(allShapes_vec[i],LocalizationParticleData);
	const float weight = current->getWeight();
	liststream << "orient/weight: " << current->getOrientation()
		   << " " << (!finite(weight) ? (weight>0 ? FLT_MAX : -FLT_MAX) : weight) << endl;
      }
    }
  }
#undef writept
  return liststream.str();	
}

void ShapeSpace::printParams() {
  cout << endl;
  cout << "SHAPE SPACE : PARAMETERS BEGIN" << endl;
  int cur, num;
  
  std::vector<ShapeRoot> &allShapes_vec = allShapes();
  num = (int)(allShapes_vec.size());
  
  for(cur = 0; cur < num; cur++) {
    
    cout << "SHAPE " <<  allShapes_vec[cur].getId() 
	 << " (" << cur+1 << " of " << num << ")" 
	 << endl;
    
    allShapes_vec[cur]->printParams();
    
    cout << "===========================================" << endl;
    cout << endl;
    
  }
  
  cout << endl;
  cout << "SHAPE SPACE : PARAMETERS END" << endl;
}

void ShapeSpace::printSummary()
{
  // JJW will this cause a memory leak?
  std::vector<ShapeRoot> allShapes_vec = allShapes();
  int cur;
  int num = (int)(allShapes_vec.size());
  cout << "SHAPE SPACE : SUMMARY BEGIN" << endl;
  std::vector<int> shape_counts;
  shape_counts.resize(numDataTypes,0);
  
  cout << endl << "Shape Listing:" << endl;
  for(cur = 0; cur < num; cur++) {
    cout << "Shape " << allShapes_vec[cur].getId() 
	 << " (" << cur+1 << " of " << num << ")."
	 << "\tType: " << allShapes_vec[cur]->getTypeName() 
	 << endl;
    shape_counts[allShapes_vec[cur]->getType()]++;
  }
  
  cout << endl << "Shape Counts:" << endl;
  num = numDataTypes;
  for(cur = 0; cur < num; cur++) {
    cout << "Shape Type " << cur << " " << data_name(cur) 
	 << ":\t" << shape_counts[cur] << endl;
  }
  cout << endl;
  
  cout << "SHAPE SPACE : SUMMARY END" << endl;
}

void ShapeSpace::deleteShapeType(ShapeType_t t) {
  std::vector<ShapeRoot> temp;
  temp.reserve(shapeCache.size());
  for ( std::vector<ShapeRoot>::const_iterator it = shapeCache.begin();
	it != shapeCache.end(); it++ )
    if ( (*it)->getType() != t )
      temp.push_back(*it);
  shapeCache = temp;
}

} // namespace
