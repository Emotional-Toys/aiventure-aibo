//-*-c++-*-

#include "ShapeRoot.h"
#include "ShapeLine.h"
#include "ShapeBlob.h"

#include "ShapeFuns.h"

namespace DualCoding {

// ================

bool IsLeftOf::operator() (const ShapeRoot &s1, const ShapeRoot &s2) const {
  const Point c1 = s1->getCentroid();
  const Point c2 = s2->getCentroid();
  return c1.isLeftOf(c2,dist);
}

bool IsAbove::operator() (const ShapeRoot &s1, const ShapeRoot &s2) const {
  const Point c1 = s1->getCentroid();
  const Point c2 = s2->getCentroid();
  return c1.isAbove(c2,dist);
}

} // namespace
