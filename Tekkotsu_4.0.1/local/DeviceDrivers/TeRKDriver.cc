#ifdef HAVE_ICE
#include <iostream>
#include <sstream>
#include <unistd.h>
#include "TeRKDriver.h"

using namespace std; 

const std::string TeRKDriver::autoRegisterTeRKDriver = DeviceDriver::getRegistry().registerType<TeRKDriver>("TeRK");

//! this array defines the values for TeRKDriver::TeRKProperties::defaults
pair<string,string> TeRKDriver::TeRKProperties::defaultValues[NUM_DEFAULT_VALUES] = {
	make_pair( "Ice.Package.peer", "edu.cmu.ri.mrpl" ),
	make_pair( "Ice.Package.TeRK", "edu.cmu.ri.mrpl" ),
	make_pair( "Ice.Default.Package", "edu.cmu.ri.mrpl" ),
	
	make_pair( "TerkClient.Client.Endpoints", "tcp" ),
	
	// Active connection management must be disabled when using bidirectional connections.
	make_pair( "Ice.ACM.Client", "0" ),
	make_pair( "Ice.ACM.Server", "0" ),
	
	// Print warning messages for certain exceptional conditions in connections
	make_pair( "Ice.Warn.Connections", "1" ),
	
	make_pair( "Ice.Logger.Timestamp", "1" ),
	
	make_pair( "Ice.ThreadPool.Client.Size", "5" ),
	make_pair( "Ice.ThreadPool.Client.SizeMax", "20" ),
	make_pair( "Ice.ThreadPool.Server.Size", "5" ),
	make_pair( "Ice.ThreadPool.Server.SizeMax", "20" ),
	
	// protocol and port to use for direct connect
	make_pair( "TeRK.direct-connect.protocol", "tcp" ),
	make_pair( "TeRK.direct-connect.port", "10101" ),
};

void TeRKDriver::motionUpdated(const std::vector<size_t>& changedIndices, const float outputs[][NumOutputs]) {
	if(qwerk==NULL)
		return;
	
	std::set<size_t> updatedIndices(changedIndices.begin(),changedIndices.end());
	
#ifdef TGT_HAS_LEDS
	for(unsigned int i=LEDOffset; i<LEDOffset+NumLEDs; ++i) {
		float v = outputs[NumFrames-1][i];
		if(v>0 && v<1) // intermediate value, need to flicker activation
			updatedIndices.insert(i);
	}
#endif
	
	if(updatedIndices.size()==0)
		return;
	try {
		//cout << "Got motor command. " << endl;
		//QwerkState qstate = qwerk->requestStateFrame();
		//cout << "Getting state packet with: " << qstate.motor.motorVelocities.size() << " " << qstate.motor.motorVelocities[0] << " " << qstate.motor.motorVelocities[1] << " " << qstate.motor.motorVelocities[2] << " " << qstate.motor.motorVelocities[3] << endl;
		//cout << "Setting motor to: " << outputs[NumFrames-1][0] << " " << outputs[NumFrames-1][1] << endl;
		//cout << "Setting servo to: " << outputs[NumFrames-1][2] << " " << outputs[NumFrames-1][3] << endl;
		
#ifdef TGT_HAS_WHEELS
		const int MOTOR_MAX=100000;
		::TeRK::MotorCommand mc;
		mc.motorVelocities.assign(NumWheels,0);
		mc.motorPositions.assign(NumWheels,0);
		mc.motorAccelerations.assign(NumWheels,1000000);
		mc.motorMask.assign(NumWheels,false); // will set to true for the motors being updated
		mc.motorModes.assign(NumWheels,::TeRK::MotorSpeedControl);
#endif
		
#ifdef TGT_HAS_LEDS
		::TeRK::LEDCommand ledc;
		ledc.ledMask.assign(NumLEDs,false);
		ledc.ledModes.assign(NumLEDs,::TeRK::LEDOff);
#endif
		
		const int SERVO_MAX=255;
		const int NUM_SERVOS = NumOutputs-NumWheels-NumLEDs;
		::TeRK::ServoCommand sc;
		sc.servoPositions.assign(NUM_SERVOS,0);
		sc.servoSpeeds.assign(NUM_SERVOS,INT_MAX);
		sc.servoMask.assign(NUM_SERVOS,false);
		sc.servoModes.assign(NUM_SERVOS,::TeRK::ServoMotorPositionControl);
		
		for(std::set<size_t>::const_iterator it=updatedIndices.begin(); it!=updatedIndices.end(); ++it) {
#ifdef TGT_HAS_WHEELS
			if(WheelOffset<=*it && *it<WheelOffset+NumWheels) {
				const unsigned int motorIdx=(*it-WheelOffset);
				float scale=outputRanges[*it][MaxRange]*3; // assuming symmetric range!
				mc.motorVelocities[motorIdx]=static_cast<int>(MOTOR_MAX*outputs[NumFrames-1][*it]/scale);
				mc.motorMask[motorIdx]=true;
				cout << "Setting " << outputNames[*it] << " (motor " << motorIdx << ") to: " << outputs[NumFrames-1][*it] << " (" << mc.motorVelocities[motorIdx] << ")" << endl;
			} else
#endif
#ifdef TGT_HAS_LEDS
			if(LEDOffset<=*it && *it<LEDOffset+NumLEDs){
				const unsigned int ledIdx = (*it-LEDOffset);
				ledc.ledMask[ledIdx] = true;
				ledc.ledModes[ledIdx] = calcLEDValue(ledIdx,outputs[NumFrames-1][*it]);
			} else
#endif
			{
				// if it's not a wheel or LED, treat it as a servo
				unsigned int servoIdx=*it;
#ifdef TGT_HAS_WHEELS
				if(servoIdx>WheelOffset)
					servoIdx-=NumWheels;
#endif
#ifdef TGT_HAS_LEDS
				if(servoIdx>LEDOffset)
					servoIdx-=NumLEDs;
#endif
				const float range = outputRanges[*it][MaxRange]-outputRanges[*it][MinRange];
				sc.servoPositions[servoIdx]=(int)(SERVO_MAX*(outputs[NumFrames-1][*it]-outputRanges[*it][MinRange])/range);
				//int cur=(SERVO_MAX*(lastOutputs[*it]-outputRanges[*it][MinRange])/range);
				//sc.servoSpeeds[servoIdx]=abs(sc.servoPositions[servoIdx]-cur)*1000/(FrameTime*NumFrames);
				sc.servoMask[servoIdx]=true;
				cout << "Setting " << outputNames[*it] << " (servo " << servoIdx << ") to: " << outputs[NumFrames-1][*it] << " (" << sc.servoPositions[servoIdx] << ")" << endl;
			}
		}
		
		// push the state to the qwerk
#ifdef TGT_HAS_WHEELS
		qwerk->motorControl->execute(mc);
#endif
#ifdef TGT_HAS_LEDS
		qwerk->ledControl->execute(ledc);
#endif
		qwerk->servoControl->execute(sc);
	} catch(...) {
		std::cerr << "TerkMotionHook::motionCheck caught exception -- communication lost?" << std::endl;
		close();

		while(qwerk==NULL){
			std::cerr << "Reconnecting in 10 seconds..." << std::endl;
			sleep(10);
			this->connect();
		}
	}
}


void TeRKDriver::plistValueChanged(const plist::PrimitiveBase& pl) {
	this->connect();
}

void TeRKDriver::connect(){
	bool wasConnected=(qwerk!=NULL);
	close();

	if(host.size()!=0) {
		string port = properties->getProperty("TeRK.direct-connect.port");
		try {
			qwerk = connectToPeer();
		} catch(const Ice::Exception& e) {
			std::cerr << "TeRKDriver threw exception '" << e << "' when connecting to " << host << " on port " << port << std::endl;
		} catch(const std::string& e) {
			std::cerr << "TeRKDriver threw exception string '" << e << "' when connecting to " << host << " on port " << port << std::endl;
		} catch(const char* e) {
			std::cerr << "TeRKDriver threw exception cstring '" << e << "' when connecting to " << host << " on port " << port << std::endl;
		} catch(...) {
			std::cerr << "TeRKDriver threw unknown exception when connecting to " << host << " on port " << port << std::endl;
		}
		if(qwerk==NULL) {
			std::cerr << "TeRKDriver could not connect to " << host << " on port " << port << std::endl;
			close();
			return;
		}
	}
	
	bool isConnected=(qwerk!=NULL);
	if(wasConnected!=isConnected) // if we changed connection status...
		fireDataSourcesUpdated(); // ...either new data sources are available, or old ones are gone
}

QwerkBot* TeRKDriver::connectToPeer() {
  if(ic)
    ic->destroy();
  Ice::InitializationData iceData;
  iceData.properties = properties;
  ic = Ice::initialize(iceData);


  ostringstream uuidSS;
  uuidSS << "direct_connect_client|";
  uuidSS << IceUtil::generateUUID();
  uuidSS << "|" << idcount;
  uuid = uuidSS.str();
  idcount++;

	string port = properties->getProperty("TeRK.direct-connect.port");
	cout << "Connecting to " << host << ":" << port << endl;
	
	::Ice::Identity peerIdentity;
	peerIdentity.name = "::TeRK::TerkUser";
	peerIdentity.category = "";
	
	::Ice::ObjectPrx objectPrx = getPeerProxy(peerIdentity);
	cout << "objectPrx: " << objectPrx << endl;
	::TeRK::TerkUserPrx peerProxy = ::TeRK::TerkUserPrx::checkedCast(objectPrx);
	
	::Ice::ObjectAdapterPtr adapter = ic->createObjectAdapter("TerkClient.Client");

	peerProxy->ice_getConnection()->setAdapter(adapter);
	
	::Ice::Identity callbackReceiverIdent;
	callbackReceiverIdent.name = "terkCallbackReceiver";
	callbackReceiverIdent.category = "";
	
	TerkUserI* terkUserI = new TerkUserI();
	::TeRK::TerkClientPrx qwerkServantPrx = ::TeRK::TerkClientPrx::checkedCast(adapter->add(terkUserI, callbackReceiverIdent));
	cout << "qwerkServantPrx: " << qwerkServantPrx << endl;
	
	adapter->activate();
	
	// Clean up old ping thread if exists
	if(ping) ping->destroy();
	// Starting ping thread - to make sure connection is still alive
	ping = new ObjectPingThread(peerProxy);
	ping->start();
	
	// Validate it is a qwerk proxy 
	::TeRK::QwerkPrx peer = ::TeRK::QwerkPrx::checkedCast(peerProxy);
	cout << "Casting to qwerkPrx: " << peer << endl;
	
	// now tell the bot that we're connected.
	peerProxy->peerConnected(uuid, ::peer::AccessLevelOwner, qwerkServantPrx);
	
	cout << "Told bot I exist..." << endl;
	QwerkBot *qb = new QwerkBot(this, peerProxy, peer);
	cout << "Created QwerkBot" << endl;
	terkUserI->setImageCache(qb->imageCache);
	cout << "Set image cache to bot" << endl;
	if(qb->video){
		printf("Starting Video...\n");
		// first time we connect to "fake qwerk" throws an internal Ice exception on startVideoStream()
		// this is apparently supposed to indicate you should try again, so we do.
		// (this whole issue may be an Ice bug that we're even seeing the exception)
		const unsigned int TRIES=5;
		for(unsigned int i=0; i<TRIES; i++) {
			try {
				qb->video->startVideoStream();
				break; // no exception? leave retry loop...
			} catch(const Ice::Exception& e) {
				std::cerr << "TeRKDriver threw exception '" << e << "' connecting to video on " << host << " port " << port << " (attempt " << i+1 << " of " << TRIES << ")" << std::endl;
			} catch(const std::string& e) {
				std::cerr << "TeRKDriver threw exception string '" << e << "' connecting to video on " << host << " port " << port << " (attempt " << i+1 << " of " << TRIES << ")" << std::endl;
			} catch(const char* e) {
				std::cerr << "TeRKDriver threw exception cstring '" << e << "' connecting to video on " << host << " port " << port << " (attempt " << i+1 << " of " << TRIES << ")" << std::endl;
			} catch(...) {
				std::cerr << "TeRKDriver threw unknown exception connecting to video on " << host << " port " << port << " (attempt " << i+1 << " of " << TRIES << ")" << std::endl;
			}
			usleep(10000);
		}
	}
	return qb;
}

Ice::ObjectPrx TeRKDriver::getPeerProxy(Ice::Identity proxyIdentity) const {
	// doesn't work... ?
	//Ice::ObjectPrx objectPrx = peerProxy->ice_getConnection()->getAdapter()->createProxy(proxyIdentity);
	
	// instead:
	string port = properties->getProperty("TeRK.direct-connect.port");
	string protocol = properties->getProperty("TeRK.direct-connect.protocol");
	// I can't find a way to create proxies (attempt above) without using this "build a stringified command line" interface.  Sigh.
	string proxyString = "'" + ic->identityToString(proxyIdentity) + "':" + protocol + " -h " + host + " -p " + port;
	Ice::ObjectPrx objectPrx = ic->stringToProxy(proxyString);

	// now define a custom context for interacting with this proxy
	::Ice::Context context;
	// not sure this first property is really necessary -- for now it's not being used.
	// if it could be taken out, we could set the other two as the "defaultContext" and not have to make each context unique.
	context["__peerProxyIdentity"] = ic->identityToString(objectPrx->ice_getIdentity()); // e.g. "::TeRK::TerkUser"
	context["__peerUserId"] = uuid;
	context["__isDirectConnect"] = "true";
		
	return objectPrx->ice_context(context); // get a new proxy with the specified context
}



// CONFIGURATION STUFF
// TeRKDriver::TeRKProperties gives an Ice interface to the tekkotsu configuration system (plist)

::std::string TeRKDriver::TeRKProperties::getPropertyWithDefault(const ::std::string& key, const ::std::string& def) {
	const_iterator dit = findEntry(key);
	if(dit!=end())
		return dit->second->toString();
	::Ice::PropertyDict::const_iterator it = defaults.find(key);
	return it==defaults.end() ? def : it->second;
}

::Ice::Int TeRKDriver::TeRKProperties::getPropertyAsIntWithDefault(const ::std::string& key, ::Ice::Int def) {
	const_iterator dit = findEntry(key);
	if(dit!=end())
		return dit->second->toLong();
	::Ice::PropertyDict::const_iterator it = defaults.find(key);
	return it==defaults.end() ? def : atoi(it->second.c_str());
}

::Ice::PropertyDict TeRKDriver::TeRKProperties::getPropertiesForPrefix(const ::std::string& prefix) {
	::Ice::PropertyDict ans;
	for(const_iterator it=begin(); it!=end(); ++it)
		if(it->first.compare(0, prefix.size(), prefix) == 0)
			ans.insert(make_pair(it->first,it->second->toString()));
	// remember that map::insert ignores the call if it already exists...
	// so these defaults below won't override explicitly set values which have just been added
	for(::Ice::PropertyDict::const_iterator it=defaults.begin(); it!=defaults.end(); ++it)
		if(it->first.compare(0, prefix.size(), prefix) == 0)
			ans.insert(*it);
	return ans;
}

void TeRKDriver::TeRKProperties::load(const ::std::string& filename) {
	if(!loadFile(filename.c_str())) {
		::Ice::FileException ex(__FILE__, __LINE__);
		ex.path = filename;
		ex.error = getSystemErrno();
		throw ex;
	}
}

/*! @file
 * @brief 
 * @author Ethan Tira-Thompson (ejt) (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.12 $
 * $State: Exp $
 * $Date: 2007/11/11 23:57:29 $
 */
#endif
