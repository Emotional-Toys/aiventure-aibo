#include "Dynamixel.h"
#include "Shared/get_time.h"
#include "Shared/MarkScope.h"
#include "Shared/debuget.h"

using namespace std; 

const char * DynamixelDriver::protocolNames[] = { "BINARY", "CM-5", "" };
INSTANTIATE_NAMEDENUMERATION_STATICS(DynamixelDriver::protocol_t);

const std::string DynamixelDriver::autoRegisterDynamixelDriver = DeviceDriver::getRegistry().registerType<DynamixelDriver>("Dynamixel");

static char nchecksum(const char* p, size_t len, size_t off=0) {
	const char* end=p+len;
	p+=off;
	char c=0;
	while(p!=end) {
		//debuget::charhexout(*p); cout << ' '; debuget::charhexout(c+*p); cout << endl;
		c+=*p++;
	}
	return c;
}

struct GenericCmdHeader {
	GenericCmdHeader(unsigned char bytelen, unsigned char instruction)
	: markerA(0xFF), markerB(0xFF), servoid(0xFE), cmdlen(bytelen),  cmdid(instruction) {}
	operator const char*() const { return reinterpret_cast<const char*>(&markerA); }
	unsigned char markerA;
	unsigned char markerB;
	unsigned char servoid;
	unsigned char cmdlen;
	unsigned char cmdid;
};
static char nchecksum(const struct GenericCmdHeader& p, size_t len) { return nchecksum(p,len,2); }

template<class T>
struct SyncWriteHeader : public GenericCmdHeader {
	SyncWriteHeader(unsigned char len)
	: GenericCmdHeader(sizeof(T)*len+4,0x83), addr(T::ADDRESS), writelen(sizeof(T)-1) {}
	unsigned char addr;
	unsigned char writelen;
};
struct WriteHeader : public GenericCmdHeader {
	WriteHeader(unsigned char address, unsigned char len)
	: GenericCmdHeader(len+3,0x3), addr(address) {}
	unsigned char addr;
};
struct SyncWritePosSpeedEntry {
	SyncWritePosSpeedEntry() : servoid(), posl(), posh(), speedl(), speedh() {}
	SyncWritePosSpeedEntry(unsigned char sid, unsigned short pos, unsigned short speed)
	: servoid(sid), posl(pos), posh(pos>>8), speedl(speed), speedh(speed>>8) {}
	static const unsigned char ADDRESS=0x1E;
	operator const char*() const { return reinterpret_cast<const char*>(&servoid); }
	unsigned char servoid;
	unsigned char posl;
	unsigned char posh;
	unsigned char speedl;
	unsigned char speedh;
};
struct BroadcastTorqueCmd : public WriteHeader {
	BroadcastTorqueCmd(bool enable)
	: WriteHeader(0x18,1), torqueEnable(enable?1:0), checksum(0) { updateChecksum(); }
	void updateChecksum() { checksum=~nchecksum(*this,sizeof(*this)-1); }
	unsigned char torqueEnable;
	char checksum;
};

void DynamixelDriver::motionStarting() {
	MotionHook::motionStarting();
	CommPort * comm = CommPort::getRegistry().getInstance(commName);
	if(comm!=NULL) {
		if(comm->open()) {
			if(!comm->isWriteable()) {
				std::cerr << "Dynamixel unable to send initialization" << std::endl;
			} else {
				MarkScope autolock(*comm);
				std::ostream os(&comm->getWriteStreambuf());
				//printf("\nHEADER: ");
				//debuget::hexout(BroadcastTorqueCmd(true),sizeof(BroadcastTorqueCmd));
				os.write(BroadcastTorqueCmd(true),sizeof(BroadcastTorqueCmd)).flush();
			}
		}
	}
	motionActive=true;
	commName.addPrimitiveListener(this);
}

void DynamixelDriver::motionStopping() {
	motionActive=false;
	if(!sensorsActive) // listener count is not recursive, so only remove if we're the last one
		commName.removePrimitiveListener(this);
	CommPort * comm = CommPort::getRegistry().getInstance(commName);
	if(comm!=NULL)
		comm->close(); // this *is* recursive, so we always close it to match our open() in motionStarting()
	MotionHook::motionStopping();
}

void DynamixelDriver::motionCheck(const float outputs[][NumOutputs]) {
	CommPort * comm = CommPort::getRegistry().getInstance(commName);
	if(comm==NULL || !comm->isWriteable())
		std::cerr << "Bad comm!" << std::endl;
	if(comm==NULL || !comm->isWriteable())
		return;
	
	SyncWritePosSpeedEntry packets[servos.size()];
	unsigned int packetsUsed=0;
	for(servo_iterator it=servos.begin(); it!=servos.end(); ++it) {
		int idx=it->second->output;
		if(idx<0 || static_cast<unsigned int>(idx)>=NumOutputs) {
			if(idx!=ServoInfo::UNUSED)
				std::cerr << "Warning: Dynamixel driver mapping servo " << it->first << " to invalid output index " << idx << std::endl;
			continue; // invalid/unused servo
		}
		if(isFirstCheck || lastOutputs[idx]!=outputs[NumFrames-1][idx])
			setServo(packets[packetsUsed++], it, outputs[NumFrames-1][idx]);
	}
	if(packetsUsed>0) { // if no changes, skip update altogether
		ThreadNS::Lock& l = comm->getLock();
		unsigned int t=get_time();
		// keep trying to get the lock, sleeping 1 ms each time, until 3/4 the frame time is gone (then give up)
		unsigned int dt = static_cast<unsigned int>(NumFrames*FrameTime/((getTimeScale()>0)?getTimeScale():1.f));
		unsigned int giveup = t+dt*3/4;
		while(!l.trylock()) {
			if(get_time()>=giveup) {
				if(MotionHook::verbose>0)
					cerr << "Dropping dynamixel motion update: couldn't get lock on comm port" << endl;
				return;
			}
			usleep(1000);
		}
		MarkScope autolock(l); l.unlock(); //transfer lock to MarkScope
		std::ostream os(&comm->getWriteStreambuf());
		SyncWriteHeader<SyncWritePosSpeedEntry> header(packetsUsed);
		char checksum=nchecksum(header,sizeof(header));
		checksum+=nchecksum(packets[0],packetsUsed*sizeof(SyncWritePosSpeedEntry));
		checksum=~checksum;
		os.write(header,sizeof(header));
		os.write(packets[0],packetsUsed*sizeof(SyncWritePosSpeedEntry));
		os.put(checksum);
		os.flush();
		printf("\nHEADER: ");
		debuget::hexout(reinterpret_cast<char*>(&header),sizeof(header));
		printf("\nBODY: ");
		debuget::hexout(packets[0],packetsUsed*sizeof(SyncWritePosSpeedEntry));
		printf("\nCHECKSUM: ");
		debuget::hexout(&checksum,1);
		printf("\n");
	}
	
	MotionHook::motionCheck(outputs); // updates lastOutputs and isFirstCheck, we ignore its motionUpdated() call
}

void DynamixelDriver::plistValueChanged(const plist::PrimitiveBase& pl) {
	if(&pl==&commName) {
		// if here, then motionStarted or setDataSourceThread has been called, thus when commName changes,
		// need to close old one and reopen new one
		CommPort * comm = CommPort::getRegistry().getInstance(commName.getPreviousValue());
		if(comm!=NULL) {
			// close each of our old references
			if(sensorsActive)
				comm->close();
			if(motionActive)
				comm->close();
		}
		comm = CommPort::getRegistry().getInstance(commName);
		if(comm!=NULL) {
			// open each of our new references
			if(sensorsActive)
				comm->open();
			if(motionActive)
				comm->open();
		}
	} else if(&pl==&queryServos) {
		// if here, LoadDataThread has been assigned, need to update providing/ignoring outputs
		// (and maintain listeners for individual servos while providing)
		if(queryServos) {
			for(servo_iterator it=servos.begin(); it!=servos.end(); ++it) {
				provideOutput(it->second->output);
				it->second->output.addPrimitiveListener(this);
			}
		} else {
			for(servo_iterator it=servos.begin(); it!=servos.end(); ++it) {
				it->second->output.removePrimitiveListener(this);
				ignoreOutput(it->second->output);
			}
		}
	} else {
		// check if it's one of the individual servos... if it is, means we're providing servo feedback,
		// need to call providingOutput/ignoringOutput as appropriate
		for(servo_iterator it=servos.begin(); it!=servos.end(); ++it) {
			if(&pl==&it->second->output) {
				ignoreOutput(it->second->output.getPreviousValue());
				provideOutput(it->second->output);
				return; // found it, DON'T fall through to error message below...
			}
		}
		std::cerr << "Unhandled value change in " << getClassName() << ": " << pl.get() << std::endl;
	}
}

void DynamixelDriver::setServo(SyncWritePosSpeedEntry& packet, const servo_iterator& servo, float v) {
	unsigned int servoIdx = atoi(servo->first.c_str());
	if(servoIdx>255)
		return;
	unsigned int outputIdx = servo->second->output;
	// get output's range in radians
	float outRange = outputRanges[outputIdx][MaxRange]-outputRanges[outputIdx][MinRange];
	// get servo's range in pulse width (ms)
	unsigned int servoRange = 1023;
	// get commanded position as percent of range of motion
	float cmd = (v-outputRanges[outputIdx][MinRange])/outRange;
	// do conversion from radians (output domain) to pulse width (servo domain)
	float pw = cmd*servoRange;
	// round to int
	int bpw = static_cast<int>(pw+0.5);
	// check bounds
	if(bpw<0)
		bpw=0;
	if(bpw>1023)
		bpw=1023;
	packet = SyncWritePosSpeedEntry(servoIdx,bpw,0);
}

/*! @file
 * @brief 
 * @author Ethan Tira-Thompson (ejt) (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Date: 2007/11/05 00:20:11 $
 */
