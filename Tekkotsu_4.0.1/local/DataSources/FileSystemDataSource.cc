#include "FileSystemDataSource.h"
#include "local/DeviceDrivers/LoggedDataDriver.h"
#include "Shared/get_time.h"
#include "Shared/RobotInfo.h"
#include "Shared/string_util.h"
#include <set>
#include <fstream>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <regex.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/resource.h>
#include <cmath>
#include <errno.h>

using namespace std; 

FileSystemDataSource::~FileSystemDataSource() {
	clearFiles();
}

unsigned int FileSystemDataSource::nextTimestamp() {
	return (curfile!=files.end()) ? static_cast<unsigned int>(nextTime) : -1U;
}

const std::string& FileSystemDataSource::nextName() {
	if(curfile!=files.end())
		return (*curfile)->filename;
	else {
		static const std::string noneStr="(none)";
		return noneStr;
	}
}

unsigned int FileSystemDataSource::getData(const char *& payload, unsigned int& payloadSize, unsigned int& timestamp, std::string& name) {
	if(curfile==files.end()) {
		payload=NULL; payloadSize=0;
		return sn;
	}
	if(nextTime<timestamp && std::abs(static_cast<int>(timestamp-nextTime))>std::abs(timestamp-(nextTime+(*curfile)->lifetime))) {
		float looptime=getLoopTime(true);
		if(looptime>0) {
			while(nextTime+looptime<timestamp)
				nextTime+=looptime;
		}
		while(nextTime<timestamp && std::abs(static_cast<int>(timestamp-nextTime))>std::abs(timestamp-(nextTime+(*curfile)->lifetime))) {
			nextFrame(0); // too late, drop frames
			sn++;
		}
	}
	if(!(*curfile)->prepared)
		(*curfile)->prepare();
	payload=(*curfile)->data;
	payloadSize=(*curfile)->size;
	unsigned int reqTime=timestamp;
	float origNext=nextTime;
	timestamp=static_cast<unsigned int>(nextTime);
	name=(*curfile)->filename;
	nextFrame();
	if(reqTime==0 && timestamp!=0 && (frozen || getTimeScale()>=0))
		nextTime=origNext; // the requested time is 0, this is an advance frame, hold nextTime unless not frozen and full speed mode
	return sn++;
}

void FileSystemDataSource::setDataSourceThread(LoadDataThread* th) {
	bool toggle=(th!=NULL && thread==NULL || th==NULL && thread!=NULL);
	DataSource::setDataSourceThread(th);
	nextTime=get_time();
	if(toggle)
		updateProvidingOutputs();
}

void FileSystemDataSource::setDataSourceFramerate(float fr) {
	DataSource::setDataSourceFramerate(fr);
	if(!usingIndexFile()) {
		const float dt=1000.f/framerate;
		bool first = (curfile==files.begin());
		if(!first)
			nextTime-=(*--curfile)->lifetime;
		for(files_t::const_iterator it=files.begin(); it!=files.end(); ++it)
			(*it)->lifetime=dt;
		if(!first)
			nextTime+=(*curfile++)->lifetime;
		parent.plistValueChanged(path); // to reset the loop time if sharing a source
	}
}

const std::string& FileSystemDataSource::getUsedPath() const { return (path.size()==0) ? parent.path : path; }

void FileSystemDataSource::loadFileList(bool clearCurrent/*=true*/, bool reportMissing/*=true*/) {
	nextTime=freezeTime=get_time();
	struct stat sb;
	if(clearCurrent)
		clearFiles();
	else if(files.size()==0)
		clearCurrent=true; // was empty, pretend we just cleared it, have to do the same re-initialization
	if(getUsedPath().size()==0)
		return; //empty path means disabled
	if(stat(getUsedPath().c_str(),&sb)) {
		if(reportMissing)
			std::cerr << "FileSystemDataSource could not access path '" << getUsedPath() << "'" << std::endl;
		return;
	}
	if(sb.st_mode&S_IFDIR) {
		loadFileListFromDirectory();
	} else {
		//Test to see if the file matches the filter
		try {
			if(string_util::reMatch(getUsedPath(),filenameFilter))
				loadSingleFile(getUsedPath().c_str());
			else { //if it doesn't match the image RE, assume it's an index file
				if(!loadFileListFromIndex())
					std::cerr << "Source '" << getUsedPath() << "' does not match the filename filter '" << filenameFilter << "' and is not an index list." << std::endl;
			}
		} catch(const std::string& err) {
			std::cerr << err << std::endl;
		}
	}
	if(clearCurrent) {
		curfile=files.begin();
		files_t::iterator it=curfile;
		for(unsigned int numPreload=2; numPreload>0 && it!=files.end(); numPreload--) {
			(*it)->prepare();
			if(++it==files.end() && loop)
				it=files.begin();
		}
	}
	actualLoopTime=naturalLoopTime=calcLoopTime();
}

void FileSystemDataSource::setFrame(unsigned int f, unsigned int numPreload/*=2*/) {
	for(;curfile!=files.end() && (*curfile)->prepared; ++curfile) {
		if(files.size()>MAX_LOAD)
			(*curfile)->release();
		else
			(*curfile)->done();
	}
	nextTime=freezeTime=get_time();
	curfile=files.begin();
	advance(curfile,f);
	files_t::iterator it=curfile;
	for(; numPreload>0 && it!=files.end(); numPreload--) {
		(*it)->prepare();
		if(++it==files.end() && loop)
			it=files.begin();
	}
}

void FileSystemDataSource::nextFrame(unsigned int numPreload/*=2*/) {
	if(numPreload==0 && verbose>=2)
		cout << "Dropping " << (*curfile)->filename << ' ' << nextTime << ' ' << (*curfile)->lifetime << endl;
	if(files.size()>MAX_LOAD)
		(*curfile)->release();
	else
		(*curfile)->done();
	nextTime+=(*curfile)->lifetime;
	if(++curfile==files.end() && loop) {
		nextTime+=initialDelay;
		curfile=files.begin();
		if(verbose>=3)
			cout << "Looping file system data source at " << nextTime << " to " << (*curfile)->filename << " (loop time=" << getLoopTime() << ")" << endl;
	}
	files_t::iterator it=curfile;
	for(; numPreload>0 && it!=files.end(); numPreload--) {
		(*it)->prepare();
		if(++it==files.end() && loop)
			it=files.begin();
	}
}

float FileSystemDataSource::calcLoopTime() const {
	if(files.size()==0)
		return 0;
	float t=initialDelay;
	for(files_t::const_iterator it=files.begin(); it!=files.end(); ++it)
		t+=(*it)->lifetime;
	return t;
}
void FileSystemDataSource::setLoopTime(float t) {
	if(files.size()==0)
		return;
	float remain = t - getLoopTime(true);
	if(remain + files.back()->lifetime < 0) {
		std::cerr << "FileSystemDataSource::setLoopTime(" << t << ") would result in a negative frame lifetime" << std::endl;
		return;
	}
	files.back()->lifetime+=remain;
	actualLoopTime=t;
}

void FileSystemDataSource::clearFiles() {
	// FileInfo destructor should take care of deleting data buffers...
	for(files_t::iterator it=files.begin(); it!=files.end(); ++it)
		delete *it;
	files.clear();
	curfile=files.begin();
	initialDelay=0;
	actualLoopTime=naturalLoopTime=0;
}

void FileSystemDataSource::plistValueChanged(const plist::PrimitiveBase& pl) {
	if(&pl==&path) {
		loadFileList();
	} else {
		cerr << "FileSystemDataSource didn't handle call to plistValueChanged for " << pl.get() << endl;
	}
}

void FileSystemDataSource::loadXML(xmlNode* node) {
	path.removePrimitiveListener(this);
	plist::Dictionary::loadXML(node);
	path.addPrimitiveListener(this);
	plistValueChanged(path);
}

void FileSystemDataSource::updateProvidingOutputs() {
	if(thread!=NULL) {
		for(unsigned int i=0; i<NumOutputs; i++)
			providingOutput(i);
	} else {
		for(unsigned int i=0; i<NumOutputs; i++)
			ignoringOutput(i);
	}
}

void FileSystemDataSource::loadSingleFile(const std::string& file) {
	indexed=false;
	enqueueFile(file,1000.f/framerate);
}

void FileSystemDataSource::loadFileListFromDirectory() {
	indexed=false;
	regex_t re;
	if(int err=regcomp(&re,filenameFilter.c_str(),REG_EXTENDED | REG_NOSUB)) {
		char msg[128];
		regerror(err,&re,msg,128);
		std::cerr << "Bad filter '" << filenameFilter << "': " << msg << std::endl;
		regfree(&re);
		return;
	}
	DIR * d=opendir(getUsedPath().c_str());
	if(d==NULL) {
		std::cerr << "Could not open directory " << getUsedPath() << std::endl;
		regfree(&re);
		return;
	}
	struct dirent* res;
	
#ifdef HAVE_READDIR_R
	struct dirent cur;
	if(readdir_r(d,&cur,&res)) {
		std::cerr << "Error reading files from " << getUsedPath() << std::endl;
		closedir(d);
		regfree(&re);
		return;
	}
#else
	res=readdir(d);
#endif

	std::set<std::string> dirfiles;
	while(res!=NULL) {
		int match=regexec(&re,res->d_name,0,NULL,0);
		if(match==0) {
			dirfiles.insert(res->d_name);
		} else if(match!=REG_NOMATCH) {
			char msg[128];
			regerror(match,&re,msg,128);
			std::cerr << "Regex error on '" << res->d_name << "': " << msg << std::endl;
		} // else std::cout << "Skipping " << res->d_name << std::endl;
#ifdef HAVE_READDIR_R
		if(readdir_r(d,&cur,&res)) {
			std::cerr << "Error reading files from " << getUsedPath() << std::endl;
			closedir(d);
			regfree(&re);
			return;
		}
#else
		res=readdir(d);
#endif
	}
	closedir(d);
	regfree(&re);
	
	//std::cout << "Processing " << getUsedPath() << std::endl;
	float tinc=1000.f/framerate;
	for(std::set<std::string>::const_iterator it=dirfiles.begin(); it!=dirfiles.end(); ++it) {
		//std::cout << "Enqueuing " << *it << std::endl;
		enqueueFile((getUsedPath()+"/")+(*it),tinc);
	}
}

bool FileSystemDataSource::loadFileListFromIndex() {
	indexed=(indexed || files.size()==0);
	regex_t re;
	if(int err=regcomp(&re,filenameFilter.c_str(),REG_EXTENDED | REG_NOSUB)) {
		char msg[128];
		regerror(err,&re,msg,128);
		std::cerr << "Bad filter '" << filenameFilter << "': " << msg << std::endl;
		regfree(&re);
		return false;
	}
	
	ifstream in(getUsedPath().c_str());
	string cur;
	getline(in,cur);
	if(cur.find("First frame ")==0) //skip the header line from the GUI, e.g. 'First frame 42898 timestamp: 1439018'
		getline(in,cur);
	
	float tinc=1000.f/framerate;
	float lasttime=-tinc;
	while(in) {
		string fn = cur.substr(0,cur.find('\t'));
		int match=regexec(&re,fn.c_str(),0,NULL,0);
		if(match==0) {
			float curtime=lasttime+tinc;
			if(fn.size()!=cur.size()) {
				const char * timep=cur.c_str()+cur.rfind('\t');
				char * endp=NULL;
				curtime=strtof(timep,&endp);
				if(timep==endp) {
					std::cerr << "ERROR: '" << getUsedPath() << "' does not seem to be a valid index file." << std::endl;
					std::cerr << "       Use output from VisionGUI, or use format 'filename <tab> time'" << std::endl;
					std::cerr << "       Where 'time' is the time in milliseconds at which the file should be processed, relative" << std::endl;
					std::cerr << "       to the time at which the index file is loaded." << std::endl;
					regfree(&re);
					return false;
				}
				if(lasttime>=0) {
					files.back()->lifetime=curtime-lasttime;
					//std::cout << "(previous frame lifetime " << files.back()->lifetime << ") ";
				} else if(files.size()>0) {
					files.back()->lifetime+=curtime;
					//std::cout << "(previous frame increased lifetime to " << files.back()->lifetime << ") ";
				} else {
					initialDelay=curtime;
					nextTime=get_time()+curtime;
					//std::cout << "nextTime set to " << nextTime << " ";
				}
			}
			if(fn[0]!='/') { // if not absolute path, tack on path to index file (*do this after previous check*!)
				string::size_type srcdir=getUsedPath().rfind('/');
				if(srcdir!=string::npos)
					fn=getUsedPath().substr(0,srcdir+1)+fn;
			}
			//std::cout << "Enqueuing " << fn << " at " << curtime << endl;
			enqueueFile(fn,tinc);
			lasttime=curtime;
		} else if(match!=REG_NOMATCH) {
			char msg[128];
			regerror(match,&re,msg,128);
			std::cerr << "Regex error on '" << fn << "': " << msg << std::endl;
		} // else std::cout << "Skipping " << res->d_name << std::endl;
		getline(in,cur);
	}
	regfree(&re);
	return true;
}

char * FileSystemDataSource::loadData(const std::string& file) {
	struct stat statbuf;
	if(stat(file.c_str(),&statbuf)!=0) {
		perror("LoadDataThread::loadData");
		return NULL;
	}
	FILE * f=fopen(file.c_str(),"rb");
	if(f==NULL) {
		std::cerr << "LoadDataThread::loadData(): File open failed" << std::endl;
		return NULL;
	}
	char* buf=new char[statbuf.st_size];
	int nread=fread(buf,1,statbuf.st_size,f);;
	fclose(f);
	f=NULL;
	if(nread!=statbuf.st_size) {
		std::cerr << "LoadDataThread::,datagram(): failed to load entire file, "<<nread<<" read, "<<statbuf.st_size<<" requested" << std::endl;
		return NULL;
	}
	return buf;
}

void FileSystemDataSource::FileInfo::prepare() {
	if(prepared)
		return;
	if(data==NULL) {
		struct stat statbuf;
		if(stat(filename.c_str(),&statbuf)!=0) {
			std::string err="FileSystemDataSource::FileInfo::prepare() failed to stat file ";
			err+=filename;
			perror(err.c_str());
			return;
		}
		int fd=open(filename.c_str(),O_RDONLY);
		if(fd<0) {
			std::string err="FileSystemDataSource::FileInfo::prepare() unable to open file ";
			err+=filename;
			perror(err.c_str());
			return;
		}
		size=statbuf.st_size;
		data=static_cast<char*>(mmap(NULL,size,PROT_READ,MAP_PRIVATE|MAP_FILE,fd,0));
		if(data==MAP_FAILED) {
			data=NULL;
			size=0;
			std::string err="FileSystemDataSource::FileInfo::prepare() unable to mmap file ";
			err+=filename;
			perror(err.c_str());
			return;
		}
		if(close(fd)!=0) {
			std::string err="FileSystemDataSource::FileInfo::prepare() unable to close file ";
			err+=filename;
			perror(err.c_str());
			return;
		}
	}
	if(mlock(data,size)!=0) {
		if(errno==ENOMEM) {
			static bool firsterr=true; // give a warning just the first time if mlock fails because RLIMIT_MEMLOCK is too low
			if(firsterr) {
				firsterr=false;
				rlimit rl;
#ifndef __CYGWIN__
				getrlimit(RLIMIT_MEMLOCK,&rl);
				cerr << "Notice: mlock() failed because RLIMIT_MEMLOCK is too low, limited to " << (rl.rlim_cur/1024) << "KB\n"
				     << "Increasing this limit can smooth logged data I/O in low memory situations. (see ulimit/limit commands)" << endl;
#endif
			}
		} else {
			std::string err="FileSystemDataSource::FileInfo::prepare() unable to mlock file ";
			err+=filename;
			perror(err.c_str());
		}
		return;
	}
	prepared=true;
}

void FileSystemDataSource::FileInfo::done() {
	if(data==NULL || !prepared)
		return;
	prepared=false;
	if(munlock(data,size)!=0) {
		std::string err="FileSystemDataSource::FileInfo::prepare() unable to munlock file ";
		err+=filename;
		perror(err.c_str());
	}
}

void FileSystemDataSource::FileInfo::release() {
	if(data==NULL)
		return;
	done();
	if(munmap(data,size)!=0) {
		std::string err="FileSystemDataSource::FileInfo::prepare() unable to munmap file ";
		err+=filename;
		perror(err.c_str());
	}
	data=NULL;
	size=0;
}

/*! @file
 * @brief 
 * @author Ethan Tira-Thompson (ejt) (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.4 $
 * $State: Exp $
 * $Date: 2007/11/13 04:16:05 $
 */
