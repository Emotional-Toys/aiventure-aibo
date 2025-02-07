#ifdef HAVE_ICE

#include "SerialPort.h"

const std::string QwerkSerialPort::autoRegisterQwerkSerialPort = "QwerkSerialPort";

const char * QwerkSerialPort::baudNames[] = {
	"50",
	"75",
	"110",
	"134",
	"150",
	"200",
	"300",
	"600",
	"1200",
	"1800",
	"2400",
	"4800",
	"9600",
	"19200",
	"38400",
	"57600",
	"115200",
	""
};
const char * QwerkSerialPort::charSizeNames[] = { "5", "6", "7", "8", "" };
const char * QwerkSerialPort::parityNames[] = { "EVEN", "ODD", "NONE", "" };
const char * QwerkSerialPort::stopNames[] = { "1", "2", "" };
const char * QwerkSerialPort::flowNames[] = { "HARDWARE", "SOFTWARE", "NONE", "" };
INSTANTIATE_NAMEDENUMERATION_STATICS(TeRK::SerialIOBaudRate);
INSTANTIATE_NAMEDENUMERATION_STATICS(TeRK::SerialIOCharacterSize);
INSTANTIATE_NAMEDENUMERATION_STATICS(TeRK::SerialIOParity);
INSTANTIATE_NAMEDENUMERATION_STATICS(TeRK::SerialIOStopBits);
INSTANTIATE_NAMEDENUMERATION_STATICS(TeRK::SerialIOFlowControl);

bool QwerkSerialPort::open() {
	if(opened++>0)
		return true;
	device.addPrimitiveListener(this);
	baudRate.addPrimitiveListener(this);
	charSize.addPrimitiveListener(this);
	parity.addPrimitiveListener(this);
	stopBits.addPrimitiveListener(this);
	flow.addPrimitiveListener(this);
	return true;
}

bool QwerkSerialPort::close() {
	if(opened==0)
		std::cerr << "Warning: NetworkCommPort close() without open()" << std::endl;
	if(--opened>0)
		return false;
	device.removePrimitiveListener(this);
	baudRate.removePrimitiveListener(this);
	charSize.removePrimitiveListener(this);
	parity.removePrimitiveListener(this);
	stopBits.removePrimitiveListener(this);
	flow.removePrimitiveListener(this);
	return true;
}

void QwerkSerialPort::plistValueChanged(const plist::PrimitiveBase& pl) {
	if(&pl==&device) {
	} else if(&pl==&baudRate) {
	} else if(&pl==&charSize) {
	} else if(&pl==&parity) {
	} else if(&pl==&stopBits) {
	} else if(&pl==&flow) {
	} else {
		std::cerr << "Unknown value in QwerkSerialPort::plistValueChanged!" << std::endl;
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
 * $Date: 2007/11/16 20:20:22 $
 */

#endif
