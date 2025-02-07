#include "BehaviorBase.h"
#include "Events/EventRouter.h"

BehaviorBase::BehaviorBase(const std::string& name)
	: ReferenceCounter(), EventListener(), started(false),
	  instanceName(name), className(name)
{
	getRegistryInstance().insert(this);
}

BehaviorBase::BehaviorBase(const std::string& classname, const std::string& instancename)
	: ReferenceCounter(), EventListener(), started(false),
	  instanceName(instancename), className(classname)
{
	getRegistryInstance().insert(this);
}


BehaviorBase::BehaviorBase(const BehaviorBase& b)
	: ReferenceCounter(b), EventListener(b), started(b.started),
	  instanceName(b.instanceName), className(b.className)
{
	getRegistryInstance().insert(this);
}

BehaviorBase&
BehaviorBase::operator=(const BehaviorBase& b) {
	ReferenceCounter::operator=(b);
	EventListener::operator=(b);
	started=b.started;
	instanceName=b.instanceName;
	return *this;
}

BehaviorBase::~BehaviorBase() {
	SetAutoDelete(false);
	if(started)
		std::cerr << "Behavior " << getName() << " deleted while running: use 'RemoveReference', not 'delete'" << std::endl;
	if(erouter!=NULL)
		erouter->removeListener(this);
	getRegistryInstance().erase(this);
}

void
BehaviorBase::DoStart() {
	//std::cout << getName() << " started " << this << std::endl;
	if(!started) {
		started=true;
		AddReference();
	}
}

void
BehaviorBase::DoStop() {
	//std::cout << getName() << " stopped " << this << std::endl;
	if(started) {
		started=false;
		erouter->remove(this);
		RemoveReference();
	}
}

std::set<BehaviorBase*>& BehaviorBase::getRegistryInstance() {
	static std::set<BehaviorBase*> registry;
	return registry;
}

/*! @file
 * @brief Implements BehaviorBase from which all Behaviors should inherit
 * @author ejt (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.12 $
 * $State: Exp $
 * $Date: 2007/06/14 06:24:03 $
 */

