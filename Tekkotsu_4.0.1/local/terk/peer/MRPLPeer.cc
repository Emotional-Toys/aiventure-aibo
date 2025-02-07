#ifdef HAVE_ICE

// **********************************************************************
//
// Copyright (c) 2003-2006 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

// Ice version 3.1.1
// Generated from file `MRPLPeer.ice'

#include "MRPLPeer.h"
#include <Ice/LocalException.h>
#include <Ice/ObjectFactory.h>
#include <Ice/BasicStream.h>
#include <Ice/Object.h>
#include <IceUtil/Iterator.h>

#ifndef ICE_IGNORE_VERSION
#   if ICE_INT_VERSION / 100 != 301
#       error Ice version mismatch!
#   endif
#   if ICE_INT_VERSION % 100 < 1
#       error Ice patch level mismatch!
#   endif
#endif

static const ::std::string __peer__UserConnectionEventHandler__forcedLogoutNotification_name = "forcedLogoutNotification";

static const ::std::string __peer__PeerConnectionEventHandler__peerConnected_name = "peerConnected";

static const ::std::string __peer__PeerConnectionEventHandler__peerConnectedNoProxy_name = "peerConnectedNoProxy";

static const ::std::string __peer__PeerConnectionEventHandler__peerDisconnected_name = "peerDisconnected";

static const ::std::string __peer__PeerRegistrationHandler__registerCallbacks_name = "registerCallbacks";

static const ::std::string __peer__PeerRegistrationHandler__registerProxy_name = "registerProxy";

static const ::std::string __peer__PeerRegistrationHandler__registerProxies_name = "registerProxies";

static const ::std::string __peer__PeerRegistrationHandler__getPeerProxy_name = "getPeerProxy";

static const ::std::string __peer__PeerRegistrationHandler__getPeerProxies_name = "getPeerProxies";

static const ::std::string __peer__UserSession__getMyAvailablePeers_name = "getMyAvailablePeers";

static const ::std::string __peer__UserSession__connectToPeer_name = "connectToPeer";

static const ::std::string __peer__UserSession__getConnectedPeers_name = "getConnectedPeers";

static const ::std::string __peer__UserSession__disconnectFromPeer_name = "disconnectFromPeer";

static const ::std::string __peer__UserSession__disconnectFromPeers_name = "disconnectFromPeers";

void
IceInternal::incRef(::peer::UserConnectionEventHandler* p)
{
    p->__incRef();
}

void
IceInternal::decRef(::peer::UserConnectionEventHandler* p)
{
    p->__decRef();
}

void
IceInternal::incRef(::IceProxy::peer::UserConnectionEventHandler* p)
{
    p->__incRef();
}

void
IceInternal::decRef(::IceProxy::peer::UserConnectionEventHandler* p)
{
    p->__decRef();
}

void
IceInternal::incRef(::peer::PeerConnectionEventHandler* p)
{
    p->__incRef();
}

void
IceInternal::decRef(::peer::PeerConnectionEventHandler* p)
{
    p->__decRef();
}

void
IceInternal::incRef(::IceProxy::peer::PeerConnectionEventHandler* p)
{
    p->__incRef();
}

void
IceInternal::decRef(::IceProxy::peer::PeerConnectionEventHandler* p)
{
    p->__decRef();
}

void
IceInternal::incRef(::peer::ConnectionEventHandler* p)
{
    p->__incRef();
}

void
IceInternal::decRef(::peer::ConnectionEventHandler* p)
{
    p->__decRef();
}

void
IceInternal::incRef(::IceProxy::peer::ConnectionEventHandler* p)
{
    p->__incRef();
}

void
IceInternal::decRef(::IceProxy::peer::ConnectionEventHandler* p)
{
    p->__decRef();
}

void
IceInternal::incRef(::peer::PeerRegistrationHandler* p)
{
    p->__incRef();
}

void
IceInternal::decRef(::peer::PeerRegistrationHandler* p)
{
    p->__decRef();
}

void
IceInternal::incRef(::IceProxy::peer::PeerRegistrationHandler* p)
{
    p->__incRef();
}

void
IceInternal::decRef(::IceProxy::peer::PeerRegistrationHandler* p)
{
    p->__decRef();
}

void
IceInternal::incRef(::peer::UserSession* p)
{
    p->__incRef();
}

void
IceInternal::decRef(::peer::UserSession* p)
{
    p->__decRef();
}

void
IceInternal::incRef(::IceProxy::peer::UserSession* p)
{
    p->__incRef();
}

void
IceInternal::decRef(::IceProxy::peer::UserSession* p)
{
    p->__decRef();
}

void
peer::__write(::IceInternal::BasicStream* __os, const ::peer::UserConnectionEventHandlerPrx& v)
{
    __os->write(::Ice::ObjectPrx(v));
}

void
peer::__read(::IceInternal::BasicStream* __is, ::peer::UserConnectionEventHandlerPrx& v)
{
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    if(!proxy)
    {
	v = 0;
    }
    else
    {
	v = new ::IceProxy::peer::UserConnectionEventHandler;
	v->__copyFrom(proxy);
    }
}

void
peer::__write(::IceInternal::BasicStream* __os, const ::peer::UserConnectionEventHandlerPtr& v)
{
    __os->write(::Ice::ObjectPtr(v));
}

void
peer::__write(::IceInternal::BasicStream* __os, const ::peer::PeerConnectionEventHandlerPrx& v)
{
    __os->write(::Ice::ObjectPrx(v));
}

void
peer::__read(::IceInternal::BasicStream* __is, ::peer::PeerConnectionEventHandlerPrx& v)
{
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    if(!proxy)
    {
	v = 0;
    }
    else
    {
	v = new ::IceProxy::peer::PeerConnectionEventHandler;
	v->__copyFrom(proxy);
    }
}

void
peer::__write(::IceInternal::BasicStream* __os, const ::peer::PeerConnectionEventHandlerPtr& v)
{
    __os->write(::Ice::ObjectPtr(v));
}

void
peer::__write(::IceInternal::BasicStream* __os, const ::peer::ConnectionEventHandlerPrx& v)
{
    __os->write(::Ice::ObjectPrx(v));
}

void
peer::__read(::IceInternal::BasicStream* __is, ::peer::ConnectionEventHandlerPrx& v)
{
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    if(!proxy)
    {
	v = 0;
    }
    else
    {
	v = new ::IceProxy::peer::ConnectionEventHandler;
	v->__copyFrom(proxy);
    }
}

void
peer::__write(::IceInternal::BasicStream* __os, const ::peer::ConnectionEventHandlerPtr& v)
{
    __os->write(::Ice::ObjectPtr(v));
}

void
peer::__write(::IceInternal::BasicStream* __os, const ::peer::PeerRegistrationHandlerPrx& v)
{
    __os->write(::Ice::ObjectPrx(v));
}

void
peer::__read(::IceInternal::BasicStream* __is, ::peer::PeerRegistrationHandlerPrx& v)
{
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    if(!proxy)
    {
	v = 0;
    }
    else
    {
	v = new ::IceProxy::peer::PeerRegistrationHandler;
	v->__copyFrom(proxy);
    }
}

void
peer::__write(::IceInternal::BasicStream* __os, const ::peer::PeerRegistrationHandlerPtr& v)
{
    __os->write(::Ice::ObjectPtr(v));
}

void
peer::__write(::IceInternal::BasicStream* __os, const ::peer::UserSessionPrx& v)
{
    __os->write(::Ice::ObjectPrx(v));
}

void
peer::__read(::IceInternal::BasicStream* __is, ::peer::UserSessionPrx& v)
{
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    if(!proxy)
    {
	v = 0;
    }
    else
    {
	v = new ::IceProxy::peer::UserSession;
	v->__copyFrom(proxy);
    }
}

void
peer::__write(::IceInternal::BasicStream* __os, const ::peer::UserSessionPtr& v)
{
    __os->write(::Ice::ObjectPtr(v));
}

void
peer::__write(::IceInternal::BasicStream* __os, ::peer::PeerAccessLevel v)
{
    __os->write(static_cast< ::Ice::Byte>(v));
}

void
peer::__read(::IceInternal::BasicStream* __is, ::peer::PeerAccessLevel& v)
{
    ::Ice::Byte val;
    __is->read(val);
    v = static_cast< ::peer::PeerAccessLevel>(val);
}

void
peer::__write(::IceInternal::BasicStream* __os, const ::Ice::Identity* begin, const ::Ice::Identity* end, ::peer::__U__IdentityArray)
{
    ::Ice::Int size = static_cast< ::Ice::Int>(end - begin);
    __os->writeSize(size);
    for(int i = 0; i < size; ++i)
    {
	begin[i].__write(__os);
    }
}

void
peer::__read(::IceInternal::BasicStream* __is, ::peer::IdentityArray& v, ::peer::__U__IdentityArray)
{
    ::Ice::Int sz;
    __is->readSize(sz);
    __is->startSeq(sz, 2);
    v.resize(sz);
    for(int i = 0; i < sz; ++i)
    {
	v[i].__read(__is);
	__is->checkSeq();
	__is->endElement();
    }
    __is->endSeq(sz);
}

void
peer::__write(::IceInternal::BasicStream* __os, const ::Ice::ObjectPrx* begin, const ::Ice::ObjectPrx* end, ::peer::__U__ObjectProxyArray)
{
    ::Ice::Int size = static_cast< ::Ice::Int>(end - begin);
    __os->writeSize(size);
    for(int i = 0; i < size; ++i)
    {
	__os->write(begin[i]);
    }
}

void
peer::__read(::IceInternal::BasicStream* __is, ::peer::ObjectProxyArray& v, ::peer::__U__ObjectProxyArray)
{
    ::Ice::Int sz;
    __is->readSize(sz);
    __is->startSeq(sz, 2);
    v.resize(sz);
    for(int i = 0; i < sz; ++i)
    {
	__is->read(v[i]);
	__is->checkSeq();
	__is->endElement();
    }
    __is->endSeq(sz);
}

void
peer::__write(::IceInternal::BasicStream* __os, const ::peer::IdentityToProxyMap& v, ::peer::__U__IdentityToProxyMap)
{
    __os->writeSize(::Ice::Int(v.size()));
    ::peer::IdentityToProxyMap::const_iterator p;
    for(p = v.begin(); p != v.end(); ++p)
    {
	p->first.__write(__os);
	__os->write(p->second);
    }
}

void
peer::__read(::IceInternal::BasicStream* __is, ::peer::IdentityToProxyMap& v, ::peer::__U__IdentityToProxyMap)
{
    ::Ice::Int sz;
    __is->readSize(sz);
    while(sz--)
    {
	::std::pair<const  ::Ice::Identity, ::Ice::ObjectPrx> pair;
	const_cast< ::Ice::Identity&>(pair.first).__read(__is);
	::peer::IdentityToProxyMap::iterator __i = v.insert(v.end(), pair);
	__is->read(__i->second);
    }
}

bool
peer::PeerIdentifier::operator==(const PeerIdentifier& __rhs) const
{
    return !operator!=(__rhs);
}

bool
peer::PeerIdentifier::operator!=(const PeerIdentifier& __rhs) const
{
    if(this == &__rhs)
    {
	return false;
    }
    if(userId != __rhs.userId)
    {
	return true;
    }
    if(firstName != __rhs.firstName)
    {
	return true;
    }
    if(lastName != __rhs.lastName)
    {
	return true;
    }
    return false;
}

bool
peer::PeerIdentifier::operator<(const PeerIdentifier& __rhs) const
{
    if(this == &__rhs)
    {
	return false;
    }
    if(userId < __rhs.userId)
    {
	return true;
    }
    else if(__rhs.userId < userId)
    {
	return false;
    }
    if(firstName < __rhs.firstName)
    {
	return true;
    }
    else if(__rhs.firstName < firstName)
    {
	return false;
    }
    if(lastName < __rhs.lastName)
    {
	return true;
    }
    else if(__rhs.lastName < lastName)
    {
	return false;
    }
    return false;
}

void
peer::PeerIdentifier::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(userId);
    __os->write(firstName);
    __os->write(lastName);
}

void
peer::PeerIdentifier::__read(::IceInternal::BasicStream* __is)
{
    __is->read(userId);
    __is->read(firstName);
    __is->read(lastName);
}

peer::PeerException::PeerException(const ::std::string& __ice_reason) :
#if (defined(_MSC_VER) && (_MSC_VER < 1300)) // VC++ 6 compiler bug
    UserException(),
#else
    ::Ice::UserException(),
#endif
    reason(__ice_reason)
{
}

static const char* __peer__PeerException_name = "peer::PeerException";

const ::std::string
peer::PeerException::ice_name() const
{
    return __peer__PeerException_name;
}

::Ice::Exception*
peer::PeerException::ice_clone() const
{
    return new PeerException(*this);
}

void
peer::PeerException::ice_throw() const
{
    throw *this;
}

void
peer::PeerException::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(::std::string("::peer::PeerException"), false);
    __os->startWriteSlice();
    __os->write(reason);
    __os->endWriteSlice();
}

void
peer::PeerException::__read(::IceInternal::BasicStream* __is, bool __rid)
{
    if(__rid)
    {
	::std::string myId;
	__is->read(myId, false);
    }
    __is->startReadSlice();
    __is->read(reason);
    __is->endReadSlice();
}

void
peer::PeerException::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "exception peer::PeerException was not generated with stream support";
    throw ex;
}

void
peer::PeerException::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "exception peer::PeerException was not generated with stream support";
    throw ex;
}

struct __F__peer__PeerException : public ::IceInternal::UserExceptionFactory
{
    virtual void
    createAndThrow()
    {
	throw ::peer::PeerException();
    }
};

static ::IceInternal::UserExceptionFactoryPtr __F__peer__PeerException__Ptr = new __F__peer__PeerException;

const ::IceInternal::UserExceptionFactoryPtr&
peer::PeerException::ice_factory()
{
    return __F__peer__PeerException__Ptr;
}

class __F__peer__PeerException__Init
{
public:

    __F__peer__PeerException__Init()
    {
	::IceInternal::factoryTable->addExceptionFactory("::peer::PeerException", ::peer::PeerException::ice_factory());
    }

    ~__F__peer__PeerException__Init()
    {
	::IceInternal::factoryTable->removeExceptionFactory("::peer::PeerException");
    }
};

static __F__peer__PeerException__Init __F__peer__PeerException__i;

#ifdef __APPLE__
extern "C" { void __F__peer__PeerException__initializer() {} }
#endif

peer::InvalidIdentityException::InvalidIdentityException(const ::std::string& __ice_reason) :
#if (defined(_MSC_VER) && (_MSC_VER < 1300)) // VC++ 6 compiler bug
    PeerException(__ice_reason)
#else
    ::peer::PeerException(__ice_reason)
#endif
{
}

static const char* __peer__InvalidIdentityException_name = "peer::InvalidIdentityException";

const ::std::string
peer::InvalidIdentityException::ice_name() const
{
    return __peer__InvalidIdentityException_name;
}

::Ice::Exception*
peer::InvalidIdentityException::ice_clone() const
{
    return new InvalidIdentityException(*this);
}

void
peer::InvalidIdentityException::ice_throw() const
{
    throw *this;
}

void
peer::InvalidIdentityException::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(::std::string("::peer::InvalidIdentityException"), false);
    __os->startWriteSlice();
    __os->endWriteSlice();
#if (defined(_MSC_VER) && (_MSC_VER < 1300)) // VC++ 6 compiler bug
    PeerException::__write(__os);
#else
    ::peer::PeerException::__write(__os);
#endif
}

void
peer::InvalidIdentityException::__read(::IceInternal::BasicStream* __is, bool __rid)
{
    if(__rid)
    {
	::std::string myId;
	__is->read(myId, false);
    }
    __is->startReadSlice();
    __is->endReadSlice();
#if (defined(_MSC_VER) && (_MSC_VER < 1300)) // VC++ 6 compiler bug
    PeerException::__read(__is, true);
#else
    ::peer::PeerException::__read(__is, true);
#endif
}

void
peer::InvalidIdentityException::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "exception peer::InvalidIdentityException was not generated with stream support";
    throw ex;
}

void
peer::InvalidIdentityException::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "exception peer::InvalidIdentityException was not generated with stream support";
    throw ex;
}

struct __F__peer__InvalidIdentityException : public ::IceInternal::UserExceptionFactory
{
    virtual void
    createAndThrow()
    {
	throw ::peer::InvalidIdentityException();
    }
};

static ::IceInternal::UserExceptionFactoryPtr __F__peer__InvalidIdentityException__Ptr = new __F__peer__InvalidIdentityException;

const ::IceInternal::UserExceptionFactoryPtr&
peer::InvalidIdentityException::ice_factory()
{
    return __F__peer__InvalidIdentityException__Ptr;
}

class __F__peer__InvalidIdentityException__Init
{
public:

    __F__peer__InvalidIdentityException__Init()
    {
	::IceInternal::factoryTable->addExceptionFactory("::peer::InvalidIdentityException", ::peer::InvalidIdentityException::ice_factory());
    }

    ~__F__peer__InvalidIdentityException__Init()
    {
	::IceInternal::factoryTable->removeExceptionFactory("::peer::InvalidIdentityException");
    }
};

static __F__peer__InvalidIdentityException__Init __F__peer__InvalidIdentityException__i;

#ifdef __APPLE__
extern "C" { void __F__peer__InvalidIdentityException__initializer() {} }
#endif

peer::PeerAccessException::PeerAccessException(const ::std::string& __ice_reason) :
#if (defined(_MSC_VER) && (_MSC_VER < 1300)) // VC++ 6 compiler bug
    PeerException(__ice_reason)
#else
    ::peer::PeerException(__ice_reason)
#endif
{
}

static const char* __peer__PeerAccessException_name = "peer::PeerAccessException";

const ::std::string
peer::PeerAccessException::ice_name() const
{
    return __peer__PeerAccessException_name;
}

::Ice::Exception*
peer::PeerAccessException::ice_clone() const
{
    return new PeerAccessException(*this);
}

void
peer::PeerAccessException::ice_throw() const
{
    throw *this;
}

void
peer::PeerAccessException::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(::std::string("::peer::PeerAccessException"), false);
    __os->startWriteSlice();
    __os->endWriteSlice();
#if (defined(_MSC_VER) && (_MSC_VER < 1300)) // VC++ 6 compiler bug
    PeerException::__write(__os);
#else
    ::peer::PeerException::__write(__os);
#endif
}

void
peer::PeerAccessException::__read(::IceInternal::BasicStream* __is, bool __rid)
{
    if(__rid)
    {
	::std::string myId;
	__is->read(myId, false);
    }
    __is->startReadSlice();
    __is->endReadSlice();
#if (defined(_MSC_VER) && (_MSC_VER < 1300)) // VC++ 6 compiler bug
    PeerException::__read(__is, true);
#else
    ::peer::PeerException::__read(__is, true);
#endif
}

void
peer::PeerAccessException::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "exception peer::PeerAccessException was not generated with stream support";
    throw ex;
}

void
peer::PeerAccessException::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "exception peer::PeerAccessException was not generated with stream support";
    throw ex;
}

struct __F__peer__PeerAccessException : public ::IceInternal::UserExceptionFactory
{
    virtual void
    createAndThrow()
    {
	throw ::peer::PeerAccessException();
    }
};

static ::IceInternal::UserExceptionFactoryPtr __F__peer__PeerAccessException__Ptr = new __F__peer__PeerAccessException;

const ::IceInternal::UserExceptionFactoryPtr&
peer::PeerAccessException::ice_factory()
{
    return __F__peer__PeerAccessException__Ptr;
}

class __F__peer__PeerAccessException__Init
{
public:

    __F__peer__PeerAccessException__Init()
    {
	::IceInternal::factoryTable->addExceptionFactory("::peer::PeerAccessException", ::peer::PeerAccessException::ice_factory());
    }

    ~__F__peer__PeerAccessException__Init()
    {
	::IceInternal::factoryTable->removeExceptionFactory("::peer::PeerAccessException");
    }
};

static __F__peer__PeerAccessException__Init __F__peer__PeerAccessException__i;

#ifdef __APPLE__
extern "C" { void __F__peer__PeerAccessException__initializer() {} }
#endif

peer::PeerUnavailableException::PeerUnavailableException(const ::std::string& __ice_reason) :
#if (defined(_MSC_VER) && (_MSC_VER < 1300)) // VC++ 6 compiler bug
    PeerException(__ice_reason)
#else
    ::peer::PeerException(__ice_reason)
#endif
{
}

static const char* __peer__PeerUnavailableException_name = "peer::PeerUnavailableException";

const ::std::string
peer::PeerUnavailableException::ice_name() const
{
    return __peer__PeerUnavailableException_name;
}

::Ice::Exception*
peer::PeerUnavailableException::ice_clone() const
{
    return new PeerUnavailableException(*this);
}

void
peer::PeerUnavailableException::ice_throw() const
{
    throw *this;
}

void
peer::PeerUnavailableException::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(::std::string("::peer::PeerUnavailableException"), false);
    __os->startWriteSlice();
    __os->endWriteSlice();
#if (defined(_MSC_VER) && (_MSC_VER < 1300)) // VC++ 6 compiler bug
    PeerException::__write(__os);
#else
    ::peer::PeerException::__write(__os);
#endif
}

void
peer::PeerUnavailableException::__read(::IceInternal::BasicStream* __is, bool __rid)
{
    if(__rid)
    {
	::std::string myId;
	__is->read(myId, false);
    }
    __is->startReadSlice();
    __is->endReadSlice();
#if (defined(_MSC_VER) && (_MSC_VER < 1300)) // VC++ 6 compiler bug
    PeerException::__read(__is, true);
#else
    ::peer::PeerException::__read(__is, true);
#endif
}

void
peer::PeerUnavailableException::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "exception peer::PeerUnavailableException was not generated with stream support";
    throw ex;
}

void
peer::PeerUnavailableException::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "exception peer::PeerUnavailableException was not generated with stream support";
    throw ex;
}

struct __F__peer__PeerUnavailableException : public ::IceInternal::UserExceptionFactory
{
    virtual void
    createAndThrow()
    {
	throw ::peer::PeerUnavailableException();
    }
};

static ::IceInternal::UserExceptionFactoryPtr __F__peer__PeerUnavailableException__Ptr = new __F__peer__PeerUnavailableException;

const ::IceInternal::UserExceptionFactoryPtr&
peer::PeerUnavailableException::ice_factory()
{
    return __F__peer__PeerUnavailableException__Ptr;
}

class __F__peer__PeerUnavailableException__Init
{
public:

    __F__peer__PeerUnavailableException__Init()
    {
	::IceInternal::factoryTable->addExceptionFactory("::peer::PeerUnavailableException", ::peer::PeerUnavailableException::ice_factory());
    }

    ~__F__peer__PeerUnavailableException__Init()
    {
	::IceInternal::factoryTable->removeExceptionFactory("::peer::PeerUnavailableException");
    }
};

static __F__peer__PeerUnavailableException__Init __F__peer__PeerUnavailableException__i;

#ifdef __APPLE__
extern "C" { void __F__peer__PeerUnavailableException__initializer() {} }
#endif

peer::PeerConnectionFailedException::PeerConnectionFailedException(const ::std::string& __ice_reason) :
#if (defined(_MSC_VER) && (_MSC_VER < 1300)) // VC++ 6 compiler bug
    PeerException(__ice_reason)
#else
    ::peer::PeerException(__ice_reason)
#endif
{
}

static const char* __peer__PeerConnectionFailedException_name = "peer::PeerConnectionFailedException";

const ::std::string
peer::PeerConnectionFailedException::ice_name() const
{
    return __peer__PeerConnectionFailedException_name;
}

::Ice::Exception*
peer::PeerConnectionFailedException::ice_clone() const
{
    return new PeerConnectionFailedException(*this);
}

void
peer::PeerConnectionFailedException::ice_throw() const
{
    throw *this;
}

void
peer::PeerConnectionFailedException::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(::std::string("::peer::PeerConnectionFailedException"), false);
    __os->startWriteSlice();
    __os->endWriteSlice();
#if (defined(_MSC_VER) && (_MSC_VER < 1300)) // VC++ 6 compiler bug
    PeerException::__write(__os);
#else
    ::peer::PeerException::__write(__os);
#endif
}

void
peer::PeerConnectionFailedException::__read(::IceInternal::BasicStream* __is, bool __rid)
{
    if(__rid)
    {
	::std::string myId;
	__is->read(myId, false);
    }
    __is->startReadSlice();
    __is->endReadSlice();
#if (defined(_MSC_VER) && (_MSC_VER < 1300)) // VC++ 6 compiler bug
    PeerException::__read(__is, true);
#else
    ::peer::PeerException::__read(__is, true);
#endif
}

void
peer::PeerConnectionFailedException::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "exception peer::PeerConnectionFailedException was not generated with stream support";
    throw ex;
}

void
peer::PeerConnectionFailedException::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "exception peer::PeerConnectionFailedException was not generated with stream support";
    throw ex;
}

struct __F__peer__PeerConnectionFailedException : public ::IceInternal::UserExceptionFactory
{
    virtual void
    createAndThrow()
    {
	throw ::peer::PeerConnectionFailedException();
    }
};

static ::IceInternal::UserExceptionFactoryPtr __F__peer__PeerConnectionFailedException__Ptr = new __F__peer__PeerConnectionFailedException;

const ::IceInternal::UserExceptionFactoryPtr&
peer::PeerConnectionFailedException::ice_factory()
{
    return __F__peer__PeerConnectionFailedException__Ptr;
}

class __F__peer__PeerConnectionFailedException__Init
{
public:

    __F__peer__PeerConnectionFailedException__Init()
    {
	::IceInternal::factoryTable->addExceptionFactory("::peer::PeerConnectionFailedException", ::peer::PeerConnectionFailedException::ice_factory());
    }

    ~__F__peer__PeerConnectionFailedException__Init()
    {
	::IceInternal::factoryTable->removeExceptionFactory("::peer::PeerConnectionFailedException");
    }
};

static __F__peer__PeerConnectionFailedException__Init __F__peer__PeerConnectionFailedException__i;

#ifdef __APPLE__
extern "C" { void __F__peer__PeerConnectionFailedException__initializer() {} }
#endif

peer::DuplicateConnectionException::DuplicateConnectionException(const ::std::string& __ice_reason) :
#if (defined(_MSC_VER) && (_MSC_VER < 1300)) // VC++ 6 compiler bug
    PeerException(__ice_reason)
#else
    ::peer::PeerException(__ice_reason)
#endif
{
}

static const char* __peer__DuplicateConnectionException_name = "peer::DuplicateConnectionException";

const ::std::string
peer::DuplicateConnectionException::ice_name() const
{
    return __peer__DuplicateConnectionException_name;
}

::Ice::Exception*
peer::DuplicateConnectionException::ice_clone() const
{
    return new DuplicateConnectionException(*this);
}

void
peer::DuplicateConnectionException::ice_throw() const
{
    throw *this;
}

void
peer::DuplicateConnectionException::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(::std::string("::peer::DuplicateConnectionException"), false);
    __os->startWriteSlice();
    __os->endWriteSlice();
#if (defined(_MSC_VER) && (_MSC_VER < 1300)) // VC++ 6 compiler bug
    PeerException::__write(__os);
#else
    ::peer::PeerException::__write(__os);
#endif
}

void
peer::DuplicateConnectionException::__read(::IceInternal::BasicStream* __is, bool __rid)
{
    if(__rid)
    {
	::std::string myId;
	__is->read(myId, false);
    }
    __is->startReadSlice();
    __is->endReadSlice();
#if (defined(_MSC_VER) && (_MSC_VER < 1300)) // VC++ 6 compiler bug
    PeerException::__read(__is, true);
#else
    ::peer::PeerException::__read(__is, true);
#endif
}

void
peer::DuplicateConnectionException::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "exception peer::DuplicateConnectionException was not generated with stream support";
    throw ex;
}

void
peer::DuplicateConnectionException::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "exception peer::DuplicateConnectionException was not generated with stream support";
    throw ex;
}

struct __F__peer__DuplicateConnectionException : public ::IceInternal::UserExceptionFactory
{
    virtual void
    createAndThrow()
    {
	throw ::peer::DuplicateConnectionException();
    }
};

static ::IceInternal::UserExceptionFactoryPtr __F__peer__DuplicateConnectionException__Ptr = new __F__peer__DuplicateConnectionException;

const ::IceInternal::UserExceptionFactoryPtr&
peer::DuplicateConnectionException::ice_factory()
{
    return __F__peer__DuplicateConnectionException__Ptr;
}

class __F__peer__DuplicateConnectionException__Init
{
public:

    __F__peer__DuplicateConnectionException__Init()
    {
	::IceInternal::factoryTable->addExceptionFactory("::peer::DuplicateConnectionException", ::peer::DuplicateConnectionException::ice_factory());
    }

    ~__F__peer__DuplicateConnectionException__Init()
    {
	::IceInternal::factoryTable->removeExceptionFactory("::peer::DuplicateConnectionException");
    }
};

static __F__peer__DuplicateConnectionException__Init __F__peer__DuplicateConnectionException__i;

#ifdef __APPLE__
extern "C" { void __F__peer__DuplicateConnectionException__initializer() {} }
#endif

peer::RegistrationException::RegistrationException(const ::std::string& __ice_reason) :
#if (defined(_MSC_VER) && (_MSC_VER < 1300)) // VC++ 6 compiler bug
    PeerException(__ice_reason)
#else
    ::peer::PeerException(__ice_reason)
#endif
{
}

static const char* __peer__RegistrationException_name = "peer::RegistrationException";

const ::std::string
peer::RegistrationException::ice_name() const
{
    return __peer__RegistrationException_name;
}

::Ice::Exception*
peer::RegistrationException::ice_clone() const
{
    return new RegistrationException(*this);
}

void
peer::RegistrationException::ice_throw() const
{
    throw *this;
}

void
peer::RegistrationException::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(::std::string("::peer::RegistrationException"), false);
    __os->startWriteSlice();
    __os->endWriteSlice();
#if (defined(_MSC_VER) && (_MSC_VER < 1300)) // VC++ 6 compiler bug
    PeerException::__write(__os);
#else
    ::peer::PeerException::__write(__os);
#endif
}

void
peer::RegistrationException::__read(::IceInternal::BasicStream* __is, bool __rid)
{
    if(__rid)
    {
	::std::string myId;
	__is->read(myId, false);
    }
    __is->startReadSlice();
    __is->endReadSlice();
#if (defined(_MSC_VER) && (_MSC_VER < 1300)) // VC++ 6 compiler bug
    PeerException::__read(__is, true);
#else
    ::peer::PeerException::__read(__is, true);
#endif
}

void
peer::RegistrationException::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "exception peer::RegistrationException was not generated with stream support";
    throw ex;
}

void
peer::RegistrationException::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "exception peer::RegistrationException was not generated with stream support";
    throw ex;
}

struct __F__peer__RegistrationException : public ::IceInternal::UserExceptionFactory
{
    virtual void
    createAndThrow()
    {
	throw ::peer::RegistrationException();
    }
};

static ::IceInternal::UserExceptionFactoryPtr __F__peer__RegistrationException__Ptr = new __F__peer__RegistrationException;

const ::IceInternal::UserExceptionFactoryPtr&
peer::RegistrationException::ice_factory()
{
    return __F__peer__RegistrationException__Ptr;
}

class __F__peer__RegistrationException__Init
{
public:

    __F__peer__RegistrationException__Init()
    {
	::IceInternal::factoryTable->addExceptionFactory("::peer::RegistrationException", ::peer::RegistrationException::ice_factory());
    }

    ~__F__peer__RegistrationException__Init()
    {
	::IceInternal::factoryTable->removeExceptionFactory("::peer::RegistrationException");
    }
};

static __F__peer__RegistrationException__Init __F__peer__RegistrationException__i;

#ifdef __APPLE__
extern "C" { void __F__peer__RegistrationException__initializer() {} }
#endif

void
peer::__write(::IceInternal::BasicStream* __os, const ::peer::PeerIdentifier* begin, const ::peer::PeerIdentifier* end, ::peer::__U__PeerIdentifierSet)
{
    ::Ice::Int size = static_cast< ::Ice::Int>(end - begin);
    __os->writeSize(size);
    for(int i = 0; i < size; ++i)
    {
	begin[i].__write(__os);
    }
}

void
peer::__read(::IceInternal::BasicStream* __is, ::peer::PeerIdentifierSet& v, ::peer::__U__PeerIdentifierSet)
{
    ::Ice::Int sz;
    __is->readSize(sz);
    __is->startSeq(sz, 3);
    v.resize(sz);
    for(int i = 0; i < sz; ++i)
    {
	v[i].__read(__is);
	__is->checkSeq();
	__is->endElement();
    }
    __is->endSeq(sz);
}

void
peer::AMI_UserConnectionEventHandler_forcedLogoutNotification::__invoke(const ::peer::UserConnectionEventHandlerPrx& __prx, const ::Ice::Context& __ctx)
{
    try
    {
	__prepare(__prx, __peer__UserConnectionEventHandler__forcedLogoutNotification_name, ::Ice::Normal, __ctx);
	__os->endWriteEncaps();
    }
    catch(const ::Ice::LocalException& __ex)
    {
	__finished(__ex);
	return;
    }
    __send();
}

void
peer::AMI_UserConnectionEventHandler_forcedLogoutNotification::__response(bool __ok)
{
    try
    {
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::Ice::UserException& __ex)
	    {
		throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
	    }
	}
    }
    catch(const ::Ice::LocalException& __ex)
    {
	__finished(__ex);
	return;
    }
    ice_response();
}

void
peer::AMI_PeerConnectionEventHandler_peerConnected::__invoke(const ::peer::PeerConnectionEventHandlerPrx& __prx, const ::std::string& peerUserId, ::peer::PeerAccessLevel accessLevel, const ::Ice::ObjectPrx& peerProxy, const ::Ice::Context& __ctx)
{
    try
    {
	__prepare(__prx, __peer__PeerConnectionEventHandler__peerConnected_name, ::Ice::Normal, __ctx);
	__os->write(peerUserId);
	::peer::__write(__os, accessLevel);
	__os->write(peerProxy);
	__os->endWriteEncaps();
    }
    catch(const ::Ice::LocalException& __ex)
    {
	__finished(__ex);
	return;
    }
    __send();
}

void
peer::AMI_PeerConnectionEventHandler_peerConnected::__response(bool __ok)
{
    try
    {
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::Ice::UserException& __ex)
	    {
		throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
	    }
	}
    }
    catch(const ::Ice::LocalException& __ex)
    {
	__finished(__ex);
	return;
    }
    ice_response();
}

void
peer::AMI_PeerConnectionEventHandler_peerConnectedNoProxy::__invoke(const ::peer::PeerConnectionEventHandlerPrx& __prx, const ::std::string& peerUserId, ::peer::PeerAccessLevel accessLevel, const ::Ice::Context& __ctx)
{
    try
    {
	__prepare(__prx, __peer__PeerConnectionEventHandler__peerConnectedNoProxy_name, ::Ice::Normal, __ctx);
	__os->write(peerUserId);
	::peer::__write(__os, accessLevel);
	__os->endWriteEncaps();
    }
    catch(const ::Ice::LocalException& __ex)
    {
	__finished(__ex);
	return;
    }
    __send();
}

void
peer::AMI_PeerConnectionEventHandler_peerConnectedNoProxy::__response(bool __ok)
{
    try
    {
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::Ice::UserException& __ex)
	    {
		throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
	    }
	}
    }
    catch(const ::Ice::LocalException& __ex)
    {
	__finished(__ex);
	return;
    }
    ice_response();
}

void
peer::AMI_PeerConnectionEventHandler_peerDisconnected::__invoke(const ::peer::PeerConnectionEventHandlerPrx& __prx, const ::std::string& peerUserId, const ::Ice::Context& __ctx)
{
    try
    {
	__prepare(__prx, __peer__PeerConnectionEventHandler__peerDisconnected_name, ::Ice::Normal, __ctx);
	__os->write(peerUserId);
	__os->endWriteEncaps();
    }
    catch(const ::Ice::LocalException& __ex)
    {
	__finished(__ex);
	return;
    }
    __send();
}

void
peer::AMI_PeerConnectionEventHandler_peerDisconnected::__response(bool __ok)
{
    try
    {
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::Ice::UserException& __ex)
	    {
		throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
	    }
	}
    }
    catch(const ::Ice::LocalException& __ex)
    {
	__finished(__ex);
	return;
    }
    ice_response();
}

void
IceProxy::peer::UserConnectionEventHandler::forcedLogoutNotification(const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::peer::UserConnectionEventHandler* __del = dynamic_cast< ::IceDelegate::peer::UserConnectionEventHandler*>(__delBase.get());
	    __del->forcedLogoutNotification(__ctx);
	    return;
	}
	catch(const ::IceInternal::LocalExceptionWrapper& __ex)
	{
	    __handleExceptionWrapper(__ex);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    __handleException(__ex, __cnt);
	}
    }
}

void
IceProxy::peer::UserConnectionEventHandler::forcedLogoutNotification_async(const ::peer::AMI_UserConnectionEventHandler_forcedLogoutNotificationPtr& __cb)
{
    forcedLogoutNotification_async(__cb, __defaultContext());
}

void
IceProxy::peer::UserConnectionEventHandler::forcedLogoutNotification_async(const ::peer::AMI_UserConnectionEventHandler_forcedLogoutNotificationPtr& __cb, const ::Ice::Context& __ctx)
{
    __cb->__invoke(this, __ctx);
}

const ::std::string&
IceProxy::peer::UserConnectionEventHandler::ice_staticId()
{
    return ::peer::UserConnectionEventHandler::ice_staticId();
}

::IceInternal::Handle< ::IceDelegateM::Ice::Object>
IceProxy::peer::UserConnectionEventHandler::__createDelegateM()
{
    return ::IceInternal::Handle< ::IceDelegateM::Ice::Object>(new ::IceDelegateM::peer::UserConnectionEventHandler);
}

::IceInternal::Handle< ::IceDelegateD::Ice::Object>
IceProxy::peer::UserConnectionEventHandler::__createDelegateD()
{
    return ::IceInternal::Handle< ::IceDelegateD::Ice::Object>(new ::IceDelegateD::peer::UserConnectionEventHandler);
}

bool
IceProxy::peer::operator==(const ::IceProxy::peer::UserConnectionEventHandler& l, const ::IceProxy::peer::UserConnectionEventHandler& r)
{
    return static_cast<const ::IceProxy::Ice::Object&>(l) == static_cast<const ::IceProxy::Ice::Object&>(r);
}

bool
IceProxy::peer::operator!=(const ::IceProxy::peer::UserConnectionEventHandler& l, const ::IceProxy::peer::UserConnectionEventHandler& r)
{
    return static_cast<const ::IceProxy::Ice::Object&>(l) != static_cast<const ::IceProxy::Ice::Object&>(r);
}

bool
IceProxy::peer::operator<(const ::IceProxy::peer::UserConnectionEventHandler& l, const ::IceProxy::peer::UserConnectionEventHandler& r)
{
    return static_cast<const ::IceProxy::Ice::Object&>(l) < static_cast<const ::IceProxy::Ice::Object&>(r);
}

bool
IceProxy::peer::operator<=(const ::IceProxy::peer::UserConnectionEventHandler& l, const ::IceProxy::peer::UserConnectionEventHandler& r)
{
    return l < r || l == r;
}

bool
IceProxy::peer::operator>(const ::IceProxy::peer::UserConnectionEventHandler& l, const ::IceProxy::peer::UserConnectionEventHandler& r)
{
    return !(l < r) && !(l == r);
}

bool
IceProxy::peer::operator>=(const ::IceProxy::peer::UserConnectionEventHandler& l, const ::IceProxy::peer::UserConnectionEventHandler& r)
{
    return !(l < r);
}

void
IceProxy::peer::PeerConnectionEventHandler::peerConnected(const ::std::string& peerUserId, ::peer::PeerAccessLevel accessLevel, const ::Ice::ObjectPrx& peerProxy, const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::peer::PeerConnectionEventHandler* __del = dynamic_cast< ::IceDelegate::peer::PeerConnectionEventHandler*>(__delBase.get());
	    __del->peerConnected(peerUserId, accessLevel, peerProxy, __ctx);
	    return;
	}
	catch(const ::IceInternal::LocalExceptionWrapper& __ex)
	{
	    __handleExceptionWrapper(__ex);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    __handleException(__ex, __cnt);
	}
    }
}

void
IceProxy::peer::PeerConnectionEventHandler::peerConnected_async(const ::peer::AMI_PeerConnectionEventHandler_peerConnectedPtr& __cb, const ::std::string& peerUserId, ::peer::PeerAccessLevel accessLevel, const ::Ice::ObjectPrx& peerProxy)
{
    peerConnected_async(__cb, peerUserId, accessLevel, peerProxy, __defaultContext());
}

void
IceProxy::peer::PeerConnectionEventHandler::peerConnected_async(const ::peer::AMI_PeerConnectionEventHandler_peerConnectedPtr& __cb, const ::std::string& peerUserId, ::peer::PeerAccessLevel accessLevel, const ::Ice::ObjectPrx& peerProxy, const ::Ice::Context& __ctx)
{
    __cb->__invoke(this, peerUserId, accessLevel, peerProxy, __ctx);
}

void
IceProxy::peer::PeerConnectionEventHandler::peerConnectedNoProxy(const ::std::string& peerUserId, ::peer::PeerAccessLevel accessLevel, const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::peer::PeerConnectionEventHandler* __del = dynamic_cast< ::IceDelegate::peer::PeerConnectionEventHandler*>(__delBase.get());
	    __del->peerConnectedNoProxy(peerUserId, accessLevel, __ctx);
	    return;
	}
	catch(const ::IceInternal::LocalExceptionWrapper& __ex)
	{
	    __handleExceptionWrapper(__ex);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    __handleException(__ex, __cnt);
	}
    }
}

void
IceProxy::peer::PeerConnectionEventHandler::peerConnectedNoProxy_async(const ::peer::AMI_PeerConnectionEventHandler_peerConnectedNoProxyPtr& __cb, const ::std::string& peerUserId, ::peer::PeerAccessLevel accessLevel)
{
    peerConnectedNoProxy_async(__cb, peerUserId, accessLevel, __defaultContext());
}

void
IceProxy::peer::PeerConnectionEventHandler::peerConnectedNoProxy_async(const ::peer::AMI_PeerConnectionEventHandler_peerConnectedNoProxyPtr& __cb, const ::std::string& peerUserId, ::peer::PeerAccessLevel accessLevel, const ::Ice::Context& __ctx)
{
    __cb->__invoke(this, peerUserId, accessLevel, __ctx);
}

void
IceProxy::peer::PeerConnectionEventHandler::peerDisconnected(const ::std::string& peerUserId, const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::peer::PeerConnectionEventHandler* __del = dynamic_cast< ::IceDelegate::peer::PeerConnectionEventHandler*>(__delBase.get());
	    __del->peerDisconnected(peerUserId, __ctx);
	    return;
	}
	catch(const ::IceInternal::LocalExceptionWrapper& __ex)
	{
	    __handleExceptionWrapper(__ex);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    __handleException(__ex, __cnt);
	}
    }
}

void
IceProxy::peer::PeerConnectionEventHandler::peerDisconnected_async(const ::peer::AMI_PeerConnectionEventHandler_peerDisconnectedPtr& __cb, const ::std::string& peerUserId)
{
    peerDisconnected_async(__cb, peerUserId, __defaultContext());
}

void
IceProxy::peer::PeerConnectionEventHandler::peerDisconnected_async(const ::peer::AMI_PeerConnectionEventHandler_peerDisconnectedPtr& __cb, const ::std::string& peerUserId, const ::Ice::Context& __ctx)
{
    __cb->__invoke(this, peerUserId, __ctx);
}

const ::std::string&
IceProxy::peer::PeerConnectionEventHandler::ice_staticId()
{
    return ::peer::PeerConnectionEventHandler::ice_staticId();
}

::IceInternal::Handle< ::IceDelegateM::Ice::Object>
IceProxy::peer::PeerConnectionEventHandler::__createDelegateM()
{
    return ::IceInternal::Handle< ::IceDelegateM::Ice::Object>(new ::IceDelegateM::peer::PeerConnectionEventHandler);
}

::IceInternal::Handle< ::IceDelegateD::Ice::Object>
IceProxy::peer::PeerConnectionEventHandler::__createDelegateD()
{
    return ::IceInternal::Handle< ::IceDelegateD::Ice::Object>(new ::IceDelegateD::peer::PeerConnectionEventHandler);
}

bool
IceProxy::peer::operator==(const ::IceProxy::peer::PeerConnectionEventHandler& l, const ::IceProxy::peer::PeerConnectionEventHandler& r)
{
    return static_cast<const ::IceProxy::Ice::Object&>(l) == static_cast<const ::IceProxy::Ice::Object&>(r);
}

bool
IceProxy::peer::operator!=(const ::IceProxy::peer::PeerConnectionEventHandler& l, const ::IceProxy::peer::PeerConnectionEventHandler& r)
{
    return static_cast<const ::IceProxy::Ice::Object&>(l) != static_cast<const ::IceProxy::Ice::Object&>(r);
}

bool
IceProxy::peer::operator<(const ::IceProxy::peer::PeerConnectionEventHandler& l, const ::IceProxy::peer::PeerConnectionEventHandler& r)
{
    return static_cast<const ::IceProxy::Ice::Object&>(l) < static_cast<const ::IceProxy::Ice::Object&>(r);
}

bool
IceProxy::peer::operator<=(const ::IceProxy::peer::PeerConnectionEventHandler& l, const ::IceProxy::peer::PeerConnectionEventHandler& r)
{
    return l < r || l == r;
}

bool
IceProxy::peer::operator>(const ::IceProxy::peer::PeerConnectionEventHandler& l, const ::IceProxy::peer::PeerConnectionEventHandler& r)
{
    return !(l < r) && !(l == r);
}

bool
IceProxy::peer::operator>=(const ::IceProxy::peer::PeerConnectionEventHandler& l, const ::IceProxy::peer::PeerConnectionEventHandler& r)
{
    return !(l < r);
}

const ::std::string&
IceProxy::peer::ConnectionEventHandler::ice_staticId()
{
    return ::peer::ConnectionEventHandler::ice_staticId();
}

::IceInternal::Handle< ::IceDelegateM::Ice::Object>
IceProxy::peer::ConnectionEventHandler::__createDelegateM()
{
    return ::IceInternal::Handle< ::IceDelegateM::Ice::Object>(new ::IceDelegateM::peer::ConnectionEventHandler);
}

::IceInternal::Handle< ::IceDelegateD::Ice::Object>
IceProxy::peer::ConnectionEventHandler::__createDelegateD()
{
    return ::IceInternal::Handle< ::IceDelegateD::Ice::Object>(new ::IceDelegateD::peer::ConnectionEventHandler);
}

bool
IceProxy::peer::operator==(const ::IceProxy::peer::ConnectionEventHandler& l, const ::IceProxy::peer::ConnectionEventHandler& r)
{
    return static_cast<const ::IceProxy::Ice::Object&>(l) == static_cast<const ::IceProxy::Ice::Object&>(r);
}

bool
IceProxy::peer::operator!=(const ::IceProxy::peer::ConnectionEventHandler& l, const ::IceProxy::peer::ConnectionEventHandler& r)
{
    return static_cast<const ::IceProxy::Ice::Object&>(l) != static_cast<const ::IceProxy::Ice::Object&>(r);
}

bool
IceProxy::peer::operator<(const ::IceProxy::peer::ConnectionEventHandler& l, const ::IceProxy::peer::ConnectionEventHandler& r)
{
    return static_cast<const ::IceProxy::Ice::Object&>(l) < static_cast<const ::IceProxy::Ice::Object&>(r);
}

bool
IceProxy::peer::operator<=(const ::IceProxy::peer::ConnectionEventHandler& l, const ::IceProxy::peer::ConnectionEventHandler& r)
{
    return l < r || l == r;
}

bool
IceProxy::peer::operator>(const ::IceProxy::peer::ConnectionEventHandler& l, const ::IceProxy::peer::ConnectionEventHandler& r)
{
    return !(l < r) && !(l == r);
}

bool
IceProxy::peer::operator>=(const ::IceProxy::peer::ConnectionEventHandler& l, const ::IceProxy::peer::ConnectionEventHandler& r)
{
    return !(l < r);
}

void
IceProxy::peer::PeerRegistrationHandler::registerCallbacks(const ::Ice::ObjectPrx& selfCallbackProxy, const ::peer::ConnectionEventHandlerPrx& connectionEventHandlerProxy, const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__peer__PeerRegistrationHandler__registerCallbacks_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::peer::PeerRegistrationHandler* __del = dynamic_cast< ::IceDelegate::peer::PeerRegistrationHandler*>(__delBase.get());
	    __del->registerCallbacks(selfCallbackProxy, connectionEventHandlerProxy, __ctx);
	    return;
	}
	catch(const ::IceInternal::LocalExceptionWrapper& __ex)
	{
	    __handleExceptionWrapper(__ex);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    __handleException(__ex, __cnt);
	}
    }
}

void
IceProxy::peer::PeerRegistrationHandler::registerProxy(const ::Ice::ObjectPrx& proxy, const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__peer__PeerRegistrationHandler__registerProxy_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::peer::PeerRegistrationHandler* __del = dynamic_cast< ::IceDelegate::peer::PeerRegistrationHandler*>(__delBase.get());
	    __del->registerProxy(proxy, __ctx);
	    return;
	}
	catch(const ::IceInternal::LocalExceptionWrapper& __ex)
	{
	    __handleExceptionWrapper(__ex);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    __handleException(__ex, __cnt);
	}
    }
}

void
IceProxy::peer::PeerRegistrationHandler::registerProxies(const ::peer::ObjectProxyArray& proxies, const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__peer__PeerRegistrationHandler__registerProxies_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::peer::PeerRegistrationHandler* __del = dynamic_cast< ::IceDelegate::peer::PeerRegistrationHandler*>(__delBase.get());
	    __del->registerProxies(proxies, __ctx);
	    return;
	}
	catch(const ::IceInternal::LocalExceptionWrapper& __ex)
	{
	    __handleExceptionWrapper(__ex);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    __handleException(__ex, __cnt);
	}
    }
}

::Ice::ObjectPrx
IceProxy::peer::PeerRegistrationHandler::getPeerProxy(const ::std::string& peerUserId, const ::Ice::Identity& privateProxyIdentity, const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__peer__PeerRegistrationHandler__getPeerProxy_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::peer::PeerRegistrationHandler* __del = dynamic_cast< ::IceDelegate::peer::PeerRegistrationHandler*>(__delBase.get());
	    return __del->getPeerProxy(peerUserId, privateProxyIdentity, __ctx);
	}
	catch(const ::IceInternal::LocalExceptionWrapper& __ex)
	{
	    __handleExceptionWrapper(__ex);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    __handleException(__ex, __cnt);
	}
    }
}

::peer::IdentityToProxyMap
IceProxy::peer::PeerRegistrationHandler::getPeerProxies(const ::std::string& peerUserId, const ::peer::IdentityArray& privateProxyIdentities, const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__peer__PeerRegistrationHandler__getPeerProxies_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::peer::PeerRegistrationHandler* __del = dynamic_cast< ::IceDelegate::peer::PeerRegistrationHandler*>(__delBase.get());
	    return __del->getPeerProxies(peerUserId, privateProxyIdentities, __ctx);
	}
	catch(const ::IceInternal::LocalExceptionWrapper& __ex)
	{
	    __handleExceptionWrapper(__ex);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    __handleException(__ex, __cnt);
	}
    }
}

const ::std::string&
IceProxy::peer::PeerRegistrationHandler::ice_staticId()
{
    return ::peer::PeerRegistrationHandler::ice_staticId();
}

::IceInternal::Handle< ::IceDelegateM::Ice::Object>
IceProxy::peer::PeerRegistrationHandler::__createDelegateM()
{
    return ::IceInternal::Handle< ::IceDelegateM::Ice::Object>(new ::IceDelegateM::peer::PeerRegistrationHandler);
}

::IceInternal::Handle< ::IceDelegateD::Ice::Object>
IceProxy::peer::PeerRegistrationHandler::__createDelegateD()
{
    return ::IceInternal::Handle< ::IceDelegateD::Ice::Object>(new ::IceDelegateD::peer::PeerRegistrationHandler);
}

bool
IceProxy::peer::operator==(const ::IceProxy::peer::PeerRegistrationHandler& l, const ::IceProxy::peer::PeerRegistrationHandler& r)
{
    return static_cast<const ::IceProxy::Ice::Object&>(l) == static_cast<const ::IceProxy::Ice::Object&>(r);
}

bool
IceProxy::peer::operator!=(const ::IceProxy::peer::PeerRegistrationHandler& l, const ::IceProxy::peer::PeerRegistrationHandler& r)
{
    return static_cast<const ::IceProxy::Ice::Object&>(l) != static_cast<const ::IceProxy::Ice::Object&>(r);
}

bool
IceProxy::peer::operator<(const ::IceProxy::peer::PeerRegistrationHandler& l, const ::IceProxy::peer::PeerRegistrationHandler& r)
{
    return static_cast<const ::IceProxy::Ice::Object&>(l) < static_cast<const ::IceProxy::Ice::Object&>(r);
}

bool
IceProxy::peer::operator<=(const ::IceProxy::peer::PeerRegistrationHandler& l, const ::IceProxy::peer::PeerRegistrationHandler& r)
{
    return l < r || l == r;
}

bool
IceProxy::peer::operator>(const ::IceProxy::peer::PeerRegistrationHandler& l, const ::IceProxy::peer::PeerRegistrationHandler& r)
{
    return !(l < r) && !(l == r);
}

bool
IceProxy::peer::operator>=(const ::IceProxy::peer::PeerRegistrationHandler& l, const ::IceProxy::peer::PeerRegistrationHandler& r)
{
    return !(l < r);
}

::peer::PeerIdentifierSet
IceProxy::peer::UserSession::getMyAvailablePeers(const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__peer__UserSession__getMyAvailablePeers_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::peer::UserSession* __del = dynamic_cast< ::IceDelegate::peer::UserSession*>(__delBase.get());
	    return __del->getMyAvailablePeers(__ctx);
	}
	catch(const ::IceInternal::LocalExceptionWrapper& __ex)
	{
	    __handleExceptionWrapper(__ex);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    __handleException(__ex, __cnt);
	}
    }
}

::Ice::ObjectPrx
IceProxy::peer::UserSession::connectToPeer(const ::std::string& peerUserId, const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__peer__UserSession__connectToPeer_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::peer::UserSession* __del = dynamic_cast< ::IceDelegate::peer::UserSession*>(__delBase.get());
	    return __del->connectToPeer(peerUserId, __ctx);
	}
	catch(const ::IceInternal::LocalExceptionWrapper& __ex)
	{
	    __handleExceptionWrapper(__ex);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    __handleException(__ex, __cnt);
	}
    }
}

::peer::PeerIdentifierSet
IceProxy::peer::UserSession::getConnectedPeers(const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__peer__UserSession__getConnectedPeers_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::peer::UserSession* __del = dynamic_cast< ::IceDelegate::peer::UserSession*>(__delBase.get());
	    return __del->getConnectedPeers(__ctx);
	}
	catch(const ::IceInternal::LocalExceptionWrapper& __ex)
	{
	    __handleExceptionWrapper(__ex);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    __handleException(__ex, __cnt);
	}
    }
}

void
IceProxy::peer::UserSession::disconnectFromPeer(const ::std::string& peerUserId, const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::peer::UserSession* __del = dynamic_cast< ::IceDelegate::peer::UserSession*>(__delBase.get());
	    __del->disconnectFromPeer(peerUserId, __ctx);
	    return;
	}
	catch(const ::IceInternal::LocalExceptionWrapper& __ex)
	{
	    __handleExceptionWrapper(__ex);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    __handleException(__ex, __cnt);
	}
    }
}

void
IceProxy::peer::UserSession::disconnectFromPeers(const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::peer::UserSession* __del = dynamic_cast< ::IceDelegate::peer::UserSession*>(__delBase.get());
	    __del->disconnectFromPeers(__ctx);
	    return;
	}
	catch(const ::IceInternal::LocalExceptionWrapper& __ex)
	{
	    __handleExceptionWrapper(__ex);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    __handleException(__ex, __cnt);
	}
    }
}

const ::std::string&
IceProxy::peer::UserSession::ice_staticId()
{
    return ::peer::UserSession::ice_staticId();
}

::IceInternal::Handle< ::IceDelegateM::Ice::Object>
IceProxy::peer::UserSession::__createDelegateM()
{
    return ::IceInternal::Handle< ::IceDelegateM::Ice::Object>(new ::IceDelegateM::peer::UserSession);
}

::IceInternal::Handle< ::IceDelegateD::Ice::Object>
IceProxy::peer::UserSession::__createDelegateD()
{
    return ::IceInternal::Handle< ::IceDelegateD::Ice::Object>(new ::IceDelegateD::peer::UserSession);
}

bool
IceProxy::peer::operator==(const ::IceProxy::peer::UserSession& l, const ::IceProxy::peer::UserSession& r)
{
    return static_cast<const ::IceProxy::Ice::Object&>(l) == static_cast<const ::IceProxy::Ice::Object&>(r);
}

bool
IceProxy::peer::operator!=(const ::IceProxy::peer::UserSession& l, const ::IceProxy::peer::UserSession& r)
{
    return static_cast<const ::IceProxy::Ice::Object&>(l) != static_cast<const ::IceProxy::Ice::Object&>(r);
}

bool
IceProxy::peer::operator<(const ::IceProxy::peer::UserSession& l, const ::IceProxy::peer::UserSession& r)
{
    return static_cast<const ::IceProxy::Ice::Object&>(l) < static_cast<const ::IceProxy::Ice::Object&>(r);
}

bool
IceProxy::peer::operator<=(const ::IceProxy::peer::UserSession& l, const ::IceProxy::peer::UserSession& r)
{
    return l < r || l == r;
}

bool
IceProxy::peer::operator>(const ::IceProxy::peer::UserSession& l, const ::IceProxy::peer::UserSession& r)
{
    return !(l < r) && !(l == r);
}

bool
IceProxy::peer::operator>=(const ::IceProxy::peer::UserSession& l, const ::IceProxy::peer::UserSession& r)
{
    return !(l < r);
}

void
IceDelegateM::peer::UserConnectionEventHandler::forcedLogoutNotification(const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __peer__UserConnectionEventHandler__forcedLogoutNotification_name, ::Ice::Normal, __context, __compress);
    bool __ok = __og.invoke();
    try
    {
	::IceInternal::BasicStream* __is = __og.is();
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::Ice::UserException& __ex)
	    {
		throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
	    }
	}
    }
    catch(const ::Ice::LocalException& __ex)
    {
	throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

void
IceDelegateM::peer::PeerConnectionEventHandler::peerConnected(const ::std::string& peerUserId, ::peer::PeerAccessLevel accessLevel, const ::Ice::ObjectPrx& peerProxy, const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __peer__PeerConnectionEventHandler__peerConnected_name, ::Ice::Normal, __context, __compress);
    try
    {
	::IceInternal::BasicStream* __os = __og.os();
	__os->write(peerUserId);
	::peer::__write(__os, accessLevel);
	__os->write(peerProxy);
    }
    catch(const ::Ice::LocalException& __ex)
    {
	__og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
	::IceInternal::BasicStream* __is = __og.is();
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::Ice::UserException& __ex)
	    {
		throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
	    }
	}
    }
    catch(const ::Ice::LocalException& __ex)
    {
	throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

void
IceDelegateM::peer::PeerConnectionEventHandler::peerConnectedNoProxy(const ::std::string& peerUserId, ::peer::PeerAccessLevel accessLevel, const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __peer__PeerConnectionEventHandler__peerConnectedNoProxy_name, ::Ice::Normal, __context, __compress);
    try
    {
	::IceInternal::BasicStream* __os = __og.os();
	__os->write(peerUserId);
	::peer::__write(__os, accessLevel);
    }
    catch(const ::Ice::LocalException& __ex)
    {
	__og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
	::IceInternal::BasicStream* __is = __og.is();
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::Ice::UserException& __ex)
	    {
		throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
	    }
	}
    }
    catch(const ::Ice::LocalException& __ex)
    {
	throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

void
IceDelegateM::peer::PeerConnectionEventHandler::peerDisconnected(const ::std::string& peerUserId, const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __peer__PeerConnectionEventHandler__peerDisconnected_name, ::Ice::Normal, __context, __compress);
    try
    {
	::IceInternal::BasicStream* __os = __og.os();
	__os->write(peerUserId);
    }
    catch(const ::Ice::LocalException& __ex)
    {
	__og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
	::IceInternal::BasicStream* __is = __og.is();
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::Ice::UserException& __ex)
	    {
		throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
	    }
	}
    }
    catch(const ::Ice::LocalException& __ex)
    {
	throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

void
IceDelegateM::peer::PeerRegistrationHandler::registerCallbacks(const ::Ice::ObjectPrx& selfCallbackProxy, const ::peer::ConnectionEventHandlerPrx& connectionEventHandlerProxy, const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __peer__PeerRegistrationHandler__registerCallbacks_name, ::Ice::Normal, __context, __compress);
    try
    {
	::IceInternal::BasicStream* __os = __og.os();
	__os->write(selfCallbackProxy);
	::peer::__write(__os, connectionEventHandlerProxy);
    }
    catch(const ::Ice::LocalException& __ex)
    {
	__og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
	::IceInternal::BasicStream* __is = __og.is();
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::peer::RegistrationException&)
	    {
		throw;
	    }
	    catch(const ::Ice::UserException& __ex)
	    {
		throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
	    }
	}
    }
    catch(const ::Ice::LocalException& __ex)
    {
	throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

void
IceDelegateM::peer::PeerRegistrationHandler::registerProxy(const ::Ice::ObjectPrx& proxy, const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __peer__PeerRegistrationHandler__registerProxy_name, ::Ice::Normal, __context, __compress);
    try
    {
	::IceInternal::BasicStream* __os = __og.os();
	__os->write(proxy);
    }
    catch(const ::Ice::LocalException& __ex)
    {
	__og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
	::IceInternal::BasicStream* __is = __og.is();
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::peer::RegistrationException&)
	    {
		throw;
	    }
	    catch(const ::Ice::UserException& __ex)
	    {
		throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
	    }
	}
    }
    catch(const ::Ice::LocalException& __ex)
    {
	throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

void
IceDelegateM::peer::PeerRegistrationHandler::registerProxies(const ::peer::ObjectProxyArray& proxies, const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __peer__PeerRegistrationHandler__registerProxies_name, ::Ice::Normal, __context, __compress);
    try
    {
	::IceInternal::BasicStream* __os = __og.os();
	if(proxies.size() == 0)
	{
	    __os->writeSize(0);
	}
	else
	{
	    ::peer::__write(__os, &proxies[0], &proxies[0] + proxies.size(), ::peer::__U__ObjectProxyArray());
	}
    }
    catch(const ::Ice::LocalException& __ex)
    {
	__og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
	::IceInternal::BasicStream* __is = __og.is();
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::peer::RegistrationException&)
	    {
		throw;
	    }
	    catch(const ::Ice::UserException& __ex)
	    {
		throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
	    }
	}
    }
    catch(const ::Ice::LocalException& __ex)
    {
	throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

::Ice::ObjectPrx
IceDelegateM::peer::PeerRegistrationHandler::getPeerProxy(const ::std::string& peerUserId, const ::Ice::Identity& privateProxyIdentity, const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __peer__PeerRegistrationHandler__getPeerProxy_name, ::Ice::Normal, __context, __compress);
    try
    {
	::IceInternal::BasicStream* __os = __og.os();
	__os->write(peerUserId);
	privateProxyIdentity.__write(__os);
    }
    catch(const ::Ice::LocalException& __ex)
    {
	__og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
	::IceInternal::BasicStream* __is = __og.is();
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::peer::InvalidIdentityException&)
	    {
		throw;
	    }
	    catch(const ::peer::PeerAccessException&)
	    {
		throw;
	    }
	    catch(const ::Ice::UserException& __ex)
	    {
		throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
	    }
	}
	::Ice::ObjectPrx __ret;
	__is->read(__ret);
	return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
	throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

::peer::IdentityToProxyMap
IceDelegateM::peer::PeerRegistrationHandler::getPeerProxies(const ::std::string& peerUserId, const ::peer::IdentityArray& privateProxyIdentities, const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __peer__PeerRegistrationHandler__getPeerProxies_name, ::Ice::Normal, __context, __compress);
    try
    {
	::IceInternal::BasicStream* __os = __og.os();
	__os->write(peerUserId);
	if(privateProxyIdentities.size() == 0)
	{
	    __os->writeSize(0);
	}
	else
	{
	    ::peer::__write(__os, &privateProxyIdentities[0], &privateProxyIdentities[0] + privateProxyIdentities.size(), ::peer::__U__IdentityArray());
	}
    }
    catch(const ::Ice::LocalException& __ex)
    {
	__og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
	::IceInternal::BasicStream* __is = __og.is();
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::peer::InvalidIdentityException&)
	    {
		throw;
	    }
	    catch(const ::peer::PeerAccessException&)
	    {
		throw;
	    }
	    catch(const ::Ice::UserException& __ex)
	    {
		throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
	    }
	}
	::peer::IdentityToProxyMap __ret;
	::peer::__read(__is, __ret, ::peer::__U__IdentityToProxyMap());
	return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
	throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

::peer::PeerIdentifierSet
IceDelegateM::peer::UserSession::getMyAvailablePeers(const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __peer__UserSession__getMyAvailablePeers_name, ::Ice::Normal, __context, __compress);
    bool __ok = __og.invoke();
    try
    {
	::IceInternal::BasicStream* __is = __og.is();
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::peer::PeerException&)
	    {
		throw;
	    }
	    catch(const ::Ice::UserException& __ex)
	    {
		throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
	    }
	}
	::peer::PeerIdentifierSet __ret;
	::peer::__read(__is, __ret, ::peer::__U__PeerIdentifierSet());
	return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
	throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

::Ice::ObjectPrx
IceDelegateM::peer::UserSession::connectToPeer(const ::std::string& peerUserId, const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __peer__UserSession__connectToPeer_name, ::Ice::Normal, __context, __compress);
    try
    {
	::IceInternal::BasicStream* __os = __og.os();
	__os->write(peerUserId);
    }
    catch(const ::Ice::LocalException& __ex)
    {
	__og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
	::IceInternal::BasicStream* __is = __og.is();
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::peer::DuplicateConnectionException&)
	    {
		throw;
	    }
	    catch(const ::peer::PeerAccessException&)
	    {
		throw;
	    }
	    catch(const ::peer::PeerConnectionFailedException&)
	    {
		throw;
	    }
	    catch(const ::peer::PeerUnavailableException&)
	    {
		throw;
	    }
	    catch(const ::Ice::UserException& __ex)
	    {
		throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
	    }
	}
	::Ice::ObjectPrx __ret;
	__is->read(__ret);
	return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
	throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

::peer::PeerIdentifierSet
IceDelegateM::peer::UserSession::getConnectedPeers(const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __peer__UserSession__getConnectedPeers_name, ::Ice::Normal, __context, __compress);
    bool __ok = __og.invoke();
    try
    {
	::IceInternal::BasicStream* __is = __og.is();
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::peer::PeerException&)
	    {
		throw;
	    }
	    catch(const ::Ice::UserException& __ex)
	    {
		throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
	    }
	}
	::peer::PeerIdentifierSet __ret;
	::peer::__read(__is, __ret, ::peer::__U__PeerIdentifierSet());
	return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
	throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

void
IceDelegateM::peer::UserSession::disconnectFromPeer(const ::std::string& peerUserId, const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __peer__UserSession__disconnectFromPeer_name, ::Ice::Normal, __context, __compress);
    try
    {
	::IceInternal::BasicStream* __os = __og.os();
	__os->write(peerUserId);
    }
    catch(const ::Ice::LocalException& __ex)
    {
	__og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
	::IceInternal::BasicStream* __is = __og.is();
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::Ice::UserException& __ex)
	    {
		throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
	    }
	}
    }
    catch(const ::Ice::LocalException& __ex)
    {
	throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

void
IceDelegateM::peer::UserSession::disconnectFromPeers(const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __peer__UserSession__disconnectFromPeers_name, ::Ice::Normal, __context, __compress);
    bool __ok = __og.invoke();
    try
    {
	::IceInternal::BasicStream* __is = __og.is();
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::Ice::UserException& __ex)
	    {
		throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
	    }
	}
    }
    catch(const ::Ice::LocalException& __ex)
    {
	throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

void
IceDelegateD::peer::UserConnectionEventHandler::forcedLogoutNotification(const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __peer__UserConnectionEventHandler__forcedLogoutNotification_name, ::Ice::Normal, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::peer::UserConnectionEventHandler* __servant = dynamic_cast< ::peer::UserConnectionEventHandler*>(__direct.servant().get());
	if(!__servant)
	{
	    ::Ice::OperationNotExistException __opEx(__FILE__, __LINE__);
	    __opEx.id = __current.id;
	    __opEx.facet = __current.facet;
	    __opEx.operation = __current.operation;
	    throw __opEx;
	}
	try
	{
	    __servant->forcedLogoutNotification(__current);
	    return;
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

void
IceDelegateD::peer::PeerConnectionEventHandler::peerConnected(const ::std::string& peerUserId, ::peer::PeerAccessLevel accessLevel, const ::Ice::ObjectPrx& peerProxy, const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __peer__PeerConnectionEventHandler__peerConnected_name, ::Ice::Normal, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::peer::PeerConnectionEventHandler* __servant = dynamic_cast< ::peer::PeerConnectionEventHandler*>(__direct.servant().get());
	if(!__servant)
	{
	    ::Ice::OperationNotExistException __opEx(__FILE__, __LINE__);
	    __opEx.id = __current.id;
	    __opEx.facet = __current.facet;
	    __opEx.operation = __current.operation;
	    throw __opEx;
	}
	try
	{
	    __servant->peerConnected(peerUserId, accessLevel, peerProxy, __current);
	    return;
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

void
IceDelegateD::peer::PeerConnectionEventHandler::peerConnectedNoProxy(const ::std::string& peerUserId, ::peer::PeerAccessLevel accessLevel, const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __peer__PeerConnectionEventHandler__peerConnectedNoProxy_name, ::Ice::Normal, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::peer::PeerConnectionEventHandler* __servant = dynamic_cast< ::peer::PeerConnectionEventHandler*>(__direct.servant().get());
	if(!__servant)
	{
	    ::Ice::OperationNotExistException __opEx(__FILE__, __LINE__);
	    __opEx.id = __current.id;
	    __opEx.facet = __current.facet;
	    __opEx.operation = __current.operation;
	    throw __opEx;
	}
	try
	{
	    __servant->peerConnectedNoProxy(peerUserId, accessLevel, __current);
	    return;
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

void
IceDelegateD::peer::PeerConnectionEventHandler::peerDisconnected(const ::std::string& peerUserId, const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __peer__PeerConnectionEventHandler__peerDisconnected_name, ::Ice::Normal, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::peer::PeerConnectionEventHandler* __servant = dynamic_cast< ::peer::PeerConnectionEventHandler*>(__direct.servant().get());
	if(!__servant)
	{
	    ::Ice::OperationNotExistException __opEx(__FILE__, __LINE__);
	    __opEx.id = __current.id;
	    __opEx.facet = __current.facet;
	    __opEx.operation = __current.operation;
	    throw __opEx;
	}
	try
	{
	    __servant->peerDisconnected(peerUserId, __current);
	    return;
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

void
IceDelegateD::peer::PeerRegistrationHandler::registerCallbacks(const ::Ice::ObjectPrx& selfCallbackProxy, const ::peer::ConnectionEventHandlerPrx& connectionEventHandlerProxy, const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __peer__PeerRegistrationHandler__registerCallbacks_name, ::Ice::Normal, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::peer::PeerRegistrationHandler* __servant = dynamic_cast< ::peer::PeerRegistrationHandler*>(__direct.servant().get());
	if(!__servant)
	{
	    ::Ice::OperationNotExistException __opEx(__FILE__, __LINE__);
	    __opEx.id = __current.id;
	    __opEx.facet = __current.facet;
	    __opEx.operation = __current.operation;
	    throw __opEx;
	}
	try
	{
	    __servant->registerCallbacks(selfCallbackProxy, connectionEventHandlerProxy, __current);
	    return;
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

void
IceDelegateD::peer::PeerRegistrationHandler::registerProxy(const ::Ice::ObjectPrx& proxy, const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __peer__PeerRegistrationHandler__registerProxy_name, ::Ice::Normal, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::peer::PeerRegistrationHandler* __servant = dynamic_cast< ::peer::PeerRegistrationHandler*>(__direct.servant().get());
	if(!__servant)
	{
	    ::Ice::OperationNotExistException __opEx(__FILE__, __LINE__);
	    __opEx.id = __current.id;
	    __opEx.facet = __current.facet;
	    __opEx.operation = __current.operation;
	    throw __opEx;
	}
	try
	{
	    __servant->registerProxy(proxy, __current);
	    return;
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

void
IceDelegateD::peer::PeerRegistrationHandler::registerProxies(const ::peer::ObjectProxyArray& proxies, const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __peer__PeerRegistrationHandler__registerProxies_name, ::Ice::Normal, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::peer::PeerRegistrationHandler* __servant = dynamic_cast< ::peer::PeerRegistrationHandler*>(__direct.servant().get());
	if(!__servant)
	{
	    ::Ice::OperationNotExistException __opEx(__FILE__, __LINE__);
	    __opEx.id = __current.id;
	    __opEx.facet = __current.facet;
	    __opEx.operation = __current.operation;
	    throw __opEx;
	}
	try
	{
	    __servant->registerProxies(proxies, __current);
	    return;
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

::Ice::ObjectPrx
IceDelegateD::peer::PeerRegistrationHandler::getPeerProxy(const ::std::string& peerUserId, const ::Ice::Identity& privateProxyIdentity, const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __peer__PeerRegistrationHandler__getPeerProxy_name, ::Ice::Normal, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::peer::PeerRegistrationHandler* __servant = dynamic_cast< ::peer::PeerRegistrationHandler*>(__direct.servant().get());
	if(!__servant)
	{
	    ::Ice::OperationNotExistException __opEx(__FILE__, __LINE__);
	    __opEx.id = __current.id;
	    __opEx.facet = __current.facet;
	    __opEx.operation = __current.operation;
	    throw __opEx;
	}
	try
	{
	    return __servant->getPeerProxy(peerUserId, privateProxyIdentity, __current);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

::peer::IdentityToProxyMap
IceDelegateD::peer::PeerRegistrationHandler::getPeerProxies(const ::std::string& peerUserId, const ::peer::IdentityArray& privateProxyIdentities, const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __peer__PeerRegistrationHandler__getPeerProxies_name, ::Ice::Normal, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::peer::PeerRegistrationHandler* __servant = dynamic_cast< ::peer::PeerRegistrationHandler*>(__direct.servant().get());
	if(!__servant)
	{
	    ::Ice::OperationNotExistException __opEx(__FILE__, __LINE__);
	    __opEx.id = __current.id;
	    __opEx.facet = __current.facet;
	    __opEx.operation = __current.operation;
	    throw __opEx;
	}
	try
	{
	    return __servant->getPeerProxies(peerUserId, privateProxyIdentities, __current);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

::peer::PeerIdentifierSet
IceDelegateD::peer::UserSession::getMyAvailablePeers(const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __peer__UserSession__getMyAvailablePeers_name, ::Ice::Normal, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::peer::UserSession* __servant = dynamic_cast< ::peer::UserSession*>(__direct.servant().get());
	if(!__servant)
	{
	    ::Ice::OperationNotExistException __opEx(__FILE__, __LINE__);
	    __opEx.id = __current.id;
	    __opEx.facet = __current.facet;
	    __opEx.operation = __current.operation;
	    throw __opEx;
	}
	try
	{
	    return __servant->getMyAvailablePeers(__current);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

::Ice::ObjectPrx
IceDelegateD::peer::UserSession::connectToPeer(const ::std::string& peerUserId, const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __peer__UserSession__connectToPeer_name, ::Ice::Normal, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::peer::UserSession* __servant = dynamic_cast< ::peer::UserSession*>(__direct.servant().get());
	if(!__servant)
	{
	    ::Ice::OperationNotExistException __opEx(__FILE__, __LINE__);
	    __opEx.id = __current.id;
	    __opEx.facet = __current.facet;
	    __opEx.operation = __current.operation;
	    throw __opEx;
	}
	try
	{
	    return __servant->connectToPeer(peerUserId, __current);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

::peer::PeerIdentifierSet
IceDelegateD::peer::UserSession::getConnectedPeers(const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __peer__UserSession__getConnectedPeers_name, ::Ice::Normal, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::peer::UserSession* __servant = dynamic_cast< ::peer::UserSession*>(__direct.servant().get());
	if(!__servant)
	{
	    ::Ice::OperationNotExistException __opEx(__FILE__, __LINE__);
	    __opEx.id = __current.id;
	    __opEx.facet = __current.facet;
	    __opEx.operation = __current.operation;
	    throw __opEx;
	}
	try
	{
	    return __servant->getConnectedPeers(__current);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

void
IceDelegateD::peer::UserSession::disconnectFromPeer(const ::std::string& peerUserId, const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __peer__UserSession__disconnectFromPeer_name, ::Ice::Normal, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::peer::UserSession* __servant = dynamic_cast< ::peer::UserSession*>(__direct.servant().get());
	if(!__servant)
	{
	    ::Ice::OperationNotExistException __opEx(__FILE__, __LINE__);
	    __opEx.id = __current.id;
	    __opEx.facet = __current.facet;
	    __opEx.operation = __current.operation;
	    throw __opEx;
	}
	try
	{
	    __servant->disconnectFromPeer(peerUserId, __current);
	    return;
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

void
IceDelegateD::peer::UserSession::disconnectFromPeers(const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __peer__UserSession__disconnectFromPeers_name, ::Ice::Normal, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::peer::UserSession* __servant = dynamic_cast< ::peer::UserSession*>(__direct.servant().get());
	if(!__servant)
	{
	    ::Ice::OperationNotExistException __opEx(__FILE__, __LINE__);
	    __opEx.id = __current.id;
	    __opEx.facet = __current.facet;
	    __opEx.operation = __current.operation;
	    throw __opEx;
	}
	try
	{
	    __servant->disconnectFromPeers(__current);
	    return;
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

::Ice::ObjectPtr
peer::UserConnectionEventHandler::ice_clone() const
{
    throw ::Ice::CloneNotImplementedException(__FILE__, __LINE__);
    return 0; // to avoid a warning with some compilers
}

static const ::std::string __peer__UserConnectionEventHandler_ids[2] =
{
    "::Ice::Object",
    "::peer::UserConnectionEventHandler"
};

bool
peer::UserConnectionEventHandler::ice_isA(const ::std::string& _s, const ::Ice::Current&) const
{
    return ::std::binary_search(__peer__UserConnectionEventHandler_ids, __peer__UserConnectionEventHandler_ids + 2, _s);
}

::std::vector< ::std::string>
peer::UserConnectionEventHandler::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&__peer__UserConnectionEventHandler_ids[0], &__peer__UserConnectionEventHandler_ids[2]);
}

const ::std::string&
peer::UserConnectionEventHandler::ice_id(const ::Ice::Current&) const
{
    return __peer__UserConnectionEventHandler_ids[1];
}

const ::std::string&
peer::UserConnectionEventHandler::ice_staticId()
{
    return __peer__UserConnectionEventHandler_ids[1];
}

::IceInternal::DispatchStatus
peer::UserConnectionEventHandler::___forcedLogoutNotification(::IceInternal::Incoming&, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    forcedLogoutNotification(__current);
    return ::IceInternal::DispatchOK;
}

static ::std::string __peer__UserConnectionEventHandler_all[] =
{
    "forcedLogoutNotification",
    "ice_id",
    "ice_ids",
    "ice_isA",
    "ice_ping"
};

::IceInternal::DispatchStatus
peer::UserConnectionEventHandler::__dispatch(::IceInternal::Incoming& in, const ::Ice::Current& current)
{
    ::std::pair< ::std::string*, ::std::string*> r = ::std::equal_range(__peer__UserConnectionEventHandler_all, __peer__UserConnectionEventHandler_all + 5, current.operation);
    if(r.first == r.second)
    {
	return ::IceInternal::DispatchOperationNotExist;
    }

    switch(r.first - __peer__UserConnectionEventHandler_all)
    {
	case 0:
	{
	    return ___forcedLogoutNotification(in, current);
	}
	case 1:
	{
	    return ___ice_id(in, current);
	}
	case 2:
	{
	    return ___ice_ids(in, current);
	}
	case 3:
	{
	    return ___ice_isA(in, current);
	}
	case 4:
	{
	    return ___ice_ping(in, current);
	}
    }

    assert(false);
    return ::IceInternal::DispatchOperationNotExist;
}

void
peer::UserConnectionEventHandler::__write(::IceInternal::BasicStream* __os) const
{
    __os->writeTypeId(ice_staticId());
    __os->startWriteSlice();
    __os->endWriteSlice();
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    Object::__write(__os);
#else
    ::Ice::Object::__write(__os);
#endif
}

void
peer::UserConnectionEventHandler::__read(::IceInternal::BasicStream* __is, bool __rid)
{
    if(__rid)
    {
	::std::string myId;
	__is->readTypeId(myId);
    }
    __is->startReadSlice();
    __is->endReadSlice();
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    Object::__read(__is, true);
#else
    ::Ice::Object::__read(__is, true);
#endif
}

void
peer::UserConnectionEventHandler::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type peer::UserConnectionEventHandler was not generated with stream support";
    throw ex;
}

void
peer::UserConnectionEventHandler::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type peer::UserConnectionEventHandler was not generated with stream support";
    throw ex;
}

void 
peer::__patch__UserConnectionEventHandlerPtr(void* __addr, ::Ice::ObjectPtr& v)
{
    ::peer::UserConnectionEventHandlerPtr* p = static_cast< ::peer::UserConnectionEventHandlerPtr*>(__addr);
    assert(p);
    *p = ::peer::UserConnectionEventHandlerPtr::dynamicCast(v);
    if(v && !*p)
    {
	::Ice::NoObjectFactoryException e(__FILE__, __LINE__);
	e.type = ::peer::UserConnectionEventHandler::ice_staticId();
	throw e;
    }
}

bool
peer::operator==(const ::peer::UserConnectionEventHandler& l, const ::peer::UserConnectionEventHandler& r)
{
    return static_cast<const ::Ice::Object&>(l) == static_cast<const ::Ice::Object&>(r);
}

bool
peer::operator!=(const ::peer::UserConnectionEventHandler& l, const ::peer::UserConnectionEventHandler& r)
{
    return static_cast<const ::Ice::Object&>(l) != static_cast<const ::Ice::Object&>(r);
}

bool
peer::operator<(const ::peer::UserConnectionEventHandler& l, const ::peer::UserConnectionEventHandler& r)
{
    return static_cast<const ::Ice::Object&>(l) < static_cast<const ::Ice::Object&>(r);
}

bool
peer::operator<=(const ::peer::UserConnectionEventHandler& l, const ::peer::UserConnectionEventHandler& r)
{
    return l < r || l == r;
}

bool
peer::operator>(const ::peer::UserConnectionEventHandler& l, const ::peer::UserConnectionEventHandler& r)
{
    return !(l < r) && !(l == r);
}

bool
peer::operator>=(const ::peer::UserConnectionEventHandler& l, const ::peer::UserConnectionEventHandler& r)
{
    return !(l < r);
}

::Ice::ObjectPtr
peer::PeerConnectionEventHandler::ice_clone() const
{
    throw ::Ice::CloneNotImplementedException(__FILE__, __LINE__);
    return 0; // to avoid a warning with some compilers
}

static const ::std::string __peer__PeerConnectionEventHandler_ids[2] =
{
    "::Ice::Object",
    "::peer::PeerConnectionEventHandler"
};

bool
peer::PeerConnectionEventHandler::ice_isA(const ::std::string& _s, const ::Ice::Current&) const
{
    return ::std::binary_search(__peer__PeerConnectionEventHandler_ids, __peer__PeerConnectionEventHandler_ids + 2, _s);
}

::std::vector< ::std::string>
peer::PeerConnectionEventHandler::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&__peer__PeerConnectionEventHandler_ids[0], &__peer__PeerConnectionEventHandler_ids[2]);
}

const ::std::string&
peer::PeerConnectionEventHandler::ice_id(const ::Ice::Current&) const
{
    return __peer__PeerConnectionEventHandler_ids[1];
}

const ::std::string&
peer::PeerConnectionEventHandler::ice_staticId()
{
    return __peer__PeerConnectionEventHandler_ids[1];
}

::IceInternal::DispatchStatus
peer::PeerConnectionEventHandler::___peerConnected(::IceInternal::Incoming&__inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::std::string peerUserId;
    ::peer::PeerAccessLevel accessLevel;
    ::Ice::ObjectPrx peerProxy;
    __is->read(peerUserId);
    ::peer::__read(__is, accessLevel);
    __is->read(peerProxy);
    peerConnected(peerUserId, accessLevel, peerProxy, __current);
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
peer::PeerConnectionEventHandler::___peerConnectedNoProxy(::IceInternal::Incoming&__inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::std::string peerUserId;
    ::peer::PeerAccessLevel accessLevel;
    __is->read(peerUserId);
    ::peer::__read(__is, accessLevel);
    peerConnectedNoProxy(peerUserId, accessLevel, __current);
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
peer::PeerConnectionEventHandler::___peerDisconnected(::IceInternal::Incoming&__inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::std::string peerUserId;
    __is->read(peerUserId);
    peerDisconnected(peerUserId, __current);
    return ::IceInternal::DispatchOK;
}

static ::std::string __peer__PeerConnectionEventHandler_all[] =
{
    "ice_id",
    "ice_ids",
    "ice_isA",
    "ice_ping",
    "peerConnected",
    "peerConnectedNoProxy",
    "peerDisconnected"
};

::IceInternal::DispatchStatus
peer::PeerConnectionEventHandler::__dispatch(::IceInternal::Incoming& in, const ::Ice::Current& current)
{
    ::std::pair< ::std::string*, ::std::string*> r = ::std::equal_range(__peer__PeerConnectionEventHandler_all, __peer__PeerConnectionEventHandler_all + 7, current.operation);
    if(r.first == r.second)
    {
	return ::IceInternal::DispatchOperationNotExist;
    }

    switch(r.first - __peer__PeerConnectionEventHandler_all)
    {
	case 0:
	{
	    return ___ice_id(in, current);
	}
	case 1:
	{
	    return ___ice_ids(in, current);
	}
	case 2:
	{
	    return ___ice_isA(in, current);
	}
	case 3:
	{
	    return ___ice_ping(in, current);
	}
	case 4:
	{
	    return ___peerConnected(in, current);
	}
	case 5:
	{
	    return ___peerConnectedNoProxy(in, current);
	}
	case 6:
	{
	    return ___peerDisconnected(in, current);
	}
    }

    assert(false);
    return ::IceInternal::DispatchOperationNotExist;
}

void
peer::PeerConnectionEventHandler::__write(::IceInternal::BasicStream* __os) const
{
    __os->writeTypeId(ice_staticId());
    __os->startWriteSlice();
    __os->endWriteSlice();
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    Object::__write(__os);
#else
    ::Ice::Object::__write(__os);
#endif
}

void
peer::PeerConnectionEventHandler::__read(::IceInternal::BasicStream* __is, bool __rid)
{
    if(__rid)
    {
	::std::string myId;
	__is->readTypeId(myId);
    }
    __is->startReadSlice();
    __is->endReadSlice();
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    Object::__read(__is, true);
#else
    ::Ice::Object::__read(__is, true);
#endif
}

void
peer::PeerConnectionEventHandler::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type peer::PeerConnectionEventHandler was not generated with stream support";
    throw ex;
}

void
peer::PeerConnectionEventHandler::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type peer::PeerConnectionEventHandler was not generated with stream support";
    throw ex;
}

void 
peer::__patch__PeerConnectionEventHandlerPtr(void* __addr, ::Ice::ObjectPtr& v)
{
    ::peer::PeerConnectionEventHandlerPtr* p = static_cast< ::peer::PeerConnectionEventHandlerPtr*>(__addr);
    assert(p);
    *p = ::peer::PeerConnectionEventHandlerPtr::dynamicCast(v);
    if(v && !*p)
    {
	::Ice::NoObjectFactoryException e(__FILE__, __LINE__);
	e.type = ::peer::PeerConnectionEventHandler::ice_staticId();
	throw e;
    }
}

bool
peer::operator==(const ::peer::PeerConnectionEventHandler& l, const ::peer::PeerConnectionEventHandler& r)
{
    return static_cast<const ::Ice::Object&>(l) == static_cast<const ::Ice::Object&>(r);
}

bool
peer::operator!=(const ::peer::PeerConnectionEventHandler& l, const ::peer::PeerConnectionEventHandler& r)
{
    return static_cast<const ::Ice::Object&>(l) != static_cast<const ::Ice::Object&>(r);
}

bool
peer::operator<(const ::peer::PeerConnectionEventHandler& l, const ::peer::PeerConnectionEventHandler& r)
{
    return static_cast<const ::Ice::Object&>(l) < static_cast<const ::Ice::Object&>(r);
}

bool
peer::operator<=(const ::peer::PeerConnectionEventHandler& l, const ::peer::PeerConnectionEventHandler& r)
{
    return l < r || l == r;
}

bool
peer::operator>(const ::peer::PeerConnectionEventHandler& l, const ::peer::PeerConnectionEventHandler& r)
{
    return !(l < r) && !(l == r);
}

bool
peer::operator>=(const ::peer::PeerConnectionEventHandler& l, const ::peer::PeerConnectionEventHandler& r)
{
    return !(l < r);
}

::Ice::ObjectPtr
peer::ConnectionEventHandler::ice_clone() const
{
    throw ::Ice::CloneNotImplementedException(__FILE__, __LINE__);
    return 0; // to avoid a warning with some compilers
}

static const ::std::string __peer__ConnectionEventHandler_ids[4] =
{
    "::Ice::Object",
    "::peer::ConnectionEventHandler",
    "::peer::PeerConnectionEventHandler",
    "::peer::UserConnectionEventHandler"
};

bool
peer::ConnectionEventHandler::ice_isA(const ::std::string& _s, const ::Ice::Current&) const
{
    return ::std::binary_search(__peer__ConnectionEventHandler_ids, __peer__ConnectionEventHandler_ids + 4, _s);
}

::std::vector< ::std::string>
peer::ConnectionEventHandler::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&__peer__ConnectionEventHandler_ids[0], &__peer__ConnectionEventHandler_ids[4]);
}

const ::std::string&
peer::ConnectionEventHandler::ice_id(const ::Ice::Current&) const
{
    return __peer__ConnectionEventHandler_ids[1];
}

const ::std::string&
peer::ConnectionEventHandler::ice_staticId()
{
    return __peer__ConnectionEventHandler_ids[1];
}

static ::std::string __peer__ConnectionEventHandler_all[] =
{
    "forcedLogoutNotification",
    "ice_id",
    "ice_ids",
    "ice_isA",
    "ice_ping",
    "peerConnected",
    "peerConnectedNoProxy",
    "peerDisconnected"
};

::IceInternal::DispatchStatus
peer::ConnectionEventHandler::__dispatch(::IceInternal::Incoming& in, const ::Ice::Current& current)
{
    ::std::pair< ::std::string*, ::std::string*> r = ::std::equal_range(__peer__ConnectionEventHandler_all, __peer__ConnectionEventHandler_all + 8, current.operation);
    if(r.first == r.second)
    {
	return ::IceInternal::DispatchOperationNotExist;
    }

    switch(r.first - __peer__ConnectionEventHandler_all)
    {
	case 0:
	{
	    return ___forcedLogoutNotification(in, current);
	}
	case 1:
	{
	    return ___ice_id(in, current);
	}
	case 2:
	{
	    return ___ice_ids(in, current);
	}
	case 3:
	{
	    return ___ice_isA(in, current);
	}
	case 4:
	{
	    return ___ice_ping(in, current);
	}
	case 5:
	{
	    return ___peerConnected(in, current);
	}
	case 6:
	{
	    return ___peerConnectedNoProxy(in, current);
	}
	case 7:
	{
	    return ___peerDisconnected(in, current);
	}
    }

    assert(false);
    return ::IceInternal::DispatchOperationNotExist;
}

void
peer::ConnectionEventHandler::__write(::IceInternal::BasicStream* __os) const
{
    __os->writeTypeId(ice_staticId());
    __os->startWriteSlice();
    __os->endWriteSlice();
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    Object::__write(__os);
#else
    ::Ice::Object::__write(__os);
#endif
}

void
peer::ConnectionEventHandler::__read(::IceInternal::BasicStream* __is, bool __rid)
{
    if(__rid)
    {
	::std::string myId;
	__is->readTypeId(myId);
    }
    __is->startReadSlice();
    __is->endReadSlice();
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    Object::__read(__is, true);
#else
    ::Ice::Object::__read(__is, true);
#endif
}

void
peer::ConnectionEventHandler::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type peer::ConnectionEventHandler was not generated with stream support";
    throw ex;
}

void
peer::ConnectionEventHandler::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type peer::ConnectionEventHandler was not generated with stream support";
    throw ex;
}

void 
peer::__patch__ConnectionEventHandlerPtr(void* __addr, ::Ice::ObjectPtr& v)
{
    ::peer::ConnectionEventHandlerPtr* p = static_cast< ::peer::ConnectionEventHandlerPtr*>(__addr);
    assert(p);
    *p = ::peer::ConnectionEventHandlerPtr::dynamicCast(v);
    if(v && !*p)
    {
	::Ice::NoObjectFactoryException e(__FILE__, __LINE__);
	e.type = ::peer::ConnectionEventHandler::ice_staticId();
	throw e;
    }
}

bool
peer::operator==(const ::peer::ConnectionEventHandler& l, const ::peer::ConnectionEventHandler& r)
{
    return static_cast<const ::Ice::Object&>(l) == static_cast<const ::Ice::Object&>(r);
}

bool
peer::operator!=(const ::peer::ConnectionEventHandler& l, const ::peer::ConnectionEventHandler& r)
{
    return static_cast<const ::Ice::Object&>(l) != static_cast<const ::Ice::Object&>(r);
}

bool
peer::operator<(const ::peer::ConnectionEventHandler& l, const ::peer::ConnectionEventHandler& r)
{
    return static_cast<const ::Ice::Object&>(l) < static_cast<const ::Ice::Object&>(r);
}

bool
peer::operator<=(const ::peer::ConnectionEventHandler& l, const ::peer::ConnectionEventHandler& r)
{
    return l < r || l == r;
}

bool
peer::operator>(const ::peer::ConnectionEventHandler& l, const ::peer::ConnectionEventHandler& r)
{
    return !(l < r) && !(l == r);
}

bool
peer::operator>=(const ::peer::ConnectionEventHandler& l, const ::peer::ConnectionEventHandler& r)
{
    return !(l < r);
}

::Ice::ObjectPtr
peer::PeerRegistrationHandler::ice_clone() const
{
    throw ::Ice::CloneNotImplementedException(__FILE__, __LINE__);
    return 0; // to avoid a warning with some compilers
}

static const ::std::string __peer__PeerRegistrationHandler_ids[2] =
{
    "::Ice::Object",
    "::peer::PeerRegistrationHandler"
};

bool
peer::PeerRegistrationHandler::ice_isA(const ::std::string& _s, const ::Ice::Current&) const
{
    return ::std::binary_search(__peer__PeerRegistrationHandler_ids, __peer__PeerRegistrationHandler_ids + 2, _s);
}

::std::vector< ::std::string>
peer::PeerRegistrationHandler::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&__peer__PeerRegistrationHandler_ids[0], &__peer__PeerRegistrationHandler_ids[2]);
}

const ::std::string&
peer::PeerRegistrationHandler::ice_id(const ::Ice::Current&) const
{
    return __peer__PeerRegistrationHandler_ids[1];
}

const ::std::string&
peer::PeerRegistrationHandler::ice_staticId()
{
    return __peer__PeerRegistrationHandler_ids[1];
}

::IceInternal::DispatchStatus
peer::PeerRegistrationHandler::___registerCallbacks(::IceInternal::Incoming&__inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::Ice::ObjectPrx selfCallbackProxy;
    ::peer::ConnectionEventHandlerPrx connectionEventHandlerProxy;
    __is->read(selfCallbackProxy);
    ::peer::__read(__is, connectionEventHandlerProxy);
    try
    {
	registerCallbacks(selfCallbackProxy, connectionEventHandlerProxy, __current);
    }
    catch(const ::peer::RegistrationException& __ex)
    {
	__os->write(__ex);
	return ::IceInternal::DispatchUserException;
    }
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
peer::PeerRegistrationHandler::___registerProxy(::IceInternal::Incoming&__inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    try
    {
	registerProxy(proxy, __current);
    }
    catch(const ::peer::RegistrationException& __ex)
    {
	__os->write(__ex);
	return ::IceInternal::DispatchUserException;
    }
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
peer::PeerRegistrationHandler::___registerProxies(::IceInternal::Incoming&__inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::peer::ObjectProxyArray proxies;
    ::peer::__read(__is, proxies, ::peer::__U__ObjectProxyArray());
    try
    {
	registerProxies(proxies, __current);
    }
    catch(const ::peer::RegistrationException& __ex)
    {
	__os->write(__ex);
	return ::IceInternal::DispatchUserException;
    }
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
peer::PeerRegistrationHandler::___getPeerProxy(::IceInternal::Incoming&__inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::std::string peerUserId;
    ::Ice::Identity privateProxyIdentity;
    __is->read(peerUserId);
    privateProxyIdentity.__read(__is);
    try
    {
	::Ice::ObjectPrx __ret = getPeerProxy(peerUserId, privateProxyIdentity, __current);
	__os->write(__ret);
    }
    catch(const ::peer::InvalidIdentityException& __ex)
    {
	__os->write(__ex);
	return ::IceInternal::DispatchUserException;
    }
    catch(const ::peer::PeerAccessException& __ex)
    {
	__os->write(__ex);
	return ::IceInternal::DispatchUserException;
    }
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
peer::PeerRegistrationHandler::___getPeerProxies(::IceInternal::Incoming&__inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::std::string peerUserId;
    ::peer::IdentityArray privateProxyIdentities;
    __is->read(peerUserId);
    ::peer::__read(__is, privateProxyIdentities, ::peer::__U__IdentityArray());
    try
    {
	::peer::IdentityToProxyMap __ret = getPeerProxies(peerUserId, privateProxyIdentities, __current);
	::peer::__write(__os, __ret, ::peer::__U__IdentityToProxyMap());
    }
    catch(const ::peer::InvalidIdentityException& __ex)
    {
	__os->write(__ex);
	return ::IceInternal::DispatchUserException;
    }
    catch(const ::peer::PeerAccessException& __ex)
    {
	__os->write(__ex);
	return ::IceInternal::DispatchUserException;
    }
    return ::IceInternal::DispatchOK;
}

static ::std::string __peer__PeerRegistrationHandler_all[] =
{
    "getPeerProxies",
    "getPeerProxy",
    "ice_id",
    "ice_ids",
    "ice_isA",
    "ice_ping",
    "registerCallbacks",
    "registerProxies",
    "registerProxy"
};

::IceInternal::DispatchStatus
peer::PeerRegistrationHandler::__dispatch(::IceInternal::Incoming& in, const ::Ice::Current& current)
{
    ::std::pair< ::std::string*, ::std::string*> r = ::std::equal_range(__peer__PeerRegistrationHandler_all, __peer__PeerRegistrationHandler_all + 9, current.operation);
    if(r.first == r.second)
    {
	return ::IceInternal::DispatchOperationNotExist;
    }

    switch(r.first - __peer__PeerRegistrationHandler_all)
    {
	case 0:
	{
	    return ___getPeerProxies(in, current);
	}
	case 1:
	{
	    return ___getPeerProxy(in, current);
	}
	case 2:
	{
	    return ___ice_id(in, current);
	}
	case 3:
	{
	    return ___ice_ids(in, current);
	}
	case 4:
	{
	    return ___ice_isA(in, current);
	}
	case 5:
	{
	    return ___ice_ping(in, current);
	}
	case 6:
	{
	    return ___registerCallbacks(in, current);
	}
	case 7:
	{
	    return ___registerProxies(in, current);
	}
	case 8:
	{
	    return ___registerProxy(in, current);
	}
    }

    assert(false);
    return ::IceInternal::DispatchOperationNotExist;
}

void
peer::PeerRegistrationHandler::__write(::IceInternal::BasicStream* __os) const
{
    __os->writeTypeId(ice_staticId());
    __os->startWriteSlice();
    __os->endWriteSlice();
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    Object::__write(__os);
#else
    ::Ice::Object::__write(__os);
#endif
}

void
peer::PeerRegistrationHandler::__read(::IceInternal::BasicStream* __is, bool __rid)
{
    if(__rid)
    {
	::std::string myId;
	__is->readTypeId(myId);
    }
    __is->startReadSlice();
    __is->endReadSlice();
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    Object::__read(__is, true);
#else
    ::Ice::Object::__read(__is, true);
#endif
}

void
peer::PeerRegistrationHandler::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type peer::PeerRegistrationHandler was not generated with stream support";
    throw ex;
}

void
peer::PeerRegistrationHandler::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type peer::PeerRegistrationHandler was not generated with stream support";
    throw ex;
}

void 
peer::__patch__PeerRegistrationHandlerPtr(void* __addr, ::Ice::ObjectPtr& v)
{
    ::peer::PeerRegistrationHandlerPtr* p = static_cast< ::peer::PeerRegistrationHandlerPtr*>(__addr);
    assert(p);
    *p = ::peer::PeerRegistrationHandlerPtr::dynamicCast(v);
    if(v && !*p)
    {
	::Ice::NoObjectFactoryException e(__FILE__, __LINE__);
	e.type = ::peer::PeerRegistrationHandler::ice_staticId();
	throw e;
    }
}

bool
peer::operator==(const ::peer::PeerRegistrationHandler& l, const ::peer::PeerRegistrationHandler& r)
{
    return static_cast<const ::Ice::Object&>(l) == static_cast<const ::Ice::Object&>(r);
}

bool
peer::operator!=(const ::peer::PeerRegistrationHandler& l, const ::peer::PeerRegistrationHandler& r)
{
    return static_cast<const ::Ice::Object&>(l) != static_cast<const ::Ice::Object&>(r);
}

bool
peer::operator<(const ::peer::PeerRegistrationHandler& l, const ::peer::PeerRegistrationHandler& r)
{
    return static_cast<const ::Ice::Object&>(l) < static_cast<const ::Ice::Object&>(r);
}

bool
peer::operator<=(const ::peer::PeerRegistrationHandler& l, const ::peer::PeerRegistrationHandler& r)
{
    return l < r || l == r;
}

bool
peer::operator>(const ::peer::PeerRegistrationHandler& l, const ::peer::PeerRegistrationHandler& r)
{
    return !(l < r) && !(l == r);
}

bool
peer::operator>=(const ::peer::PeerRegistrationHandler& l, const ::peer::PeerRegistrationHandler& r)
{
    return !(l < r);
}

::Ice::ObjectPtr
peer::UserSession::ice_clone() const
{
    throw ::Ice::CloneNotImplementedException(__FILE__, __LINE__);
    return 0; // to avoid a warning with some compilers
}

static const ::std::string __peer__UserSession_ids[4] =
{
    "::Glacier2::Session",
    "::Ice::Object",
    "::peer::PeerRegistrationHandler",
    "::peer::UserSession"
};

bool
peer::UserSession::ice_isA(const ::std::string& _s, const ::Ice::Current&) const
{
    return ::std::binary_search(__peer__UserSession_ids, __peer__UserSession_ids + 4, _s);
}

::std::vector< ::std::string>
peer::UserSession::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&__peer__UserSession_ids[0], &__peer__UserSession_ids[4]);
}

const ::std::string&
peer::UserSession::ice_id(const ::Ice::Current&) const
{
    return __peer__UserSession_ids[3];
}

const ::std::string&
peer::UserSession::ice_staticId()
{
    return __peer__UserSession_ids[3];
}

::IceInternal::DispatchStatus
peer::UserSession::___getMyAvailablePeers(::IceInternal::Incoming&__inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __os = __inS.os();
    try
    {
	::peer::PeerIdentifierSet __ret = getMyAvailablePeers(__current);
	if(__ret.size() == 0)
	{
	    __os->writeSize(0);
	}
	else
	{
	    ::peer::__write(__os, &__ret[0], &__ret[0] + __ret.size(), ::peer::__U__PeerIdentifierSet());
	}
    }
    catch(const ::peer::PeerException& __ex)
    {
	__os->write(__ex);
	return ::IceInternal::DispatchUserException;
    }
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
peer::UserSession::___connectToPeer(::IceInternal::Incoming&__inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::std::string peerUserId;
    __is->read(peerUserId);
    try
    {
	::Ice::ObjectPrx __ret = connectToPeer(peerUserId, __current);
	__os->write(__ret);
    }
    catch(const ::peer::DuplicateConnectionException& __ex)
    {
	__os->write(__ex);
	return ::IceInternal::DispatchUserException;
    }
    catch(const ::peer::PeerAccessException& __ex)
    {
	__os->write(__ex);
	return ::IceInternal::DispatchUserException;
    }
    catch(const ::peer::PeerConnectionFailedException& __ex)
    {
	__os->write(__ex);
	return ::IceInternal::DispatchUserException;
    }
    catch(const ::peer::PeerUnavailableException& __ex)
    {
	__os->write(__ex);
	return ::IceInternal::DispatchUserException;
    }
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
peer::UserSession::___getConnectedPeers(::IceInternal::Incoming&__inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __os = __inS.os();
    try
    {
	::peer::PeerIdentifierSet __ret = getConnectedPeers(__current);
	if(__ret.size() == 0)
	{
	    __os->writeSize(0);
	}
	else
	{
	    ::peer::__write(__os, &__ret[0], &__ret[0] + __ret.size(), ::peer::__U__PeerIdentifierSet());
	}
    }
    catch(const ::peer::PeerException& __ex)
    {
	__os->write(__ex);
	return ::IceInternal::DispatchUserException;
    }
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
peer::UserSession::___disconnectFromPeer(::IceInternal::Incoming&__inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::std::string peerUserId;
    __is->read(peerUserId);
    disconnectFromPeer(peerUserId, __current);
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
peer::UserSession::___disconnectFromPeers(::IceInternal::Incoming&, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    disconnectFromPeers(__current);
    return ::IceInternal::DispatchOK;
}

static ::std::string __peer__UserSession_all[] =
{
    "connectToPeer",
    "destroy",
    "disconnectFromPeer",
    "disconnectFromPeers",
    "getConnectedPeers",
    "getMyAvailablePeers",
    "getPeerProxies",
    "getPeerProxy",
    "ice_id",
    "ice_ids",
    "ice_isA",
    "ice_ping",
    "registerCallbacks",
    "registerProxies",
    "registerProxy"
};

::IceInternal::DispatchStatus
peer::UserSession::__dispatch(::IceInternal::Incoming& in, const ::Ice::Current& current)
{
    ::std::pair< ::std::string*, ::std::string*> r = ::std::equal_range(__peer__UserSession_all, __peer__UserSession_all + 15, current.operation);
    if(r.first == r.second)
    {
	return ::IceInternal::DispatchOperationNotExist;
    }

    switch(r.first - __peer__UserSession_all)
    {
	case 0:
	{
	    return ___connectToPeer(in, current);
	}
	case 1:
	{
	    return ___destroy(in, current);
	}
	case 2:
	{
	    return ___disconnectFromPeer(in, current);
	}
	case 3:
	{
	    return ___disconnectFromPeers(in, current);
	}
	case 4:
	{
	    return ___getConnectedPeers(in, current);
	}
	case 5:
	{
	    return ___getMyAvailablePeers(in, current);
	}
	case 6:
	{
	    return ___getPeerProxies(in, current);
	}
	case 7:
	{
	    return ___getPeerProxy(in, current);
	}
	case 8:
	{
	    return ___ice_id(in, current);
	}
	case 9:
	{
	    return ___ice_ids(in, current);
	}
	case 10:
	{
	    return ___ice_isA(in, current);
	}
	case 11:
	{
	    return ___ice_ping(in, current);
	}
	case 12:
	{
	    return ___registerCallbacks(in, current);
	}
	case 13:
	{
	    return ___registerProxies(in, current);
	}
	case 14:
	{
	    return ___registerProxy(in, current);
	}
    }

    assert(false);
    return ::IceInternal::DispatchOperationNotExist;
}

void
peer::UserSession::__write(::IceInternal::BasicStream* __os) const
{
    __os->writeTypeId(ice_staticId());
    __os->startWriteSlice();
    __os->endWriteSlice();
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    Object::__write(__os);
#else
    ::Ice::Object::__write(__os);
#endif
}

void
peer::UserSession::__read(::IceInternal::BasicStream* __is, bool __rid)
{
    if(__rid)
    {
	::std::string myId;
	__is->readTypeId(myId);
    }
    __is->startReadSlice();
    __is->endReadSlice();
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    Object::__read(__is, true);
#else
    ::Ice::Object::__read(__is, true);
#endif
}

void
peer::UserSession::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type peer::UserSession was not generated with stream support";
    throw ex;
}

void
peer::UserSession::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type peer::UserSession was not generated with stream support";
    throw ex;
}

void 
peer::__patch__UserSessionPtr(void* __addr, ::Ice::ObjectPtr& v)
{
    ::peer::UserSessionPtr* p = static_cast< ::peer::UserSessionPtr*>(__addr);
    assert(p);
    *p = ::peer::UserSessionPtr::dynamicCast(v);
    if(v && !*p)
    {
	::Ice::NoObjectFactoryException e(__FILE__, __LINE__);
	e.type = ::peer::UserSession::ice_staticId();
	throw e;
    }
}

bool
peer::operator==(const ::peer::UserSession& l, const ::peer::UserSession& r)
{
    return static_cast<const ::Ice::Object&>(l) == static_cast<const ::Ice::Object&>(r);
}

bool
peer::operator!=(const ::peer::UserSession& l, const ::peer::UserSession& r)
{
    return static_cast<const ::Ice::Object&>(l) != static_cast<const ::Ice::Object&>(r);
}

bool
peer::operator<(const ::peer::UserSession& l, const ::peer::UserSession& r)
{
    return static_cast<const ::Ice::Object&>(l) < static_cast<const ::Ice::Object&>(r);
}

bool
peer::operator<=(const ::peer::UserSession& l, const ::peer::UserSession& r)
{
    return l < r || l == r;
}

bool
peer::operator>(const ::peer::UserSession& l, const ::peer::UserSession& r)
{
    return !(l < r) && !(l == r);
}

bool
peer::operator>=(const ::peer::UserSession& l, const ::peer::UserSession& r)
{
    return !(l < r);
}

#endif
