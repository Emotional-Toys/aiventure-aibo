#include "IPCMotionHook.h"
#include "IPC/MessageQueue.h"
#include <sstream>

using namespace std;

IPCMotionHook::~IPCMotionHook() {
	for(msgbuf_t::iterator it=regions.begin();it!=regions.end(); ++it)
		(*it)->RemoveReference();
}

void IPCMotionHook::motionCheck(const float outputs[][NumOutputs]) {
	RCRegion * r = getRegion();
	memcpy(r->Base(),outputs,r->Size());
	mq.sendMessage(r);
	regions.push_back(r);
}

RCRegion* IPCMotionHook::getRegion() {
	for(msgbuf_t::iterator it=regions.begin();it!=regions.end(); ++it) {
		if((*it)->NumberOfReference()==1) {
			RCRegion * ans=*it;
			regions.erase(it);
			return ans;
		}
	}
	// no unused regions found, make a new one
	static int count=0; // how many have we made (unique names for easier debugging)
	stringstream ss;
	ss << "MotionUpdate." << count++;
	cout << "Created " << ss.str() << endl;
	return new RCRegion(ss.str(),sizeof(float)*NumOutputs*NumFrames);
}



/*! @file
 * @brief 
 * @author Ethan Tira-Thompson (ejt) (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.1 $
 * $State: Exp $
 * $Date: 2007/06/03 17:03:26 $
 */
