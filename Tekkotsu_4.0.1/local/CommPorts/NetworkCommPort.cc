#include "NetworkCommPort.h"
#include "Shared/MarkScope.h"

using namespace std;

const std::string NetworkCommPort::autoRegisterNetworkCommPort = CommPort::getRegistry().registerType<NetworkCommPort>("NetworkCommPort");

bool NetworkCommPort::open() {
	if(opened++>0)
		return true;
	sbuf.setReconnect(block);
	block.addPrimitiveListener(dynamic_cast<plist::PrimitiveListener*>(this));
	host.addPrimitiveListener(dynamic_cast<plist::PrimitiveListener*>(this));
	port.addPrimitiveListener(dynamic_cast<plist::PrimitiveListener*>(this));
	transport.addPrimitiveListener(dynamic_cast<plist::PrimitiveListener*>(this));
	server.addPrimitiveListener(dynamic_cast<plist::PrimitiveListener*>(this));
	curaddr.set_name(host);
	curaddr.set_port(port);
	return doOpen(true);
}

bool NetworkCommPort::doOpen(bool dispError) {
	if(lastAttempt.Age()<.75)
		return false;
	lastAttempt.Set();
	if(server) {
		if(block)
			cout << "Waiting for '" << instanceName << "' connection on port " << port << "... " << flush;
		if(!sbuf.listen(curaddr,transport==Config::UDP)) {
			if(dispError) {
				stringstream ss;
				ss << "Could not listen on port " << port;
				connectionError(ss.str(),true);
			}
			return false;
		}
	} else {
		if(block)
			cout << "Waiting for '" << instanceName << "' connection to " << host << ':' << port << "... " << flush;
		if(!sbuf.open(curaddr,transport==Config::UDP)) {
			if(dispError) {
				stringstream ss;
				ss << "Could not open connection to " << host << ":" << port;
				connectionError(ss.str(),true);
			}
			return false;
		}
	}
	if(block)
		cout << instanceName <<  " connected." << endl;
	return true;
}

bool NetworkCommPort::close() {
	MarkScope l(getLock());
	if(opened==0)
		std::cerr << "Warning: NetworkCommPort close() without open()" << std::endl;
	if(--opened>0)
		return false;
	sbuf.setReconnect(false);
	sbuf.close();
	host.removePrimitiveListener(dynamic_cast<plist::PrimitiveListener*>(this));
	port.removePrimitiveListener(dynamic_cast<plist::PrimitiveListener*>(this));
	transport.removePrimitiveListener(dynamic_cast<plist::PrimitiveListener*>(this));
	server.removePrimitiveListener(dynamic_cast<plist::PrimitiveListener*>(this));
	return true;
}

void NetworkCommPort::plistValueChanged(const plist::PrimitiveBase& pl) {
	if(&pl==&host || &pl==&port || &pl==&transport) {
		MarkScope l(getLock());
		if(host!=curaddr.get_name() || port!=curaddr.get_port() || curtrans!=transport) {
			close();
			open();
		}
	} else if(&pl==&server) {
		MarkScope l(getLock());
		close();
		open();
	} else if(&pl==&verbose) {
		sbuf.setEcho(verbose);
	} else if(&pl==&block) {
		sbuf.setReconnect(block);
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
 * $Revision: 1.5 $
 * $State: Exp $
 * $Date: 2007/11/11 23:57:29 $
 */
