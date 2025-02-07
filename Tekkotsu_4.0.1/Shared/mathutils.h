//-*-c++-*-
#ifndef __mathutils_h__
#define __mathutils_h__

#include <math.h>

//! a variety of handy mathematical functions, many of which are templated
namespace mathutils {
	//! euclidean distance of two points (squared), see distance()
	template <class num>
	inline num squareDistance(num x1, num ya, num x2, num yb) {
		return (x1-x2)*(x1-x2)+(ya-yb)*(ya-yb);
	}
	
	//! euclidean distance of two points, see squareDistance()
	template <class num>
	inline num distance(num x1, num ya, num x2, num yb) {
		return sqrt(squareDistance(x1, ya, x2, yb));
	}
	
	//! Clips @a n to a minimum of @a low or maximum of @a high.
	/*! If @a low and @a high are inverted, high is returned. */
	template <class num>
	inline num limitRange(num n, num low, num high) {
		if (n<low) n=low;
		if (n>high) n=high;
		return n;
	}
	
	//! returns n*n;
	template <class num>
	inline num squared(num n) {
		return n*n;
	}
	
	//! returns the maximum of n or -n
	template <class num>
	inline num abs_t(num n) {
		return (n>-n)?n:-n;
	}
	
	//! Returns the log base 2 of a number
	/*! This template implementation does a bit shifting method appropriate for
	 *  integers.  Specializations are provided for float and double to use the 'real' log() */
	template <class num>
	inline num log2t(num x) {
		num ans=0;
		for(unsigned int mag=sizeof(num)*4; mag>0; mag/=2) {
			num y=x>>mag;
			if(y>0) {
				x=y;
				ans+=mag;
			}
		}
		return ans;
	}
	//! returns the log base 2 for a 'float' value
	template <>
	inline float log2t(float x) {
		return log(x)/M_LN2;
	}
	//! returns the log base 2 for a 'double' value
	template <>
	inline double log2t(double x) {
		return log(x)/M_LN2;
	}
	
	//! converts from degrees to radians
	template <class num>
	inline num deg2rad(num x) {
		return x*M_PI/180;
	}
	
	//! converts from radians to degrees
	template <class num>
	inline num rad2deg(num x) {
		return x*180/M_PI;
	}
	
}

#endif
