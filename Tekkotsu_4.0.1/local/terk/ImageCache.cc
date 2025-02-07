#ifdef HAVE_ICE

#include "Shared/ImageUtil.h"
#include "Shared/debuget.h"
#include "Shared/LoadSave.h"
#include "Shared/MarkScope.h"
#include "ImageCache.h"
#include <unistd.h>

using namespace std;

void ImageCache::newImage(::TeRK::ByteArray data){
	MarkScope l(frameLock);
	int timeNow = get_time();
	
	this->buffers[currentBuffer].setData(data, number);
	
	number++;
	fps = (timeNow - lastTime);
	lastTime = timeNow;
}

unsigned int ImageCache::nextTimestamp() {
	unsigned int diff = get_time() - lastTime;
	if(diff > fps)
		return 0;
	return fps - diff;
}

const std::string& ImageCache::nextName() {
	static const std::string noneStr="(none)";
	return noneStr;
}

unsigned int ImageCache::getData(const char *& payload, unsigned int& size, unsigned int& timestamp, string& name) {
	//printf("IMAGECACHE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
	while(!(buffers[currentBuffer].unread)){
		//cout << "Waiting for buffer " << currentBuffer << " to fill." << endl;
		usleep(10000);
		//add sleep
	}
	MarkScope l(frameLock);
	Frame& image = buffers[currentBuffer];
	image.unread = false;
	
	//printf("READING: timestamp:%d name:%s size:%d data:%p\n", lastTime, image.filename.c_str(), image.size, image.data);
	
	payload=image.data;
	//cout << "sending width " << *(unsigned int*)(image.data) << endl;
	size=image.size;
	//unsigned int reqTime=timestamp;
	timestamp=lastTime;
	name="TeRK";
	
	currentBuffer = currentBuffer ^ 1;
	
	return image.number;
}


void ImageCache::setDataSourceThread(LoadDataThread* p) {
	DataSource::setDataSourceThread(p);
	//if(p!=NULL)
	//  setSource(p->src);
}



// ------------------------------------------------

void ImageCache::Frame::setData(::TeRK::ByteArray dataArray, int dataNumber){
	char* cdata = (char *) &(dataArray[0]);
	
	// get the image info
	size_t nwidth=0, nheight=0, ncomponents=0;
	
	// doesn't decompress whole image, just header info (dimensions)
	image_util::decodeImage(cdata,dataArray.size(),nwidth,nheight,ncomponents);
	if(nwidth*nheight*ncomponents==0) {
		cerr << "TeRK returned empty image" << endl;
		return;
	}
	size_t headerSize=getHeaderSize();
	
	size_t totalSize = headerSize + nwidth*nheight*ncomponents;
	
	if(totalSize > this->size){
		if(this->data)
			delete [] this->data;
		
		this->data = new char[totalSize];
	}
	this->size=totalSize;
	
	char* imgbuf = this->data;
	
	// decompress the image for real this time:
	char * img=imgbuf+headerSize;
	size_t imgSize=totalSize-headerSize;
	if(!image_util::decodeImage(cdata,dataArray.size(),nwidth,nheight,ncomponents,img,imgSize)) {
		cerr << "Image decompression failed" << endl;
		delete [] imgbuf;
		this->data=NULL;
		this->size=0;
		return; // don't fall through to the prepare() below!
	}
	
	if(nwidth*nheight*ncomponents==0) {
		cerr << "TeRK returned empty image" << endl;
		delete [] imgbuf;
		this->data=NULL;
		this->size=0;
	}
	
	//printf("headerSize:%d width:%d height:%d components:%d totalSize:%d data:%p\n", headerSize, width, height, components, totalSize, imgbuf);
	
	this->width = nwidth;
	this->height = nheight;
	this->components = ncomponents;
	
	if(writeHeader(imgbuf,headerSize)==0) {
		cerr << "FileSystemImageSource::ImageInfo::prepare(): Ran out of space writing image header" << endl;
		delete [] imgbuf;
		this->data=NULL;
		this->size=0;
		return;
	}
	
	number = dataNumber;
	this->unread = true;
}

size_t ImageCache::Frame::getHeaderSize() const {
	unsigned int layer=0;
	return LoadSave::getSerializedSize(layer)
		+ LoadSave::getSerializedSize(width)
		+ LoadSave::getSerializedSize(height)
		+ LoadSave::getSerializedSize(components);
}

size_t ImageCache::Frame::writeHeader(char* buf, size_t sz) const {
	unsigned int remain=sz;
	unsigned int layer=0;
	if(!LoadSave::encodeInc(layer,buf,remain)) return 0;
	if(!LoadSave::encodeInc(width,buf,remain)) return 0;
	if(!LoadSave::encodeInc(height,buf,remain)) return 0;
	if(!LoadSave::encodeInc(components,buf,remain)) return 0;
	return sz-remain;
}

#endif
