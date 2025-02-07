#include "LGmixin.h"
#include "Shared/ProjectInterface.h"
#include "Vision/JPEGGenerator.h"

#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <unistd.h>

using namespace std;

unsigned int LGmixin::instanceCount = 0;
Socket* LGmixin::LGsock = NULL;
LGmixin* LGmixin::theOne = NULL;

LGmixin::LGmixin() {
  if ( instanceCount++ > 0 )
    return;
  if (theOne != NULL) {
      cerr << "LGmixin statics already constructed!?!?!" << endl;
      return;
    }
    theOne=this;

    LGsock = wireless->socket(Socket::SOCK_STREAM, 1024, LGbufferSize);
    wireless->setDaemon(LGsock, false);
    wireless->listen(LGsock, LGport);
}

LGmixin::~LGmixin() {
  if ( --instanceCount > 0 )
    return;
  if (theOne == NULL) {
    cerr << "LGmixin statics already destructed!?!?!" << endl;
    return;
  }
  wireless->close(LGsock->sock);
  theOne = NULL;
}

void LGmixin::uploadFile(const std::string &filename, bool display, bool isImage) {
  if ( !wireless->isConnected(LGsock->sock) ) {
    cerr << "LookingGlass not connected." << endl;
    return;
  }

  int in_file = open(filename.c_str(), O_RDONLY);
  if ( in_file < 0 ) {
    cerr << "Error: Unable to open file\n";
    return;
  }
  struct stat s;
  stat(filename.c_str(),&s);
  const std::string remoteFilename = filename.substr(filename.rfind('/')+1);
  LGsock->printf("UPLOAD_BINARY %s %u\n", remoteFilename.c_str(), (unsigned int)s.st_size);
  
  while(1){
    char *buffer = (char*)LGsock->getWriteBuffer(s.st_size);
    if ( buffer==NULL ) {
      cerr << "NULL buffer in LG file upload" << endl;
      break;
    }
    int read_size = read(in_file, buffer, s.st_size);
    if( read_size == 0 )
      break;
    
    if ( read_size < 0 ){
      cerr << "Error: Read error " << read_size << endl;
      break;
    }
    LGsock->write(read_size);
  }
  
  if(display)
    if ( isImage )
      displayImageFile(remoteFilename);
    else
      displayHtmlFile(remoteFilename);

  close(in_file);
}

void LGmixin::displayHtmlFile(const std::string &remoteFilename) {
  LGsock->printf("DISPLAY LookingGlass_Temp_File\n"); // work-around because renderer can't display same file twice
  LGsock->printf("DISPLAY %s\n",remoteFilename.c_str());
}

void LGmixin::displayImageFile(const std::string &remoteFilename) {
  LGsock->printf((string("UPLOAD_HTML %s.html\n") +
		 string("<html><body><table width=100%% height=100%%><tr><td align=center valign=middle>") +
		  string("<img src=\"%s\"></td></tr></table></body>\n</html>\n")).c_str(),
		 remoteFilename.c_str(), remoteFilename.c_str());
  displayHtmlFile(remoteFilename+".html");
}

void LGmixin::displayHtmlText(const std::string &text) {
  unsigned int const msgSize = text.size();
  string const tempfilename = "temp9999";
  LGsock->printf("UPLOAD_BINARY %s %u\n", tempfilename.c_str(), msgSize);
  char *buffer = (char*)LGsock->getWriteBuffer(msgSize);
  memcpy(buffer,text.c_str(),msgSize);
  LGsock->write(msgSize);
  displayHtmlFile(tempfilename);
}

void LGmixin::sendCommand(const std::string &command) {
  LGsock->printf("%s\n", command.c_str());
}

void LGmixin::uploadCameraImage(const std::string &remoteFilename) {
    JPEGGenerator *jpeg = ProjectInterface::defColorJPEGGenerator;  
    // call to getImage must come before call to getImageSize
    char *image = (char*)jpeg->getImage(ProjectInterface::fullLayer, 0);
    int const imgSize = jpeg->getImageSize(ProjectInterface::fullLayer, 0);
    LGsock->printf("UPLOAD_BINARY %s %u\n", remoteFilename.c_str(), imgSize);
    char *buffer = (char*)LGsock->getWriteBuffer(imgSize);
    if ( buffer==NULL ) {
      cerr << "NULL buffer in LG camera upload" << endl;
      return;
    }
    memcpy(buffer,image,imgSize);
    LGsock->write(imgSize);
}

void LGmixin::uploadSketch(const DualCoding::Sketch<DualCoding::uchar> &/*sketch*/,
			   const std::string &/*remoteFilename*/) {
  // this function has not been written yet
}

