#ifdef HAVE_ICE

#include "Shared/RobotInfo.h"
#include "Shared/ImageUtil.h"
#include "Shared/debuget.h"
#include "Shared/LoadSave.h"
#include "Shared/MarkScope.h"
#include "DataCache.h"
#include "QwerkBot.h"
#include <unistd.h>

using namespace std;

DataCache::DataCache(QwerkBot* qwerk) : number(0), qb(qwerk), state(), states(), cstate(0), frameLock() {
}

unsigned int DataCache::nextTimestamp() {
	return 0;
}

const std::string& DataCache::nextName() {
	static const std::string noneStr="(none)";
	return noneStr;
}

unsigned int DataCache::getData(const char *& payload, unsigned int& size, unsigned int& timestamp, string& name) {
	::TeRK::QwerkState qstate = qb->requestStateFrame();
	timestamp = get_time();
	number++;
	
	// TODO: ask ethen about state serialization
	// clear the state data
	state.str("");
	state << "#POS\n";
	state << "condensed " << RobotInfo::RobotName << "\n";
	
	state << "meta-info = ";
	state << timestamp;
	state << " ";
	state << number;
	state << "\n";
	
	state << "outputs =";
	
	const int SERVO_MAX=255;
#ifdef TGT_HAS_WHEELS
	const int MOTOR_MAX=100000;
#endif
	for(unsigned int i=0; i<NumOutputs; ++i) {
		float v;
#ifdef TGT_HAS_WHEELS
		if(i>=WheelOffset && i<WheelOffset+NumWheels) {
			float scale=outputRanges[i][MaxRange]*3; // assuming symmetric range!
			v = qstate.motor.motorVelocities[i-WheelOffset]/MOTOR_MAX*scale;
		} else
#endif
#ifdef TGT_HAS_LEDS
		if(i>=LEDOffset && i<LEDOffset+NumLEDs) {
			v = qb->ledStatus[i-LEDOffset];
		} else
#endif
		{
			unsigned int servoIdx=i;
#ifdef TGT_HAS_WHEELS
			if(servoIdx>WheelOffset)
				servoIdx-=NumWheels;
#endif
#ifdef TGT_HAS_LEDS
			if(servoIdx>LEDOffset)
				servoIdx-=NumLEDs;
#endif
			const float range = outputRanges[i][MaxRange]-outputRanges[i][MinRange];
			v = static_cast<float>(qstate.servo.servoPositions[servoIdx])/SERVO_MAX*range+outputRanges[i][MinRange];
		}
		state << " " << v;
	}		
	state << "\n";
	
	state << "sensors =";
	state << " " << qstate.battery.batteryVoltage;
	state << " " << qstate.analogIn.analogInValues[0];
	state << " " << qstate.analogIn.analogInValues[1];
	state << " " << qstate.analogIn.analogInValues[2];
	state << " " << qstate.analogIn.analogInValues[3];
	state << "\n";
	
	/*state << "weights =";
	for(unsigned int i=0; i<NumOutputs; ++i)
		state << " 0";
	state << '\n';*/
	
	state << "#END\n";
	
	states[cstate] = state.str();
	
	size = states[cstate].size();
	//std::cout << "state\n" << state.str() << std::endl;
	name = "(none-sensor)";
	
	//cout << states[cstate] << endl;
	payload = const_cast<char *>(states[cstate].c_str());
	
	cstate = !cstate;
	
	return number;
}


void DataCache::setDataSourceThread(LoadDataThread* p) {
	if(thread==NULL && p!=NULL) {
		// just starting to be used, announce what feedback we provide
		for(unsigned int i=0; i<NumOutputs; i++)
			providingOutput(i);
	} else if(thread!=NULL && p==NULL) {
		// going offline, cancel our announcement
	  for(unsigned int i=0; i<NumOutputs; i++)
		  ignoringOutput(i);
	}
	DataSource::setDataSourceThread(p);
	//if(p!=NULL)
	//  setSource(p->src);
}

#endif
