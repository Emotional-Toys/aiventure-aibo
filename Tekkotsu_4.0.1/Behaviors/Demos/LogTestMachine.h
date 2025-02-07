//-*-c++-*-
#ifndef INCLUDED_LogTestMachine_h_
#define INCLUDED_LogTestMachine_h_

#include "Behaviors/StateNode.h"
#include "Behaviors/Nodes/LedNode.h"
#include "Behaviors/Controls/EventLogger.h"
#include "Shared/ProjectInterface.h"
#include "Behaviors/Transitions/TextMsgTrans.h"
#include "Behaviors/Transitions/NullTrans.h"
#include "Vision/JPEGGenerator.h"

//! a class for testing the image logging facilities
class ImageLogTestNode : public LedNode {
public:
	//!constructor
	ImageLogTestNode(std::string name, int n)
		: LedNode("ImageLogTestNode",name)
	{
		getMC()->displayNumber(n,LedEngine::onedigit);
	}
	virtual void DoStart() {
		LedNode::DoStart();
		EventLogger::logImage(*ProjectInterface::defColorJPEGGenerator,ProjectInterface::fullLayer,0,this);
		sndman->playFile("camera.wav");
	}
};

//! a class for testing the text message logging facilities
class MessageLogTestNode : public LedNode {
public:
	//!constructor
	MessageLogTestNode(std::string name, int n)
		: LedNode("MessageLogTestNode",name)
	{
		getMC()->displayNumber(n,LedEngine::onedigit);
	}
	virtual void DoStart() {
		LedNode::DoStart();
		EventLogger::logMessage("Hello World!",this); //icon and placement arguments also available
	}
};

//! a class for testing the external camera request facilities
class WebcamLogTestNode : public LedNode {
public:
	//!constructor
	WebcamLogTestNode(std::string name, int n)
		: LedNode("WebcamLogTestNode",name)
	{
		getMC()->displayNumber(n,LedEngine::onedigit);
	}
	virtual void DoStart() {
		LedNode::DoStart();
		EventLogger::logWebcam(this);
	}
};


//! tests different methods of the state machine viewer logging facilities
class LogTestMachine : public StateNode {

	// ****************************
	// ******* CONSTRUCTORS *******
	// ****************************
public:
	//! default constructor, use type name as instance name
	LogTestMachine()
		: StateNode("LogTestMachine","LogTestMachine"), start(NULL)
	{}

	//! constructor, take an instance name
	LogTestMachine(const std::string& nm)
		: StateNode("LogTestMachine",nm), start(NULL)
	{}

protected:
  //! constructor for subclasses (which would need to provide a different class name)
  LogTestMachine(const std::string &class_name, const std::string &node_name)
    : StateNode(class_name,node_name), start(NULL)
	{}
	
	
	// ****************************
	// ********* METHODS **********
	// ****************************
public:
	virtual void setup() {
		StateNode::setup(); 

		//setup sub-nodes
		start=addNode(new StateNode("Waiting"));
		ImageLogTestNode * img_node=new ImageLogTestNode("Image",1); addNode(img_node);
		MessageLogTestNode * msg_node=new MessageLogTestNode("Message",2); addNode(msg_node);
		WebcamLogTestNode * webcam_node=new WebcamLogTestNode("Webcam",3); addNode(webcam_node);

		//link with transitions
		start->addTransition(new TextMsgTrans(img_node,"image"));
		start->addTransition(new TextMsgTrans(msg_node,"message"));
		start->addTransition(new TextMsgTrans(webcam_node,"webcam"));
		img_node->addTransition(new NullTrans(start));
		msg_node->addTransition(new NullTrans(start));
		webcam_node->addTransition(new NullTrans(start));
	}

	virtual void DoStart() {
		StateNode::DoStart(); // do this first (required)
		start->DoStart();
	}

	static std::string getClassDescription() { return "Allows testing of various EventLogger facilities via text message events"; }
	virtual std::string getDescription() const { return getClassDescription(); }


	// ****************************
	// ********* MEMBERS **********
	// ****************************
protected:
	StateNode * start; //!< the subnode to begin within on DoStart()


	// ****************************
	// ********** OTHER ***********
	// ****************************
private:
	// Providing declarations for these functions will avoid a compiler warning if
	// you have any class members which are pointers.  However, as it is, an error
	// will result if you inadvertantly cause a call to either (which is probably
	// a good thing, unless you really intended to copy/assign a behavior, in
	// which case simply provide implementations for the functions)
	LogTestMachine(const LogTestMachine&); //!< don't call (copy constructor)
	LogTestMachine& operator=(const LogTestMachine&); //!< don't call (assignment operator)
};


/*! @file
 * @brief Defines LogTestMachine, which allows testing of various EventLogger facilities via text message events
 * @author ejt (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.5 $
 * $State: Exp $
 * $Date: 2006/09/18 18:07:57 $
 */

#endif
