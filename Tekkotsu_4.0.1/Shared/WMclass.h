//-*-c++-*-
// David S. Touretzky
#ifndef INCLUDED_WMclass_h_
#define INCLUDED_WMclass_h_

#include <string>
#include <vector>
#include <iostream>
#include "Events/EventRouter.h"

/*! @file
 *	@brief  Watchable memory.  Variables are monitored and changes are announced.
 *  @author dst (Creator)
 *  @author alokl (Ported)
 *  
 *  $Author: ejt $
 *  $Name: tekkotsu-4_0-branch $
 *  $Revision: 1.7 $
 *  $State: Exp $
 *  $Date: 2006/09/11 23:05:15 $
 *
 * \par These macros allow you to quickly create a WM variable, but only if
 * it's staticly defined - you can't use these with a dynamic number
 * of variables.  You'll have to manually create a WMitem<T> or
 * another subclass of WMitem_base if you want define these on the
 * fly... it's not hard to do, just look at the macro definitions.
 *
 * \par WMvar(type,name) declares the variable in the registry
 * GlobalWM.
 *
 * \par WMvari(type,name,initval) declares the variable in GlobalWM
 * and initializes it if it doesn't already exist.
 *
 * \par WMvar_(type,name,registry) declares the variable in the
 * specified registry.
 *
 * \par WMvari_(type,name,initval,registry) declares the variable in
 * the specified registry and initializes it if it doesn't already
 * exist.
 *
 * \par WMreg(name) declares a new WMregistry as a variable in
 * GlobalWM.
 *
 * \par WMreg(name,registry) declares a new WMregistry as a variable
 * in the specified registry,
 *
 * Examples:  
 * - WMvar(int, foo);  
 * - WMvari(double, head_angle, 37.5);  
 * - WMreg(myreg);<br>
 * - WMvar_(bool, winflag, myreg);
 */

#define WM_max_stringlen 60		//!< Maximum displayed length of a char* or std::string variable.

class WMentry;
class WMitem_base;

//================================================================

//! A WMregistry is a namespace; it holds a collection of WMEntry instances describing the variables that live in this registry.
class WMregistry {
public:
  std::string	name; //!< Name of the registry.
  std::vector<WMentry*> entries; //!< Entries that make up the registry.
  bool watched; //!< True if changes to this registry are being monitored.
  WMregistry* parent; //!< pointer to parent registry (allows recursive structure)

public:
	//! Constructor
  WMregistry(const std::string &n = "anonymous", WMregistry* p=NULL) : name(n), entries(), watched(0), parent(p) {};
	WMregistry(const WMregistry& in) : name(in.name), entries(in.entries), watched(in.watched), parent(in.parent) {} //!< shallow copy

	//! shallow assignment
	WMregistry& operator=(const WMregistry& in) {
		name=in.name;
		entries=in.entries;
		watched=in.watched;
		parent=in.parent;
		return *this;
	}

  //! Turn on monitoring of changes to variables.
  void watch(void) { watched = 1; };

  //! Turn off monitoring of changes to variables.
  void unwatch(void) { watched = 0; };

  //! Dump the registry to the console, for debugging.
  void dump(void) const;

};

//================================================================

//! A WMentry is an entry in a WMregistry.
/*! The item it describes is a unique variable, which may be accessed
 *  via many WMitem instances, all of which point back to the same
 *  WMentry. */
class WMentry {
public:
  std::string const item_name; //!< Name of the variable described by this entry.
  std::string const type_name; //!< Type of the variable, e.g., "int".
  WMitem_base* const item; //!< Pointer to a WMitem instance for the variable.
  WMregistry* registry; //!< The registry in which this entry resides.
  bool watched;//!< True if changes to this variable should be reported.

public:
	//! constructor
  WMentry(const std::string &iname, const std::string &tname, WMitem_base* ival, WMregistry* reg = 0) : 
    item_name(iname), type_name(tname), item(ival), registry(reg), watched(0) {};
	WMentry(const WMentry& in) : item_name(in.item_name), type_name(in.type_name), item(in.item), registry(in.registry), watched(in.watched) {} //!< shallow copy

private:
	WMentry operator=(const WMentry&); //!< don't assign
};


//================================================================

//! WMitem_base is the base class from which all WMitem<T> subclasses are derived.
class WMitem_base {
public:
  virtual ~WMitem_base() {} //!< destructor, does nothing
  void* const         value;    //!< Pointer to the actual variable referenced by this WMitem.
  WMentry*            entry;    //!< Pointer to the WMentry describing this variable.
  virtual std::string toString(void) const = 0; //!< override this to return appropriate string representation

	//!Constructor
  WMitem_base(void* const val) : value(val), entry(NULL) {};
	WMitem_base(const WMitem_base &in ) : value(in.value), entry(in.entry) {} //!< *can* copy

  //! Turn on monitoring of changes to the variable described by this entry.
  void watch(void) const {
		entry->watched = 1; 
		erouter->postEvent(EventBase::wmVarEGID,reinterpret_cast<size_t>(entry),EventBase::activateETID);
	};

  //! Turn off monitoring of changes to the variable described by this entry.
  void unwatch(void) const {
		entry->watched = 0;
		erouter->postEvent(EventBase::wmVarEGID,reinterpret_cast<size_t>(entry),EventBase::deactivateETID);
	};

private:
	WMitem_base& operator=(const WMitem_base & ); //!< don't assign
};



//! WMitem<T> is a reference to the variable of type T described by the associated WMentry
/*! If the variable is declared in multiple functions, each
 *	declaration will have an associated WMitem<T> instance, but all
 *	the instances will reference the same variable. */
template<class T>
class WMitem : public WMitem_base {
public:
  WMitem(T* const initval) : WMitem_base(initval)  {} //!< constructor
	WMitem(const WMitem& in) : WMitem_base(in) {} //!< shallow copy

  virtual WMitem<T>& operator= (const T &new_value); //!< Assignment operator.
  WMitem<T>& operator++ ();                  //!< Generic operator prefix (++x)
  WMitem<T>  operator++ (int);               //!< Generic operator postfix (x++)
  WMitem<T>& operator-- ();                  //!< Generic operator prefix (--x)
  WMitem<T>  operator-- (int);               //!< Generic operator postfix (x--)
  WMitem<T>& operator+= (const T &val);      //!< Generic operator+=
  WMitem<T>& operator-= (const T &val);      //!< Generic operator-=
  WMitem<T>& operator*= (const T &val);      //!< Generic operator*=
  WMitem<T>& operator/= (const T &val);      //!< Generic operator/=
	
	//! Annonces changes, throws an event from wmVarEGID
  void announce (const T &val);
	
	//! Explicit retrieval of variable value, used by toString()
  virtual T& get_value(void) const { return *static_cast<T*>(value); };
	
	//! Coerce a WMitem<T> to type T by retrieving the value.
  virtual operator T&() const { return  *static_cast<T*>(value); };
	
	//! std::String representation of the value.
  virtual std::string toString(void) const;    
};


//================================================================

//!Allows you to create a static WMvar
#define WMvar(T,name) \
  static WMitem<T> name(lookup_WMentry<T>(#name,#T,GlobalWM));

//!Allows you to create a static WMvar with a default initialization value
#define WMvari(T,name,initval) \
  static WMitem<T> name(lookup_WMentry<T>(#name,#T,initval,GlobalWM));

//!Allows you to create a static WMvar in a registry with a default initialization value
#define WMvar_(T,name,registry) \
  static WMitem<T> name(lookup_WMentry<T>(#name,#T,registry));

//!Allows you to create a static WMvar in a registry with a default initialization value
#define WMvari_(T,name,initval,registry) \
  static WMitem<T> name(lookup_WMentry<T>(#name,#T,initval,registry));

//!Allows you to create a new static registry
#define WMreg(name) \
  static WMitem<WMregistry> name(lookup_reg(#name,GlobalWM));

//!Allows you to create a new static registry with a parent registry
#define WMreg_(name,parent) \
  static WMitem<WMregistry> name(lookup_reg(#name,parent));

//================================================================



//! Three versions of lookup_WMentry with different argument types, and one version of create_WMentry, which they share.  
//!@name Global Lookup functions

//! The lookup_reg function is defined in WMclass.cc since it's not templated.
template<typename T>
WMitem<T> lookup_WMentry(const std::string &iname,
												 const std::string &tname,
												 const std::string &regname);

template<typename T>
WMitem<T> lookup_WMentry(const std::string &iname,
												 const std::string &tname, 
												 const WMregistry &reg);

template<typename T>
WMitem<T> lookup_WMentry(const std::string &iname,
												 const std::string &tname,
												 const T &initval, WMregistry &reg);

template<typename T>
WMitem<T> create_WMentry(const std::string &iname, 
												 const std::string &tname, 
												 const T* const value, WMregistry &reg);

//! the global registry for all others to put their stuff in
extern WMregistry GlobalWM;

WMitem<WMregistry> lookup_reg(const std::string &name, WMregistry &registry);

void dump(const WMitem<WMregistry> &wmreg);

//@}

/*! Generic toString() method that just displays the variable's type and
 *  address.  Will be overridden for common types like int or double. */
template<class T>
std::string WMitem<T>::toString(void) const {
  char print_buffer[30];
  sprintf(print_buffer,"%p",value);
  return "<" + entry->type_name + " at 0x" + print_buffer + ">";
}

/*! The assignment operator for WMitem<T> changes the value of the
 *  variable referenced by the WMitem, and reports the change if the
 *  variable is "watched". */
template<class T>
WMitem<T>& WMitem<T>::operator= (const T &new_value) {
	get_value() = new_value;
	//	*static_cast<T*>(value)=new_value;
	announce (new_value);
  return *this;
}

template<class T>
void WMitem<T>::announce (const T&) {
  if (entry->watched) {
		erouter->postEvent(EventBase::wmVarEGID,reinterpret_cast<size_t>(entry),EventBase::statusETID);
	}
	/*    if (wmMonitorBehavior!=NULL) {
				std::string s(entry->item_name);
				WMregistry *temp=entry->registry;
				while (temp!=&GlobalWM && temp!=NULL) {
        s=temp->name + "." + s;
        temp=temp->parent;
				}
				wmMonitorBehavior->report(entry->type_name, s, toString());
				}
	*/
	/*
		if (entry->registry->watched)
    if (wmMonitorBehavior!=NULL)
		wmMonitorBehavior->report(entry->registry->name, "registry_change",
		"change to var");
	*/
}

template<class T>
WMitem<T>& WMitem<T>::operator++ (void) {
  announce(++get_value());
  return *this;
}

template<class T>
WMitem<T> WMitem<T>::operator++ (int) {
  WMitem<T> temp(*this);
  announce(++get_value());
  return temp;
}

template<class T>
WMitem<T>& WMitem<T>::operator-- (void) {
  announce(--get_value());
  return *this;
}

template<class T>
WMitem<T> WMitem<T>::operator-- (int) {
  WMitem<T> temp(*this);
  announce(--get_value());
  return temp;
}

template<class T>
WMitem<T>& WMitem<T>::operator+= (const T &val) {
  return operator=(get_value()+val);
}

template<class T>
WMitem<T>& WMitem<T>::operator-= (const T &val) {
  return operator=(get_value()-val);
}

template<class T>
WMitem<T>& WMitem<T>::operator*= (const T &val) {
  return operator=(get_value()*val);
}

template<class T>
WMitem<T>& WMitem<T>::operator/= (const T &val) {
  return operator=(get_value()/val);
}

//================================================================
//
// Three versions of lookup_WMentry with different argument types,
// and one version of create_WMentry, which they share.  
//
// The lookup_reg function is defined in WMclass.c since it's not
// templated.

template<typename T>
WMitem<T> lookup_WMentry(const std::string &iname,
												 const std::string &tname,
												 const std::string &regname) {
  WMregistry* reg = 0;
  for (std::vector<WMentry*>::const_iterator it = GlobalWM.entries.begin(); it != GlobalWM.entries.end(); it++) {
    if ( (*it)->item_name == regname ) {
      reg = static_cast<WMregistry*>((*it)->item->value);
      break;
    };
  };
  if ( reg == 0 ) {
    reg = new WMregistry(regname, &GlobalWM);
    create_WMentry(regname, "WMregistry", reg, GlobalWM);
  };
  return lookup_WMentry<T>(iname, tname, *reg);
}

template<typename T>
WMitem<T> lookup_WMentry(const std::string &iname,
												 const std::string &tname, 
												 WMregistry &reg) {
  for (std::vector<WMentry*>::const_iterator it = reg.entries.begin(); it != reg.entries.end(); it++) {
    if ( (*it)->item_name == iname )
      return *static_cast<WMitem<T> const*>((*it)->item);
  };
  return create_WMentry(iname, tname, new T, reg);
}

template<typename T>
WMitem<T> lookup_WMentry(const std::string &iname,
												 const std::string &tname,
												 T const &initval, WMregistry &reg) {
  for (std::vector<WMentry*>::const_iterator it = reg.entries.begin(); it != reg.entries.end(); it++) {
    if ( (*it)->item_name == iname )
      return *static_cast<WMitem<T> const*>((*it)->item);
  };
  return create_WMentry(iname, tname, new T(initval), reg);
}
 
template<typename T>
WMitem<T> create_WMentry(const std::string &iname, 
												 const std::string &tname, 
												 T* const value, WMregistry &reg) {
  WMentry* new_entry = new WMentry(iname, tname, new WMitem<T>(value), &reg);
  new_entry->item->entry = new_entry;
  reg.entries.push_back(new_entry);
  return *static_cast<WMitem<T> const*>(new_entry->item);
}

#endif

