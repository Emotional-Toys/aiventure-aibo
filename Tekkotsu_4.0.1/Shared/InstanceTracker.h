//-*-c++-*-
#ifndef INCLUDED_InstanceTracker_h_
#define INCLUDED_InstanceTracker_h_

#include "plistCollections.h"
#include "FamilyFactory.h"
#include <string>

//! Attempts to provide references to all currently instantiated objects of a class (and its subclasses)
/*! The classes being tracked need to register themselves via registerInstance() -- this isn't magic.
 *  The easiest way to do this is simply to have the base FamilyT's constructor do the registration,
 *  and then all of the subclasses will get it automatically.  This only works however, on the assumption
 *  that nothing is going to try to immediately access the newly registered entry while subclass
 *  construction is still in progress. */
template<class FamilyT, typename ClassNameT=std::string, class FactoryBaseT=Factory1Arg<FamilyT,std::string>, template<class U> class FactoryT=FactoryBaseT::template Factory>
class InstanceTracker : public plist::Dictionary, protected FamilyFactory<FamilyT,ClassNameT,FactoryBaseT,FactoryT> {
public:
	//! shorthand for iterators to be returned
	typedef plist::Dictionary::iterator iterator;
	//! shorthand for iterators to be returned
	typedef plist::Dictionary::const_iterator const_iterator;
	
	//! allows indirect access to the product family type
	typedef FamilyT FamilyType;
	//! allows indirect access to the product class name type
	typedef ClassNameT ClassNameType;
	//! allows indirect access to the factory base class
	typedef FactoryBaseT FactoryBaseType;
	//! allows indirect access to create factory classes
	template<class T> struct FactoryType : FactoryT<T> {};
	
	//! constructor
	InstanceTracker() : plist::Dictionary(), FamilyFactory<FamilyT,ClassNameT,FactoryBaseT,FactoryT>() {}
	
	using FamilyFactory<FamilyT,ClassNameT,FactoryBaseT,FactoryT>::getTypeNames;
	using FamilyFactory<FamilyT,ClassNameT,FactoryBaseT,FactoryT>::getNumTypes;
	using FamilyFactory<FamilyT,ClassNameT,FactoryBaseT,FactoryT>::registerType;
	using FamilyFactory<FamilyT,ClassNameT,FactoryBaseT,FactoryT>::registerFactory;
	
	//! Register an existing instance via reference, does @em not assume responsibility for memory deallocation, returns false if @a name is already in use.
	template<class T> bool registerInstance(const std::string& name, T& inst);
	//! Register an existing instance via pointer, @em does assume responsibility for memory deallocation, returns false if @a name is already in use.
	template<class T> bool registerInstance(const std::string& name, T* inst);
	//! Register an existing instance via reference, does @em not assume responsibility for memory deallocation, returns false if @a name is already in use.
	/*! If @a type is not an empty string, and @a inst is a plist::Dictionary, a '.type' entry will be added to store
	  *  the type.  This allows polymorphic loading.   Non-dictionaries will be wrapped in a new plist::Dictionary with a '.type'. */
	template<class T> bool registerInstance(const ClassNameT& type, const std::string& name, T& inst);
	//! Register an existing instance via pointer, @em does assume responsibility for memory deallocation, returns false if @a name is already in use.
	/*! If @a type is not an empty string, and @a inst is a plist::Dictionary, a '.type' entry will be added to store
	 *  the type.  This allows polymorphic loading.   Non-dictionaries will be wrapped in a new plist::Dictionary with a '.type'. */
	template<class T> bool registerInstance(const ClassNameT& type, const std::string& name, T* inst);
	
	//! create and register an instance in one call
	FamilyT* create(const ClassNameType& type, const std::string& name);
	//! looks up a previously registered instance, returning NULL if not found
	FamilyT* getInstance(const std::string& name );
	//! Removes an instance from the tracker.
	/*! If the tracker was managing the memory allocation (via create() or the registerInstance() functions
	  *  which take a pointer instead of a reference), then this will cause the instance to be deallocated. */
	bool destroy(const std::string& name);
	
	void loadXML(xmlNode* node);

protected:
	//! Provides a wrapper for non-plist::Dictionary entries.
	/*! This allows InstanceTracker to use plist::Dictionary as a superclass to manage the tracked instances,
	  *  as well as polymorphically load and save the entire set of instances.  Which is why we use a dictionary
	  *  here instead of a simple plist::ObjectBase so we can handle a '.type' entry to store the instance's class type. */
	class InstanceEntry : public plist::Dictionary {
	public:
		//! constructor, assigns @a inst to both #instance and #alloc (thus marking the instance for deallocation on InstanceEntry destruction)
		explicit InstanceEntry(FamilyT* inst) : plist::Dictionary(), alloc(inst), instance(*inst) {}
		//! constructor, assigns @a inst to #instance only, leaving #alloc NULL (thus leaving deallocation of instance to the caller)
		explicit InstanceEntry(FamilyT& inst) : plist::Dictionary(), alloc(NULL), instance(inst) {}
		//! destructor, deallocates #alloc (which might be NULL, and thus a no-op)
		~InstanceEntry() { delete alloc; }
		
		FamilyT* const alloc; //!< if the original instance was passed as a pointer, we will store that here for easy deallocation on destruction
		FamilyT& instance; //!< reference to the actual instance
		
	private:
		InstanceEntry(const InstanceEntry&); //!< copy constructor, don't call
		InstanceEntry& operator=(const InstanceEntry&); //!< assignment, don't call
	};
	
private:
	InstanceTracker(const InstanceTracker&); //!< copy constructor, don't call
	InstanceTracker& operator=(const InstanceTracker&); //!< assignment, don't call
};

template<class FamilyT, typename ClassNameT, class FactoryBaseT, template<class U> class FactoryT>
template<class T>
bool InstanceTracker<FamilyT,ClassNameT,FactoryBaseT,FactoryT>::registerInstance(const std::string& name, T& inst) {
	if(findEntry(name)!=end())
		return false; // already have instance by that name
	plist::Dictionary* dinst=dynamic_cast<plist::Dictionary*>(&inst);
	if(dinst!=NULL) {
		addEntry(name,*dinst); // note adding as reference so we don't try to delete the reference we were given
	} else {
		dinst=new InstanceEntry(inst); // create a wrapper for the non-Dictionary type
		addEntry(name,dinst); // adding as pointer since we want to delete our wrapper
	}
	return true;
}
template<class FamilyT, typename ClassNameT, class FactoryBaseT, template<class U> class FactoryT>
template<class T>
bool InstanceTracker<FamilyT,ClassNameT,FactoryBaseT,FactoryT>::registerInstance(const std::string& name, T* inst) {
	if(findEntry(name)!=end())
		return false; // already have instance by that name
	plist::Dictionary* dinst=dynamic_cast<plist::Dictionary*>(inst);
	if(dinst==NULL)
		dinst=new InstanceEntry(inst); // create a wrapper for the non-Dictionary type
	addEntry(name,dinst);
	return true;
}
template<class FamilyT, typename ClassNameT, class FactoryBaseT, template<class U> class FactoryT>
template<class T>
bool InstanceTracker<FamilyT,ClassNameT,FactoryBaseT,FactoryT>::registerInstance(const ClassNameT& type, const std::string& name, T& inst) {
	if(findEntry(name)!=end())
		return false; // already have instance by that name
	plist::Dictionary* dinst=dynamic_cast<plist::Dictionary*>(&inst);
	if(dinst!=NULL) {
		if(type.size()>0)
			dinst->addEntry(".type",new plist::Primitive<ClassNameT>(type),"Stores the typename of the class so it can be re-instantiated on load.\n** Do not edit ** ");
		addEntry(name,*dinst); // note adding as reference so we don't try to delete the reference we were given
	} else {
		dinst=new InstanceEntry(inst); // create a wrapper for the non-Dictionary type
		if(type.size()>0)
			dinst->addEntry(".type",new plist::Primitive<ClassNameT>(type),"Stores the typename of the class so it can be re-instantiated on load.\n** Do not edit ** ");
		addEntry(name,dinst); // adding as pointer since we want to delete our wrapper
	}
	return true;
}
template<class FamilyT, typename ClassNameT, class FactoryBaseT, template<class U> class FactoryT>
template<class T>
bool InstanceTracker<FamilyT,ClassNameT,FactoryBaseT,FactoryT>::registerInstance(const ClassNameT& type, const std::string& name, T* inst) {
	if(findEntry(name)!=end())
		return false; // already have instance by that name
	plist::Dictionary* dinst=dynamic_cast<plist::Dictionary*>(inst);
	if(dinst==NULL)
		dinst=new InstanceEntry(inst); // create a wrapper for the non-Dictionary type
	if(type.size()>0)
		dinst->addEntry(".type",new plist::Primitive<ClassNameT>(type),"Stores the typename of the class so it can be re-instantiated on load.\n** Do not edit ** ");
	addEntry(name,dinst);
	return true;
}

template<class FamilyT, typename ClassNameT, class FactoryBaseT, template<class U> class FactoryT>
FamilyT* InstanceTracker<FamilyT,ClassNameT,FactoryBaseT,FactoryT>::create(const ClassNameT& type, const std::string& name) {
	if(findEntry(name)!=end())
		return NULL; // already have instance by that name
	FamilyT* inst=FamilyFactory<FamilyT,ClassNameT,FactoryBaseT,FactoryT>::create(type,name);
	if(inst==NULL)
		return NULL; // apparently type isn't valid
	registerInstance(type,name,inst); // if it fails, means instance registered itself from constructor, that's fine
	return inst;
}

template<class FamilyT, typename ClassNameT, class FactoryBaseT, template<class U> class FactoryT>
FamilyT* InstanceTracker<FamilyT,ClassNameT,FactoryBaseT,FactoryT>::getInstance(const std::string& name) {
	const_iterator it=findEntry(name);
	if(it==end())
		return NULL; // doesn't exit
	if(InstanceEntry* inst=dynamic_cast<InstanceEntry*>(it->second))
		return &inst->instance; // was a wrapper because FamilyT isn't a Dictionary, extract actual instance
	return dynamic_cast<FamilyT*>(it->second); // otherwise this is our instance right here
}

template<class FamilyT, typename ClassNameT, class FactoryBaseT, template<class U> class FactoryT>
bool InstanceTracker<FamilyT,ClassNameT,FactoryBaseT,FactoryT>::destroy(const std::string& name) {
	const_iterator it=findEntry(name);
	if(it==end())
		return false;
	removeEntry(name);
	return true;
}		

template<class FamilyT, typename ClassNameT, class FactoryBaseT, template<class U> class FactoryT>
void InstanceTracker<FamilyT,ClassNameT,FactoryBaseT,FactoryT>::loadXML(xmlNode* node) {
	// first load the dictionary as a set of generic sub-dictionaries so we can pull out the type fields
	plist::Dictionary d;
	d.loadXML(node);
	for(plist::Dictionary::const_iterator it=d.begin(); it!=d.end(); ++it) {
		if(plist::Dictionary* dd = dynamic_cast<plist::Dictionary*>(it->second)) {
			plist::Dictionary::const_iterator obit = dd->findEntry(".type");
			if(obit==dd->end()) {
				std::cerr << "ERROR: could not instantiate driver named '" << it->first << "'; no '.type' field found!" << std::endl;
			} else {
				std::string typestr = obit->second->toString();
				plist::Dictionary::const_iterator drobit = findEntry(it->first);
				plist::ObjectBase * drob = (drobit==end()) ? NULL : drobit->second;
				if(plist::Dictionary* dr = dynamic_cast<plist::Dictionary*>(drob)) {
					plist::Dictionary::const_iterator tyobit = dr->findEntry(".type");
					if(tyobit==dr->end())
						std::cerr << "WARNING: Driver dictionary lacking .type field for '" << it->first << "'" << std::endl;
					else if(tyobit->second->toString()==typestr)
						continue; // name and type already match, don't need to recreate
					destroy(it->first); // otherwise clear the old instance and fall through to allocate a new instance
					dr=NULL;
				}
				if(create(typestr,it->first)==NULL)
					std::cerr << "ERROR: failed to instantiate driver named '" << it->first << "' with type '" << typestr << "'" << std::endl;
			}
		} else {
			std::cerr << "ERROR: could not instantiate driver named '" << it->first << "'; not a dictionary!" << std::endl;
		}
	}
	// now that we've created basic instances, do the "real" loading to give them settings
	plist::Dictionary::loadXML(node);
}

/*! @file
 * @brief 
 * @author Ethan Tira-Thompson (ejt) (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.6 $
 * $State: Exp $
 * $Date: 2007/11/15 02:44:06 $
 */

#endif
