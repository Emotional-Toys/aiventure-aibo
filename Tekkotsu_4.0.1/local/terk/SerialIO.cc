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
// Generated from file `SerialIO.ice'

#include "SerialIO.h"
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

static const ::std::string __TeRK__SerialIOService__open_name = "open";

static const ::std::string __TeRK__SerialIOService__isOpen_name = "isOpen";

static const ::std::string __TeRK__SerialIOService__isDataAvailable_name = "isDataAvailable";

static const ::std::string __TeRK__SerialIOService__read_name = "read";

static const ::std::string __TeRK__SerialIOService__blockingRead_name = "blockingRead";

static const ::std::string __TeRK__SerialIOService__readUntilDelimiter_name = "readUntilDelimiter";

static const ::std::string __TeRK__SerialIOService__blockingReadUntilDelimiter_name = "blockingReadUntilDelimiter";

static const ::std::string __TeRK__SerialIOService__write_name = "write";

static const ::std::string __TeRK__SerialIOService__close_name = "close";

void
IceInternal::incRef(::TeRK::SerialIOService* p)
{
    p->__incRef();
}

void
IceInternal::decRef(::TeRK::SerialIOService* p)
{
    p->__decRef();
}

void
IceInternal::incRef(::IceProxy::TeRK::SerialIOService* p)
{
    p->__incRef();
}

void
IceInternal::decRef(::IceProxy::TeRK::SerialIOService* p)
{
    p->__decRef();
}

void
TeRK::__write(::IceInternal::BasicStream* __os, const ::TeRK::SerialIOServicePrx& v)
{
    __os->write(::Ice::ObjectPrx(v));
}

void
TeRK::__read(::IceInternal::BasicStream* __is, ::TeRK::SerialIOServicePrx& v)
{
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    if(!proxy)
    {
	v = 0;
    }
    else
    {
	v = new ::IceProxy::TeRK::SerialIOService;
	v->__copyFrom(proxy);
    }
}

void
TeRK::__write(::IceInternal::BasicStream* __os, const ::TeRK::SerialIOServicePtr& v)
{
    __os->write(::Ice::ObjectPtr(v));
}

void
TeRK::__write(::IceInternal::BasicStream* __os, ::TeRK::SerialIOBaudRate v)
{
    __os->write(static_cast< ::Ice::Byte>(v));
}

void
TeRK::__read(::IceInternal::BasicStream* __is, ::TeRK::SerialIOBaudRate& v)
{
    ::Ice::Byte val;
    __is->read(val);
    v = static_cast< ::TeRK::SerialIOBaudRate>(val);
}

void
TeRK::__write(::IceInternal::BasicStream* __os, ::TeRK::SerialIOCharacterSize v)
{
    __os->write(static_cast< ::Ice::Byte>(v));
}

void
TeRK::__read(::IceInternal::BasicStream* __is, ::TeRK::SerialIOCharacterSize& v)
{
    ::Ice::Byte val;
    __is->read(val);
    v = static_cast< ::TeRK::SerialIOCharacterSize>(val);
}

void
TeRK::__write(::IceInternal::BasicStream* __os, ::TeRK::SerialIOParity v)
{
    __os->write(static_cast< ::Ice::Byte>(v));
}

void
TeRK::__read(::IceInternal::BasicStream* __is, ::TeRK::SerialIOParity& v)
{
    ::Ice::Byte val;
    __is->read(val);
    v = static_cast< ::TeRK::SerialIOParity>(val);
}

void
TeRK::__write(::IceInternal::BasicStream* __os, ::TeRK::SerialIOStopBits v)
{
    __os->write(static_cast< ::Ice::Byte>(v));
}

void
TeRK::__read(::IceInternal::BasicStream* __is, ::TeRK::SerialIOStopBits& v)
{
    ::Ice::Byte val;
    __is->read(val);
    v = static_cast< ::TeRK::SerialIOStopBits>(val);
}

void
TeRK::__write(::IceInternal::BasicStream* __os, ::TeRK::SerialIOFlowControl v)
{
    __os->write(static_cast< ::Ice::Byte>(v));
}

void
TeRK::__read(::IceInternal::BasicStream* __is, ::TeRK::SerialIOFlowControl& v)
{
    ::Ice::Byte val;
    __is->read(val);
    v = static_cast< ::TeRK::SerialIOFlowControl>(val);
}

bool
TeRK::SerialIOConfig::operator==(const SerialIOConfig& __rhs) const
{
    return !operator!=(__rhs);
}

bool
TeRK::SerialIOConfig::operator!=(const SerialIOConfig& __rhs) const
{
    if(this == &__rhs)
    {
	return false;
    }
    if(portName != __rhs.portName)
    {
	return true;
    }
    if(baud != __rhs.baud)
    {
	return true;
    }
    if(characterSize != __rhs.characterSize)
    {
	return true;
    }
    if(parity != __rhs.parity)
    {
	return true;
    }
    if(stopBits != __rhs.stopBits)
    {
	return true;
    }
    if(flowControl != __rhs.flowControl)
    {
	return true;
    }
    return false;
}

bool
TeRK::SerialIOConfig::operator<(const SerialIOConfig& __rhs) const
{
    if(this == &__rhs)
    {
	return false;
    }
    if(portName < __rhs.portName)
    {
	return true;
    }
    else if(__rhs.portName < portName)
    {
	return false;
    }
    if(baud < __rhs.baud)
    {
	return true;
    }
    else if(__rhs.baud < baud)
    {
	return false;
    }
    if(characterSize < __rhs.characterSize)
    {
	return true;
    }
    else if(__rhs.characterSize < characterSize)
    {
	return false;
    }
    if(parity < __rhs.parity)
    {
	return true;
    }
    else if(__rhs.parity < parity)
    {
	return false;
    }
    if(stopBits < __rhs.stopBits)
    {
	return true;
    }
    else if(__rhs.stopBits < stopBits)
    {
	return false;
    }
    if(flowControl < __rhs.flowControl)
    {
	return true;
    }
    else if(__rhs.flowControl < flowControl)
    {
	return false;
    }
    return false;
}

void
TeRK::SerialIOConfig::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(portName);
    ::TeRK::__write(__os, baud);
    ::TeRK::__write(__os, characterSize);
    ::TeRK::__write(__os, parity);
    ::TeRK::__write(__os, stopBits);
    ::TeRK::__write(__os, flowControl);
}

void
TeRK::SerialIOConfig::__read(::IceInternal::BasicStream* __is)
{
    __is->read(portName);
    ::TeRK::__read(__is, baud);
    ::TeRK::__read(__is, characterSize);
    ::TeRK::__read(__is, parity);
    ::TeRK::__read(__is, stopBits);
    ::TeRK::__read(__is, flowControl);
}

TeRK::SerialIOException::SerialIOException(const ::std::string& __ice_reason) :
#if (defined(_MSC_VER) && (_MSC_VER < 1300)) // VC++ 6 compiler bug
    TeRKException(__ice_reason)
#else
    ::TeRK::TeRKException(__ice_reason)
#endif
{
}

static const char* __TeRK__SerialIOException_name = "TeRK::SerialIOException";

const ::std::string
TeRK::SerialIOException::ice_name() const
{
    return __TeRK__SerialIOException_name;
}

::Ice::Exception*
TeRK::SerialIOException::ice_clone() const
{
    return new SerialIOException(*this);
}

void
TeRK::SerialIOException::ice_throw() const
{
    throw *this;
}

void
TeRK::SerialIOException::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(::std::string("::TeRK::SerialIOException"), false);
    __os->startWriteSlice();
    __os->endWriteSlice();
#if (defined(_MSC_VER) && (_MSC_VER < 1300)) // VC++ 6 compiler bug
    TeRKException::__write(__os);
#else
    ::TeRK::TeRKException::__write(__os);
#endif
}

void
TeRK::SerialIOException::__read(::IceInternal::BasicStream* __is, bool __rid)
{
    if(__rid)
    {
	::std::string myId;
	__is->read(myId, false);
    }
    __is->startReadSlice();
    __is->endReadSlice();
#if (defined(_MSC_VER) && (_MSC_VER < 1300)) // VC++ 6 compiler bug
    TeRKException::__read(__is, true);
#else
    ::TeRK::TeRKException::__read(__is, true);
#endif
}

void
TeRK::SerialIOException::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "exception TeRK::SerialIOException was not generated with stream support";
    throw ex;
}

void
TeRK::SerialIOException::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "exception TeRK::SerialIOException was not generated with stream support";
    throw ex;
}

struct __F__TeRK__SerialIOException : public ::IceInternal::UserExceptionFactory
{
    virtual void
    createAndThrow()
    {
	throw ::TeRK::SerialIOException();
    }
};

static ::IceInternal::UserExceptionFactoryPtr __F__TeRK__SerialIOException__Ptr = new __F__TeRK__SerialIOException;

const ::IceInternal::UserExceptionFactoryPtr&
TeRK::SerialIOException::ice_factory()
{
    return __F__TeRK__SerialIOException__Ptr;
}

class __F__TeRK__SerialIOException__Init
{
public:

    __F__TeRK__SerialIOException__Init()
    {
	::IceInternal::factoryTable->addExceptionFactory("::TeRK::SerialIOException", ::TeRK::SerialIOException::ice_factory());
    }

    ~__F__TeRK__SerialIOException__Init()
    {
	::IceInternal::factoryTable->removeExceptionFactory("::TeRK::SerialIOException");
    }
};

static __F__TeRK__SerialIOException__Init __F__TeRK__SerialIOException__i;

#ifdef __APPLE__
extern "C" { void __F__TeRK__SerialIOException__initializer() {} }
#endif

void
TeRK::AMI_SerialIOService_open::__invoke(const ::TeRK::SerialIOServicePrx& __prx, const ::TeRK::SerialIOConfig& config, const ::Ice::Context& __ctx)
{
    try
    {
	__prepare(__prx, __TeRK__SerialIOService__open_name, ::Ice::Normal, __ctx);
	config.__write(__os);
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
TeRK::AMI_SerialIOService_open::__response(bool __ok)
{
    try
    {
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::TeRK::SerialIOException& __ex)
	    {
		ice_exception(__ex);
		return;
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
TeRK::AMI_SerialIOService_isOpen::__invoke(const ::TeRK::SerialIOServicePrx& __prx, const ::std::string& portName, const ::Ice::Context& __ctx)
{
    try
    {
	__prepare(__prx, __TeRK__SerialIOService__isOpen_name, ::Ice::Normal, __ctx);
	__os->write(portName);
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
TeRK::AMI_SerialIOService_isOpen::__response(bool __ok)
{
    bool __ret;
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
	__is->read(__ret);
    }
    catch(const ::Ice::LocalException& __ex)
    {
	__finished(__ex);
	return;
    }
    ice_response(__ret);
}

void
TeRK::AMI_SerialIOService_isDataAvailable::__invoke(const ::TeRK::SerialIOServicePrx& __prx, const ::std::string& portName, const ::Ice::Context& __ctx)
{
    try
    {
	__prepare(__prx, __TeRK__SerialIOService__isDataAvailable_name, ::Ice::Normal, __ctx);
	__os->write(portName);
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
TeRK::AMI_SerialIOService_isDataAvailable::__response(bool __ok)
{
    bool __ret;
    try
    {
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::TeRK::SerialIOException& __ex)
	    {
		ice_exception(__ex);
		return;
	    }
	    catch(const ::Ice::UserException& __ex)
	    {
		throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
	    }
	}
	__is->read(__ret);
    }
    catch(const ::Ice::LocalException& __ex)
    {
	__finished(__ex);
	return;
    }
    ice_response(__ret);
}

void
TeRK::AMI_SerialIOService_read::__invoke(const ::TeRK::SerialIOServicePrx& __prx, const ::std::string& portName, ::Ice::Int maxNumberOfBytesToRead, const ::Ice::Context& __ctx)
{
    try
    {
	__prepare(__prx, __TeRK__SerialIOService__read_name, ::Ice::Normal, __ctx);
	__os->write(portName);
	__os->write(maxNumberOfBytesToRead);
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
TeRK::AMI_SerialIOService_read::__response(bool __ok)
{
    ::TeRK::ByteArray __ret;
    try
    {
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::TeRK::SerialIOException& __ex)
	    {
		ice_exception(__ex);
		return;
	    }
	    catch(const ::Ice::UserException& __ex)
	    {
		throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
	    }
	}
	::std::pair<const ::Ice::Byte*, const ::Ice::Byte*> _____ret;
	__is->read(_____ret);
	::std::vector< ::Ice::Byte>(_____ret.first, _____ret.second).swap(__ret);
    }
    catch(const ::Ice::LocalException& __ex)
    {
	__finished(__ex);
	return;
    }
    ice_response(__ret);
}

void
TeRK::AMI_SerialIOService_blockingRead::__invoke(const ::TeRK::SerialIOServicePrx& __prx, const ::std::string& portName, ::Ice::Int maxNumberOfBytesToRead, ::Ice::Int timeoutMilliseconds, const ::Ice::Context& __ctx)
{
    try
    {
	__prepare(__prx, __TeRK__SerialIOService__blockingRead_name, ::Ice::Normal, __ctx);
	__os->write(portName);
	__os->write(maxNumberOfBytesToRead);
	__os->write(timeoutMilliseconds);
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
TeRK::AMI_SerialIOService_blockingRead::__response(bool __ok)
{
    ::TeRK::ByteArray __ret;
    try
    {
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::TeRK::SerialIOException& __ex)
	    {
		ice_exception(__ex);
		return;
	    }
	    catch(const ::Ice::UserException& __ex)
	    {
		throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
	    }
	}
	::std::pair<const ::Ice::Byte*, const ::Ice::Byte*> _____ret;
	__is->read(_____ret);
	::std::vector< ::Ice::Byte>(_____ret.first, _____ret.second).swap(__ret);
    }
    catch(const ::Ice::LocalException& __ex)
    {
	__finished(__ex);
	return;
    }
    ice_response(__ret);
}

void
TeRK::AMI_SerialIOService_readUntilDelimiter::__invoke(const ::TeRK::SerialIOServicePrx& __prx, const ::std::string& portName, ::Ice::Int maxNumberOfBytesToRead, ::Ice::Byte delimiterCharacter, const ::Ice::Context& __ctx)
{
    try
    {
	__prepare(__prx, __TeRK__SerialIOService__readUntilDelimiter_name, ::Ice::Normal, __ctx);
	__os->write(portName);
	__os->write(maxNumberOfBytesToRead);
	__os->write(delimiterCharacter);
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
TeRK::AMI_SerialIOService_readUntilDelimiter::__response(bool __ok)
{
    ::TeRK::ByteArray __ret;
    try
    {
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::TeRK::SerialIOException& __ex)
	    {
		ice_exception(__ex);
		return;
	    }
	    catch(const ::Ice::UserException& __ex)
	    {
		throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
	    }
	}
	::std::pair<const ::Ice::Byte*, const ::Ice::Byte*> _____ret;
	__is->read(_____ret);
	::std::vector< ::Ice::Byte>(_____ret.first, _____ret.second).swap(__ret);
    }
    catch(const ::Ice::LocalException& __ex)
    {
	__finished(__ex);
	return;
    }
    ice_response(__ret);
}

void
TeRK::AMI_SerialIOService_blockingReadUntilDelimiter::__invoke(const ::TeRK::SerialIOServicePrx& __prx, const ::std::string& portName, ::Ice::Int maxNumberOfBytesToRead, ::Ice::Byte delimiterCharacter, ::Ice::Int timeoutMilliseconds, const ::Ice::Context& __ctx)
{
    try
    {
	__prepare(__prx, __TeRK__SerialIOService__blockingReadUntilDelimiter_name, ::Ice::Normal, __ctx);
	__os->write(portName);
	__os->write(maxNumberOfBytesToRead);
	__os->write(delimiterCharacter);
	__os->write(timeoutMilliseconds);
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
TeRK::AMI_SerialIOService_blockingReadUntilDelimiter::__response(bool __ok)
{
    ::TeRK::ByteArray __ret;
    try
    {
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::TeRK::SerialIOException& __ex)
	    {
		ice_exception(__ex);
		return;
	    }
	    catch(const ::Ice::UserException& __ex)
	    {
		throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
	    }
	}
	::std::pair<const ::Ice::Byte*, const ::Ice::Byte*> _____ret;
	__is->read(_____ret);
	::std::vector< ::Ice::Byte>(_____ret.first, _____ret.second).swap(__ret);
    }
    catch(const ::Ice::LocalException& __ex)
    {
	__finished(__ex);
	return;
    }
    ice_response(__ret);
}

void
TeRK::AMI_SerialIOService_write::__invoke(const ::TeRK::SerialIOServicePrx& __prx, const ::std::string& portName, const ::TeRK::ByteArray& data, const ::Ice::Context& __ctx)
{
    try
    {
	__prepare(__prx, __TeRK__SerialIOService__write_name, ::Ice::Normal, __ctx);
	__os->write(portName);
	if(data.size() == 0)
	{
	    __os->writeSize(0);
	}
	else
	{
	    __os->write(&data[0], &data[0] + data.size());
	}
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
TeRK::AMI_SerialIOService_write::__response(bool __ok)
{
    try
    {
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::TeRK::SerialIOException& __ex)
	    {
		ice_exception(__ex);
		return;
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
TeRK::AMI_SerialIOService_close::__invoke(const ::TeRK::SerialIOServicePrx& __prx, const ::std::string& portName, const ::Ice::Context& __ctx)
{
    try
    {
	__prepare(__prx, __TeRK__SerialIOService__close_name, ::Ice::Normal, __ctx);
	__os->write(portName);
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
TeRK::AMI_SerialIOService_close::__response(bool __ok)
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
IceProxy::TeRK::SerialIOService::open(const ::TeRK::SerialIOConfig& config, const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__TeRK__SerialIOService__open_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::TeRK::SerialIOService* __del = dynamic_cast< ::IceDelegate::TeRK::SerialIOService*>(__delBase.get());
	    __del->open(config, __ctx);
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
IceProxy::TeRK::SerialIOService::open_async(const ::TeRK::AMI_SerialIOService_openPtr& __cb, const ::TeRK::SerialIOConfig& config)
{
    open_async(__cb, config, __defaultContext());
}

void
IceProxy::TeRK::SerialIOService::open_async(const ::TeRK::AMI_SerialIOService_openPtr& __cb, const ::TeRK::SerialIOConfig& config, const ::Ice::Context& __ctx)
{
    __cb->__invoke(this, config, __ctx);
}

bool
IceProxy::TeRK::SerialIOService::isOpen(const ::std::string& portName, const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__TeRK__SerialIOService__isOpen_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::TeRK::SerialIOService* __del = dynamic_cast< ::IceDelegate::TeRK::SerialIOService*>(__delBase.get());
	    return __del->isOpen(portName, __ctx);
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
IceProxy::TeRK::SerialIOService::isOpen_async(const ::TeRK::AMI_SerialIOService_isOpenPtr& __cb, const ::std::string& portName)
{
    isOpen_async(__cb, portName, __defaultContext());
}

void
IceProxy::TeRK::SerialIOService::isOpen_async(const ::TeRK::AMI_SerialIOService_isOpenPtr& __cb, const ::std::string& portName, const ::Ice::Context& __ctx)
{
    __cb->__invoke(this, portName, __ctx);
}

bool
IceProxy::TeRK::SerialIOService::isDataAvailable(const ::std::string& portName, const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__TeRK__SerialIOService__isDataAvailable_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::TeRK::SerialIOService* __del = dynamic_cast< ::IceDelegate::TeRK::SerialIOService*>(__delBase.get());
	    return __del->isDataAvailable(portName, __ctx);
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
IceProxy::TeRK::SerialIOService::isDataAvailable_async(const ::TeRK::AMI_SerialIOService_isDataAvailablePtr& __cb, const ::std::string& portName)
{
    isDataAvailable_async(__cb, portName, __defaultContext());
}

void
IceProxy::TeRK::SerialIOService::isDataAvailable_async(const ::TeRK::AMI_SerialIOService_isDataAvailablePtr& __cb, const ::std::string& portName, const ::Ice::Context& __ctx)
{
    __cb->__invoke(this, portName, __ctx);
}

::TeRK::ByteArray
IceProxy::TeRK::SerialIOService::read(const ::std::string& portName, ::Ice::Int maxNumberOfBytesToRead, const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__TeRK__SerialIOService__read_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::TeRK::SerialIOService* __del = dynamic_cast< ::IceDelegate::TeRK::SerialIOService*>(__delBase.get());
	    return __del->read(portName, maxNumberOfBytesToRead, __ctx);
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
IceProxy::TeRK::SerialIOService::read_async(const ::TeRK::AMI_SerialIOService_readPtr& __cb, const ::std::string& portName, ::Ice::Int maxNumberOfBytesToRead)
{
    read_async(__cb, portName, maxNumberOfBytesToRead, __defaultContext());
}

void
IceProxy::TeRK::SerialIOService::read_async(const ::TeRK::AMI_SerialIOService_readPtr& __cb, const ::std::string& portName, ::Ice::Int maxNumberOfBytesToRead, const ::Ice::Context& __ctx)
{
    __cb->__invoke(this, portName, maxNumberOfBytesToRead, __ctx);
}

::TeRK::ByteArray
IceProxy::TeRK::SerialIOService::blockingRead(const ::std::string& portName, ::Ice::Int maxNumberOfBytesToRead, ::Ice::Int timeoutMilliseconds, const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__TeRK__SerialIOService__blockingRead_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::TeRK::SerialIOService* __del = dynamic_cast< ::IceDelegate::TeRK::SerialIOService*>(__delBase.get());
	    return __del->blockingRead(portName, maxNumberOfBytesToRead, timeoutMilliseconds, __ctx);
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
IceProxy::TeRK::SerialIOService::blockingRead_async(const ::TeRK::AMI_SerialIOService_blockingReadPtr& __cb, const ::std::string& portName, ::Ice::Int maxNumberOfBytesToRead, ::Ice::Int timeoutMilliseconds)
{
    blockingRead_async(__cb, portName, maxNumberOfBytesToRead, timeoutMilliseconds, __defaultContext());
}

void
IceProxy::TeRK::SerialIOService::blockingRead_async(const ::TeRK::AMI_SerialIOService_blockingReadPtr& __cb, const ::std::string& portName, ::Ice::Int maxNumberOfBytesToRead, ::Ice::Int timeoutMilliseconds, const ::Ice::Context& __ctx)
{
    __cb->__invoke(this, portName, maxNumberOfBytesToRead, timeoutMilliseconds, __ctx);
}

::TeRK::ByteArray
IceProxy::TeRK::SerialIOService::readUntilDelimiter(const ::std::string& portName, ::Ice::Int maxNumberOfBytesToRead, ::Ice::Byte delimiterCharacter, const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__TeRK__SerialIOService__readUntilDelimiter_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::TeRK::SerialIOService* __del = dynamic_cast< ::IceDelegate::TeRK::SerialIOService*>(__delBase.get());
	    return __del->readUntilDelimiter(portName, maxNumberOfBytesToRead, delimiterCharacter, __ctx);
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
IceProxy::TeRK::SerialIOService::readUntilDelimiter_async(const ::TeRK::AMI_SerialIOService_readUntilDelimiterPtr& __cb, const ::std::string& portName, ::Ice::Int maxNumberOfBytesToRead, ::Ice::Byte delimiterCharacter)
{
    readUntilDelimiter_async(__cb, portName, maxNumberOfBytesToRead, delimiterCharacter, __defaultContext());
}

void
IceProxy::TeRK::SerialIOService::readUntilDelimiter_async(const ::TeRK::AMI_SerialIOService_readUntilDelimiterPtr& __cb, const ::std::string& portName, ::Ice::Int maxNumberOfBytesToRead, ::Ice::Byte delimiterCharacter, const ::Ice::Context& __ctx)
{
    __cb->__invoke(this, portName, maxNumberOfBytesToRead, delimiterCharacter, __ctx);
}

::TeRK::ByteArray
IceProxy::TeRK::SerialIOService::blockingReadUntilDelimiter(const ::std::string& portName, ::Ice::Int maxNumberOfBytesToRead, ::Ice::Byte delimiterCharacter, ::Ice::Int timeoutMilliseconds, const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__TeRK__SerialIOService__blockingReadUntilDelimiter_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::TeRK::SerialIOService* __del = dynamic_cast< ::IceDelegate::TeRK::SerialIOService*>(__delBase.get());
	    return __del->blockingReadUntilDelimiter(portName, maxNumberOfBytesToRead, delimiterCharacter, timeoutMilliseconds, __ctx);
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
IceProxy::TeRK::SerialIOService::blockingReadUntilDelimiter_async(const ::TeRK::AMI_SerialIOService_blockingReadUntilDelimiterPtr& __cb, const ::std::string& portName, ::Ice::Int maxNumberOfBytesToRead, ::Ice::Byte delimiterCharacter, ::Ice::Int timeoutMilliseconds)
{
    blockingReadUntilDelimiter_async(__cb, portName, maxNumberOfBytesToRead, delimiterCharacter, timeoutMilliseconds, __defaultContext());
}

void
IceProxy::TeRK::SerialIOService::blockingReadUntilDelimiter_async(const ::TeRK::AMI_SerialIOService_blockingReadUntilDelimiterPtr& __cb, const ::std::string& portName, ::Ice::Int maxNumberOfBytesToRead, ::Ice::Byte delimiterCharacter, ::Ice::Int timeoutMilliseconds, const ::Ice::Context& __ctx)
{
    __cb->__invoke(this, portName, maxNumberOfBytesToRead, delimiterCharacter, timeoutMilliseconds, __ctx);
}

void
IceProxy::TeRK::SerialIOService::write(const ::std::string& portName, const ::TeRK::ByteArray& data, const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__TeRK__SerialIOService__write_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::TeRK::SerialIOService* __del = dynamic_cast< ::IceDelegate::TeRK::SerialIOService*>(__delBase.get());
	    __del->write(portName, data, __ctx);
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
IceProxy::TeRK::SerialIOService::write_async(const ::TeRK::AMI_SerialIOService_writePtr& __cb, const ::std::string& portName, const ::TeRK::ByteArray& data)
{
    write_async(__cb, portName, data, __defaultContext());
}

void
IceProxy::TeRK::SerialIOService::write_async(const ::TeRK::AMI_SerialIOService_writePtr& __cb, const ::std::string& portName, const ::TeRK::ByteArray& data, const ::Ice::Context& __ctx)
{
    __cb->__invoke(this, portName, data, __ctx);
}

void
IceProxy::TeRK::SerialIOService::close(const ::std::string& portName, const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::TeRK::SerialIOService* __del = dynamic_cast< ::IceDelegate::TeRK::SerialIOService*>(__delBase.get());
	    __del->close(portName, __ctx);
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
IceProxy::TeRK::SerialIOService::close_async(const ::TeRK::AMI_SerialIOService_closePtr& __cb, const ::std::string& portName)
{
    close_async(__cb, portName, __defaultContext());
}

void
IceProxy::TeRK::SerialIOService::close_async(const ::TeRK::AMI_SerialIOService_closePtr& __cb, const ::std::string& portName, const ::Ice::Context& __ctx)
{
    __cb->__invoke(this, portName, __ctx);
}

const ::std::string&
IceProxy::TeRK::SerialIOService::ice_staticId()
{
    return ::TeRK::SerialIOService::ice_staticId();
}

::IceInternal::Handle< ::IceDelegateM::Ice::Object>
IceProxy::TeRK::SerialIOService::__createDelegateM()
{
    return ::IceInternal::Handle< ::IceDelegateM::Ice::Object>(new ::IceDelegateM::TeRK::SerialIOService);
}

::IceInternal::Handle< ::IceDelegateD::Ice::Object>
IceProxy::TeRK::SerialIOService::__createDelegateD()
{
    return ::IceInternal::Handle< ::IceDelegateD::Ice::Object>(new ::IceDelegateD::TeRK::SerialIOService);
}

bool
IceProxy::TeRK::operator==(const ::IceProxy::TeRK::SerialIOService& l, const ::IceProxy::TeRK::SerialIOService& r)
{
    return static_cast<const ::IceProxy::Ice::Object&>(l) == static_cast<const ::IceProxy::Ice::Object&>(r);
}

bool
IceProxy::TeRK::operator!=(const ::IceProxy::TeRK::SerialIOService& l, const ::IceProxy::TeRK::SerialIOService& r)
{
    return static_cast<const ::IceProxy::Ice::Object&>(l) != static_cast<const ::IceProxy::Ice::Object&>(r);
}

bool
IceProxy::TeRK::operator<(const ::IceProxy::TeRK::SerialIOService& l, const ::IceProxy::TeRK::SerialIOService& r)
{
    return static_cast<const ::IceProxy::Ice::Object&>(l) < static_cast<const ::IceProxy::Ice::Object&>(r);
}

bool
IceProxy::TeRK::operator<=(const ::IceProxy::TeRK::SerialIOService& l, const ::IceProxy::TeRK::SerialIOService& r)
{
    return l < r || l == r;
}

bool
IceProxy::TeRK::operator>(const ::IceProxy::TeRK::SerialIOService& l, const ::IceProxy::TeRK::SerialIOService& r)
{
    return !(l < r) && !(l == r);
}

bool
IceProxy::TeRK::operator>=(const ::IceProxy::TeRK::SerialIOService& l, const ::IceProxy::TeRK::SerialIOService& r)
{
    return !(l < r);
}

void
IceDelegateM::TeRK::SerialIOService::open(const ::TeRK::SerialIOConfig& config, const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __TeRK__SerialIOService__open_name, ::Ice::Normal, __context, __compress);
    try
    {
	::IceInternal::BasicStream* __os = __og.os();
	config.__write(__os);
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
	    catch(const ::TeRK::SerialIOException&)
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

bool
IceDelegateM::TeRK::SerialIOService::isOpen(const ::std::string& portName, const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __TeRK__SerialIOService__isOpen_name, ::Ice::Normal, __context, __compress);
    try
    {
	::IceInternal::BasicStream* __os = __og.os();
	__os->write(portName);
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
	bool __ret;
	__is->read(__ret);
	return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
	throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

bool
IceDelegateM::TeRK::SerialIOService::isDataAvailable(const ::std::string& portName, const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __TeRK__SerialIOService__isDataAvailable_name, ::Ice::Normal, __context, __compress);
    try
    {
	::IceInternal::BasicStream* __os = __og.os();
	__os->write(portName);
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
	    catch(const ::TeRK::SerialIOException&)
	    {
		throw;
	    }
	    catch(const ::Ice::UserException& __ex)
	    {
		throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
	    }
	}
	bool __ret;
	__is->read(__ret);
	return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
	throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

::TeRK::ByteArray
IceDelegateM::TeRK::SerialIOService::read(const ::std::string& portName, ::Ice::Int maxNumberOfBytesToRead, const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __TeRK__SerialIOService__read_name, ::Ice::Normal, __context, __compress);
    try
    {
	::IceInternal::BasicStream* __os = __og.os();
	__os->write(portName);
	__os->write(maxNumberOfBytesToRead);
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
	    catch(const ::TeRK::SerialIOException&)
	    {
		throw;
	    }
	    catch(const ::Ice::UserException& __ex)
	    {
		throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
	    }
	}
	::TeRK::ByteArray __ret;
	::std::pair<const ::Ice::Byte*, const ::Ice::Byte*> _____ret;
	__is->read(_____ret);
	::std::vector< ::Ice::Byte>(_____ret.first, _____ret.second).swap(__ret);
	return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
	throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

::TeRK::ByteArray
IceDelegateM::TeRK::SerialIOService::blockingRead(const ::std::string& portName, ::Ice::Int maxNumberOfBytesToRead, ::Ice::Int timeoutMilliseconds, const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __TeRK__SerialIOService__blockingRead_name, ::Ice::Normal, __context, __compress);
    try
    {
	::IceInternal::BasicStream* __os = __og.os();
	__os->write(portName);
	__os->write(maxNumberOfBytesToRead);
	__os->write(timeoutMilliseconds);
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
	    catch(const ::TeRK::SerialIOException&)
	    {
		throw;
	    }
	    catch(const ::Ice::UserException& __ex)
	    {
		throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
	    }
	}
	::TeRK::ByteArray __ret;
	::std::pair<const ::Ice::Byte*, const ::Ice::Byte*> _____ret;
	__is->read(_____ret);
	::std::vector< ::Ice::Byte>(_____ret.first, _____ret.second).swap(__ret);
	return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
	throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

::TeRK::ByteArray
IceDelegateM::TeRK::SerialIOService::readUntilDelimiter(const ::std::string& portName, ::Ice::Int maxNumberOfBytesToRead, ::Ice::Byte delimiterCharacter, const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __TeRK__SerialIOService__readUntilDelimiter_name, ::Ice::Normal, __context, __compress);
    try
    {
	::IceInternal::BasicStream* __os = __og.os();
	__os->write(portName);
	__os->write(maxNumberOfBytesToRead);
	__os->write(delimiterCharacter);
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
	    catch(const ::TeRK::SerialIOException&)
	    {
		throw;
	    }
	    catch(const ::Ice::UserException& __ex)
	    {
		throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
	    }
	}
	::TeRK::ByteArray __ret;
	::std::pair<const ::Ice::Byte*, const ::Ice::Byte*> _____ret;
	__is->read(_____ret);
	::std::vector< ::Ice::Byte>(_____ret.first, _____ret.second).swap(__ret);
	return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
	throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

::TeRK::ByteArray
IceDelegateM::TeRK::SerialIOService::blockingReadUntilDelimiter(const ::std::string& portName, ::Ice::Int maxNumberOfBytesToRead, ::Ice::Byte delimiterCharacter, ::Ice::Int timeoutMilliseconds, const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __TeRK__SerialIOService__blockingReadUntilDelimiter_name, ::Ice::Normal, __context, __compress);
    try
    {
	::IceInternal::BasicStream* __os = __og.os();
	__os->write(portName);
	__os->write(maxNumberOfBytesToRead);
	__os->write(delimiterCharacter);
	__os->write(timeoutMilliseconds);
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
	    catch(const ::TeRK::SerialIOException&)
	    {
		throw;
	    }
	    catch(const ::Ice::UserException& __ex)
	    {
		throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
	    }
	}
	::TeRK::ByteArray __ret;
	::std::pair<const ::Ice::Byte*, const ::Ice::Byte*> _____ret;
	__is->read(_____ret);
	::std::vector< ::Ice::Byte>(_____ret.first, _____ret.second).swap(__ret);
	return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
	throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

void
IceDelegateM::TeRK::SerialIOService::write(const ::std::string& portName, const ::TeRK::ByteArray& data, const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __TeRK__SerialIOService__write_name, ::Ice::Normal, __context, __compress);
    try
    {
	::IceInternal::BasicStream* __os = __og.os();
	__os->write(portName);
	if(data.size() == 0)
	{
	    __os->writeSize(0);
	}
	else
	{
	    __os->write(&data[0], &data[0] + data.size());
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
	    catch(const ::TeRK::SerialIOException&)
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
IceDelegateM::TeRK::SerialIOService::close(const ::std::string& portName, const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __TeRK__SerialIOService__close_name, ::Ice::Normal, __context, __compress);
    try
    {
	::IceInternal::BasicStream* __os = __og.os();
	__os->write(portName);
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
IceDelegateD::TeRK::SerialIOService::open(const ::TeRK::SerialIOConfig& config, const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __TeRK__SerialIOService__open_name, ::Ice::Normal, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::TeRK::SerialIOService* __servant = dynamic_cast< ::TeRK::SerialIOService*>(__direct.servant().get());
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
	    __servant->open(config, __current);
	    return;
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

bool
IceDelegateD::TeRK::SerialIOService::isOpen(const ::std::string& portName, const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __TeRK__SerialIOService__isOpen_name, ::Ice::Normal, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::TeRK::SerialIOService* __servant = dynamic_cast< ::TeRK::SerialIOService*>(__direct.servant().get());
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
	    return __servant->isOpen(portName, __current);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

bool
IceDelegateD::TeRK::SerialIOService::isDataAvailable(const ::std::string& portName, const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __TeRK__SerialIOService__isDataAvailable_name, ::Ice::Normal, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::TeRK::SerialIOService* __servant = dynamic_cast< ::TeRK::SerialIOService*>(__direct.servant().get());
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
	    return __servant->isDataAvailable(portName, __current);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

::TeRK::ByteArray
IceDelegateD::TeRK::SerialIOService::read(const ::std::string& portName, ::Ice::Int maxNumberOfBytesToRead, const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __TeRK__SerialIOService__read_name, ::Ice::Normal, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::TeRK::SerialIOService* __servant = dynamic_cast< ::TeRK::SerialIOService*>(__direct.servant().get());
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
	    return __servant->read(portName, maxNumberOfBytesToRead, __current);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

::TeRK::ByteArray
IceDelegateD::TeRK::SerialIOService::blockingRead(const ::std::string& portName, ::Ice::Int maxNumberOfBytesToRead, ::Ice::Int timeoutMilliseconds, const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __TeRK__SerialIOService__blockingRead_name, ::Ice::Normal, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::TeRK::SerialIOService* __servant = dynamic_cast< ::TeRK::SerialIOService*>(__direct.servant().get());
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
	    return __servant->blockingRead(portName, maxNumberOfBytesToRead, timeoutMilliseconds, __current);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

::TeRK::ByteArray
IceDelegateD::TeRK::SerialIOService::readUntilDelimiter(const ::std::string& portName, ::Ice::Int maxNumberOfBytesToRead, ::Ice::Byte delimiterCharacter, const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __TeRK__SerialIOService__readUntilDelimiter_name, ::Ice::Normal, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::TeRK::SerialIOService* __servant = dynamic_cast< ::TeRK::SerialIOService*>(__direct.servant().get());
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
	    return __servant->readUntilDelimiter(portName, maxNumberOfBytesToRead, delimiterCharacter, __current);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

::TeRK::ByteArray
IceDelegateD::TeRK::SerialIOService::blockingReadUntilDelimiter(const ::std::string& portName, ::Ice::Int maxNumberOfBytesToRead, ::Ice::Byte delimiterCharacter, ::Ice::Int timeoutMilliseconds, const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __TeRK__SerialIOService__blockingReadUntilDelimiter_name, ::Ice::Normal, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::TeRK::SerialIOService* __servant = dynamic_cast< ::TeRK::SerialIOService*>(__direct.servant().get());
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
	    return __servant->blockingReadUntilDelimiter(portName, maxNumberOfBytesToRead, delimiterCharacter, timeoutMilliseconds, __current);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

void
IceDelegateD::TeRK::SerialIOService::write(const ::std::string& portName, const ::TeRK::ByteArray& data, const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __TeRK__SerialIOService__write_name, ::Ice::Normal, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::TeRK::SerialIOService* __servant = dynamic_cast< ::TeRK::SerialIOService*>(__direct.servant().get());
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
	    __servant->write(portName, data, __current);
	    return;
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

void
IceDelegateD::TeRK::SerialIOService::close(const ::std::string& portName, const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __TeRK__SerialIOService__close_name, ::Ice::Normal, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::TeRK::SerialIOService* __servant = dynamic_cast< ::TeRK::SerialIOService*>(__direct.servant().get());
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
	    __servant->close(portName, __current);
	    return;
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

::Ice::ObjectPtr
TeRK::SerialIOService::ice_clone() const
{
    throw ::Ice::CloneNotImplementedException(__FILE__, __LINE__);
    return 0; // to avoid a warning with some compilers
}

static const ::std::string __TeRK__SerialIOService_ids[4] =
{
    "::Ice::Object",
    "::TeRK::AbstractCommandController",
    "::TeRK::PropertyManager",
    "::TeRK::SerialIOService"
};

bool
TeRK::SerialIOService::ice_isA(const ::std::string& _s, const ::Ice::Current&) const
{
    return ::std::binary_search(__TeRK__SerialIOService_ids, __TeRK__SerialIOService_ids + 4, _s);
}

::std::vector< ::std::string>
TeRK::SerialIOService::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&__TeRK__SerialIOService_ids[0], &__TeRK__SerialIOService_ids[4]);
}

const ::std::string&
TeRK::SerialIOService::ice_id(const ::Ice::Current&) const
{
    return __TeRK__SerialIOService_ids[3];
}

const ::std::string&
TeRK::SerialIOService::ice_staticId()
{
    return __TeRK__SerialIOService_ids[3];
}

::IceInternal::DispatchStatus
TeRK::SerialIOService::___open(::IceInternal::Incoming&__inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::TeRK::SerialIOConfig config;
    config.__read(__is);
    try
    {
	open(config, __current);
    }
    catch(const ::TeRK::SerialIOException& __ex)
    {
	__os->write(__ex);
	return ::IceInternal::DispatchUserException;
    }
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
TeRK::SerialIOService::___isOpen(::IceInternal::Incoming&__inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::std::string portName;
    __is->read(portName);
    bool __ret = isOpen(portName, __current);
    __os->write(__ret);
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
TeRK::SerialIOService::___isDataAvailable(::IceInternal::Incoming&__inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::std::string portName;
    __is->read(portName);
    try
    {
	bool __ret = isDataAvailable(portName, __current);
	__os->write(__ret);
    }
    catch(const ::TeRK::SerialIOException& __ex)
    {
	__os->write(__ex);
	return ::IceInternal::DispatchUserException;
    }
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
TeRK::SerialIOService::___read(::IceInternal::Incoming&__inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::std::string portName;
    ::Ice::Int maxNumberOfBytesToRead;
    __is->read(portName);
    __is->read(maxNumberOfBytesToRead);
    try
    {
	::TeRK::ByteArray __ret = read(portName, maxNumberOfBytesToRead, __current);
	if(__ret.size() == 0)
	{
	    __os->writeSize(0);
	}
	else
	{
	    __os->write(&__ret[0], &__ret[0] + __ret.size());
	}
    }
    catch(const ::TeRK::SerialIOException& __ex)
    {
	__os->write(__ex);
	return ::IceInternal::DispatchUserException;
    }
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
TeRK::SerialIOService::___blockingRead(::IceInternal::Incoming&__inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::std::string portName;
    ::Ice::Int maxNumberOfBytesToRead;
    ::Ice::Int timeoutMilliseconds;
    __is->read(portName);
    __is->read(maxNumberOfBytesToRead);
    __is->read(timeoutMilliseconds);
    try
    {
	::TeRK::ByteArray __ret = blockingRead(portName, maxNumberOfBytesToRead, timeoutMilliseconds, __current);
	if(__ret.size() == 0)
	{
	    __os->writeSize(0);
	}
	else
	{
	    __os->write(&__ret[0], &__ret[0] + __ret.size());
	}
    }
    catch(const ::TeRK::SerialIOException& __ex)
    {
	__os->write(__ex);
	return ::IceInternal::DispatchUserException;
    }
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
TeRK::SerialIOService::___readUntilDelimiter(::IceInternal::Incoming&__inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::std::string portName;
    ::Ice::Int maxNumberOfBytesToRead;
    ::Ice::Byte delimiterCharacter;
    __is->read(portName);
    __is->read(maxNumberOfBytesToRead);
    __is->read(delimiterCharacter);
    try
    {
	::TeRK::ByteArray __ret = readUntilDelimiter(portName, maxNumberOfBytesToRead, delimiterCharacter, __current);
	if(__ret.size() == 0)
	{
	    __os->writeSize(0);
	}
	else
	{
	    __os->write(&__ret[0], &__ret[0] + __ret.size());
	}
    }
    catch(const ::TeRK::SerialIOException& __ex)
    {
	__os->write(__ex);
	return ::IceInternal::DispatchUserException;
    }
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
TeRK::SerialIOService::___blockingReadUntilDelimiter(::IceInternal::Incoming&__inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::std::string portName;
    ::Ice::Int maxNumberOfBytesToRead;
    ::Ice::Byte delimiterCharacter;
    ::Ice::Int timeoutMilliseconds;
    __is->read(portName);
    __is->read(maxNumberOfBytesToRead);
    __is->read(delimiterCharacter);
    __is->read(timeoutMilliseconds);
    try
    {
	::TeRK::ByteArray __ret = blockingReadUntilDelimiter(portName, maxNumberOfBytesToRead, delimiterCharacter, timeoutMilliseconds, __current);
	if(__ret.size() == 0)
	{
	    __os->writeSize(0);
	}
	else
	{
	    __os->write(&__ret[0], &__ret[0] + __ret.size());
	}
    }
    catch(const ::TeRK::SerialIOException& __ex)
    {
	__os->write(__ex);
	return ::IceInternal::DispatchUserException;
    }
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
TeRK::SerialIOService::___write(::IceInternal::Incoming&__inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::std::string portName;
    ::TeRK::ByteArray data;
    __is->read(portName);
    ::std::pair<const ::Ice::Byte*, const ::Ice::Byte*> ___data;
    __is->read(___data);
    ::std::vector< ::Ice::Byte>(___data.first, ___data.second).swap(data);
    try
    {
	write(portName, data, __current);
    }
    catch(const ::TeRK::SerialIOException& __ex)
    {
	__os->write(__ex);
	return ::IceInternal::DispatchUserException;
    }
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
TeRK::SerialIOService::___close(::IceInternal::Incoming&__inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::std::string portName;
    __is->read(portName);
    close(portName, __current);
    return ::IceInternal::DispatchOK;
}

static ::std::string __TeRK__SerialIOService_all[] =
{
    "blockingRead",
    "blockingReadUntilDelimiter",
    "close",
    "getProperties",
    "getProperty",
    "getPropertyKeys",
    "ice_id",
    "ice_ids",
    "ice_isA",
    "ice_ping",
    "isDataAvailable",
    "isOpen",
    "open",
    "read",
    "readUntilDelimiter",
    "setProperty",
    "write"
};

::IceInternal::DispatchStatus
TeRK::SerialIOService::__dispatch(::IceInternal::Incoming& in, const ::Ice::Current& current)
{
    ::std::pair< ::std::string*, ::std::string*> r = ::std::equal_range(__TeRK__SerialIOService_all, __TeRK__SerialIOService_all + 17, current.operation);
    if(r.first == r.second)
    {
	return ::IceInternal::DispatchOperationNotExist;
    }

    switch(r.first - __TeRK__SerialIOService_all)
    {
	case 0:
	{
	    return ___blockingRead(in, current);
	}
	case 1:
	{
	    return ___blockingReadUntilDelimiter(in, current);
	}
	case 2:
	{
	    return ___close(in, current);
	}
	case 3:
	{
	    return ___getProperties(in, current);
	}
	case 4:
	{
	    return ___getProperty(in, current);
	}
	case 5:
	{
	    return ___getPropertyKeys(in, current);
	}
	case 6:
	{
	    return ___ice_id(in, current);
	}
	case 7:
	{
	    return ___ice_ids(in, current);
	}
	case 8:
	{
	    return ___ice_isA(in, current);
	}
	case 9:
	{
	    return ___ice_ping(in, current);
	}
	case 10:
	{
	    return ___isDataAvailable(in, current);
	}
	case 11:
	{
	    return ___isOpen(in, current);
	}
	case 12:
	{
	    return ___open(in, current);
	}
	case 13:
	{
	    return ___read(in, current);
	}
	case 14:
	{
	    return ___readUntilDelimiter(in, current);
	}
	case 15:
	{
	    return ___setProperty(in, current);
	}
	case 16:
	{
	    return ___write(in, current);
	}
    }

    assert(false);
    return ::IceInternal::DispatchOperationNotExist;
}

void
TeRK::SerialIOService::__write(::IceInternal::BasicStream* __os) const
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
TeRK::SerialIOService::__read(::IceInternal::BasicStream* __is, bool __rid)
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
TeRK::SerialIOService::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type TeRK::SerialIOService was not generated with stream support";
    throw ex;
}

void
TeRK::SerialIOService::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type TeRK::SerialIOService was not generated with stream support";
    throw ex;
}

void 
TeRK::__patch__SerialIOServicePtr(void* __addr, ::Ice::ObjectPtr& v)
{
    ::TeRK::SerialIOServicePtr* p = static_cast< ::TeRK::SerialIOServicePtr*>(__addr);
    assert(p);
    *p = ::TeRK::SerialIOServicePtr::dynamicCast(v);
    if(v && !*p)
    {
	::Ice::NoObjectFactoryException e(__FILE__, __LINE__);
	e.type = ::TeRK::SerialIOService::ice_staticId();
	throw e;
    }
}

bool
TeRK::operator==(const ::TeRK::SerialIOService& l, const ::TeRK::SerialIOService& r)
{
    return static_cast<const ::Ice::Object&>(l) == static_cast<const ::Ice::Object&>(r);
}

bool
TeRK::operator!=(const ::TeRK::SerialIOService& l, const ::TeRK::SerialIOService& r)
{
    return static_cast<const ::Ice::Object&>(l) != static_cast<const ::Ice::Object&>(r);
}

bool
TeRK::operator<(const ::TeRK::SerialIOService& l, const ::TeRK::SerialIOService& r)
{
    return static_cast<const ::Ice::Object&>(l) < static_cast<const ::Ice::Object&>(r);
}

bool
TeRK::operator<=(const ::TeRK::SerialIOService& l, const ::TeRK::SerialIOService& r)
{
    return l < r || l == r;
}

bool
TeRK::operator>(const ::TeRK::SerialIOService& l, const ::TeRK::SerialIOService& r)
{
    return !(l < r) && !(l == r);
}

bool
TeRK::operator>=(const ::TeRK::SerialIOService& l, const ::TeRK::SerialIOService& r)
{
    return !(l < r);
}

#endif
