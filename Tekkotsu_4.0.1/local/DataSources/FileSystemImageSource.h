//-*-c++-*-
#ifndef INCLUDED_FileSystemImageSource_h_
#define INCLUDED_FileSystemImageSource_h_

#include "FileSystemDataSource.h"

//! Extends FileSystemDataSource to decompress image data
class FileSystemImageSource : public FileSystemDataSource {
public:
	//! constructor
	FileSystemImageSource(LoggedDataDriver& p, const std::string& filter)
		: FileSystemDataSource(p,filter), layer(0)
	{
		addEntry("Layer",layer,"Controls the resolution layer at which the image should be processed.\n0 indicates \"automatic\" mode (picks layer closest to image's resolution), positive numbers indicate the resolution layer directly.\nNegative values are relative to the number of layers marked available by the vision setup, so that typically -1 would correspond to the \"double\" layer, and -2 would correspond to the \"full\" layer.");
	}

	//! Controls the resolution layer at which the image should be processed
	/*! 0 indicates "automatic" mode (picks layer closest to image's resolution), positive
	 *  numbers indicate the resolution layer directly.
	 *  
	 *  Negative values are relative to the number of layers marked available by the
	 *  vision setup, so that typically -1 would correspond to the "double" layer, and -2
	 *  would correspond to the "full" layer. */
	plist::Primitive<int> layer;

protected:
	
	//! ignores call -- images don't provide outputs
	virtual void updateProvidingOutputs() {}
	
	//! extends FileInfo to provide image decompression and some additional fields for image meta-data
	class ImageInfo : public FileInfo {
	public:
		//! constructor
		ImageInfo(const FileSystemImageSource& ds, const std::string& name, float time) : FileInfo(name,time), dataSource(ds), width(), height(), components() {}
		
		//! uses FileInfo's prepare to load file into memory, and then replaces with a decompressed version
		virtual void prepare();
		
		//! size of the header containing image meta data
		virtual size_t getHeaderSize() const;
		//! encodes the file meta-data into the buffer: FileSystemImageSource::layer, #width, #height, #components; raw image data will then follow
		virtual size_t writeHeader(char* buf, size_t size) const;
		
		//! reference back to the containing data source so we can access FileSystemImageSource::layer
		const FileSystemImageSource& dataSource;
		size_t width; //!< width of image
		size_t height; //!< height of image
		size_t components; //!< number of color channels
	};
	
	virtual void enqueueFile(const std::string& name, float lifetime) { files.push_back(new ImageInfo(*this,name,lifetime)); } 
};

/*! @file
 * @brief 
 * @author Ethan Tira-Thompson (ejt) (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Date: 2007/11/09 19:01:13 $
 */

#endif
