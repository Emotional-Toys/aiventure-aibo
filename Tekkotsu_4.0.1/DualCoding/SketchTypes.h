//-*-c++-*-
#ifndef INCLUDED_SketchTypes_h
#define INCLUDED_SketchTypes_h

#include <string>

namespace DualCoding {

typedef unsigned char uchar;
typedef unsigned short int usint;

#if defined(TGT_ERS7) || defined(TGT_ERS2xx) || defined(TGT_ERS210) || defined(TGT_ERS220)
	typedef unsigned short int uint;
#else
	typedef unsigned int uint;
#endif


enum SketchType_t {
  sketchUnknown = 0,
  sketchBool,
  sketchUchar,
  sketchUint,
  sketchFloat
};

const std::string SketchTypeNames[] = {"unknown","bool","uchar","uint","float"};

enum ColorMapType_t {
  segMap = 0,	// use the color segmentation table (default)
  grayMap = 1,	// gray scale image
  jetMap = 2,	// Matlab-style blue to red color map
  jetMapScaled = 3  // blue to red map that  scales the spectrum to the image
};

} // namespace

#endif
