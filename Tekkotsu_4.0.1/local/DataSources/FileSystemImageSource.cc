#include "FileSystemImageSource.h"
#include "Shared/ImageUtil.h"
#include "Shared/debuget.h"
#include <sys/types.h>
#include <sys/mman.h>

//better to put this here instead of the header
using namespace std; 

void FileSystemImageSource::ImageInfo::prepare() {
	if(prepared)
		return;
	if(data==NULL) {
		// load the file from disk
		FileInfo::prepare();
		if(data==NULL)
			return;
		
		// get the image info
		width=height=components=0;
		size_t totalSize=0;
		char * imgbuf = ((char*)NULL)-1;
		image_util::decodeImage(data,size,width,height,components,imgbuf,totalSize); // this call fails (no allocation), but sets image info
		size_t headerSize=getHeaderSize();
		totalSize = headerSize + width*height*components;
		// using mmap to allocate space so we can still use mlock/munlock on it later
		imgbuf = static_cast<char*>(mmap(NULL,totalSize,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANON,-1,0));
		if(imgbuf==MAP_FAILED) {
			std::string err="FileSystemImageSource::ImageInfo::prepare() unable to mmap allocation for image decompression of ";
			err+=filename;
			perror(err.c_str());
			FileInfo::release();
			return;
		}
		if(writeHeader(imgbuf,headerSize)==0) {
			cerr << "FileSystemImageSource::ImageInfo::prepare(): Ran out of space writing image header" << endl;
			munmap(imgbuf,totalSize); // error, give up and clear our memory usage
			FileInfo::release();
			return;
		}
		
		// decompress the image for real this time:
		char * img=imgbuf+headerSize;
		size_t imgSize=totalSize-headerSize;
		if(!image_util::decodeImage(data,size,width,height,components,img,imgSize)) {
			cerr << "Image decompression failed for " << filename << endl;
			munmap(imgbuf,totalSize); // error, give up and clear our memory usage
			FileInfo::release();
			return; // don't fall through to the prepare() below!
		}
		
		// replace the raw compressed data with the uncompressed image
		FileInfo::release();
		data=imgbuf;
		size=totalSize;
	}
	FileInfo::prepare();
}

size_t FileSystemImageSource::ImageInfo::getHeaderSize() const {
	return LoadSave::getSerializedSize<unsigned int>()*4;
}

size_t FileSystemImageSource::ImageInfo::writeHeader(char* buf, size_t sz) const {
	unsigned int remain=sz;
	if(!LoadSave::encodeInc(*dataSource.layer,buf,remain)) return 0;
	if(!LoadSave::encodeInc(width,buf,remain)) return 0;
	if(!LoadSave::encodeInc(height,buf,remain)) return 0;
	if(!LoadSave::encodeInc(components,buf,remain)) return 0;
	ASSERT(remain==0,"FileSystemImageSource::ImageInfo::writeHeader(): Leftover bytes in header? getHeaderSize() is wrong\n");
	return sz-remain;
}

/*! @file
 * @brief 
 * @author Ethan Tira-Thompson (ejt) (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Date: 2007/06/22 18:33:52 $
 */
