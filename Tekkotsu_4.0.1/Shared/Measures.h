#ifndef INCLUDED_Measures_h_
#define INCLUDED_Measures_h_

#include <cmath>

typedef float coordinate_t; //!< type used for positional coordinates
typedef float orientation_t; //!< type used for orientation values (0 to Pi)
typedef float direction_t; //!< type used for direction values (0 to 2*Pi)

const direction_t Pi=M_PI; //!< shorthand for ::M_PI from math.h
const direction_t TwoPi=2*M_PI; //!< shorthand for 2*M_PI 

typedef coordinate_t Slope; //!< type used for ratio of coordinate offsets
const Slope BIG_SLOPE=5000.0; //!< slopes larger than this are considered vertical, or in other words, infinite slopes are rounded to this

//! Circular arithmetic on angles between 0 and pi (180 degrees)
class AngPi {
	friend AngPi angdist(AngPi const &arg1, AngPi const &arg2);
public:
	AngPi(void) : value(0) {}; //!< constructor, #value defaults to 0
	AngPi(orientation_t const &v) : value(v) { normalize(); } //!< conversion operator allows implicit construction from primitive
	
	AngPi operator+(AngPi const &arg) const { return AngPi(value+arg.value); };
	AngPi operator-(AngPi const &arg) const { return AngPi(value-arg.value); };
	AngPi operator*(orientation_t const &arg) const { return AngPi(value*arg); };
	AngPi operator/(orientation_t const &arg) const { return AngPi(value/arg); };
	
	AngPi& operator=(AngPi const &arg) { value = arg.value; return(*this); };
	AngPi& operator=(orientation_t const &arg) { value = arg; normalize(); return(*this); };
	AngPi& operator+=(orientation_t const &arg) { value += arg; normalize(); return(*this); };
	AngPi& operator-=(orientation_t const &arg) { value -= arg; normalize(); return(*this); };
	AngPi& operator*=(orientation_t const &arg) { value *= arg; normalize(); return(*this); };
	AngPi& operator/=(orientation_t const &arg) { value /= arg; normalize(); return(*this); };
	
	operator orientation_t() const { return value; }; //!< conversion operator for going back to the primitive type
	
protected:
	void normalize(); //!< modifies #value to put it back in range
	orientation_t value; //!< holds the angle, should be kept normalized at all times
};

//! Angular distance: value is between 0 and pi/2
AngPi angdist(AngPi const &arg1, AngPi const &arg2);

//! Circular arithmetic on angles between 0 and two pi (360 degrees)
class AngTwoPi {
	friend AngPi angdist(AngTwoPi const &arg1, AngTwoPi const &arg2);
public:
	AngTwoPi(void) : value(0) {}; //!< constructor, #value defaults to 0
	AngTwoPi(direction_t const &v) : value(v) { normalize(); } //!< conversion operator allows implicit construction from primitive
	
	AngTwoPi operator+(AngTwoPi const &arg) const { return AngTwoPi(value+arg.value); };
	AngTwoPi operator-(AngTwoPi const &arg) const { return AngTwoPi(value-arg.value); };
	AngTwoPi operator*(direction_t const &arg) const { return AngTwoPi(value*arg); };
	AngTwoPi operator/(direction_t const &arg) const { return AngTwoPi(value/arg); };
	
	AngTwoPi& operator=(AngTwoPi const &arg) { value = arg.value; return(*this); };
	AngTwoPi& operator=(direction_t const &arg) { value = arg; normalize(); return(*this); };
	AngTwoPi& operator+=(direction_t const &arg) { value += arg; normalize(); return(*this); };
	AngTwoPi& operator-=(direction_t const &arg) { value -= arg; normalize(); return(*this); };
	AngTwoPi& operator*=(direction_t const &arg) { value *= arg; normalize(); return(*this); };
	AngTwoPi& operator/=(direction_t const &arg) { value /= arg; normalize(); return(*this); };
	
	operator direction_t() const { return value; }; //!< conversion operator for going back to the primitive type
	
protected:
	void normalize(); //!< modifies #value to put it back in range
	direction_t value; //!< holds the angle, should be kept normalized at all times
};

//! Angular distance: value is between 0 and pi
AngPi angdist(AngTwoPi const &arg1, AngTwoPi const &arg2);

//! Circular arithmetic on angles between -pi and pi (360 degrees)
class AngSignPi {
	friend AngPi angdist(AngSignPi const &arg1, AngSignPi const &arg2);
public:
	AngSignPi(void) : value(0) {}; //!< constructor, #value defaults to 0
	AngSignPi(direction_t const &v) : value(v) { normalize(); } //!< conversion operator allows implicit construction from primitive
	
	AngSignPi operator+(AngSignPi const &arg) const { return AngSignPi(value+arg.value); };
	AngSignPi operator-(AngSignPi const &arg) const { return AngSignPi(value-arg.value); };
	AngSignPi operator*(direction_t const &arg) const { return AngSignPi(value*arg); };
	AngSignPi operator/(direction_t const &arg) const { return AngSignPi(value/arg); };
	
	AngSignPi& operator=(AngSignPi const &arg) { value = arg.value; return(*this); };
	AngSignPi& operator=(direction_t const &arg) { value = arg; normalize(); return(*this); };
	AngSignPi& operator+=(direction_t const &arg) { value += arg; normalize(); return(*this); };
	AngSignPi& operator-=(direction_t const &arg) { value -= arg; normalize(); return(*this); };
	AngSignPi& operator*=(direction_t const &arg) { value *= arg; normalize(); return(*this); };
	AngSignPi& operator/=(direction_t const &arg) { value /= arg; normalize(); return(*this); };
	
	operator direction_t() const { return value; }; //!< conversion operator for going back to the primitive type

protected:
	void normalize(); //!< modifies #value to put it back in range
	direction_t value; //!< holds the angle, should be kept normalized at all times
};

//! Angular distance: value is between 0 and pi
AngPi angdist(AngSignPi const &arg1, AngSignPi const &arg2);

#endif
