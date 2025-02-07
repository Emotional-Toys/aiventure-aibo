//-*-c++-*-
#ifndef INCLUDED_FreezeTestBehavior_h_
#define INCLUDED_FreezeTestBehavior_h_

#include "Behaviors/BehaviorBase.h"
#include "Wireless/Wireless.h"

//! Demonstrates an infinite loop condition in the Main process
class FreezeTestBehavior : public BehaviorBase {
public:
	FreezeTestBehavior() : BehaviorBase("FreezeTestBehavior")	{}
	
	virtual void DoStart() {
		//call superclass first for housekeeping:
		BehaviorBase::DoStart();

		//now do your code:
		const unsigned int start=2500001;
		serr->printf("Now computing all primes greater than %d... (this might take... forever)\n",start-1);
		serr->printf("Motion process should be able to continue, but Main process will freeze.\n");
		for(unsigned int i=start;;i+=2) {
			unsigned int j=3;
			const unsigned int irt=(unsigned int)sqrt((double)i);
			for(; j<=irt; j++)
				if((i/j)*j==i)
					break;
			if(j==irt)
				serr->printf("%d is prime\n",i);
		}
		//Hate to break it to you, but we're never going to get here...
	}
	
	static std::string getClassDescription() {
		// This string will be shown by the HelpControl or by the tooltips of the Controller GUI
		return "A little demo of a Main process infinite loop freeze (yes, this hangs the AIBO)";
	}
	virtual std::string getDescription() const { return getClassDescription(); }
};

/*! @file
 * @brief Defines FreezeTestBehavior, demonstrates (lack of) blocking using serr to (not) pinpoint a crash
 * @author ejt (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Date: 2004/11/11 01:45:36 $
 */

#endif
