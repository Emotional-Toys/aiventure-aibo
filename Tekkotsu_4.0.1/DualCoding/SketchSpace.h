//-*-c++-*-
#ifndef INCLUDED_SketchSpace_h
#define INCLUDED_SketchSpace_h

#include "Shared/newmat/newmat.h"

#include "Macrodefs.h"
#include "SketchTypes.h"
#include "ShapeTypes.h"
#include "SketchPool.h"
#include "SketchIndices.h"

namespace DualCoding {

class BoundingBox;
class ShapeSpace;
template<typename T> class Sketch;
class ViewerConnection;

//! Holds a collection of sketches of various types
/*! All the sketches in a SketchSpace have the same dimensions (width and height).
   They are organized into pools, managed by SketchPool<T> instances.
   Each SketchSpace has a dual, called a ShapeSpace.  Several standard
   SketchSpace/ShapeSpace pairs are built in to VisualRoutinesBehavior.
   The most basic is @a camSkS, the camera sketch space.   */

class SketchSpace {
public:
  std::string name;    //!< name of this SketchSpace

private:
  size_t width;	  //!< pixels along x axis
  size_t height;  //!< pixels along y axis
  size_t numPixels; //!< total pixels = width * height
  NEWMAT::Matrix Tmat; //!< transformation matrix for rendering shapes
  NEWMAT::Matrix Tmatinv; //!< inverse transformation matrix for extracting shapes
    
  int idCounter; //!< Incremented with each new Sketch, to guarantee a unique ID.
  int refreshCounter; //!< Incremented each time SketchGUI refreshes the sketch/shape list
  
  ShapeSpace* dualSpace; //!< Pointer to the ShapeSpace associated with this SketchSpace
  
  //! Pool for one of the SketchData<T> classes
  //@{
  SketchPool<bool>  boolPool;
  SketchPool<uchar> ucharPool;
  SketchPool<uint> uintPool;
  SketchPool<float> floatPool; 
  //@}
  
public:
  //! The value assigned to out-of-bounds indices: @a numPixels, i.e., one beyond the last image pixel.
  int dummy;

  //!@name Pre-generated indices for different directions
  //@{
  Sketch<uint> *idx;
  Sketch<uint> *idxN;
  Sketch<uint> *idxS;
  Sketch<uint> *idxE;
  Sketch<uint> *idxW;
  Sketch<uint> *idxNE;
  Sketch<uint> *idxNW;
  Sketch<uint> *idxSE;
  Sketch<uint> *idxSW;
  //@}
  
  SketchSpace(std::string const _name, ReferenceFrameType_t _refFrameType,
	      int const init_id, size_t const _width, size_t const _height);
  
  ~SketchSpace();
  
  ShapeSpace& getDualSpace() const { return *dualSpace; }
  
  //! dumps contents of sketch space
  void dumpSpace() const;
  
  //! Clears out viewable Sketches.
  void clear();

  //! returns the width of contained images, in pixels
  size_t getWidth() const { return width; }
  //! returns the height of contained images, in pixels
  size_t getHeight() const { return height; }
  //! returns the number of pixels of images in this space
  size_t getNumPixels() const { return numPixels; }
  
  int getRefreshCounter() const { return refreshCounter; }
  void bumpRefreshCounter() { ++refreshCounter; }

  //! creates #idx if needed
  void requireIdx();
	
  //! creates #idxN, #idxS, #idxE, and #idxW if needed
  void requireIdx4way();

  //! creates #idxNE, #idxNW, #idxSE, and #idxSW, plus NSEW cases via requireIdx4way(), if needed
  void requireIdx8way();
	
  //! frees the idx members
  void freeIndexes();

	//! change the size of sketches in this sketch space (discards all existing sketches)
	void resize(const size_t new_width, const size_t new_height);

  //! return the ShapeSpace-to-SketchSpace coordinate transformation matrix
  NEWMAT::Matrix& getTmat() { return Tmat; }

  //! return the SketchSpace-to-ShapeSpace coordinate transformation matrix
  NEWMAT::Matrix& getTmatinv() { return Tmatinv; }

  //! set the ShapeSpace-to-SketchSpace coordinate transformation matrix
  void setTmat(const NEWMAT::Matrix &mat);

  void setTmat(float scale=1, float tx=0, float ty=0);

  void setTmat(const BoundingBox &b);

  //! apply the ShapeSpace-to-SketchSpace coordinate transformation to a vector
  void applyTmat(NEWMAT::ColumnVector &vec);

  //! apply the SketchSpace-to-ShapeSpace coordinate transformation to a vector
  void applyTmatinv(NEWMAT::ColumnVector &vec);

  //! Returns the SketchPool of appropriate type for the calling Sketch
  //@{
  SketchPool<bool>&  get_pool(const Sketch<bool>&)  { return boolPool; }
  SketchPool<uchar>& get_pool(const Sketch<uchar>&) { return ucharPool; }
  SketchPool<uint>&  get_pool(const Sketch<uint>&)  { return uintPool; }
  SketchPool<float>& get_pool(const Sketch<float>&) { return floatPool; }
  
  //@}
  
  //!@name SketchGUI interface
  //@{
  
  //! Socket and port info for communication with a sketch viewer GUI.
  ViewerConnection *viewer;

  //! Returns a string describing the shape-to-sketch transformation matrix.
  std::string getTmatForGUI();

  //! Returns a string containing a list of all the sketches and their attributes
  std::string getSketchListForGUI();

  //! Returns a pointer to the sketch with specified ID number; null if not found
  SketchDataRoot* retrieveSketch(int const id);
  //@}
  
protected:
  //! helper function to ensure indices of idx Sketches are proper
  void setIdx(Sketch<uint>& indices, int const x, int const y, int const indexval);
  
  // We don't want clients accidentally copying or assigning SketchSpace.
  SketchSpace(const SketchSpace&); //!< never call this
  SketchSpace& operator= (const SketchSpace&); //!< never call this
};

} // namespace

#endif
