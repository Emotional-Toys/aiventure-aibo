//-*-c++-*-
#ifndef INCLUDED_BehaviorBase_h_
#define INCLUDED_BehaviorBase_h_

#include "Events/EventListener.h"
#include "Shared/ReferenceCounter.h"
#include <string>
#include <set>

//! The basis from which all other Behaviors should inherit
/*! 
 *  For complex behaviors, it may be helpful to break aspects of the behaviors into independent 'states', and
 *  use a state machine formalism to control them.  See StateNode and Transition for more information.
 *
 *  Quick-start boilerplate is included in the distribution: <a href="http://cvs.tekkotsu.org/cgi/viewcvs.cgi/Tekkotsu/project/templates/behavior.h?rev=HEAD&content-type=text/vnd.viewcvs-markup"><i>project</i><tt>/templates/behavior.h</tt></a>:
 * 
 *  Tutorials:
 *  - <a href="../FirstBehavior.html">Tekkotsu's "First Behavior" Tutorial</a>
 *  - <a href="http://www.cs.cmu.edu/~dst/Tekkotsu/Tutorial/behaviors.shtml">David Touretzky's "Behaviors" Chapter</a>
 *  - <a href="http://www.cs.cmu.edu/afs/cs/academic/class/15494-s06/www/lectures/behaviors.pdf">CMU's Cognitive Robotics course slides</a>
 *  
 *  REMEMBER: If/when you override DoStart() / DoStop(), make sure that your own implementation calls BehaviorBase's implementation to allow
 *  proper reference counting... otherwise you'll get memory leaks and other odd issues. (see boilerplate link above for example usage)
 *
 *  Also, if you instantiate a behavior on the stack instead of the heap (this is very rarely done), remember to call 
 *  SetAutoDelete(false) (provided from the ReferenceCounter base class) -- don't want it to try to free memory
 *  on the stack when the behavior is stopped!  (The stack limits the allocation of the behavior
 *  to the current scope, which overrides the reference counting.)
 */
class BehaviorBase : public ReferenceCounter, public EventListener {
public:
	//! destructor - if is active when deleted, will display a warning (don't delete directly - use RemoveReference())
	virtual ~BehaviorBase();
	
	//! By default, merely adds to the reference counter (through AddReference()); Note you should still call this from your overriding methods
	virtual void DoStart();

	//! By default, subtracts from the reference counter (RemoveReference()), and thus may deletex if zero;  Don't forget to still call this when you override this; <b>Warning:</b> call this at the <i>end</i> of your DoStop(), not beginning (it might @c delete @c this )
	virtual void DoStop();
	
	//! By defining here, allows you to get away with not supplying a processEvent() function for the EventListener interface.  By default, does nothing.
	virtual void processEvent(const EventBase& /*event*/) {}

	//! Identifies the behavior in menus and such
	virtual std::string getName() const { return instanceName; }

	//! Allows dynamic renaming of behaviors
	virtual void setName(const std::string& name) { instanceName=name; }

	//! Gives a short description of what this particular instantiation does (in case a more specific description is needed on an individual basis)
	/*! By default simply returns getName(), because any calls from a
	 *  BehaviorBase function to getClassDescription() are going to call
	 *  BehaviorBase::getClassDescription(), not
	 *  ~YourSubClass~::getClassDescription(), because static functions
	 *  can't be virtual in C++ (doh!)
	 *
	 *  This means that getDescription called on a pointer to a
	 *  BehaviorBase of unknown subtype would always return an empty
	 *  string, which is pretty useless.  So instead we return the name
	 *  in this situation.  If you want getDescription to return
	 *  getClassDescription, you'll have to override it in your subclass
	 *  to do so. */
	virtual std::string getDescription() const {
		std::string d=getClassDescription();
		return (d.size()==0)?getName():d;
	}

	//! Returns the name of the class of this behavior (aka its type)
	/*! Note that this isn't static to avoid the problems we found with
	 *  getDescription/getClassDescription.  So instead we wind up
	 *  wasting some memory in each instance of the class to store the
	 *  className, but at least it will work the way you expect. */
	virtual std::string getClassName() const { return className; }
	
	//! Gives a short description of what this class of behaviors does... you should override this (but don't have to)
	/*! If you do override this, also consider overriding getDescription() to return it */
	static std::string getClassDescription() { return ""; }

	//! Returns true if the behavior is currently running
	virtual bool isActive() const { return started; }
	
	//! This read-only set allows us list all the currently instantiated behaviors
	/*! Not all of these behaviors are necessarily active, this is everything that has been allocated and not yet deallocated */
	static const std::set<BehaviorBase*>& getRegistry() { return getRegistryInstance(); }

	// Just some debugging stuff in stasis
	/*	virtual void AddReference() {
			std::cout << getName() << " AddReference()==" << GetReferences() << ' ' << this << std::endl;
			ReferenceCounter::AddReference();
			}
			
			virtual void RemoveReference() {
			std::cout << getName() << " RemoveReference()==" << GetReferences() << ' ' << this << std::endl;
			ReferenceCounter::RemoveReference();
			}
	*/
	
protected:
	//! static function to provide well-defined initialization order
	static std::set<BehaviorBase*>& getRegistryInstance();

	//! constructor, @a name is used as both instance name and class name
	explicit BehaviorBase(const std::string& name);
	//! constructor, allows different initial values for class name and instance name
	BehaviorBase(const std::string& classname, const std::string& instancename);
	//! copy constructor; assumes subclass handles copying approriately - i.e. if @a b is active, the copy will be as well, even though DoStart was never called..
	BehaviorBase(const BehaviorBase& b);
	//! assignment operator; assumes subclass handles assignment appropriately - i.e. if @a b is active, the copy will be as well, even though DoStart was never called..
	BehaviorBase& operator=(const BehaviorBase& b);

	bool started; //!< true when the behavior is active
	std::string instanceName; //!< holds the name of this instance of behavior
	const std::string className; //!< holds the type of the subclass of this behavior as a string
};

/*! @file
 * @brief Describes BehaviorBase from which all Behaviors should inherit
 * @author ejt (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.28 $
 * $State: Exp $
 * $Date: 2006/10/03 22:09:40 $
 */

#endif
