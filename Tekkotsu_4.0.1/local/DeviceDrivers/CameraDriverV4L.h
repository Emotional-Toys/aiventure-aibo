//-*-c++-*-
#ifndef INCLUDED_CameraDriver_h_
#define INCLUDED_CameraDriver_h_

#include "local/DeviceDriver.h"
#include "local/DataSource.h"
#include "Shared/get_time.h"
#include "IPC/Thread.h"

#include <linux/types.h> 
#include <linux/videodev.h>

#include <map>
#include <vector>

//! description of CameraDriver
class CameraDriver : public virtual DeviceDriver, public virtual plist::PrimitiveListener, public DataSource {
public:
	explicit CameraDriver(const std::string& name)
		: DeviceDriver(autoRegisterCameraDriver,name), DataSource(),
			path("/dev/video0"), resolution(1), layer(0), autobright(true),
			camfd(-1), img_size(0), vid_caps(), vid_win(), vid_pic(), frameCount(0), isYUVMode(false), blocking(false),
			curBuf(), lastBuf(), tmpBuf(), lock()
	{
		addEntry("Path",path,"Path to the video device, e.g. /dev/video0");
		addEntry("Downsample",resolution,"The downsampling level, each increment cuts the image in half (0 is full-size)");
		addEntry("Layer",layer,"Controls the resolution layer at which the image should be processed.\n"
		          "0 indicates \"automatic\" mode (picks layer closest to image's resolution), positive numbers indicate the resolution layer directly.\n"
		          "Negative values are relative to the number of layers marked available by the vision setup, so that typically -1 would correspond to the \"double\" layer, and -2 would correspond to the \"full\" layer.");
		addEntry("Autobright",autobright,"If true, will automatically adjust camera gain");
	}
	
	~CameraDriver() {
		resolution.removePrimitiveListener(this);
		path.removePrimitiveListener(this);
	}
	
	virtual std::string getClassName() const { return autoRegisterCameraDriver; }
	
	virtual void getImageSources(std::map<std::string,DataSource*>& sources) {
		sources.clear(); sources.insert(std::make_pair("Camera",this));
	}

	virtual unsigned int nextTimestamp() { return get_time(); }
	virtual const std::string& nextName() { return instanceName; }
	virtual unsigned int getData(const char *& payload, unsigned int& payloadSize, unsigned int& timestamp, std::string& name);
	virtual void setDataSourceThread(LoadDataThread* th);

	//! watches #path, triggers a close() and re-open() if it changes
	virtual void plistValueChanged(const plist::PrimitiveBase& pl);

	plist::Primitive<std::string> path;
	plist::Primitive<unsigned int> resolution;
	plist::Primitive<int> layer; //!< Controls the resolution layer at which the image should be processed.\n 0 indicates "automatic" mode (picks layer closest to image's resolution), positive numbers indicate the resolution layer directly.\n Negative values are relative to the number of layers marked available by the vision setup, so that typically -1 would correspond to the "double" layer, and -2 would correspond to the "full" layer.
	plist::Primitive<bool> autobright;

protected:
	void close_cam();
	void open_cam();
	void get_cam_info();
	void set_cam_info();
	void set_resolution();
	void auto_bright();
	void interleave_yuv_down();
	void interleave_yuv_up();

	size_t getWidth() const { return (resolution==0 ? vid_win.width : vid_win.width/2); }
	size_t getHeight() const { return (resolution==0 ? vid_win.height : vid_win.height/2); }
	size_t getBufferSize() const { size_t a=vid_win.width*vid_win.height; return HEADER_SIZE+(resolution==0 ? a*3 : a*3/2); }

	static const size_t HEADER_SIZE;

	int camfd;
  size_t img_size;
  struct video_capability vid_caps;
  struct video_window vid_win;
  struct video_picture vid_pic;
  unsigned int frameCount;
	bool isYUVMode;
	bool blocking;

	std::vector<unsigned char> curBuf; //!< the buffer to write the image into
	std::vector<unsigned char> lastBuf; //!< the buffer we returned last time (might still be in use while writing #curBuf)
	std::vector<unsigned char> tmpBuf; //!< used during interleave_yuv_up(), swapped back and forth with #curBuf
	ThreadNS::Lock lock; // buffer/img_size lock so we can't change resolution while reading

	/*class GrabThread : public Thread {
	public:
		GrabThread(std::vector<unsigned char>& buf, int cameraFD) : Thread(), curBuf(buf), camfd(cameraFD), lock() {}
		ThreadNS::Lock& getLock() { return lock; }
		void setImageSize(size_t imageSize) { image_size = imageSize; }
	protected:
		std::vector<unsigned char>& curBuf;
		int camfd;
		ThreadNS::Lock lock;
		};*/

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
 * $Revision: 1.4 $
 * $State: Exp $
 * $Date: 2007/11/09 19:56:07 $
 */

#endif
