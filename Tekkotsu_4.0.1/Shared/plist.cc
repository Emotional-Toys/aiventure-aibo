#include "plist.h"

using namespace std;

namespace plist {

	ObjectBase* loadXML(xmlNode* node) {
		ObjectBase* obj=NULL;
		if(ObjectBase::xNodeHasName(node,"array")) {
			Array * a=new Array;
			a->setLoadSavePolicy(Collection::SYNC,Collection::SYNC); // just in case defaults change...
			obj=a;
		} else if(ObjectBase::xNodeHasName(node,"dict")) {
			Dictionary * d=new Dictionary;
			d->setLoadSavePolicy(Collection::SYNC,Collection::SYNC); // just in case defaults change...
			obj=d;
		} else if(ObjectBase::xNodeHasName(node,"real"))
			obj=new Primitive<double>;
		else if(ObjectBase::xNodeHasName(node,"integer"))
			obj=new Primitive<long>;
		else if(ObjectBase::xNodeHasName(node,"string"))
			obj=new Primitive<std::string>;
		else if(ObjectBase::xNodeHasName(node,"true"))
			obj=new Primitive<bool>;
		else if(ObjectBase::xNodeHasName(node,"false"))
			obj=new Primitive<bool>;
		else
			return NULL;
		obj->loadXML(node);
		return obj;
	}
	
	//! allows us to use the LoadSave suite for loading and parsing general XML functions, but forwards loadXML to plist::loadXML() and stores the result as a member
	class PolymorphicLoader : public XMLLoadSave {
	public:
		PolymorphicLoader() : XMLLoadSave(), obj(NULL) {} //!< constructor
		virtual ~PolymorphicLoader() {} //!< destructor
		PolymorphicLoader(const PolymorphicLoader& p) : XMLLoadSave(p), obj(NULL) {} //!< copy constructor -- doesn't copy #obj, sets the local instance to NULL
		PolymorphicLoader& operator=(const PolymorphicLoader& p) { XMLLoadSave::operator=(p); return *this; } //!< copy constructor -- doesn't copy #obj, keeps current value of #obj
		virtual void loadXML(xmlNode* node) { obj=plist::loadXML(node); } //!< forward call to the static plist implementation
		virtual void saveXML(xmlNode * /*node*/) const {*(int*)NULL=0xDEADDEAD;} //!< shouldn't ever be called -- call saveXML() on #obj directly
		ObjectBase * obj; //!< storage of results from loadXML
	};
	
	ObjectBase* loadFile(const std::string& file) {
		PolymorphicLoader loader;
		if(!loader.loadFile(file.c_str()))
			return NULL;
		return loader.obj;
	}
	
	ObjectBase* loadBuffer(const char* buf, unsigned int len) {
		PolymorphicLoader loader;
		if(!loader.loadBuffer(buf,len))
			return NULL;
		return loader.obj;
	}
	
} //namespace plist

/*! @file
 * @brief 
 * @author Ethan Tira-Thompson (ejt) (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.12 $
 * $State: Exp $
 * $Date: 2007/11/08 19:48:47 $
 */
