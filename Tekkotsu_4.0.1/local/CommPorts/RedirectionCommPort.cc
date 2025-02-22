#include "RedirectionCommPort.h"

using namespace std; 

RedirectionCommPort::invalid_streambuf RedirectionCommPort::invalid;
const std::string RedirectionCommPort::autoRegisterRedirectionCommPort = CommPort::getRegistry().registerType<RedirectionCommPort>("RedirectionCommPort");

bool RedirectionCommPort::open() {
	if(opened++>0)
		return true;
	input.addPrimitiveListener(this);
	output.addPrimitiveListener(this);
	curin=input;
	curout=output;
	bool ans=true;
	if(CommPort * cur=getInputCP())
		ans&=cur->open();
	if(CommPort * cur=getOutputCP())
		ans&=cur->open();
	return ans;
}

bool RedirectionCommPort::close() {
	if(opened==0)
		std::cerr << "Warning: RedirectionCommPort close() without open()" << std::endl;
	if(--opened>0)
		return false;
	input.removePrimitiveListener(this);
	output.removePrimitiveListener(this);
	bool ans=true;
	if(CommPort * cur=getInputCP())
		ans&=cur->close();
	if(CommPort * cur=getOutputCP())
		ans&=cur->close();
	return ans;
}

void RedirectionCommPort::plistValueChanged(const plist::PrimitiveBase& pl) {
	if(&pl==&input) {
		if(CommPort * prev=getInputCP())
			prev->close();
		curin=input;
		if(CommPort * cur=getInputCP())
			cur->open();
	} else if(&pl==&output) {
		if(CommPort * prev=getOutputCP())
			prev->close();
		curout=output;
		if(CommPort * cur=getOutputCP())
			cur->open();
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
 * $Revision: 1.1 $
 * $State: Exp $
 * $Date: 2007/06/03 17:03:36 $
 */
