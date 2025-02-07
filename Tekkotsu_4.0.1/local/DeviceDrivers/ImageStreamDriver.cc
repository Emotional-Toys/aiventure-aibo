#include "ImageStreamDriver.h"
#include "Shared/get_time.h"
#include "Shared/MarkScope.h"
#include "Shared/RobotInfo.h"
#include "Shared/Config.h"
#include "Shared/ImageUtil.h"
#include "Shared/debuget.h"

using namespace std; 

const char * ImageStreamDriver::formatNames[NUM_FORMATS+1] = { "yuv", "png", "jpeg", "tekkotsu", "" };
//declare explicit instances of the NamedEnumerations we're using
//(cuts down on some pretty significant binary size / debugging symbol bloat)
INSTANTIATE_NAMEDENUMERATION_STATICS(ImageStreamDriver::format_t);

const std::string ImageStreamDriver::autoRegisterImageStreamDriver = DeviceDriver::getRegistry().registerType<ImageStreamDriver>("ImageStream");

unsigned int ImageStreamDriver::nextTimestamp() {
	CommPort * comm = CommPort::getRegistry().getInstance(commName);
	if(comm==NULL || !comm->isReadable())
		return -1U;
	return get_time();
}

unsigned int ImageStreamDriver::getData(const char *& payload, unsigned int& payloadSize, unsigned int& timestamp, std::string& name) {
	payload=NULL; payloadSize=0; // in case of error
	CommPort * comm = CommPort::getRegistry().getInstance(commName);
	if(comm==NULL || !comm->isReadable())
		return frameNumber;
	/*unsigned int t=get_time();
	if(timestamp>t)
		usleep(static_cast<unsigned int>((timestamp-t)*1000/(getTimeScale()>0?getTimeScale():1.f)));
	if(thread==NULL) // in case we shut down while waiting
		return frameNumber;*/

	unsigned int layer=0;
	unsigned int width=CameraResolutionX;
	unsigned int height=CameraResolutionY;
	unsigned int components=3;
	{
		MarkScope autolock(*comm);
		//std::ostream os(&comm->getWriteStreambuf());
		std::istream is(&comm->getReadStreambuf());
		switch(*format) {
			case FORMAT_YUV: {
				unsigned int remain = LoadSave::getSerializedSize<unsigned int>()*4+width*height*components;
				buffer.resize(remain);
				char * buf=&buffer.front();
				if(!LoadSave::encodeInc(layer,buf,remain)) return frameNumber;
				if(!LoadSave::encodeInc(width,buf,remain)) return frameNumber;
				if(!LoadSave::encodeInc(height,buf,remain)) return frameNumber;
				if(!LoadSave::encodeInc(components,buf,remain)) return frameNumber;
				is.read(buf,remain);
				++frameNumber;
			} break;
				
			case FORMAT_PNG: {
				size_t w,h,c;
				bool isFirst=(img==NULL);
				if(!image_util::decodePNG(is,w,h,c,img,imgSize)) {
					cerr << "ImageStreamDriver PNG decompression failed" << endl;
					return frameNumber;
				}
				width=w;
				height=h;
				components=c;
				
				ASSERT(imgSize==width*height*components,"image size doesn't match expected size");
				unsigned int remain = LoadSave::getSerializedSize<unsigned int>()*4+width*height*components;
				buffer.resize(remain);
				char * buf=&buffer.front();
				if(!LoadSave::encodeInc(layer,buf,remain)) return frameNumber;
				if(!LoadSave::encodeInc(width,buf,remain)) return frameNumber;
				if(!LoadSave::encodeInc(height,buf,remain)) return frameNumber;
				if(!LoadSave::encodeInc(components,buf,remain)) return frameNumber;
				if(isFirst) {
					memcpy(buf,img,imgSize);
					delete [] img;
					img=buf;
				}
				
				++frameNumber;
			} break;
				
			case FORMAT_JPEG: {
				size_t w,h,c;
				bool isFirst=(img==NULL);
				if(!image_util::decodeJPEG(is,w,h,c,img,imgSize)) {
					cerr << "ImageStreamDriver JPEG decompression failed" << endl;
					return frameNumber;
				}
				width=w;
				height=h;
				components=c;
				
				ASSERT(imgSize==width*height*components,"image size doesn't match expected size");
				unsigned int remain = LoadSave::getSerializedSize<unsigned int>()*4+width*height*components;
				buffer.resize(remain);
				char * buf=&buffer.front();
				if(!LoadSave::encodeInc(layer,buf,remain)) return frameNumber;
				if(!LoadSave::encodeInc(width,buf,remain)) return frameNumber;
				if(!LoadSave::encodeInc(height,buf,remain)) return frameNumber;
				if(!LoadSave::encodeInc(components,buf,remain)) return frameNumber;
				if(isFirst) {
					memcpy(buf,img,imgSize);
					delete [] img;
					img=buf;
				}
				
				++frameNumber;
			} break;
				
			case FORMAT_TEKKOTSU: {
				// ugh, LoadSave handles FILE* and char*, but not iostreams...
				// gonna be a little ugly :(
				
				// RAW CAM BEHAVIOR HEADER
				char tmp[256];
				unsigned int len;
				string fmt;
				is.read(tmp,LoadSave::getSerializedSize(len));
				LoadSave::decode(len,tmp,LoadSave::getSerializedSize(len));
				if(len!=13) {
					cerr << "Expecting Tekkotsu image format, but image header doesn't match! (len==" << len << ")" << endl;
					return frameNumber;
				}
				is.read(tmp,len+1);
				if(strncmp(tmp,"TekkotsuImage",len+1)!=0) {
					tmp[len+2]='\0';
					cerr << "Expecting Tekkotsu image format, but image header doesn't match! (" << tmp << ")" << endl;
					return frameNumber;
				}
				
				int encoding; //Config::vision_config::RawCamConfig::encoding_t
				is.read(tmp,LoadSave::getSerializedSize(encoding));
				LoadSave::decode(encoding,tmp,LoadSave::getSerializedSize(encoding));
				if(encoding==Config::vision_config::RawCamConfig::ENCODE_SINGLE_CHANNEL)
					components=1;
				int compression; //Config::vision_config::RawCamConfig::compression_t
				is.read(tmp,LoadSave::getSerializedSize(compression));
				LoadSave::decode(compression,tmp,LoadSave::getSerializedSize(compression));
				is.read(tmp,LoadSave::getSerializedSize(width));
				LoadSave::decode(width,tmp,LoadSave::getSerializedSize(width));
				is.read(tmp,LoadSave::getSerializedSize(height));
				LoadSave::decode(height,tmp,LoadSave::getSerializedSize(height));
				unsigned int remote_timestamp;
				is.read(tmp,LoadSave::getSerializedSize(remote_timestamp));
				LoadSave::decode(remote_timestamp,tmp,LoadSave::getSerializedSize(remote_timestamp));
				unsigned int fnum;
				is.read(tmp,LoadSave::getSerializedSize(fnum));
				LoadSave::decode(fnum,tmp,LoadSave::getSerializedSize(fnum));
				
				// have everything we need to set up output header...
				unsigned int remain = LoadSave::getSerializedSize<unsigned int>()*4+width*height*components;
				buffer.resize(remain);
				char * buf=&buffer.front();
				if(!LoadSave::encodeInc(layer,buf,remain)) return frameNumber;
				if(!LoadSave::encodeInc(width,buf,remain)) return frameNumber;
				if(!LoadSave::encodeInc(height,buf,remain)) return frameNumber;
				if(!LoadSave::encodeInc(components,buf,remain)) return frameNumber;
				
				for(unsigned int i=0; i<components; ++i) {
					// FILTER BANK GENERATOR HEADER
					is.read(tmp,LoadSave::getSerializedSize(len));
					LoadSave::decode(len,tmp,LoadSave::getSerializedSize(len));
					if(len!=8) {
						cerr << "Expecting FbkImage image format, but header doesn't match! (len==" << len << ")" << endl;
						return frameNumber;
					}
					is.read(tmp,len+1);
					if(strncmp(tmp,"FbkImage",len+1)!=0) {
						tmp[len+2]='\0';
						cerr << "Expecting FbkImage image format, but image header doesn't match! (" << tmp << ")" << endl;
						return frameNumber;
					}
					unsigned int lwidth;
					is.read(tmp,LoadSave::getSerializedSize(lwidth));
					LoadSave::decode(lwidth,tmp,LoadSave::getSerializedSize(lwidth));
					unsigned int lheight;
					is.read(tmp,LoadSave::getSerializedSize(lheight));
					LoadSave::decode(lheight,tmp,LoadSave::getSerializedSize(lheight));
					unsigned int lyr;
					is.read(tmp,LoadSave::getSerializedSize(lyr));
					LoadSave::decode(lyr,tmp,LoadSave::getSerializedSize(lyr));
					unsigned int chan_id;
					is.read(tmp,LoadSave::getSerializedSize(chan_id));
					LoadSave::decode(chan_id,tmp,LoadSave::getSerializedSize(chan_id));
					
					// GENERATOR SUBCLASS HEADER
					is.read(tmp,LoadSave::getSerializedSize(len));
					LoadSave::decode(len,tmp,LoadSave::getSerializedSize(len));
					is.read(tmp,len+1);
					if(strcmp(tmp,"blank")==0) {
						int useChan=(components==1)?i:chan_id;
						int off=useChan;
						for(unsigned int y=0; y<height; y++) {
							for(unsigned int x=0; x<width; x++) {
								buf[off]=128;
								off+=components;
							}
						}
					} else if(strcmp(tmp,"RawImage")==0) {
						int useChan=(components==1)?i:chan_id;
						vector<char> chan(lwidth*lheight);
						is.read(&chan.front(),chan.size());
						copyImage(buf,width,height,components,&chan.front(),lwidth,lheight,useChan);
						
					} else if(strcmp(tmp,"JPEGGrayscale")==0) {
						int useChan=(components==1)?i:chan_id;
						is.read(tmp,LoadSave::getSerializedSize(len));
						LoadSave::decode(len,tmp,LoadSave::getSerializedSize(len));
						vector<char> jpeg(len);
						is.read(&jpeg.front(),jpeg.size());
						size_t jwidth,jheight,jcomp;
						image_util::decodeJPEG(&jpeg.front(), jpeg.size(), jwidth, jheight, jcomp);
						if(jcomp!=1) {
							cerr << "Got color image where grayscale was expected" << endl;
							return frameNumber;
						}
						vector<char> chan(jwidth*jheight);
						size_t tsize=chan.size();
						char* tchan=&chan.front();
						if(!image_util::decodeJPEG(&jpeg.front(), jpeg.size(), jwidth, jheight, jcomp, tchan, tsize)) {
							cerr << "JPEG decompression failed" << endl;
							return frameNumber;
						}
						copyImage(buf,width,height,components,&chan.front(),lwidth,lheight,useChan);
						
					} else if(strcmp(tmp,"JPEGColor")==0) {
						is.read(tmp,LoadSave::getSerializedSize(len));
						LoadSave::decode(len,tmp,LoadSave::getSerializedSize(len));
						vector<char> jpeg(len);
						is.read(&jpeg.front(),jpeg.size());
						size_t jwidth,jheight,jcomp;
						size_t tsize=remain;
						if(!image_util::decodeJPEG(&jpeg.front(), jpeg.size(), jwidth, jheight, jcomp, buf, tsize)) {
							cerr << "JPEG decompression failed" << endl;
							return frameNumber;
						}
						i=components;
						
					} else {
						cerr << "Unknown image generator " << tmp << endl;
						return frameNumber;
					}
				}
				
				if(!is)
					return frameNumber;
				frameNumber=fnum;
			} break;
		}
	}
	payload = &buffer.front();
	payloadSize = buffer.size();
	timestamp=get_time();
	name=instanceName;
	return frameNumber;
}

void ImageStreamDriver::setDataSourceThread(LoadDataThread* th) {
	if(thread==NULL && th!=NULL) {
		CommPort * comm = CommPort::getRegistry().getInstance(commName);
		if(comm!=NULL)
			comm->open();
		commName.addPrimitiveListener(this);
	}
	if(thread!=NULL && th==NULL) {
		CommPort * comm = CommPort::getRegistry().getInstance(commName);
		if(comm!=NULL)
			comm->close();
		commName.removePrimitiveListener(this);
	}
	DataSource::setDataSourceThread(th);
}

void ImageStreamDriver::plistValueChanged(const plist::PrimitiveBase& pl) {
	if(&pl==&commName) {
		// if here, then setDataSourceThread has been called, thus when commName changes,
		// need to close old one and reopen new one
		CommPort * comm = CommPort::getRegistry().getInstance(commName.getPreviousValue());
		if(comm!=NULL)
			comm->close();
		comm = CommPort::getRegistry().getInstance(commName);
		if(comm!=NULL)
			comm->open();
	} else if(&pl==&format) {
		CommPort * comm = CommPort::getRegistry().getInstance(commName.getPreviousValue());
		if(comm!=NULL) {
			MarkScope autolock(*comm);
			delete [] img;
			img=NULL;
			imgSize=0;
		} else {
			delete [] img;
			img=NULL;
			imgSize=0;
		}
	} else {
		std::cerr << "Unhandled value change in " << getClassName() << ": " << pl.get() << std::endl;
	}
}

void ImageStreamDriver::copyImage(char * buf, unsigned int width, unsigned int height, unsigned int channels, const char * chan, unsigned int lwidth, unsigned int lheight, unsigned int c) {
	if(lwidth==width && lheight==height) {
		// same size, straight copy
		for(unsigned int y=0;y<height;y++) {
			unsigned int datarowstart=y*width*channels+c;
			unsigned int tmprowstart=y*lwidth;
			for(unsigned int x=0;x<width;x++)
				buf[datarowstart+x*channels]=chan[tmprowstart+x];
		}
	} else {
		// upsample image
		
		//we'll linearly interpolate between pixels
		//METHOD A:
		//hold edges, interpolate through middle:
		//  if we have 2 samples, scaling up to 4
		//   index: 0   1    2   3
		// maps to: 0  1/3  2/3  1
		float xsc=(lwidth-1)/(float)(width-1);
		float ysc=(lheight-1)/(float)(height-1);
		for(unsigned int y=0;y<height-1;y++) {
			unsigned int datarowstart=y*width*channels+c;
			float ty=y*ysc;
			unsigned int ly=(int)ty; //lower pixel index
			float fy=ty-ly; //upper pixel weight
			unsigned int tmprowstart=ly*lwidth;
			for(unsigned int x=0;x<width-1;x++) {
				float tx=x*xsc;
				unsigned int lx=(int)tx; //lower pixel index
				float fx=tx-lx; //upper pixel weight
				
				float lv=((int)chan[tmprowstart+lx]&0xFF)*(1-fx)+((int)chan[tmprowstart+lx+1]&0xFF)*fx;
				float uv=((int)chan[tmprowstart+lx+lwidth]&0xFF)*(1-fx)+((int)chan[tmprowstart+lx+1+lwidth]&0xFF)*fx;
				buf[datarowstart+x*channels]=(char)(lv*(1-fy)+uv*fy);
			}
			buf[datarowstart+(width-1)*channels]=chan[tmprowstart+lwidth-1];
		}
		unsigned int datarowstart=width*(height-1)*channels+c;
		unsigned int tmprowstart=lwidth*(lheight-1);
		for(unsigned int x=0;x<width-1;x++) {
			float tx=x*xsc;
			unsigned int lx=(int)tx; //lower pixel index
			float fx=tx-lx; //upper pixel weight
			buf[datarowstart+x*channels]=(char)(((int)chan[tmprowstart+lx]&0xFF)*(1-fx)+((int)chan[tmprowstart+lx+1]&0xFF)*fx);
		}
		buf[datarowstart+(width-1)*channels]=chan[tmprowstart+lwidth-1];
		
		//Unfortunately, pixels are simply interleaved, starting at the
		//top right.  So, Method A will stretch things to the bottom-right
		//a bit.  This method holds left edge and spacing, so it lines up
		//better with what's being transmitted (but the bottom right edges
		//wind up smeared)
		//METHOD B:
		//  if we have 2 samples, scaling up to 4
		//   index: 0   1    2   3
		// maps to: 0  1/2   1   1  <-- this last one would be 3/2, so we have to replicate 1
		/*float xsc=(chanW)/(float)(width);
		float ysc=(chanH)/(float)(height);
		unsigned int xgap=(unsigned int)roundf(1.0f/xsc);
		unsigned int ygap=(unsigned int)roundf(1.0f/ysc);
		for(unsigned int y=0;y<height-ygap;y++) {
			unsigned int datarowstart=y*width*channels+c;
			float ty=y*ysc;
			unsigned int ly=(int)ty; //lower pixel index
			float fy=ty-ly; //upper pixel weight
			unsigned int tmprowstart=ly*chanW;
			for(unsigned int x=0;x<width-xgap;x++) {
				float tx=x*xsc;
				unsigned int lx=(int)tx; //lower pixel index
				float fx=tx-lx; //upper pixel weight
				
				float lv=(chan[tmprowstart+lx]&0xFF)*(1-fx)+(chan[tmprowstart+lx+1]&0xFF)*fx;
				float uv=(chan[tmprowstart+lx+chanW]&0xFF)*(1-fx)+(chan[tmprowstart+lx+1+chanW]&0xFF)*fx;
				buf[datarowstart+x*channels]=(char)(lv*(1-fy)+uv*fy);
			}
			for(unsigned int x=width-xgap;x<width;x++) {
				float lv=(chan[tmprowstart+chanW-1]&0xFF);
				float uv=(chan[tmprowstart+chanW-1+chanW]&0xFF);
				buf[datarowstart+x*channels]=(char)(lv*(1-fy)+uv*fy);
			}
		}
		for(unsigned int y=height-ygap;y<height;y++) {
			unsigned int datarowstart=y*width*channels+c;
			unsigned int tmprowstart=chanW*(chanH-1);
			for(unsigned int x=0;x<width-xgap;x++) {
				float tx=x*xsc;
				unsigned int lx=(int)tx; //lower pixel index
				float fx=tx-lx; //upper pixel weight
				
				float lv=(chan[tmprowstart+lx]&0xFF)*(1-fx)+(chan[tmprowstart+lx+1]&0xFF)*fx;
				buf[datarowstart+x*channels]=(byte)(lv);
			}
			for(unsigned int x=width-xgap;x<width;x++)
				buf[datarowstart+x*channels]=chan[tmprowstart+chanW-1];
		}*/
	}
}

/*! @file
 * @brief 
 * @author Ethan Tira-Thompson (ejt) (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.4 $
 * $State: Exp $
 * $Date: 2007/11/05 00:20:11 $
 */
