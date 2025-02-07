#include "StateNode.h"
#include "Events/EventRouter.h"
#include "Wireless/Wireless.h"

StateNode::~StateNode() {
	ASSERT(!isActive(), "Destructing while active?")
	for(std::vector<Transition*>::iterator it=transitions.begin(); it!=transitions.end(); it++)
		(*it)->RemoveReference();
	if(issetup) {
		teardown();
		if(issetup) {
			serr->printf("WARNING %s doesn't seem to call StateNode::teardown() in its\n"
			             "        implementation of the function: issetup=%d, nodes.size()=%lu\n"
			             "        Attempting to recover...\n",getClassName().c_str(),issetup,(unsigned long)nodes.size());
			StateNode::teardown();
		}
	}
}

Transition* StateNode::addTransition(Transition* trans) {
	transitions.push_back(trans);
	trans->AddReference();
	trans->addSource(this);
	return trans;
}

StateNode* StateNode::addNode(StateNode* node) {
  nodes.push_back(node);
  node->AddReference();
  if ( node->parent == NULL )
    node->parent = this;
  return node;
}

void StateNode::DoStart() {
  if ( parent == NULL && transitions.size() > 0 )
    serr->printf("WARNING StateNode '%s' has transitions but no parent; you probably forgot to call addNode().\n",getName().c_str());
  BehaviorBase::DoStart();
  if(!issetup) {
    setup();
    issetup=true;
  }
  postStartEvent();
  for(std::vector<Transition*>::iterator it=transitions.begin(); it!=transitions.end(); it++) {
    if ( !(*it)->isActive()  )
			(*it)->DoStart();
		if(!isActive()) //a transition fired upon its start
			break;
	}
}

void StateNode::DoStop() {
	for(std::vector<Transition*>::iterator it=transitions.begin(); it!=transitions.end(); it++) {
		if((*it)->isActive())
			(*it)->DoStop();
	}
	for(std::vector<StateNode*>::iterator it=nodes.begin(); it!=nodes.end(); it++)
		if((*it)->isActive())
			(*it)->DoStop();
	if(!retain && issetup) {
		teardown();
		if(issetup) {
			serr->printf("WARNING %s doesn't seem to call StateNode::teardown() in its\n"
			             "        implementation of the function: issetup=%d, nodes.size()=%lu\n"
			             "        Attempting to recover...\n",getClassName().c_str(),issetup,(unsigned long)nodes.size());
			StateNode::teardown();
		}
	}
	postStopEvent();
	BehaviorBase::DoStop();
}

void StateNode::teardown() {
	for(std::vector<StateNode*>::iterator it=nodes.begin(); it!=nodes.end(); it++)
		(*it)->RemoveReference();
	nodes.clear();
	issetup=false;
	/*std::cout << "Teardown!!!!!!!!" << std::endl;*/
}

void StateNode::postStartEvent() {
	erouter->postEvent(EventBase::stateMachineEGID,reinterpret_cast<size_t>(this),EventBase::activateETID,0,getName(),1);
}

void StateNode::postCompletionEvent(float magnitude/*=0*/) {
	erouter->postEvent(EventBase::stateMachineEGID,reinterpret_cast<size_t>(this),EventBase::statusETID,get_time()-startedTime,getName(),magnitude);
}

void StateNode::postStopEvent() {
	erouter->postEvent(EventBase::stateMachineEGID,reinterpret_cast<size_t>(this),EventBase::deactivateETID,get_time()-startedTime,getName(),0);
}

/*! @file 
 * @brief Describes StateNode, which is both a state machine controller as well as a node within a state machine itself
 * @author ejt (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.26 $
 * $State: Exp $
 * $Date: 2005/11/08 21:36:06 $
 */

