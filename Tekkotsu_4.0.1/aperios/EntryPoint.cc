#include "EntryPoint.h"

void EntryPoint::setPool(WorldStatePool* wspool) {
	if(wspool==pool)
		return;
	if(pool!=NULL) {
		for(std::list<Data*>::reverse_iterator cit=data.rbegin(); cit!=data.rend(); ++cit)
			pool->doReleaseResource(**cit);
	}
	pool=wspool;
	if(pool!=NULL) {
		for(std::list<Data*>::iterator cit=data.begin(); cit!=data.end(); ++cit)
			if(*cit!=NULL)
				pool->doUseResource(**cit);
	}
}

void EntryPoint::useResource(Data& d) {
	data.push_back(&d);
	if(data.size()==1 || epFrame==NULL) {
		epFrame=ProcessID::getMapFrame();
		if(epFrame!=NULL) { // check that setMap has been called
#ifdef DEBUG_STACKTRACE
			epFrame->debug=0;
#endif
			getCurrentStackFrame(epFrame);
			while(unrollStackFrame(epFrame,epFrame)) {}
		}
	}
	if(pool!=NULL) {
		pool->doUseResource(*data.back());
	}/* else if(pool==NULL)
		cout << ProcessID::getIDStr() << " pool is NULL" << endl; */
}

void EntryPoint::releaseResource(Data& d) {
	ASSERTRET(data.size()>0,"EntryPoint::releaseResource underflow");
	ASSERT(data.back()==&d,"Warning: data changed between resource usage and release");
	if(WorldStateWrite * wsw=dynamic_cast<WorldStateWrite*>(&d)) {
		if(wsw->getComplete() && state!=NULL && state!=wsw->src && wsw->src!=NULL) {
			/*for(unsigned int i=0; i<NumPIDJoints; i++) {
			pids[i][0]=lastState->pids[i][0];
			pids[i][1]=lastState->pids[i][1];
			pids[i][2]=lastState->pids[i][2];
			}*/
			memcpy(state->pids,wsw->src->pids,sizeof(state->pids)); //pids is probably big enough it's better to use memcpy (?)
			state->vel_x=wsw->src->vel_x;
			state->vel_y=wsw->src->vel_y;
			state->vel_a=wsw->src->vel_a;
			state->vel_time=wsw->src->vel_time;
		}
	}
	if(pool!=NULL) {
		pool->doReleaseResource(*data.back());
	}/* else if(pool==NULL)
		cout << ProcessID::getIDStr() << " pool is NULL" << endl; */
	if(data.size()==1) {
#ifdef DEBUG_STACKTRACE
		//if(epFrame!=NULL)
		//memset(epFrame,0,sizeof(StackFrame));
#endif
		epFrame=NULL;
	}
	data.pop_back();
}

/*! @file
* @brief 
* @author Ethan Tira-Thompson (ejt) (Creator)
*
* $Author: ejt $
* $Name: tekkotsu-4_0-branch $
* $Revision: 1.1 $
* $State: Exp $
* $Date: 2006/08/22 22:23:05 $
*/
