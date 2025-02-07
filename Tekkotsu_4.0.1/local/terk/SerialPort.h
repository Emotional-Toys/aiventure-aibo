//-*-c++-*-
#ifndef INCLUDED_QwerkSerialPort_h_
#define INCLUDED_QwerkSerialPort_h_

#include "local/CommPort.h"
#include "Shared/plist.h"
#include <fstream>
#include <ios>

#include "TeRKPeerCommon.h"
#include "peer/MRPLPeer.h"
#include "SerialIO.h"

#include "SerialStream.h"

//! Provides CommPort interface to file system devices, such as serial ports
/*! Pass a path to use a file system device, or see NetworkCommPort for a network interface */
class QwerkSerialPort : public CommPort, public virtual plist::PrimitiveListener {
public:
	//! name - name of the serial port to open
	QwerkSerialPort(const std::string& name, ::TeRK::SerialIOServicePrx serialIO)
		: CommPort(autoRegisterQwerkSerialPort,name),
		device("/dev/ttyAM1"), baudRate(TeRK::BAUD115200,baudNames), charSize(TeRK::CHARSIZE8,charSizeNames),
		parity(TeRK::PARITYNONE,parityNames), stopBits(TeRK::STOPBITS1,stopNames), flow(TeRK::FLOWCONTROLHARDWARE,flowNames),
		rbuf(serialIO, name), wbuf(serialIO, name), curloc(), curmode(), opened(0)
	{
		registerInstance();
		addEntry("Device",device,"The path to the device on the Qwerk");
		addEntry("BaudRate",baudRate,"The communication speed (bps)");
		addEntry("CharSize",charSize,"The number of bits per character");
		addEntry("Parity",parity);
		addEntry("StopBits",stopBits);
		addEntry("Flow",flow);
	}
	
	//! destructor, checks that the file descriptor has already been closed
	virtual ~QwerkSerialPort() {
		if(opened>0)
			connectionError("File descriptor still open in QwerkSerialPort destructor",true);
	}
	
	virtual void setSerialPort(const std::string& name, ::TeRK::SerialIOServicePrx serialIO){
		rbuf.open(serialIO, name);
		wbuf.open(serialIO, name);
	}

	virtual std::string getClassName() const { return autoRegisterQwerkSerialPort; }
	
	virtual QwerkSerialBuffer& getReadStreambuf() { return rbuf; }
	virtual QwerkSerialBuffer& getWriteStreambuf() { return wbuf; }
	virtual bool isReadable() { return 1; }
	virtual bool isWriteable() { return 1; }
	
	//! tries to have #sbuf open #path in #mode
	virtual bool open();
	
	//! closes #sbuf
	virtual bool close();
	
	//! watches #path, triggers a close() and re-open() if it changes
	virtual void plistValueChanged(const plist::PrimitiveBase& pl);
	
	static const char * baudNames[];
	static const char * charSizeNames[];
	static const char * parityNames[];
	static const char * stopNames[];
	static const char * flowNames[];
	
	plist::Primitive<std::string> device;
	plist::NamedEnumeration<TeRK::SerialIOBaudRate> baudRate;
	plist::NamedEnumeration<TeRK::SerialIOCharacterSize> charSize;
	plist::NamedEnumeration<TeRK::SerialIOParity> parity;
	plist::NamedEnumeration<TeRK::SerialIOStopBits> stopBits;
	plist::NamedEnumeration<TeRK::SerialIOFlowControl> flow;
	
protected:
	//! Displays message on stderr and if @a fatal is set, calls closeFD()
	virtual void connectionError(const std::string& msg, bool fatal) {
		std::cerr << msg << std::endl;
		if(fatal){
			opened=1;
			close();
		}
	}
	
	QwerkSerialBuffer rbuf;
	QwerkSerialBuffer wbuf;
	std::string curloc;
	std::ios_base::openmode curmode;
	unsigned int opened;
	
	//! holds the class name, set via registration with the CommPort registry
	static const std::string autoRegisterQwerkSerialPort;
};

/*! @file
 * @brief Describes FileDescriptorMotionHook, which provides a file descriptor interface for hardware "drivers"
 * @author Ethan Tira-Thompson (ejt) (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.3 $
 * $State: Exp $
 * $Date: 2007/11/11 23:57:31 $
 */

#endif
