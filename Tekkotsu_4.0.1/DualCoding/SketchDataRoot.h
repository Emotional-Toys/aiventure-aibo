//-*-c++-*-

#ifndef INCLUDED_SketchDataRoot_h
#define INCLUDED_SketchDataRoot_h

#include <string>

#include "Shared/ProjectInterface.h"

#include "SketchTypes.h"
#include "SketchSpace.h"
#include "ShapeRoot.h"

namespace DualCoding {

//! Parent class for SketchData<T>
class SketchDataRoot {
private:
  //! The SketchSpace that owns the pool containing this SketchData object.
  SketchSpace *space;
  
  //! Name of this sketch.
  std::string name;

  //! Sketch-specific integer ID, for unique identification
  int id;

  //! Integer ID of the "parent" Sketch, 0 if no parent; used in GUI
  int parentId;

  //! Reference count for the sketch. When SketchPool detects this as 0, it may reuse it.
  int refcount;

  //! True if the Sketch is currently viewable.
  bool viewable;

  //! Last time this sketch was included in a sketch list sent to the GUI
  int refreshTag;

  //! True if we've tried to clear this sketch but the SketchGUI was looking at it
  bool clearPending;

  //! Color to use for displaying this sketch.  Only meaningful for Sketch<bool>,
  //! but info is preserved in case we coerce bool to int or float and then back to bool.
  rgb color;

  //! Which color map to use; default is to use the robot's own color table.
  //! Other tables are used for displaying grayscale images, or scaled quantities like
  //! distance or area using a continuous color scale from red to blue.
  ColorMapType_t colormap;

  template<typename T> friend class SketchPool;
  template<typename T> friend class Sketch;
  template<typename T> friend class SketchData;
  friend class SketchRoot;

public:
  SketchDataRoot(SketchSpace *s) : 
	  space(s), name(), id(0), parentId(0), 
	  refcount(0), viewable(false), refreshTag(0), clearPending(false),
	  color((ProjectInterface::getNumColors() != -1U) ? ProjectInterface::getColorRGB(1) : rgb(0,0,255)), // color 0 is invalid, so use color 1 as default, or blue if colors aren't loaded yet
	  colormap(segMap) {}

  virtual ~SketchDataRoot() {};

  //! Returns the SketchSpace that owns the pool containing this SketchData object.
  SketchSpace& getSpace() const { return *space; }

  //! Returns the ShapeSpace associated with the SketchSpace for this Sketch
  ShapeSpace& getDualSpace() const { return space->getDualSpace(); }

  int getId() const { return id; }
  int getParentId() const { return parentId; }
  int getViewableId() const { return (isViewable() ? getId() : getParentId()); }
  void setParentId(int const _id) { parentId = _id; }
  bool isViewable() const { return viewable; }
  void setViewable(bool const v) { viewable=v; }
  int getRefcount() const { return refcount; }
  rgb getColor() const { return color; }
  void setColor(const rgb &_color) { color = _color; }
  void setColor(const std::string &colorname);
  void setColor(const color_index cindex);
  ColorMapType_t getColorMap() const { return colormap; }
  void setColorMap(const ColorMapType_t _map) { colormap = _map; }
  const std::string& getName() const { return name; }
  void setName(const std::string &_name) { name = _name; }

  virtual SketchType_t getType() const=0;

  void V(std::string const &_name="");
  void N(std::string const &_name="");

  void inheritFrom(const SketchDataRoot &parent);
  void inheritFrom(const ShapeRoot &parent);
  void inheritFrom(const BaseData &parent);

  //@{
  //! Width and height of sketches in this space.
  const size_t getWidth() const;
  const size_t getHeight() const;
  const size_t getNumPixels() const;
  //@}

  //! X coordinate encoded by sketch index
  int indexX(int index) { return index % getWidth(); }

  //! Y coordinate encoded by sketch index
  int indexY(int index) { return index / getWidth(); }

  //! converts (x,y) into a sketch index
  int indexOf(int x, int y) { return y*getWidth() + x; }

  virtual unsigned int saveBuffer(char buf[], unsigned int avail) const=0;

private:
  SketchDataRoot(const SketchDataRoot&); //!< never call this
  SketchDataRoot& operator=(const SketchDataRoot&); //!< never call this
};

} // namespace

#endif
