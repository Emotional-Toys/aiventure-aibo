#include "FileSystemCommPort.h"

const std::string FileSystemCommPort::autoRegisterFileSystemCommPort = CommPort::getRegistry().registerType<FileSystemCommPort>("FileSystemCommPort");

bool FileSystemCommPort::open() {
	if(opened++>0)
		return true;
	path.addPrimitiveListener(this);
	mode.addPrimitiveListener(this);
	curloc=path;
	curmode=static_cast<std::ios_base::openmode>((int)mode);
	bool ans=true;
	if(mode & std::ios_base::in) {
		if(rbuf.open(path.c_str(),curmode & ~(std::ios_base::out | std::ios_base::trunc))==NULL) {
			connectionError("Could not open file system path for reading "+path,true);
			ans=false;
		}
	}
	if(mode & std::ios_base::out) {
		if(wbuf.open(path.c_str(),curmode & ~std::ios_base::in)==NULL) {
			connectionError("Could not open file system path for writing "+path,true);
			ans=false;
		}
	}
	return ans;
}

bool FileSystemCommPort::close() {
	if(opened==0)
		std::cerr << "Warning: FileSystemCommPort close() without open()" << std::endl;
	if(--opened>0)
		return false;
	path.removePrimitiveListener(this);
	mode.removePrimitiveListener(this);
	bool rc = (rbuf.close()!=NULL);
	bool wc = (wbuf.close()!=NULL);
	return rc && wc;
}

void FileSystemCommPort::plistValueChanged(const plist::PrimitiveBase& pl) {
	if(&pl==&path) {
		if(path!=curloc) {
			unsigned int ref=opened;
			while(opened)
				close();
			while(opened<ref)
				open();
		}
	} else if(&pl==&mode) {
		if(mode!=curmode) {
			std::cerr << "Cannot change access mode while file is open" << std::endl;
		}
	} else {
		std::cerr << "Unhandled value change in " << getClassName() << ": " << pl.get() << std::endl;
	}
}

/*! @file
 * @brief 
 * @author Ethan Tira-Thompson (ejt) (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Date: 2007/06/18 21:47:20 $
 */
