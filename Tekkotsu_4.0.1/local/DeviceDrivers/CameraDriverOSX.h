//-*-c++-*-
#ifndef INCLUDED_CameraDriver_h_
#define INCLUDED_CameraDriver_h_

#include "local/DeviceDriver.h"
#include "local/DataSources/CameraSourceOSX.h"
#include "Shared/get_time.h"

//! description of CameraDriver
class CameraDriver : public virtual DeviceDriver, public virtual plist::CollectionListener {
public:
	//! constructor, may throw a const char* on error
	explicit CameraDriver(const std::string& name)
		: DeviceDriver(autoRegisterCameraDriver,name)
	{
		if(!checkQTThreadInit()) {
			std::cerr << "CameraDriver: Couldn't initialize QuickTime" << std::endl;
			return;
		}
		
		updateCameraList();
	}
	
	~CameraDriver() {}
	
	virtual std::string getClassName() const { return autoRegisterCameraDriver; }
	
	virtual void getImageSources(std::map<std::string,DataSource*>& sources) {
		sources.clear();
		for(const_iterator it=begin(); it!=end(); ++it) {
			if(it->first==".type")
				continue;
			if(CameraSource * ds = dynamic_cast<CameraSource*>(it->second))
				sources[it->first]=ds;
			else
				std::cerr << "WARNING CameraDriver entry " << it->first << " is not actually a CameraSource!!!" << std::endl;
		}
	}
	
protected:
	//! converts from pascal-format string to c-format string
	static std::string p2c(unsigned char pascalStr[]) {
		unsigned char len = *pascalStr++;
		return std::string(reinterpret_cast<char*>(pascalStr),len);
	}
	
	static void dumpLiteral(OSType t);
	
	void updateCameraList();
	
private:
	//! holds the class name, set via registration with the DeviceDriver registry
	static const std::string autoRegisterCameraDriver;
};

/*! @file
 * @brief 
 * @author Ethan Tira-Thompson (ejt) (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Date: 2007/09/14 21:29:03 $
 */

#endif
