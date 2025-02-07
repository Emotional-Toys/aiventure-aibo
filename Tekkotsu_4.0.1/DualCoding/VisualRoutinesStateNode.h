//-*-c++-*-
#ifndef INCLUDED_VisualRoutinesStateNode_h_
#define INCLUDED_VisualRoutinesStateNode_h_

#include "Behaviors/StateNode.h"
#include "DualCoding/VRmixin.h"

namespace DualCoding {

/*! @brief Base class from which visual-routines based state nodes
 *  (which all share a common SketchSpace) inherit */
class VisualRoutinesStateNode : public StateNode, public VRmixin {
public:
	virtual void DoStart();
	virtual void DoStop();
	
protected:
	//! constructor, @a name is used as both instance name and class name
	explicit VisualRoutinesStateNode(const std::string &name)
		: StateNode(name), VRmixin()
	{}
	
	//! constructor, allows different initial values for class name and instance name
	VisualRoutinesStateNode(const std::string& classname, const std::string& instancename)
		: StateNode(classname,instancename), VRmixin()
	{}
	
	//! destructor, does nothing
	virtual ~VisualRoutinesStateNode(void) {}
	
private:
	// dummy functions to satisfy the compiler
	VisualRoutinesStateNode (const VisualRoutinesStateNode&);  //!< never call this
	VisualRoutinesStateNode& operator=(const VisualRoutinesStateNode&); //!< never call this
};

} // namespace

#endif
