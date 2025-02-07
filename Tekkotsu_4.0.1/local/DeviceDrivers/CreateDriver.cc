#include <arpa/inet.h>
#include <stdio.h>
#include "CreateCommands.h"
#include "CreateDriver.h"
#include "Shared/RobotInfo.h"
#include "Shared/MarkScope.h"
#include "Shared/get_time.h"
#include "Shared/debuget.h"

using namespace std; 

const std::string CreateDriver::autoRegisterCreateDriver = DeviceDriver::getRegistry().registerType<CreateDriver>("Create");

void CreateDriver::motionStarting() {
	std::cout << "motionStarting called!" << std::endl;
	MotionHook::motionStarting();
	CommPort * comm = CommPort::getRegistry().getInstance(commName);
	if(comm!=NULL){
		comm->open();

		std::cout << "Motion Starting Connect" << std::endl;
		// connect :D
		connect();
	}
	motionActive=true;
	commName.addPrimitiveListener(this);
}

void CreateDriver::motionStopping() {
	motionActive=false;
	if(!sensorsActive) // listener count is not recursive, so only remove if we're the last one
		commName.removePrimitiveListener(this);
	CommPort * comm = CommPort::getRegistry().getInstance(commName);
	if(comm!=NULL)
		comm->close(); // this *is* recursive, so we always close it to match our open() in motionStarting()
	MotionHook::motionStopping();
}

void CreateDriver::motionCheck(const float outputs[][NumOutputs]) {
	CommPort * comm = CommPort::getRegistry().getInstance(commName);
	if(comm==NULL || !comm->isWriteable())
		return;

	stringstream ss;

	unsigned short output;

	ss << CREATE_DRIVE_DIRECT;

	//output = htons((short)outputs[NumFrames-1][1]);
	output = (unsigned short)(outputs[NumFrames-1][1] * 10);
	//output = -100;
	ss << (char)(output >> 8);
	ss << (char)(output & 0xFF);

	//output = htons((short)outputs[NumFrames-1][0]);
	output = (unsigned short)(outputs[NumFrames-1][0] * 10);
	//output = -100;
	ss << (char)(output >> 8);
	ss << (char)(output & 0xFF);

	//std::cout << "MOTION CHECK DATA: " << (short) outputs[NumFrames-1][1] << " " << (short)outputs[NumFrames-1][0] << std::endl;

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
				/*
				if(MotionHook::verbose>0)
					cerr << "Dropping motion update: couldn't get lock on comm port" << endl;
				*/
				return;
			}
			usleep(1000);
		}
		MarkScope autolock(l); l.unlock(); //transfer lock to MarkScope
		std::ostream os(&comm->getWriteStreambuf());
		dt=t-get_time();
		os << s << flush;
	}
	
	MotionHook::motionCheck(outputs); // updates lastOutputs and isFirstCheck, we ignore its motionUpdated() call
}

// htonl (long)
// htons (short)
void CreateDriver::connect(){
	CommPort * comm = CommPort::getRegistry().getInstance(commName);
	ThreadNS::Lock& l = comm->getLock();
	unsigned int t=get_time();
	unsigned int giveup = t+3000000;
	while(!l.trylock()) {
		if(get_time()>=giveup) {
			if(MotionHook::verbose>0)
				cerr << "Unable to connect: couldn't get lock on comm port" << endl;
			return;
		}
		usleep(1000);
	}
	MarkScope autolock(l); l.unlock(); //transfer lock to MarkScope

	std::cout << "SENDING START/SAFE COMMANDS" << std::endl;
	std::ostream os(&comm->getWriteStreambuf());
	os << CREATE_START << std::flush; // send the start command
	os << CREATE_SAFE  << std::flush; // send the safe mode

	// start stream for _everything_
	os << CREATE_STREAM << (char)1 << CREATE_SENSOR_GROUP_6 << std::flush;
}

unsigned int CreateDriver::nextTimestamp() {
	CommPort * comm = CommPort::getRegistry().getInstance(commName);
	if(comm==NULL || !comm->isReadable())
		return -1U;
	return get_time();
}

unsigned char CreateDriver::readChar(std::istream &is){
	return (unsigned char) is.get();
}

short CreateDriver::readShort(std::istream &is){
	return ((short) is.get() << 8) | ((short) is.get());
}

int CreateDriver::readPacket(std::istream &is, const char &type){
	int total = 0;
	switch(type){
	case CREATE_SENSOR_GROUP_0:
		total += readPacket(is, CREATE_SENSOR_DROP);
		total += readPacket(is, CREATE_SENSOR_WALL);
		total += readPacket(is, CREATE_SENSOR_CLIFF_LEFT);
		total += readPacket(is, CREATE_SENSOR_CLIFF_FRONT_LEFT);
		total += readPacket(is, CREATE_SENSOR_CLIFF_FRONT_RIGHT);
		total += readPacket(is, CREATE_SENSOR_CLIFF_RIGHT);
		total += readPacket(is, CREATE_SENSOR_VIRTUAL_WALL);
		total += readPacket(is, CREATE_SENSOR_OVERCURRENT);
		total += readPacket(is, CREATE_SENSOR_UNUSED_1);
		total += readPacket(is, CREATE_SENSOR_UNUSED_2);
		total += readPacket(is, CREATE_SENSOR_IR);
		total += readPacket(is, CREATE_SENSOR_BUTTONS);
		total += readPacket(is, CREATE_SENSOR_DISTANCE);
		total += readPacket(is, CREATE_SENSOR_ANGLE);
		total += readPacket(is, CREATE_SENSOR_CHANGING_STATE);
		total += readPacket(is, CREATE_SENSOR_VOLTAGE);
		total += readPacket(is, CREATE_SENSOR_CURRENT);
		total += readPacket(is, CREATE_SENSOR_BATTERY_TEMP);
		total += readPacket(is, CREATE_SENSOR_BATTERY_CHARGE);
		total += readPacket(is, CREATE_SENSOR_BATTERY_CAPACITY);
		break; 
	case CREATE_SENSOR_GROUP_1:
		total += readPacket(is, CREATE_SENSOR_DROP);
		total += readPacket(is, CREATE_SENSOR_WALL);
		total += readPacket(is, CREATE_SENSOR_CLIFF_LEFT);
		total += readPacket(is, CREATE_SENSOR_CLIFF_FRONT_LEFT);
		total += readPacket(is, CREATE_SENSOR_CLIFF_FRONT_RIGHT);
		total += readPacket(is, CREATE_SENSOR_CLIFF_RIGHT);
		total += readPacket(is, CREATE_SENSOR_VIRTUAL_WALL);
		total += readPacket(is, CREATE_SENSOR_OVERCURRENT);
		total += readPacket(is, CREATE_SENSOR_UNUSED_1);
		total += readPacket(is, CREATE_SENSOR_UNUSED_2);
		break;
	case CREATE_SENSOR_GROUP_2:
		total += readPacket(is, CREATE_SENSOR_IR);
		total += readPacket(is, CREATE_SENSOR_BUTTONS);
		total += readPacket(is, CREATE_SENSOR_DISTANCE);
		total += readPacket(is, CREATE_SENSOR_ANGLE);
		break;
	case CREATE_SENSOR_GROUP_3:
		total += readPacket(is, CREATE_SENSOR_CHANGING_STATE);
		total += readPacket(is, CREATE_SENSOR_VOLTAGE);
		total += readPacket(is, CREATE_SENSOR_CURRENT);
		total += readPacket(is, CREATE_SENSOR_BATTERY_TEMP);
		total += readPacket(is, CREATE_SENSOR_BATTERY_CHARGE);
		total += readPacket(is, CREATE_SENSOR_BATTERY_CAPACITY);
		break;
	case CREATE_SENSOR_GROUP_4:
		total += readPacket(is, CREATE_SENSOR_WALL_SIGNAL);
		total += readPacket(is, CREATE_SENSOR_CLIFF_L_SIGNAL);
		total += readPacket(is, CREATE_SENSOR_CLIFF_FL_SIGNAL);
		total += readPacket(is, CREATE_SENSOR_CLIFF_FR_SIGNAL);
		total += readPacket(is, CREATE_SENSOR_CLIFF_R_SIGNAL);
		total += readPacket(is, CREATE_SENSOR_DIGITAL_IN);
		total += readPacket(is, CREATE_SENSOR_ANALOG);
		total += readPacket(is, CREATE_SENSOR_CAN_CHARGE);
		break;
	case CREATE_SENSOR_GROUP_5:
		total += readPacket(is, CREATE_SENSOR_OI_MODE);
		total += readPacket(is, CREATE_SENSOR_SONG_NUMBER);
		total += readPacket(is, CREATE_SENSOR_SONG_PLAYING);
		total += readPacket(is, CREATE_SENSOR_STREAM_SIZE);
		total += readPacket(is, CREATE_SENSOR_REQ_VELOCITY);
		total += readPacket(is, CREATE_SENSOR_REQ_RADIUS);
		total += readPacket(is, CREATE_SENSOR_REQ_RIGHT_VELOCITY);
		total += readPacket(is, CREATE_SENSOR_REQ_LEFT_VELOCITY);
		break;
	case CREATE_SENSOR_GROUP_6:
		total += readPacket(is, CREATE_SENSOR_DROP);
		total += readPacket(is, CREATE_SENSOR_WALL);
		total += readPacket(is, CREATE_SENSOR_CLIFF_LEFT);
		total += readPacket(is, CREATE_SENSOR_CLIFF_FRONT_LEFT);
		total += readPacket(is, CREATE_SENSOR_CLIFF_FRONT_RIGHT);
		total += readPacket(is, CREATE_SENSOR_CLIFF_RIGHT);
		total += readPacket(is, CREATE_SENSOR_VIRTUAL_WALL);
		total += readPacket(is, CREATE_SENSOR_OVERCURRENT);
		total += readPacket(is, CREATE_SENSOR_UNUSED_1);
		total += readPacket(is, CREATE_SENSOR_UNUSED_2);
		total += readPacket(is, CREATE_SENSOR_IR);
		total += readPacket(is, CREATE_SENSOR_BUTTONS);
		total += readPacket(is, CREATE_SENSOR_DISTANCE);
		total += readPacket(is, CREATE_SENSOR_ANGLE);
		total += readPacket(is, CREATE_SENSOR_CHANGING_STATE);
		total += readPacket(is, CREATE_SENSOR_VOLTAGE);
		total += readPacket(is, CREATE_SENSOR_CURRENT);
		total += readPacket(is, CREATE_SENSOR_BATTERY_TEMP);
		total += readPacket(is, CREATE_SENSOR_BATTERY_CHARGE);
		total += readPacket(is, CREATE_SENSOR_BATTERY_CAPACITY);
		total += readPacket(is, CREATE_SENSOR_WALL_SIGNAL);
		total += readPacket(is, CREATE_SENSOR_CLIFF_L_SIGNAL);
		total += readPacket(is, CREATE_SENSOR_CLIFF_FL_SIGNAL);
		total += readPacket(is, CREATE_SENSOR_CLIFF_FR_SIGNAL);
		total += readPacket(is, CREATE_SENSOR_CLIFF_R_SIGNAL);
		total += readPacket(is, CREATE_SENSOR_DIGITAL_IN);
		total += readPacket(is, CREATE_SENSOR_ANALOG);
		total += readPacket(is, CREATE_SENSOR_CAN_CHARGE);
		total += readPacket(is, CREATE_SENSOR_OI_MODE);
		total += readPacket(is, CREATE_SENSOR_SONG_NUMBER);
		total += readPacket(is, CREATE_SENSOR_SONG_PLAYING);
		total += readPacket(is, CREATE_SENSOR_STREAM_SIZE);
		total += readPacket(is, CREATE_SENSOR_REQ_VELOCITY);
		total += readPacket(is, CREATE_SENSOR_REQ_RADIUS);
		total += readPacket(is, CREATE_SENSOR_REQ_RIGHT_VELOCITY);
		total += readPacket(is, CREATE_SENSOR_REQ_LEFT_VELOCITY);
		break;
	case CREATE_SENSOR_STREAM_SIZE:
		total = 1;
		readChar(is);
		break;
	case CREATE_SENSOR_DROP:
		total = 1;
		createStatus.bumpsWheelDrops = readChar(is);
		break;
	case CREATE_SENSOR_WALL:
		total = 1;
		createStatus.wall = readChar(is);
		break;
	case CREATE_SENSOR_CLIFF_LEFT:
		total = 1;
		createStatus.cliffLeft = readChar(is);
		break;
	case CREATE_SENSOR_CLIFF_FRONT_LEFT:
		total = 1;
	  createStatus.cliffFrontLeft = readChar(is);
		break;
	case CREATE_SENSOR_CLIFF_FRONT_RIGHT:
		total = 1;
		createStatus.cliffFrontRight = readChar(is);
		break;
	case CREATE_SENSOR_CLIFF_RIGHT:
		total = 1;
		createStatus.cliffRight = readChar(is);
		break;
	case CREATE_SENSOR_VIRTUAL_WALL:
		total = 1;
		createStatus.virtualWall = readChar(is);
		break;
	case CREATE_SENSOR_OVERCURRENT:
		total = 1;
		createStatus.overcurrents = readChar(is);
		break;
	case CREATE_SENSOR_IR:
		total = 1;
		createStatus.ir = readChar(is);
		break;
	case CREATE_SENSOR_BUTTONS:
		total = 1;
		createStatus.buttons = readChar(is);
		break;
	case CREATE_SENSOR_DISTANCE:
		total = 2;
		createStatus.distance = readShort(is);
		break;
	case CREATE_SENSOR_ANGLE:
		total = 2;
		createStatus.angle = readShort(is);
		break;
	case CREATE_SENSOR_CHANGING_STATE:
		total = 1;
		createStatus.chargingState = readChar(is);
		break;
	case CREATE_SENSOR_VOLTAGE:
		total = 2;
		createStatus.voltage = readShort(is);
		break;
	case CREATE_SENSOR_CURRENT:
		total = 2;
		createStatus.current = readShort(is);
		break;
	case CREATE_SENSOR_BATTERY_TEMP:
		total = 1;
		createStatus.batteryTemperature = readChar(is);
		break;
	case CREATE_SENSOR_BATTERY_CHARGE:
		total = 2;
		createStatus.batteryCharge = readShort(is);
		break;
	case CREATE_SENSOR_BATTERY_CAPACITY:
		total = 2;
		createStatus.batteryCapacity = readShort(is);
		break;
	case CREATE_SENSOR_WALL_SIGNAL:
		total = 2;
		createStatus.wallSignal = readShort(is);
		break;
	case CREATE_SENSOR_CLIFF_L_SIGNAL:
		total = 2;
		createStatus.cliffLeftSignal = readShort(is);
		break;
	case CREATE_SENSOR_CLIFF_FL_SIGNAL:
		total = 2;
		createStatus.cliffFrontLeftSignal = readShort(is);
		break;
	case CREATE_SENSOR_CLIFF_FR_SIGNAL:
		total = 2;
		createStatus.cliffFrontRightSignal = readShort(is);
		break;
	case CREATE_SENSOR_CLIFF_R_SIGNAL:
		total = 2;
		createStatus.cliffRightSignal = readShort(is);
		break;
	case CREATE_SENSOR_DIGITAL_IN:
		total = 1;
	  createStatus.userDigitalInputs= readChar(is);
		break;
	case CREATE_SENSOR_ANALOG:
		total = 2;
		createStatus.userAnalogInput = readShort(is);
		break;
	case CREATE_SENSOR_CAN_CHARGE:
		total = 1;
		createStatus.chargingSourcesAvailable = readChar(is);
		break;
	case CREATE_SENSOR_OI_MODE:
		total = 1;
		createStatus.oiMode = readChar(is);
		break;
	case CREATE_SENSOR_SONG_NUMBER:
		total = 1;
		createStatus.songNumber = readChar(is);
		break;
	case CREATE_SENSOR_SONG_PLAYING:
		total = 1;
		createStatus.songPlay = readChar(is);
		break;
	case CREATE_SENSOR_REQ_VELOCITY:
		total = 2;
	  createStatus.velocity = readShort(is);
		break;
	case CREATE_SENSOR_REQ_RADIUS:
		total = 2;
		createStatus.radius = readShort(is);
		break;
	case CREATE_SENSOR_REQ_RIGHT_VELOCITY:
		total = 2;
	  createStatus.rightVelocity = readShort(is);
		break;
	case CREATE_SENSOR_REQ_LEFT_VELOCITY:
		total = 2;
		createStatus.leftVelocity = readShort(is);
		break;
	case CREATE_SENSOR_UNUSED_1:
	case CREATE_SENSOR_UNUSED_2:
		total = 1;
		readChar(is);
		break;
	default:
		std::cerr << "CREATE DRIVER: unknown packet type " << (int) type << std::endl;
		break;
	}

	return total;
}

unsigned int CreateDriver::getData(const char *& payload, unsigned int& payloadSize, unsigned int& timestamp, std::string& name) {
	std::cout << "getData call!" << std::endl;
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
	ss << "condensed " << RobotInfo::RobotName << "\n";
	ss << "meta-info = ";
	ss << t;
	ss << " ";
	ss << t;
	ss << "\n";
	std::cout << "begin read" << std::endl;
	{
		MarkScope autolock(comm->getLock());
		std::ostream os(&comm->getWriteStreambuf());
		std::istream is(&comm->getReadStreambuf());
		
		os << CREATE_STREAM << (char)1 << CREATE_SENSOR_GROUP_6 << std::flush;
		
		// Begin packet reading...
		unsigned char in=0;
		unsigned char type=0;
		unsigned char numPackets = 0;
		int i;


		while(1){
			type = readChar(is);
			if(type == 19){
				numPackets = readChar(is);
				printf("reading %d packets\n", numPackets);
				for(i = 0; i < numPackets; i+=in){
					type = readChar(is);
					printf("reading type %d\n", type);
					in = readPacket(is, type) + 1;
					printf("read %d packets\n", in);
				}
				if(i != numPackets){
					std::cerr << "CREATE DRIVER: read in more than expected! Read " << i << " Expected " << (int)numPackets << std::endl;
				}
				printf("CREATE DRIVER: checksum %d\n", readChar(is));
				break;
			}
			else{
				std::cerr << "CREATE DRIVER: did not get packet with data '19' as expected! Got: " << (unsigned short) type << std::endl;
			}
		}

	}
	ss << "outputs =" << createStatus.leftVelocity << " " << createStatus.rightVelocity << " 0 0" << "\n";
	ss << "sensors =";
	ss << (createStatus.userDigitalInputs & 0x1) << " ";
	ss << ((createStatus.userDigitalInputs >> 1) & 0x1) << " ";
	ss << ((createStatus.userDigitalInputs >> 2) & 0x1) << " ";
	ss << ((createStatus.userDigitalInputs >> 3) & 0x1) << " ";
	ss << createStatus.userAnalogInput << " ";
	ss << createStatus.wallSignal << " ";
	ss << createStatus.ir << " ";
	ss << createStatus.cliffLeftSignal << " ";
	ss << createStatus.cliffFrontLeftSignal << " ";
	ss << createStatus.cliffFrontRightSignal << " ";
	ss << createStatus.cliffRightSignal << " ";
	ss << createStatus.distance << " ";
	ss << createStatus.angle << " ";
	ss << "0 0 0 0 0\n";
	ss << "#END\n";
	lastSensor=ss.str();
	//cerr << lastSensor << endl;
	payload=lastSensor.c_str();
	payloadSize=lastSensor.size();
	name=nextName();
	std::cout << "finish getData!" << std::endl;
	return frameNumber++;
}

void CreateDriver::setDataSourceThread(LoadDataThread* th) {
	std::cout << "setDataSounceThread called!!" << std::endl;
	if(thread==NULL && th!=NULL) {
		CommPort * comm = CommPort::getRegistry().getInstance(commName);
		if(comm!=NULL){
			comm->open();
			
			std::cout << "set data sounce thread" << std::endl;
			// connect :D
			connect();
		}
		sensorsActive=true;
		commName.addPrimitiveListener(this);
	}
	if(thread!=NULL && th==NULL) {
		CommPort * comm = CommPort::getRegistry().getInstance(commName);
		if(comm!=NULL)
			comm->close();
		sensorsActive=false;
		if(!motionActive) // listener count is not recursive, so only remove if we're the last one
			commName.removePrimitiveListener(this);

	}
	DataSource::setDataSourceThread(th);
}

void CreateDriver::plistValueChanged(const plist::PrimitiveBase& pl) {
	std::cout << "plistvalueChanged Called!" << std::endl;
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

			// connect :D
			connect();
		}
	}
}

float CreateDriver::getAnalog(unsigned int /*inputIdx*/, unsigned char s) {
	return s*5.f/256;
}

float CreateDriver::getDigital(unsigned int /*inputIdx*/, unsigned char cur, unsigned char latch) {
	if(cur=='0')
		return 0;
	return (latch=='0') ? 0.5f : 1;
}

/*! @file
 * @brief 
 * @author Ethan Tira-Thompson (ejt) (Creator)
 *
 * $Author: bensont $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.7 $
 * $State: Exp $
 * $Date: 2007/11/18 19:16:10 $
 */
