#ifndef __COLORS_H__
#define __COLORS_H__

/*! @file
* @brief Color definitions for #CMVision
* @author James R. Bruce, School of Computer Science, Carnegie Mellon University
*
* Licensed under the <a href="../gpl-2.0.txt">GNU GPL version 2</a>
*
* $Author: ejt $
* $Name: tekkotsu-4_0-branch $
* $Revision: 1.13 $
* $State: Exp $
* $Date: 2007/08/14 18:24:07 $
*/

typedef unsigned int color_index;

//==== Color Classes =================================================//
namespace CMVision{
  typedef unsigned char uchar;
}

#ifndef YUV_STRUCT
#define YUV_STRUCT
struct yuv{
  CMVision::uchar y,u,v;
};

/* Depricated
struct yuv_int{
  int y,u,v;
};
*/
#endif

#ifndef YUVI_STRUCT
#define YUVI_STRUCT
struct yuvi{
  int y,u,v;
};
#endif

#ifndef YUVF_STRUCT
#define YUVF_STRUCT
struct yuvf{
  float y,u,v;
};
#endif

#ifndef YUYV_STRUCT
#define YUYV_STRUCT
struct yuyv{
  CMVision::uchar y1,u,y2,v;
};
#endif

struct uyvy{
#ifndef UYVY_STRUCT
#define UYVY_STRUCT
  CMVision::uchar u,y1,v,y2;
};
#endif

#ifndef RGB_STRUCT
#define RGB_STRUCT
struct rgb{
  CMVision::uchar red,green,blue;

  rgb() : red(0), green(0), blue(0) {};

  rgb(int r, int g, int b)
    : red((CMVision::uchar)r), green((CMVision::uchar)g), blue((CMVision::uchar)b)
  { };
			 

  bool operator ==(const rgb &x) const {
    return (red  ==x.red   &&
            green==x.green &&
            blue ==x.blue);
  }

  bool operator !=(const rgb &x) const {
    return !operator==(x);
  }
};
#endif

#ifndef RGBA_STRUCT
#define RGBA_STRUCT
struct rgba{
  CMVision::uchar r,g,b,a;
};
#endif

#ifndef ARGB_STRUCT
#define ARGB_STRUCT
struct argb{
  CMVision::uchar a,r,g,b;
};
#endif

#ifndef RGBF_STRUCT
#define RGBF_STRUCT
struct rgbf{
  float red,green,blue;

  rgbf()
		: red(0),green(0),blue(0) {}
  rgbf(float nred,float ngreen,float nblue)
		: red(nred),green(ngreen),blue(nblue) {}

};
#endif

#ifndef RGBAF_STRUCT
#define RGBAF_STRUCT
struct rgbaf{
  float r,g,b,a;
};
#endif

#ifndef ARGBF_STRUCT
#define ARGBF_STRUCT
struct argbf{
  float a,r,g,b;
};
#endif

//==== converting rgb to string
#include <iostream>
#include <string>
std::ostream& operator<<(std::ostream &os, const rgb &rgbval);
std::string toString(const rgb &rgbval);

#endif
