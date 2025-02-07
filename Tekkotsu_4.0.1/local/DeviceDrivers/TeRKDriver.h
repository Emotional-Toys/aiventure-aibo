//-*-c++-*-
#ifndef INCLUDED_TeRKDriver_h_
#define INCLUDED_TeRKDriver_h_

#include "local/DeviceDriver.h"
#include "local/MotionHook.h"

#include "local/terk/ObjectPingThread.h"
#include "local/terk/QwerkBot.h"

#include "IceUtil/UUID.h"
#include "Ice/ProxyHandle.h"
#include "Ice/Identity.h"

//! Provides access to the Telepresence Robotics Kit, which specifies an ICE (Internet Communications Engine) based protocol for robotics control
/*!
 *  TeRK information:http://www.terk.ri.cmu.edu/
 *  ICE information: http://www.zeroc.com/
 *
 *  To use this driver, you must install ICE, and cause the HAVE_ICE compiler flag to be set.
 *  This is most easily done by the Tekkotsu Environment.conf, set the ICE_ROOT parameter to
 *  the location of the Ice libraries.  This will automatically cause HAVE_ICE to be set by the
 *  Makefiles whenever ICE_ROOT is a valid filesystem path.
 */
class TeRKDriver : public virtual DeviceDriver, public MotionHook, public virtual plist::PrimitiveListener {
public:
	//! constructor
	explicit TeRKDriver(const std::string& name)
		: DeviceDriver(autoRegisterTeRKDriver,name), MotionHook(), 
		host(), uuid("direct_connect_client|"+IceUtil::generateUUID()), properties(), ic(), 
		  ping(), qwerk(NULL), idcount(0)
	{
		for(unsigned int i=0; i<NumLEDs; ++i)
			ledActivation[i]=0;
		TeRKProperties* tprop = new TeRKProperties;
		addEntry("Host",host,"Hostname of the robot");
		//addEntry("Port",port,"Port number of the robot's ICE server");
		addEntry("IceConfig",*tprop,"Settings for ICE protocol");
		Ice::InitializationData iceData;
		iceData.properties = properties = tprop;
		ic = Ice::initialize(iceData);
		host.addPrimitiveListener(this);
	}
	
	//! destructor
	virtual ~TeRKDriver() {
		host.removePrimitiveListener(this);
		close();
		if(ic)
			ic->destroy();
		// don't need to delete properties, smart pointer
	}

	virtual std::string getClassName() const { return autoRegisterTeRKDriver; }

	virtual MotionHook* getMotionSink() { return dynamic_cast<MotionHook*>(this); }
	virtual void getSensorSources(std::map<std::string,DataSource*>& sources) {
		sources.clear();
		if(qwerk!=NULL && qwerk->dataCache!=NULL)
			sources["Sensors"]=qwerk->dataCache;
	}
	virtual void getImageSources(std::map<std::string,DataSource*>& sources) {
		sources.clear();
		if(qwerk!=NULL && qwerk->imageCache!=NULL)
			sources["Camera"]=qwerk->imageCache;
	}
	
	virtual void motionStarting() {}
	virtual void motionUpdated(const std::vector<size_t>& changedIndices, const float outputs[][NumOutputs]);
	virtual void motionStopping() {}
	
	virtual void plistValueChanged(const plist::PrimitiveBase& pl);

	//! creates a proxy for a specified service
	/*! @todo import QwerkBot and make this protected?) */
	Ice::ObjectPrx getPeerProxy(Ice::Identity proxyIdentity) const;
	
	//! hostname of the qwerk we are connecting to.
	plist::Primitive<std::string> host;
	
protected:
	//! connect to qwerk
	virtual void connect();

	//! initializes a connection to #host
	QwerkBot* connectToPeer();
	
	//! closes the current proxies (reuses the communicator instance though)
	virtual void close() {
		if(ping){
			ping->destroy();
			ping=NULL;
		}
		delete qwerk;
		qwerk=NULL;
	}
	
	//! allows LEDs to flicker at various frequencies to emulate having linear brightness control instead of boolean control
	inline ::TeRK::LEDMode calcLEDValue(unsigned int i,float x) {
		if(x<=0.0) {
			ledActivation[i]*=.9; //decay activation... resets to keeps LEDs in sync, looks a little better
			return ::TeRK::LEDOff;
		} else if(x>=1.0) {
			return ::TeRK::LEDOn;
		} else {
			x*=x; // squared to "gamma correct" - we can see a single pulse better than a single flicker - after image and all that
			ledActivation[i]+=x;
			if(ledActivation[i]>=1.0) {
				ledActivation[i]-=1.0;
				return ::TeRK::LEDOn;
			} else {
				return ::TeRK::LEDOff;
			}
		}
	}
	float ledActivation[NumLEDs]; //!< used to track partial LED activation (see calcLEDValue
	
	//! unique identifier, should be different for every active connection
	/*! This will be constant for the lifetime of the TeRKDriver, as we only ever have one
	 *  active connection per driver instance.  (If more than one instance, each will have
	 *  its own uuid however!) */
	std::string uuid;
	
	//! TeRKProperties instance provides Ice configuration values, default values for which are in TeRKDriver.cc.
	/*! These aren't really user-configurable settings, but are exposed via a Tekkotsu plist entry. */
	Ice::PropertiesPtr properties;
	
	//! The core Ice object, which manages connections, proxies, adapters, etc.
	/*! We create one instance and reuse it over the life of the driver.
	 *  (In other words, we shouldn't need to delete/reallocate it every time we
	 *  switch hosts, although if that winds up being easier, we could certainly
	 *  do that instead) */
	Ice::CommunicatorPtr ic;
	
	//! tests to make sure the remote host is still alive
	/*! @todo do we do anything if the remote host goes down? */
	ObjectPingThreadPtr ping;
	
	//! storage class holds proxies for various TeRK services.
	/*! @todo should be imported and stored directly in driver? */
	QwerkBot* qwerk;
		
  int idcount;
	//! Grafts a Ice::Properties interface on a plist::Dictionary so we can use the Tekkotsu configuration system to control ICE
	/*! The constructor assigns various default values to #defaults - these will be used
	 * unless a value is found in the plist::Dictionary storage.  We keep these values
	 * separate because they shouldn't be written to persistent storage unless explicitly
	 * set.  (Default values may change in the future, could break things if the defaults
	 * were written to file... most of these values are not really configurable by end user) */
	class TeRKProperties : public virtual ::Ice::Properties, public virtual plist::Dictionary {
	public:
		typedef ::Ice::PropertiesPtr PointerType; //!< forwarding typedef for good form
		typedef plist::Dictionary::iterator iterator; //!< forwarding typedef for good form
		typedef plist::Dictionary::const_iterator const_iterator; //!< forwarding typedef for good form
		
		//! constructor, initialize #defaults from #defaultValues (C++0x will make this much more elegant!)
		TeRKProperties() : defaults(defaultValues, defaultValues+NUM_DEFAULT_VALUES) {}
		
		//! These provide the TeRK configuration parameters for ICE communication, unless overridden by a setting in the class itself (see class notes)
		/*! These may be changed from the initial #defaultValues settings if a corresponding
		 *  TeRKDriver setting (e.g. port) is modified.  This is to avoid user confusion with having
		 *  settings appear twice (once in the more human-readable driver settings, and again
		 *  in this dictionary.)  So settings stored in TeRKProperties itself is reserved for explicit
		 *  overrides from the user */
		::Ice::PropertyDict defaults;
		
		//! returns value associated with @a key, or empty string if not found
		virtual ::std::string getProperty(const ::std::string& key) { return getPropertyWithDefault(key,""); }
		//! returns value associated with @a key, or @a def if not found
		virtual ::std::string getPropertyWithDefault(const ::std::string& key, const ::std::string& def);
		//! returns value associated with @a key, or 0 if not found
		virtual ::Ice::Int getPropertyAsInt(const ::std::string& key) { return getPropertyAsIntWithDefault(key,0); }
		//! returns value associated with @a key, or @a def if not found
		virtual ::Ice::Int getPropertyAsIntWithDefault(const ::std::string& key, ::Ice::Int def);
		//! returns a sub-map, selecting keys which match the specified prefix
		virtual ::Ice::PropertyDict getPropertiesForPrefix(const ::std::string& prefix);
		//! assigns a new value to a @a key
		virtual void setProperty(const ::std::string& key, const ::std::string& val) { setEntry(key,new plist::Primitive<std::string>(val)); }
		//! returns an empty sequence
		virtual ::Ice::StringSeq getCommandLineOptions() { return ::Ice::StringSeq(); }
		//! returns the same sequence it is passed (doesn't handle parsing command line options, we're not using that interface)
		virtual ::Ice::StringSeq parseCommandLineOptions(const ::std::string&, const ::Ice::StringSeq& options) { return options; }
		//! returns the same sequence it is passed (doesn't handle parsing command line options, we're not using that interface)
		virtual ::Ice::StringSeq parseIceCommandLineOptions(const ::Ice::StringSeq& options) { return options; }
		//! tries to load the specified file name (assumes plist format, not the ICE format!)
		virtual void load(const ::std::string& filename);
		//! makes a copy of the current property list
		virtual ::Ice::PropertiesPtr clone() { return new TeRKProperties(*this); }
		virtual TeRKProperties* clone() const { return new TeRKProperties(*this); }
		
	protected:
		static const unsigned int NUM_DEFAULT_VALUES=14;
		static std::pair<std::string,std::string> defaultValues[NUM_DEFAULT_VALUES];
	};
	
private:
	//! holds the class name, set via registration with the DeviceDriver registry
	static const std::string autoRegisterTeRKDriver;

	TeRKDriver(const TeRKDriver&); //!< do not call
	TeRKDriver& operator=(const TeRKDriver&); //!< do not call
};

/*! @file
 * @brief 
 * @author Ethan Tira-Thompson (ejt) (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.13 $
 * $State: Exp $
 * $Date: 2007/11/11 23:57:29 $
 */

#endif
