#include <stdlib.h>
#include <vector>

#include "../BehaviorBase.h"
#include "../../Sound/SoundManager.h"
#include "../StateNode.h"
#include "RandomTrans.h"

// Constructors

RandomTrans::RandomTrans(StateNode* destination, float weight) :
  NullTrans("RandomTrans",destination), weights()
   { if (destination!=NULL) addWeight(weight); }

RandomTrans::RandomTrans(const std::string& name, StateNode* destination, float weight) : 
  NullTrans("RandomTrans",name,destination), weights()
   { if (destination!=NULL) addWeight(weight); }

RandomTrans::RandomTrans(const std::string &classname, const std::string &instancename, 
			 StateNode* destination, float weight) :
  NullTrans(classname,instancename,destination), weights()
   { if (destination!=NULL) 
     addWeight(weight>0 ? weight : 0); }

void RandomTrans::addDestination(StateNode* destination, float weight) {
  NullTrans::addDestination(destination);
  addWeight(weight);
}

void RandomTrans::addWeight(float weight) {
  weights.push_back(weight);
}

void RandomTrans::fire() {
  AddReference(); // for safety
	
	erouter->postEvent(EventBase::stateTransitionEGID,reinterpret_cast<size_t>(this),EventBase::activateETID,0,getName(),1);
	
  if ( sound.size()!=0 )
    sndman->playFile(sound);

  for(size_t i=0; i<srcs.size(); i++)
    if(srcs[i]->isActive())  // don't deactivate a non-active node
      srcs[i]->DoStop();

  float weightsum = 0;
  for  (size_t i = 0; i < dsts.size(); i++)
    weightsum += weights[i]; // could check if not in history list, if we had one
  if (weightsum == 0)
    std::cerr << getName() << " has no non-zero-weighted destinations!" << std::endl;
  else {
    const float randval = weightsum * (rand()/(RAND_MAX+1.0));
    float s = weights[0];
    for  (size_t i = 0; i < dsts.size(); s+=weights[++i])
      if (randval <= s) {
	if (!dsts[i]->isActive())
	  dsts[i]->DoStart();
	break;
      };
  }

	erouter->postEvent(EventBase::stateTransitionEGID,reinterpret_cast<size_t>(this),EventBase::deactivateETID,0,getName(),0);
	
  RemoveReference();
}
    
