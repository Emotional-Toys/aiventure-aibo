//-*-c++-*-
#ifndef _SHAPEBRICK_H_
#define _SHAPEBRICK_H_

#include "ShapeRoot.h"
#include "BrickData.h"

namespace DualCoding {

class ShapeSpace;
class Point;

template<>
class Shape<BrickData> : public ShapeRoot {
public:
  SHAPESTUFF_H(BrickData);   // defined in ShapeRoot.h

  Shape<BrickData>(ShapeSpace &s, Point &GFL, Point &GFR, Point &GBL, Point &GBR, 
		   Point &TFL, Point &TFR, Point &TBL, Point &TBR) 
    : ShapeRoot(addShape(new BrickData(s, GFL, GFR, GBL, GBR, TFL, TFR, TBL, TBR))) {};
	
};

} // namespace

#endif
