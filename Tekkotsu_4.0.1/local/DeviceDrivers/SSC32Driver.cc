#include "SSC32Driver.h"
#include "Shared/MarkScope.h"
#include "Shared/get_time.h"
#include "Shared/debuget.h"

using namespace std; 

const unsigned int SSC32Driver::NUM_SERVO;
const int SSC32Driver::UNUSED;
const std::string SSC32Driver::autoRegisterSSC32Driver = DeviceDriver::getRegistry().registerType<SSC32Driver>("SSC32");

void SSC32Driver::motionStarting() {
	MotionHook::motionStarting();
	CommPort * comm = CommPort::getRegistry().getInstance(commName);
	if(comm!=NULL)
		comm->open();
	motionActive=true;
	commName.addPrimitiveListener(this);
}

void SSC32Driver::motionStopping() {
	motionActive=false;
	if(!sensorsActive) // listener count is not recursive, so only remove if we're the last one
		commName.removePrimitiveListener(this);
	CommPort * comm = CommPort::getRegistry().getInstance(commName);
	if(comm!=NULL)
		comm->close(); // this *is* recursive, so we always close it to match our open() in motionStarting()
	MotionHook::motionStopping();
}

void SSC32Driver::motionCheck(const float outputs[][NumOutputs]) {
	CommPort * comm = CommPort::getRegistry().getInstance(commName);
	if(comm==NULL || !comm->isWriteable())
		return;
	
	stringstream ss;
	for(unsigned int i=0; i<NUM_SERVO; i++) {
		int idx=servos[i];
		if(idx<0 || static_cast<unsigned int>(idx)>=NumOutputs) {
			if(idx!=UNUSED)
				std::cerr << "Warning: SSC32 driver mapping servo " << i << " to invalid output index " << idx << std::endl;
			continue; // invalid/unused servo
		}
		if(isFirstCheck || !sparse || lastOutputs[idx]!=outputs[NumFrames-1][idx])
			setServo(ss, i, outputs[NumFrames-1][idx]);
	}
	string s=ss.str();
	if(s.size()>0) { // if sparse and no changes, skip update altogether
		ThreadNS::Lock& l = comm->getLock();
		unsigned int t=get_time();
		// keep trying to get the lock, sleeping 1 ms each time, until 3/4 the frame time is gone (then give up)
		unsigned int dt = static_cast<unsigned int>(NumFrames*FrameTime/((getTimeScale()>0)?getTimeScale():1.f));
		unsigned int giveup = t+dt*3/4;
		t+=dt;
		while(!l.trylock()) {
			if(get_time()>=giveup) {
				if(MotionHook::verbose>0)
					cerr << "Dropping SSC32 motion update: couldn't get lock on comm port" << endl;
				return;
			}
			usleep(1000);
		}
		MarkScope autolock(l); l.unlock(); //transfer lock to MarkScope
		std::ostream os(&comm->getWriteStreambuf());
		unsigned int curt = get_time();
		if(curt>=t) // too late!
			os << s << '\r' << flush;
		else {
			dt=t-curt;
			os << s << 'T' << dt << '\r' << flush; // indicate time until next update
		}
	}
	
	MotionHook::motionCheck(outputs); // updates lastOutputs and isFirstCheck, we ignore its motionUpdated() call
}


unsigned int SSC32Driver::nextTimestamp() {
	CommPort * comm = CommPort::getRegistry().getInstance(commName);
	if(comm==NULL || !comm->isReadable())
		return -1U;
	return get_time();
}

unsigned int SSC32Driver::getData(const char *& payload, unsigned int& payloadSize, unsigned int& timestamp, std::string& name) {
	payload=NULL; payloadSize=0; // in case of error
	CommPort * comm = CommPort::getRegistry().getInstance(commName);
	if(comm==NULL || !comm->isReadable() || !comm->isWriteable())
		return frameNumber;
	unsigned int t=get_time();
	if(timestamp>t)
		usleep(static_cast<unsigned int>((timestamp-t)*1000/(getTimeScale()>0?getTimeScale():1.f)));
	if(thread==NULL) // in case we shut down while waiting
		return frameNumber;
	stringstream ss;
	timestamp=get_time();
	ss << "#POS\n";
	{
		MarkScope autolock(comm->getLock());
		std::ostream os(&comm->getWriteStreambuf());
		std::istream is(&comm->getReadStreambuf());
		// generally, bad idea to request servo position before position has been sent
		// But if the arm is still powered, it'll know it's position.  If the user has set WaitForSensors,
		// then DataSource::requiresFirstSensor will be set, otherwise we skip queries until we've sent
		// a servo position (and thus set the isFirstCheck flag
		if((!isFirstCheck || DataSource::requiresFirstSensor) && queryServos) {
			// check joint positions
			stringstream q;
			for(unsigned int i=0; i<NUM_SERVO; i++) {
				int idx=servos[i];
				if(idx<0 || static_cast<unsigned int>(idx)>=NumOutputs)
					continue; // invalid/unused servo
				q << "QP " << i << ' ';
			}
			os << q.rdbuf() << '\r' << flush;
			for(unsigned int i=0; i<NUM_SERVO; i++) {
				int idx=servos[i];
				if(idx<0 || static_cast<unsigned int>(idx)>=NumOutputs)
					continue; // invalid/unused servo
				int check=is.get();
				if(check==-1) {
					cerr << "SSC32Driver: bad read!" << endl;
					return frameNumber;
				}
				unsigned int v=(unsigned char)check;
				ss << outputNames[idx] << '\t' << getServo(i,v*10) << "\t1\n";
			}
		}
		stringstream aq,dq;
		bool acnt=0,dcnt=0;
		for(unsigned int i=0; i<NUM_INPUT; i++) {
			int idx=inputs[i];
			if(!buttonMode[i]) {
				if(idx<0 || static_cast<unsigned int>(idx)>=NumSensors) {
					if(idx!=UNUSED)
						std::cerr << "Warning: SSC32 driver mapping input " << i << " to invalid sensor index " << idx << std::endl;
					continue; // invalid/unused servo
				}
				++acnt;
				aq << 'V' << char('A'+i) << ' ';
			} else {
				if(idx<0 || static_cast<unsigned int>(idx)>=NumButtons) {
					if(idx!=UNUSED)
						std::cerr << "Warning: SSC32 driver mapping input " << i << " to invalid button index " << idx << std::endl;
					continue; // invalid/unused servo
				}
				++dcnt;
				dq << char('A'+i) << ' ' << char('A'+i) << "L ";
			}
		}
		// send both queries now, we can process first response while SSC is processing second query
		if(dcnt>0)
			os << dq.rdbuf() << '\r';
		if(acnt>0)
			os << aq.rdbuf() << '\r';
		if(dcnt>0 || acnt>0)
			os << flush;
		// store responses
		if(dcnt>0) {
			ss << "<buttons>\n";
			for(unsigned int i=0; i<NUM_INPUT; i++) {
				int idx=inputs[i];
				if(idx>=0 && static_cast<unsigned int>(idx)<NumButtons && buttonMode[i]) {
					int check=is.get();
					if(check==-1) {
						cerr << "SSC32Driver: bad read!" << endl;
						return frameNumber;
					}
					unsigned char cur=check;
					check=is.get();
					if(check==-1) {
						cerr << "SSC32Driver: bad read!" << endl;
						return frameNumber;
					}
					unsigned char latch=check;
					float v=getDigital(i,cur,latch);
					ss << buttonNames[idx] << '\t' << v << '\n';
				}
			}
			ss << "</buttons>\n";
		}
		if(acnt>0) {
			ss << "<sensors>\n";
			for(unsigned int i=0; i<NUM_INPUT; i++) {
				int idx=inputs[i];
				if(idx>=0 && static_cast<unsigned int>(idx)<NumSensors && !buttonMode[i]) {
					int check=is.get();
					if(check==-1) {
						cerr << "SSC32Driver: bad read!" << endl;
						return frameNumber;
					}
					float v=getAnalog(i,check);
					ss << sensorNames[idx] << '\t' << v << '\n';
				}
			}
			ss << "</sensors>\n";
		}
	}
	ss << "#END\n";
	lastSensor=ss.str();
	//cerr << lastSensor << endl;
	payload=lastSensor.c_str();
	payloadSize=lastSensor.size();
	name=nextName();
	return frameNumber++;
}

void SSC32Driver::setDataSourceThread(LoadDataThread* th) {
	if(thread==NULL && th!=NULL) {
		CommPort * comm = CommPort::getRegistry().getInstance(commName);
		if(comm!=NULL)
			comm->open();
		queryServos.addPrimitiveListener(this);
		if(queryServos) {
			for(unsigned int i=0; i<NUM_SERVO; i++) {
				provideOutput(servos[i]);
				servos[i].addPrimitiveListener(this);
			}
		}
		sensorsActive=true;
		commName.addPrimitiveListener(this);
	}
	if(thread!=NULL && th==NULL) {
		CommPort * comm = CommPort::getRegistry().getInstance(commName);
		if(comm!=NULL)
			comm->close();
		if(queryServos) {
			for(unsigned int i=0; i<NUM_SERVO; ++i) {
				servos[i].removePrimitiveListener(this);
				ignoreOutput(servos[i]);
			}
		}
		queryServos.removePrimitiveListener(this);
		sensorsActive=false;
		if(!motionActive) // listener count is not recursive, so only remove if we're the last one
			commName.removePrimitiveListener(this);
	}
	DataSource::setDataSourceThread(th);
}

void SSC32Driver::plistValueChanged(const plist::PrimitiveBase& pl) {
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
			for(unsigned int i=0; i<NUM_SERVO; i++) {
				provideOutput(servos[i]);
				servos[i].addPrimitiveListener(this);
			}
		} else {
			for(unsigned int i=0; i<NUM_SERVO; ++i) {
				servos[i].removePrimitiveListener(this);
				ignoreOutput(servos[i]);
			}
		}
	} else {
		// check if it's one of the individual servos... if it is, means we're providing servo feedback,
		// need to call providingOutput/ignoringOutput as appropriate
		for(unsigned int i=0; i<NUM_SERVO; ++i) {
			if(&pl==&servos[i]) {
				ignoreOutput(servos[i].getPreviousValue());
				provideOutput(servos[i]);
				return; // found it, DON'T fall through to error message below...
			}
		}
		std::cerr << "Unhandled value change in " << getClassName() << ": " << pl.get() << std::endl;
	}
}

void SSC32Driver::setServo(std::ostream& ss, unsigned int servoIdx, float v) {
	unsigned int outputIdx = servos[servoIdx];
	// get output's range in radians
	float outRange = outputRanges[outputIdx][MaxRange]-outputRanges[outputIdx][MinRange];
	// get servo's range in pulse width (ms)
	unsigned int servoRange = maxPW[servoIdx]-minPW[servoIdx];
	// get commanded position as percent of range of motion
	float cmd = (v-outputRanges[outputIdx][MinRange])/outRange;
	// flip commanded position -- map positive (high) rotation to low pulse width
	// this is so if you mount a servo "up", coordinate system will work correctly
	cmd=1-cmd;
	// do conversion from radians (output domain) to pulse width (servo domain)
	float pw = cmd*servoRange+minPW[servoIdx];
	// round to int
	unsigned int bpw = static_cast<unsigned int>(pw+0.5);
	// check bounds
	if(bpw<minPW[servoIdx])
		bpw=minPW[servoIdx];
	if(bpw>maxPW[servoIdx])
		bpw=maxPW[servoIdx];
	// send to output buffer
	ss << '#' << servoIdx << " P" << bpw << ' ';
}

float SSC32Driver::getServo(unsigned int servoIdx, unsigned int pw) {
	unsigned int outputIdx = servos[servoIdx];
	// get output's range in radians
	double outRange = outputRanges[outputIdx][MaxRange]-outputRanges[outputIdx][MinRange];
	// get servo's range in pulse width (ms)
	unsigned int servoRange = maxPW[servoIdx]-minPW[servoIdx];
	// do conversion from pulse width (servo domain) to radians (output domain)
	return (pw-minPW[servoIdx])*outRange/servoRange + outputRanges[outputIdx][MinRange];
}

float SSC32Driver::getAnalog(unsigned int /*inputIdx*/, unsigned char s) {
	return s*5.f/256;
}

float SSC32Driver::getDigital(unsigned int /*inputIdx*/, unsigned char cur, unsigned char latch) {
	if(cur=='0')
		return 0;
	return (latch=='0') ? 0.5f : 1;
}

/*! @file
 * @brief 
 * @author Ethan Tira-Thompson (ejt) (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.6 $
 * $State: Exp $
 * $Date: 2007/11/01 20:25:05 $
 */
