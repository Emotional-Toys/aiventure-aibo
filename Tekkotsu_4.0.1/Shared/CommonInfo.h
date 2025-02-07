//-*-c++-*-
#ifndef INCLUDED_CommonInfo_h_
#define INCLUDED_CommonInfo_h_

#include <map>
#include <set>
#include <string>
#include <stdexcept>

namespace RobotInfo {

	//! Defines the indexes to use as indices to access the min and max entries of joint limit specifications (e.g. ERS7Info::outputRanges and ERS7Info::mechanicalLimits)
	enum MinMaxRange_t { MinRange,MaxRange };
	
	//! Some target models, such as ERS2xxInfo, may be dual-booting compatability modes.  This function returns the actual robot name (e.g. ERS210Info::TargetName or ERS220Info::TargetName)
	/*! This function should return the actual RobotInfo::TargetName and not a hard coded string.
	 *  This way, we can rely on testing equality with a direct pointer comparison instead of strcmp().
	 *  (Also eliminates chance of typo or missing refactorization if renamed!).
	 *
	 *  The result of this function is stored in #RobotName, so you don't need to call
	 *  this function -- the only reason it's declared in the header is so you can call
	 *  it during static initialization, when you can't rely on RobotName having been
	 *  initialized yet. */
	const char* const detectModel();
	
	//! Name of the robot which is actually running
	/*! This is usually set to the TargetName, but if the target model is a "compatability" target,
	 *  where the actual host hardware may be a different (more restrictive) configuration,
	 *  then RobotName will be set to the TargetName of that configuration.
	 *
	 *  Note that you should be able to rely on doing pointer comparisons
	 *  between RobotName and various TargetNames to test for different robot models,
	 *  instead of using strcmp() for each. 
	 *
	 *  However, a std::string is used on Aperios to transparently trigger the strcmp because
	 *  of the way the process model is handled there screws up the pointer comparison
	 *  (a different process does the static initialization, so we get a pointer relative to its
	 *  memory space instead of the one we're executing in.  Unix-based platforms don't
	 *  have this problem by using a "real" fork() operation.) */
#ifndef PLATFORM_APERIOS
	extern const char* const RobotName;
#else // have to use a string because aperios is annoying like that
	extern const std::string RobotName;
#endif
	
	//! Allows behaviors to lookup output/button/sensor names from other models to support basic cross-model portability
	/*! Use the getCapabilities() function to look up the Capabalities instance for a given model based on its string robot name */
	class Capabilities {
		friend Capabilities* getCapabilities(const std::string& robName);
	public:
		//! constructor, pass the robot name this is regarding, and outputs, buttons, and sensor names
		Capabilities(const char* robName, size_t numOut, const char * const outNames[], size_t numBut, const char * const butNames[], size_t numSen, const char * const senNames[], size_t pidOff, size_t numPID, size_t ledOff, size_t numLED);
		//! shallow copy (explicit to satisfy warning)
		Capabilities(const Capabilities& cap)
			: name(cap.name), numOutputs(cap.numOutputs), numButtons(cap.numButtons), numSensors(cap.numSensors),
			outputs(cap.outputs), buttons(cap.buttons), sensors(cap.sensors),
			outputToIndex(cap.outputToIndex), buttonToIndex(cap.buttonToIndex), sensorToIndex(cap.sensorToIndex),
			pidJointOffset(cap.pidJointOffset), numPIDJoints(cap.numPIDJoints), ledOffset(cap.ledOffset), numLEDs(cap.numLEDs),
			fakeOutputs() {}
		//! shallow assign (explicit to satisfy warning)
		Capabilities& operator=(const Capabilities& cap) {
			name=cap.name;
			numOutputs=cap.numOutputs; numButtons=cap.numButtons; numSensors=cap.numSensors;
			outputs=cap.outputs; buttons=cap.buttons; sensors=cap.sensors;
			outputToIndex=cap.outputToIndex; buttonToIndex=cap.buttonToIndex; sensorToIndex=cap.sensorToIndex;
			pidJointOffset=cap.pidJointOffset; numPIDJoints=cap.numPIDJoints; ledOffset=cap.ledOffset; numLEDs=cap.numLEDs;
			fakeOutputs=cap.fakeOutputs;
			return *this;
		}
		//! destructor, explicit just to avoid warning when used as base class
		virtual ~Capabilities() {}
		
		//! returns the name of the robot this corresponds to
		inline const char * getRobotName() const { return name; }
		
		//! returns the number of unique outputs (i.e. not counting aliases)
		inline unsigned int getNumOutputs() const { return numOutputs; }
		//! returns the number of unique buttons (i.e. not counting aliases)
		inline unsigned int getNumButtons() const { return numButtons; }
		//! returns the number of unique sensors (i.e. not counting aliases)
		inline unsigned int getNumSensors() const { return numSensors; }
		
		//! look up the name corresponding to an offset, returns NULL if not found/available
		inline const char * getOutputName(unsigned int i) const { return i<numOutputs ? outputs[i] : NULL; }
		//! look up the name corresponding to an offset, returns NULL if not found/available
		inline const char * getButtonName(unsigned int i) const { return i<numButtons ? buttons[i] : NULL; }
		//! look up the name corresponding to an offset, returns NULL if not found/available
		inline const char * getSensorName(unsigned int i) const { return i<numSensors ? sensors[i] : NULL; }
		
		//! Look up the offset corresponding to a output name, throws std::invalid_argument if not found
		/*! Identical to findOutputOffset(), except throws an exception instead of returning an invalid value.
		 *  Use this if you expect that the output is available, and want a noisy fail-fast if something's wrong (e.g. typo in name?) */
		inline unsigned int getOutputOffset(const char* out) const { return lookupT("output",outputToIndex,out); }
		//! look up the offset corresponding to a button name, throws std::invalid_argument if not found
		/*! Identical to findButtonOffset(), except throws an exception instead of returning an invalid value.
		 *  Use this if you expect that the output is available, and want a noisy fail-fast if something's wrong (e.g. typo in name?) */
		inline unsigned int getButtonOffset(const char* but) const { return lookupT("button",buttonToIndex,but); }
		//! look up the offset corresponding to a sensor name, throws std::invalid_argument if not found
		/*! Identical to findSensorOffset(), except throws an exception instead of returning an invalid value.
		 *  Use this if you expect that the output is available, and want a noisy fail-fast if something's wrong (e.g. typo in name?) */
		inline unsigned int getSensorOffset(const char* sen) const { return lookupT("sensor",sensorToIndex,sen); }
		
		//! look up the offset corresponding to a output name, returns -1U if not found/available
		/*! Identical to getOutputOffset(), except returns an invalid value instead of throwing an exception.
		 *  Use this if you are testing to see if a capability exists, and don't want to incur exception handling if it isn't (say you're doing a lot of testing) */ 
		inline unsigned int findOutputOffset(const char* out) const { return lookup(outputToIndex,out); }
		//! look up the offset corresponding to a button name, returns -1U if not found/available
		/*! Identical to getButtonOffset(), except returns an invalid value instead of throwing an exception.
		 *  Use this if you are testing to see if a capability exists, and don't want to incur exception handling if it isn't (say you're doing a lot of testing) */ 
		inline unsigned int findButtonOffset(const char* but) const { return lookup(buttonToIndex,but); }
		//! look up the offset corresponding to a sensor name, returns -1U if not found/available
		/*! Identical to getSensorOffset(), except returns an invalid value instead of throwing an exception.
		 *  Use this if you are testing to see if a capability exists, and don't want to incur exception handling if it isn't (say you're doing a lot of testing) */ 
		inline unsigned int findSensorOffset(const char* sen) const { return lookup(sensorToIndex,sen); }
		
		inline unsigned int getPIDJointOffset() const { return pidJointOffset; } //!< returns the offset of the block of 'PID' joints in an output array
		inline unsigned int getNumPIDJoints() const { return numPIDJoints; } //!< returns the number of 'PID' joints
		inline unsigned int getLEDOffset() const { return ledOffset; } //!< returns the offset of the block of LEDs in an output array
		inline unsigned int getNumLEDs() const { return numLEDs; } //!< returns the number of LEDs
		
		//! returns the offsets of "fake" outputs, see #fakeOutputs
		inline const std::set<unsigned int>& getFakeOutputs() const { return fakeOutputs; }
		
	protected:
		//! helper function, does the work of the get..Offset functions
		inline unsigned int lookupT(const char * errStr, const std::map<std::string,unsigned int>& nameToIndex, const char * capname) const {
			std::map<std::string,unsigned int>::const_iterator it=nameToIndex.find(capname);
			if(it==nameToIndex.end()) {
				std::string str; str.append(name).append("::capabilities could not find ").append(errStr).append(" named ").append(capname);
				throw std::invalid_argument(str);
			}
			return it->second;
		}
		//! helper function, does the work of the find..Offset functions
		inline unsigned int lookup(const std::map<std::string,unsigned int>& nameToIndex, const char * capname) const {
			std::map<std::string,unsigned int>::const_iterator it=nameToIndex.find(capname);
			return it==nameToIndex.end() ? -1U : it->second;
		}
		
		const char* name; //!< name of robot model
		size_t numOutputs; //!< length of #outputs
		size_t numButtons; //!< length of #buttons
		size_t numSensors; //!< length of #sensors
		const char * const * outputs; //!< array of names for outputs -- this is the "primary" name for each output, #outputToIndex may contain additional aliases
		const char * const * buttons; //!< array of names for buttons -- this is the "primary" name for each button, #buttonToIndex may contain additional aliases
		const char * const * sensors; //!< array of names for sensors -- this is the "primary" name for each sensor, #sensorToIndex may contain additional aliases
		std::map<std::string,unsigned int> outputToIndex; //!< maps output names to offset values
		std::map<std::string,unsigned int> buttonToIndex; //!< maps button names to offset values
		std::map<std::string,unsigned int> sensorToIndex; //!< maps sensor names to offset values
		
		size_t pidJointOffset; //!< the offset of the PID joints
		size_t numPIDJoints; //!< the number of PID joints
		size_t ledOffset; //!< the offset of the LEDs
		size_t numLEDs; //!< the number of LEDs
		
		//! Offsets of "fake" outputs, which don't correspond to any physical device on the robot
		/*! This is used in compatability modes, where some outputs may not be available on the
		 *  host hardware, or for meta-outputs, which control the interpretation of other outputs.
		 *  (such as the A/B LED mode setting for the ERS-7, where a "virtual" LED switches
		 *  the system's intepretation of the face panel LEDs).
		 *
		 *  Most robots can probably just leave this empty -- on Aperios the "fake" outputs are
		 *  skipped when interfacing with the system and their values receive feedback from
		 *  the motion process.  When using the tekkotsu executable under unix-based systems,
		 *  the HAL layer handles this functionality via its own configuration settings, and these
		 *  values are ignored. */
		std::set<unsigned int> fakeOutputs;
		
		//! returns a static map from robot names to capability instances, which are externally allocated
		/*! The Capabilties base class will automatically insert entries into this collection. */
		static std::map<std::string, class Capabilities*>& getCaps();
	};

}

/*! @file
 * @brief 
 * @author ejt (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.7 $
 * $State: Exp $
 * $Date: 2007/11/09 19:01:13 $
 */

#endif
