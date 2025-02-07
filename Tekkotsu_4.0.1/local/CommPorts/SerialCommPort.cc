#include "SerialCommPort.h"
#include <termios.h>
#include <sys/ioctl.h>
#include <errno.h>
#ifdef __APPLE__
#  include <IOKit/serial/ioss.h>
#endif

#ifdef __CYGWIN__
// Cygwin doesn't provide cfmakeraw...
// this definition found in port of unix 'script' utility
// by Alan Evans (Alan_Evans AT iwv com), 2002-09-27
// http://marc.info/?l=cygwin&m=103314951904556&w=2
void cfmakeraw(struct termios *termios_p) {
  termios_p->c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP
			  |INLCR|IGNCR|ICRNL|IXON);
  termios_p->c_oflag &= ~OPOST;
  termios_p->c_lflag &= ~(ECHO|ECHONL|ICANON|ISIG|IEXTEN);
  termios_p->c_cflag &= ~(CSIZE|PARENB);
  termios_p->c_cflag |= CS8;
}
#endif

using namespace std;

const char * const SerialCommPort::parityNames[] = { "EVEN", "ODD", "NONE", "" };
INSTANTIATE_NAMEDENUMERATION_STATICS(SerialCommPort::parity_t);

const std::string SerialCommPort::autoRegisterSerialCommPort = CommPort::getRegistry().registerType<SerialCommPort>("SerialCommPort");

void SerialCommPort::setupSerial() {
	if(fd<0)
		return;
	
	// ** first do a basic setup to initialize our access to the serial port **
	
	// get current termios structure (sys/termios.h)
	struct termios	theTermios;
	int ret = tcgetattr(fd, &theTermios);
	cfmakeraw(&theTermios);
	
#ifndef __APPLE__
	// linux can handle non-standard baud rates directly
	// we'll handle Darwin below, uses an ioctl call instead
	cfsetispeed(&theTermios, baudRate);
	cfsetospeed(&theTermios, baudRate);
#endif
	
	// turn on READ and ignore modem control lines
	theTermios.c_cflag |= CREAD | CLOCAL;
	//theTermios.c_cflag = CIGNORE;
	
	switch (dataBits) {
		case 5:		theTermios.c_cflag |= CS5;		break;
		case 6:		theTermios.c_cflag |= CS6;		break;
		case 7:		theTermios.c_cflag |= CS7;		break;
		case 8:		theTermios.c_cflag |= CS8;		break;
		default: std::cerr << "SerialCommPort: bad DataBits value " << dataBits << std::endl; 
	}
	
	// stop bit(s)?
	if(stopBits==1)
		theTermios.c_cflag &= ~CSTOPB;
	else if(stopBits==2)
		theTermios.c_cflag |= CSTOPB;
	else
		std::cerr << "SerialCommPort: bad StopBits value " << stopBits << std::endl; 
	
	// parity?
	switch(parity) {
		case EVEN:
			theTermios.c_cflag |= PARENB; theTermios.c_cflag &= ~PARODD; break;
		case ODD:
			theTermios.c_cflag |= PARENB; theTermios.c_cflag |= PARODD; break;
		case NONE:
			theTermios.c_cflag &= ~PARENB; break;
	}
	// default no flow control:
	theTermios.c_iflag &= ~(IXON | IXOFF);
	theTermios.c_cflag &= ~CRTSCTS;
	
	// apply our settings
	ret = tcsetattr(fd, TCSAFLUSH, &theTermios);
	//ret = ioctl(fd, TIOCSETA, &theTermios); // alternative ?
	if (ret)
		dispError("tcsetattr(TCSAFLUSH)",ret,errno);
	
#ifdef __APPLE__
	// this allows higher (non-standard) baud rates, apparently not supported (on darwin) via termios
	const int TGTBAUD = baudRate;
	ret = ioctl(fd, IOSSIOSPEED, &TGTBAUD); // as opposed to setting it in theTermios ?
	if (ret)
		dispError("ioctl(IOSSIOSPEED)",ret,errno);
#endif
	
	// this part doesn't seem to be necessary, but better safe than sorry...
	int modem;
	ret = ioctl(fd, TIOCMGET, &modem);
	if (ret)
		dispError("ioctl(TIOCMGET)",ret,errno);
	modem |= TIOCM_DTR;
	ret = ioctl(fd, TIOCMSET, &modem);
	if (ret)
		dispError("ioctl(TIOCMSET)",ret,errno);
	
	if(sttyConfig.size()>0) {
		// ** do additional setup by calling out to stty **
	#ifdef __linux__
		std::string cmd="stty -F "+path+" "+sttyConfig;
	#else /* assume BSD style... use a -f instead of -F (...sigh...) */
		std::string cmd="stty -f "+path+" "+sttyConfig;
	#endif
		switch(::system(cmd.c_str())) {
			case 0:
				break; // good!
			case -1:
				perror("Warning: SerialCommPort could not make system call to stty"); break;
			case 127:
				std::cerr << "Warning: SerialCommPort could not make system call to stty: no shell found" << std::endl; break;
			default:
				std::cerr << "Warning: SerialCommPort stty reported error on configuration string: " << sttyConfig << std::endl; break;
		}
	}
}

void SerialCommPort::dispError(const char* where, int ret, int err) {
	std::cerr << where << " returned " << ret << " (errno " << err << ": " << strerror(err) << ")" << std::endl;
}


/*! @file
 * @brief 
 * @author Ethan Tira-Thompson (ejt) (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.9 $
 * $State: Exp $
 * $Date: 2007/11/08 16:30:46 $
 */
