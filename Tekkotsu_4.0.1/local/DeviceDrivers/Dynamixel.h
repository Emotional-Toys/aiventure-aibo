//-*-c++-*-
#ifndef INCLUDED_Dynamixel_h_
#define INCLUDED_Dynamixel_h_

#include "local/DeviceDriver.h"
#include "local/MotionHook.h"
#include "local/DataSource.h"
#include "local/CommPort.h"
#include "Shared/plist.h"
#include <iostream>
#include <sstream>

//! description of Dynamixel
class DynamixelDriver : public virtual DeviceDriver, public MotionHook, /*public DataSource,*/ public virtual plist::PrimitiveListener {
public:
	explicit DynamixelDriver(const std::string& name)
		: DeviceDriver(autoRegisterDynamixelDriver,name), /*MotionHook(), DataSource(),*/
		protocol(CM5,protocolNames), servos(), commName(), queryServos(false), 
		motionActive(false), sensorsActive(false), lastSensor(), frameNumber(0)
	{
		addEntry("Protocol",protocol,"Indicates whether to send CM-5 controller commands or direct binary commands\n"
				 "(latter can only be used if you have a direct connection to the Dynamixel TTL bus)\n"+protocol.getDescription());
		addEntry("Servos",servos,"Maps servo IDs to Tekkotsu output offsets, use command line new/delete commands to add/remove mappings.");
		addEntry("CommPort",commName,"The name of the comm port where output will be sent");
		addEntry("QueryServos",queryServos,"If set to true, will attempt to query the servo positions with each sensor update.\nThis may decrease the sampling frequency");
		for(unsigned int i=0; i<NumPIDJoints; ++i) {
			std::stringstream bio_id; bio_id<<i+1; // bioloid sets start the id count at 1
			servos[bio_id.str()]=ServoInfo(i);
		}
	}
	virtual ~DynamixelDriver() {}
	
	virtual std::string getClassName() const { return autoRegisterDynamixelDriver; }
	
	virtual MotionHook* getMotionSink() { return dynamic_cast<MotionHook*>(this); }
	virtual void getSensorSources(std::map<std::string,DataSource*>& sources) {
		sources.clear();
		sources["Sensors"]=dynamic_cast<DataSource*>(this);
	}
	
	virtual void motionStarting();
	virtual void motionStopping();
	virtual void motionCheck(const float outputs[][NumOutputs]);
	
	/*virtual unsigned int nextTimestamp();
	virtual const std::string& nextName() { return instanceName; }
	virtual unsigned int getData(const char *& payload, unsigned int& payloadSize, unsigned int& timestamp, std::string& name);
	virtual void setDataSourceThread(LoadDataThread* th);*/
	
	virtual void plistValueChanged(const plist::PrimitiveBase& pl);
	
	static const char * protocolNames[];
	enum protocol_t {
		BINARY,
		CM5
	};
	plist::NamedEnumeration<protocol_t> protocol;
	
	class ServoInfo : public virtual plist::Dictionary {
	public:
		static const int UNUSED=-1;
		ServoInfo(int i=UNUSED)
			: plist::Dictionary(false), output(i<(int)NumPIDJoints?i:UNUSED), led(i<(int)NumLEDs?i:UNUSED), freeSpin(false)
		{
			setLoadSavePolicy(FIXED,SYNC);
			addEntry("Output",output,"Tekkotsu offset to pull servo positions from, relative to PIDJointOffset");
			addEntry("LED",led,"Tekkotsu offset to pull LED values from, relative to LEDOffset");
			addEntry("FreeSpin",freeSpin,"If true, servo will spin freely, interpreting output value as rotation speed");
		}
		plist::Primitive<int> output;
		plist::Primitive<int> led;
		plist::Primitive<bool> freeSpin;
	};
	
	plist::DictionaryOf< ServoInfo > servos;
	typedef plist::DictionaryOf< ServoInfo >::const_iterator servo_iterator;
	
	plist::Primitive<std::string> commName;
	plist::Primitive<bool> queryServos;
	
protected:
	//! forwards call to DataSource::providingOutput() if the index is valid
		void provideOutput(unsigned int idx) {}//{ if(idx<NumOutputs) providingOutput(idx); }
	//! forwards call to DataSource::ignoringOutput() if the index is valid
		void ignoreOutput(unsigned int idx) {}//{ if(idx<NumOutputs) ignoringOutput(idx); }
	
	//! converts the value @a v from radians into the specified servo's pulse width range
	virtual void setServo(struct SyncWritePosSpeedEntry& packet, const servo_iterator& servo, float v);
	/*
	//! converts the value @a pw from specified servo's pulse width range into radians
	virtual float getServo(unsigned int servoIdx, unsigned int pw);
	//! converts the value @a s from specified input's signal to voltage
	virtual float getAnalog(unsigned int inputIdx, unsigned char s);
	//! converts the value @a cur and @a latch to the output format (0 if low, 0.5 if high but has been low, 1 if consistent high)
	virtual float getDigital(unsigned int inputIdx, unsigned char cur, unsigned char latch);
	*/
	bool motionActive;
	bool sensorsActive;
	std::string lastSensor;
	unsigned int frameNumber;
	
private:
	//! holds the class name, set via registration with the DeviceDriver registry
	static const std::string autoRegisterDynamixelDriver;
};

/*! @file
 * @brief 
 * @author Ethan Tira-Thompson (ejt) (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.1 $
 * $State: Exp $
 * $Date: 2007/11/01 20:27:04 $
 */

#endif
