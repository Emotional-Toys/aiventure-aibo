#include "WorldStatePool.h"
#include "Shared/MarkScope.h"
#include "Shared/debuget.h"
#include "Shared/Config.h"
#include "IPC/RCRegion.h"
#ifndef PLATFORM_APERIOS
#  include "Events/EventRouter.h"
#  include "local/LoadDataThread.h"
#endif

//better to put this here instead of the header
using namespace std; 

WorldStatePool::WorldStatePool() : order(), lock() {
	//initialize order with 1,2,3...NUM_STATES
	while(order.size()<order.getMaxCapacity()) {
		unsigned int i=order.size();
		reading[i]=writing[i]=frames[i]=0;
		status[i]=0;
#ifdef PLATFORM_APERIOS
		stateLookupMap[i]=NULL;
#else
		complete[i]=MessageQueueBase::getSemaphoreManager()->getSemaphore();
#endif
		order.push_back(i);
	}
#ifdef PLATFORM_APERIOS
	complete[order.size()-1].lock(ProcessID::getID());
#else
	MessageQueueBase::getSemaphoreManager()->setValue(complete[order.size()-1],1);
#endif
}

void WorldStatePool::doUseResource(Data& d) {
	Request& r=static_cast<Request&>(d);
	if(r.isRead) {
		ReadRequest& rd = static_cast<ReadRequest&>(d);
		if(rd.depth++==0) {
			rd.prev=rd.tgt;
			rd.bufUsed=getCurrentReadState(rd.tgt,rd.bl);
			//cout << ProcessID::getIDStr() << " reading " << rd.bufUsed << " (" << state << ")" << endl;
		}
	} else {
		WriteRequest& write = static_cast<WriteRequest&>(d);
		if(write.depth++==0) {
			useResource(write.srcRequest);
			write.prev=write.tgt;
			write.bufUsed=getCurrentWriteState(write.tgt,write.frame,write.bl);
			if(write.bufUsed!=-1U) {
				write.setStatus(status[write.bufUsed]);
				write.setComplete(isComplete(write.bufUsed));
				//cout << ProcessID::getIDStr() << " writing " << write.bufUsed << " (" << state << ")" << endl;
			}
		}
	}
#ifdef PLATFORM_APERIOS
	stateLookupMap[ProcessID::getID()]=state;
#endif
}
void WorldStatePool::doReleaseResource(Data& d) {
	Request& r=static_cast<Request&>(d);
	if(r.isRead) {
		ReadRequest& rd = static_cast<ReadRequest&>(d);
		if(rd.depth==0)
			throw std::underflow_error("WorldStatePool read resource usage underflow");
		if(--rd.depth==0) {
			if(rd.bufUsed!=-1U) {
				doneReadingState(rd.bufUsed);
				rd.bufUsed=-1U;
			}
			rd.tgt=rd.prev;
		}
	} else {
		WriteRequest& write = static_cast<WriteRequest&>(d);
		if(write.depth==0)
			throw std::underflow_error("WorldStatePool write resource usage underflow");
		if(--write.depth==0) {
			if(write.bufUsed!=-1U) {
				status[write.bufUsed]=write.getStatus();
				doneWritingState(write.bufUsed,write.getComplete());
				write.bufUsed=-1U;
			}
			write.tgt=write.prev;
			releaseResource(write.srcRequest);
		}
	}
#ifdef PLATFORM_APERIOS
	stateLookupMap[ProcessID::getID()]=state;
#endif
}

#ifdef PLATFORM_APERIOS

WorldStatePool::UpdateInfo* WorldStatePool::isUnread(OSensorFrameVectorData& msg, unsigned int& lastFrameNumber) {
	//cout << ProcessID::getIDStr() << " checking tgtFr=" << msg.GetInfo(0)->frameNumber << " lastFr=" << lastFrameNumber << endl;
	
	static UpdateInfo info; // avoiding heap allocation by using static here, but this means not threadsafe (shouldn't be a problem...)
	info.msg=&msg;
	info.frameNumber=msg.GetInfo(0)->frameNumber;
	unsigned int toUse=selectWriteState(info.frameNumber,true);
#ifdef DEBUG
	info.intendedBuf=toUse;
#endif
	if(toUse==-1U)
		return NULL; //error occurred, should already be reported
	//cout << ProcessID::getIDStr() << " Intending to use " << toUse << " holding frame " << frames[toUse] << " marked " << (isComplete(toUse)?"complete":"not complete") << " with status " << status[toUse] << " for frame " << info.frameNumber << endl;
	if(frames[toUse]==info.frameNumber && isComplete(toUse)) {
		if(info.frameNumber-lastFrameNumber!=NumFrames)
			cout << ProcessID::getIDStr() << " dropped " << (info.frameNumber-lastFrameNumber-NumFrames)/NumFrames << " sensor frame(s)" << endl;
		lastFrameNumber=info.frameNumber;
		//cout << ProcessID::getIDStr() << " decided frame is complete or nothing to contribute" << endl;
		return NULL;
	}
	return &info;
}

#else //PLATFORM_LOCAL

WorldStatePool::UpdateInfo* WorldStatePool::isUnread(const RCRegion& msg, unsigned int curFrameNumber, unsigned int& lastFrameNumber, bool /*haveFeedback*/, bool /*motionOverride*/) {
	//cout << ProcessID::getIDStr() << " checking tgtFr=" << curFrameNumber << " lastFr=" << lastFrameNumber << " fdbkAv=" << haveFeedback << " ovrd="<<motionOverride << endl;
	
	static UpdateInfo info; // avoiding heap allocation by using static here, but this means not threadsafe (shouldn't be a problem...)
	info.payload=LoadDataThread::deserializeHeader(msg.Base(),msg.Size(),&info.verbose,&info.frameNumber,&info.filename,&info.dataInQueue,&info.payloadSize);
	if(info.payload==NULL)
		throw std::runtime_error("deserialization of sensor update header failed");
	if(info.frameNumber<curFrameNumber) {
		// this means there's a newer one already in the queue -- skip this (and we'll report the drop next time -- no assignment to lastFrameNumber)
		return NULL;
	}
	if(info.payloadSize==0)
		info.payload=NULL;
	
	unsigned int toUse=selectWriteState(info.frameNumber,true);
#ifdef DEBUG
	info.intendedBuf=toUse;
#endif
	if(toUse==-1U)
		return NULL; //error occurred, should already be reported
	if(info.verbose>=3 && info.payloadSize==0)
		cout << ProcessID::getIDStr() << " received sensor heartbeat at " << get_time() << endl;
	else if(info.verbose>=2 && info.payloadSize!=0)
		cout << ProcessID::getIDStr() << " received sensor data \"" << info.filename << "\" at " << get_time() << endl;
	//cout << "Intending to use " << toUse << " holding frame " << frames[toUse] << " marked " << (isComplete(toUse)?"complete":"not complete") << " with status " << status[toUse] << " for frame " << info.frameNumber << endl;
	if(frames[toUse]==info.frameNumber && isComplete(toUse)) {
		if(info.frameNumber-lastFrameNumber!=1 && info.verbose>=1)
			cout << ProcessID::getIDStr() << " dropped " << (info.frameNumber-lastFrameNumber-1) << " sensor frame(s)" << endl;
		lastFrameNumber=info.frameNumber;
		//cout << ProcessID::getIDStr() << " decided frame is complete or nothing to contribute" << endl;
		return NULL;
	}
	return &info;
}

bool WorldStatePool::read(const WorldStatePool::UpdateInfo& info, WriteRequest& wsw, bool feedbackGenerated, bool zeroPIDFeedback, const PostureEngine* feedback/*=NULL*/) {
	//cout << ProcessID::getIDStr() << " writing " << wsw.frame << " state=" << state << "(" << wsw.bufUsed << ") source=" << wsw.src << "("<<wsw.srcRequest.bufUsed<<") status="<<wsw.getStatus() << " complete="<<wsw.getComplete() << endl;
	
	ASSERT(feedbackGenerated || feedback==NULL, "feedbackGenerated is false, yet feedback was supplied?");
	if(wsw.getComplete()) {
		// this could happen if both processes were trying to get the write lock at the same time
		// and the first in filled everything by itself (i.e. no feedback required, or the one with feedback was there first)
		return true;
	}	
	ASSERT((wsw.getStatus()&FEEDBACK_APPLIED)==0, "feedback applied, but apparently not completed "<<wsw.getComplete());
	//ASSERT(wsw.bufUsed==info.intendedBuf,"read() not using expected state buffer"); //not a problem (I think? just fell an even number of frames behind the other process?)
	
	if((wsw.getStatus()&SENSORS_APPLIED) != 0) {
		// sensors already applied, just need to do feedback, if any
		if(feedback!=NULL) {
			// have feedback, copy it in
			if(zeroPIDFeedback) {
				// copy all values, regardless of PID values
				for(unsigned int i=0; i<NumOutputs; i++)
					state->outputs[i]=feedback->getOutputCmd(i).value;
			} else {
				// only copy non-zero PID values
				for(unsigned int i=0; i<NumOutputs; i++)
					if(!isZeroPID(wsw.src,i))
						state->outputs[i]=feedback->getOutputCmd(i).value;
			}
			//Apply calibration to the joints (this reverses the calibration from MotionManager
			for (uint i=0; i<NumPIDJoints; i++)
				state->outputs[i+PIDJointOffset] = state->outputs[i+PIDJointOffset] / config->motion.calibration_scale[i]
					- config->motion.calibration_offset[i];
			wsw.setStatus(wsw.getStatus() | FEEDBACK_APPLIED);
		}
	
	} else {
		// need to do sensors (if present), as well as feedback (if any)
		static PostureEngine pose; //this is already thread-unsafe, might as well use another static tmp here to avoid initialization cost
		if(info.payload==NULL) {
			
			// no new sensors, but still have to copy over old values (downside of having multiple state buffers)
			pose.setLoadedSensors(wsw.src);
			if(!feedbackGenerated) {
				// no feedback either, take all of the old positions
				pose.takeSnapshot(*wsw.src);
				pose.setWeights(1);
			} else if(zeroPIDFeedback) {
				// feedback is applied, regardless of PID settings
				if(feedback!=NULL) {
					for(unsigned int i=0; i<NumPIDJoints; ++i)
						pose(i+PIDJointOffset)=feedback->getOutputCmd(i).value;
					wsw.setStatus(wsw.getStatus() | FEEDBACK_APPLIED);
				} //else wait for process with feedback to fill it in
			} else {
				// feedback is only applied to non-zero PIDs, the rest need to take the previous value
				if(feedback!=NULL) {
					// this process does have the feedback, fill in everything 
					for(unsigned int i=0; i<NumPIDJoints; ++i)
						if(isZeroPID(wsw.src,i))
							pose(i+PIDJointOffset)=wsw.src->outputs[i+PIDJointOffset];
						else
							pose(i+PIDJointOffset)=feedback->getOutputCmd(i).value;
					wsw.setStatus(wsw.getStatus() | FEEDBACK_APPLIED);
				} else {
					// this process doesn't have the feedback, only fill in the non-feedback
					for(unsigned int i=0; i<NumPIDJoints; ++i)
						if(isZeroPID(wsw.src,i))
							pose(i+PIDJointOffset)=wsw.src->outputs[i+PIDJointOffset];
				}
			}
			
		} else {
			
			unsigned int stateFrame=state->frameNumber; //back up current frame number so we can restore it after the load 
			// new sensor values are provided
			pose.setLoadedSensors(state);
			//cout << "Parsing sensor data " << info.payloadSize << " bytes" << endl;
			//cout << string(info.payload,info.payloadSize) << endl;
			size_t x=pose.loadBuffer(info.payload,info.payloadSize);
			//cout << "read " << x << endl;
			if(!x) {
				cerr << "ERROR: Corrupted sensor readings received by Main" << endl;
				return false;
			}
			state->frameNumber=stateFrame; // restore the frame number over what was loaded from the file so state->read doesn't unnecessarily skip it below
			
			if(stateFrame>wsw.src->frameNumber)
				return false; // I'm not sure how this occurs, but sometimes (if doing a lot of paging/kernel stuff that screws up the scheduler) the source is newer than our update
			ASSERT(stateFrame<wsw.src->frameNumber || stateFrame==1 && wsw.src->frameNumber==1,"already updated (" << stateFrame << " vs " << wsw.src->frameNumber << ")?  So why did " << ProcessID::getIDStr() << " parse...");
			
			if(!feedbackGenerated) {
				// no feedback, so just use all of the values we just loaded
				// weight may be 0 if sensor which sent update doesn't provide joint positions
				//for(unsigned int i=0; i<NumOutputs; ++i)
				//ASSERT(pose(i).weight>0,"zero weight found after loading"); // one would expect the compiler to no-op this loop ifndef DEBUG
			} else if(zeroPIDFeedback) {
				// feedback is applied, regardless of PID settings
				if(feedback!=NULL) {
					for(unsigned int i=0; i<NumPIDJoints; ++i)
						pose(i+PIDJointOffset)=feedback->getOutputCmd(i).value;
					wsw.setStatus(wsw.getStatus() | FEEDBACK_APPLIED);
				} //else wait for process with feedback to fill it in
			} else {
				// feedback is only applied to non-zero PIDs, the rest need to take the previous value
				if(feedback!=NULL) {
					// this process does have the feedback, fill in everything 
					for(unsigned int i=0; i<NumPIDJoints; ++i)
						if(!isZeroPID(wsw.src,i))
							pose(i+PIDJointOffset)=feedback->getOutputCmd(i).value;
						/*else // this can happen validly if starting up with zero PID and unknown positions
							ASSERT(pose(i+PIDJointOffset).weight!=0,"zero weight found after loading");*/
					wsw.setStatus(wsw.getStatus() | FEEDBACK_APPLIED);
				} else {
					// this process doesn't have the feedback, only fill in the non-feedback
					for(unsigned int i=0; i<NumPIDJoints; ++i)
						if(!isZeroPID(wsw.src,i))
							pose(i+PIDJointOffset).weight=0; // don't apply sensor values to non-zero pids (this will come from feedback later)
						/*else // this can happen validly if starting up with zero PID and unknown positions
							ASSERT(pose(i+PIDJointOffset).weight!=0,"zero weight found after loading");*/
				}
			}
		}
		state->read(pose,wsw.src,erouter);
		//ASSERT((state->frameNumber-5)/4==wsw.frame,"state frame number and message serial number are desynchronized " << state->frameNumber << " vs " << wsw.frame);
		state->frameNumber=(wsw.frame+1)*NumFrames+1; //lets dropped messages be skipped
		wsw.setStatus(wsw.getStatus() | SENSORS_APPLIED);
	}
	wsw.setComplete(feedbackGenerated && wsw.getStatus()==(SENSORS_APPLIED|FEEDBACK_APPLIED) || !feedbackGenerated && (wsw.getStatus()&SENSORS_APPLIED)!=0);
	//cout << ProcessID::getIDStr() << " fr=" << info.frameNumber << " pyld=" << info.payloadSize << " fdbkG=" << feedbackGenerated << " pidfdbk=" << zeroPIDFeedback << " fdbk=" << feedback << " set status="<<wsw.getStatus() << " complete=" << wsw.getComplete() << endl;
	return true;
}

#endif


unsigned int WorldStatePool::getCurrentReadState(WorldState*& tgt, bool block) {
	unsigned int toUse=-1U;
	if(block) {
		while(toUse!=order.back()) {
			{
				MarkScope l(lock);
				if(toUse!=-1U) { //newer frame added while we were waiting
					//std::cerr << ProcessID::getID() << " WARNING WorldStatePool: whoa new frame while blocking read " << toUse << " vs " << order.back() << endl;
					reading[toUse]--;
				}
				toUse=order.back();
				reading[toUse]++;
			} //release WorldStatePool lock to allow new readers to come through
#ifdef PLATFORM_APERIOS
			MarkScope wl(complete[toUse]); //now block until writing lock is released
#else
			MessageQueueBase::getSemaphoreManager()->testZero(complete[toUse],true); //block until complete semaphore is released (set to 0)
#endif
		}
	} else {
		MarkScope l(lock);
		order_t::index_t idx;
		for(idx=order.prev(order.end()); idx!=order.end(); idx=order.prev(idx)) {
			toUse=order[idx];
			if(isComplete(toUse))
				break;
		}
		if(idx==order.end()) {
			std::cerr << "ERROR: WorldStatePool unable to read state because none available" << std::endl;
			//return -1U;
			std::cerr << "       falling into most recent buffer" << std::endl;
			toUse=order.prev(order.end());
		}
		reading[toUse]++;
	}
	//std::cout << ProcessID::getID() << " reading " << toUse << endl;
	//ASSERTRETVAL(toUse!=-1U,"toUse was not chosen!",-1U);
#ifdef DEBUG
	if(toUse==-1U) {
		std::cerr << "ERROR: WorldStatePool: toUse was not chosen!" << std::endl;
		//return -1U;
		std::cerr << "       falling into most recent buffer" << std::endl;
		toUse=order.prev(order.end());
		reading[toUse]++;
	}
#endif
	tgt=&states[toUse];
	return toUse;
}
void WorldStatePool::doneReadingState(unsigned int i) {
	MarkScope l(lock);
	reading[i]--;
	//std::cout << ProcessID::getID() << " reading " << i << " - done" << endl;
}

bool WorldStatePool::isComplete(unsigned int idx) const {
#ifdef PLATFORM_APERIOS
	return static_cast<unsigned int>(complete[idx].owner())==MutexLockBase::NO_OWNER;
#else
	return MessageQueueBase::getSemaphoreManager()->testZero(complete[idx],false);
#endif
}

unsigned int WorldStatePool::selectWriteState(unsigned int frame, bool block, order_t::index_t& idx) const {
	//check the end to verify a newer (or equivalent) update isn't already in progress
	idx=order.prev(order.end());
	unsigned int toUse=order[idx];
	if(frames[toUse]>frame) {
		// can happen if one thread (say MotionExec running a motion update) takes too long, another thread (say motion gotSensors updater) blocks on it, is delayed too long
		// we'll leave it up to the client to detect and display an appropriate message if dropping updates is a problem.
		//std::cerr << "WARNING: WorldStatePool found a newer write already in progress (writing " << frame << ", found " << frames[toUse] << ")" << std::endl;
		return -1U;
	}
	if(frames[toUse]!=frame || !block) {
		//start at the beginning and run forward to get the oldest unreferenced entry
		order_t::index_t fallback=order.end();
		for(idx=order.begin(); idx!=order.end(); idx=order.next(idx)) {
			toUse=order[idx];
			if(frames[toUse]>=frame) {
				idx=order.end();
				break;
			}
			if(writing[toUse]==0) { //note: could be incomplete -- if this is the oldest frame and it's not currently being written to, but yet wasn't completed, we'll recycle it
				if(reading[toUse]==0)
					break; //found our entry!
			} else if(fallback==order.end())
				fallback=idx; //in case we don't find an unreferenced one later in the list
		}
		if(idx==order.end()) {
			if(block && fallback!=order.end())
				toUse=order[idx=fallback];
			else {
				std::cerr << "ERROR: WorldStatePool unable to update state because none available" << std::endl;
				return -1U;
			}
		}
	}
	return toUse;
}

unsigned int WorldStatePool::getCurrentWriteState(WorldState*& tgt, unsigned int frame, bool block) {
	unsigned int toUse;
	{ //artificial scope to limit lock on next line
		MarkScope l(lock);
		order_t::index_t idx;
		toUse=selectWriteState(frame,block,idx);
		if(toUse==-1U)
			return -1U;
		//std::cout << ProcessID::getID() << " writing " << toUse << " (frame " << frame << ")" << endl;
		//ASSERTRETVAL(toUse!=-1U,"toUse was not chosen!",-1U);
		writing[toUse]++;
		if(frames[toUse]!=frame) {
			//move this entry to the back of the list, keep frames sorted (in order of write initiation / frame number)
			order.erase(idx);
			order.push_back(toUse);
			frames[toUse]=frame;
			status[toUse]=0;
#ifdef PLATFORM_APERIOS
			complete[toUse].releaseAll(); //make sure it wasn't an incomplete frame that's being recycled
			complete[toUse].lock(ProcessID::getID());
#else
			MessageQueueBase::getSemaphoreManager()->setValue(complete[toUse],1);
#endif
		}
	}
	if(block) {
		writeLocks[toUse].lock(ProcessID::getID());
	} else if(!writeLocks[toUse].try_lock(ProcessID::getID())) {
		std::cerr << "WARNING: WorldStatePool unable to acquire write lock (blocking reader?).  Trying again..." << std::endl;
		writeLocks[toUse].lock(ProcessID::getID());
	}
	if(toUse!=order.back()) { //newer frame added while we were waiting
		// This is a very rare case
		//std::cerr << ProcessID::getID() << " WARNING WorldStatePool: whoa new frame while blocking write " << toUse << " vs " << order.back() << endl;
		// go back to square one
		doneWritingState(toUse,false);
		return getCurrentWriteState(tgt,frame,block);
	}
	tgt=&states[toUse];
	return toUse;
}
void WorldStatePool::doneWritingState(unsigned int i, bool completed) {
	MarkScope l(lock);
	//std::cout << ProcessID::getID() << " writing " << i << " - done" << endl;
	writing[i]--;
	if(completed) {
#ifdef PLATFORM_APERIOS
		if(!isComplete(i))
			complete[i].unlock();
#else
		MessageQueueBase::getSemaphoreManager()->setValue(complete[i],0);
#endif
	}
	writeLocks[i].unlock();
}



/*! @file
 * @brief 
 * @author Ethan Tira-Thompson (ejt) (Creator)
 *
 * $Author: kcomer $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.30 $
 * $State: Exp $
 * $Date: 2007/11/15 21:33:04 $
 */
