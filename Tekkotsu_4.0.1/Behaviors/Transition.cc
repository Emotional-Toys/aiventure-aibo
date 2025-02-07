#include "Transition.h"
#include "StateNode.h"
#include "Wireless/Wireless.h"
#include "Sound/SoundManager.h"
#include "Events/EventRouter.h"

void Transition::fire() {
	//serr->printf("%s fire() - enter %d\n",getName().c_str(),get_time());

	AddReference(); //just in case a side effect of this transition is to dereference the transition, we don't want to be deleted while still transitioning

	if(sound.size()!=0)
		sndman->playFile(sound);

	erouter->postEvent(EventBase::stateTransitionEGID,reinterpret_cast<size_t>(this),EventBase::activateETID,0,getName(),1);

	for(unsigned int i=0; i<srcs.size(); i++)
		if(srcs[i]->isActive()) //It's usually a bad idea to call DoStop/DoStart when it's already stopped/started...
			srcs[i]->DoStop();
	for(unsigned int i=0; i<dsts.size(); i++)
		if(!dsts[i]->isActive())
			dsts[i]->DoStart();

	erouter->postEvent(EventBase::stateTransitionEGID,reinterpret_cast<size_t>(this),EventBase::deactivateETID,0,getName(),0);
	
	//serr->printf("%s fire() - leave %d\n",getName().c_str(),get_time());
	RemoveReference();
}

std::string Transition::getName() const {
	if(instanceName != className) {
		return instanceName;
	} else {
		std::string ans;
		ans+='{';
		for(unsigned int i=0; i<srcs.size(); i++)
			ans+=srcs[i]->getName()+(i<srcs.size()-1?',':'}');
		ans+="--"+instanceName+"-->";
		ans+='{';
		for(unsigned int i=0; i<dsts.size(); i++)
			ans+=dsts[i]->getName()+(i<dsts.size()-1?',':'}');
		return ans;
	}
} 

/*! @file
 * @brief Implements Transition, represents a transition between StateNodes.
 * @author ejt (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.19 $
 * $State: Exp $
 * $Date: 2007/03/02 17:20:51 $
 */

