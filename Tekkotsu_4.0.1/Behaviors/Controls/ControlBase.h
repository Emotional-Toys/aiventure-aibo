//-*-c++-*-
#ifndef INCLUDED_ControlBase_h
#define INCLUDED_ControlBase_h

#include "Events/EventBase.h"
#include "Motion/MotionManager.h"
#include "Shared/Config.h"
#include "Wireless/Socket.h"
#include <string>
#include <vector>

//! Base class for all items in the Controller hierarchy.
/*! These are similar to behaviors in that they can do processing and
 *  are told when to start and stop.
 *
 *  However, the important difference is that these have to follow a
 *  much tighter set of guidelines for a more refined purpose - user
 *  interface.  Controls do not directly receive events - the
 *  Controller will process events for them and call the appropriate
 *  functions at the appropriate times.  Controls are expected to fit
 *  into a heirarchical scheme, where each control (except the root)
 *  has a parent which created it, and may return its own children
 *  where appropriate.
 *
 *  Controls can be very powerful, and a class can be both a behavior
 *  and a control.  This allows integrated user interface to
 *  controlling a complex behavior.  Some controls may simply need
 *  EventListener access instead to perform a few tricks.  Mix and
 *  match as you see fit.  (multiple inheritance can be nice if it's
 *  planned for, as these have been)
 *
 *  This base class will do most of the work of maintaining submenus
 *  for you, and will call appropriate virtual functions which you are
 *  expected to override.  Controls generally live in
 *  <tt>Behaviors/Controls/</tt> as the Controller itself is a
 *  behavior, and these controls merely its tools.
 *
 *  The ControlBase pointers which are returned at various points are the
 *  responsibility of the creator.  Controller will not delete them upon
 *  deactivation.
 *
 *  GUI Theory: \n
 *  There are 3 levels to the user interface.
 *  -# Robot/Local: Uses the buttons for input, and LEDs and sounds for immediate feedback.  No external resources needed
 *  -# Text: Uses a console to display/request information.
 *  -# GUI: Uses a graphical interface on a desktop machine
 *
 *  Obviously, higher levels require more technological resources, which also
 *  means there's more to go wrong and debug.  However, another important
 *  distinction between the first level and the others is that the first level
 *  does not require the user to lose direct contact with the robot.  Requiring
 *  the user to move back and forth from robot to computer can be much more
 *  frustrating than decoding LED signals or press head buttons.  There are also
 *  safety issues when triggering behaviors remotely if the robot is out of
 *  immediate reach.  But of course, having a GUI and text output is extremely
 *  valuable in terms of ease of use and efficiency.
 *
 *  So, the lesson is to try to support all 3 levels so that your interfaces
 *  will be robust and efficient in a variety of environments.  You'll thank
 *  yourself when you're demoing on the road and you can't get wavelan up, or
 *  the guest machine you're supposed to use doesn't have Java, or whatever.
 *
 * @todo ControlBase's should use ReferenceCounter so memory management is not an issue
 * @see Controller, NullControl */
class ControlBase {
public:
	
	//! @name Constructors/Destructors

	ControlBase() : name("(null name)"), description(), hilights(), options(), doRewrite(false), display_id(MotionManager::invalid_MC_ID), gui_comm(NULL) {} //!< Contructor
	ControlBase(const std::string& n) : name(n), description((n.size()>16)?n:std::string()), hilights(), options(), doRewrite(false), display_id(MotionManager::invalid_MC_ID), gui_comm(NULL) {} //!< Contructor, initializes with a name
	ControlBase(const std::string& n, const std::string& d) : name(n), description(d), hilights(), options(), doRewrite(false), display_id(MotionManager::invalid_MC_ID), gui_comm(NULL) {} //!< Contructor, initializes with a name

	//! Destructor
	virtual ~ControlBase() {
		//		deactivate();
		//cout << "~ControlBase(): " << name << endl;
		clearSlots();
	}

	//@}

	//! You probably want to override some of these, call the ControlBase functions from your code if you want default sound effects (or look in Config::controller_config).
	//! @name Controller Functions

	//! Called when the control is activated (or the control system is reactivating)
	/*! Takes the id number of a LedMC which the control should use, maintained by Controller.
	 *  Controls share the display which is passed, and may use the socket @a gui to communicate with the GUI controller, if it is connected.
	 *  @return a ControlBase pointer.  Return: @li @a this if the control should stay active (if it's not a one-shot command) @li @c NULL to return to parent @li other address to spawn a child control*/
	virtual ControlBase * activate(MotionManager::MC_ID disp_id, Socket * gui);
	virtual void pause();      //!< called when a control is being overriden by a child, or the control system is deactivating (e-stop being turned off)
	virtual void refresh();    //!< called when the child has deactivated and this control should refresh its display, or some other event (such as the user pressing the refresh button) has happened to cause a refresh to be needed
	virtual void deactivate(); //!< called when this control is being popped from the control stack

	//! when the user has trigger an "open selection" - default is to return the hilighted control*/
	/*! The value which is returned is then activate()ed and pushed on the Controller's stack*/
	virtual ControlBase * doSelect();
	//! when the user wants to increment the control - default is to hilight the first non-null slot after the last hilight, and return @c this
	/*! The value which is returned is then activate()ed and pushed on the Controller's stack, so you probably want to return @c this */
	virtual ControlBase * doNextItem();
	//! when the user wants to decrement the control - default is to hilight the last non-null slot before the first hilight, and return @c this
	/*! The value which is returned is then activate()ed and pushed on the Controller's stack, so you probably want to return @c this */
	virtual ControlBase * doPrevItem();
	//! when the user wants to cancel - you should almost always return NULL now unless you need to override the cancel in order to confirm something (e.g. "Save changes?")
	/*! The value which is returned is then activate()ed and pushed on the Controller's stack, you probably want to return @c NULL */
	virtual ControlBase * doCancel();
	//! prompt the user for text input on the current input device (cin, tekkotsu console (sout), or GUI)
	/*! The value which is returned is then activate()ed and pushed on the Controller's stack, so you probably want to return @c this */
	virtual ControlBase * doReadStdIn(const std::string& prompt=std::string());
	//! called when the user has supplied a text string (may or may not have been prompted by doReadStdIn()!  May not even be active yet - the user can direct the same input to a set of hilighted menus)
	/*! The value which is returned is then activate()ed and pushed on the Controller's stack*/
	virtual ControlBase * takeInput(const std::string& msg); 
	//! may be called before takeInput to verify this Control can make sense of msg
	virtual bool validInput(const std::string& msg); 
	//@}
	
	//! @name Accessors

	virtual ControlBase& setName(const std::string& n) { name=n; return *this; } //!< sets the name of the control
	virtual std::string getName() const { return name; } //!< returns the name of the control

	virtual ControlBase& setDescription(const std::string d) { description=d; return *this; } //!< sets the description of the control
	virtual std::string getDescription() const { return description; } //!< returns a short description of what the control does

	virtual const std::vector<ControlBase*>& getSlots() const { return options; } //!< returns the vector of sub-controls
	virtual std::string getSlotName(unsigned int i) const; //!< returns the string that will appear in slot @a i
	virtual unsigned int slotsSize() const { return options.size(); } //!< returns the number of options available
	virtual void setSlot(unsigned int i,ControlBase* o); //!< sets @a i'th element of #options to @a o
	virtual void pushSlot(ControlBase* o); //!< sets next unused element of #options to @a o
	virtual void clearSlots(); //!< deletes each slot item and clears the slots

	virtual const std::vector<unsigned int>& getHilights() const { return hilights; } //!< returns a vector of the indicies of hilighted slots
	virtual void setHilights(const std::vector<unsigned int>& hi); //!< sets the hilighted slots
	virtual void hilightFirst(); //!< sets the hilight to the first non-null slot

	virtual MotionManager::MC_ID getDisplay() { return display_id; } //!< returns display being used
	virtual ControlBase& setDisplay(MotionManager::MC_ID d) { display_id=d; return *this; } //!< sets display to use

	//@}

protected:
		
	//! clears the display (if use_VT100 is on)
	virtual void clearMenu();
	
	//! called by takeInput if the input doesn't match any slots or matches multiple slots -- the ControlBase implementation displays an error and returns itself
	/*! @param msg the input originally sent to takeInput()
	 *  @param ambiguous true if the input matched more than one item, false if it didn't match any */
	virtual ControlBase* invalidInput(const std::string& msg, bool ambiguous);

	//! returns the average of the hilighted indicies - used to know to play the "next" sound, or the "prev" sound when the hilight changes
	float hilightsAvg() const;

	std::string name; //!< the name of this control
	std::string description; //!< the description of this control
	std::vector<unsigned int> hilights; //!< keep sorted - index(es) of current selection - can have multiple if using GUI
	std::vector<ControlBase*> options; //!< vector of controls to select from
	bool doRewrite; //!< toggles using VT100 codes to reposition the cursor at the beginning of the menu
	                /*!< we don't always want to do this, any time someone else might have written to
									 *   the display we set this to false so we don't overwrite it. */

	MotionManager::MC_ID display_id; //!< LedMC to use for displaying selection
	Socket * gui_comm; //!< socket to communicate with the GUI, if it is connected
	
private:
	ControlBase(const ControlBase&); //!< you can override, but don't call this...
	ControlBase& operator=(const ControlBase&); //!< you can override, but don't call this...
};

/*! @file
 * @brief Defines ControlBase from which all items in the control system should inherit
 * @author ejt (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.24 $
 * $State: Exp $
 * $Date: 2006/10/30 21:26:23 $
 */
#endif
