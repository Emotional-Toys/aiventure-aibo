#ifdef HAVE_ICE

#include "Shared/RobotInfo.h"
#include "QwerkBot.h"
#include "local/DeviceDrivers/TeRKDriver.h"
#include "Shared/get_time.h"
#include <unistd.h>

#include "SerialPort.h"

QwerkBot::QwerkBot(TeRKDriver *cl, ::TeRK::TerkUserPrx peerPrx, ::TeRK::QwerkPrx proxy): 
	motorControl(NULL), servoControl(NULL), analogControl(NULL), digitalInControl(NULL),
	digitalOutControl(NULL), ledControl(NULL), serialIO(NULL), video(NULL),
	ledStatus(), imageCache(0), dataCache(0), serialPort(0), client(cl), peerProxy(), 
	serviceMap(proxy->getSupportedServices()), qstate(), prx(proxy), terkUserI(), serialConfig() {
  // not yet implemented
  //this->terkUserI.bindFrameEvent(this);

  ::TeRK::ProxyTypeIdToIdentityMap::iterator tmp;

  // now attempt to start the services
  tmp = this->serviceMap.find("::TeRK::MotorController");
  if(tmp != this->serviceMap.end())
    this->motorControl = ::TeRK::MotorControllerPrx::uncheckedCast(client->getPeerProxy(tmp->second));
  else
    this->motorControl = 0;
  
  tmp = this->serviceMap.find("::TeRK::ServoController");
  if(tmp != this->serviceMap.end())
    this->servoControl = ::TeRK::ServoControllerPrx::uncheckedCast(client->getPeerProxy(tmp->second));
  else
    this->servoControl = 0;

  tmp = this->serviceMap.find("::TeRK::AnalogInController");
  if(tmp != this->serviceMap.end())
    this->analogControl = ::TeRK::AnalogInControllerPrx::uncheckedCast(client->getPeerProxy(tmp->second));
  else
    this->analogControl = 0;
  
  tmp = this->serviceMap.find("::TeRK::DigitalInController");
  if(tmp != this->serviceMap.end())
    this->digitalInControl = ::TeRK::DigitalInControllerPrx::uncheckedCast(client->getPeerProxy(tmp->second));
  else
    this->digitalInControl = 0;
  
  tmp = this->serviceMap.find("::TeRK::DigitalOutController");
  if(tmp != this->serviceMap.end())
    this->digitalOutControl = ::TeRK::DigitalOutControllerPrx::uncheckedCast(client->getPeerProxy(tmp->second));
  else
    this->digitalOutControl = 0;

  tmp = this->serviceMap.find("::TeRK::LEDController");
  if(tmp != this->serviceMap.end())
    this->ledControl = ::TeRK::LEDControllerPrx::uncheckedCast(client->getPeerProxy(tmp->second));
  else
    this->ledControl = 0;

  // Setup Video Stream
  tmp = this->serviceMap.find("::TeRK::VideoStreamerServer");
  std::cout << "Video? " << (tmp != this->serviceMap.end()) << std::endl;
  if(tmp != this->serviceMap.end()){
    this->video = ::TeRK::VideoStreamerServerPrx::uncheckedCast(client->getPeerProxy(tmp->second));
    this->imageCache = new ImageCache();
  }
  else{
    this->video = 0;
    this->imageCache = 0;
  }
/*
  for(tmp = this->serviceMap.begin(); tmp != this->serviceMap.end(); tmp++){
	  cout << tmp->first << endl;
  }
*/

	// Setup SerialIO
	tmp = this->serviceMap.find("::TeRK::SerialIOService");
	if(tmp != this->serviceMap.end()){
		this->serialIO = ::TeRK::SerialIOServicePrx::uncheckedCast(client->getPeerProxy(tmp->second));

		// configure serial
		serialConfig.portName = "/dev/ttyAM1";
		serialConfig.baud = ::TeRK::BAUD57600;
		serialConfig.characterSize = ::TeRK::CHARSIZE8;
		serialConfig.parity = ::TeRK::PARITYNONE;
		serialConfig.stopBits = ::TeRK::STOPBITS1;
		serialConfig.flowControl = ::TeRK::FLOWCONTROLNONE;
		// open the serial port
		this->serialIO->close(serialConfig.portName);
		this->serialIO->open(serialConfig);
		this->serialPort = new QwerkSerialPort(serialConfig.portName, serialIO);
	} else{
		this->serialIO = 0;
		this->serialPort = 0;
	}

	ledStatus.assign(NumLEDs,::TeRK::LEDOff);

	dataCache = new DataCache(this);
}


bool QwerkBot::setConfig(::TeRK::SerialIOConfig newconfig){
	if(this->serialIO){
		this->serialIO->close(newconfig.portName);
		this->serialIO->open(newconfig);
		serialConfig = newconfig;
		return true;
	}
	return false;
}

::TeRK::QwerkState QwerkBot::requestStateFrame(){
	qstate = prx->getState();
	return qstate;
}

QwerkBot::~QwerkBot(){
  
}

void QwerkBot::executeLED(::TeRK::LEDCommand ledc){
	for(unsigned int i = 0; i < NumLEDs; i++){
		if(ledc.ledMask[i])
			ledStatus[i] = ledStatus[i];
	}
	this->ledControl->execute(ledc);
}

#endif
