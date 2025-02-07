#include "Shared/Measures.h"

void AngPi::normalize() {
	// Do cheap tests first; use fmod only if necessary.
	if ( value < 0 ) 
		value += Pi;
	else if ( value >= Pi)
		value -= Pi;
	else return;
	// If we're still out of range, give up and call fmod.
	if ( value < 0 || value >= Pi) {
		value = fmod(value,Pi);
		if ( value < 0 ) 
			value += Pi;
	};
}


AngPi angdist(AngPi const &arg1, AngPi const &arg2) {
	AngPi diff = arg1.value - arg2.value;
	if ( diff > Pi/2 )
		diff = Pi - diff;
	return diff;
}

void AngTwoPi::normalize() {
	// Do cheap tests first; use fmod only if necessary.
	if ( value < 0 ) 
		value += TwoPi;
	else if ( value >= TwoPi)
		value -= TwoPi;
	else return;
	// If we're still out of range, give up and call fmod.
	if ( value < 0 || value >= TwoPi) {
		value = fmod(value,TwoPi);
		if ( value < 0 ) 
			value += TwoPi;
	};
}		     

AngPi angdist(AngTwoPi const &arg1, AngTwoPi const &arg2) {
	AngTwoPi diff = arg1.value - arg2.value;
	if ( diff > Pi )
		diff = TwoPi - diff;
	return AngPi(diff);
}

void AngSignPi::normalize() {
	// Do cheap tests first; use fmod only if necessary.
	if ( value < -Pi ) 
		value += TwoPi;
	else if ( value > Pi)
		value -= TwoPi;
	else return;
	// If we're still out of range, give up and call fmod.
	if ( value < -Pi || value > Pi) {
		value = fmod(value,TwoPi);
		if (value < -Pi) 
			value += TwoPi;
	};
}		     

AngPi angdist(AngSignPi const &arg1, AngSignPi const &arg2) {
	AngSignPi diff = arg1.value - arg2.value;
	if ( diff > Pi )
		diff = TwoPi - diff;
	return AngPi(diff);
}
