#include "RobotInfo.h"
#include <iostream>

#if defined(TGT_ERS2xx) && defined(PLATFORM_APERIOS)
#  include <OPENR/OPENRAPI.h>
#endif

// collecting these static allocations here so we don't have to have a separate file for each one
// you can either make a .cc file dedicated to your Info.h, or just add an entry below...

#include "ERS210Info.h"
namespace ERS210Info {
	const char* const TargetName="ERS-210";
	ERS210Capabilities capabilities;
}

#include "ERS220Info.h"
namespace ERS220Info {
	const char* const TargetName="ERS-220";
	ERS220Capabilities capabilities;
}

#include "ERS2xxInfo.h"
namespace ERS2xxInfo {
	const char* const TargetName="ERS-2xx";
	ERS2xxCapabilities capabilities;
}

#include "ERS7Info.h"
namespace ERS7Info {
	const char* const TargetName="ERS-7";
	ERS7Capabilities capabilities;
}

#include "LynxArm6Info.h"
namespace LynxArm6Info {
	const char* const TargetName="LynxArm6";
	Capabilities capabilities(TargetName,NumOutputs,outputNames,NumButtons,buttonNames,NumSensors,sensorNames,PIDJointOffset,NumPIDJoints,0,0);
}

#include "Regis1Info.h"
namespace Regis1Info {
	const char* const TargetName="Regis1";
	Regis1Capabilities capabilities;
}

#include "QBotPlusInfo.h"
namespace QBotPlusInfo {
	const char* const TargetName="QBotPlus";
	QBotPlusCapabilities capabilities;
}

#include "QwerkInfo.h"
namespace QwerkInfo {
	const char* const TargetName="Qwerk";
	Capabilities capabilities(TargetName,NumOutputs,outputNames,NumButtons,buttonNames,NumSensors,sensorNames,PIDJointOffset,NumPIDJoints,LEDOffset,NumLEDs);
}

#include "CreateInfo.h"
namespace CreateInfo {
	const char* const TargetName="Create";
	Capabilities capabilities(TargetName,NumOutputs,outputNames,NumButtons,buttonNames,NumSensors,sensorNames,PIDJointOffset,NumPIDJoints,LEDOffset,NumLEDs);
}


// and now for RobotInfo's own stuff:
namespace RobotInfo {
	
	const char* const detectModel() {
#ifdef TGT_ERS2xx
#  ifdef PLATFORM_APERIOS
		// might be running on either 210 or 220, check
		char robotDesignStr[orobotdesignNAME_MAX + 1];
		memset(robotDesignStr, 0, sizeof(robotDesignStr));
		if (OPENR::GetRobotDesign(robotDesignStr) != oSUCCESS) {
			std::cout << "OPENR::GetRobotDesign() failed." << std::endl;
			return TargetName;
		} else {
			if(strcmp(robotDesignStr,"ERS-210")==0)
				return ERS210Info::TargetName;
			else if(strcmp(robotDesignStr,"ERS-220")==0)
				return ERS220Info::TargetName;
			else {
				std::cerr << "ERROR: Unknown name '" << robotDesignStr << "' for target ERS2xx" << std::endl;
				return TargetName;
			}
		}
#  else
#    warning TGT_ERS2xx assuming ERS-210 for simulation on local platform
		return ERS210Info::TargetName;
#  endif
		
#else
		// target is directly the robot, return the target name
		return TargetName;
#endif
	}
	
#ifndef PLATFORM_APERIOS
	const char* const RobotName = detectModel();
#else // have to use a string because aperios is annoying like that
	const std::string RobotName = detectModel();
#endif
	
	
	Capabilities::Capabilities(const char* robName, size_t numOut, const char * const outNames[], size_t numBut, const char * const butNames[], size_t numSen, const char * const senNames[], size_t pidOff, size_t numPID, size_t ledOff, size_t numLED)
		: name(robName), numOutputs(numOut), numButtons(numBut), numSensors(numSen),
		outputs(outNames), buttons(butNames), sensors(senNames),
		outputToIndex(), buttonToIndex(), sensorToIndex(),
		pidJointOffset(pidOff), numPIDJoints(numPID), ledOffset(ledOff), numLEDs(numLED),
		fakeOutputs()
	{
		for(size_t i=0; i<numOutputs; ++i)
			outputToIndex[outputs[i]]=i;
		for(size_t i=0; i<numButtons; ++i)
			buttonToIndex[buttons[i]]=i;
		for(size_t i=0; i<numSensors; ++i)
			sensorToIndex[sensors[i]]=i;
		
		std::map<std::string, class Capabilities*>::const_iterator it=getCaps().find(robName);
		if(it!=getCaps().end())
			std::cerr << "WARNING: RobotInfo '" << robName << "' capabilities has already been registered!  Name conflict?  Replacing previous..." << std::endl;
		getCaps()[robName]=this;
	}

	std::map<std::string, class Capabilities*>& Capabilities::getCaps() {
		static std::map<std::string, class Capabilities*> caps;
		return caps;
	}
	
}
