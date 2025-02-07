//-*-c++-*-
#ifndef INCLUDED_QBotPlusInfo_h
#define INCLUDED_QBotPlusInfo_h

#include <cmath>
#include <stdlib.h>
#include "CommonInfo.h"
using namespace RobotInfo;

// see http://tekkotsu.org/porting.html#configuration for more information on TGT_HAS_* flags
#if defined(TGT_QBOTPLUS)
#  define TGT_IS_QWERK
#  define TGT_HAS_WHEELS 2
#  define TGT_HAS_LEDS 10
#  define TGT_HAS_CAMERA 1
#  define TGT_HAS_HEAD 1
#endif

namespace QBotPlusInfo {

	// *******************************
	//       ROBOT CONFIGURATION
	// *******************************

	extern const char* const TargetName; //!< the name of the model, to be used for logging and remote GUIs

	const unsigned int FrameTime=15;        //!< time between frames in the motion system (milliseconds)
	const unsigned int NumFrames=1;        //!< the number of frames per buffer (don't forget also double buffered)
	const unsigned int SoundBufferTime=32; //!< the number of milliseconds per sound buffer... I'm not sure if this can be changed
	
	//!@name Output Types Information
	const unsigned NumWheels      =  2;
	
	const unsigned JointsPerArm   =  0;
	const unsigned NumArms        =  0;
	const unsigned NumArmJoints   =  JointsPerArm*NumArms;
	
	const unsigned JointsPerLeg   =  0; //!< The number of joints per leg
	const unsigned NumLegs        =  0; //!< The number of legs
	const unsigned NumLegJoints   =  JointsPerLeg*NumLegs; //!< the TOTAL number of joints on ALL legs
	const unsigned NumHeadJoints  =  2; //!< The number of joints in the neck
	const unsigned NumTailJoints  =  0; //!< The number of joints assigned to the tail
	const unsigned NumMouthJoints =  0; //!< the number of joints that control the mouth
	const unsigned NumEarJoints   =  0; //!< The number of joints which control the ears (NOT per ear, is total)
	const unsigned NumButtons     =  0; //!< the number of buttons that are available, 2 head, 4 paws, 3 back, 1 underbelly see ERS7Info::ButtonOffset_t
	const unsigned NumSensors     =  5;  //!< the number of sensors available
	const unsigned NumLEDs        =  10; //!< The number of LEDs which can be controlled
	const unsigned NumFacePanelLEDs = 0; //!< The number of face panel LEDs
	
	const unsigned NumPIDJoints   = NumWheels + NumArmJoints + NumLegJoints+NumHeadJoints+NumTailJoints+NumMouthJoints; //!< The number of joints which use PID motion - everything except ears
	const unsigned NumOutputs     = NumPIDJoints + NumLEDs; //!< the total number of outputs
	const unsigned NumReferenceFrames = NumOutputs + 1 + NumArms + 1; //!< for the base, gripper (* NumArms), and camera reference frames

	// webcam ?
	const float CameraHorizFOV=56.9/180*M_PI; //!< horizontal field of view (radians)
	const float CameraVertFOV=45.2/180*M_PI; //!< vertical field of view (radians)
	const float CameraFOV=CameraHorizFOV; //!< should be set to maximum of #CameraHorizFOV or #CameraVertFOV
	const unsigned int CameraResolutionX=320; //!< the number of pixels available in the 'full' layer
	const unsigned int CameraResolutionY=240; //!< the number of pixels available in the 'full' layer
	//@}


	// *******************************
	//         OUTPUT OFFSETS
	// *******************************

	//!Corresponds to entries in ERS7Info::PrimitiveName, defined at the end of this file
	//!@name Output Offsets

	
	const unsigned PIDJointOffset = 0; //!< The beginning of the PID Joints
	const unsigned WheelOffset = PIDJointOffset;
	const unsigned HeadOffset  = WheelOffset+NumWheels;   //!< the offset of the beginning of the head joints, add TPROffset_t to get specific joint

	const unsigned LEDOffset   = PIDJointOffset + NumPIDJoints; //!< the offset of LEDs in WorldState::outputs and MotionCommand functions, see LedOffset_t for specific offsets

	const unsigned BaseFrameOffset   = NumOutputs; //!< Use with kinematics to refer to base reference frame
	const unsigned CameraFrameOffset = BaseFrameOffset + 1; //!< Use with kinematics to refer to camera reference frame

	//! The offsets of appendages with pan (heading), tilt (elevation), note that this should be added to HeadOffset, otherwise use HeadOffset_t (#HeadPanOffset and #HeadTiltOffset)
	enum TPROffset_t {
		PanOffset = 0,      //!< pan/heading (horizontal)
		TiltOffset, //!< tilt/elevation (vertical)
		NodOffset = TiltOffset //!< replicated tilt (could be left undefined instead...)
	};
	
	//! These are 'absolute' offsets for the neck joints, don't need to add to HeadOffset like TPROffset_t values do
	enum HeadOffset_t {
		HeadPanOffset = HeadOffset,      //!< pan/heading (horizontal)
		HeadTiltOffset, //!< tilt/elevation (vertical)
	};
	
	//! 'Absolute' offsets for each of the wheels
	enum WheelOffset_t {
		LWheelOffset=WheelOffset,
		RWheelOffset
	};
		
	//! The offsets of the individual LEDs -- except the qwerk board doesn't have any particularly symbolic LEDs, just use numeric values...
	/*! @hideinitializer */
	enum LEDOffset_t { };
	
	typedef unsigned int LEDBitMask_t; //!< So you can be clear when you're refering to a LED bitmask
	//! LEDs for the face panel (all FaceLEDPanelMask<<(0:NumFacePanelLEDs-1) entries)
	const LEDBitMask_t FaceLEDMask = 0;
	//! selects all of the leds
	const LEDBitMask_t AllLEDMask  = (LEDBitMask_t)~0;
	//@}


	// *******************************
	//          INPUT OFFSETS
	// *******************************


	//! The order in which inputs should be stored
	//!@name Input Offsets

	//! holds offsets to different buttons in WorldState::buttons[]
	/*! Should be a straight mapping to the ButtonSourceIDs
	 *
	 *  Note that the chest (power) button is not a normal button.  It kills
	 *  power to the motors at a hardware level, and isn't sensed in the
	 *  normal way.  If you want to know when it is pressed (and you are
	 *  about to shut down) see PowerSrcID::PauseSID.
	 *
	 *  @see WorldState::buttons @see ButtonSourceID_t
	 * @hideinitializer */
	enum ButtonOffset_t { };

	//! Provides a string name for each button
	const char* const buttonNames[NumButtons+1] = { NULL }; // non-empty array to avoid gcc 3.4.2 internal error

	//! holds offset to different sensor values in WorldState::sensors[]
	/*! @see WorldState::sensors[] */
	enum SensorOffset_t { BatteryVoltage };

	//! Provides a string name for each sensor
	const char* const sensorNames[NumSensors] = { 
	  "BatteryVoltage",
	  "analogInValues0",
	  "analogInValues1",
	  "analogInValues2",
	  "analogInValues3"
	};

	//@}


	//! Names for each of the outputs
	const char* const outputNames[NumOutputs] = {
		"WHEEL:L",
		"WHEEL:R",
		"NECK:pan",
		"NECK:nod",
		"LED:00000",
		"LED:00001",
		"LED:00002",
		"LED:00003",
		"LED:00004",
		"LED:00005",
		"LED:00006",
		"LED:00007",
		"LED:00008",
		"LED:00009"
	};
	
	//! provides polymorphic robot capability detection/mapping
	class QBotPlusCapabilities : public Capabilities {
	public:
		//! constructor
		QBotPlusCapabilities()
		: Capabilities(TargetName,NumOutputs,outputNames,NumButtons,buttonNames,NumSensors,sensorNames,PIDJointOffset,NumPIDJoints,LEDOffset,NumLEDs)
		{
			// alias neck tilt to "nod"
			outputToIndex["NECK:tilt"]=HeadTiltOffset;
		}
	};
	//! allocation declared in RobotInfo.cc
	extern QBotPlusCapabilities capabilities;
	
	//! This table holds the default PID values for each joint.  see PIDMC
	const float DefaultPIDs[NumPIDJoints][3] = {
		{1,0,0},
		{1,0,0},
		{1,0,0},
		{1,0,0}
	};
	
	//!These values are our recommended maximum joint velocities, in rad/ms
	/*! a value <= 0 means infinite speed (e.g. LEDs)
	 *  
	 *  These limits are <b>not</b> enforced by the framework.  They are simply available for you to use as you see fit.
	 *  HeadPointerMC and PostureMC are primary examples of included classes which do respect these values (although they can be overridden)
	 *  
	 *  These values were obtained from the administrators of the Sony OPEN-R BBS */
	const float MaxOutputSpeed[NumOutputs] = {
		1e-3, //Wheels: left, right
		1e-3,
		3.14e-3, // Neck: tilt, pan
		6.28e-3,

		0,0,0,0,0,0,0,0,0,0 // LED
	};

	#ifndef RAD
		//!Just a little macro for converting degrees to radians
	#define RAD(deg) (((deg) * M_PI ) / 180.0)
		//!a flag so we undef these after we're done - do you have a cleaner solution?
	#define __RI_RAD_FLAG
	#endif
	
	//! This table holds the software limits of each of the outputs, first index is the output offset, second index is MinMaxRange_t (i.e. MinRange or MaxRange)
	const double outputRanges[NumOutputs][2] =
		{
			{ -1 , 1 },
			{ -1 , 1 },
			{ RAD(-90) , RAD(90) },
			{ RAD(-90) , RAD(90) },

			// LED
			{0,1}, {0,1}, {0,1}, {0,1}, {0,1},
			{0,1}, {0,1}, {0,1}, {0,1}, {0,1}
		};

	//! This table holds the mechanical limits of each of the outputs, first index is the output offset, second index is MinMaxRange_t (i.e. MinRange or MaxRange)
	/*! Same as #outputRanges, don't know actual values because they were never specified by Sony */
	const double mechanicalLimits[NumOutputs][2] =
		{
			{ -1 , 1 },
			{ -1 , 1 },
			{ RAD(-90) , RAD(90) },
			{ RAD(-90) , RAD(90) },

			// LED
			{0,1}, {0,1}, {0,1}, {0,1}, {0,1},
			{0,1}, {0,1}, {0,1}, {0,1}, {0,1}
		};

#ifdef __RI_RAD_FLAG
#undef RAD
#undef __RI_RAD_FLAG
#endif
}

#endif
