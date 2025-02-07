//-*-c++-*-
#ifndef INCLUDED_BehaviorSwitchControl_h_
#define INCLUDED_BehaviorSwitchControl_h_

#include "ControlBase.h"
#include "Behaviors/BehaviorBase.h"
#include "Shared/ReferenceCounter.h"
#include "Shared/Factories.h"
#include "Shared/debuget.h"
#include "Events/TextMsgEvent.h"

//! Holds some utility classes and functions for BehaviorSwitchControl which shouldn't be stored in a templated class
class BehaviorSwitchControlBase : public ControlBase {
public:
	//! A simple utility class to allow the BehaviorSwitchControl's to be able to deactivate the current behavior when a new one becomes active
	/*! Most behaviors are either major actions which you'll only want one of active
	 *  at a time, or else their background monitors of some sort, that can run in different
	 *  combinations.  Think radio buttons vs. checkboxes.  This will help you implement the
	 *  "radio button" style... just assign all the behaviors to the same group, they will
	 *  automatically use it to turn the previous behavior off when a new one becomes active.\n
	 *  Pass NULL instead of one of these to get checkbox-style. */
	class BehaviorGroup : public ReferenceCounter {
	public:
		BehaviorGroup() : curBehavior(NULL), members() { } //!< contructor
		~BehaviorGroup() { if(curBehavior!=NULL) curBehavior->DoStop(); } //!< destructor, will stop the current behavior if it was a one-shot
		BehaviorBase * curBehavior; //!< pointer to current behavior
		std::set<BehaviorSwitchControlBase*> members; //!< set of members of the group
	private:
		BehaviorGroup(const BehaviorGroup&); //!< shouldn't be called
		BehaviorGroup operator=(const BehaviorGroup&); //!< shouldn't be called
	};
	
	//! constructor
	BehaviorSwitchControlBase(const std::string& n, BehaviorBase* beh, BehaviorGroup* bg=NULL)
		: ControlBase(n), behgrp(bg), mybeh(beh) {
		if(mybeh!=NULL) {
			mybeh->AddReference();
			mybeh->setName(n);
			if(mybeh->isActive())
				mybeh->AddReference();
		}
		if(behgrp!=NULL) {
			behgrp->AddReference();
			behgrp->members.insert(this);
			if(mybeh!=NULL && mybeh->isActive()) {
				if(behgrp->curBehavior!=NULL) {
					behgrp->curBehavior->DoStop();
					notifyGroupMembers();
				}
				behgrp->curBehavior=mybeh;
			}
		}
	}
	//! constructor, behavior must not be NULL
	BehaviorSwitchControlBase(BehaviorBase* beh, BehaviorGroup* bg=NULL)
		: ControlBase(beh->getName()), behgrp(bg), mybeh(beh) {
		mybeh->AddReference();
		if(mybeh->isActive())
			mybeh->AddReference();
		if(behgrp!=NULL) {
			behgrp->AddReference();
			behgrp->members.insert(this);
			if(mybeh!=NULL && mybeh->isActive()) {
				if(behgrp->curBehavior!=NULL) {
					behgrp->curBehavior->DoStop();
					notifyGroupMembers();
				}
				behgrp->curBehavior=mybeh;
			}
		}
	}

	//! destructor
	virtual ~BehaviorSwitchControlBase() {
		//cout << "~BehaviorSwitchControlBase(): " << getName() << endl;
		if(mybeh!=NULL)
			stop();
		if(behgrp!=NULL) {
			behgrp->members.erase(this);
			behgrp->RemoveReference();
			behgrp=NULL;
		}
		if(mybeh!=NULL)
			mybeh->RemoveReference();
	}

	//! activates the behavior, handy for making start-up behaviors that you can turn off again with the Controller
	/*! If you start twice without stopping (ie it's already running), shouldn't do anything */
	virtual BehaviorSwitchControlBase* start() { if(!isRunning()) { stopother(); startmine(); } return this; }

	//! stops the behavior
	virtual BehaviorSwitchControlBase* stop() { if(isRunning()) stopother(); return this; }

	//! toggles the behavior
	virtual BehaviorSwitchControlBase* toggle() { if(isRunning()) stopother(); else { stopother(); startmine(); } return this; }

	virtual ControlBase * takeInput(const std::string& msg) {
		if(options.size()>0)
			return ControlBase::takeInput(msg);
		if(!isRunning())
			startmine();
		mybeh->processEvent(TextMsgEvent(msg,1));
		return NULL;
	}
	
	//! tells the current behavior (if there is one) to stop then loads its own
	/*! @return NULL unless there are submenus */
	virtual ControlBase * activate(MotionManager::MC_ID display, Socket * gui) {
		if(slotsSize()==0) {
			toggle();
			return NULL;
		} else
			return ControlBase::activate(display,gui);
	}

	//! adds a status to the name: - if in memory, # if running
	virtual std::string getName() const {
		if(mybeh==NULL)
			return ControlBase::getName();
		return (mybeh->isActive()?'#':'-')+mybeh->getName();
	}
	virtual std::string getDescription() const {
		if(mybeh==NULL)
			return ControlBase::getDescription();
		return "Class "+mybeh->getClassName()+": "+mybeh->getDescription();
	}
	
	//! Returns true if the associated behavior is running
	virtual bool isRunning() const {
		if(mybeh==NULL) //not created or has been destroyed, definitely not running
			return false;
		// so, beh has been created (but may have been stopped by another in the group)
		return mybeh->isActive(); //just check active flag (is valid object, we would have set it to NULL if we stopped it ourselves)
	}
	
protected:
	//! Stops the "other" guy's behavior - if ::behgrp is NULL, stops ourselves
	virtual void stopother() {
		if(behgrp==NULL) {
			if(mybeh!=NULL && mybeh->isActive()) {
				mybeh->DoStop();
				behaviorStopped();
			}
		} else if(behgrp->curBehavior!=NULL) {
			if(behgrp->curBehavior->isActive()) {
				behgrp->curBehavior->DoStop();
				notifyGroupMembers();
			}
			behgrp->curBehavior=NULL;
		}
	}
	
	//! Starts our behavior
	virtual void startmine() {
		if(behgrp!=NULL)
			behgrp->curBehavior=mybeh;
		mybeh->DoStart();
	}
	
	//! updates other members in the group that the current behavior stopped -- do not call if behgrp is NULL
	virtual void notifyGroupMembers() {
		for(std::set<BehaviorSwitchControlBase*>::iterator it=behgrp->members.begin(); it!=behgrp->members.end(); ++it)
			if((*it)->mybeh==behgrp->curBehavior)
				(*it)->behaviorStopped();
	}
	//! called by notifyGroupMembers if #mybeh was destructed when stopped
	virtual void behaviorStopped() {}

	BehaviorGroup * behgrp; //!< the behavior group this belongs to.  Uses this to track the "current" behavior
	BehaviorBase* mybeh; //!< used to store the behavior.  If retained and non-NULL, will be valid.  However, if not retained, only valid if equals behgrp->curBehavior

private:
	BehaviorSwitchControlBase(const BehaviorSwitchControlBase&); //!< shouldn't copy these
	BehaviorSwitchControlBase operator=(const BehaviorSwitchControlBase&); //!< shouldn't assign these
};




//! Allows proper switching between major behaviors, calling DoStart and DoStop
template < class B, class Al = typename Factory0Arg<B>::template Factory<B> >
class BehaviorSwitchControl : public BehaviorSwitchControlBase {
public:
	//! constructor, can use this to toggle a single behavior on and off
	BehaviorSwitchControl(const std::string& n, bool retain=false)
		: BehaviorSwitchControlBase(n,NULL,NULL), retained(retain), startref(NULL)
	{}
	//! constructor, if you want to use an already constructed behavior
	BehaviorSwitchControl(B* beh, BehaviorGroup* bg=NULL)
		: BehaviorSwitchControlBase(beh,bg), retained(true), startref(NULL)
	{}
	//! constructor, if you want to use an already constructed behavior, but unretain it if it's stopped (if not retaining, will start @a beh if it's not already started)
	BehaviorSwitchControl(const std::string& n, B* beh, BehaviorGroup* bg=NULL, bool retain=false)
		: BehaviorSwitchControlBase(n,beh,bg), retained(retain), startref(NULL)
	{
		if(!retained) {
			// have to make sure behavior is started to maintain invariants
			if(!mybeh->isActive()) {
				startmine();
			}
			mybeh->RemoveReference(); //cancels reference from BehaviorSwitchControlBase's constructor
		}
	}
	//! constructor, needs to know what group its in and whether to retain its behavior
	BehaviorSwitchControl(const std::string& n, BehaviorGroup* bg, bool retain=false)
		: BehaviorSwitchControlBase(n,NULL,bg), retained(retain), startref(NULL)
	{}
	
	//! destructor
	virtual ~BehaviorSwitchControl() {
		stop();
		if(behgrp!=NULL) {
			behgrp->members.erase(this);
			behgrp->RemoveReference();
			behgrp=NULL;
		}
		if(mybeh!=NULL && retained)
			mybeh->RemoveReference();
		mybeh=NULL;
	}

	virtual std::string getName() const {
		if(!isValid())
			return ControlBase::getName();
		else
			return BehaviorSwitchControlBase::getName();
	}
	virtual std::string getDescription() const {
		if(!isValid() || mybeh->getDescription().size()==0)
			return B::getClassDescription();
		else
			return BehaviorSwitchControlBase::getDescription();
	}
	
protected:

	virtual void startmine() {
		if(!retained) {
			Al allocator;
			mybeh=allocator();
			mybeh->setName(getName());
			if(behgrp!=NULL)
				behgrp->curBehavior=mybeh;
		} else {
			if(mybeh==NULL) {
				Al allocator;
				mybeh=allocator();
				mybeh->setName(getName());
				mybeh->AddReference();
			}
			if(behgrp!=NULL)
				behgrp->curBehavior=mybeh;
		}
		startref=mybeh;
		startref->AddReference();
		mybeh->DoStart();
	}

	//! adds a check to see if behavior has stopped itself -- if so, remove startref
	virtual bool isRunning() const {
		if(BehaviorSwitchControlBase::isRunning())
			return true;
		else if(startref!=NULL)
			const_cast<BehaviorSwitchControl<B,Al>*>(this)->stopother();
		return false;
	}		
	
	//! Returns true if mybeh is pointing to a valid object
	virtual bool isValid() const {
		if(isRunning())
			return true;
		return retained;
	}

	virtual void behaviorStopped() {
		if(!retained)
			mybeh=NULL;
		if(startref!=NULL) {
			startref->RemoveReference();
			startref=NULL;
		}
	}
	
	bool retained; //!< true if the behavior should be generated once and retained after DoStop.  Otherwise, a new one is generated each time it is started
	BehaviorBase * startref; //!< true if a reference was added (and still current) from calling DoStart
	
private:
	BehaviorSwitchControl(const BehaviorSwitchControl&); //!< shouldn't call this
	BehaviorSwitchControl operator=(const BehaviorSwitchControl&); //!<shouldn't call this
};

/*! @file
 * @brief Defines BehaviorSwitchControl and the BehaviorSwitch namespace - a control for turning behaviors on and off
 * @author ejt (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.25 $
 * $State: Exp $
 * $Date: 2007/05/22 04:24:26 $
 */

#endif
