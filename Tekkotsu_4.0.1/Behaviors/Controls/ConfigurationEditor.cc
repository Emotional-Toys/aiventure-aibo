#include "ConfigurationEditor.h"
#include "Behaviors/Controls/NullControl.h"
#include "Shared/plistCollections.h"
#include "Shared/Config.h"
#include "Shared/string_util.h"
#include "Wireless/Wireless.h"
#include <exception>

using namespace std; 

void ConfigurationEditor::init() {
	if(root==NULL)
		setRootCollection(config);
	pushSlot(NULL); // just to mark as having sub-nodes, will replace during refresh
}

void ConfigurationEditor::refresh() {
	plist::Collection * curcol = (path.size()==0) ? root : dynamic_cast<plist::Collection*>(root->resolveEntry(path));
	if(curcol==NULL) {
		clearSlots();
		pushSlot(new NullControl("[NULL Collection]"));
		ControlBase::refresh();
		return;
	}
	for(std::vector<ControlBase*>::const_iterator it=options.begin(); it!=options.end(); ++it) {
		if(StringInputControl * input = dynamic_cast<StringInputControl*>(*it)) {
			if(input->getLastInput().size()>0) {
				std::string key=input->getName();
				if(dynamic_cast<plist::Dictionary*>(curcol))
					key=key.substr(0,key.find('='));
				plist::PrimitiveBase * prim = dynamic_cast<plist::PrimitiveBase*>(curcol->resolveEntry(key));
				if(prim==NULL) {
					std::cerr << "ERROR: key " << key << " was set but does not correspond to a primitive value" << std::endl;
					continue;
				}
				try {
					prim->set(input->getLastInput());
				} catch(const std::exception& e) {
					std::cerr << "ERROR: unable to set key " << key << " to value " << input->getLastInput() << '\n'
					<< "        An exception occurred: " << e.what() << std::endl;
				} catch(...) {
					std::cerr << "ERROR: unable to set key '" << key << "' to value '" << input->getLastInput() << "', an exception occurred." << std::endl;
				}
			}
		}
	}
	
	for(std::vector<ControlBase*>::iterator it=options.begin(); it!=options.end(); ++it)
		if(*it!=NULL && *it!=&load && *it!=&save)
			delete *it;
	options.clear();
	pushSlot(&load);
	pushSlot(&save);
	pushSlot(NULL);
	if(curcol->size()==0) {
		pushSlot(new NullControl("[Empty Collection]"));
	} else if(plist::Dictionary * d = dynamic_cast<plist::Dictionary*>(curcol)) {
		for(plist::Dictionary::const_iterator it=d->begin(); it!=d->end(); ++it) {
			if(plist::NamedEnumerationBase * neb = dynamic_cast<plist::NamedEnumerationBase*>(it->second)) {
				pushSlot(new NamedEnumerationEditor(it->first+"="+neb->get(),d->getComment(it->first),*neb));
			} else if(plist::PrimitiveBase * po = dynamic_cast<plist::PrimitiveBase*>(it->second)) {
				pushSlot(new StringInputControl(it->first+"="+po->get(),d->getComment(it->first)));
			} else if(dynamic_cast<plist::Collection*>(it->second)) {
				ConfigurationEditor * sube = new ConfigurationEditor(it->first,d->getComment(it->first),root);
				sube->setPath(path.size()==0 ? it->first : path+plist::Collection::subCollectionSep()+it->first);
				pushSlot(sube);
			} else {
				std::cerr << "WARNING: unknown type for " << it->first << ": " << it->second->toString() << std::endl;
			}
		}
	} else if(plist::ArrayBase * a = dynamic_cast<plist::ArrayBase*>(curcol)) {
		for(unsigned int i=0; i!=a->size(); ++i) {
			if(plist::NamedEnumerationBase * neb = dynamic_cast<plist::NamedEnumerationBase*>(&a->getEntry(i))) {
				pushSlot(new NamedEnumerationEditor(neb->get(),a->getComment(i),*neb));
			} else if(plist::PrimitiveBase * po = dynamic_cast<plist::PrimitiveBase*>(&a->getEntry(i))) {
				pushSlot(new StringInputControl(po->get(),a->getComment(i)));
			} else if(dynamic_cast<plist::Collection*>(&a->getEntry(i))) {
				ConfigurationEditor * sube = new ConfigurationEditor("",a->getComment(i),root);
				stringstream ss;
				if(path.size()==0)
					ss << i;
				else
					ss << path << plist::Collection::subCollectionSep() << i;
				sube->setPath(ss.str());
				pushSlot(sube);
			} else {
				std::cerr << "WARNING: unknown type for entry " << i << ": " << a->getEntry(i).toString() << std::endl;
			}
		}
	}
	ControlBase::refresh();
}

void ConfigurationEditor::clearSlots() {
	// don't erase load or save controls -- we'll reuse them
	for(std::vector<ControlBase*>::iterator it=options.begin(); it!=options.end(); ++it)
		if(*it==&load || *it==&save)
			*it=NULL;
	ControlBase::clearSlots();
}

void ConfigurationEditor::setRootCollection(plist::Collection* rootCollection) {
	if(root==rootCollection)
		return;
	root=rootCollection;
	load.setRootCollection(root);
	save.setRootCollection(root);
	for(unsigned int i=0; i<options.size(); i++)
		if(ConfigurationEditor * confe = dynamic_cast<ConfigurationEditor*>(options[i]))
			confe->setRootCollection(root);
}

void ConfigurationEditor::setPath(const std::string& p) {
	path=p;
}

plist::Dictionary& ConfigurationEditor::getObjectTemplates() {
	static plist::Dictionary dict;
	if(dict.size()==0) {
		//first call, set up
		dict.addEntry("float",new plist::Primitive<float>);
		dict.addEntry("int",new plist::Primitive<int>);
		dict.addEntry("int (unsigned)",new plist::Primitive<unsigned int>);
		dict.addEntry("string",new plist::Primitive<std::string>);
		dict.addEntry("sub-array",new plist::Array);
		dict.addEntry("sub-dictionary",new plist::Dictionary);
	}
	return dict;
}

ControlBase* ConfigurationEditor::LoadSettings::selectedFile(const std::string& f) {
	if(rootcol==NULL) {
		std::cerr << "ERROR: Unable to load, no plist::Collection has been provided" << std::endl;
		return NULL;
	}
	rootcol->loadFile(f.c_str());
	return NULL;
}
ControlBase* ConfigurationEditor::SaveSettings::selectedFile(const std::string& f) {
	if(rootcol==NULL) {
		std::cerr << "ERROR: Unable to save, no plist::Collection has been provided" << std::endl;
		return NULL;
	}
	rootcol->saveFile(f.c_str());
	return NULL;
}

void ConfigurationEditor::AddCollectionEntry::refresh() {
	options.clear();
	for(plist::Dictionary::const_iterator it=ConfigurationEditor::getObjectTemplates().begin(); it!=ConfigurationEditor::getObjectTemplates().end(); ++it) {
		if(tgt->canContain(*it->second)) {
			pushSlot(new ConfigurationEditor::NewCollectionEntry(it->first,*tgt,static_cast<plist::ObjectBase&>(*it->second->clone())));
		}
	}
}

void ConfigurationEditor::NewCollectionEntry::refresh() {
	std::string key;
	if(options.size()>0 && options[0]!=NULL)
		key=dynamic_cast<const StringInputControl&>(*options[0]).getLastInput();
	options.clear();
	StringInputControl * keyEdit=NULL;
	if(dynamic_cast<plist::Dictionary*>(tgt)) {
		keyEdit = new StringInputControl("Key: "+key,"Enter key for the dictionary entry");
	} else if(dynamic_cast<plist::ArrayBase*>(tgt)) {
		if(key=="")
			key="end";
		keyEdit = new StringInputControl("Position: "+key,"Enter an index position or keyword 'end', optionally followed by '-n' to offset from the end");
	}
	keyEdit->takeInput(key);
	pushSlot(keyEdit);
	if(plist::PrimitiveBase * po = dynamic_cast<plist::PrimitiveBase*>(obj)) {
		pushSlot(new StringInputControl(po->get(),"Enter the intial value for the item"));
	} else if(plist::Collection * c = dynamic_cast<plist::Collection*>(obj)) {
		ConfigurationEditor * sube = new ConfigurationEditor("sub-entries...","Add entries to the new collection",c);
		pushSlot(sube);
	} else if(obj!=NULL) {
		std::cerr << "WARNING: unknown type for NewCollectionEntry editor" << ": " << obj->toString() << std::endl;
	}
	pushSlot(NULL);
	pushSlot(new NullControl("Commit Entry"));
}

ControlBase * ConfigurationEditor::NewCollectionEntry::doSelect() {
	if(hilights.size()!=1 || hilights.front()!=options.size()-1)
		return ControlBase::doSelect();
	
	if(plist::Dictionary* d = dynamic_cast<plist::Dictionary*>(tgt)) {
		std::string key = dynamic_cast<const StringInputControl&>(*options[0]).getLastInput();
		d->addEntry(key,obj);
	} else if(plist::ArrayBase* a = dynamic_cast<plist::ArrayBase*>(tgt)) {
		std::string key = dynamic_cast<const StringInputControl&>(*options[0]).getLastInput();
		int idx;
		if(key=="" || string_util::makeLower(key).substr(0,3)=="end") {
			idx=a->size();
			if(key.size()>3)
				idx+=atoi(key.substr(3).c_str());
		} else {
			idx=atoi(key.c_str()) % (options.size()+1);
			if(idx<0)
				idx+=options.size()+1;
		}
		reinterpret_cast<plist::ArrayBase::EntryConstraint<plist::ObjectBase>*>(a)->addEntry(idx,obj);
	}
	return NULL;
}

void ConfigurationEditor::NamedEnumerationEditor::refresh() {
	options.clear();
	std::map<int,std::string> names;
	tgt->getPreferredNames(names);
	for(std::map<int,std::string>::const_iterator it=names.begin(); it!=names.end(); ++it)
		pushSlot(new NullControl(it->second));
	ControlBase::refresh();
	if(gui_comm!=NULL && wireless->isConnected(gui_comm->sock)) {
		if(userPrompt.size()>0)
			gui_comm->printf("status\n%td\n%s\n",std::count(userPrompt.begin(),userPrompt.end(),'\n'),userPrompt.c_str());
	}
}

ControlBase * ConfigurationEditor::NamedEnumerationEditor::doSelect() {
	if(hilights.size()!=1)
		return this;
	clearLastInput();
	tgt->set(options[hilights.front()]->getName());
	return NULL;
}

/*! @file
 * @brief 
 * @author Ethan Tira-Thompson (ejt) (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Date: 2007/11/13 04:16:00 $
 */
