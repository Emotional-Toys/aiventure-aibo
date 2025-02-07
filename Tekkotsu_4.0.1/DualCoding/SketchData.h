//-*-c++-*-

#ifndef INCLUDED_SketchData_h
#define INCLUDED_SketchData_h

#include <valarray>
#include <iostream>
#include <stdexcept>
#include <limits> // needed for findMinPlus
#include <map>

#include "Shared/LoadSave.h"
#include "Shared/Config.h"
#include "Shared/debuget.h" // needed for ASSERT macros
#include "Vision/SegmentedColorGenerator.h"
#include "Shared/ProjectInterface.h" // needed for defSegmentedColorGenerator

#include "SketchTypes.h"
#include "SketchDataRoot.h"
#include "ViewerConnection.h"

namespace DualCoding {

class SketchSpace;
template<typename T> class SketchPool;
template<typename T> class Sketch;

//! Holds the pixels for an individual sketch.

/*! SketchData<T> holds the pixels for an individual sketch of type T, using a valarray<T>.  A collection of
    SketchData<T> objects is maintained in a SketchPool<T>.  Basic functions such as indexed access and empty test
    that are not implemented as Sketch operators are implemented as SketchData<T> member functions.  Sketch<T>
    overrides the -> operator to provide "smart pointer" access to these SketchData<T> functions.   */

template<class T>
class SketchData : public SketchDataRoot {
  //! the valarray which actually stores the image
  std::valarray<T> pixels;

  friend class Sketch<T>;
  friend class SketchPool<T>;

public:
  //! Constructor.  Don't call this.  SketchData objects should only be created and managed by their SketchSpace
  SketchData(SketchSpace *_space);
  ~SketchData();

  //! The type of this sketch.
  virtual SketchType_t getType() const;  // must go here, not in SketchDataRoot, due to templating

  //! Address of the memory area containing the actual pixel data.
  T* getRawPixels() { return &(pixels[0]); }

  //! Address of the memory area containing the actual pixel data.
  const T* getRawPixels() const { return &(pixels[0]); }

  //! Serializes a sketch to a buffer, for transmission to the sketch viewer.
  unsigned int saveBuffer(char buf[], unsigned int avail) const;

  //@{
  //! Indexed access, with bounds checking
  T& at(size_t x);

  //! Subscripted (x,y) access, with bounds checking
  T& at(size_t x, size_t y);

  //@}

  //! Returns true if all pixels are zero.
  bool empty() const;

  //!@name Sum/Max/Min
  //@{

  //! Sum of pixels
  T sum() const;

  //! Max of pixel values
  T max() const;

  //! Index of first maximum-value pixel
  int findMax() const;

  //! Min of pixel values
  T min() const;

  //! Index of first minimum-value pixel
  int findMin() const;

  //! Min of non-zero pixel values
  T minPlus() const;

  //! Index of first minimum non-zero pixel, or -1 if none
  int findMinPlus() const;

  //! Mode (most common) pixel value
  T mode() const;

  //! Mode (most common) non-zero pixel value
  T modePlus() const;

  //@}

protected:
  unsigned int savePixels(char buf[], unsigned int avail) const; //!< handle copying pixels to buffer

private:
  SketchData (const SketchData& other); //!< never call this
};

} // namespace

#include "SketchSpace.h"

namespace DualCoding {

template<class T>
SketchData<T>::SketchData(SketchSpace *_space) :
  SketchDataRoot(_space), pixels(_space->getNumPixels()+1) {
  if  ( getType() == sketchUint || getType() == sketchFloat )
    colormap = jetMapScaled;
  else
    colormap = segMap;
}

template<class T> SketchData<T>::~SketchData() {}

template <class T>
SketchData<T>::SketchData (const SketchData<T> &other)
  : SketchDataRoot(other.space), pixels(other.pixels) {}

template<>
inline SketchType_t SketchData<bool>:: getType() const { return sketchBool; }
template<>
inline SketchType_t SketchData<uchar>::getType() const { return sketchUchar; }
template<>
inline SketchType_t SketchData<uint>::getType() const { return sketchUint; }
template<>
inline SketchType_t SketchData<float>::getType() const { return sketchFloat; }

template<class T>
T& SketchData<T>::at(size_t x) {
  if ( x < (unsigned int)(space->getWidth()*space->getHeight()) )
    return pixels[x];
  else
    throw std::out_of_range("Sketch subscript out of bounds");
}

template <class T>
T& SketchData<T>::at(size_t x, size_t y) {
  if ( x < (unsigned int)(space->getWidth()) && y < (unsigned int)(space->getHeight()) )
    return pixels[y*space->getWidth()+x];
  else
    throw std::out_of_range("Sketch subscript out of bounds");
}

template <class T>
bool SketchData<T>::empty() const {
  for ( size_t i=0; i < pixels.size(); i++ )
    if ( pixels[i] != 0 )
      return false;
  return true;
}

template <class T>
T SketchData<T>::sum() const {
  T result = pixels[0];
  for (unsigned int i=1; i<getNumPixels(); i++)
    result += pixels[i];
  return result;
}

template <class T>
T SketchData<T>::max() const {
  T result = pixels[0];
  for (unsigned int i=1; i<getNumPixels(); i++)
    result = std::max(result,pixels[i]);
  return result;
}

template <class T>
int SketchData<T>::findMax() const {
  T maxval = pixels[0];
  int maxidx = -1;
  for (unsigned int i = 1; i<getNumPixels(); i++)
    if ( pixels[i] > maxval ) {
      maxidx = i;
      maxval = pixels[i];
    }
  return maxidx;
}

template <class T>
T SketchData<T>::min() const {
  T result = pixels[0];
  for (unsigned int i=1; i<getNumPixels(); i++)
    result = std::min(result,pixels[i]);
  return result;
}

template <class T>
int SketchData<T>::findMin() const {
  T minval = pixels[0];
  int minidx = -1;
  for (unsigned int i = 1; i<getNumPixels(); i++)
    if ( pixels[i] < minval ) {
      minidx = i;
      minval = pixels[i];
    }
  return minidx;
}

template <class T>
T SketchData<T>::minPlus() const {
  T result = 0;
  unsigned int i = 0;
  for (; i<getNumPixels(); i++)
    if ( pixels[i] != 0 ) {
      result = pixels[i];
      break;
    }
  for (; i<getNumPixels(); i++)
    if ( pixels[i] != 0 )
      result = std::min(result,pixels[i]);
  return result;
}

template <class T>
int SketchData<T>::findMinPlus() const {
  T minval=std::numeric_limits<T>::max();
  int minidx = -1;
  for (unsigned int i=getNumPixels()-1; i!=-1U; --i)
    if ( pixels[i] != 0 && pixels[i] <= minval ) {
      minidx = i;
      minval = pixels[i];
    }
  return minidx;
}

template <class T>
T SketchData<T>::mode() const {
	std::map<T,size_t> hist;
	for(unsigned int i=0; i<getNumPixels(); i++)
		hist[pixels[i]]++;
	T maxval=T();
	size_t maxcnt=0;
	for(typename std::map<T,size_t>::const_iterator it=hist.begin(); it!=hist.end(); ++it) {
		if(maxcnt<=it->second) {
			maxval=it->first;
			maxcnt=it->second;
		}
	}
	return maxval;
}

template <class T>
T SketchData<T>::modePlus() const {
	std::map<T,size_t> hist;
	for(unsigned int i=0; i<getNumPixels(); i++)
		if(pixels[i]!=0)
			hist[pixels[i]]++;
	T maxval=T();
	size_t maxcnt=0;
	for(typename std::map<T,size_t>::const_iterator it=hist.begin(); it!=hist.end(); ++it) {
		if(maxcnt<=it->second) {
			maxval=it->first;
			maxcnt=it->second;
		}
	}
	return maxval;
}


// ================================================================

#define SKETCHDATA_ENCODE(a) \
  if(!LoadSave::encodeInc(a,buf,avail,"SketchData encode ran out of space at %s:%u\n",__FILE__,__LINE__)) return 0;

template <class T>
unsigned int SketchData<T>::saveBuffer(char buf[], unsigned int avail) const
{
  char* packet = buf; // beginning of packet
  static int frameNum = 0; // should this become a static member variable?
  unsigned int used=0;

  SKETCHDATA_ENCODE("TekkotsuImage");
  SKETCHDATA_ENCODE(Config::vision_config::RawCamConfig::ENCODE_SINGLE_CHANNEL);
  SKETCHDATA_ENCODE(Config::vision_config::RawCamConfig::COMPRESS_NONE);
  SKETCHDATA_ENCODE(getWidth());
  SKETCHDATA_ENCODE(getHeight());
  SKETCHDATA_ENCODE(get_time()); // is this what should be used for time stamp?
  SKETCHDATA_ENCODE(frameNum++);

  // encode filterbank info
  SKETCHDATA_ENCODE("FbkImage");
  SKETCHDATA_ENCODE(getWidth());
  SKETCHDATA_ENCODE(getHeight());
  SKETCHDATA_ENCODE(CAM_LAYER);
  SKETCHDATA_ENCODE(CAM_CHANNEL);

  // encode actual image data
  SKETCHDATA_ENCODE("SketchImage");
  SKETCHDATA_ENCODE((unsigned char)getType());
  const unsigned int imglength  = savePixels(buf,avail);
  if(imglength==0)
    return 0; // savePixels should have already reported the error
  avail-=imglength;
  buf+=imglength;

  // encode color table(same as color table of segmentedcolorgenerator for now
  used = (dynamic_cast<SegmentedColorGenerator*>
	  (ProjectInterface::defSegmentedColorGenerator))->encodeColorsInc(buf,avail);
  return buf-packet;
}

template <class T>
unsigned int SketchData<T>::savePixels(char buf[], unsigned int avail) const
{
  const unsigned int imglength  = getWidth() * getHeight() * sizeof(T);
  ASSERTRETVAL(imglength<avail,"Insufficient buffer space for image",0);
  memcpy(buf,(const unsigned char*)&(pixels[0]),imglength);
  return imglength;
}

#ifdef __POWERPC__
//bool can be 4 bytes on PowerPC systems
template<>
inline unsigned int SketchData<bool>::savePixels(char buf[], unsigned int avail) const
{
  const unsigned int imglength  = getWidth() * getHeight() * sizeof(char);
  ASSERTRETVAL(imglength<avail,"Insufficient buffer space for image",0);
  if(sizeof(bool)==sizeof(char))
    memcpy(buf,(const unsigned char*)&(pixels[0]),imglength);
  else
    for(unsigned int i=0; i<imglength; ++i)
      buf[i]=pixels[i]; //do manual copy to ensure one byte per pixel
  return imglength;
}
#endif

} // namespace

/*! @file
 * @brief A resource which holds the image date for a Sketch, managed collectively by a SketchSpace
 * @author neilh (Creator)
 *
 * $Author: dst $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.22 $
 * $State: Exp $
 * $Date: 2007/08/25 01:49:16 $
 */

#endif
