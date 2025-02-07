#ifndef QWERKBOT
#define QWERKBOT

#include "Ice/Application.h"
#include "Ice/IdentityUtil.h"
#include "IceUtil/UUID.h"

#include <unistd.h>
#include <map>
#include <iostream>
#include <fstream>

#include "TeRKPeerCommon.h"
#include "peer/MRPLPeer.h"
#include "SerialIO.h"

#include "PropertyManagerI.h"
#include "TerkUserI.h"
#include "ImageCache.h"
#include "DataCache.h"

#include "SerialPort.h"

class TeRKDriver;

class QwerkBot {
public:
	QwerkBot(TeRKDriver *client, ::TeRK::TerkUserPrx peerProxy, ::TeRK::QwerkPrx prx);
	~QwerkBot();
	
	::TeRK::QwerkState requestStateFrame();
	bool setConfig(::TeRK::SerialIOConfig config);
	
	// various controllers
	::TeRK::MotorControllerPrx motorControl;
	::TeRK::ServoControllerPrx servoControl;
	::TeRK::AnalogInControllerPrx analogControl;
	::TeRK::DigitalInControllerPrx digitalInControl;
	::TeRK::DigitalOutControllerPrx digitalOutControl;
	::TeRK::LEDControllerPrx ledControl;
	::TeRK::SerialIOServicePrx serialIO;
	::TeRK::VideoStreamerServerPrx video;
	
	::TeRK::LEDModeArray ledStatus;
	void executeLED(::TeRK::LEDCommand ledc);
	
	ImageCache* imageCache;
	DataCache* dataCache;

	QwerkSerialPort* serialPort;
private:
	TeRKDriver *client;
	
	::TeRK::TerkUserPrx peerProxy;
	::TeRK::ProxyTypeIdToIdentityMap serviceMap;
	::TeRK::QwerkState qstate;
	::TeRK::QwerkPrx prx;
	TerkUserI terkUserI;
	
	::TeRK::SerialIOConfig serialConfig;
	
	QwerkBot(const QwerkBot&); //!< do not call
	QwerkBot& operator=(const QwerkBot&); //!< do not call
};


#endif
