#include "LoadDataThread.h"
#include "Shared/get_time.h"
#include "Shared/debuget.h"
#include <sys/stat.h>
#include <regex.h>
#include <dirent.h>
#include <set>
#include <errno.h>

using namespace std;

// allocated here to avoid creating otherwise empty DataSource.cc
unsigned int * DataSource::providedOutputs=NULL;
bool DataSource::requiresFirstSensor=false;

LoadDataThread::~LoadDataThread() {
	MarkScope l(lock);
	DeviceDriver::getRegistry().removeCollectionListener(this);
	framerate.removePrimitiveListener(this);
	verbose.removePrimitiveListener(this);
	frozen.removePrimitiveListener(this);
	while(regions.size()>0) {
		freeRegion(regions.front());
		regions.pop_front();
	}
}

bool LoadDataThread::advanceFrame(bool forceQueue) {
	MarkScope l(lock);
	if(frozen && !forceQueue || curDS==NULL) {
		if(heartbeat)
			sendHeartbeat();
		return false;
	}
	
	// check to see if runloop has a frame in progress -- send that one ASAP, don't cut line
	if(runloopState==GET_FRAME) {
		runloopState=ADVANCE_FRAME;
		return true;
	} else if(runloopState==GOT_FRAME) {
		runloopState=ADVANCE_FRAME;
		interrupt(); //break out of sleep
		return true;
	}
	
	//! safe to force the next frame ourselves
	const char* payload;
	unsigned int sz;
	unsigned int advTime = get_time();
	unsigned int timestamp = (forceQueue ? 0 : advTime);
	std::string name;
	unsigned int sn=curDS->getData(payload, sz, timestamp, name);
	if(payload==NULL) {
		if(heartbeat)
			sendHeartbeat();
		return false;
	} else {
		if(verbose>=1 && sn!=sourceSN+1 && sourceSN!=0)
			std::cout << "Data Source dropped " << (sn-sourceSN-1) << " frames"  << std::endl;
		sourceSN=sn;
	}
	if(verbose>=2)
		std::cout << "Advancing frame: \"" << name << "\" (advanced at " << advTime << ')' << std::endl;
	RCRegion * r=firstUnusedRegion();
	char * buf=setupRegion(r,name,sz,curDS->nextTimestamp()!=-1U);
	ASSERTRETVAL(buf!=NULL,"setupRegion returned null region",false);
	memcpy(buf,payload,sz);
	msgr.sendMessage(r);
	regions.push_back(r);
	lastSent=advTime;
	return true;
}

void LoadDataThread::plistValueChanged(const plist::PrimitiveBase& pl) {
	MarkScope l(lock);
	if(&pl==&source) {
		std::string::size_type dot=source.find('.');
		DeviceDriver* dd=DeviceDriver::getRegistry().getInstance(source.substr(0,dot));
		if(dd==NULL) {
			if(source.size()!=0)
				std::cerr << "Could not find driver named '" << source << "' for LoadDataThread" << std::endl;
			setDataSource(dd, NULL);
		} else {
			std::map<std::string,DataSource*> dsmap;
			(dd->*getDataSources)(dsmap);
			if(dsmap.size()==0)
				setDataSource(dd, NULL);
			else if(dot==std::string::npos)
				setDataSource(dd, dsmap.begin()->second);
			else {
				std::map<std::string,DataSource*>::const_iterator ds=dsmap.find(source.substr(dot+1));
				setDataSource(dd, ds==dsmap.end() ? NULL : ds->second);
			}
		}
	} else if(&pl==&framerate) {
		if(curDS!=NULL)
			curDS->setDataSourceFramerate(framerate);
	} else if(&pl==&verbose) {
		msgr.setReportDroppings(verbose>=1);
		if(curDS!=NULL)
			curDS->setDataSourceVerbose(verbose);
	} else if(&pl==&frozen) {
		if(curDS!=NULL)
			curDS->setFrozen(frozen);
		if(enabled) {
			if(!frozen) {
				start();
			} else if(isStarted() && !heartbeat) {
				stop();
				enabled=true; // retain enabled setting
			}
		}
	} else if(&pl==&heartbeat) {
		if(enabled) {
			if(heartbeat) {
				start();
			} else if(isStarted() && frozen) {
				stop();
				enabled=true; // retain enabled setting
			}
		}
	} else {
		cerr << "LoadDataThread didn't handle call to plistValueChanged for " << pl.get() << endl;
	}
}

void LoadDataThread::start() {
	enabled=true;
	if(!isStarted() && getTimeScale()>0 && (!frozen || heartbeat) )
		PollThread::start();
}
void LoadDataThread::stop() {
	enabled=false;
	runloopState=INTERFRAME;
	PollThread::stop();
}

RCRegion* LoadDataThread::firstUnusedRegion() {
	for(msgbuf_t::iterator it=regions.begin();it!=regions.end(); ++it) {
		if((*it)->NumberOfReference()==1) {
			RCRegion * ans=*it;
			regions.erase(it);
			return ans;
		}
	}
	return NULL;
}

char* LoadDataThread::setupRegion(RCRegion*& region, const std::string& filename, unsigned int payload, bool hasMoreData) {
	// header fields: bool:verbose, uint:frameSN, str:filename, uint:payload
	const unsigned int FILE_HEADER_SIZE=LoadSave::getSerializedSize<unsigned int>() + LoadSave::getSerializedSize(frameSN) + LoadSave::getSerializedSize(filename) + LoadSave::getSerializedSize<bool>() + LoadSave::getSerializedSize(payload);
	if(region==NULL)
		region=new RCRegion(FILE_HEADER_SIZE+payload+sizeof(char)*filename.size()*2); //triple the allocation for filename so we don't have to resize if we get a longer name later
	else if(region->Size()<FILE_HEADER_SIZE+payload) {
		//too small -- free it, we'll drop it and make a bigger one
		freeRegion(region);
		region=new RCRegion(FILE_HEADER_SIZE+payload+sizeof(char)*filename.size()*2); //triple the allocation for filename so we don't have to resize if we get a longer name later
	}
	char* buf=region->Base();
	unsigned int remain=FILE_HEADER_SIZE;
	if(!LoadSave::encodeInc((unsigned int)verbose,buf,remain)) return NULL;
	if(!LoadSave::encodeInc(frameSN++,buf,remain)) return NULL;
	if(!LoadSave::encodeInc(filename,buf,remain)) return NULL;
	if(!LoadSave::encodeInc(hasMoreData,buf,remain)) return NULL;
	if(!LoadSave::encodeInc(payload,buf,remain)) return NULL;
	ASSERT(remain==0,"LoadDataThread::setupRegion(): Leftover bytes in header? FILE_HEADER_SIZE is wrong\n");
	return buf;
}

char* LoadDataThread::deserializeHeader(char* buf, unsigned int size, unsigned int* verbose, unsigned int* sn, std::string* filename, bool* dataInQueue, unsigned int* payloadSize) {
	if(verbose!=NULL) { if(!LoadSave::decodeInc(*verbose,buf,size)) return NULL; } else buf+=LoadSave::getSerializedSize(*verbose);
	if(sn!=NULL) { if(!LoadSave::decodeInc(*sn,buf,size)) return NULL; } else buf+=LoadSave::getSerializedSize(*sn);
	if(filename!=NULL) { if(!LoadSave::decodeInc(*filename,buf,size)) return NULL; } else {
		unsigned int len=0;
		if(!LoadSave::decodeInc(len,buf,size)) return NULL;
		buf+=len+1;
	}
	if(dataInQueue!=NULL) { if(!LoadSave::decodeInc(*dataInQueue,buf,size)) return NULL; } else buf+=LoadSave::getSerializedSize(*dataInQueue);
#ifndef DEBUG
	if(payloadSize!=NULL) { if(!LoadSave::decodeInc(*payloadSize,buf,size)) return NULL; } else buf+=LoadSave::getSerializedSize(*payloadSize);
#else
	unsigned int lPayloadSize; // want to error check payloadSize regardless of whether caller wants the info
	if(payloadSize==NULL)
		payloadSize=&lPayloadSize;
	if(!LoadSave::decodeInc(*payloadSize,buf,size)) return NULL;
	ASSERT(size>=*payloadSize,"short payload (" << size << " vs expected " << *payloadSize << ")");
	// this is normal (regions are generated a little bigger than they need to be to allow recycling)
	//ASSERT(size<=*payloadSize,"unhandled bytes after payload (" << size << " vs expected " << *payloadSize << ")");
#endif
	return buf;
}

void LoadDataThread::sendHeartbeat(unsigned int curt) {
	RCRegion *unused=firstUnusedRegion();
	char * buf=setupRegion(unused,"heartbeat",0,curDS!=NULL && curDS->nextTimestamp()!=-1U);
	if(unused==NULL || buf==NULL)
		return;
	if(verbose>=3)
		std::cout << "Sending heartbeat at " << curt << std::endl;
	msgr.sendMessage(unused);
	regions.push_back(unused);
	lastSent=curt;
}

unsigned int LoadDataThread::calcNextHeartbeat(unsigned int curt) const {
	unsigned int skip=static_cast<unsigned int>((curt-lastSent)*framerate/1000);
	return static_cast<unsigned int>(lastSent+(skip+1)*1000.f/framerate);
}

std::string LoadDataThread::getAvailableDataSources() {
	std::string str;
	for(DeviceDriver::registry_t::const_iterator dit=DeviceDriver::getRegistry().begin(); dit!=DeviceDriver::getRegistry().end(); ++dit) {
		DeviceDriver * dd=DeviceDriver::getRegistry().getInstance(dit->first);
		if(dd!=NULL) {
			std::map<std::string,DataSource*> srcmap;
			(dd->*getDataSources)(srcmap);
			for(std::map<std::string,DataSource*>::const_iterator sit=srcmap.begin(); sit!=srcmap.end(); ++sit)
				str+=dit->first+'.'+sit->first+' ';
		}
	}
	std::string sourcehelp="Names a DeviceDriver instance from which data will be taken.\n"
		"Can be either just the driver name (use first data source), or 'DriverName.QueueName'.\n"
		"Available data sources: ";
	if(str.size()==0)
		sourcehelp+="(none available, see 'new' command to instantiate drivers)";
	else
		sourcehelp+=str;
	setComment("Source",sourcehelp);
	if(str.size()>0) // we actually want the extra space at the end in the plist::Dictionary comments, but we'll remove it before we return
		str.resize(str.size()-1);
	plistValueChanged(source); // check that our source is still valid (or if it wasn't valid, perhaps now it is)
	return str;
}

void LoadDataThread::setDataSource(DeviceDriver* dd, DataSource* ds) {
	if(curDS==ds && dsDriver==dd)
		return;
	if(dsDriver!=NULL)
		dsDriver->removeSourceListener(this);
	if(dd!=NULL)
		dd->addSourceListener(this);
	dsDriver=dd;
	if(curDS!=NULL) {
		curDS->setDataSourceThread(NULL);
		curDS=NULL;
		if(isStarted()) {
			stop();
			enabled=true; // retain enabled setting
			for(unsigned int checks=0; checks<8 && isStarted(); checks++)
				usleep(250*1000);
			if(isStarted()) {
				cerr << "Trying to close LoadDataThread's data source, looks like it's blocked, killing..." << endl;
				kill();
				join();
			}
		}
	}
	curDS=ds;
	sourceSN=0;
	lastSent=-1;
	if(curDS!=NULL) {
		curDS->setDataSourceThread(this);
		curDS->setDataSourceVerbose(verbose);
		curDS->setDataSourceFramerate(framerate);
		curDS->setFrozen(frozen);
		if(enabled)
			start();
	}
}

unsigned int LoadDataThread::runloop() {
	float nextT = (lastSent<0) ? get_time() : (lastSent+1000.f/framerate);
	const char* payload;
	unsigned int sz;
	unsigned int timestamp=static_cast<unsigned int>(nextT+.5f);
	std::string name;
	{
		MarkScope l(lock);
		unsigned int curt=get_time();
		if(frozen || curDS==NULL) {
			if(heartbeat && curt>=nextT)
				sendHeartbeat(curt);
			return calcSleepTime();
		}
		runloopState=GET_FRAME;
	}
	unsigned int sn=curDS->getData(payload, sz, timestamp, name);
	if(curDS==NULL) // broke out, our source has been marked invalid by another thread
		return -1U;
	unsigned int curt;
	if(payload!=NULL) {
		if(verbose>=1 && sn!=sourceSN+1 && sourceSN!=0)
			std::cout << "Data Source dropped " << (sn-sourceSN-1) << " frames"  << std::endl;
		sourceSN=sn;
		// if we have a payload and timestamp is still in the future, wait for it
		{
			MarkScope l(lock); // don't hold lock throughout sleep, just on updating runloopState
			runloopState=GOT_FRAME;
		}
		while(timestamp>(curt=get_time()) && runloopState!=ADVANCE_FRAME) {
			//cout << "size " << sz << " timestamp " << timestamp << " vs " << curt << " name " << name << endl;
			int res=usleep(static_cast<unsigned int>((timestamp-curt)*1000/getTimeScale()));
			if(res!=0) {
				if(errno==EINTR && runloopState==ADVANCE_FRAME) {
					break;
				} else {
					perror("LoadDataThread::runloop() nanosleep");
					break;
				}
			}
		}
	}
	{
		MarkScope l(lock);
		curt=get_time(); //recheck time, lock might've taken a while
		if(payload==NULL) {
			if(heartbeat && curt>=nextT)
				sendHeartbeat(curt);
			runloopState=INTERFRAME;
			return calcSleepTime();
		}
		if(verbose>=2)
			std::cout << "Sending frame: \"" << name << "\" (req. " << static_cast<unsigned int>(nextT+.5f) << ", sched. " << timestamp << ", now " << curt << ')' << std::endl;
		RCRegion * r=firstUnusedRegion();
		char * buf=setupRegion(r,name,sz,curDS->nextTimestamp()!=-1U);
		ASSERTRETVAL(buf!=NULL,"setupRegion returned null region",false);
		memcpy(buf,payload,sz);
		//if(sz < 1024)
		//	cout << "size: " << sz << " payload:" << payload << endl;
		msgr.sendMessage(r);
		regions.push_back(r);
		if(curt - nextT > 1000.f/framerate*5)
			lastSent=curt; // apparently can't keep up, don't let requested time fall ridiculously behind...
		else
			lastSent=nextT;
		runloopState=INTERFRAME;
		return 0;
	}
}


/*! @file
 * @brief 
 * @author Ethan Tira-Thompson (ejt) (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.18 $
 * $State: Exp $
 * $Date: 2007/10/12 16:55:04 $
 */
