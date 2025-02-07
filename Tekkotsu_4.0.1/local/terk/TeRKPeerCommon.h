// **********************************************************************
//
// Copyright (c) 2003-2006 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

// Ice version 3.1.1
// Generated from file `TeRKPeerCommon.ice'

#ifndef ___home_btsai_terk_source_TeRKPeerCommon_code_c____TeRKPeerCommon_h__
#define ___home_btsai_terk_source_TeRKPeerCommon_code_c____TeRKPeerCommon_h__

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
#include <Ice/Identity.h>
#include "peer/MRPLPeer.h"
#include <Ice/UndefSysMacros.h>

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

class PropertyManager;
bool operator==(const PropertyManager&, const PropertyManager&);
bool operator!=(const PropertyManager&, const PropertyManager&);
bool operator<(const PropertyManager&, const PropertyManager&);
bool operator<=(const PropertyManager&, const PropertyManager&);
bool operator>(const PropertyManager&, const PropertyManager&);
bool operator>=(const PropertyManager&, const PropertyManager&);

class AbstractCommandController;
bool operator==(const AbstractCommandController&, const AbstractCommandController&);
bool operator!=(const AbstractCommandController&, const AbstractCommandController&);
bool operator<(const AbstractCommandController&, const AbstractCommandController&);
bool operator<=(const AbstractCommandController&, const AbstractCommandController&);
bool operator>(const AbstractCommandController&, const AbstractCommandController&);
bool operator>=(const AbstractCommandController&, const AbstractCommandController&);

class AnalogInController;
bool operator==(const AnalogInController&, const AnalogInController&);
bool operator!=(const AnalogInController&, const AnalogInController&);
bool operator<(const AnalogInController&, const AnalogInController&);
bool operator<=(const AnalogInController&, const AnalogInController&);
bool operator>(const AnalogInController&, const AnalogInController&);
bool operator>=(const AnalogInController&, const AnalogInController&);

class AudioController;
bool operator==(const AudioController&, const AudioController&);
bool operator!=(const AudioController&, const AudioController&);
bool operator<(const AudioController&, const AudioController&);
bool operator<=(const AudioController&, const AudioController&);
bool operator>(const AudioController&, const AudioController&);
bool operator>=(const AudioController&, const AudioController&);

class DigitalInController;
bool operator==(const DigitalInController&, const DigitalInController&);
bool operator!=(const DigitalInController&, const DigitalInController&);
bool operator<(const DigitalInController&, const DigitalInController&);
bool operator<=(const DigitalInController&, const DigitalInController&);
bool operator>(const DigitalInController&, const DigitalInController&);
bool operator>=(const DigitalInController&, const DigitalInController&);

class DigitalOutController;
bool operator==(const DigitalOutController&, const DigitalOutController&);
bool operator!=(const DigitalOutController&, const DigitalOutController&);
bool operator<(const DigitalOutController&, const DigitalOutController&);
bool operator<=(const DigitalOutController&, const DigitalOutController&);
bool operator>(const DigitalOutController&, const DigitalOutController&);
bool operator>=(const DigitalOutController&, const DigitalOutController&);

class LEDController;
bool operator==(const LEDController&, const LEDController&);
bool operator!=(const LEDController&, const LEDController&);
bool operator<(const LEDController&, const LEDController&);
bool operator<=(const LEDController&, const LEDController&);
bool operator>(const LEDController&, const LEDController&);
bool operator>=(const LEDController&, const LEDController&);

class MotorController;
bool operator==(const MotorController&, const MotorController&);
bool operator!=(const MotorController&, const MotorController&);
bool operator<(const MotorController&, const MotorController&);
bool operator<=(const MotorController&, const MotorController&);
bool operator>(const MotorController&, const MotorController&);
bool operator>=(const MotorController&, const MotorController&);

class ServoController;
bool operator==(const ServoController&, const ServoController&);
bool operator!=(const ServoController&, const ServoController&);
bool operator<(const ServoController&, const ServoController&);
bool operator<=(const ServoController&, const ServoController&);
bool operator>(const ServoController&, const ServoController&);
bool operator>=(const ServoController&, const ServoController&);

class VideoStreamerClient;
bool operator==(const VideoStreamerClient&, const VideoStreamerClient&);
bool operator!=(const VideoStreamerClient&, const VideoStreamerClient&);
bool operator<(const VideoStreamerClient&, const VideoStreamerClient&);
bool operator<=(const VideoStreamerClient&, const VideoStreamerClient&);
bool operator>(const VideoStreamerClient&, const VideoStreamerClient&);
bool operator>=(const VideoStreamerClient&, const VideoStreamerClient&);

class VideoStreamerServer;
bool operator==(const VideoStreamerServer&, const VideoStreamerServer&);
bool operator!=(const VideoStreamerServer&, const VideoStreamerServer&);
bool operator<(const VideoStreamerServer&, const VideoStreamerServer&);
bool operator<=(const VideoStreamerServer&, const VideoStreamerServer&);
bool operator>(const VideoStreamerServer&, const VideoStreamerServer&);
bool operator>=(const VideoStreamerServer&, const VideoStreamerServer&);

class TerkUser;
bool operator==(const TerkUser&, const TerkUser&);
bool operator!=(const TerkUser&, const TerkUser&);
bool operator<(const TerkUser&, const TerkUser&);
bool operator<=(const TerkUser&, const TerkUser&);
bool operator>(const TerkUser&, const TerkUser&);
bool operator>=(const TerkUser&, const TerkUser&);

class Qwerk;
bool operator==(const Qwerk&, const Qwerk&);
bool operator!=(const Qwerk&, const Qwerk&);
bool operator<(const Qwerk&, const Qwerk&);
bool operator<=(const Qwerk&, const Qwerk&);
bool operator>(const Qwerk&, const Qwerk&);
bool operator>=(const Qwerk&, const Qwerk&);

class TerkClient;
bool operator==(const TerkClient&, const TerkClient&);
bool operator!=(const TerkClient&, const TerkClient&);
bool operator<(const TerkClient&, const TerkClient&);
bool operator<=(const TerkClient&, const TerkClient&);
bool operator>(const TerkClient&, const TerkClient&);
bool operator>=(const TerkClient&, const TerkClient&);

}

}

namespace TeRK
{

class PropertyManager;
bool operator==(const PropertyManager&, const PropertyManager&);
bool operator!=(const PropertyManager&, const PropertyManager&);
bool operator<(const PropertyManager&, const PropertyManager&);
bool operator<=(const PropertyManager&, const PropertyManager&);
bool operator>(const PropertyManager&, const PropertyManager&);
bool operator>=(const PropertyManager&, const PropertyManager&);

class AbstractCommandController;
bool operator==(const AbstractCommandController&, const AbstractCommandController&);
bool operator!=(const AbstractCommandController&, const AbstractCommandController&);
bool operator<(const AbstractCommandController&, const AbstractCommandController&);
bool operator<=(const AbstractCommandController&, const AbstractCommandController&);
bool operator>(const AbstractCommandController&, const AbstractCommandController&);
bool operator>=(const AbstractCommandController&, const AbstractCommandController&);

class AnalogInController;
bool operator==(const AnalogInController&, const AnalogInController&);
bool operator!=(const AnalogInController&, const AnalogInController&);
bool operator<(const AnalogInController&, const AnalogInController&);
bool operator<=(const AnalogInController&, const AnalogInController&);
bool operator>(const AnalogInController&, const AnalogInController&);
bool operator>=(const AnalogInController&, const AnalogInController&);

class AudioController;
bool operator==(const AudioController&, const AudioController&);
bool operator!=(const AudioController&, const AudioController&);
bool operator<(const AudioController&, const AudioController&);
bool operator<=(const AudioController&, const AudioController&);
bool operator>(const AudioController&, const AudioController&);
bool operator>=(const AudioController&, const AudioController&);

class DigitalInController;
bool operator==(const DigitalInController&, const DigitalInController&);
bool operator!=(const DigitalInController&, const DigitalInController&);
bool operator<(const DigitalInController&, const DigitalInController&);
bool operator<=(const DigitalInController&, const DigitalInController&);
bool operator>(const DigitalInController&, const DigitalInController&);
bool operator>=(const DigitalInController&, const DigitalInController&);

class DigitalOutController;
bool operator==(const DigitalOutController&, const DigitalOutController&);
bool operator!=(const DigitalOutController&, const DigitalOutController&);
bool operator<(const DigitalOutController&, const DigitalOutController&);
bool operator<=(const DigitalOutController&, const DigitalOutController&);
bool operator>(const DigitalOutController&, const DigitalOutController&);
bool operator>=(const DigitalOutController&, const DigitalOutController&);

class LEDController;
bool operator==(const LEDController&, const LEDController&);
bool operator!=(const LEDController&, const LEDController&);
bool operator<(const LEDController&, const LEDController&);
bool operator<=(const LEDController&, const LEDController&);
bool operator>(const LEDController&, const LEDController&);
bool operator>=(const LEDController&, const LEDController&);

class MotorController;
bool operator==(const MotorController&, const MotorController&);
bool operator!=(const MotorController&, const MotorController&);
bool operator<(const MotorController&, const MotorController&);
bool operator<=(const MotorController&, const MotorController&);
bool operator>(const MotorController&, const MotorController&);
bool operator>=(const MotorController&, const MotorController&);

class ServoController;
bool operator==(const ServoController&, const ServoController&);
bool operator!=(const ServoController&, const ServoController&);
bool operator<(const ServoController&, const ServoController&);
bool operator<=(const ServoController&, const ServoController&);
bool operator>(const ServoController&, const ServoController&);
bool operator>=(const ServoController&, const ServoController&);

class VideoStreamerClient;
bool operator==(const VideoStreamerClient&, const VideoStreamerClient&);
bool operator!=(const VideoStreamerClient&, const VideoStreamerClient&);
bool operator<(const VideoStreamerClient&, const VideoStreamerClient&);
bool operator<=(const VideoStreamerClient&, const VideoStreamerClient&);
bool operator>(const VideoStreamerClient&, const VideoStreamerClient&);
bool operator>=(const VideoStreamerClient&, const VideoStreamerClient&);

class VideoStreamerServer;
bool operator==(const VideoStreamerServer&, const VideoStreamerServer&);
bool operator!=(const VideoStreamerServer&, const VideoStreamerServer&);
bool operator<(const VideoStreamerServer&, const VideoStreamerServer&);
bool operator<=(const VideoStreamerServer&, const VideoStreamerServer&);
bool operator>(const VideoStreamerServer&, const VideoStreamerServer&);
bool operator>=(const VideoStreamerServer&, const VideoStreamerServer&);

class TerkUser;
bool operator==(const TerkUser&, const TerkUser&);
bool operator!=(const TerkUser&, const TerkUser&);
bool operator<(const TerkUser&, const TerkUser&);
bool operator<=(const TerkUser&, const TerkUser&);
bool operator>(const TerkUser&, const TerkUser&);
bool operator>=(const TerkUser&, const TerkUser&);

class Qwerk;
bool operator==(const Qwerk&, const Qwerk&);
bool operator!=(const Qwerk&, const Qwerk&);
bool operator<(const Qwerk&, const Qwerk&);
bool operator<=(const Qwerk&, const Qwerk&);
bool operator>(const Qwerk&, const Qwerk&);
bool operator>=(const Qwerk&, const Qwerk&);

class TerkClient;
bool operator==(const TerkClient&, const TerkClient&);
bool operator!=(const TerkClient&, const TerkClient&);
bool operator<(const TerkClient&, const TerkClient&);
bool operator<=(const TerkClient&, const TerkClient&);
bool operator>(const TerkClient&, const TerkClient&);
bool operator>=(const TerkClient&, const TerkClient&);

}

namespace IceInternal
{

void incRef(::TeRK::PropertyManager*);
void decRef(::TeRK::PropertyManager*);

void incRef(::IceProxy::TeRK::PropertyManager*);
void decRef(::IceProxy::TeRK::PropertyManager*);

void incRef(::TeRK::AbstractCommandController*);
void decRef(::TeRK::AbstractCommandController*);

void incRef(::IceProxy::TeRK::AbstractCommandController*);
void decRef(::IceProxy::TeRK::AbstractCommandController*);

void incRef(::TeRK::AnalogInController*);
void decRef(::TeRK::AnalogInController*);

void incRef(::IceProxy::TeRK::AnalogInController*);
void decRef(::IceProxy::TeRK::AnalogInController*);

void incRef(::TeRK::AudioController*);
void decRef(::TeRK::AudioController*);

void incRef(::IceProxy::TeRK::AudioController*);
void decRef(::IceProxy::TeRK::AudioController*);

void incRef(::TeRK::DigitalInController*);
void decRef(::TeRK::DigitalInController*);

void incRef(::IceProxy::TeRK::DigitalInController*);
void decRef(::IceProxy::TeRK::DigitalInController*);

void incRef(::TeRK::DigitalOutController*);
void decRef(::TeRK::DigitalOutController*);

void incRef(::IceProxy::TeRK::DigitalOutController*);
void decRef(::IceProxy::TeRK::DigitalOutController*);

void incRef(::TeRK::LEDController*);
void decRef(::TeRK::LEDController*);

void incRef(::IceProxy::TeRK::LEDController*);
void decRef(::IceProxy::TeRK::LEDController*);

void incRef(::TeRK::MotorController*);
void decRef(::TeRK::MotorController*);

void incRef(::IceProxy::TeRK::MotorController*);
void decRef(::IceProxy::TeRK::MotorController*);

void incRef(::TeRK::ServoController*);
void decRef(::TeRK::ServoController*);

void incRef(::IceProxy::TeRK::ServoController*);
void decRef(::IceProxy::TeRK::ServoController*);

void incRef(::TeRK::VideoStreamerClient*);
void decRef(::TeRK::VideoStreamerClient*);

void incRef(::IceProxy::TeRK::VideoStreamerClient*);
void decRef(::IceProxy::TeRK::VideoStreamerClient*);

void incRef(::TeRK::VideoStreamerServer*);
void decRef(::TeRK::VideoStreamerServer*);

void incRef(::IceProxy::TeRK::VideoStreamerServer*);
void decRef(::IceProxy::TeRK::VideoStreamerServer*);

void incRef(::TeRK::TerkUser*);
void decRef(::TeRK::TerkUser*);

void incRef(::IceProxy::TeRK::TerkUser*);
void decRef(::IceProxy::TeRK::TerkUser*);

void incRef(::TeRK::Qwerk*);
void decRef(::TeRK::Qwerk*);

void incRef(::IceProxy::TeRK::Qwerk*);
void decRef(::IceProxy::TeRK::Qwerk*);

void incRef(::TeRK::TerkClient*);
void decRef(::TeRK::TerkClient*);

void incRef(::IceProxy::TeRK::TerkClient*);
void decRef(::IceProxy::TeRK::TerkClient*);

}

namespace TeRK
{

typedef ::IceInternal::Handle< ::TeRK::PropertyManager> PropertyManagerPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::TeRK::PropertyManager> PropertyManagerPrx;

void __write(::IceInternal::BasicStream*, const PropertyManagerPrx&);
void __read(::IceInternal::BasicStream*, PropertyManagerPrx&);
void __write(::IceInternal::BasicStream*, const PropertyManagerPtr&);
void __patch__PropertyManagerPtr(void*, ::Ice::ObjectPtr&);

typedef ::IceInternal::Handle< ::TeRK::AbstractCommandController> AbstractCommandControllerPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::TeRK::AbstractCommandController> AbstractCommandControllerPrx;

void __write(::IceInternal::BasicStream*, const AbstractCommandControllerPrx&);
void __read(::IceInternal::BasicStream*, AbstractCommandControllerPrx&);
void __write(::IceInternal::BasicStream*, const AbstractCommandControllerPtr&);
void __patch__AbstractCommandControllerPtr(void*, ::Ice::ObjectPtr&);

typedef ::IceInternal::Handle< ::TeRK::AnalogInController> AnalogInControllerPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::TeRK::AnalogInController> AnalogInControllerPrx;

void __write(::IceInternal::BasicStream*, const AnalogInControllerPrx&);
void __read(::IceInternal::BasicStream*, AnalogInControllerPrx&);
void __write(::IceInternal::BasicStream*, const AnalogInControllerPtr&);
void __patch__AnalogInControllerPtr(void*, ::Ice::ObjectPtr&);

typedef ::IceInternal::Handle< ::TeRK::AudioController> AudioControllerPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::TeRK::AudioController> AudioControllerPrx;

void __write(::IceInternal::BasicStream*, const AudioControllerPrx&);
void __read(::IceInternal::BasicStream*, AudioControllerPrx&);
void __write(::IceInternal::BasicStream*, const AudioControllerPtr&);
void __patch__AudioControllerPtr(void*, ::Ice::ObjectPtr&);

typedef ::IceInternal::Handle< ::TeRK::DigitalInController> DigitalInControllerPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::TeRK::DigitalInController> DigitalInControllerPrx;

void __write(::IceInternal::BasicStream*, const DigitalInControllerPrx&);
void __read(::IceInternal::BasicStream*, DigitalInControllerPrx&);
void __write(::IceInternal::BasicStream*, const DigitalInControllerPtr&);
void __patch__DigitalInControllerPtr(void*, ::Ice::ObjectPtr&);

typedef ::IceInternal::Handle< ::TeRK::DigitalOutController> DigitalOutControllerPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::TeRK::DigitalOutController> DigitalOutControllerPrx;

void __write(::IceInternal::BasicStream*, const DigitalOutControllerPrx&);
void __read(::IceInternal::BasicStream*, DigitalOutControllerPrx&);
void __write(::IceInternal::BasicStream*, const DigitalOutControllerPtr&);
void __patch__DigitalOutControllerPtr(void*, ::Ice::ObjectPtr&);

typedef ::IceInternal::Handle< ::TeRK::LEDController> LEDControllerPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::TeRK::LEDController> LEDControllerPrx;

void __write(::IceInternal::BasicStream*, const LEDControllerPrx&);
void __read(::IceInternal::BasicStream*, LEDControllerPrx&);
void __write(::IceInternal::BasicStream*, const LEDControllerPtr&);
void __patch__LEDControllerPtr(void*, ::Ice::ObjectPtr&);

typedef ::IceInternal::Handle< ::TeRK::MotorController> MotorControllerPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::TeRK::MotorController> MotorControllerPrx;

void __write(::IceInternal::BasicStream*, const MotorControllerPrx&);
void __read(::IceInternal::BasicStream*, MotorControllerPrx&);
void __write(::IceInternal::BasicStream*, const MotorControllerPtr&);
void __patch__MotorControllerPtr(void*, ::Ice::ObjectPtr&);

typedef ::IceInternal::Handle< ::TeRK::ServoController> ServoControllerPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::TeRK::ServoController> ServoControllerPrx;

void __write(::IceInternal::BasicStream*, const ServoControllerPrx&);
void __read(::IceInternal::BasicStream*, ServoControllerPrx&);
void __write(::IceInternal::BasicStream*, const ServoControllerPtr&);
void __patch__ServoControllerPtr(void*, ::Ice::ObjectPtr&);

typedef ::IceInternal::Handle< ::TeRK::VideoStreamerClient> VideoStreamerClientPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::TeRK::VideoStreamerClient> VideoStreamerClientPrx;

void __write(::IceInternal::BasicStream*, const VideoStreamerClientPrx&);
void __read(::IceInternal::BasicStream*, VideoStreamerClientPrx&);
void __write(::IceInternal::BasicStream*, const VideoStreamerClientPtr&);
void __patch__VideoStreamerClientPtr(void*, ::Ice::ObjectPtr&);

typedef ::IceInternal::Handle< ::TeRK::VideoStreamerServer> VideoStreamerServerPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::TeRK::VideoStreamerServer> VideoStreamerServerPrx;

void __write(::IceInternal::BasicStream*, const VideoStreamerServerPrx&);
void __read(::IceInternal::BasicStream*, VideoStreamerServerPrx&);
void __write(::IceInternal::BasicStream*, const VideoStreamerServerPtr&);
void __patch__VideoStreamerServerPtr(void*, ::Ice::ObjectPtr&);

typedef ::IceInternal::Handle< ::TeRK::TerkUser> TerkUserPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::TeRK::TerkUser> TerkUserPrx;

void __write(::IceInternal::BasicStream*, const TerkUserPrx&);
void __read(::IceInternal::BasicStream*, TerkUserPrx&);
void __write(::IceInternal::BasicStream*, const TerkUserPtr&);
void __patch__TerkUserPtr(void*, ::Ice::ObjectPtr&);

typedef ::IceInternal::Handle< ::TeRK::Qwerk> QwerkPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::TeRK::Qwerk> QwerkPrx;

void __write(::IceInternal::BasicStream*, const QwerkPrx&);
void __read(::IceInternal::BasicStream*, QwerkPrx&);
void __write(::IceInternal::BasicStream*, const QwerkPtr&);
void __patch__QwerkPtr(void*, ::Ice::ObjectPtr&);

typedef ::IceInternal::Handle< ::TeRK::TerkClient> TerkClientPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::TeRK::TerkClient> TerkClientPrx;

void __write(::IceInternal::BasicStream*, const TerkClientPrx&);
void __read(::IceInternal::BasicStream*, TerkClientPrx&);
void __write(::IceInternal::BasicStream*, const TerkClientPtr&);
void __patch__TerkClientPtr(void*, ::Ice::ObjectPtr&);

}

namespace TeRK
{

enum ImageFormat
{
    ImageJPEG,
    ImageRGB24,
    ImageRGB32,
    ImageGray8,
    ImageYUV420P,
    ImageUnknown
};

void __write(::IceInternal::BasicStream*, ImageFormat);
void __read(::IceInternal::BasicStream*, ImageFormat&);

enum AudioMode
{
    AudioTone,
    AudioClip
};

void __write(::IceInternal::BasicStream*, AudioMode);
void __read(::IceInternal::BasicStream*, AudioMode&);

enum LEDMode
{
    LEDOn,
    LEDOff,
    LEDBlinking
};

void __write(::IceInternal::BasicStream*, LEDMode);
void __read(::IceInternal::BasicStream*, LEDMode&);

enum MotorMode
{
    MotorSpeedControl,
    MotorPositionControl,
    MotorOff
};

void __write(::IceInternal::BasicStream*, MotorMode);
void __read(::IceInternal::BasicStream*, MotorMode&);

enum ServoMode
{
    ServoMotorSpeedControl,
    ServoMotorPositionControl,
    ServoOff
};

void __write(::IceInternal::BasicStream*, ServoMode);
void __read(::IceInternal::BasicStream*, ServoMode&);

typedef ::std::vector<bool> BooleanArray;

class __U__BooleanArray { };

typedef ::std::vector< ::Ice::Byte> ByteArray;

class __U__ByteArray { };

typedef ::std::vector< ::Ice::Int> IntArray;

class __U__IntArray { };

typedef ::std::vector< ::Ice::Short> ShortArray;

class __U__ShortArray { };

typedef ::std::vector< ::std::string> StringArray;

class __U__StringArray { };

typedef ::std::vector< ::TeRK::LEDMode> LEDModeArray;

class __U__LEDModeArray { };
void __write(::IceInternal::BasicStream*, const ::TeRK::LEDMode*, const ::TeRK::LEDMode*, __U__LEDModeArray);
void __read(::IceInternal::BasicStream*, LEDModeArray&, __U__LEDModeArray);

typedef ::std::vector< ::TeRK::MotorMode> MotorModeArray;

class __U__MotorModeArray { };
void __write(::IceInternal::BasicStream*, const ::TeRK::MotorMode*, const ::TeRK::MotorMode*, __U__MotorModeArray);
void __read(::IceInternal::BasicStream*, MotorModeArray&, __U__MotorModeArray);

typedef ::std::vector< ::TeRK::ServoMode> ServoModeArray;

class __U__ServoModeArray { };
void __write(::IceInternal::BasicStream*, const ::TeRK::ServoMode*, const ::TeRK::ServoMode*, __U__ServoModeArray);
void __read(::IceInternal::BasicStream*, ServoModeArray&, __U__ServoModeArray);

typedef ::std::map< ::std::string, ::Ice::Identity> ProxyTypeIdToIdentityMap;

class __U__ProxyTypeIdToIdentityMap { };
void __write(::IceInternal::BasicStream*, const ProxyTypeIdToIdentityMap&, __U__ProxyTypeIdToIdentityMap);
void __read(::IceInternal::BasicStream*, ProxyTypeIdToIdentityMap&, __U__ProxyTypeIdToIdentityMap);

typedef ::std::map< ::std::string, ::std::string> PropertyMap;

class __U__PropertyMap { };
void __write(::IceInternal::BasicStream*, const PropertyMap&, __U__PropertyMap);
void __read(::IceInternal::BasicStream*, PropertyMap&, __U__PropertyMap);

class GenericError : public ::Ice::UserException
{
public:

    GenericError() {}
    explicit GenericError(const ::std::string&);

    virtual const ::std::string ice_name() const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;

    static const ::IceInternal::UserExceptionFactoryPtr& ice_factory();

    ::std::string reason;

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);

    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

static GenericError __GenericError_init;

class TeRKException : public ::Ice::UserException
{
public:

    TeRKException() {}
    explicit TeRKException(const ::std::string&);

    virtual const ::std::string ice_name() const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;

    static const ::IceInternal::UserExceptionFactoryPtr& ice_factory();

    ::std::string reason;

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);

    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

class MotorException : public ::TeRK::TeRKException
{
public:

    MotorException() {}
    explicit MotorException(const ::std::string&);

    virtual const ::std::string ice_name() const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;

    static const ::IceInternal::UserExceptionFactoryPtr& ice_factory();

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);

    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

class VideoException : public ::TeRK::TeRKException
{
public:

    VideoException() {}
    explicit VideoException(const ::std::string&);

    virtual const ::std::string ice_name() const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;

    static const ::IceInternal::UserExceptionFactoryPtr& ice_factory();

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);

    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

class ReadOnlyPropertyException : public ::TeRK::TeRKException
{
public:

    ReadOnlyPropertyException() {}
    explicit ReadOnlyPropertyException(const ::std::string&);

    virtual const ::std::string ice_name() const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;

    static const ::IceInternal::UserExceptionFactoryPtr& ice_factory();

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);

    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

class CommandException : public ::TeRK::TeRKException
{
public:

    CommandException() {}
    explicit CommandException(const ::std::string&);

    virtual const ::std::string ice_name() const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;

    static const ::IceInternal::UserExceptionFactoryPtr& ice_factory();

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);

    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

class AudioCommandException : public ::TeRK::CommandException
{
public:

    AudioCommandException() {}
    explicit AudioCommandException(const ::std::string&);

    virtual const ::std::string ice_name() const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;

    static const ::IceInternal::UserExceptionFactoryPtr& ice_factory();

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);

    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

class AudioCommandQueueFullException : public ::TeRK::AudioCommandException
{
public:

    AudioCommandQueueFullException() {}
    explicit AudioCommandQueueFullException(const ::std::string&);

    virtual const ::std::string ice_name() const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;

    static const ::IceInternal::UserExceptionFactoryPtr& ice_factory();

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);

    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

class AudioFileTooLargeException : public ::TeRK::AudioCommandException
{
public:

    AudioFileTooLargeException() {}
    explicit AudioFileTooLargeException(const ::std::string&);

    virtual const ::std::string ice_name() const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;

    static const ::IceInternal::UserExceptionFactoryPtr& ice_factory();

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);

    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

class DigitalOutCommandException : public ::TeRK::CommandException
{
public:

    DigitalOutCommandException() {}
    explicit DigitalOutCommandException(const ::std::string&);

    virtual const ::std::string ice_name() const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;

    static const ::IceInternal::UserExceptionFactoryPtr& ice_factory();

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);

    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

class LEDCommandException : public ::TeRK::CommandException
{
public:

    LEDCommandException() {}
    explicit LEDCommandException(const ::std::string&);

    virtual const ::std::string ice_name() const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;

    static const ::IceInternal::UserExceptionFactoryPtr& ice_factory();

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);

    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

class MotorCommandException : public ::TeRK::CommandException
{
public:

    MotorCommandException() {}
    explicit MotorCommandException(const ::std::string&);

    virtual const ::std::string ice_name() const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;

    static const ::IceInternal::UserExceptionFactoryPtr& ice_factory();

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);

    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

class ServoCommandException : public ::TeRK::CommandException
{
public:

    ServoCommandException() {}
    explicit ServoCommandException(const ::std::string&);

    virtual const ::std::string ice_name() const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;

    static const ::IceInternal::UserExceptionFactoryPtr& ice_factory();

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);

    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

struct MotorBuffer
{
    ::TeRK::IntArray values;

    bool operator==(const MotorBuffer&) const;
    bool operator!=(const MotorBuffer&) const;
    bool operator<(const MotorBuffer&) const;
    bool operator<=(const MotorBuffer& __rhs) const
    {
	return operator<(__rhs) || operator==(__rhs);
    }
    bool operator>(const MotorBuffer& __rhs) const
    {
	return !operator<(__rhs) && !operator==(__rhs);
    }
    bool operator>=(const MotorBuffer& __rhs) const
    {
	return !operator<(__rhs);
    }

    void __write(::IceInternal::BasicStream*) const;
    void __read(::IceInternal::BasicStream*);
};

typedef ::std::vector< ::TeRK::MotorBuffer> MotorBufferArray;

class __U__MotorBufferArray { };
void __write(::IceInternal::BasicStream*, const ::TeRK::MotorBuffer*, const ::TeRK::MotorBuffer*, __U__MotorBufferArray);
void __read(::IceInternal::BasicStream*, MotorBufferArray&, __U__MotorBufferArray);

struct Image
{
    ::Ice::Int height;
    ::Ice::Int width;
    ::Ice::Int frameNum;
    ::TeRK::ImageFormat format;
    ::TeRK::ByteArray data;

    bool operator==(const Image&) const;
    bool operator!=(const Image&) const;
    bool operator<(const Image&) const;
    bool operator<=(const Image& __rhs) const
    {
	return operator<(__rhs) || operator==(__rhs);
    }
    bool operator>(const Image& __rhs) const
    {
	return !operator<(__rhs) && !operator==(__rhs);
    }
    bool operator>=(const Image& __rhs) const
    {
	return !operator<(__rhs);
    }

    void __write(::IceInternal::BasicStream*) const;
    void __read(::IceInternal::BasicStream*);
};

struct AnalogInState
{
    ::TeRK::ShortArray analogInValues;

    bool operator==(const AnalogInState&) const;
    bool operator!=(const AnalogInState&) const;
    bool operator<(const AnalogInState&) const;
    bool operator<=(const AnalogInState& __rhs) const
    {
	return operator<(__rhs) || operator==(__rhs);
    }
    bool operator>(const AnalogInState& __rhs) const
    {
	return !operator<(__rhs) && !operator==(__rhs);
    }
    bool operator>=(const AnalogInState& __rhs) const
    {
	return !operator<(__rhs);
    }

    void __write(::IceInternal::BasicStream*) const;
    void __read(::IceInternal::BasicStream*);
};

struct BatteryState
{
    ::Ice::Int batteryVoltage;

    bool operator==(const BatteryState&) const;
    bool operator!=(const BatteryState&) const;
    bool operator<(const BatteryState&) const;
    bool operator<=(const BatteryState& __rhs) const
    {
	return operator<(__rhs) || operator==(__rhs);
    }
    bool operator>(const BatteryState& __rhs) const
    {
	return !operator<(__rhs) && !operator==(__rhs);
    }
    bool operator>=(const BatteryState& __rhs) const
    {
	return !operator<(__rhs);
    }

    void __write(::IceInternal::BasicStream*) const;
    void __read(::IceInternal::BasicStream*);
};

struct ButtonState
{
    ::TeRK::BooleanArray buttonStates;

    bool operator==(const ButtonState&) const;
    bool operator!=(const ButtonState&) const;
    bool operator<(const ButtonState&) const;
    bool operator<=(const ButtonState& __rhs) const
    {
	return operator<(__rhs) || operator==(__rhs);
    }
    bool operator>(const ButtonState& __rhs) const
    {
	return !operator<(__rhs) && !operator==(__rhs);
    }
    bool operator>=(const ButtonState& __rhs) const
    {
	return !operator<(__rhs);
    }

    void __write(::IceInternal::BasicStream*) const;
    void __read(::IceInternal::BasicStream*);
};

struct DigitalInState
{
    ::TeRK::BooleanArray digitalInStates;

    bool operator==(const DigitalInState&) const;
    bool operator!=(const DigitalInState&) const;
    bool operator<(const DigitalInState&) const;
    bool operator<=(const DigitalInState& __rhs) const
    {
	return operator<(__rhs) || operator==(__rhs);
    }
    bool operator>(const DigitalInState& __rhs) const
    {
	return !operator<(__rhs) && !operator==(__rhs);
    }
    bool operator>=(const DigitalInState& __rhs) const
    {
	return !operator<(__rhs);
    }

    void __write(::IceInternal::BasicStream*) const;
    void __read(::IceInternal::BasicStream*);
};

struct MotorState
{
    ::TeRK::IntArray motorVelocities;
    ::TeRK::IntArray motorPositions;
    ::TeRK::IntArray motorCurrents;
    ::TeRK::IntArray motorDutyCycles;
    ::TeRK::BooleanArray motorDone;

    bool operator==(const MotorState&) const;
    bool operator!=(const MotorState&) const;
    bool operator<(const MotorState&) const;
    bool operator<=(const MotorState& __rhs) const
    {
	return operator<(__rhs) || operator==(__rhs);
    }
    bool operator>(const MotorState& __rhs) const
    {
	return !operator<(__rhs) && !operator==(__rhs);
    }
    bool operator>=(const MotorState& __rhs) const
    {
	return !operator<(__rhs);
    }

    void __write(::IceInternal::BasicStream*) const;
    void __read(::IceInternal::BasicStream*);
};

struct ServoState
{
    ::TeRK::IntArray servoPositions;

    bool operator==(const ServoState&) const;
    bool operator!=(const ServoState&) const;
    bool operator<(const ServoState&) const;
    bool operator<=(const ServoState& __rhs) const
    {
	return operator<(__rhs) || operator==(__rhs);
    }
    bool operator>(const ServoState& __rhs) const
    {
	return !operator<(__rhs) && !operator==(__rhs);
    }
    bool operator>=(const ServoState& __rhs) const
    {
	return !operator<(__rhs);
    }

    void __write(::IceInternal::BasicStream*) const;
    void __read(::IceInternal::BasicStream*);
};

struct QwerkState
{
    ::TeRK::AnalogInState analogIn;
    ::TeRK::BatteryState battery;
    ::TeRK::ButtonState button;
    ::TeRK::DigitalInState digitalIn;
    ::TeRK::MotorState motor;
    ::TeRK::ServoState servo;

    bool operator==(const QwerkState&) const;
    bool operator!=(const QwerkState&) const;
    bool operator<(const QwerkState&) const;
    bool operator<=(const QwerkState& __rhs) const
    {
	return operator<(__rhs) || operator==(__rhs);
    }
    bool operator>(const QwerkState& __rhs) const
    {
	return !operator<(__rhs) && !operator==(__rhs);
    }
    bool operator>=(const QwerkState& __rhs) const
    {
	return !operator<(__rhs);
    }

    void __write(::IceInternal::BasicStream*) const;
    void __read(::IceInternal::BasicStream*);
};

struct AudioCommand
{
    ::TeRK::AudioMode mode;
    ::Ice::Int frequency;
    ::Ice::Byte amplitude;
    ::Ice::Int duration;
    ::TeRK::ByteArray sound;

    bool operator==(const AudioCommand&) const;
    bool operator!=(const AudioCommand&) const;
    bool operator<(const AudioCommand&) const;
    bool operator<=(const AudioCommand& __rhs) const
    {
	return operator<(__rhs) || operator==(__rhs);
    }
    bool operator>(const AudioCommand& __rhs) const
    {
	return !operator<(__rhs) && !operator==(__rhs);
    }
    bool operator>=(const AudioCommand& __rhs) const
    {
	return !operator<(__rhs);
    }

    void __write(::IceInternal::BasicStream*) const;
    void __read(::IceInternal::BasicStream*);
};

struct DigitalOutCommand
{
    ::TeRK::BooleanArray digitalOutMask;
    ::TeRK::BooleanArray digitalOutValues;

    bool operator==(const DigitalOutCommand&) const;
    bool operator!=(const DigitalOutCommand&) const;
    bool operator<(const DigitalOutCommand&) const;
    bool operator<=(const DigitalOutCommand& __rhs) const
    {
	return operator<(__rhs) || operator==(__rhs);
    }
    bool operator>(const DigitalOutCommand& __rhs) const
    {
	return !operator<(__rhs) && !operator==(__rhs);
    }
    bool operator>=(const DigitalOutCommand& __rhs) const
    {
	return !operator<(__rhs);
    }

    void __write(::IceInternal::BasicStream*) const;
    void __read(::IceInternal::BasicStream*);
};

struct LEDCommand
{
    ::TeRK::BooleanArray ledMask;
    ::TeRK::LEDModeArray ledModes;

    bool operator==(const LEDCommand&) const;
    bool operator!=(const LEDCommand&) const;
    bool operator<(const LEDCommand&) const;
    bool operator<=(const LEDCommand& __rhs) const
    {
	return operator<(__rhs) || operator==(__rhs);
    }
    bool operator>(const LEDCommand& __rhs) const
    {
	return !operator<(__rhs) && !operator==(__rhs);
    }
    bool operator>=(const LEDCommand& __rhs) const
    {
	return !operator<(__rhs);
    }

    void __write(::IceInternal::BasicStream*) const;
    void __read(::IceInternal::BasicStream*);
};

struct MotorCommand
{
    ::TeRK::BooleanArray motorMask;
    ::TeRK::MotorModeArray motorModes;
    ::TeRK::IntArray motorPositions;
    ::TeRK::IntArray motorVelocities;
    ::TeRK::IntArray motorAccelerations;

    bool operator==(const MotorCommand&) const;
    bool operator!=(const MotorCommand&) const;
    bool operator<(const MotorCommand&) const;
    bool operator<=(const MotorCommand& __rhs) const
    {
	return operator<(__rhs) || operator==(__rhs);
    }
    bool operator>(const MotorCommand& __rhs) const
    {
	return !operator<(__rhs) && !operator==(__rhs);
    }
    bool operator>=(const MotorCommand& __rhs) const
    {
	return !operator<(__rhs);
    }

    void __write(::IceInternal::BasicStream*) const;
    void __read(::IceInternal::BasicStream*);
};

struct ServoCommand
{
    ::TeRK::BooleanArray servoMask;
    ::TeRK::ServoModeArray servoModes;
    ::TeRK::IntArray servoPositions;
    ::TeRK::IntArray servoSpeeds;

    bool operator==(const ServoCommand&) const;
    bool operator!=(const ServoCommand&) const;
    bool operator<(const ServoCommand&) const;
    bool operator<=(const ServoCommand& __rhs) const
    {
	return operator<(__rhs) || operator==(__rhs);
    }
    bool operator>(const ServoCommand& __rhs) const
    {
	return !operator<(__rhs) && !operator==(__rhs);
    }
    bool operator>=(const ServoCommand& __rhs) const
    {
	return !operator<(__rhs);
    }

    void __write(::IceInternal::BasicStream*) const;
    void __read(::IceInternal::BasicStream*);
};

struct QwerkCommand
{
    ::TeRK::AudioCommand audioCmd;
    ::TeRK::DigitalOutCommand digitalOutCmd;
    ::TeRK::LEDCommand ledCmd;
    ::TeRK::MotorCommand motorCmd;
    ::TeRK::ServoCommand servoCmd;

    bool operator==(const QwerkCommand&) const;
    bool operator!=(const QwerkCommand&) const;
    bool operator<(const QwerkCommand&) const;
    bool operator<=(const QwerkCommand& __rhs) const
    {
	return operator<(__rhs) || operator==(__rhs);
    }
    bool operator>(const QwerkCommand& __rhs) const
    {
	return !operator<(__rhs) && !operator==(__rhs);
    }
    bool operator>=(const QwerkCommand& __rhs) const
    {
	return !operator<(__rhs);
    }

    void __write(::IceInternal::BasicStream*) const;
    void __read(::IceInternal::BasicStream*);
};

typedef ::std::vector< ::TeRK::AbstractCommandControllerPrx> AbstractCommandControllerProxyArray;

class __U__AbstractCommandControllerProxyArray { };
void __write(::IceInternal::BasicStream*, const ::TeRK::AbstractCommandControllerPrx*, const ::TeRK::AbstractCommandControllerPrx*, __U__AbstractCommandControllerProxyArray);
void __read(::IceInternal::BasicStream*, AbstractCommandControllerProxyArray&, __U__AbstractCommandControllerProxyArray);

}

namespace TeRK
{

class AMI_PropertyManager_getProperty : public ::IceInternal::OutgoingAsync
{
public:

    virtual void ice_response(const ::std::string&) = 0;
    virtual void ice_exception(const ::Ice::Exception&) = 0;

    void __invoke(const ::TeRK::PropertyManagerPrx&, const ::std::string&, const ::Ice::Context&);

protected:

    virtual void __response(bool);
};

typedef ::IceUtil::Handle< ::TeRK::AMI_PropertyManager_getProperty> AMI_PropertyManager_getPropertyPtr;

class AMI_PropertyManager_getProperties : public ::IceInternal::OutgoingAsync
{
public:

    virtual void ice_response(const ::TeRK::PropertyMap&) = 0;
    virtual void ice_exception(const ::Ice::Exception&) = 0;

    void __invoke(const ::TeRK::PropertyManagerPrx&, const ::Ice::Context&);

protected:

    virtual void __response(bool);
};

typedef ::IceUtil::Handle< ::TeRK::AMI_PropertyManager_getProperties> AMI_PropertyManager_getPropertiesPtr;

class AMI_PropertyManager_getPropertyKeys : public ::IceInternal::OutgoingAsync
{
public:

    virtual void ice_response(const ::TeRK::StringArray&) = 0;
    virtual void ice_exception(const ::Ice::Exception&) = 0;

    void __invoke(const ::TeRK::PropertyManagerPrx&, const ::Ice::Context&);

protected:

    virtual void __response(bool);
};

typedef ::IceUtil::Handle< ::TeRK::AMI_PropertyManager_getPropertyKeys> AMI_PropertyManager_getPropertyKeysPtr;

class AMI_PropertyManager_setProperty : public ::IceInternal::OutgoingAsync
{
public:

    virtual void ice_response() = 0;
    virtual void ice_exception(const ::Ice::Exception&) = 0;

    void __invoke(const ::TeRK::PropertyManagerPrx&, const ::std::string&, const ::std::string&, const ::Ice::Context&);

protected:

    virtual void __response(bool);
};

typedef ::IceUtil::Handle< ::TeRK::AMI_PropertyManager_setProperty> AMI_PropertyManager_setPropertyPtr;

class AMI_AnalogInController_getState : public ::IceInternal::OutgoingAsync
{
public:

    virtual void ice_response(const ::TeRK::AnalogInState&) = 0;
    virtual void ice_exception(const ::Ice::Exception&) = 0;

    void __invoke(const ::TeRK::AnalogInControllerPrx&, const ::Ice::Context&);

protected:

    virtual void __response(bool);
};

typedef ::IceUtil::Handle< ::TeRK::AMI_AnalogInController_getState> AMI_AnalogInController_getStatePtr;

class AMI_AudioController_execute : public ::IceInternal::OutgoingAsync
{
public:

    virtual void ice_response() = 0;
    virtual void ice_exception(const ::Ice::Exception&) = 0;

    void __invoke(const ::TeRK::AudioControllerPrx&, const ::TeRK::AudioCommand&, const ::Ice::Context&);

protected:

    virtual void __response(bool);
};

typedef ::IceUtil::Handle< ::TeRK::AMI_AudioController_execute> AMI_AudioController_executePtr;

class AMI_DigitalInController_getState : public ::IceInternal::OutgoingAsync
{
public:

    virtual void ice_response(const ::TeRK::DigitalInState&) = 0;
    virtual void ice_exception(const ::Ice::Exception&) = 0;

    void __invoke(const ::TeRK::DigitalInControllerPrx&, const ::Ice::Context&);

protected:

    virtual void __response(bool);
};

typedef ::IceUtil::Handle< ::TeRK::AMI_DigitalInController_getState> AMI_DigitalInController_getStatePtr;

class AMI_DigitalOutController_execute : public ::IceInternal::OutgoingAsync
{
public:

    virtual void ice_response() = 0;
    virtual void ice_exception(const ::Ice::Exception&) = 0;

    void __invoke(const ::TeRK::DigitalOutControllerPrx&, const ::TeRK::DigitalOutCommand&, const ::Ice::Context&);

protected:

    virtual void __response(bool);
};

typedef ::IceUtil::Handle< ::TeRK::AMI_DigitalOutController_execute> AMI_DigitalOutController_executePtr;

class AMI_LEDController_execute : public ::IceInternal::OutgoingAsync
{
public:

    virtual void ice_response() = 0;
    virtual void ice_exception(const ::Ice::Exception&) = 0;

    void __invoke(const ::TeRK::LEDControllerPrx&, const ::TeRK::LEDCommand&, const ::Ice::Context&);

protected:

    virtual void __response(bool);
};

typedef ::IceUtil::Handle< ::TeRK::AMI_LEDController_execute> AMI_LEDController_executePtr;

class AMI_MotorController_execute : public ::IceInternal::OutgoingAsync
{
public:

    virtual void ice_response(const ::TeRK::MotorState&) = 0;
    virtual void ice_exception(const ::Ice::Exception&) = 0;

    void __invoke(const ::TeRK::MotorControllerPrx&, const ::TeRK::MotorCommand&, const ::Ice::Context&);

protected:

    virtual void __response(bool);
};

typedef ::IceUtil::Handle< ::TeRK::AMI_MotorController_execute> AMI_MotorController_executePtr;

class AMI_MotorController_getFrequency : public ::IceInternal::OutgoingAsync
{
public:

    virtual void ice_response(::Ice::Int) = 0;
    virtual void ice_exception(const ::Ice::Exception&) = 0;

    void __invoke(const ::TeRK::MotorControllerPrx&, const ::Ice::Context&);

protected:

    virtual void __response(bool);
};

typedef ::IceUtil::Handle< ::TeRK::AMI_MotorController_getFrequency> AMI_MotorController_getFrequencyPtr;

class AMI_MotorController_startMotorBufferRecord : public ::IceInternal::OutgoingAsync
{
public:

    virtual void ice_response() = 0;
    virtual void ice_exception(const ::Ice::Exception&) = 0;

    void __invoke(const ::TeRK::MotorControllerPrx&, const ::TeRK::BooleanArray&, const ::Ice::Context&);

protected:

    virtual void __response(bool);
};

typedef ::IceUtil::Handle< ::TeRK::AMI_MotorController_startMotorBufferRecord> AMI_MotorController_startMotorBufferRecordPtr;

class AMI_MotorController_stopMotorBufferRecord : public ::IceInternal::OutgoingAsync
{
public:

    virtual void ice_response() = 0;
    virtual void ice_exception(const ::Ice::Exception&) = 0;

    void __invoke(const ::TeRK::MotorControllerPrx&, const ::TeRK::BooleanArray&, const ::Ice::Context&);

protected:

    virtual void __response(bool);
};

typedef ::IceUtil::Handle< ::TeRK::AMI_MotorController_stopMotorBufferRecord> AMI_MotorController_stopMotorBufferRecordPtr;

class AMI_MotorController_getMotorBuffers : public ::IceInternal::OutgoingAsync
{
public:

    virtual void ice_response(const ::TeRK::MotorBufferArray&) = 0;
    virtual void ice_exception(const ::Ice::Exception&) = 0;

    void __invoke(const ::TeRK::MotorControllerPrx&, const ::TeRK::BooleanArray&, const ::Ice::Context&);

protected:

    virtual void __response(bool);
};

typedef ::IceUtil::Handle< ::TeRK::AMI_MotorController_getMotorBuffers> AMI_MotorController_getMotorBuffersPtr;

class AMI_MotorController_setMotorBuffer : public ::IceInternal::OutgoingAsync
{
public:

    virtual void ice_response() = 0;
    virtual void ice_exception(const ::Ice::Exception&) = 0;

    void __invoke(const ::TeRK::MotorControllerPrx&, const ::TeRK::BooleanArray&, const ::TeRK::MotorBufferArray&, const ::Ice::Context&);

protected:

    virtual void __response(bool);
};

typedef ::IceUtil::Handle< ::TeRK::AMI_MotorController_setMotorBuffer> AMI_MotorController_setMotorBufferPtr;

class AMI_MotorController_playMotorBuffer : public ::IceInternal::OutgoingAsync
{
public:

    virtual void ice_response() = 0;
    virtual void ice_exception(const ::Ice::Exception&) = 0;

    void __invoke(const ::TeRK::MotorControllerPrx&, const ::TeRK::BooleanArray&, const ::Ice::Context&);

protected:

    virtual void __response(bool);
};

typedef ::IceUtil::Handle< ::TeRK::AMI_MotorController_playMotorBuffer> AMI_MotorController_playMotorBufferPtr;

class AMI_ServoController_execute : public ::IceInternal::OutgoingAsync
{
public:

    virtual void ice_response(const ::TeRK::ServoState&) = 0;
    virtual void ice_exception(const ::Ice::Exception&) = 0;

    void __invoke(const ::TeRK::ServoControllerPrx&, const ::TeRK::ServoCommand&, const ::Ice::Context&);

protected:

    virtual void __response(bool);
};

typedef ::IceUtil::Handle< ::TeRK::AMI_ServoController_execute> AMI_ServoController_executePtr;

class AMI_VideoStreamerServer_startVideoStream : public ::IceInternal::OutgoingAsync
{
public:

    virtual void ice_response() = 0;
    virtual void ice_exception(const ::Ice::Exception&) = 0;

    void __invoke(const ::TeRK::VideoStreamerServerPrx&, const ::Ice::Context&);

protected:

    virtual void __response(bool);
};

typedef ::IceUtil::Handle< ::TeRK::AMI_VideoStreamerServer_startVideoStream> AMI_VideoStreamerServer_startVideoStreamPtr;

class AMI_VideoStreamerServer_stopVideoStream : public ::IceInternal::OutgoingAsync
{
public:

    virtual void ice_response() = 0;
    virtual void ice_exception(const ::Ice::Exception&) = 0;

    void __invoke(const ::TeRK::VideoStreamerServerPrx&, const ::Ice::Context&);

protected:

    virtual void __response(bool);
};

typedef ::IceUtil::Handle< ::TeRK::AMI_VideoStreamerServer_stopVideoStream> AMI_VideoStreamerServer_stopVideoStreamPtr;

class AMI_VideoStreamerServer_startCamera : public ::IceInternal::OutgoingAsync
{
public:

    virtual void ice_response(::Ice::Int) = 0;
    virtual void ice_exception(const ::Ice::Exception&) = 0;

    void __invoke(const ::TeRK::VideoStreamerServerPrx&, const ::Ice::Context&);

protected:

    virtual void __response(bool);
};

typedef ::IceUtil::Handle< ::TeRK::AMI_VideoStreamerServer_startCamera> AMI_VideoStreamerServer_startCameraPtr;

class AMI_VideoStreamerServer_stopCamera : public ::IceInternal::OutgoingAsync
{
public:

    virtual void ice_response(::Ice::Int) = 0;
    virtual void ice_exception(const ::Ice::Exception&) = 0;

    void __invoke(const ::TeRK::VideoStreamerServerPrx&, const ::Ice::Context&);

protected:

    virtual void __response(bool);
};

typedef ::IceUtil::Handle< ::TeRK::AMI_VideoStreamerServer_stopCamera> AMI_VideoStreamerServer_stopCameraPtr;

class AMI_VideoStreamerServer_getFrame : public ::IceInternal::OutgoingAsync
{
public:

    virtual void ice_response(const ::TeRK::Image&) = 0;
    virtual void ice_exception(const ::Ice::Exception&) = 0;

    void __invoke(const ::TeRK::VideoStreamerServerPrx&, ::Ice::Int, const ::Ice::Context&);

protected:

    virtual void __response(bool);
};

typedef ::IceUtil::Handle< ::TeRK::AMI_VideoStreamerServer_getFrame> AMI_VideoStreamerServer_getFramePtr;

class AMI_TerkUser_getSupportedServices : public ::IceInternal::OutgoingAsync
{
public:

    virtual void ice_response(const ::TeRK::ProxyTypeIdToIdentityMap&) = 0;
    virtual void ice_exception(const ::Ice::Exception&) = 0;

    void __invoke(const ::TeRK::TerkUserPrx&, const ::Ice::Context&);

protected:

    virtual void __response(bool);
};

typedef ::IceUtil::Handle< ::TeRK::AMI_TerkUser_getSupportedServices> AMI_TerkUser_getSupportedServicesPtr;

class AMI_Qwerk_getState : public ::IceInternal::OutgoingAsync
{
public:

    virtual void ice_response(const ::TeRK::QwerkState&) = 0;
    virtual void ice_exception(const ::Ice::Exception&) = 0;

    void __invoke(const ::TeRK::QwerkPrx&, const ::Ice::Context&);

protected:

    virtual void __response(bool);
};

typedef ::IceUtil::Handle< ::TeRK::AMI_Qwerk_getState> AMI_Qwerk_getStatePtr;

class AMI_Qwerk_execute : public ::IceInternal::OutgoingAsync
{
public:

    virtual void ice_response(const ::TeRK::QwerkState&) = 0;
    virtual void ice_exception(const ::Ice::Exception&) = 0;

    void __invoke(const ::TeRK::QwerkPrx&, const ::TeRK::QwerkCommand&, const ::Ice::Context&);

protected:

    virtual void __response(bool);
};

typedef ::IceUtil::Handle< ::TeRK::AMI_Qwerk_execute> AMI_Qwerk_executePtr;

class AMI_Qwerk_emergencyStop : public ::IceInternal::OutgoingAsync
{
public:

    virtual void ice_response(const ::TeRK::QwerkState&) = 0;
    virtual void ice_exception(const ::Ice::Exception&) = 0;

    void __invoke(const ::TeRK::QwerkPrx&, const ::Ice::Context&);

protected:

    virtual void __response(bool);
};

typedef ::IceUtil::Handle< ::TeRK::AMI_Qwerk_emergencyStop> AMI_Qwerk_emergencyStopPtr;

class AMI_Qwerk_getCommandControllerTypeToProxyIdentityMap : public ::IceInternal::OutgoingAsync
{
public:

    virtual void ice_response(const ::TeRK::ProxyTypeIdToIdentityMap&) = 0;
    virtual void ice_exception(const ::Ice::Exception&) = 0;

    void __invoke(const ::TeRK::QwerkPrx&, const ::Ice::Context&);

protected:

    virtual void __response(bool);
};

typedef ::IceUtil::Handle< ::TeRK::AMI_Qwerk_getCommandControllerTypeToProxyIdentityMap> AMI_Qwerk_getCommandControllerTypeToProxyIdentityMapPtr;

}

namespace IceProxy
{

namespace TeRK
{

class PropertyManager : virtual public ::IceProxy::Ice::Object
{
public:

    ::std::string getProperty(const ::std::string& key)
    {
	return getProperty(key, __defaultContext());
    }
    ::std::string getProperty(const ::std::string&, const ::Ice::Context&);
    void getProperty_async(const ::TeRK::AMI_PropertyManager_getPropertyPtr&, const ::std::string&);
    void getProperty_async(const ::TeRK::AMI_PropertyManager_getPropertyPtr&, const ::std::string&, const ::Ice::Context&);

    ::TeRK::PropertyMap getProperties()
    {
	return getProperties(__defaultContext());
    }
    ::TeRK::PropertyMap getProperties(const ::Ice::Context&);
    void getProperties_async(const ::TeRK::AMI_PropertyManager_getPropertiesPtr&);
    void getProperties_async(const ::TeRK::AMI_PropertyManager_getPropertiesPtr&, const ::Ice::Context&);

    ::TeRK::StringArray getPropertyKeys()
    {
	return getPropertyKeys(__defaultContext());
    }
    ::TeRK::StringArray getPropertyKeys(const ::Ice::Context&);
    void getPropertyKeys_async(const ::TeRK::AMI_PropertyManager_getPropertyKeysPtr&);
    void getPropertyKeys_async(const ::TeRK::AMI_PropertyManager_getPropertyKeysPtr&, const ::Ice::Context&);

    void setProperty(const ::std::string& key, const ::std::string& value)
    {
	setProperty(key, value, __defaultContext());
    }
    void setProperty(const ::std::string&, const ::std::string&, const ::Ice::Context&);
    void setProperty_async(const ::TeRK::AMI_PropertyManager_setPropertyPtr&, const ::std::string&, const ::std::string&);
    void setProperty_async(const ::TeRK::AMI_PropertyManager_setPropertyPtr&, const ::std::string&, const ::std::string&, const ::Ice::Context&);
    
    static const ::std::string& ice_staticId();

private: 

    virtual ::IceInternal::Handle< ::IceDelegateM::Ice::Object> __createDelegateM();
    virtual ::IceInternal::Handle< ::IceDelegateD::Ice::Object> __createDelegateD();
};

class AbstractCommandController : virtual public ::IceProxy::TeRK::PropertyManager
{
public:
    
    static const ::std::string& ice_staticId();

private: 

    virtual ::IceInternal::Handle< ::IceDelegateM::Ice::Object> __createDelegateM();
    virtual ::IceInternal::Handle< ::IceDelegateD::Ice::Object> __createDelegateD();
};

class AnalogInController : virtual public ::IceProxy::TeRK::AbstractCommandController
{
public:

    ::TeRK::AnalogInState getState()
    {
	return getState(__defaultContext());
    }
    ::TeRK::AnalogInState getState(const ::Ice::Context&);
    void getState_async(const ::TeRK::AMI_AnalogInController_getStatePtr&);
    void getState_async(const ::TeRK::AMI_AnalogInController_getStatePtr&, const ::Ice::Context&);
    
    static const ::std::string& ice_staticId();

private: 

    virtual ::IceInternal::Handle< ::IceDelegateM::Ice::Object> __createDelegateM();
    virtual ::IceInternal::Handle< ::IceDelegateD::Ice::Object> __createDelegateD();
};

class AudioController : virtual public ::IceProxy::TeRK::AbstractCommandController
{
public:

    void execute(const ::TeRK::AudioCommand& command)
    {
	execute(command, __defaultContext());
    }
    void execute(const ::TeRK::AudioCommand&, const ::Ice::Context&);
    void execute_async(const ::TeRK::AMI_AudioController_executePtr&, const ::TeRK::AudioCommand&);
    void execute_async(const ::TeRK::AMI_AudioController_executePtr&, const ::TeRK::AudioCommand&, const ::Ice::Context&);
    
    static const ::std::string& ice_staticId();

private: 

    virtual ::IceInternal::Handle< ::IceDelegateM::Ice::Object> __createDelegateM();
    virtual ::IceInternal::Handle< ::IceDelegateD::Ice::Object> __createDelegateD();
};

class DigitalInController : virtual public ::IceProxy::TeRK::AbstractCommandController
{
public:

    ::TeRK::DigitalInState getState()
    {
	return getState(__defaultContext());
    }
    ::TeRK::DigitalInState getState(const ::Ice::Context&);
    void getState_async(const ::TeRK::AMI_DigitalInController_getStatePtr&);
    void getState_async(const ::TeRK::AMI_DigitalInController_getStatePtr&, const ::Ice::Context&);
    
    static const ::std::string& ice_staticId();

private: 

    virtual ::IceInternal::Handle< ::IceDelegateM::Ice::Object> __createDelegateM();
    virtual ::IceInternal::Handle< ::IceDelegateD::Ice::Object> __createDelegateD();
};

class DigitalOutController : virtual public ::IceProxy::TeRK::AbstractCommandController
{
public:

    void execute(const ::TeRK::DigitalOutCommand& command)
    {
	execute(command, __defaultContext());
    }
    void execute(const ::TeRK::DigitalOutCommand&, const ::Ice::Context&);
    void execute_async(const ::TeRK::AMI_DigitalOutController_executePtr&, const ::TeRK::DigitalOutCommand&);
    void execute_async(const ::TeRK::AMI_DigitalOutController_executePtr&, const ::TeRK::DigitalOutCommand&, const ::Ice::Context&);
    
    static const ::std::string& ice_staticId();

private: 

    virtual ::IceInternal::Handle< ::IceDelegateM::Ice::Object> __createDelegateM();
    virtual ::IceInternal::Handle< ::IceDelegateD::Ice::Object> __createDelegateD();
};

class LEDController : virtual public ::IceProxy::TeRK::AbstractCommandController
{
public:

    void execute(const ::TeRK::LEDCommand& command)
    {
	execute(command, __defaultContext());
    }
    void execute(const ::TeRK::LEDCommand&, const ::Ice::Context&);
    void execute_async(const ::TeRK::AMI_LEDController_executePtr&, const ::TeRK::LEDCommand&);
    void execute_async(const ::TeRK::AMI_LEDController_executePtr&, const ::TeRK::LEDCommand&, const ::Ice::Context&);
    
    static const ::std::string& ice_staticId();

private: 

    virtual ::IceInternal::Handle< ::IceDelegateM::Ice::Object> __createDelegateM();
    virtual ::IceInternal::Handle< ::IceDelegateD::Ice::Object> __createDelegateD();
};

class MotorController : virtual public ::IceProxy::TeRK::AbstractCommandController
{
public:

    ::TeRK::MotorState execute(const ::TeRK::MotorCommand& command)
    {
	return execute(command, __defaultContext());
    }
    ::TeRK::MotorState execute(const ::TeRK::MotorCommand&, const ::Ice::Context&);
    void execute_async(const ::TeRK::AMI_MotorController_executePtr&, const ::TeRK::MotorCommand&);
    void execute_async(const ::TeRK::AMI_MotorController_executePtr&, const ::TeRK::MotorCommand&, const ::Ice::Context&);

    ::Ice::Int getFrequency()
    {
	return getFrequency(__defaultContext());
    }
    ::Ice::Int getFrequency(const ::Ice::Context&);
    void getFrequency_async(const ::TeRK::AMI_MotorController_getFrequencyPtr&);
    void getFrequency_async(const ::TeRK::AMI_MotorController_getFrequencyPtr&, const ::Ice::Context&);

    void startMotorBufferRecord(const ::TeRK::BooleanArray& motorMask)
    {
	startMotorBufferRecord(motorMask, __defaultContext());
    }
    void startMotorBufferRecord(const ::TeRK::BooleanArray&, const ::Ice::Context&);
    void startMotorBufferRecord_async(const ::TeRK::AMI_MotorController_startMotorBufferRecordPtr&, const ::TeRK::BooleanArray&);
    void startMotorBufferRecord_async(const ::TeRK::AMI_MotorController_startMotorBufferRecordPtr&, const ::TeRK::BooleanArray&, const ::Ice::Context&);

    void stopMotorBufferRecord(const ::TeRK::BooleanArray& motorMask)
    {
	stopMotorBufferRecord(motorMask, __defaultContext());
    }
    void stopMotorBufferRecord(const ::TeRK::BooleanArray&, const ::Ice::Context&);
    void stopMotorBufferRecord_async(const ::TeRK::AMI_MotorController_stopMotorBufferRecordPtr&, const ::TeRK::BooleanArray&);
    void stopMotorBufferRecord_async(const ::TeRK::AMI_MotorController_stopMotorBufferRecordPtr&, const ::TeRK::BooleanArray&, const ::Ice::Context&);

    ::TeRK::MotorBufferArray getMotorBuffers(const ::TeRK::BooleanArray& motorMask)
    {
	return getMotorBuffers(motorMask, __defaultContext());
    }
    ::TeRK::MotorBufferArray getMotorBuffers(const ::TeRK::BooleanArray&, const ::Ice::Context&);
    void getMotorBuffers_async(const ::TeRK::AMI_MotorController_getMotorBuffersPtr&, const ::TeRK::BooleanArray&);
    void getMotorBuffers_async(const ::TeRK::AMI_MotorController_getMotorBuffersPtr&, const ::TeRK::BooleanArray&, const ::Ice::Context&);

    void setMotorBuffer(const ::TeRK::BooleanArray& motorMask, const ::TeRK::MotorBufferArray& motorBuffers)
    {
	setMotorBuffer(motorMask, motorBuffers, __defaultContext());
    }
    void setMotorBuffer(const ::TeRK::BooleanArray&, const ::TeRK::MotorBufferArray&, const ::Ice::Context&);
    void setMotorBuffer_async(const ::TeRK::AMI_MotorController_setMotorBufferPtr&, const ::TeRK::BooleanArray&, const ::TeRK::MotorBufferArray&);
    void setMotorBuffer_async(const ::TeRK::AMI_MotorController_setMotorBufferPtr&, const ::TeRK::BooleanArray&, const ::TeRK::MotorBufferArray&, const ::Ice::Context&);

    void playMotorBuffer(const ::TeRK::BooleanArray& motorMask)
    {
	playMotorBuffer(motorMask, __defaultContext());
    }
    void playMotorBuffer(const ::TeRK::BooleanArray&, const ::Ice::Context&);
    void playMotorBuffer_async(const ::TeRK::AMI_MotorController_playMotorBufferPtr&, const ::TeRK::BooleanArray&);
    void playMotorBuffer_async(const ::TeRK::AMI_MotorController_playMotorBufferPtr&, const ::TeRK::BooleanArray&, const ::Ice::Context&);

    void driveForward()
    {
	driveForward(__defaultContext());
    }
    void driveForward(const ::Ice::Context&);

    void driveBack()
    {
	driveBack(__defaultContext());
    }
    void driveBack(const ::Ice::Context&);

    void spinLeft()
    {
	spinLeft(__defaultContext());
    }
    void spinLeft(const ::Ice::Context&);

    void spinRight()
    {
	spinRight(__defaultContext());
    }
    void spinRight(const ::Ice::Context&);

    void setMotorVelocities(const ::TeRK::IntArray& motorValues)
    {
	setMotorVelocities(motorValues, __defaultContext());
    }
    void setMotorVelocities(const ::TeRK::IntArray&, const ::Ice::Context&);

    void stopMotors()
    {
	stopMotors(__defaultContext());
    }
    void stopMotors(const ::Ice::Context&);
    
    static const ::std::string& ice_staticId();

private: 

    virtual ::IceInternal::Handle< ::IceDelegateM::Ice::Object> __createDelegateM();
    virtual ::IceInternal::Handle< ::IceDelegateD::Ice::Object> __createDelegateD();
};

class ServoController : virtual public ::IceProxy::TeRK::AbstractCommandController
{
public:

    ::TeRK::ServoState execute(const ::TeRK::ServoCommand& command)
    {
	return execute(command, __defaultContext());
    }
    ::TeRK::ServoState execute(const ::TeRK::ServoCommand&, const ::Ice::Context&);
    void execute_async(const ::TeRK::AMI_ServoController_executePtr&, const ::TeRK::ServoCommand&);
    void execute_async(const ::TeRK::AMI_ServoController_executePtr&, const ::TeRK::ServoCommand&, const ::Ice::Context&);

    void cameraTiltUp()
    {
	cameraTiltUp(__defaultContext());
    }
    void cameraTiltUp(const ::Ice::Context&);

    void cameraTiltDown()
    {
	cameraTiltDown(__defaultContext());
    }
    void cameraTiltDown(const ::Ice::Context&);

    void cameraPanLeft()
    {
	cameraPanLeft(__defaultContext());
    }
    void cameraPanLeft(const ::Ice::Context&);

    void cameraPanRight()
    {
	cameraPanRight(__defaultContext());
    }
    void cameraPanRight(const ::Ice::Context&);

    void setServoPositions(const ::TeRK::IntArray& servoPositions)
    {
	setServoPositions(servoPositions, __defaultContext());
    }
    void setServoPositions(const ::TeRK::IntArray&, const ::Ice::Context&);

    void setServoVelocities(const ::TeRK::IntArray& servoVelocities)
    {
	setServoVelocities(servoVelocities, __defaultContext());
    }
    void setServoVelocities(const ::TeRK::IntArray&, const ::Ice::Context&);

    void stopServos()
    {
	stopServos(__defaultContext());
    }
    void stopServos(const ::Ice::Context&);
    
    static const ::std::string& ice_staticId();

private: 

    virtual ::IceInternal::Handle< ::IceDelegateM::Ice::Object> __createDelegateM();
    virtual ::IceInternal::Handle< ::IceDelegateD::Ice::Object> __createDelegateD();
};

class VideoStreamerClient : virtual public ::IceProxy::Ice::Object
{
public:

    void newFrame(const ::TeRK::Image& frame)
    {
	newFrame(frame, __defaultContext());
    }
    void newFrame(const ::TeRK::Image&, const ::Ice::Context&);
    
    static const ::std::string& ice_staticId();

private: 

    virtual ::IceInternal::Handle< ::IceDelegateM::Ice::Object> __createDelegateM();
    virtual ::IceInternal::Handle< ::IceDelegateD::Ice::Object> __createDelegateD();
};

class VideoStreamerServer : virtual public ::IceProxy::TeRK::AbstractCommandController
{
public:

    void startVideoStream()
    {
	startVideoStream(__defaultContext());
    }
    void startVideoStream(const ::Ice::Context&);
    void startVideoStream_async(const ::TeRK::AMI_VideoStreamerServer_startVideoStreamPtr&);
    void startVideoStream_async(const ::TeRK::AMI_VideoStreamerServer_startVideoStreamPtr&, const ::Ice::Context&);

    void stopVideoStream()
    {
	stopVideoStream(__defaultContext());
    }
    void stopVideoStream(const ::Ice::Context&);
    void stopVideoStream_async(const ::TeRK::AMI_VideoStreamerServer_stopVideoStreamPtr&);
    void stopVideoStream_async(const ::TeRK::AMI_VideoStreamerServer_stopVideoStreamPtr&, const ::Ice::Context&);

    ::Ice::Int startCamera()
    {
	return startCamera(__defaultContext());
    }
    ::Ice::Int startCamera(const ::Ice::Context&);
    void startCamera_async(const ::TeRK::AMI_VideoStreamerServer_startCameraPtr&);
    void startCamera_async(const ::TeRK::AMI_VideoStreamerServer_startCameraPtr&, const ::Ice::Context&);

    ::Ice::Int stopCamera()
    {
	return stopCamera(__defaultContext());
    }
    ::Ice::Int stopCamera(const ::Ice::Context&);
    void stopCamera_async(const ::TeRK::AMI_VideoStreamerServer_stopCameraPtr&);
    void stopCamera_async(const ::TeRK::AMI_VideoStreamerServer_stopCameraPtr&, const ::Ice::Context&);

    ::TeRK::Image getFrame(::Ice::Int frameNumber)
    {
	return getFrame(frameNumber, __defaultContext());
    }
    ::TeRK::Image getFrame(::Ice::Int, const ::Ice::Context&);
    void getFrame_async(const ::TeRK::AMI_VideoStreamerServer_getFramePtr&, ::Ice::Int);
    void getFrame_async(const ::TeRK::AMI_VideoStreamerServer_getFramePtr&, ::Ice::Int, const ::Ice::Context&);
    
    static const ::std::string& ice_staticId();

private: 

    virtual ::IceInternal::Handle< ::IceDelegateM::Ice::Object> __createDelegateM();
    virtual ::IceInternal::Handle< ::IceDelegateD::Ice::Object> __createDelegateD();
};

class TerkUser : virtual public ::IceProxy::peer::ConnectionEventHandler,
		 virtual public ::IceProxy::TeRK::PropertyManager
{
public:

    ::TeRK::ProxyTypeIdToIdentityMap getSupportedServices()
    {
	return getSupportedServices(__defaultContext());
    }
    ::TeRK::ProxyTypeIdToIdentityMap getSupportedServices(const ::Ice::Context&);
    void getSupportedServices_async(const ::TeRK::AMI_TerkUser_getSupportedServicesPtr&);
    void getSupportedServices_async(const ::TeRK::AMI_TerkUser_getSupportedServicesPtr&, const ::Ice::Context&);
    
    static const ::std::string& ice_staticId();

private: 

    virtual ::IceInternal::Handle< ::IceDelegateM::Ice::Object> __createDelegateM();
    virtual ::IceInternal::Handle< ::IceDelegateD::Ice::Object> __createDelegateD();
};

class Qwerk : virtual public ::IceProxy::TeRK::TerkUser
{
public:

    ::TeRK::QwerkState getState()
    {
	return getState(__defaultContext());
    }
    ::TeRK::QwerkState getState(const ::Ice::Context&);
    void getState_async(const ::TeRK::AMI_Qwerk_getStatePtr&);
    void getState_async(const ::TeRK::AMI_Qwerk_getStatePtr&, const ::Ice::Context&);

    ::TeRK::QwerkState execute(const ::TeRK::QwerkCommand& command)
    {
	return execute(command, __defaultContext());
    }
    ::TeRK::QwerkState execute(const ::TeRK::QwerkCommand&, const ::Ice::Context&);
    void execute_async(const ::TeRK::AMI_Qwerk_executePtr&, const ::TeRK::QwerkCommand&);
    void execute_async(const ::TeRK::AMI_Qwerk_executePtr&, const ::TeRK::QwerkCommand&, const ::Ice::Context&);

    ::TeRK::QwerkState emergencyStop()
    {
	return emergencyStop(__defaultContext());
    }
    ::TeRK::QwerkState emergencyStop(const ::Ice::Context&);
    void emergencyStop_async(const ::TeRK::AMI_Qwerk_emergencyStopPtr&);
    void emergencyStop_async(const ::TeRK::AMI_Qwerk_emergencyStopPtr&, const ::Ice::Context&);

    ::TeRK::ProxyTypeIdToIdentityMap getCommandControllerTypeToProxyIdentityMap()
    {
	return getCommandControllerTypeToProxyIdentityMap(__defaultContext());
    }
    ::TeRK::ProxyTypeIdToIdentityMap getCommandControllerTypeToProxyIdentityMap(const ::Ice::Context&);
    void getCommandControllerTypeToProxyIdentityMap_async(const ::TeRK::AMI_Qwerk_getCommandControllerTypeToProxyIdentityMapPtr&);
    void getCommandControllerTypeToProxyIdentityMap_async(const ::TeRK::AMI_Qwerk_getCommandControllerTypeToProxyIdentityMapPtr&, const ::Ice::Context&);
    
    static const ::std::string& ice_staticId();

private: 

    virtual ::IceInternal::Handle< ::IceDelegateM::Ice::Object> __createDelegateM();
    virtual ::IceInternal::Handle< ::IceDelegateD::Ice::Object> __createDelegateD();
};

class TerkClient : virtual public ::IceProxy::TeRK::TerkUser,
		   virtual public ::IceProxy::TeRK::VideoStreamerClient
{
public:
    
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

class PropertyManager : virtual public ::IceDelegate::Ice::Object
{
public:

    virtual ::std::string getProperty(const ::std::string&, const ::Ice::Context&) = 0;

    virtual ::TeRK::PropertyMap getProperties(const ::Ice::Context&) = 0;

    virtual ::TeRK::StringArray getPropertyKeys(const ::Ice::Context&) = 0;

    virtual void setProperty(const ::std::string&, const ::std::string&, const ::Ice::Context&) = 0;
};

class AbstractCommandController : virtual public ::IceDelegate::TeRK::PropertyManager
{
public:
};

class AnalogInController : virtual public ::IceDelegate::TeRK::AbstractCommandController
{
public:

    virtual ::TeRK::AnalogInState getState(const ::Ice::Context&) = 0;
};

class AudioController : virtual public ::IceDelegate::TeRK::AbstractCommandController
{
public:

    virtual void execute(const ::TeRK::AudioCommand&, const ::Ice::Context&) = 0;
};

class DigitalInController : virtual public ::IceDelegate::TeRK::AbstractCommandController
{
public:

    virtual ::TeRK::DigitalInState getState(const ::Ice::Context&) = 0;
};

class DigitalOutController : virtual public ::IceDelegate::TeRK::AbstractCommandController
{
public:

    virtual void execute(const ::TeRK::DigitalOutCommand&, const ::Ice::Context&) = 0;
};

class LEDController : virtual public ::IceDelegate::TeRK::AbstractCommandController
{
public:

    virtual void execute(const ::TeRK::LEDCommand&, const ::Ice::Context&) = 0;
};

class MotorController : virtual public ::IceDelegate::TeRK::AbstractCommandController
{
public:

    virtual ::TeRK::MotorState execute(const ::TeRK::MotorCommand&, const ::Ice::Context&) = 0;

    virtual ::Ice::Int getFrequency(const ::Ice::Context&) = 0;

    virtual void startMotorBufferRecord(const ::TeRK::BooleanArray&, const ::Ice::Context&) = 0;

    virtual void stopMotorBufferRecord(const ::TeRK::BooleanArray&, const ::Ice::Context&) = 0;

    virtual ::TeRK::MotorBufferArray getMotorBuffers(const ::TeRK::BooleanArray&, const ::Ice::Context&) = 0;

    virtual void setMotorBuffer(const ::TeRK::BooleanArray&, const ::TeRK::MotorBufferArray&, const ::Ice::Context&) = 0;

    virtual void playMotorBuffer(const ::TeRK::BooleanArray&, const ::Ice::Context&) = 0;

    virtual void driveForward(const ::Ice::Context&) = 0;

    virtual void driveBack(const ::Ice::Context&) = 0;

    virtual void spinLeft(const ::Ice::Context&) = 0;

    virtual void spinRight(const ::Ice::Context&) = 0;

    virtual void setMotorVelocities(const ::TeRK::IntArray&, const ::Ice::Context&) = 0;

    virtual void stopMotors(const ::Ice::Context&) = 0;
};

class ServoController : virtual public ::IceDelegate::TeRK::AbstractCommandController
{
public:

    virtual ::TeRK::ServoState execute(const ::TeRK::ServoCommand&, const ::Ice::Context&) = 0;

    virtual void cameraTiltUp(const ::Ice::Context&) = 0;

    virtual void cameraTiltDown(const ::Ice::Context&) = 0;

    virtual void cameraPanLeft(const ::Ice::Context&) = 0;

    virtual void cameraPanRight(const ::Ice::Context&) = 0;

    virtual void setServoPositions(const ::TeRK::IntArray&, const ::Ice::Context&) = 0;

    virtual void setServoVelocities(const ::TeRK::IntArray&, const ::Ice::Context&) = 0;

    virtual void stopServos(const ::Ice::Context&) = 0;
};

class VideoStreamerClient : virtual public ::IceDelegate::Ice::Object
{
public:

    virtual void newFrame(const ::TeRK::Image&, const ::Ice::Context&) = 0;
};

class VideoStreamerServer : virtual public ::IceDelegate::TeRK::AbstractCommandController
{
public:

    virtual void startVideoStream(const ::Ice::Context&) = 0;

    virtual void stopVideoStream(const ::Ice::Context&) = 0;

    virtual ::Ice::Int startCamera(const ::Ice::Context&) = 0;

    virtual ::Ice::Int stopCamera(const ::Ice::Context&) = 0;

    virtual ::TeRK::Image getFrame(::Ice::Int, const ::Ice::Context&) = 0;
};

class TerkUser : virtual public ::IceDelegate::peer::ConnectionEventHandler,
		 virtual public ::IceDelegate::TeRK::PropertyManager
{
public:

    virtual ::TeRK::ProxyTypeIdToIdentityMap getSupportedServices(const ::Ice::Context&) = 0;
};

class Qwerk : virtual public ::IceDelegate::TeRK::TerkUser
{
public:

    virtual ::TeRK::QwerkState getState(const ::Ice::Context&) = 0;

    virtual ::TeRK::QwerkState execute(const ::TeRK::QwerkCommand&, const ::Ice::Context&) = 0;

    virtual ::TeRK::QwerkState emergencyStop(const ::Ice::Context&) = 0;

    virtual ::TeRK::ProxyTypeIdToIdentityMap getCommandControllerTypeToProxyIdentityMap(const ::Ice::Context&) = 0;
};

class TerkClient : virtual public ::IceDelegate::TeRK::TerkUser,
		   virtual public ::IceDelegate::TeRK::VideoStreamerClient
{
public:
};

}

}

namespace IceDelegateM
{

namespace TeRK
{

class PropertyManager : virtual public ::IceDelegate::TeRK::PropertyManager,
			virtual public ::IceDelegateM::Ice::Object
{
public:

    virtual ::std::string getProperty(const ::std::string&, const ::Ice::Context&);

    virtual ::TeRK::PropertyMap getProperties(const ::Ice::Context&);

    virtual ::TeRK::StringArray getPropertyKeys(const ::Ice::Context&);

    virtual void setProperty(const ::std::string&, const ::std::string&, const ::Ice::Context&);
};

class AbstractCommandController : virtual public ::IceDelegate::TeRK::AbstractCommandController,
				  virtual public ::IceDelegateM::TeRK::PropertyManager
{
public:
};

class AnalogInController : virtual public ::IceDelegate::TeRK::AnalogInController,
			   virtual public ::IceDelegateM::TeRK::AbstractCommandController
{
public:

    virtual ::TeRK::AnalogInState getState(const ::Ice::Context&);
};

class AudioController : virtual public ::IceDelegate::TeRK::AudioController,
			virtual public ::IceDelegateM::TeRK::AbstractCommandController
{
public:

    virtual void execute(const ::TeRK::AudioCommand&, const ::Ice::Context&);
};

class DigitalInController : virtual public ::IceDelegate::TeRK::DigitalInController,
			    virtual public ::IceDelegateM::TeRK::AbstractCommandController
{
public:

    virtual ::TeRK::DigitalInState getState(const ::Ice::Context&);
};

class DigitalOutController : virtual public ::IceDelegate::TeRK::DigitalOutController,
			     virtual public ::IceDelegateM::TeRK::AbstractCommandController
{
public:

    virtual void execute(const ::TeRK::DigitalOutCommand&, const ::Ice::Context&);
};

class LEDController : virtual public ::IceDelegate::TeRK::LEDController,
		      virtual public ::IceDelegateM::TeRK::AbstractCommandController
{
public:

    virtual void execute(const ::TeRK::LEDCommand&, const ::Ice::Context&);
};

class MotorController : virtual public ::IceDelegate::TeRK::MotorController,
			virtual public ::IceDelegateM::TeRK::AbstractCommandController
{
public:

    virtual ::TeRK::MotorState execute(const ::TeRK::MotorCommand&, const ::Ice::Context&);

    virtual ::Ice::Int getFrequency(const ::Ice::Context&);

    virtual void startMotorBufferRecord(const ::TeRK::BooleanArray&, const ::Ice::Context&);

    virtual void stopMotorBufferRecord(const ::TeRK::BooleanArray&, const ::Ice::Context&);

    virtual ::TeRK::MotorBufferArray getMotorBuffers(const ::TeRK::BooleanArray&, const ::Ice::Context&);

    virtual void setMotorBuffer(const ::TeRK::BooleanArray&, const ::TeRK::MotorBufferArray&, const ::Ice::Context&);

    virtual void playMotorBuffer(const ::TeRK::BooleanArray&, const ::Ice::Context&);

    virtual void driveForward(const ::Ice::Context&);

    virtual void driveBack(const ::Ice::Context&);

    virtual void spinLeft(const ::Ice::Context&);

    virtual void spinRight(const ::Ice::Context&);

    virtual void setMotorVelocities(const ::TeRK::IntArray&, const ::Ice::Context&);

    virtual void stopMotors(const ::Ice::Context&);
};

class ServoController : virtual public ::IceDelegate::TeRK::ServoController,
			virtual public ::IceDelegateM::TeRK::AbstractCommandController
{
public:

    virtual ::TeRK::ServoState execute(const ::TeRK::ServoCommand&, const ::Ice::Context&);

    virtual void cameraTiltUp(const ::Ice::Context&);

    virtual void cameraTiltDown(const ::Ice::Context&);

    virtual void cameraPanLeft(const ::Ice::Context&);

    virtual void cameraPanRight(const ::Ice::Context&);

    virtual void setServoPositions(const ::TeRK::IntArray&, const ::Ice::Context&);

    virtual void setServoVelocities(const ::TeRK::IntArray&, const ::Ice::Context&);

    virtual void stopServos(const ::Ice::Context&);
};

class VideoStreamerClient : virtual public ::IceDelegate::TeRK::VideoStreamerClient,
			    virtual public ::IceDelegateM::Ice::Object
{
public:

    virtual void newFrame(const ::TeRK::Image&, const ::Ice::Context&);
};

class VideoStreamerServer : virtual public ::IceDelegate::TeRK::VideoStreamerServer,
			    virtual public ::IceDelegateM::TeRK::AbstractCommandController
{
public:

    virtual void startVideoStream(const ::Ice::Context&);

    virtual void stopVideoStream(const ::Ice::Context&);

    virtual ::Ice::Int startCamera(const ::Ice::Context&);

    virtual ::Ice::Int stopCamera(const ::Ice::Context&);

    virtual ::TeRK::Image getFrame(::Ice::Int, const ::Ice::Context&);
};

class TerkUser : virtual public ::IceDelegate::TeRK::TerkUser,
		 virtual public ::IceDelegateM::peer::ConnectionEventHandler,
		 virtual public ::IceDelegateM::TeRK::PropertyManager
{
public:

    virtual ::TeRK::ProxyTypeIdToIdentityMap getSupportedServices(const ::Ice::Context&);
};

class Qwerk : virtual public ::IceDelegate::TeRK::Qwerk,
	      virtual public ::IceDelegateM::TeRK::TerkUser
{
public:

    virtual ::TeRK::QwerkState getState(const ::Ice::Context&);

    virtual ::TeRK::QwerkState execute(const ::TeRK::QwerkCommand&, const ::Ice::Context&);

    virtual ::TeRK::QwerkState emergencyStop(const ::Ice::Context&);

    virtual ::TeRK::ProxyTypeIdToIdentityMap getCommandControllerTypeToProxyIdentityMap(const ::Ice::Context&);
};

class TerkClient : virtual public ::IceDelegate::TeRK::TerkClient,
		   virtual public ::IceDelegateM::TeRK::TerkUser,
		   virtual public ::IceDelegateM::TeRK::VideoStreamerClient
{
public:
};

}

}

namespace IceDelegateD
{

namespace TeRK
{

class PropertyManager : virtual public ::IceDelegate::TeRK::PropertyManager,
			virtual public ::IceDelegateD::Ice::Object
{
public:

    virtual ::std::string getProperty(const ::std::string&, const ::Ice::Context&);

    virtual ::TeRK::PropertyMap getProperties(const ::Ice::Context&);

    virtual ::TeRK::StringArray getPropertyKeys(const ::Ice::Context&);

    virtual void setProperty(const ::std::string&, const ::std::string&, const ::Ice::Context&);
};

class AbstractCommandController : virtual public ::IceDelegate::TeRK::AbstractCommandController,
				  virtual public ::IceDelegateD::TeRK::PropertyManager
{
public:
};

class AnalogInController : virtual public ::IceDelegate::TeRK::AnalogInController,
			   virtual public ::IceDelegateD::TeRK::AbstractCommandController
{
public:

    virtual ::TeRK::AnalogInState getState(const ::Ice::Context&);
};

class AudioController : virtual public ::IceDelegate::TeRK::AudioController,
			virtual public ::IceDelegateD::TeRK::AbstractCommandController
{
public:

    virtual void execute(const ::TeRK::AudioCommand&, const ::Ice::Context&);
};

class DigitalInController : virtual public ::IceDelegate::TeRK::DigitalInController,
			    virtual public ::IceDelegateD::TeRK::AbstractCommandController
{
public:

    virtual ::TeRK::DigitalInState getState(const ::Ice::Context&);
};

class DigitalOutController : virtual public ::IceDelegate::TeRK::DigitalOutController,
			     virtual public ::IceDelegateD::TeRK::AbstractCommandController
{
public:

    virtual void execute(const ::TeRK::DigitalOutCommand&, const ::Ice::Context&);
};

class LEDController : virtual public ::IceDelegate::TeRK::LEDController,
		      virtual public ::IceDelegateD::TeRK::AbstractCommandController
{
public:

    virtual void execute(const ::TeRK::LEDCommand&, const ::Ice::Context&);
};

class MotorController : virtual public ::IceDelegate::TeRK::MotorController,
			virtual public ::IceDelegateD::TeRK::AbstractCommandController
{
public:

    virtual ::TeRK::MotorState execute(const ::TeRK::MotorCommand&, const ::Ice::Context&);

    virtual ::Ice::Int getFrequency(const ::Ice::Context&);

    virtual void startMotorBufferRecord(const ::TeRK::BooleanArray&, const ::Ice::Context&);

    virtual void stopMotorBufferRecord(const ::TeRK::BooleanArray&, const ::Ice::Context&);

    virtual ::TeRK::MotorBufferArray getMotorBuffers(const ::TeRK::BooleanArray&, const ::Ice::Context&);

    virtual void setMotorBuffer(const ::TeRK::BooleanArray&, const ::TeRK::MotorBufferArray&, const ::Ice::Context&);

    virtual void playMotorBuffer(const ::TeRK::BooleanArray&, const ::Ice::Context&);

    virtual void driveForward(const ::Ice::Context&);

    virtual void driveBack(const ::Ice::Context&);

    virtual void spinLeft(const ::Ice::Context&);

    virtual void spinRight(const ::Ice::Context&);

    virtual void setMotorVelocities(const ::TeRK::IntArray&, const ::Ice::Context&);

    virtual void stopMotors(const ::Ice::Context&);
};

class ServoController : virtual public ::IceDelegate::TeRK::ServoController,
			virtual public ::IceDelegateD::TeRK::AbstractCommandController
{
public:

    virtual ::TeRK::ServoState execute(const ::TeRK::ServoCommand&, const ::Ice::Context&);

    virtual void cameraTiltUp(const ::Ice::Context&);

    virtual void cameraTiltDown(const ::Ice::Context&);

    virtual void cameraPanLeft(const ::Ice::Context&);

    virtual void cameraPanRight(const ::Ice::Context&);

    virtual void setServoPositions(const ::TeRK::IntArray&, const ::Ice::Context&);

    virtual void setServoVelocities(const ::TeRK::IntArray&, const ::Ice::Context&);

    virtual void stopServos(const ::Ice::Context&);
};

class VideoStreamerClient : virtual public ::IceDelegate::TeRK::VideoStreamerClient,
			    virtual public ::IceDelegateD::Ice::Object
{
public:

    virtual void newFrame(const ::TeRK::Image&, const ::Ice::Context&);
};

class VideoStreamerServer : virtual public ::IceDelegate::TeRK::VideoStreamerServer,
			    virtual public ::IceDelegateD::TeRK::AbstractCommandController
{
public:

    virtual void startVideoStream(const ::Ice::Context&);

    virtual void stopVideoStream(const ::Ice::Context&);

    virtual ::Ice::Int startCamera(const ::Ice::Context&);

    virtual ::Ice::Int stopCamera(const ::Ice::Context&);

    virtual ::TeRK::Image getFrame(::Ice::Int, const ::Ice::Context&);
};

class TerkUser : virtual public ::IceDelegate::TeRK::TerkUser,
		 virtual public ::IceDelegateD::peer::ConnectionEventHandler,
		 virtual public ::IceDelegateD::TeRK::PropertyManager
{
public:

    virtual ::TeRK::ProxyTypeIdToIdentityMap getSupportedServices(const ::Ice::Context&);
};

class Qwerk : virtual public ::IceDelegate::TeRK::Qwerk,
	      virtual public ::IceDelegateD::TeRK::TerkUser
{
public:

    virtual ::TeRK::QwerkState getState(const ::Ice::Context&);

    virtual ::TeRK::QwerkState execute(const ::TeRK::QwerkCommand&, const ::Ice::Context&);

    virtual ::TeRK::QwerkState emergencyStop(const ::Ice::Context&);

    virtual ::TeRK::ProxyTypeIdToIdentityMap getCommandControllerTypeToProxyIdentityMap(const ::Ice::Context&);
};

class TerkClient : virtual public ::IceDelegate::TeRK::TerkClient,
		   virtual public ::IceDelegateD::TeRK::TerkUser,
		   virtual public ::IceDelegateD::TeRK::VideoStreamerClient
{
public:
};

}

}

namespace TeRK
{

class PropertyManager : virtual public ::Ice::Object
{
public:

    typedef PropertyManagerPrx ProxyType;
    typedef PropertyManagerPtr PointerType;
    
    virtual ::Ice::ObjectPtr ice_clone() const;

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();

    virtual ::std::string getProperty(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___getProperty(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::TeRK::PropertyMap getProperties(const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___getProperties(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::TeRK::StringArray getPropertyKeys(const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___getPropertyKeys(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void setProperty(const ::std::string&, const ::std::string&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___setProperty(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::IceInternal::DispatchStatus __dispatch(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);
    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

void __patch__PropertyManagerPtr(void*, ::Ice::ObjectPtr&);

class AbstractCommandController : virtual public ::TeRK::PropertyManager
{
public:

    typedef AbstractCommandControllerPrx ProxyType;
    typedef AbstractCommandControllerPtr PointerType;
    
    virtual ::Ice::ObjectPtr ice_clone() const;

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();

    virtual ::IceInternal::DispatchStatus __dispatch(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);
    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

void __patch__AbstractCommandControllerPtr(void*, ::Ice::ObjectPtr&);

class AnalogInController : virtual public ::TeRK::AbstractCommandController
{
public:

    typedef AnalogInControllerPrx ProxyType;
    typedef AnalogInControllerPtr PointerType;
    
    virtual ::Ice::ObjectPtr ice_clone() const;

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();

    virtual ::TeRK::AnalogInState getState(const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___getState(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::IceInternal::DispatchStatus __dispatch(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);
    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

void __patch__AnalogInControllerPtr(void*, ::Ice::ObjectPtr&);

class AudioController : virtual public ::TeRK::AbstractCommandController
{
public:

    typedef AudioControllerPrx ProxyType;
    typedef AudioControllerPtr PointerType;
    
    virtual ::Ice::ObjectPtr ice_clone() const;

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();

    virtual void execute(const ::TeRK::AudioCommand&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___execute(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::IceInternal::DispatchStatus __dispatch(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);
    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

void __patch__AudioControllerPtr(void*, ::Ice::ObjectPtr&);

class DigitalInController : virtual public ::TeRK::AbstractCommandController
{
public:

    typedef DigitalInControllerPrx ProxyType;
    typedef DigitalInControllerPtr PointerType;
    
    virtual ::Ice::ObjectPtr ice_clone() const;

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();

    virtual ::TeRK::DigitalInState getState(const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___getState(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::IceInternal::DispatchStatus __dispatch(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);
    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

void __patch__DigitalInControllerPtr(void*, ::Ice::ObjectPtr&);

class DigitalOutController : virtual public ::TeRK::AbstractCommandController
{
public:

    typedef DigitalOutControllerPrx ProxyType;
    typedef DigitalOutControllerPtr PointerType;
    
    virtual ::Ice::ObjectPtr ice_clone() const;

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();

    virtual void execute(const ::TeRK::DigitalOutCommand&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___execute(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::IceInternal::DispatchStatus __dispatch(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);
    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

void __patch__DigitalOutControllerPtr(void*, ::Ice::ObjectPtr&);

class LEDController : virtual public ::TeRK::AbstractCommandController
{
public:

    typedef LEDControllerPrx ProxyType;
    typedef LEDControllerPtr PointerType;
    
    virtual ::Ice::ObjectPtr ice_clone() const;

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();

    virtual void execute(const ::TeRK::LEDCommand&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___execute(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::IceInternal::DispatchStatus __dispatch(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);
    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

void __patch__LEDControllerPtr(void*, ::Ice::ObjectPtr&);

class MotorController : virtual public ::TeRK::AbstractCommandController
{
public:

    typedef MotorControllerPrx ProxyType;
    typedef MotorControllerPtr PointerType;
    
    virtual ::Ice::ObjectPtr ice_clone() const;

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();

    virtual ::TeRK::MotorState execute(const ::TeRK::MotorCommand&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___execute(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::Ice::Int getFrequency(const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___getFrequency(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void startMotorBufferRecord(const ::TeRK::BooleanArray&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___startMotorBufferRecord(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void stopMotorBufferRecord(const ::TeRK::BooleanArray&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___stopMotorBufferRecord(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::TeRK::MotorBufferArray getMotorBuffers(const ::TeRK::BooleanArray&, const ::Ice::Current& = ::Ice::Current()) const = 0;
    ::IceInternal::DispatchStatus ___getMotorBuffers(::IceInternal::Incoming&, const ::Ice::Current&) const;

    virtual void setMotorBuffer(const ::TeRK::BooleanArray&, const ::TeRK::MotorBufferArray&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___setMotorBuffer(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void playMotorBuffer(const ::TeRK::BooleanArray&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___playMotorBuffer(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void driveForward(const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___driveForward(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void driveBack(const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___driveBack(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void spinLeft(const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___spinLeft(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void spinRight(const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___spinRight(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void setMotorVelocities(const ::TeRK::IntArray&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___setMotorVelocities(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void stopMotors(const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___stopMotors(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::IceInternal::DispatchStatus __dispatch(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);
    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

void __patch__MotorControllerPtr(void*, ::Ice::ObjectPtr&);

class ServoController : virtual public ::TeRK::AbstractCommandController
{
public:

    typedef ServoControllerPrx ProxyType;
    typedef ServoControllerPtr PointerType;
    
    virtual ::Ice::ObjectPtr ice_clone() const;

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();

    virtual ::TeRK::ServoState execute(const ::TeRK::ServoCommand&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___execute(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void cameraTiltUp(const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___cameraTiltUp(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void cameraTiltDown(const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___cameraTiltDown(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void cameraPanLeft(const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___cameraPanLeft(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void cameraPanRight(const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___cameraPanRight(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void setServoPositions(const ::TeRK::IntArray&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___setServoPositions(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void setServoVelocities(const ::TeRK::IntArray&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___setServoVelocities(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void stopServos(const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___stopServos(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::IceInternal::DispatchStatus __dispatch(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);
    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

void __patch__ServoControllerPtr(void*, ::Ice::ObjectPtr&);

class VideoStreamerClient : virtual public ::Ice::Object
{
public:

    typedef VideoStreamerClientPrx ProxyType;
    typedef VideoStreamerClientPtr PointerType;
    
    virtual ::Ice::ObjectPtr ice_clone() const;

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();

    virtual void newFrame(const ::TeRK::Image&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___newFrame(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::IceInternal::DispatchStatus __dispatch(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);
    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

void __patch__VideoStreamerClientPtr(void*, ::Ice::ObjectPtr&);

class VideoStreamerServer : virtual public ::TeRK::AbstractCommandController
{
public:

    typedef VideoStreamerServerPrx ProxyType;
    typedef VideoStreamerServerPtr PointerType;
    
    virtual ::Ice::ObjectPtr ice_clone() const;

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();

    virtual void startVideoStream(const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___startVideoStream(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void stopVideoStream(const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___stopVideoStream(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::Ice::Int startCamera(const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___startCamera(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::Ice::Int stopCamera(const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___stopCamera(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::TeRK::Image getFrame(::Ice::Int, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___getFrame(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::IceInternal::DispatchStatus __dispatch(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);
    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

void __patch__VideoStreamerServerPtr(void*, ::Ice::ObjectPtr&);

class TerkUser : virtual public ::peer::ConnectionEventHandler,
		 virtual public ::TeRK::PropertyManager
{
public:

    typedef TerkUserPrx ProxyType;
    typedef TerkUserPtr PointerType;
    
    virtual ::Ice::ObjectPtr ice_clone() const;

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();

    virtual ::TeRK::ProxyTypeIdToIdentityMap getSupportedServices(const ::Ice::Current& = ::Ice::Current()) const = 0;
    ::IceInternal::DispatchStatus ___getSupportedServices(::IceInternal::Incoming&, const ::Ice::Current&) const;

    virtual ::IceInternal::DispatchStatus __dispatch(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);
    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

void __patch__TerkUserPtr(void*, ::Ice::ObjectPtr&);

class Qwerk : virtual public ::TeRK::TerkUser
{
public:

    typedef QwerkPrx ProxyType;
    typedef QwerkPtr PointerType;
    
    virtual ::Ice::ObjectPtr ice_clone() const;

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();

    virtual ::TeRK::QwerkState getState(const ::Ice::Current& = ::Ice::Current()) const = 0;
    ::IceInternal::DispatchStatus ___getState(::IceInternal::Incoming&, const ::Ice::Current&) const;

    virtual ::TeRK::QwerkState execute(const ::TeRK::QwerkCommand&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___execute(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::TeRK::QwerkState emergencyStop(const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___emergencyStop(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::TeRK::ProxyTypeIdToIdentityMap getCommandControllerTypeToProxyIdentityMap(const ::Ice::Current& = ::Ice::Current()) const = 0;
    ::IceInternal::DispatchStatus ___getCommandControllerTypeToProxyIdentityMap(::IceInternal::Incoming&, const ::Ice::Current&) const;

    virtual ::IceInternal::DispatchStatus __dispatch(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);
    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

void __patch__QwerkPtr(void*, ::Ice::ObjectPtr&);

class TerkClient : virtual public ::TeRK::TerkUser,
		   virtual public ::TeRK::VideoStreamerClient
{
public:

    typedef TerkClientPrx ProxyType;
    typedef TerkClientPtr PointerType;
    
    virtual ::Ice::ObjectPtr ice_clone() const;

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();

    virtual ::IceInternal::DispatchStatus __dispatch(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);
    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

void __patch__TerkClientPtr(void*, ::Ice::ObjectPtr&);

}

#endif
