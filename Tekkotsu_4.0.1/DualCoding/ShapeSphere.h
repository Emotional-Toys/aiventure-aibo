//-*-c++-*-
#ifndef _SHAPESPHERE_H_
#define _SHAPESPHERE_H_

#include "ShapeRoot.h"
#include "SphereData.h"

namespace DualCoding {

class ShapeSpace;
class Point;
class Region;

template<>
class Shape<SphereData> : public ShapeRoot {
public:
  SHAPESTUFF_H(SphereData);   // defined in ShapeRoot.h

  Shape<SphereData>(ShapeSpace &s, Point centerval)
    : ShapeRoot(addShape(new SphereData(s,centerval))) {};
	
  Shape<SphereData>(Region& region);
};

} // namespace

#endif
