//-*-c++-*-
#ifndef INCLUDED_SSC32Driver_h_
#define INCLUDED_SSC32Driver_h_

#include "local/DeviceDriver.h"
#include "local/MotionHook.h"
#include "local/DataSource.h"
#include "local/CommPort.h"
#include "Shared/plist.h"
#include <iostream>

//! description of SSC32Driver
class SSC32Driver : public virtual DeviceDriver, public MotionHook, public DataSource, public virtual plist::PrimitiveListener {
public:
	static const unsigned int NUM_SERVO=32;
	static const unsigned int NUM_INPUT=4;
	static const int UNUSED=-1;
	
	explicit SSC32Driver(const std::string& name)
		: DeviceDriver(autoRegisterSSC32Driver,name), MotionHook(), DataSource(),
		servos(NUM_SERVO,UNUSED), inputs(NUM_INPUT,UNUSED),
		minPW(NUM_SERVO,500), maxPW(NUM_SERVO,2500), buttonMode(NUM_INPUT,false),
		sparse(false), commName(), queryServos(false), motionActive(false), sensorsActive(false), lastSensor(), frameNumber(0)
	{
		for(unsigned int i=0; i<NumOutputs && i<NUM_SERVO; ++i)
			servos[i]=i;
		for(unsigned int i=0; i<NumSensors && i<NUM_INPUT; ++i)
			inputs[i]=i;
		addEntry("OutputMap",servos,"For each of the SSC32's servo pins, lists the output index it should take its values from; -1 to mark unused");
		addEntry("InputMap",inputs,"For each of the SSC32's input pins, lists the sensor index it should send its value to; -1 to mark unused");
		addEntry("MinPulseWidth",minPW,"The low end of the servo's legal pulse width range (may correspond to unreachable position, use RobotInfo's outputRange[] to limit motion, not this)");	
		addEntry("MaxPulseWidth",maxPW,"The high end of the servo's legal pulse width range (may correspond to unreachable position, use RobotInfo's outputRange[] to limit motion, not this)");	
		addEntry("ButtonMode",buttonMode,"Controls interpretation of the input pin.\nFalse means directly measure voltage, true means test for high (1),\nhigh now but low was detected in interval (0.5), or low (0).\nButton mode implies interpreting inputMap value as a button index instead of sensor index.");
		addEntry("SparseUpdates",sparse,"If true, only send servo positions to SSC when they change, instead of all servos on every update (don't use a lossy transport like UDP if you turn this on!)");
		addEntry("CommPort",commName,"The name of the comm port where output will be sent");
		addEntry("QueryServos",queryServos,"If set to true, will attempt to query the servo positions with each sensor update.\nThis may decrease the sampling frequency");
	}
	virtual ~SSC32Driver() {}
	
	virtual std::string getClassName() const { return autoRegisterSSC32Driver; }
	
	virtual MotionHook* getMotionSink() { return dynamic_cast<MotionHook*>(this); }
	virtual void getSensorSources(std::map<std::string,DataSource*>& sources) {
		sources.clear();
		sources["Sensors"]=dynamic_cast<DataSource*>(this);
	}
	
	virtual void motionStarting();
	virtual void motionStopping();
	virtual void motionCheck(const float outputs[][NumOutputs]);
	
	virtual unsigned int nextTimestamp();
	virtual const std::string& nextName() { return instanceName; }
	virtual unsigned int getData(const char *& payload, unsigned int& payloadSize, unsigned int& timestamp, std::string& name);
	virtual void setDataSourceThread(LoadDataThread* th);
	
	virtual void plistValueChanged(const plist::PrimitiveBase& pl);
	
	plist::ArrayOf<plist::Primitive<int> > servos;
	plist::ArrayOf<plist::Primitive<int> > inputs;
	plist::ArrayOf<plist::Primitive<unsigned int> > minPW;
	plist::ArrayOf<plist::Primitive<unsigned int> > maxPW;
	plist::ArrayOf<plist::Primitive<bool> > buttonMode;
	plist::Primitive<bool> sparse;
	plist::Primitive<std::string> commName;
	plist::Primitive<bool> queryServos;
	
protected:
	//! forwards call to DataSource::providingOutput() if the index is valid
	void provideOutput(unsigned int idx) { if(idx<NumOutputs) providingOutput(idx); }
	//! forwards call to DataSource::ignoringOutput() if the index is valid
	void ignoreOutput(unsigned int idx) { if(idx<NumOutputs) ignoringOutput(idx); }
	
	//! converts the value @a v from radians into the specified servo's pulse width range
	virtual void setServo(std::ostream& ss, unsigned int servoIdx, float v);
	//! converts the value @a pw from specified servo's pulse width range into radians
	virtual float getServo(unsigned int servoIdx, unsigned int pw);
	//! converts the value @a s from specified input's signal to voltage
	virtual float getAnalog(unsigned int inputIdx, unsigned char s);
	//! converts the value @a cur and @a latch to the output format (0 if low, 0.5 if high but has been low, 1 if consistent high)
	virtual float getDigital(unsigned int inputIdx, unsigned char cur, unsigned char latch);
	
	bool motionActive;
	bool sensorsActive;
	std::string lastSensor;
	unsigned int frameNumber;
	
private:
	//! holds the class name, set via registration with the DeviceDriver registry
	static const std::string autoRegisterSSC32Driver;
};

/*! @file
 * @brief 
 * @author Ethan Tira-Thompson (ejt) (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Date: 2007/06/06 18:00:59 $
 */

#endif
