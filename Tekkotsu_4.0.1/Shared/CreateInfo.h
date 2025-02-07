//-*-c++-*-
#ifndef INCLUDED_CreateInfo_h
#define INCLUDED_CreateInfo_h

#include <cmath>
#include <stdlib.h>
#include "CommonInfo.h"
using namespace RobotInfo;

// see http://tekkotsu.org/porting.html#configuration for more information on TGT_HAS_* flags
#if defined(TGT_CREATE)
#  define TGT_HAS_WHEELS 2
#  define TGT_HAS_LEDS 2
#endif

//! Contains information about an iRobot Create, such as number of joints, timing information, etc.
namespace CreateInfo {
	
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
	const unsigned NumHeadJoints  =  0; //!< The number of joints in the neck
	const unsigned NumTailJoints  =  0; //!< The number of joints assigned to the tail
	const unsigned NumMouthJoints =  0; //!< the number of joints that control the mouth
	const unsigned NumEarJoints   =  0; //!< The number of joints which control the ears (NOT per ear, is total)
	const unsigned NumButtons     =  0; //!< the number of buttons that are available, 2 head, 4 paws, 3 back, 1 underbelly see ERS7Info::ButtonOffset_t
	const unsigned NumSensors     =  18;  //!< the number of sensors available
	const unsigned NumLEDs        =  2; //!< The number of LEDs which can be controlled
	const unsigned NumFacePanelLEDs = 0; //!< The number of face panel LEDs
	
	const unsigned NumPIDJoints   = 0; //!< servo pins
	const unsigned NumOutputs     = NumWheels + NumLEDs; //!< the total number of outputs
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

	const unsigned LEDOffset   = PIDJointOffset + NumPIDJoints; //!< the offset of LEDs in WorldState::outputs and MotionCommand functions, see LedOffset_t for specific offsets
	
	const unsigned MODE_OFFSET = LEDOffset + NumLEDs;
	const unsigned DEMO_OFFSET = MODE_OFFSET+1;

	const unsigned BaseFrameOffset   = NumOutputs; //!< Use with kinematics to refer to base reference frame

	enum WheelOffset_t {
		LWheelOffset=WheelOffset,
		RWheelOffset
	};
	
	//! The offsets of the individual LEDs on the head and tail.  Note that left/right are robot's point of view.  See also LEDBitMask_t
	/*! @hideinitializer */
	enum LEDOffset_t {
	};
	
	typedef unsigned int LEDBitMask_t; //!< So you can be clear when you're refering to a LED bitmask
	//! LEDs for the face panel (all FaceLEDPanelMask<<(0:NumFacePanelLEDs-1) entries)
	const LEDBitMask_t FaceLEDMask = 0;
	//! selects all of the leds
	const LEDBitMask_t AllLEDMask  = (LEDBitMask_t)~0;
	//@}

	enum InterfaceMode_t {
		MODE_SAFE,
		MODE_FULL,
		MODE_PASSIVE,
		MODE_OFF,
	};

	// *******************************
	//          INPUT OFFSETS
	// *******************************


	//! The order in which inputs should be stored
	//!@name Input Offsets

	//! holds offsets to different buttons in WorldState::buttons[]
	/*! Should be a straight mapping to the ButtonSourceIDs
	 *
	 *  @see WorldState::buttons
	 * @hideinitializer */
	enum ButtonOffset_t {
		PLAY_OFFSET, //!< 1 if play button is down
		ADVANCE_OFFSET, //!< 1 if advance button is down
		WALL_OFFSET, //!< 1 if wall is detected (note correspondence to WALL_SIGNAL_OFFSET's value, avoiding problems if the two are swapped)
		DROP_CASTER_OFFSET, //!< 1 if caster detects dropoff
		DROP_LEFT_WHEEL_OFFSET, //!< 1 if left wheel detects dropoff
		DROP_RIGHT_WHEEL_OFFSET, //!< 1 if right wheel detects dropoff
		BUMP_LEFT_OFFSET, //!< 1 if left bumper is pressed
		BUMP_RIGHT_OFFSET, //!< 1 if right bumper is pressed
		OVERCURRENT_LEFT_WHEEL_OFFSET, //!< 1 if the left wheel is drawing more than 1 amp
		OVERCURRENT_RIGHT_WHEEL_OFFSET, //!< 1 if the right wheel is drawing more than 1 amp
		LOW_SIDE_DRIVER_0_OFFSET, //!< 1 if low side driver 0 is pulling more than 0.5 amps
		LOW_SIDE_DRIVER_1_OFFSET, //!< 1 if low side driver 1 is pulling more than 0.5 amps
		LOW_SIDE_DRIVER_2_OFFSET, //!< 1 if low side driver 2 is pulling more than 1.6 amps
		BASE_CHARGER_OFFSET, //!< 1 if the home base charger is available
		INTERNAL_CHARGER_OFFSET //!< 1 if the internal charger is available
	};

	//! Provides a string name for each button
	const char* const buttonNames[NumButtons+1] = { NULL }; // non-empty array to avoid gcc 3.4.2 internal error

	//! holds offset to different sensor values in WorldState::sensors[]
	/*! @see WorldState::sensors[] */
	enum SensorOffset_t { 
		DIGITAL0_INPUTS_OFFSET, //!< the digital input pins in bits 0 through 4
		DIGITAL1_INPUTS_OFFSET, //!< the digital input pins in bits 0 through 4
		DIGITAL2_INPUTS_OFFSET, //!< the digital input pins in bits 0 through 4
		DIGITAL3_INPUTS_OFFSET, //!< the digital input pins in bits 0 through 4
		ANALOG_SIGNAL_OFFSET, //!< voltage on cargo bay pin 4
		WALL_SIGNAL_OFFSET, //!< strength of the wall sensor's signal (note correspondence to WALL_OFFSET's value, avoid problems if the two are swapped)
		IR_COMM_OFFSET, //!< value received by the infrared communication receiver, see IRComm_t for values sent by standard hardware
		CLIFF_LEFT_SIGNAL_OFFSET, //!< strength of the left cliff sensor
		CLIFF_FRONT_LEFT_SIGNAL_OFFSET, //!< strength of the front left cliff sensor
		CLIFF_FRONT_RIGHT_SIGNAL_OFFSET, //!< strength of the front right cliff sensor
		CLIFF_RIGHT_SIGNAL_OFFSET, //!< strength of the right cliff sensor
		ENCODER_DISTANCE_OFFSET, //!< average distance (mm) traveled by the wheels since last update
		ENCODER_ANGLE_OFFSET, //!< average angle (radians) rotated since the last update
		VOLTAGE_OFFSET, //!< mV measured at battery
		CURRENT_OFFSET, //!< mA flowing into battery (negative when discharging)
		BATTERY_CHARGE_OFFSET, //!< mAh remaining in battery (may not be accurate with alkaline battery pack)
		BATTERY_TEMP_OFFSET, //!< degrees celsius
		CHARGING_STATE_OFFSET //!< one of #ChargingState_t
	};
	
	enum IRComm_t {
		IR_REMOTE_LEFT=129,
		IR_REMOTE_FORWARD,
		IR_REMOTE_RIGHT,
		IR_REMOTE_SPOT,
		IR_REMOTE_MAX,
		IR_REMOTE_SMALL,
		IR_REMOTE_MEDIUM,
		IR_REMOTE_LARGE,
		IR_REMOTE_PAUSE,
		IR_REMOTE_POWER,
		IR_REMOTE_ARC_LEFT, 
		IR_REMOTE_ARC_RIGHT,
		IR_REMOTE_STOP,
		IR_REMOTE_SEND,
		IR_REMOTE_DOCK,
		IR_BASE_RED=248,
		IR_BASE_GREEN=244,
		IR_BASE_FORCE=242,
		IR_BASE_RED_GREEN=252,
		IR_BASE_RED_FORCE=250,
		IR_BASE_GREEN_FORCE=246,
		IR_BASE_RED_GREEN_FORCE=254
	};
	/*const unsigned IR_BASE_MASK=240;
	const unsigned IR_BASE_RED_MASK=8;
	const unsigned IR_BASE_GREEN_MASK=4;
	const unsigned IR_BASE_FORCE_MASK=2;*/
	
	enum ChargingState_t {
		CHARGING_OFF,
		CHARGING_RECONDITIONING,
		CHARGING_FULL,
		CHARGING_TRICKLE,
		CHARGING_WAITING,
		CHARGING_FAULT
	};
		
	//! Provides a string name for each sensor
	const char* const sensorNames[NumSensors] = { 
		"DigitalIn0",
		"DigitalIn1",
		"DigitalIn2",
		"DigitalIn3",
		"AnalogIn",
		"WallSignal",
		"IR",
		"CliffLeftSignal",
		"CliffFrontLeftSignal",
		"CliffFrontRightSignal",
		"CliffRight",
		"Distance",
		"Angle",
		"BatteryVoltage",
		"BatteryCurrent",
		"BatteryCharge",
		"BatteryTemp",
		"ChargingState",
	}; // non-empty array to avoid gcc 3.4.2 internal error

	//@}


	//! Names for each of the outputs
	const char* const outputNames[NumOutputs] = {
		"WHEEL:L",
		"WHEEL:R",
		"LED:00000",
		"LED:00001",
	};
	
	//! allocation declared in RobotInfo.cc
	extern Capabilities capabilities;
	
	//! This table holds the default PID values for each joint.  see PIDMC
	const float DefaultPIDs[NumPIDJoints+1][3] = {
		{0,0,0} // extra value to avoid error in older versions of gcc (doesn't like empty array
	};
	
	//!These values are our recommended maximum joint velocities, in rad/ms
	/*! a value <= 0 means infinite speed (e.g. LEDs)
	 *  
	 *  These limits are <b>not</b> enforced by the framework.  They are simply available for you to use as you see fit.
	 *  HeadPointerMC and PostureMC are primary examples of included classes which do respect these values (although they can be overridden)
	 *  
	 *  These values were obtained from the administrators of the Sony OPEN-R BBS */
	const float MaxOutputSpeed[NumOutputs] = {
		1<<8,
		1<<8,
		1<<8,
		1<<8,
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
		};

	//! This table holds the mechanical limits of each of the outputs, first index is the output offset, second index is MinMaxRange_t (i.e. MinRange or MaxRange)
	/*! Same as #outputRanges, don't know actual values because they were never specified by Sony */
	const double mechanicalLimits[NumOutputs][2] =
		{
			{ -1 , 1 },
			{ -1 , 1 },
			{ RAD(-90) , RAD(90) },
			{ RAD(-90) , RAD(90) },
		};

#ifdef __RI_RAD_FLAG
#undef RAD
#undef __RI_RAD_FLAG
#endif
}

#endif
