#include <iostream>
#include <sstream> // for ostringstream

#include "SketchSpace.h"
#include "ShapeRoot.h"
#include "ShapeSpace.h"
#include "Sketch.h"
#include "BaseData.h"
#include "ViewerConnection.h"

using namespace std;

namespace DualCoding {

SketchSpace::SketchSpace(std::string const _name,  ReferenceFrameType_t _refFrameType,
			 int const init_id, size_t const _width, size_t const _height) :
  name(_name), width(_width), height(_height), numPixels(_width*_height),
  Tmat(NEWMAT::IdentityMatrix(4)),  Tmatinv(NEWMAT::IdentityMatrix(4)),
  idCounter(1), refreshCounter(1),
  dualSpace(new ShapeSpace(this,init_id,_name,_refFrameType)),
  boolPool(this,"bool"), ucharPool(this,"uchar"), uintPool(this,"uint"), floatPool(this,"float"), 
  dummy(numPixels), idx(NULL),
  idxN(NULL), idxS(NULL), idxE(NULL), idxW(NULL), 
  idxNE(NULL), idxNW(NULL), idxSE(NULL), idxSW(NULL),
  viewer(new ViewerConnection)
{
}

void SketchSpace::requireIdx() {
  if ( idx == NULL ) {
		idx = new Sketch<uint>(*this, "idx");
		uint i = 0;
		for (size_t y = 0; y < height; y++)
			for (size_t x = 0; x < width; x++)
				setIdx(*idx, x, y, i++);
	}
}

void SketchSpace::requireIdx4way() {
  if ( idxN == NULL) {
    idxN = new Sketch<uint>(*this,"idN");
    idxS = new Sketch<uint>(*this,"idS");
    idxE = new Sketch<uint>(*this,"idE");
    idxW = new Sketch<uint>(*this,"idW");
    int i = 0;
    for (size_t y = 0; y < height; y++) {
      for (size_t x = 0; x < width; x++) {
      setIdx(*idxN, x, y, i-width);
      setIdx(*idxS, x, y, i+width);
      setIdx(*idxW, x, y, i-1);
      setIdx(*idxE, x, y, i+1);
      i++;
      }
    }
  }
}

void SketchSpace::requireIdx8way() {
  requireIdx4way();
  if ( idxNE == NULL) {
    idxNE = new Sketch<uint>(*this,"idNE");
    idxNW = new Sketch<uint>(*this,"idNW");
    idxSE = new Sketch<uint>(*this,"idSE");
    idxSW = new Sketch<uint>(*this,"idSW");
    int i = 0;
    for (size_t y = 0; y < height; y++) {
      for (size_t x = 0; x < width; x++) {
      setIdx(*idxNE, x, y, i-width+1);
      setIdx(*idxNW, x, y, i-width-1);
      setIdx(*idxSE, x, y, i+width+1);
      setIdx(*idxSW, x, y, i+width-1);
      i++;
      }
    }
  }
}

void SketchSpace::freeIndexes() {
  delete idx;
  idx=NULL;
  delete idxN; delete idxS; delete idxE; delete idxW; 
  idxN=idxS=idxE=idxW=NULL;
  delete idxNE; delete idxNW; delete idxSE; delete idxSW;
  idxNE=idxNW=idxSE=idxSW=NULL;
}


void SketchSpace::resize(const size_t new_width, const size_t new_height) {
	// delete all the old stuff
	freeIndexes();
  boolPool.deleteElements();
  ucharPool.deleteElements();
  uintPool.deleteElements();
  floatPool.deleteElements();
	// now set the new dimensions
	width = new_width;
	height = new_height;
	numPixels = new_width * new_height;
	dummy = numPixels;
}

SketchSpace::~SketchSpace() { 
  delete dualSpace;
  //printf("Destroying SketchSpace %s at 0x%p\n",name.c_str(),this);
  freeIndexes();
  delete viewer;
}


void SketchSpace::dumpSpace() const {
  boolPool.dumpPool();
  ucharPool.dumpPool();
  uintPool.dumpPool();
  floatPool.dumpPool();
}

void SketchSpace::clear() {
  boolPool.clear();
  ucharPool.clear();
  uintPool.clear();
  floatPool.clear();
}

void SketchSpace::setTmat(const NEWMAT::Matrix &mat) {
  Tmat = mat;
  Tmatinv = mat.i();
}

void SketchSpace::setTmat(float scale, float tx, float ty) {
  NEWMAT::Matrix mat(4,4);
  mat << 1 << 0 << 0 << tx
      << 0 << 1 << 0 << ty
      << 0 << 0 << 1 << 0
      << 0 << 0 << 0 << 1/scale;
  setTmat(mat);
}

void SketchSpace::setTmat(const BoundingBox &b) {
  float const xscale = (b.xmax - b.xmin) / width;
  float const yscale = (b.ymax - b.ymin) / height;
  float const scale = min(xscale,yscale);
  setTmat(scale, -b.xmin, -b.ymin);
}    

void SketchSpace::applyTmat(NEWMAT::ColumnVector &vec) {
  vec = Tmat * vec;
  if ( vec(4) != 0 )
    vec = vec / vec(4);
}

void SketchSpace::applyTmatinv(NEWMAT::ColumnVector &vec) {
  vec = Tmatinv * vec;
  if ( vec(4) != 0 )
    vec = vec / vec(4);
}

void SketchSpace::setIdx(Sketch<uint>& indices, int const x, int const y, int const indexval) {
  int const indexval_x = indexval % width;
  int const indexval_y = indexval / width;
  indices(x,y) = (indexval_x < 0 || indexval_y < 0 || indexval_x >= (int)width || indexval_y >= (int)height
									|| abs(indexval_x-x)+1 == (int)width) // loop-around check
		? dummy : indexval;
}

std::string SketchSpace::getTmatForGUI() {
  std::ostringstream tmat_stream;
  tmat_stream << "tmat" << endl;
  for (int i=1; i<5; i++)
    for (int j=1; j<5; j++)
      tmat_stream << " " << Tmat(i,j);
  tmat_stream << endl;
  return tmat_stream.str();
}

std::string SketchSpace::getSketchListForGUI() {
	bumpRefreshCounter();
	std::string sketchlist;
	sketchlist += boolPool.getSketchListForGUI();
	sketchlist += ucharPool.getSketchListForGUI();
	sketchlist += uintPool.getSketchListForGUI();
	sketchlist += floatPool.getSketchListForGUI();
	return sketchlist;	
}

SketchDataRoot* SketchSpace::retrieveSketch(int const id) {
  SketchDataRoot* sketchp;
  // Try each pool in turn until we find it.
  sketchp = boolPool.retrieveSketch(id);
  if(sketchp) return sketchp;
  sketchp = ucharPool.retrieveSketch(id);
  if(sketchp) return sketchp;
  sketchp = uintPool.retrieveSketch(id);
  if(sketchp) return sketchp;
  sketchp = floatPool.retrieveSketch(id);
  return sketchp;
}

} // namespace
