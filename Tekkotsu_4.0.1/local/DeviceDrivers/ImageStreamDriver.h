//-*-c++-*-
#ifndef INCLUDED_ImageStreamDriver_h_
#define INCLUDED_ImageStreamDriver_h_

#include "local/DeviceDriver.h"
#include "local/DataSource.h"
#include "local/CommPort.h"
#include "Shared/plist.h"
#include <iostream>

//! description of ImageStreamDriver
class ImageStreamDriver : public virtual DeviceDriver, public DataSource, public virtual plist::PrimitiveListener {
public:
	explicit ImageStreamDriver(const std::string& name)
	: DeviceDriver(autoRegisterImageStreamDriver,name), DataSource(),
	commName(), format(FORMAT_JPEG,formatNames), frameNumber(0), buffer(), img(NULL), imgSize(0)
	{
		format.addNameForVal("jpg",FORMAT_JPEG);
		addEntry("CommPort",commName,"The name of the comm port from which data will be read.");
		addEntry("Format",format,"The type of format to expect from the comm port.\n"
				 "'YUV' expects interleaved components 'CameraResolutionX' wide and 'CameraResolutionY' high\n"
				 "(defined in target's RobotInfo namespace)");
		format.addPrimitiveListener(this);
	}
	virtual ~ImageStreamDriver() {
		format.removePrimitiveListener(this);
	}

	virtual std::string getClassName() const { return autoRegisterImageStreamDriver; }

	virtual void getSensorSources(std::map<std::string,DataSource*>& sources) {
		sources.clear();
		//sources["Sensors"]=sensors;
	}
	virtual void getImageSources(std::map<std::string,DataSource*>& sources) {
		sources.clear();
		sources["Camera"]=this;
	}
	
	virtual unsigned int nextTimestamp();
	virtual const std::string& nextName() { return instanceName; }
	virtual unsigned int getData(const char *& payload, unsigned int& payloadSize, unsigned int& timestamp, std::string& name);
	virtual void setDataSourceThread(LoadDataThread* th);

	virtual void plistValueChanged(const plist::PrimitiveBase& pl);

	plist::Primitive<std::string> commName;
	
	enum format_t {
		FORMAT_YUV=0,
		FORMAT_PNG,
		FORMAT_JPEG,
		FORMAT_TEKKOTSU
	};
	static const size_t NUM_FORMATS = 4;
	static const char * formatNames[NUM_FORMATS+1];
	plist::NamedEnumeration<format_t> format;

protected:
	void copyImage(char * buf, unsigned int width, unsigned int height, unsigned int channels, const char * chan, unsigned int lwidth, unsigned int lheight, unsigned int c);
	
	//BufferSource sensors; //!< separate DataSource for sensor frames, if embedded in image stream
	unsigned int frameNumber; //!< image frame number
	std::vector<char> buffer; //!< image buffer
	char * img; //!< temporary storage for jpeg/png decompression
	size_t imgSize; //!< size of allocation of #img

private:
	//! holds the class name, set via registration with the DeviceDriver registry
	static const std::string autoRegisterImageStreamDriver;
	ImageStreamDriver(const ImageStreamDriver&); //!< no call
	ImageStreamDriver operator=(const ImageStreamDriver&); //!< no call
};

/*! @file
 * @brief 
 * @author Ethan Tira-Thompson (ejt) (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.5.2.1 $
 * $State: Exp $
 * $Date: 2009/02/08 03:34:43 $
 */

#endif
