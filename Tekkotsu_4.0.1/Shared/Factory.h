//-*-c++-*-
#if 0
#define INCLUDED_Factory_h_

//! Base class for Factory templated subclasses
class FactoryBase {
public:
	virtual ~FactoryBase() {} //!< destructor -- does nothing
	virtual void* constructTemplate()=0; //!< return a new object from the Factory subclass
};

//! A lightweight class to override for constructing new objects (if you need to pass constructors parameters, etc.)
/*! Say you don't want to construct your behavior at boot-up (if it's big and might not even be
 *  used) but your behavior needs special setup during creation (might be invoked
 *  several difference ways for instance) then you'll want to subclass this to do the setup when
 *  your behavior is activated.\n
 *  The default is to simply call the default constructor */
template<class B>
class Factory : public FactoryBase {
public:
	virtual B* construct() { return new B(); } //!< Just returns a new B
	virtual void* constructTemplate() { return construct(); }
};

//! Uses template to specify a constant parameter to the constructor
template<class B, class A1, A1 a1>
class Factory1Arg : public Factory<B> {
public:
	virtual B* construct() { return new B(a1); } //!< Just returns a new B constructed with arguments @a a1
	virtual void* constructTemplate() { return construct(); }
};

//! Uses template to specify constant parameters to the constructor
template<class B, class A1, A1 a1, class A2, A2 a2>
class Factory2Arg : public Factory<B> {
public:
	virtual B* construct() { return new B(a1,a2); } //!< Just returns a new B constructed with arguments @a a1, @a a2
	virtual void* constructTemplate() { return construct(); }
};

//! Uses template to specify constant parameters to the constructor
template<class B, class A1, A1 a1, class A2, A2 a2, class A3, A3 a3>
class Factory3Arg : public Factory<B> {
public:
	virtual B* construct() { return new B(a1,a2,a3); } //!< Just returns a new B constructed with arguments @a a1 - @a a3
	virtual void* constructTemplate() { return construct(); }
};

//! Uses template to specify constant parameters to the constructor
template<class B, class A1, A1 a1, class A2, A2 a2, class A3, A3 a3, class A4, A4 a4>
class Factory4Arg : public Factory<B> {
public:
	virtual B* construct() { return new B(a1,a2,a3,a4); } //!< Just returns a new B constructed with arguments @a a1 - @a a4
	virtual void* constructTemplate() { return construct(); }
};

/*! @file
 * @brief Defines Factory, a lightweight class to override for constructing new objects
 * @author ejt (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.8 $
 * $State: Exp $
 * $Date: 2007/05/21 17:02:37 $
 */

#endif

