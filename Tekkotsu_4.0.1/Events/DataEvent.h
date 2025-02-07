//-*-c++-*-
#ifndef INCLUDED_DataEvent_h_
#define INCLUDED_DataEvent_h_

#include "Events/EventBase.h"

//! for passing around data (or pointers to data)
template<class T, int TID=-1>
class DataEvent : public EventBase {
public:
	//!@name Constructors
	//!
	DataEvent() : EventBase(), data() {}
	DataEvent(const T& d, EventGeneratorID_t gid, size_t sid, EventTypeID_t tid, unsigned int dur=0) : EventBase(gid,sid,tid,dur), data(d) {}
	DataEvent(const T& d, EventGeneratorID_t gid, size_t sid, EventTypeID_t tid, unsigned int dur, const std::string& n, float mag) : EventBase(gid,sid,tid,dur,n,mag), data(d) {}

	//! copy constructor
	DataEvent(const DataEvent& evt) : EventBase(evt), data(evt.data) {}
	
	//! assignment
	const DataEvent& operator=(const DataEvent& evt) { EventBase::operator=(evt); data=evt.data; return *this; }

	virtual EventBase* clone() const { return new DataEvent<T>(*this); }
	//@}

	void setData(const T& d) { data=d; } //!< assigns @a d to the internal #data
	const T& getData() const { return data; } //!< returns #data
	T& getData() { return data; } //!< returns #data

	virtual classTypeID_t getClassTypeID() const { return autoRegisterDataEvent; }

protected:
	T data; //!< the data being communicated
	
	//! this silliness is to work around parsing issue in gcc 3.3 branch
	static EventBase::classTypeID_t registerDataType(EventBase::classTypeID_t classid) {
#if !defined(__GNUC__) || __GNUC__>3 || __GNUC__==3 && __GNUC_MINOR__>3
		// if here, using gcc 3.4 or later...
		// gcc 3.3 won't let me call this templated member function?!?!
		return getTypeRegistry().registerType< DataEvent<T,TID> >(classid);
#else // using gcc 3.3.x or prior
		// instead I have to wind up registering my own instance instead of using the FamilyFactory's instance generation
		return getTypeRegistry().registerFactory(classid,new EventBase::registry_t::FactoryType< DataEvent<T,TID> >);
#endif
	}
	static const EventBase::classTypeID_t autoRegisterDataEvent; //!< causes class type id to automatically be regsitered with EventBase's FamilyFactory (getTypeRegistry())
};

template<class T,int TID>
const EventBase::classTypeID_t DataEvent<T,TID>::autoRegisterDataEvent=DataEvent<T,TID>::registerDataType(makeClassTypeID("DATA")+(TID<0?getTypeRegistry().getNumTypes():TID));


/*! @file
 * @brief Defines DataEvent, for passing around data (or pointers to data)
 * @author ejt (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.6 $
 * $State: Exp $
 * $Date: 2007/11/13 04:16:02 $
 */

#endif
