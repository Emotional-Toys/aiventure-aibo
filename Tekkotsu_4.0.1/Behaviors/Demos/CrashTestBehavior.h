//-*-c++-*-
#ifndef INCLUDED_CrashTestBehavior_h_
#define INCLUDED_CrashTestBehavior_h_

#include "Behaviors/BehaviorBase.h"
#include "Wireless/Wireless.h"

//! Demonstrates (lack of) blocking using serr to (not) pinpoint a crash
class CrashTestBehavior : public BehaviorBase {
public:
	CrashTestBehavior() : BehaviorBase("CrashTestBehavior")	{}
	
	virtual void DoStart() {
		//call superclass first for housekeeping:
		BehaviorBase::DoStart();

		serr->printf("I will now crash immediately following line 33\n");
		//now do your code:
		for(unsigned int i=0; i<100; i++) {
			serr->printf("Hello serr!  This is %d\n",i);
			if(i==33)
				*(int*)0xDEADDEAD=0x600DB4E;
		}
		//Hate to break it to you, but we're never going to get here...
	}
	
	static std::string getClassDescription() {
		// This string will be shown by the HelpControl or by the tooltips of the Controller GUI
		return "A little demo of blocking output before a crash after output #33 (yes, this crashes the AIBO)";
	}
	
	virtual std::string getDescription() const { return getClassDescription(); }
};

/*! @file
 * @brief Defines CrashTestBehavior, demonstrates (lack of) blocking using serr to (not) pinpoint a crash
 * @author ejt (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.3 $
 * $State: Exp $
 * $Date: 2004/11/11 01:45:35 $
 */

#endif
