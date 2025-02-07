//-*-c++-*-
#ifndef _AGENTDATA_H_
#define _AGENTDATA_H_

#include "BaseData.h"    // superclass
#include "Point.h"       // Point data member
#include "Shared/Measures.h"    // coordinate_t; AngPi data member
#include "ShapeTypes.h"  // agentDataType

namespace DualCoding {

class ShapeRoot;
class SketchSpace;
template<typename T> class Sketch;

//! Representation of the robot on the world map
class AgentData : public BaseData {
private:
  Point center_pt;
  Point orientation_pt;
  AngTwoPi orientation;

public:
  //! Constructor
  AgentData(ShapeSpace& _space, const Point &c);
  
  //! Copy constructor
  AgentData(const AgentData& otherData);
  
  static ShapeType_t getStaticType() { return agentDataType; }

  // =========================================
  // BEGIN FUNCTIONS
  // =========================================
  
  DATASTUFF_H(AgentData);
  
  //! Centroid. (Virtual in BaseData.)
  Point getCentroid() const { return center_pt; }
  
  //! Match agents based on their parameters.  (Virtual in BaseData.)
  virtual bool isMatchFor(const ShapeRoot& other) const;
  
  //! Print information about this shape. (Virtual in BaseData.)
  virtual void printParams() const;
  
  virtual bool updateParams(const ShapeRoot& other, bool force=false);
  
  virtual void projectToGround(const NEWMAT::Matrix& camToBase,
			       const NEWMAT::ColumnVector& groundplane);
  
  //! Transformations. (Virtual in BaseData.)
  virtual void applyTransform(const NEWMAT::Matrix& Tmat, const ReferenceFrameType_t newref=unspecified);  

  virtual unsigned short getDimension() const { return 3; }  

  AngTwoPi getOrientation() const { return orientation; }
  
protected:
  //! Updates orientation according to #orientation_pt
  void updateOrientation();
  
private:
  friend class MapBuilder;
  //! Functions to set property values.
  //@{
  void setOrientation(AngTwoPi _orientation); //!< Don't call this; use MapBuilder::setAgent()
  void setCentroidPt(const Point &otherPt) { center_pt.setCoords(otherPt); }  //!< Don't call this; use MapBuilder::setAgent()
  //@}

  //! Render into a sketch space and return reference.
  virtual Sketch<bool>* render() const;

  AgentData& operator=(const AgentData&); //!< don't call
  
};

} // namespace

#endif
