//-*-c++-*-
#ifndef INCLUDED_WMMonitorBehavior_h_
#define INCLUDED_WMMonitorBehavior_h_

#include <iostream>
#include "Wireless/Wireless.h"
#include "Behaviors/BehaviorBase.h"
#include "Motion/MotionManager.h"
#include "Events/EventRouter.h"
#include "Events/EventBase.h"
#include "Shared/RobotInfo.h"
#include "Behaviors/Controller.h"
#include "Shared/WMclass.h"

//! interfaces with wireless to pass information from the gui to the monitor
int wmmonitorcmd_callback(char *buf, int bytes);
class WMMonitorBehavior;

//! so wmmonitorcmd_callback knows where to send the input from the GUI
extern WMMonitorBehavior *wmMonitorBehavior;

class WMitem_base;

//! Listens to wmmonitor control commands coming in from the command port.
class WMMonitorBehavior : public BehaviorBase {
 protected:
	//! The input command stream socket
	Socket *cmdsock;

 private:
	WMMonitorBehavior(const WMMonitorBehavior&); //!< don't call
	WMMonitorBehavior operator=(const WMMonitorBehavior&); //!< don't call

 public:
	//! constructor
	WMMonitorBehavior() :
	  BehaviorBase("WMMonitorBehavior"),
	  cmdsock(NULL)
	{ wmMonitorBehavior = this; }
	//! destructor
	virtual ~WMMonitorBehavior() { wmMonitorBehavior = NULL; }

	//! processes input from the GUI
  void registerData(const std::string& s);

	//! Finds the item with the corresponding name
  WMitem_base* find (std::string& s);

	//! Writes out the passed information to #cmdsock
  void report (const std::string& var_type,
               const std::string& var_name,
               const std::string& value);

	//! Writes out the passed information to #cmdsock
  void report (const std::string& var_type,
               const std::string& var_name,
               const char* value, int value_length);

	//! Writes out the passed information to #cmdsock
  void report(const char* var_type, int var_type_length,
              const char* var_name, int var_name_length,
              const char* value, int value_length);

	virtual void DoStart() {
		// Behavior startup
		BehaviorBase::DoStart();
		// Turn on wireless
		cmdsock=wireless->socket(Socket::SOCK_STREAM, 2048, 8192);
		wireless->setReceiver(cmdsock->sock, wmmonitorcmd_callback);
		wireless->setDaemon(cmdsock,true); 
		wireless->listen(cmdsock->sock, config->main.wmmonitor_port);
		// open gui
		// GUI is opened by separate behavior ViewWMVarsBehavior
		// until bug with clearing wmvars when restarting this is fixed
		erouter->addListener(this,EventBase::wmVarEGID);
	}

	virtual void DoStop() {
//		Controller::closeGUI(getGUIType());
		erouter->removeListener(this);
		// Close socket; turn wireless off
		wireless->setDaemon(cmdsock,false); 
		wireless->close(cmdsock);
		// Total behavior stop
		BehaviorBase::DoStop();
	}

	virtual void processEvent(const EventBase& e) {
		WMentry * entry = reinterpret_cast<WMentry*>(e.getSourceID());
		std::string s(entry->item_name);
		WMregistry *temp=entry->registry;
		while (temp!=&GlobalWM && temp!=NULL) {
			s=temp->name + "." + s;
			temp=temp->parent;
		}
		report(entry->type_name, s, entry->item->toString());
	}

	virtual unsigned int getPort() const { return config->main.wmmonitor_port; } //!< returns network port from config

	static std::string getClassDescription() { 
		char tmp[20];
		sprintf(tmp,"%d",*config->main.wmmonitor_port);
		return std::string("Bidirectional control communication with WMMonitor on port ")+tmp;
	}
	virtual std::string getDescription() const { return getClassDescription(); }

protected:
	static const unsigned int packet_wmclass=14; //!< magic id number, corresponds to Listener.java PACKET_WMCLASS
  //! writes packet type and timestamp
  inline static void encodeHeader(char **dst, unsigned int pformat) {
    encode(dst, pformat);
    encode(dst, get_time());
  }
	//! writes @a value to @a dst and advances @a dst
  template<class T>
  inline static void encode(char **dst, const T& value) {
    memcpy(*dst, &value, sizeof(T));
		// it'd be nice to use network byte order, but we'll save the aibo extra work
		//hostToNetwork(*dst, (char *)&value, sizeof(T));
    (*dst) += sizeof(T);
  }

	//! writes @a length bytes from @a src to @a dst
  template<class T>
  inline static void encode(char **dst, const T * src, int num) {
    memcpy(*dst, src, num*sizeof(T));
    (*dst) += num*sizeof(T);
  }


};

/*! @file
 * @brief Defines WMMonitorBehavior, which listens to commands from the Aibo3D gui and shows current state
 * @author alokl (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.11 $
 * $State: Exp $
 * $Date: 2007/05/21 20:51:20 $
 */

#endif 
