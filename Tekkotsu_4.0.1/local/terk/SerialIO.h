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

#ifndef ___IFYRE_terk_source_new_TeRKPeerCommon_code_c____SerialIO_h__
#define ___IFYRE_terk_source_new_TeRKPeerCommon_code_c____SerialIO_h__

#include <Ice/LocalObjectF.h>
#include <Ice/ProxyF.h>
#include <Ice/ObjectF.h>
#include <Ice/Exception.h>
#include <Ice/LocalObject.h>
#include <Ice/Proxy.h>
#include <Ice/Object.h>
#include <Ice/Outgoing.h>
#include <Ice/OutgoingAsync.h>
#include <Ice/Incoming.h>
#include <Ice/Direct.h>
#include <Ice/UserExceptionFactory.h>
#include <Ice/FactoryTable.h>
#include <Ice/StreamF.h>
#include <Ice/UndefSysMacros.h>
#include "TeRKPeerCommon.h"

#ifndef ICE_IGNORE_VERSION
#   if ICE_INT_VERSION / 100 != 301
#       error Ice version mismatch!
#   endif
#   if ICE_INT_VERSION % 100 < 1
#       error Ice patch level mismatch!
#   endif
#endif

namespace IceProxy
{

namespace TeRK
{

class SerialIOService;
bool operator==(const SerialIOService&, const SerialIOService&);
bool operator!=(const SerialIOService&, const SerialIOService&);
bool operator<(const SerialIOService&, const SerialIOService&);
bool operator<=(const SerialIOService&, const SerialIOService&);
bool operator>(const SerialIOService&, const SerialIOService&);
bool operator>=(const SerialIOService&, const SerialIOService&);

}

}

namespace TeRK
{

class SerialIOService;
bool operator==(const SerialIOService&, const SerialIOService&);
bool operator!=(const SerialIOService&, const SerialIOService&);
bool operator<(const SerialIOService&, const SerialIOService&);
bool operator<=(const SerialIOService&, const SerialIOService&);
bool operator>(const SerialIOService&, const SerialIOService&);
bool operator>=(const SerialIOService&, const SerialIOService&);

}

namespace IceInternal
{

void incRef(::TeRK::SerialIOService*);
void decRef(::TeRK::SerialIOService*);

void incRef(::IceProxy::TeRK::SerialIOService*);
void decRef(::IceProxy::TeRK::SerialIOService*);

}

namespace TeRK
{

typedef ::IceInternal::Handle< ::TeRK::SerialIOService> SerialIOServicePtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::TeRK::SerialIOService> SerialIOServicePrx;

void __write(::IceInternal::BasicStream*, const SerialIOServicePrx&);
void __read(::IceInternal::BasicStream*, SerialIOServicePrx&);
void __write(::IceInternal::BasicStream*, const SerialIOServicePtr&);
void __patch__SerialIOServicePtr(void*, ::Ice::ObjectPtr&);

}

namespace TeRK
{

enum SerialIOBaudRate
{
    BAUD50,
    BAUD75,
    BAUD110,
    BAUD134,
    BAUD150,
    BAUD200,
    BAUD300,
    BAUD600,
    BAUD1200,
    BAUD1800,
    BAUD2400,
    BAUD4800,
    BAUD9600,
    BAUD19200,
    BAUD38400,
    BAUD57600,
    BAUD115200
};

void __write(::IceInternal::BasicStream*, SerialIOBaudRate);
void __read(::IceInternal::BasicStream*, SerialIOBaudRate&);

enum SerialIOCharacterSize
{
    CHARSIZE5,
    CHARSIZE6,
    CHARSIZE7,
    CHARSIZE8
};

void __write(::IceInternal::BasicStream*, SerialIOCharacterSize);
void __read(::IceInternal::BasicStream*, SerialIOCharacterSize&);

enum SerialIOParity
{
    PARITYEVEN,
    PARITYODD,
    PARITYNONE
};

void __write(::IceInternal::BasicStream*, SerialIOParity);
void __read(::IceInternal::BasicStream*, SerialIOParity&);

enum SerialIOStopBits
{
    STOPBITS1,
    STOPBITS2
};

void __write(::IceInternal::BasicStream*, SerialIOStopBits);
void __read(::IceInternal::BasicStream*, SerialIOStopBits&);

enum SerialIOFlowControl
{
    FLOWCONTROLHARDWARE,
    FLOWCONTROLSOFTWARE,
    FLOWCONTROLNONE
};

void __write(::IceInternal::BasicStream*, SerialIOFlowControl);
void __read(::IceInternal::BasicStream*, SerialIOFlowControl&);

struct SerialIOConfig
{
    ::std::string portName;
    ::TeRK::SerialIOBaudRate baud;
    ::TeRK::SerialIOCharacterSize characterSize;
    ::TeRK::SerialIOParity parity;
    ::TeRK::SerialIOStopBits stopBits;
    ::TeRK::SerialIOFlowControl flowControl;

    bool operator==(const SerialIOConfig&) const;
    bool operator!=(const SerialIOConfig&) const;
    bool operator<(const SerialIOConfig&) const;
    bool operator<=(const SerialIOConfig& __rhs) const
    {
	return operator<(__rhs) || operator==(__rhs);
    }
    bool operator>(const SerialIOConfig& __rhs) const
    {
	return !operator<(__rhs) && !operator==(__rhs);
    }
    bool operator>=(const SerialIOConfig& __rhs) const
    {
	return !operator<(__rhs);
    }

    void __write(::IceInternal::BasicStream*) const;
    void __read(::IceInternal::BasicStream*);
};

class SerialIOException : public ::TeRK::TeRKException
{
public:

    SerialIOException() {}
    explicit SerialIOException(const ::std::string&);

    virtual const ::std::string ice_name() const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;

    static const ::IceInternal::UserExceptionFactoryPtr& ice_factory();

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);

    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

static SerialIOException __SerialIOException_init;

}

namespace TeRK
{

class AMI_SerialIOService_open : public ::IceInternal::OutgoingAsync
{
public:

    virtual void ice_response() = 0;
    virtual void ice_exception(const ::Ice::Exception&) = 0;

    void __invoke(const ::TeRK::SerialIOServicePrx&, const ::TeRK::SerialIOConfig&, const ::Ice::Context&);

protected:

    virtual void __response(bool);
};

typedef ::IceUtil::Handle< ::TeRK::AMI_SerialIOService_open> AMI_SerialIOService_openPtr;

class AMI_SerialIOService_isOpen : public ::IceInternal::OutgoingAsync
{
public:

    virtual void ice_response(bool) = 0;
    virtual void ice_exception(const ::Ice::Exception&) = 0;

    void __invoke(const ::TeRK::SerialIOServicePrx&, const ::std::string&, const ::Ice::Context&);

protected:

    virtual void __response(bool);
};

typedef ::IceUtil::Handle< ::TeRK::AMI_SerialIOService_isOpen> AMI_SerialIOService_isOpenPtr;

class AMI_SerialIOService_isDataAvailable : public ::IceInternal::OutgoingAsync
{
public:

    virtual void ice_response(bool) = 0;
    virtual void ice_exception(const ::Ice::Exception&) = 0;

    void __invoke(const ::TeRK::SerialIOServicePrx&, const ::std::string&, const ::Ice::Context&);

protected:

    virtual void __response(bool);
};

typedef ::IceUtil::Handle< ::TeRK::AMI_SerialIOService_isDataAvailable> AMI_SerialIOService_isDataAvailablePtr;

class AMI_SerialIOService_read : public ::IceInternal::OutgoingAsync
{
public:

    virtual void ice_response(const ::TeRK::ByteArray&) = 0;
    virtual void ice_exception(const ::Ice::Exception&) = 0;

    void __invoke(const ::TeRK::SerialIOServicePrx&, const ::std::string&, ::Ice::Int, const ::Ice::Context&);

protected:

    virtual void __response(bool);
};

typedef ::IceUtil::Handle< ::TeRK::AMI_SerialIOService_read> AMI_SerialIOService_readPtr;

class AMI_SerialIOService_blockingRead : public ::IceInternal::OutgoingAsync
{
public:

    virtual void ice_response(const ::TeRK::ByteArray&) = 0;
    virtual void ice_exception(const ::Ice::Exception&) = 0;

    void __invoke(const ::TeRK::SerialIOServicePrx&, const ::std::string&, ::Ice::Int, ::Ice::Int, const ::Ice::Context&);

protected:

    virtual void __response(bool);
};

typedef ::IceUtil::Handle< ::TeRK::AMI_SerialIOService_blockingRead> AMI_SerialIOService_blockingReadPtr;

class AMI_SerialIOService_readUntilDelimiter : public ::IceInternal::OutgoingAsync
{
public:

    virtual void ice_response(const ::TeRK::ByteArray&) = 0;
    virtual void ice_exception(const ::Ice::Exception&) = 0;

    void __invoke(const ::TeRK::SerialIOServicePrx&, const ::std::string&, ::Ice::Int, ::Ice::Byte, const ::Ice::Context&);

protected:

    virtual void __response(bool);
};

typedef ::IceUtil::Handle< ::TeRK::AMI_SerialIOService_readUntilDelimiter> AMI_SerialIOService_readUntilDelimiterPtr;

class AMI_SerialIOService_blockingReadUntilDelimiter : public ::IceInternal::OutgoingAsync
{
public:

    virtual void ice_response(const ::TeRK::ByteArray&) = 0;
    virtual void ice_exception(const ::Ice::Exception&) = 0;

    void __invoke(const ::TeRK::SerialIOServicePrx&, const ::std::string&, ::Ice::Int, ::Ice::Byte, ::Ice::Int, const ::Ice::Context&);

protected:

    virtual void __response(bool);
};

typedef ::IceUtil::Handle< ::TeRK::AMI_SerialIOService_blockingReadUntilDelimiter> AMI_SerialIOService_blockingReadUntilDelimiterPtr;

class AMI_SerialIOService_write : public ::IceInternal::OutgoingAsync
{
public:

    virtual void ice_response() = 0;
    virtual void ice_exception(const ::Ice::Exception&) = 0;

    void __invoke(const ::TeRK::SerialIOServicePrx&, const ::std::string&, const ::TeRK::ByteArray&, const ::Ice::Context&);

protected:

    virtual void __response(bool);
};

typedef ::IceUtil::Handle< ::TeRK::AMI_SerialIOService_write> AMI_SerialIOService_writePtr;

class AMI_SerialIOService_close : public ::IceInternal::OutgoingAsync
{
public:

    virtual void ice_response() = 0;
    virtual void ice_exception(const ::Ice::Exception&) = 0;

    void __invoke(const ::TeRK::SerialIOServicePrx&, const ::std::string&, const ::Ice::Context&);

protected:

    virtual void __response(bool);
};

typedef ::IceUtil::Handle< ::TeRK::AMI_SerialIOService_close> AMI_SerialIOService_closePtr;

}

namespace IceProxy
{

namespace TeRK
{

class SerialIOService : virtual public ::IceProxy::TeRK::AbstractCommandController
{
public:

    void open(const ::TeRK::SerialIOConfig& config)
    {
	open(config, __defaultContext());
    }
    void open(const ::TeRK::SerialIOConfig&, const ::Ice::Context&);
    void open_async(const ::TeRK::AMI_SerialIOService_openPtr&, const ::TeRK::SerialIOConfig&);
    void open_async(const ::TeRK::AMI_SerialIOService_openPtr&, const ::TeRK::SerialIOConfig&, const ::Ice::Context&);

    bool isOpen(const ::std::string& portName)
    {
	return isOpen(portName, __defaultContext());
    }
    bool isOpen(const ::std::string&, const ::Ice::Context&);
    void isOpen_async(const ::TeRK::AMI_SerialIOService_isOpenPtr&, const ::std::string&);
    void isOpen_async(const ::TeRK::AMI_SerialIOService_isOpenPtr&, const ::std::string&, const ::Ice::Context&);

    bool isDataAvailable(const ::std::string& portName)
    {
	return isDataAvailable(portName, __defaultContext());
    }
    bool isDataAvailable(const ::std::string&, const ::Ice::Context&);
    void isDataAvailable_async(const ::TeRK::AMI_SerialIOService_isDataAvailablePtr&, const ::std::string&);
    void isDataAvailable_async(const ::TeRK::AMI_SerialIOService_isDataAvailablePtr&, const ::std::string&, const ::Ice::Context&);

    ::TeRK::ByteArray read(const ::std::string& portName, ::Ice::Int maxNumberOfBytesToRead)
    {
	return read(portName, maxNumberOfBytesToRead, __defaultContext());
    }
    ::TeRK::ByteArray read(const ::std::string&, ::Ice::Int, const ::Ice::Context&);
    void read_async(const ::TeRK::AMI_SerialIOService_readPtr&, const ::std::string&, ::Ice::Int);
    void read_async(const ::TeRK::AMI_SerialIOService_readPtr&, const ::std::string&, ::Ice::Int, const ::Ice::Context&);

    ::TeRK::ByteArray blockingRead(const ::std::string& portName, ::Ice::Int maxNumberOfBytesToRead, ::Ice::Int timeoutMilliseconds)
    {
	return blockingRead(portName, maxNumberOfBytesToRead, timeoutMilliseconds, __defaultContext());
    }
    ::TeRK::ByteArray blockingRead(const ::std::string&, ::Ice::Int, ::Ice::Int, const ::Ice::Context&);
    void blockingRead_async(const ::TeRK::AMI_SerialIOService_blockingReadPtr&, const ::std::string&, ::Ice::Int, ::Ice::Int);
    void blockingRead_async(const ::TeRK::AMI_SerialIOService_blockingReadPtr&, const ::std::string&, ::Ice::Int, ::Ice::Int, const ::Ice::Context&);

    ::TeRK::ByteArray readUntilDelimiter(const ::std::string& portName, ::Ice::Int maxNumberOfBytesToRead, ::Ice::Byte delimiterCharacter)
    {
	return readUntilDelimiter(portName, maxNumberOfBytesToRead, delimiterCharacter, __defaultContext());
    }
    ::TeRK::ByteArray readUntilDelimiter(const ::std::string&, ::Ice::Int, ::Ice::Byte, const ::Ice::Context&);
    void readUntilDelimiter_async(const ::TeRK::AMI_SerialIOService_readUntilDelimiterPtr&, const ::std::string&, ::Ice::Int, ::Ice::Byte);
    void readUntilDelimiter_async(const ::TeRK::AMI_SerialIOService_readUntilDelimiterPtr&, const ::std::string&, ::Ice::Int, ::Ice::Byte, const ::Ice::Context&);

    ::TeRK::ByteArray blockingReadUntilDelimiter(const ::std::string& portName, ::Ice::Int maxNumberOfBytesToRead, ::Ice::Byte delimiterCharacter, ::Ice::Int timeoutMilliseconds)
    {
	return blockingReadUntilDelimiter(portName, maxNumberOfBytesToRead, delimiterCharacter, timeoutMilliseconds, __defaultContext());
    }
    ::TeRK::ByteArray blockingReadUntilDelimiter(const ::std::string&, ::Ice::Int, ::Ice::Byte, ::Ice::Int, const ::Ice::Context&);
    void blockingReadUntilDelimiter_async(const ::TeRK::AMI_SerialIOService_blockingReadUntilDelimiterPtr&, const ::std::string&, ::Ice::Int, ::Ice::Byte, ::Ice::Int);
    void blockingReadUntilDelimiter_async(const ::TeRK::AMI_SerialIOService_blockingReadUntilDelimiterPtr&, const ::std::string&, ::Ice::Int, ::Ice::Byte, ::Ice::Int, const ::Ice::Context&);

    void write(const ::std::string& portName, const ::TeRK::ByteArray& data)
    {
	write(portName, data, __defaultContext());
    }
    void write(const ::std::string&, const ::TeRK::ByteArray&, const ::Ice::Context&);
    void write_async(const ::TeRK::AMI_SerialIOService_writePtr&, const ::std::string&, const ::TeRK::ByteArray&);
    void write_async(const ::TeRK::AMI_SerialIOService_writePtr&, const ::std::string&, const ::TeRK::ByteArray&, const ::Ice::Context&);

    void close(const ::std::string& portName)
    {
	close(portName, __defaultContext());
    }
    void close(const ::std::string&, const ::Ice::Context&);
    void close_async(const ::TeRK::AMI_SerialIOService_closePtr&, const ::std::string&);
    void close_async(const ::TeRK::AMI_SerialIOService_closePtr&, const ::std::string&, const ::Ice::Context&);
    
    static const ::std::string& ice_staticId();

private: 

    virtual ::IceInternal::Handle< ::IceDelegateM::Ice::Object> __createDelegateM();
    virtual ::IceInternal::Handle< ::IceDelegateD::Ice::Object> __createDelegateD();
};

}

}

namespace IceDelegate
{

namespace TeRK
{

class SerialIOService : virtual public ::IceDelegate::TeRK::AbstractCommandController
{
public:

    virtual void open(const ::TeRK::SerialIOConfig&, const ::Ice::Context&) = 0;

    virtual bool isOpen(const ::std::string&, const ::Ice::Context&) = 0;

    virtual bool isDataAvailable(const ::std::string&, const ::Ice::Context&) = 0;

    virtual ::TeRK::ByteArray read(const ::std::string&, ::Ice::Int, const ::Ice::Context&) = 0;

    virtual ::TeRK::ByteArray blockingRead(const ::std::string&, ::Ice::Int, ::Ice::Int, const ::Ice::Context&) = 0;

    virtual ::TeRK::ByteArray readUntilDelimiter(const ::std::string&, ::Ice::Int, ::Ice::Byte, const ::Ice::Context&) = 0;

    virtual ::TeRK::ByteArray blockingReadUntilDelimiter(const ::std::string&, ::Ice::Int, ::Ice::Byte, ::Ice::Int, const ::Ice::Context&) = 0;

    virtual void write(const ::std::string&, const ::TeRK::ByteArray&, const ::Ice::Context&) = 0;

    virtual void close(const ::std::string&, const ::Ice::Context&) = 0;
};

}

}

namespace IceDelegateM
{

namespace TeRK
{

class SerialIOService : virtual public ::IceDelegate::TeRK::SerialIOService,
			virtual public ::IceDelegateM::TeRK::AbstractCommandController
{
public:

    virtual void open(const ::TeRK::SerialIOConfig&, const ::Ice::Context&);

    virtual bool isOpen(const ::std::string&, const ::Ice::Context&);

    virtual bool isDataAvailable(const ::std::string&, const ::Ice::Context&);

    virtual ::TeRK::ByteArray read(const ::std::string&, ::Ice::Int, const ::Ice::Context&);

    virtual ::TeRK::ByteArray blockingRead(const ::std::string&, ::Ice::Int, ::Ice::Int, const ::Ice::Context&);

    virtual ::TeRK::ByteArray readUntilDelimiter(const ::std::string&, ::Ice::Int, ::Ice::Byte, const ::Ice::Context&);

    virtual ::TeRK::ByteArray blockingReadUntilDelimiter(const ::std::string&, ::Ice::Int, ::Ice::Byte, ::Ice::Int, const ::Ice::Context&);

    virtual void write(const ::std::string&, const ::TeRK::ByteArray&, const ::Ice::Context&);

    virtual void close(const ::std::string&, const ::Ice::Context&);
};

}

}

namespace IceDelegateD
{

namespace TeRK
{

class SerialIOService : virtual public ::IceDelegate::TeRK::SerialIOService,
			virtual public ::IceDelegateD::TeRK::AbstractCommandController
{
public:

    virtual void open(const ::TeRK::SerialIOConfig&, const ::Ice::Context&);

    virtual bool isOpen(const ::std::string&, const ::Ice::Context&);

    virtual bool isDataAvailable(const ::std::string&, const ::Ice::Context&);

    virtual ::TeRK::ByteArray read(const ::std::string&, ::Ice::Int, const ::Ice::Context&);

    virtual ::TeRK::ByteArray blockingRead(const ::std::string&, ::Ice::Int, ::Ice::Int, const ::Ice::Context&);

    virtual ::TeRK::ByteArray readUntilDelimiter(const ::std::string&, ::Ice::Int, ::Ice::Byte, const ::Ice::Context&);

    virtual ::TeRK::ByteArray blockingReadUntilDelimiter(const ::std::string&, ::Ice::Int, ::Ice::Byte, ::Ice::Int, const ::Ice::Context&);

    virtual void write(const ::std::string&, const ::TeRK::ByteArray&, const ::Ice::Context&);

    virtual void close(const ::std::string&, const ::Ice::Context&);
};

}

}

namespace TeRK
{

class SerialIOService : virtual public ::TeRK::AbstractCommandController
{
public:

    typedef SerialIOServicePrx ProxyType;
    typedef SerialIOServicePtr PointerType;
    
    virtual ::Ice::ObjectPtr ice_clone() const;

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();

    virtual void open(const ::TeRK::SerialIOConfig&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___open(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual bool isOpen(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___isOpen(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual bool isDataAvailable(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___isDataAvailable(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::TeRK::ByteArray read(const ::std::string&, ::Ice::Int, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___read(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::TeRK::ByteArray blockingRead(const ::std::string&, ::Ice::Int, ::Ice::Int, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___blockingRead(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::TeRK::ByteArray readUntilDelimiter(const ::std::string&, ::Ice::Int, ::Ice::Byte, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___readUntilDelimiter(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::TeRK::ByteArray blockingReadUntilDelimiter(const ::std::string&, ::Ice::Int, ::Ice::Byte, ::Ice::Int, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___blockingReadUntilDelimiter(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void write(const ::std::string&, const ::TeRK::ByteArray&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___write(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void close(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___close(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::IceInternal::DispatchStatus __dispatch(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);
    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

void __patch__SerialIOServicePtr(void*, ::Ice::ObjectPtr&);

}

#endif
