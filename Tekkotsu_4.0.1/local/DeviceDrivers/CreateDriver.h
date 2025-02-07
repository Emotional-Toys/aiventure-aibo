//-*-c++-*-
#ifndef INCLUDED_CreateDriver_h_
#define INCLUDED_CreateDriver_h_

#include "local/DeviceDriver.h"
#include "local/MotionHook.h"
#include "local/DataSource.h"
#include "local/CommPort.h"
#include "Shared/plist.h"
#include <iostream>

/*
Specs
Big Endian <---!!!!!


Motors: 5
0 - LD1          - 0.5A
1 - LD0          - 0.5A
2 - LD2          - 1.6A
3 - Right Wheel  - 1.0A
4 - Left Wheel   - 1.0A

Digital Inputs: 4

Digital Outputs: 3

LEDs: 2 (Play + Advanced)

Buttons: 2 (Play + Advanced)

Sensors: 5
0 - bump right
1 - bump left
2 - wheeldrop right
3 - wheeldrop left
4 - wheeldrop caster

Wall Sensor

Cliff Left

Cliff Right

Cliff Front Left

Cliff Front Right

Distance (per wheel) -- capped if not polled frequent enough

Angle (per wheel) -- capped if not polled frequent enough

Voltage

Current

Battery Temperature

Battery Charge

Battery Capacity (not accurate for alkaline)
*/

typedef struct CreateStatus_t{
	unsigned char bumpsWheelDrops;
	unsigned char wall;
	unsigned char cliffLeft;
	unsigned char cliffFrontLeft;
	unsigned char cliffFrontRight;
	unsigned char cliffRight;
  unsigned char virtualWall;
	unsigned char overcurrents;
	unsigned char ir;
	unsigned char buttons;
	short distance;
	short angle;
	unsigned char chargingState;
	short voltage;
	short current;
	unsigned char batteryTemperature;
	short batteryCharge;
	short batteryCapacity;
	short wallSignal;
	short cliffLeftSignal;
	short cliffFrontLeftSignal;
	short cliffFrontRightSignal;
	short cliffRightSignal;
	unsigned char userDigitalInputs;
	short userAnalogInput;
	unsigned char chargingSourcesAvailable;
	unsigned char oiMode;
	unsigned char songNumber;
	unsigned char songPlay;
	short velocity;
	short radius;
	short rightVelocity;
	short leftVelocity;
} CreateStatus;

//! description of CreateDriver
class CreateDriver : public virtual DeviceDriver, public MotionHook, public DataSource, public virtual plist::PrimitiveListener {
public:
	explicit CreateDriver(const std::string& name)
		: DeviceDriver(autoRegisterCreateDriver,name), MotionHook(), DataSource(),
			commName(), motionActive(false), sensorsActive(false), lastSensor(), frameNumber(0), createStatus()
	{
		addEntry("CommPort",commName,"The name of the comm port where output will be sent");
	}
	virtual ~CreateDriver() {}
	
	virtual std::string getClassName() const { return autoRegisterCreateDriver; }
	
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
	
	plist::Primitive<std::string> commName;
	
protected:
	virtual void connect();

	virtual unsigned char readChar(std::istream &is);
	virtual short readShort(std::istream &is);
	virtual int readPacket(std::istream &is, const char &type);
	
	//! forwards call to DataSource::providingOutput() if the index is valid
	void provideOutput(unsigned int idx) { if(idx<NumOutputs) providingOutput(idx); }
	//! forwards call to DataSource::ignoringOutput() if the index is valid
	void ignoreOutput(unsigned int idx) { if(idx<NumOutputs) ignoringOutput(idx); }
	
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
	static const std::string autoRegisterCreateDriver;

	CreateStatus createStatus;
};

/*! @file
 * @brief 
 * @author Ethan Tira-Thompson (ejt) (Creator)
 *
 * $Author: bensont $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.3 $
 * $State: Exp $
 * $Date: 2007/11/18 19:16:10 $
 */

#endif
