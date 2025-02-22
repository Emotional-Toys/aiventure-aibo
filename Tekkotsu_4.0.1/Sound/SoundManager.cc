#include "Shared/Config.h"
#include "SoundManager.h"
#include "Shared/MarkScope.h"
#include "WAV.h"
#include "Events/EventRouter.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fstream>
#ifdef PLATFORM_APERIOS
#  include <OPENR/OSubject.h>
#  include <OPENR/ObjcommEvent.h>
#endif

using namespace std;

SoundManager * sndman=NULL;

//!for convenience when locking each of the functions
typedef MarkScope AutoLock;

SoundManager::SoundManager()
: mixerBuffer(0), mixerBufferSize(0), sndlist(),playlist(),chanlist(),mix_mode(Fast),queue_mode(Override),max_chan(4),lock(),sn(0)
{}

#ifdef PLATFORM_APERIOS
void
SoundManager::InitAccess(OSubject* subj) {
	subjs[ProcessID::getID()]=subj;
}
#else //PLATFORM_LOCAL
void
SoundManager::InitAccess(MessageQueueBase& sndbufq) {
	subjs[ProcessID::getID()]=&sndbufq;
}
#endif //PLATFORM-specific initialization

SoundManager::~SoundManager() {
	stopPlay();
	if(!sndlist.empty())
		cerr << "Warning: SoundManager was deleted with active sound buffer references" << endl;
	while(!sndlist.empty()) {
		sndlist_t::index_t it=sndlist.begin();
		if(sndlist[it].rcr==NULL)
			cerr << sndlist[it].name << " was still inflight (IPC), with " << sndlist[it].ref << " sound references" << endl;
		else {
			cerr << sndlist[it].name << " was deleted, with " << sndlist[it].ref << " sound references and " << sndlist[it].rcr->NumberOfReference() << " region references (one will be removed)" << endl;
			sndlist[it].rcr->RemoveReference();
		}
		sndlist.erase(it);
	}
	delete[] mixerBuffer;
}

//!@todo this does one more copy than it really needs to
SoundManager::Snd_ID
SoundManager::loadFile(std::string const &name) {
	AutoLock autolock(lock);
	if (name.size() == 0) {
		cout << "SoundManager::loadFile() null filename" << endl;
		return invalid_Snd_ID;
	};
	std::string path(config->sound.makePath(name));
	Snd_ID id=lookupPath(path);
	if(id!=invalid_Snd_ID) {
		//		cout << "add reference to pre-existing" << endl;
		sndlist[id].ref++;
	} else {
		//		cout << "load new file" << endl;
		struct stat buf;
		if(stat(path.c_str(),&buf)==-1) {
			cout << "SoundManager::loadFile(): Sound file not found: " << path << endl;
			return invalid_Snd_ID;
		}
		byte * sndbuf=new byte[buf.st_size];
		std::ifstream file(path.c_str());
		file.read(reinterpret_cast<char*>(sndbuf),buf.st_size);
		WAV wav;
		WAVError error = wav.Set(sndbuf);
		if (error != WAV_SUCCESS) {
			printf("%s : %s %d: '%s'","SoundManager::loadFile()","wav.Set() FAILED",error, path.c_str());
			return invalid_Snd_ID;
		}
		if(wav.GetSamplingRate()!=config->sound.sample_rate || wav.GetBitsPerSample()!=config->sound.sample_bits) {
			printf("%s : %s %d","SoundManager::loadFile()","bad sample rate/bits", error);
			return invalid_Snd_ID;
		}
		cout << "Loading " << name << endl;
		id=loadBuffer(reinterpret_cast<char*>(wav.GetDataStart()),wav.GetDataEnd()-wav.GetDataStart());
		delete [] sndbuf;
		if(path.size()>MAX_NAME_LEN)
			strncpy(sndlist[id].name,path.substr(path.size()-MAX_NAME_LEN).c_str(),MAX_NAME_LEN);
		else
			strncpy(sndlist[id].name,path.c_str(),MAX_NAME_LEN);
	}
	return id;
}

SoundManager::Snd_ID
SoundManager::loadBuffer(const char buf[], unsigned int len) {
  // cout << "SoundManager::loadBuffer() of " << len << " bytes" << endl;
	if(buf==NULL || len==0)
		return invalid_Snd_ID;
	AutoLock autolock(lock);
	//setup region
	RCRegion * region=initRegion(len+MSG_SIZE);
	//setup message
	SoundManagerMsg * msg=new (reinterpret_cast<SoundManagerMsg*>(region->Base())) SoundManagerMsg;
	Snd_ID msgid=sndlist.new_front();
	msg->setAdd(msgid,sn);
	//init sound structure
	sndlist[msg->getID()].rcr=NULL;  // set by SoundPlay upon reception
	sndlist[msg->getID()].data=NULL; // set by SoundPlay upon reception
	sndlist[msg->getID()].ref=1;
	sndlist[msg->getID()].len=len;
	sndlist[msg->getID()].sn=sn;
	//copy buffer, do any filtering needed here
	const byte* src=reinterpret_cast<const byte*>(buf);
	byte* dest=reinterpret_cast<byte*>(region->Base())+MSG_SIZE;
	byte* end=dest+len;
	if (config->sound.sample_bits==8)
		while (dest < end)
			*dest++ = *src++ ^ 0x80; // offset binary -> signed char 
	else
		while (dest < end)
			*dest++ = *src++;
	//cout << "SoundManager init region " << region->ID().key << endl;
	//send message
	if(ProcessID::getID()==ProcessID::SoundProcess) {
		//if SoundPlay is preloading files, don't need to do inter-object comm
		sndlist[msg->getID()].rcr=region;
		sndlist[msg->getID()].data=reinterpret_cast<byte*>(region->Base()+MSG_SIZE);		
	} else {
#ifdef PLATFORM_APERIOS
		//cout << "Send new at " << get_time() << '-';
		subjs[ProcessID::getID()]->SetData(region);
		subjs[ProcessID::getID()]->NotifyObservers();
		//cout << get_time() << endl;
#else
		subjs[ProcessID::getID()]->sendMessage(region);
		region->RemoveReference();
#endif
	}
	return msgid;
}
	
void
SoundManager::releaseFile(std::string const &name) {
	AutoLock autolock(lock);
	release(lookupPath(config->sound.makePath(name)));
}

void
SoundManager::release(Snd_ID id) {
	if(id==invalid_Snd_ID)
		return;
	if(sndlist[id].ref==0) {
		cerr << "SoundManager::release() " << id << " extra release" << endl;
		return;
	}
	AutoLock autolock(lock);
	sndlist[id].ref--;
	if(sndlist[id].ref==0) {
		if(sndlist[id].rcr!=NULL) {
			//The sound buffer is attached in the sound process, we need to detach it.
			//Note that if this was NULL, we just assume that's because the message for the
			//region is still in transit from the originating process, and erase the sndlist entry.
			//Then when that buffer does arrive, we'll discover the sndlist entry is invalid and ignore it
			if(ProcessID::getID()==ProcessID::SoundProcess) {
				//we're currently running in sound process -- don't need to send ourselves an IPC message
				sndlist[id].rcr->RemoveReference();
				sndlist[id].rcr=NULL;
			} else {
				//we're currently running in a foreign process -- have to send message to sound process to release
				//setup region
				RCRegion * region=initRegion(MSG_SIZE);
				//setup message
				SoundManagerMsg * msg=new (reinterpret_cast<SoundManagerMsg*>(region->Base())) SoundManagerMsg;
				msg->setDelete(sndlist[id].rcr);
				//cout << "Sending delete msg for " << sndlist[id].name << endl;
				//send message
#ifdef PLATFORM_APERIOS
				//cout << "Send delete at " << get_time() << '-';
				subjs[ProcessID::getID()]->SetData(region);
				subjs[ProcessID::getID()]->NotifyObservers();
				//cout << get_time() << endl;
#else
				subjs[ProcessID::getID()]->sendMessage(region);
				region->RemoveReference();
#endif
			}
		}
		//clean up sound data structure
		sndlist[id].sn=0; // we use '1' for the first issued, so 0 marks it as invalid
		sndlist.erase(id);
	}
}

SoundManager::Play_ID
SoundManager::playFile(std::string const &name) {
  if(playlist.size()>=playlist_t::MAX_ENTRIES)
		return invalid_Play_ID;	
	AutoLock autolock(lock);
	Snd_ID sndid=loadFile(name);
	if(sndid==invalid_Snd_ID)
		return invalid_Play_ID;
	sndlist[sndid].ref--;
	cout << "Playing " << name /*<< " from process " << ProcessID::getID()*/ << endl;
	return play(sndid);
}

SoundManager::Play_ID
SoundManager::playBuffer(const char buf[], unsigned int len) {
	if(playlist.size()>=playlist_t::MAX_ENTRIES || buf==NULL || len==0)
		return invalid_Play_ID;	
	AutoLock autolock(lock);
	Snd_ID sndid=loadBuffer(buf,len);
	if(sndid==invalid_Snd_ID)
		return invalid_Play_ID;
	sndlist[sndid].ref--;
	return play(sndid);
}
	
SoundManager::Play_ID
SoundManager::play(Snd_ID id) {
        // cout << "Play " << id << endl;
	if(id==invalid_Snd_ID)
		return invalid_Play_ID;
	AutoLock autolock(lock);
	Play_ID playid=playlist.new_front();
	if(playid!=invalid_Play_ID) {
		sndlist[id].ref++;
		playlist[playid].snd_id=id;
		playlist[playid].offset=0;
		//playlist.size() should be greater than or equal to chanlist.size
		//so if we got a playid, we can get a channel slot.
		chanlist.push_front(playid);

		//setup message to "wake-up" 
		//(only really need if chanlist was empty)
		//		if(chanlist.size()==1) { //commented out because sometimes doesn't wake up, thinks it's playing but isn't
		if(ProcessID::getID()!=ProcessID::SoundProcess) {
			RCRegion * region=initRegion(MSG_SIZE);
			ASSERT(region!=NULL,"initRegion returned NULL");
			SoundManagerMsg * msg=new (reinterpret_cast<SoundManagerMsg*>(region->Base())) SoundManagerMsg;
			msg->setWakeup();
#ifdef PLATFORM_APERIOS
			//cout << "Send wakeup at " << get_time() << '-';
			subjs[ProcessID::getID()]->SetData(region);
			subjs[ProcessID::getID()]->NotifyObservers();
			//cout << get_time() << endl;
#else
			subjs[ProcessID::getID()]->sendMessage(region);
			region->RemoveReference();
#endif
		}
		//		}
		
		if(sndlist[id].rcr!=NULL) {
			const char * name=sndlist[playlist[playid].snd_id].name;
			if(name[0]!='\0')
				erouter->postEvent(EventBase::audioEGID,playid,EventBase::activateETID,0,name,1);
			else
				erouter->postEvent(EventBase::audioEGID,playid,EventBase::activateETID,0);
		}
	}
	return playid;
}
	
SoundManager::Play_ID
SoundManager::chainFile(Play_ID base, std::string const &next) {
       if(base==invalid_Play_ID)
		return playFile(next);
	Play_ID orig=base;
	while(playlist[base].next_id!=invalid_Play_ID)
		base=playlist[base].next_id;
	Play_ID nplay=playlist.new_front();
	if(nplay==invalid_Play_ID)
		return nplay;
	Snd_ID nsnd=loadFile(next);
	if(nsnd==invalid_Snd_ID) {
		playlist.pop_front();
		return invalid_Play_ID;
	}
	playlist[nplay].snd_id=nsnd;
	playlist[base].next_id=nplay;
	return orig;
}

SoundManager::Play_ID
SoundManager::chainBuffer(Play_ID base, const char buf[], unsigned int len) {
	if(base==invalid_Play_ID || buf==NULL || len==0)
		return playBuffer(buf,len);
	Play_ID orig=base;
	while(playlist[base].next_id!=invalid_Play_ID)
		base=playlist[base].next_id;
	Play_ID nplay=playlist.new_front();
	if(nplay==invalid_Play_ID)
		return nplay;
	Snd_ID nsnd=loadBuffer(buf,len);
	if(nsnd==invalid_Snd_ID) {
		playlist.pop_front();
		return invalid_Play_ID;
	}
	playlist[nplay].snd_id=nsnd;
	playlist[base].next_id=nplay;
	return orig;
}

SoundManager::Play_ID
SoundManager::chain(Play_ID base, Snd_ID next) {
	if(base==invalid_Play_ID || next==invalid_Snd_ID)
		return play(next);
	Play_ID orig=base;
	while(playlist[base].next_id!=invalid_Play_ID)
		base=playlist[base].next_id;
	Play_ID nplay=playlist.new_front();
	if(nplay==invalid_Play_ID)
		return nplay;
	playlist[nplay].snd_id=next;
	playlist[base].next_id=nplay;
	return orig;
}

void
SoundManager::stopPlay() {
	while(!playlist.empty())
		stopPlay(playlist.begin());
}

void
SoundManager::stopPlay(Play_ID id) {
	if(id==invalid_Play_ID)
		return;
	AutoLock autolock(lock);
	//cout << "Stopping sound " << id << ": " << sndlist[playlist[id].snd_id].name << endl;
	//we start at the back (oldest) since these are the most likely to be removed...
	for(chanlist_t::index_t it=chanlist.prev(chanlist.end()); it!=chanlist.end(); it=chanlist.prev(it))
		if(chanlist[it]==id) {
			std::string name=sndlist[playlist[id].snd_id].name;
			release(playlist[id].snd_id);
			playlist.erase(id);
			chanlist.erase(it);
			playlist[id].cumulative+=playlist[id].offset;
			unsigned int ms=playlist[id].cumulative/(config->sound.sample_bits/8)/(config->sound.sample_rate/1000);
			if(name.size()>0)
				erouter->postEvent(EventBase::audioEGID,id,EventBase::deactivateETID,ms,name,0);
			else
				erouter->postEvent(EventBase::audioEGID,id,EventBase::deactivateETID,ms);
			return;
		}
	cerr << "SoundManager::stopPlay(): " << id << " does not seem to be playing" << endl;
}

void
SoundManager::pausePlay(Play_ID id) {
	if(id==invalid_Play_ID)
		return;
	AutoLock autolock(lock);
	for(chanlist_t::index_t it=chanlist.begin(); it!=chanlist.end(); it=chanlist.next(it))
		if(chanlist[it]==id) {
			chanlist.erase(it);
			return;
		}
}
	
void
SoundManager::resumePlay(Play_ID id) {
	if(id==invalid_Play_ID)
		return;
	AutoLock autolock(lock);
	for(chanlist_t::index_t it=chanlist.begin(); it!=chanlist.end(); it=chanlist.next(it))
		if(chanlist[it]==id)
			return;
	chanlist.push_front(id);
}
	
void
SoundManager::setMode(unsigned int max_channels, MixMode_t mixer_mode, QueueMode_t queuing_mode) {
	AutoLock autolock(lock);
	max_chan=max_channels;
	mix_mode=mixer_mode;
	queue_mode=queuing_mode;
}

unsigned int
SoundManager::getRemainTime(Play_ID id) const {
	AutoLock autolock(lock);
	unsigned int t=0;
	while(id!=invalid_Play_ID) {
		t+=sndlist[playlist[id].snd_id].len-playlist[id].offset;
		id=playlist[id].next_id;
	}
	const unsigned int bytesPerMS=config->sound.sample_bits/8*config->sound.sample_rate/1000;
	return t/bytesPerMS;
}

void
SoundManager::mixChannel(Play_ID channelId, void* buf, size_t destSize) {
	char *dest = (char*) buf;
	
	PlayState& channel = playlist[channelId];
	while (destSize > 0) {
		const SoundData& buffer = sndlist[channel.snd_id];
		const char* samples = ((char*) (buffer.data)) + channel.offset;
		const unsigned int samplesSize = buffer.len - channel.offset;
		if (samplesSize > destSize) {
			memcpy(dest, samples, destSize);
			channel.offset += destSize;
			dest += destSize;
			destSize = 0;
			return;
		} else {
			memcpy(dest, samples, samplesSize);
			channel.offset += samplesSize;
			dest += samplesSize;
			destSize -= samplesSize;
			if (endPlay(channelId)) {
				break;
			}
		}
	}
	if (destSize > 0) {
		memset(dest, 0, destSize);
	}
}

void
SoundManager::mixChannelAdditively(Play_ID channelId, int bitsPerSample, MixMode_t mode,
                                   short scalingFactor, void* buf, size_t destSize)
{
	PlayState& channel = playlist[channelId];
	while (destSize > 0) {
		const SoundData& buffer = sndlist[channel.snd_id];
		const unsigned int samplesSize = buffer.len - channel.offset;
		const unsigned int mixedSamplesSize =
			((mode == Fast)
				? ((samplesSize > destSize) ? destSize : samplesSize)
				: ((samplesSize > destSize / 2) ? destSize / 2 : samplesSize)); 
		
		if (bitsPerSample == 8) {
			//  8-bit mode
			const char* samples = (char*) (buffer.data + channel.offset);
			if (mode == Fast) {
				// 8-bit mixing
				char *dest = (char*) buf;
				for (size_t i = 0; i < mixedSamplesSize; i++) {
					*dest += samples[i] / scalingFactor;
					dest++;
				}
				destSize -= (char*) dest - (char*) buf;
				buf = dest;
			} else {
				// 16-bit mixing
				short* dest = (short*) buf;
				for (size_t i = 0; i < mixedSamplesSize; i++) {
					*dest += samples[i];
					*dest++;
				}
				destSize -= (char*) dest - (char*) buf;
				buf = dest;
			}
		} else {
			// 16-bit mode
			const short* samples = (short*) (buffer.data + channel.offset);
			if (mode == Fast) {
				// 16-bit mixing
				short* dest = (short*) buf;
				for (size_t i = 0; i < mixedSamplesSize / 2; i++) {
					*dest += samples[i] / scalingFactor;
					*dest++;
				}
				destSize -= (char*) dest - (char*) buf;
				buf = dest;
			} else {
				// 32-bit mixing
				int* dest = (int*) buf;
				for (size_t i = 0; i < mixedSamplesSize / 2; i++) {
					*dest += samples[i];
					*dest++;
				}
				destSize -= (char*) dest - (char*) buf;
				buf = dest;
			}
		}
		channel.offset += mixedSamplesSize;
		if (destSize == 0) {
			return;
		} else {
			if (endPlay(channelId)) {
				return;
			}
		}
	}
}

#ifdef PLATFORM_APERIOS
unsigned int
SoundManager::CopyTo(OSoundVectorData* data) {
	AutoLock autolock(lock);
	return CopyTo(data->GetData(0), data->GetInfo(0)->dataSize);
}

void
SoundManager::ReceivedMsg(const ONotifyEvent& event) {
	//cout << "Got msg at " << get_time() << endl;
	for(int x=0; x<event.NumOfData(); x++)
		ProcessMsg(event.RCData(x));
}
#endif

unsigned int SoundManager::CopyTo(void * dest, size_t destSize) {
	AutoLock autolock(lock);

	void * origdest=dest;
	size_t origDestSize=destSize;
	if(chanlist.size() == 0) {
		memset(dest, 0, destSize);
		return 0;
	}
	
	std::vector<Play_ID> channels;
	selectChannels(channels);
	
	if (channels.size() == 0) {
		// No channels to mix
		memset(dest, 0, destSize); 
	} else if (channels.size() == 1) {
		// One channel to mix
		mixChannel(channels.front(), dest, destSize);
	} else {
		// Several channels to mix	
		const MixMode_t mode = mix_mode;
		const int bitsPerSample = config->sound.sample_bits;
		if (mode == Quality) {
			// Quality mixing uses an intermediate buffer
			if ((mixerBuffer == 0) || (mixerBufferSize < destSize * 2)) {
				delete[] mixerBuffer;
				mixerBuffer = 0;
				mixerBufferSize = destSize * 2;
				mixerBuffer = new int[(mixerBufferSize / 4) + 1]; // makes sure it's int-aligned
			}
			memset(mixerBuffer, 0,  mixerBufferSize);
			dest = mixerBuffer;
			destSize *= 2;
		} else {
			// Fast mixing does not use the intermeridate buffer
			memset(dest, 0, destSize);
		}
		
		const int channelCount = channels.size();
		const short scalingFactor = (short) ((mode == Fast) ? channelCount : 1);  
		for(std::vector<Play_ID>::iterator i = channels.begin(); i != channels.end(); i++)
			mixChannelAdditively(*i, bitsPerSample, mode, scalingFactor, dest, destSize);
		
		if (mode == Quality) {
			// Quality mixing uses an intermediate buffer
			// Scale the buffer
			destSize /= 2;
			if (bitsPerSample == 8) {
				//  8-bit mode
				char* destChar = (char*) origdest;
				short* mixerBufferShort = (short*) mixerBuffer;
				for (size_t i = 0; i < destSize; i++) {
					destChar[i] = (char) (mixerBufferShort[i] / channelCount);
				} 
			} else {
				// 16-bit mode
				short* destShort = (short*) origdest;
				const size_t destSampleCount = destSize / 2; 
				for (size_t i = 0; i < destSampleCount; i++) {
					destShort[i] = (short) (mixerBuffer[i] / channelCount);
				}
			}
		}
	}
	
	updateChannels(channels, origDestSize);
	return channels.size(); 
}

void SoundManager::ProcessMsg(RCRegion * rcr) {
	SoundManagerMsg * msg = reinterpret_cast<SoundManagerMsg*>(rcr->Base());
	//cout << "Processing " << msg << ": " << rcr->ID().key << endl;
	switch(msg->type) {
		case SoundManagerMsg::add: {
			//cout << "it's an add of " << msg->id << ", sn=" << msg->sn << " (expecting " << sndlist[msg->id].sn << ", next sn is " << sn << ")" << endl;
			//first check msg->id's validity, in case it was deleted while in-flight
			/* //since Release marks deleted entries with serial number 0, we can get around this
			bool valid=false;
			for(Snd_ID it=sndlist.begin(); it!=sndlist.end(); it=sndlist.next(it)) {
				if(it==msg->id) {
					valid=true;
					break;
				}
			}
			if(!valid) {
				//was deleted while buffer was still in-flight, ignore this message
				break; //leaves switch statement, try next message if any
			}
			//now, even though the sound id is valid, verify the serial numbers match
			*/
			if(sndlist[msg->id].sn!=msg->sn) {
				/*this means that the sound this message references was deleted while
				* in-flight, even if some crazy process (perhaps the same one) has
				* created and destroyed a number of sounds before we got this message */
				/* So although there is a sound for this id, it's a different sound than
				* the one this message refers to, and we should ignore this message */
				cerr << "Warning: serial numbers don't match... may be pathological sound usage (many load/releases very quickly)" << endl;
				break; //leaves switch statement, try next message if any
			}
			ASSERT(sndlist[msg->id].rcr==NULL,"The sndlist entry for an add message already has an attached region, I'm going to leak the old region")
			rcr->AddReference();
			sndlist[msg->id].rcr=rcr;
			sndlist[msg->id].data=reinterpret_cast<byte*>(rcr->Base()+MSG_SIZE);
			//look to see if there's any play's for the sound we just finished loading
			for(playlist_t::index_t it=playlist.begin();it!=playlist.end();it=playlist.next(it))
				if(playlist[it].snd_id==msg->id) {
					//send an event if there are
					const char * name=sndlist[playlist[it].snd_id].name;
					if(name[0]!='\0')
						erouter->postEvent(EventBase::audioEGID,it,EventBase::activateETID,0,name,1);
					else
						erouter->postEvent(EventBase::audioEGID,it,EventBase::activateETID,0);
				}
		} break;
			case SoundManagerMsg::del: {
				//cout << "it's an del" << endl;
				if(msg->region==NULL) {
					cerr << "SoundManager received a delete message for a NULL region" << endl;
				} else {
					msg->region->RemoveReference();
				}
			} break;
			case SoundManagerMsg::wakeup: {
				//cout << "it's a wakeup" << endl;
				//doesn't need to do anything, just causes SoundPlay process to check activity
			} break;
			default:
				printf("*** WARNING *** unknown SoundManager msg type received\n");
	}
}


//protected:

RCRegion*
SoundManager::initRegion(unsigned int size) {
	sn++; //so the first serial number handed out will be '1', leaving '0' as 'invalid'
#ifdef PLATFORM_APERIOS
	unsigned int pagesize=4096;
	sError err=GetPageSize(&pagesize);
	if(err!=sSUCCESS)
		cerr << "Error "<<err<<" getting page size " << pagesize << endl;
	unsigned int pages=(size+pagesize-1)/pagesize;
	return new RCRegion(pages*pagesize);
#else
	char name[RCRegion::MAX_NAME_LEN];
	snprintf(name,RCRegion::MAX_NAME_LEN,"SndMsg.%d.%d",ProcessID::getID(),sn);
	name[RCRegion::MAX_NAME_LEN-1]='\0';
	//cout << "Created " << name << endl;
	return new RCRegion(name,size);
#endif
}

SoundManager::Snd_ID 
SoundManager::lookupPath(std::string const &path) const {
	std::string clippedPath;
	const char* cpath=NULL;
	if(path.size()>MAX_NAME_LEN) {  //compare against the end of the string if it's too long -- the end is more likely to be unique
		clippedPath=path.substr(path.size()-MAX_NAME_LEN);
		cpath=clippedPath.c_str();
	} else
		cpath=path.c_str();
	for(sndlist_t::index_t it=sndlist.begin(); it!=sndlist.end(); it=sndlist.next(it)) {
		if(strncasecmp(cpath,sndlist[it].name,MAX_NAME_LEN)==0)
			return it;
	}
	return invalid_Snd_ID;
}

void
SoundManager::selectChannels(std::vector<Play_ID>& mix) {
	unsigned int selected=0;
	switch(queue_mode) {
	case Enqueue: { //select the oldest channels
		for(chanlist_t::index_t it=chanlist.prev(chanlist.end());it!=chanlist.end();it=chanlist.prev(it)) {
			if(sndlist[playlist[chanlist[it]].snd_id].data!=NULL) {
				mix.push_back(chanlist[it]);
				selected++;
				if(selected==max_chan)
					return;
			}
		}
	} break;
	case Override:
	case Pause: { //select the youngest channels (difference between these two is in the final update)
		for(chanlist_t::index_t it=chanlist.begin(); it!=chanlist.end(); it=chanlist.next(it)) {
			if(sndlist[playlist[chanlist[it]].snd_id].data!=NULL) {
				mix.push_back(chanlist[it]);
				selected++;
				if(selected==max_chan)
					return;
			}
		}
	} break;
	case Stop: { //select the youngest, stop anything that remains
		unsigned int numkeep=0;
		chanlist_t::index_t it=chanlist.begin();
		for(;it!=chanlist.end(); it=chanlist.next(it), numkeep++) {
			if(sndlist[playlist[chanlist[it]].snd_id].data!=NULL) {
				mix.push_back(chanlist[it]);
				selected++;
				if(selected==max_chan) {
					for(unsigned int i=chanlist.size()-numkeep-1; i>0; i--)
						endPlay(chanlist.back());
					return;
				}
			}
		}
	} break;
	default:
		cout << "SoundManager::selectChannels(): Illegal queue mode" << endl;
	}
}

void
SoundManager::updateChannels(const std::vector<Play_ID>& mixs,size_t used) {
	switch(queue_mode) {
	case Enqueue:
	case Pause:
	case Stop: 
		break;
	case Override: { //increase offset of everything that wasn't selected
		//assumes mode hasn't changed since the mix list was created... (so order is same as chanlist)
		chanlist_t::index_t it=chanlist.begin(); 
		std::vector<Play_ID>::const_iterator mixit=mixs.begin();
		for(;it!=chanlist.end(); it=chanlist.next(it)) {
			for(;mixit!=mixs.end(); mixit++) //some mixs may have been stopped during play
				if(*mixit==chanlist[it])
					break;
			if(mixit==mixs.end())
				break;
		}
		for(;it!=chanlist.end(); it=chanlist.next(it)) {
			const Play_ID channelId = chanlist[it];
			PlayState &channel = playlist[channelId];
			size_t skip = used;
			while (skip > 0) {
				SoundData &buffer = sndlist[channel.snd_id];
				// FIXME: Don't know why the buffer.data != 0 check is done 
				if (buffer.data != 0) {
					size_t remain = buffer.len - channel.offset;
					if (remain < skip) {
						channel.offset = buffer.len;
						skip -= buffer.len;
						if (endPlay(channelId)) {
							break;
						}
					} else {
						channel.offset += skip;
						skip = 0;
					}
				} else {
					break;
				}
			}
		}
	} break;
	default:
		cout << "SoundManager::updateChannels(): Illegal queue mode" << endl;
	}
}

bool
SoundManager::endPlay(Play_ID id) {
	if(playlist[id].next_id==invalid_Play_ID) {
		stopPlay(id);
		return true;
	} else {
		//copies the next one into current so that the Play_ID consistently refers to the same "sound"
		Play_ID next=playlist[id].next_id;
		//		cout << "play " << id << " moving from " << playlist[id].snd_id << " to " << playlist[next].snd_id << endl;
		release(playlist[id].snd_id);
		playlist[id].snd_id=playlist[next].snd_id;
		playlist[id].cumulative+=playlist[id].offset;
		playlist[id].offset=0;
		playlist[id].next_id=playlist[next].next_id;
		playlist.erase(next);
		unsigned int ms=playlist[id].cumulative/(config->sound.sample_bits/8)/(config->sound.sample_rate/1000);
		const char * name=sndlist[playlist[id].snd_id].name;
		if(name[0]!='\0')
			erouter->postEvent(EventBase::audioEGID,id,EventBase::statusETID,ms,name,1);
		else
			erouter->postEvent(EventBase::audioEGID,id,EventBase::statusETID,ms);
		return false;
	}
}

SoundManager::SoundData::SoundData()
	: rcr(NULL), data(NULL), len(0), ref(0), sn(0)
{
	name[0]='\0';
}

SoundManager::PlayState::PlayState()
	: snd_id(invalid_Snd_ID), offset(0), cumulative(0), next_id(invalid_Play_ID)
{}


SoundManager::Snd_ID SoundManager::LoadFile(std::string const &name) { return loadFile(name); }
SoundManager::Snd_ID SoundManager::LoadBuffer(const char buf[], unsigned int len) { return loadBuffer(buf,len); }
void SoundManager::ReleaseFile(std::string const &name) { releaseFile(name); }
void SoundManager::Release(Snd_ID id) { release(id); }
SoundManager::Play_ID SoundManager::PlayFile(std::string const &name) { return playFile(name); }
SoundManager::Play_ID SoundManager::PlayBuffer(const char buf[], unsigned int len) { return playBuffer(buf,len); }
SoundManager::Play_ID SoundManager::Play(Snd_ID id) { return play(id); }
SoundManager::Play_ID SoundManager::ChainFile(Play_ID base, std::string const &next) { return chainFile(base,next); }
SoundManager::Play_ID SoundManager::ChainBuffer(Play_ID base, const char buf[], unsigned int len) { return chainBuffer(base,buf,len); }
SoundManager::Play_ID SoundManager::Chain(Play_ID base, Snd_ID next) { return chain(base,next); }
void SoundManager::StopPlay() { stopPlay(); }
void SoundManager::StopPlay(Play_ID id) { stopPlay(id); }
void SoundManager::PausePlay(Play_ID id) { pausePlay(id); }
void SoundManager::ResumePlay(Play_ID id) { resumePlay(id); }
void SoundManager::SetMode(unsigned int max_channels, MixMode_t mixer_mode, QueueMode_t queuing_mode) { setMode(max_channels,mixer_mode,queuing_mode); }
unsigned int SoundManager::GetRemainTime(Play_ID id) const { return getRemainTime(id); }
unsigned int SoundManager::GetNumPlaying() { return getNumPlaying(); }


/*! @file
 * @brief Implements SoundManager, which provides sound effects and caching services, as well as mixing buffers for the SoundPlay process
 * @author ejt (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.10 $
 * $State: Exp $
 * $Date: 2006/09/18 18:08:08 $
 */




//This is a faster mix algo using bit shifting, but it doesn't work with
// non power of two number of channels, despite my efforts... eh, maybe
// i'll fix it later...
		// NOT WORKING
		/*
		if(mixs.size()==2 || mix_mode==Fast) {
			unsigned int shift=0;
			unsigned int offset=0;
			unsigned int tmp=mixs.size();
			while(tmp>1) {
				tmp>>=1;
				shift++;
			}
			unsigned int mask;
			if(config->sound.sample_bits==8) {
				unsigned int c=(unsigned char)~0;
				c>>=shift;
				mask=(c<<24)|(c<<16)|(c<<8)|c;
				offset=(1<<7)|(1<<15)|(1<<23)|(1<<31);
			} else {
				unsigned int c=(unsigned short)~0;
				c>>=shift;
				mask=(c<<16)|c;
				offset=(1<<31)|(1<<15);
			}
			memset(dest,0,avail);
			for(unsigned int c=0; c<mixs.size(); c++) {
				if(ends[c]-srcs[c]>avail) {
					for(unsigned int * beg=(unsigned int*)dest;beg<(unsigned int*)end;beg++) {
						const unsigned int src=*(unsigned int*)srcs[c];
						if(beg==(unsigned int*)dest) {
							cout << src <<' '<< (void*)src << endl;
							unsigned int x=((src^offset)>>shift)&mask;
							cout << x <<' '<< (void*)x << endl;
							cout << "****" << endl;
						}
						*beg+=((src^offset)>>shift)&mask;
						if(beg==(unsigned int*)dest)
							cout << *beg <<' '<< (void*)*beg << endl << "########" << endl;
						srcs[c]+=sizeof(int);
					}
					playlist[mixs[c]].offset+=avail;
				} else {
					unsigned int * beg=(unsigned int*)dest;
					for(;srcs[c]<ends[c];srcs[c]+=sizeof(int)) {
						const unsigned int src=*(unsigned int*)srcs[c];
						*beg+=((src^offset)>>shift)&mask;
						beg++;
					}
					for(;beg<(unsigned int*)end; beg++)
						*beg+=offset>>shift;
					playlist[mixs[c]].offset=sndlist[playlist[mixs[c]].snd_id].len;
					stopPlay(mixs[c]);
				}
			}
			unsigned int leftover=(offset>>shift)*((1<<shift)-mixs.size());
			for(unsigned int * beg=(unsigned int*)dest;beg<(unsigned int*)end;beg++)
				*beg=*(beg+leftover)^offset;
			updateChannels(avail);
			return mixs.size();
			} else*/
