//-*-c++-*-
#ifndef INCLUDED_BatteryCheckControl_h_
#define INCLUDED_BatteryCheckControl_h_

#include "ControlBase.h"
#include "Shared/WorldState.h"
#include "Motion/MMAccessor.h"
#include "Motion/LedMC.h"
#include "NullControl.h"
#include "Events/EventRouter.h"
#include "Wireless/Wireless.h"

//! when activated, this will print a battery report to stdout and light up LEDs to specify power level
/*! The LEDs use the LedEngine::displayPercent() function, with minor/major style.  This means
 *  the left column (viewing the dog head on) will show the overall power level, and the
 *  right column will show the level within the tick lit up in the left column.  The more geeky
 *  among you may prefer to think of this as a two digit base 5 display.
 *
 *  This gives you pretty precise visual feedback as to remaining power (perhaps more than
 *  you really need, but it's as much a demo as a useful tool)
 *
 *  This is implemented as a Control instead of a Behavior on the assumption you
 *  wouldn't want to leave this running while you were doing other things (ie not
 *  in e-stop). But it definitely blurs the line between the two.
 */
class BatteryCheckControl : public ControlBase, public EventListener {
public:

	//!Constructor
	BatteryCheckControl() : ControlBase("Battery Check","Reports % power remaining, and gives details on console") {}

	//!Destructor
	virtual ~BatteryCheckControl() {}

	//!Prints a report to stdio and lights up the face to show battery level
	/*! keeps running until deactivated - will listen for power events and continue to update display */
	virtual ControlBase * activate(MotionManager::MC_ID display, Socket * gui) {
		sout->printf("Press the \"back\" button to leave battery display\n");
		erouter->addListener(this,EventBase::powerEGID);
		return ControlBase::activate(display,gui);
	}
	//! stops listening for power events and sets display to invalid
	virtual void pause() {
		erouter->removeListener(this);
		display_id=MotionManager::invalid_MC_ID;
	}
	//! calls report()
	virtual void refresh() {
		report();
		if(gui_comm!=NULL && wireless->isConnected(gui_comm->sock)) {
			char tmp[20];
			sprintf(tmp,"%d",(unsigned int)(state->sensors[PowerRemainOffset]*100));
			//		pushSlot(new NullControl(std::string("Power remain: ")+tmp+std::string("%"),"See console output for details"));
			std::string s("refresh\n");
			s+=getName()+"\n1\n0\n0\nPower remain: ";
			s+=tmp;
			s+="%\n0\nSee console output for details\n";
			s+="status\n1\nPower remaining: ";
			s+=tmp;
			s+="%\n";
			gui_comm->write((const byte*)s.c_str(),s.size());
		}
	}
	//! stops listening for power events and sets display to invalid
	virtual void deactivate() {
		erouter->removeListener(this);
		display_id=MotionManager::invalid_MC_ID;
	}
	//! calls refresh() to redisplay with new information if it's not a vibration event
	virtual void processEvent(const EventBase& event) {
		if(event.getSourceID()!=PowerSrcID::VibrationSID)
			refresh();
	}
	virtual ControlBase * doSelect() {
		return this;
	}
	//! redisplay text to sout and refresh LED values
	void report() {
		sout->printf("BATTERY REPORT:\n");
		sout->printf("\tPower Remain:\t%d%%\n",(int)(state->sensors[PowerRemainOffset]*100));
		sout->printf("\tCapacity:\t%g\n",state->sensors[PowerCapacityOffset]);
		sout->printf("\tVoltage:\t%g\n",state->sensors[PowerVoltageOffset]);
		sout->printf("\tCurrent:\t%g\n",state->sensors[PowerCurrentOffset]);
		sout->printf("\tTemperature:\t%g\n",state->sensors[PowerThermoOffset]);
		sout->printf("\tFlags:\t");
		if(state->powerFlags[PowerSrcID::BatteryConnectSID])
			sout->printf("BatteryConnect ");
		if(state->powerFlags[PowerSrcID::DischargingSID])
			sout->printf("Discharging ");
		if(state->powerFlags[PowerSrcID::ChargingSID])
			sout->printf("Charging ");
		if(state->powerFlags[PowerSrcID::ExternalPowerSID])
			sout->printf("ExternalPower ");
		if(state->powerFlags[PowerSrcID::PowerGoodSID])
			sout->printf("PowerGood ");
		if(state->powerFlags[PowerSrcID::LowPowerWarnSID])
			sout->printf("LowPowerWarn ");
		if(state->powerFlags[PowerSrcID::BatteryEmptySID])
			sout->printf("BatteryEmpty ");
		sout->printf("\n");
		if(display_id!=MotionManager::invalid_MC_ID) {
			MMAccessor<LedMC> disp(display_id);
			disp->displayPercent(state->sensors[PowerRemainOffset],LedEngine::major,LedEngine::major);
		}
	}

};

/*! @file
 * @brief Defines BatteryCheckControl, which will spew a power report to stdout upon activation
 * @author ejt (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.12 $
 * $State: Exp $
 * $Date: 2007/06/05 18:03:24 $
 */

#endif
