//-*-c++-*-
#ifndef INCLUDED_VisualRoutinesBehavior_h_
#define INCLUDED_VisualRoutinesBehavior_h_

#include "Behaviors/BehaviorBase.h"
#include "DualCoding/VRmixin.h"

namespace DualCoding {

/*! @brief Base class from which visual-routines based behaviors
 *  (which all share a common SketchSpace) inherit */
class VisualRoutinesBehavior : public BehaviorBase, public VRmixin {
public:
  virtual void DoStart();
  virtual void DoStop();
	
protected:
  //! constructor, @a name is used as both instance name and class name
  explicit VisualRoutinesBehavior(const std::string &name)
    : BehaviorBase(name), VRmixin()
  {}
	
  //! constructor, allows different initial values for class name and instance name
  VisualRoutinesBehavior(const std::string& classname, const std::string& instancename)
    : BehaviorBase(classname,instancename), VRmixin()
  {}
	
  //! destructor, does nothing
  virtual ~VisualRoutinesBehavior(void) {}
	
private:
  // dummy functions to satisfy the compiler
  VisualRoutinesBehavior (const VisualRoutinesBehavior&);  //!< never call this
  VisualRoutinesBehavior& operator=(const VisualRoutinesBehavior&); //!< never call this
};

} // namespace

#endif
