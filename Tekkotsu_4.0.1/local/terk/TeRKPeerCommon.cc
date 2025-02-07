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
// Generated from file `TeRKPeerCommon.ice'

#include "TeRKPeerCommon.h"
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

static const ::std::string __TeRK__PropertyManager__getProperty_name = "getProperty";

static const ::std::string __TeRK__PropertyManager__getProperties_name = "getProperties";

static const ::std::string __TeRK__PropertyManager__getPropertyKeys_name = "getPropertyKeys";

static const ::std::string __TeRK__PropertyManager__setProperty_name = "setProperty";

static const ::std::string __TeRK__AnalogInController__getState_name = "getState";

static const ::std::string __TeRK__AudioController__execute_name = "execute";

static const ::std::string __TeRK__DigitalInController__getState_name = "getState";

static const ::std::string __TeRK__DigitalOutController__execute_name = "execute";

static const ::std::string __TeRK__LEDController__execute_name = "execute";

static const ::std::string __TeRK__MotorController__execute_name = "execute";

static const ::std::string __TeRK__MotorController__getFrequency_name = "getFrequency";

static const ::std::string __TeRK__MotorController__startMotorBufferRecord_name = "startMotorBufferRecord";

static const ::std::string __TeRK__MotorController__stopMotorBufferRecord_name = "stopMotorBufferRecord";

static const ::std::string __TeRK__MotorController__getMotorBuffers_name = "getMotorBuffers";

static const ::std::string __TeRK__MotorController__setMotorBuffer_name = "setMotorBuffer";

static const ::std::string __TeRK__MotorController__playMotorBuffer_name = "playMotorBuffer";

static const ::std::string __TeRK__MotorController__driveForward_name = "driveForward";

static const ::std::string __TeRK__MotorController__driveBack_name = "driveBack";

static const ::std::string __TeRK__MotorController__spinLeft_name = "spinLeft";

static const ::std::string __TeRK__MotorController__spinRight_name = "spinRight";

static const ::std::string __TeRK__MotorController__setMotorVelocities_name = "setMotorVelocities";

static const ::std::string __TeRK__MotorController__stopMotors_name = "stopMotors";

static const ::std::string __TeRK__ServoController__execute_name = "execute";

static const ::std::string __TeRK__ServoController__cameraTiltUp_name = "cameraTiltUp";

static const ::std::string __TeRK__ServoController__cameraTiltDown_name = "cameraTiltDown";

static const ::std::string __TeRK__ServoController__cameraPanLeft_name = "cameraPanLeft";

static const ::std::string __TeRK__ServoController__cameraPanRight_name = "cameraPanRight";

static const ::std::string __TeRK__ServoController__setServoPositions_name = "setServoPositions";

static const ::std::string __TeRK__ServoController__setServoVelocities_name = "setServoVelocities";

static const ::std::string __TeRK__ServoController__stopServos_name = "stopServos";

static const ::std::string __TeRK__VideoStreamerClient__newFrame_name = "newFrame";

static const ::std::string __TeRK__VideoStreamerServer__startVideoStream_name = "startVideoStream";

static const ::std::string __TeRK__VideoStreamerServer__stopVideoStream_name = "stopVideoStream";

static const ::std::string __TeRK__VideoStreamerServer__startCamera_name = "startCamera";

static const ::std::string __TeRK__VideoStreamerServer__stopCamera_name = "stopCamera";

static const ::std::string __TeRK__VideoStreamerServer__getFrame_name = "getFrame";

static const ::std::string __TeRK__TerkUser__getSupportedServices_name = "getSupportedServices";

static const ::std::string __TeRK__Qwerk__getState_name = "getState";

static const ::std::string __TeRK__Qwerk__execute_name = "execute";

static const ::std::string __TeRK__Qwerk__emergencyStop_name = "emergencyStop";

static const ::std::string __TeRK__Qwerk__getCommandControllerTypeToProxyIdentityMap_name = "getCommandControllerTypeToProxyIdentityMap";

void
IceInternal::incRef(::TeRK::PropertyManager* p)
{
    p->__incRef();
}

void
IceInternal::decRef(::TeRK::PropertyManager* p)
{
    p->__decRef();
}

void
IceInternal::incRef(::IceProxy::TeRK::PropertyManager* p)
{
    p->__incRef();
}

void
IceInternal::decRef(::IceProxy::TeRK::PropertyManager* p)
{
    p->__decRef();
}

void
IceInternal::incRef(::TeRK::AbstractCommandController* p)
{
    p->__incRef();
}

void
IceInternal::decRef(::TeRK::AbstractCommandController* p)
{
    p->__decRef();
}

void
IceInternal::incRef(::IceProxy::TeRK::AbstractCommandController* p)
{
    p->__incRef();
}

void
IceInternal::decRef(::IceProxy::TeRK::AbstractCommandController* p)
{
    p->__decRef();
}

void
IceInternal::incRef(::TeRK::AnalogInController* p)
{
    p->__incRef();
}

void
IceInternal::decRef(::TeRK::AnalogInController* p)
{
    p->__decRef();
}

void
IceInternal::incRef(::IceProxy::TeRK::AnalogInController* p)
{
    p->__incRef();
}

void
IceInternal::decRef(::IceProxy::TeRK::AnalogInController* p)
{
    p->__decRef();
}

void
IceInternal::incRef(::TeRK::AudioController* p)
{
    p->__incRef();
}

void
IceInternal::decRef(::TeRK::AudioController* p)
{
    p->__decRef();
}

void
IceInternal::incRef(::IceProxy::TeRK::AudioController* p)
{
    p->__incRef();
}

void
IceInternal::decRef(::IceProxy::TeRK::AudioController* p)
{
    p->__decRef();
}

void
IceInternal::incRef(::TeRK::DigitalInController* p)
{
    p->__incRef();
}

void
IceInternal::decRef(::TeRK::DigitalInController* p)
{
    p->__decRef();
}

void
IceInternal::incRef(::IceProxy::TeRK::DigitalInController* p)
{
    p->__incRef();
}

void
IceInternal::decRef(::IceProxy::TeRK::DigitalInController* p)
{
    p->__decRef();
}

void
IceInternal::incRef(::TeRK::DigitalOutController* p)
{
    p->__incRef();
}

void
IceInternal::decRef(::TeRK::DigitalOutController* p)
{
    p->__decRef();
}

void
IceInternal::incRef(::IceProxy::TeRK::DigitalOutController* p)
{
    p->__incRef();
}

void
IceInternal::decRef(::IceProxy::TeRK::DigitalOutController* p)
{
    p->__decRef();
}

void
IceInternal::incRef(::TeRK::LEDController* p)
{
    p->__incRef();
}

void
IceInternal::decRef(::TeRK::LEDController* p)
{
    p->__decRef();
}

void
IceInternal::incRef(::IceProxy::TeRK::LEDController* p)
{
    p->__incRef();
}

void
IceInternal::decRef(::IceProxy::TeRK::LEDController* p)
{
    p->__decRef();
}

void
IceInternal::incRef(::TeRK::MotorController* p)
{
    p->__incRef();
}

void
IceInternal::decRef(::TeRK::MotorController* p)
{
    p->__decRef();
}

void
IceInternal::incRef(::IceProxy::TeRK::MotorController* p)
{
    p->__incRef();
}

void
IceInternal::decRef(::IceProxy::TeRK::MotorController* p)
{
    p->__decRef();
}

void
IceInternal::incRef(::TeRK::ServoController* p)
{
    p->__incRef();
}

void
IceInternal::decRef(::TeRK::ServoController* p)
{
    p->__decRef();
}

void
IceInternal::incRef(::IceProxy::TeRK::ServoController* p)
{
    p->__incRef();
}

void
IceInternal::decRef(::IceProxy::TeRK::ServoController* p)
{
    p->__decRef();
}

void
IceInternal::incRef(::TeRK::VideoStreamerClient* p)
{
    p->__incRef();
}

void
IceInternal::decRef(::TeRK::VideoStreamerClient* p)
{
    p->__decRef();
}

void
IceInternal::incRef(::IceProxy::TeRK::VideoStreamerClient* p)
{
    p->__incRef();
}

void
IceInternal::decRef(::IceProxy::TeRK::VideoStreamerClient* p)
{
    p->__decRef();
}

void
IceInternal::incRef(::TeRK::VideoStreamerServer* p)
{
    p->__incRef();
}

void
IceInternal::decRef(::TeRK::VideoStreamerServer* p)
{
    p->__decRef();
}

void
IceInternal::incRef(::IceProxy::TeRK::VideoStreamerServer* p)
{
    p->__incRef();
}

void
IceInternal::decRef(::IceProxy::TeRK::VideoStreamerServer* p)
{
    p->__decRef();
}

void
IceInternal::incRef(::TeRK::TerkUser* p)
{
    p->__incRef();
}

void
IceInternal::decRef(::TeRK::TerkUser* p)
{
    p->__decRef();
}

void
IceInternal::incRef(::IceProxy::TeRK::TerkUser* p)
{
    p->__incRef();
}

void
IceInternal::decRef(::IceProxy::TeRK::TerkUser* p)
{
    p->__decRef();
}

void
IceInternal::incRef(::TeRK::Qwerk* p)
{
    p->__incRef();
}

void
IceInternal::decRef(::TeRK::Qwerk* p)
{
    p->__decRef();
}

void
IceInternal::incRef(::IceProxy::TeRK::Qwerk* p)
{
    p->__incRef();
}

void
IceInternal::decRef(::IceProxy::TeRK::Qwerk* p)
{
    p->__decRef();
}

void
IceInternal::incRef(::TeRK::TerkClient* p)
{
    p->__incRef();
}

void
IceInternal::decRef(::TeRK::TerkClient* p)
{
    p->__decRef();
}

void
IceInternal::incRef(::IceProxy::TeRK::TerkClient* p)
{
    p->__incRef();
}

void
IceInternal::decRef(::IceProxy::TeRK::TerkClient* p)
{
    p->__decRef();
}

void
TeRK::__write(::IceInternal::BasicStream* __os, const ::TeRK::PropertyManagerPrx& v)
{
    __os->write(::Ice::ObjectPrx(v));
}

void
TeRK::__read(::IceInternal::BasicStream* __is, ::TeRK::PropertyManagerPrx& v)
{
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    if(!proxy)
    {
	v = 0;
    }
    else
    {
	v = new ::IceProxy::TeRK::PropertyManager;
	v->__copyFrom(proxy);
    }
}

void
TeRK::__write(::IceInternal::BasicStream* __os, const ::TeRK::PropertyManagerPtr& v)
{
    __os->write(::Ice::ObjectPtr(v));
}

void
TeRK::__write(::IceInternal::BasicStream* __os, const ::TeRK::AbstractCommandControllerPrx& v)
{
    __os->write(::Ice::ObjectPrx(v));
}

void
TeRK::__read(::IceInternal::BasicStream* __is, ::TeRK::AbstractCommandControllerPrx& v)
{
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    if(!proxy)
    {
	v = 0;
    }
    else
    {
	v = new ::IceProxy::TeRK::AbstractCommandController;
	v->__copyFrom(proxy);
    }
}

void
TeRK::__write(::IceInternal::BasicStream* __os, const ::TeRK::AbstractCommandControllerPtr& v)
{
    __os->write(::Ice::ObjectPtr(v));
}

void
TeRK::__write(::IceInternal::BasicStream* __os, const ::TeRK::AnalogInControllerPrx& v)
{
    __os->write(::Ice::ObjectPrx(v));
}

void
TeRK::__read(::IceInternal::BasicStream* __is, ::TeRK::AnalogInControllerPrx& v)
{
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    if(!proxy)
    {
	v = 0;
    }
    else
    {
	v = new ::IceProxy::TeRK::AnalogInController;
	v->__copyFrom(proxy);
    }
}

void
TeRK::__write(::IceInternal::BasicStream* __os, const ::TeRK::AnalogInControllerPtr& v)
{
    __os->write(::Ice::ObjectPtr(v));
}

void
TeRK::__write(::IceInternal::BasicStream* __os, const ::TeRK::AudioControllerPrx& v)
{
    __os->write(::Ice::ObjectPrx(v));
}

void
TeRK::__read(::IceInternal::BasicStream* __is, ::TeRK::AudioControllerPrx& v)
{
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    if(!proxy)
    {
	v = 0;
    }
    else
    {
	v = new ::IceProxy::TeRK::AudioController;
	v->__copyFrom(proxy);
    }
}

void
TeRK::__write(::IceInternal::BasicStream* __os, const ::TeRK::AudioControllerPtr& v)
{
    __os->write(::Ice::ObjectPtr(v));
}

void
TeRK::__write(::IceInternal::BasicStream* __os, const ::TeRK::DigitalInControllerPrx& v)
{
    __os->write(::Ice::ObjectPrx(v));
}

void
TeRK::__read(::IceInternal::BasicStream* __is, ::TeRK::DigitalInControllerPrx& v)
{
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    if(!proxy)
    {
	v = 0;
    }
    else
    {
	v = new ::IceProxy::TeRK::DigitalInController;
	v->__copyFrom(proxy);
    }
}

void
TeRK::__write(::IceInternal::BasicStream* __os, const ::TeRK::DigitalInControllerPtr& v)
{
    __os->write(::Ice::ObjectPtr(v));
}

void
TeRK::__write(::IceInternal::BasicStream* __os, const ::TeRK::DigitalOutControllerPrx& v)
{
    __os->write(::Ice::ObjectPrx(v));
}

void
TeRK::__read(::IceInternal::BasicStream* __is, ::TeRK::DigitalOutControllerPrx& v)
{
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    if(!proxy)
    {
	v = 0;
    }
    else
    {
	v = new ::IceProxy::TeRK::DigitalOutController;
	v->__copyFrom(proxy);
    }
}

void
TeRK::__write(::IceInternal::BasicStream* __os, const ::TeRK::DigitalOutControllerPtr& v)
{
    __os->write(::Ice::ObjectPtr(v));
}

void
TeRK::__write(::IceInternal::BasicStream* __os, const ::TeRK::LEDControllerPrx& v)
{
    __os->write(::Ice::ObjectPrx(v));
}

void
TeRK::__read(::IceInternal::BasicStream* __is, ::TeRK::LEDControllerPrx& v)
{
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    if(!proxy)
    {
	v = 0;
    }
    else
    {
	v = new ::IceProxy::TeRK::LEDController;
	v->__copyFrom(proxy);
    }
}

void
TeRK::__write(::IceInternal::BasicStream* __os, const ::TeRK::LEDControllerPtr& v)
{
    __os->write(::Ice::ObjectPtr(v));
}

void
TeRK::__write(::IceInternal::BasicStream* __os, const ::TeRK::MotorControllerPrx& v)
{
    __os->write(::Ice::ObjectPrx(v));
}

void
TeRK::__read(::IceInternal::BasicStream* __is, ::TeRK::MotorControllerPrx& v)
{
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    if(!proxy)
    {
	v = 0;
    }
    else
    {
	v = new ::IceProxy::TeRK::MotorController;
	v->__copyFrom(proxy);
    }
}

void
TeRK::__write(::IceInternal::BasicStream* __os, const ::TeRK::MotorControllerPtr& v)
{
    __os->write(::Ice::ObjectPtr(v));
}

void
TeRK::__write(::IceInternal::BasicStream* __os, const ::TeRK::ServoControllerPrx& v)
{
    __os->write(::Ice::ObjectPrx(v));
}

void
TeRK::__read(::IceInternal::BasicStream* __is, ::TeRK::ServoControllerPrx& v)
{
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    if(!proxy)
    {
	v = 0;
    }
    else
    {
	v = new ::IceProxy::TeRK::ServoController;
	v->__copyFrom(proxy);
    }
}

void
TeRK::__write(::IceInternal::BasicStream* __os, const ::TeRK::ServoControllerPtr& v)
{
    __os->write(::Ice::ObjectPtr(v));
}

void
TeRK::__write(::IceInternal::BasicStream* __os, const ::TeRK::VideoStreamerClientPrx& v)
{
    __os->write(::Ice::ObjectPrx(v));
}

void
TeRK::__read(::IceInternal::BasicStream* __is, ::TeRK::VideoStreamerClientPrx& v)
{
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    if(!proxy)
    {
	v = 0;
    }
    else
    {
	v = new ::IceProxy::TeRK::VideoStreamerClient;
	v->__copyFrom(proxy);
    }
}

void
TeRK::__write(::IceInternal::BasicStream* __os, const ::TeRK::VideoStreamerClientPtr& v)
{
    __os->write(::Ice::ObjectPtr(v));
}

void
TeRK::__write(::IceInternal::BasicStream* __os, const ::TeRK::VideoStreamerServerPrx& v)
{
    __os->write(::Ice::ObjectPrx(v));
}

void
TeRK::__read(::IceInternal::BasicStream* __is, ::TeRK::VideoStreamerServerPrx& v)
{
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    if(!proxy)
    {
	v = 0;
    }
    else
    {
	v = new ::IceProxy::TeRK::VideoStreamerServer;
	v->__copyFrom(proxy);
    }
}

void
TeRK::__write(::IceInternal::BasicStream* __os, const ::TeRK::VideoStreamerServerPtr& v)
{
    __os->write(::Ice::ObjectPtr(v));
}

void
TeRK::__write(::IceInternal::BasicStream* __os, const ::TeRK::TerkUserPrx& v)
{
    __os->write(::Ice::ObjectPrx(v));
}

void
TeRK::__read(::IceInternal::BasicStream* __is, ::TeRK::TerkUserPrx& v)
{
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    if(!proxy)
    {
	v = 0;
    }
    else
    {
	v = new ::IceProxy::TeRK::TerkUser;
	v->__copyFrom(proxy);
    }
}

void
TeRK::__write(::IceInternal::BasicStream* __os, const ::TeRK::TerkUserPtr& v)
{
    __os->write(::Ice::ObjectPtr(v));
}

void
TeRK::__write(::IceInternal::BasicStream* __os, const ::TeRK::QwerkPrx& v)
{
    __os->write(::Ice::ObjectPrx(v));
}

void
TeRK::__read(::IceInternal::BasicStream* __is, ::TeRK::QwerkPrx& v)
{
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    if(!proxy)
    {
	v = 0;
    }
    else
    {
	v = new ::IceProxy::TeRK::Qwerk;
	v->__copyFrom(proxy);
    }
}

void
TeRK::__write(::IceInternal::BasicStream* __os, const ::TeRK::QwerkPtr& v)
{
    __os->write(::Ice::ObjectPtr(v));
}

void
TeRK::__write(::IceInternal::BasicStream* __os, const ::TeRK::TerkClientPrx& v)
{
    __os->write(::Ice::ObjectPrx(v));
}

void
TeRK::__read(::IceInternal::BasicStream* __is, ::TeRK::TerkClientPrx& v)
{
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    if(!proxy)
    {
	v = 0;
    }
    else
    {
	v = new ::IceProxy::TeRK::TerkClient;
	v->__copyFrom(proxy);
    }
}

void
TeRK::__write(::IceInternal::BasicStream* __os, const ::TeRK::TerkClientPtr& v)
{
    __os->write(::Ice::ObjectPtr(v));
}

void
TeRK::__write(::IceInternal::BasicStream* __os, ::TeRK::ImageFormat v)
{
    __os->write(static_cast< ::Ice::Byte>(v));
}

void
TeRK::__read(::IceInternal::BasicStream* __is, ::TeRK::ImageFormat& v)
{
    ::Ice::Byte val;
    __is->read(val);
    v = static_cast< ::TeRK::ImageFormat>(val);
}

void
TeRK::__write(::IceInternal::BasicStream* __os, ::TeRK::AudioMode v)
{
    __os->write(static_cast< ::Ice::Byte>(v));
}

void
TeRK::__read(::IceInternal::BasicStream* __is, ::TeRK::AudioMode& v)
{
    ::Ice::Byte val;
    __is->read(val);
    v = static_cast< ::TeRK::AudioMode>(val);
}

void
TeRK::__write(::IceInternal::BasicStream* __os, ::TeRK::LEDMode v)
{
    __os->write(static_cast< ::Ice::Byte>(v));
}

void
TeRK::__read(::IceInternal::BasicStream* __is, ::TeRK::LEDMode& v)
{
    ::Ice::Byte val;
    __is->read(val);
    v = static_cast< ::TeRK::LEDMode>(val);
}

void
TeRK::__write(::IceInternal::BasicStream* __os, ::TeRK::MotorMode v)
{
    __os->write(static_cast< ::Ice::Byte>(v));
}

void
TeRK::__read(::IceInternal::BasicStream* __is, ::TeRK::MotorMode& v)
{
    ::Ice::Byte val;
    __is->read(val);
    v = static_cast< ::TeRK::MotorMode>(val);
}

void
TeRK::__write(::IceInternal::BasicStream* __os, ::TeRK::ServoMode v)
{
    __os->write(static_cast< ::Ice::Byte>(v));
}

void
TeRK::__read(::IceInternal::BasicStream* __is, ::TeRK::ServoMode& v)
{
    ::Ice::Byte val;
    __is->read(val);
    v = static_cast< ::TeRK::ServoMode>(val);
}

void
TeRK::__write(::IceInternal::BasicStream* __os, const ::TeRK::LEDMode* begin, const ::TeRK::LEDMode* end, ::TeRK::__U__LEDModeArray)
{
    ::Ice::Int size = static_cast< ::Ice::Int>(end - begin);
    __os->writeSize(size);
    for(int i = 0; i < size; ++i)
    {
	::TeRK::__write(__os, begin[i]);
    }
}

void
TeRK::__read(::IceInternal::BasicStream* __is, ::TeRK::LEDModeArray& v, ::TeRK::__U__LEDModeArray)
{
    ::Ice::Int sz;
    __is->readSize(sz);
    __is->checkFixedSeq(sz, 1);
    v.resize(sz);
    for(int i = 0; i < sz; ++i)
    {
	::TeRK::__read(__is, v[i]);
    }
}

void
TeRK::__write(::IceInternal::BasicStream* __os, const ::TeRK::MotorMode* begin, const ::TeRK::MotorMode* end, ::TeRK::__U__MotorModeArray)
{
    ::Ice::Int size = static_cast< ::Ice::Int>(end - begin);
    __os->writeSize(size);
    for(int i = 0; i < size; ++i)
    {
	::TeRK::__write(__os, begin[i]);
    }
}

void
TeRK::__read(::IceInternal::BasicStream* __is, ::TeRK::MotorModeArray& v, ::TeRK::__U__MotorModeArray)
{
    ::Ice::Int sz;
    __is->readSize(sz);
    __is->checkFixedSeq(sz, 1);
    v.resize(sz);
    for(int i = 0; i < sz; ++i)
    {
	::TeRK::__read(__is, v[i]);
    }
}

void
TeRK::__write(::IceInternal::BasicStream* __os, const ::TeRK::ServoMode* begin, const ::TeRK::ServoMode* end, ::TeRK::__U__ServoModeArray)
{
    ::Ice::Int size = static_cast< ::Ice::Int>(end - begin);
    __os->writeSize(size);
    for(int i = 0; i < size; ++i)
    {
	::TeRK::__write(__os, begin[i]);
    }
}

void
TeRK::__read(::IceInternal::BasicStream* __is, ::TeRK::ServoModeArray& v, ::TeRK::__U__ServoModeArray)
{
    ::Ice::Int sz;
    __is->readSize(sz);
    __is->checkFixedSeq(sz, 1);
    v.resize(sz);
    for(int i = 0; i < sz; ++i)
    {
	::TeRK::__read(__is, v[i]);
    }
}

void
TeRK::__write(::IceInternal::BasicStream* __os, const ::TeRK::ProxyTypeIdToIdentityMap& v, ::TeRK::__U__ProxyTypeIdToIdentityMap)
{
    __os->writeSize(::Ice::Int(v.size()));
    ::TeRK::ProxyTypeIdToIdentityMap::const_iterator p;
    for(p = v.begin(); p != v.end(); ++p)
    {
	__os->write(p->first);
	p->second.__write(__os);
    }
}

void
TeRK::__read(::IceInternal::BasicStream* __is, ::TeRK::ProxyTypeIdToIdentityMap& v, ::TeRK::__U__ProxyTypeIdToIdentityMap)
{
    ::Ice::Int sz;
    __is->readSize(sz);
    while(sz--)
    {
	::std::pair<const  ::std::string, ::Ice::Identity> pair;
	__is->read(const_cast< ::std::string&>(pair.first));
	::TeRK::ProxyTypeIdToIdentityMap::iterator __i = v.insert(v.end(), pair);
	__i->second.__read(__is);
    }
}

void
TeRK::__write(::IceInternal::BasicStream* __os, const ::TeRK::PropertyMap& v, ::TeRK::__U__PropertyMap)
{
    __os->writeSize(::Ice::Int(v.size()));
    ::TeRK::PropertyMap::const_iterator p;
    for(p = v.begin(); p != v.end(); ++p)
    {
	__os->write(p->first);
	__os->write(p->second);
    }
}

void
TeRK::__read(::IceInternal::BasicStream* __is, ::TeRK::PropertyMap& v, ::TeRK::__U__PropertyMap)
{
    ::Ice::Int sz;
    __is->readSize(sz);
    while(sz--)
    {
	::std::pair<const  ::std::string, ::std::string> pair;
	__is->read(const_cast< ::std::string&>(pair.first));
	::TeRK::PropertyMap::iterator __i = v.insert(v.end(), pair);
	__is->read(__i->second);
    }
}

TeRK::GenericError::GenericError(const ::std::string& __ice_reason) :
#if (defined(_MSC_VER) && (_MSC_VER < 1300)) // VC++ 6 compiler bug
    UserException(),
#else
    ::Ice::UserException(),
#endif
    reason(__ice_reason)
{
}

static const char* __TeRK__GenericError_name = "TeRK::GenericError";

const ::std::string
TeRK::GenericError::ice_name() const
{
    return __TeRK__GenericError_name;
}

::Ice::Exception*
TeRK::GenericError::ice_clone() const
{
    return new GenericError(*this);
}

void
TeRK::GenericError::ice_throw() const
{
    throw *this;
}

void
TeRK::GenericError::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(::std::string("::TeRK::GenericError"), false);
    __os->startWriteSlice();
    __os->write(reason);
    __os->endWriteSlice();
}

void
TeRK::GenericError::__read(::IceInternal::BasicStream* __is, bool __rid)
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
TeRK::GenericError::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "exception TeRK::GenericError was not generated with stream support";
    throw ex;
}

void
TeRK::GenericError::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "exception TeRK::GenericError was not generated with stream support";
    throw ex;
}

struct __F__TeRK__GenericError : public ::IceInternal::UserExceptionFactory
{
    virtual void
    createAndThrow()
    {
	throw ::TeRK::GenericError();
    }
};

static ::IceInternal::UserExceptionFactoryPtr __F__TeRK__GenericError__Ptr = new __F__TeRK__GenericError;

const ::IceInternal::UserExceptionFactoryPtr&
TeRK::GenericError::ice_factory()
{
    return __F__TeRK__GenericError__Ptr;
}

class __F__TeRK__GenericError__Init
{
public:

    __F__TeRK__GenericError__Init()
    {
	::IceInternal::factoryTable->addExceptionFactory("::TeRK::GenericError", ::TeRK::GenericError::ice_factory());
    }

    ~__F__TeRK__GenericError__Init()
    {
	::IceInternal::factoryTable->removeExceptionFactory("::TeRK::GenericError");
    }
};

static __F__TeRK__GenericError__Init __F__TeRK__GenericError__i;

#ifdef __APPLE__
extern "C" { void __F__TeRK__GenericError__initializer() {} }
#endif

TeRK::TeRKException::TeRKException(const ::std::string& __ice_reason) :
#if (defined(_MSC_VER) && (_MSC_VER < 1300)) // VC++ 6 compiler bug
    UserException(),
#else
    ::Ice::UserException(),
#endif
    reason(__ice_reason)
{
}

static const char* __TeRK__TeRKException_name = "TeRK::TeRKException";

const ::std::string
TeRK::TeRKException::ice_name() const
{
    return __TeRK__TeRKException_name;
}

::Ice::Exception*
TeRK::TeRKException::ice_clone() const
{
    return new TeRKException(*this);
}

void
TeRK::TeRKException::ice_throw() const
{
    throw *this;
}

void
TeRK::TeRKException::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(::std::string("::TeRK::TeRKException"), false);
    __os->startWriteSlice();
    __os->write(reason);
    __os->endWriteSlice();
}

void
TeRK::TeRKException::__read(::IceInternal::BasicStream* __is, bool __rid)
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
TeRK::TeRKException::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "exception TeRK::TeRKException was not generated with stream support";
    throw ex;
}

void
TeRK::TeRKException::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "exception TeRK::TeRKException was not generated with stream support";
    throw ex;
}

struct __F__TeRK__TeRKException : public ::IceInternal::UserExceptionFactory
{
    virtual void
    createAndThrow()
    {
	throw ::TeRK::TeRKException();
    }
};

static ::IceInternal::UserExceptionFactoryPtr __F__TeRK__TeRKException__Ptr = new __F__TeRK__TeRKException;

const ::IceInternal::UserExceptionFactoryPtr&
TeRK::TeRKException::ice_factory()
{
    return __F__TeRK__TeRKException__Ptr;
}

class __F__TeRK__TeRKException__Init
{
public:

    __F__TeRK__TeRKException__Init()
    {
	::IceInternal::factoryTable->addExceptionFactory("::TeRK::TeRKException", ::TeRK::TeRKException::ice_factory());
    }

    ~__F__TeRK__TeRKException__Init()
    {
	::IceInternal::factoryTable->removeExceptionFactory("::TeRK::TeRKException");
    }
};

static __F__TeRK__TeRKException__Init __F__TeRK__TeRKException__i;

#ifdef __APPLE__
extern "C" { void __F__TeRK__TeRKException__initializer() {} }
#endif

TeRK::MotorException::MotorException(const ::std::string& __ice_reason) :
#if (defined(_MSC_VER) && (_MSC_VER < 1300)) // VC++ 6 compiler bug
    TeRKException(__ice_reason)
#else
    ::TeRK::TeRKException(__ice_reason)
#endif
{
}

static const char* __TeRK__MotorException_name = "TeRK::MotorException";

const ::std::string
TeRK::MotorException::ice_name() const
{
    return __TeRK__MotorException_name;
}

::Ice::Exception*
TeRK::MotorException::ice_clone() const
{
    return new MotorException(*this);
}

void
TeRK::MotorException::ice_throw() const
{
    throw *this;
}

void
TeRK::MotorException::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(::std::string("::TeRK::MotorException"), false);
    __os->startWriteSlice();
    __os->endWriteSlice();
#if (defined(_MSC_VER) && (_MSC_VER < 1300)) // VC++ 6 compiler bug
    TeRKException::__write(__os);
#else
    ::TeRK::TeRKException::__write(__os);
#endif
}

void
TeRK::MotorException::__read(::IceInternal::BasicStream* __is, bool __rid)
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
TeRK::MotorException::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "exception TeRK::MotorException was not generated with stream support";
    throw ex;
}

void
TeRK::MotorException::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "exception TeRK::MotorException was not generated with stream support";
    throw ex;
}

struct __F__TeRK__MotorException : public ::IceInternal::UserExceptionFactory
{
    virtual void
    createAndThrow()
    {
	throw ::TeRK::MotorException();
    }
};

static ::IceInternal::UserExceptionFactoryPtr __F__TeRK__MotorException__Ptr = new __F__TeRK__MotorException;

const ::IceInternal::UserExceptionFactoryPtr&
TeRK::MotorException::ice_factory()
{
    return __F__TeRK__MotorException__Ptr;
}

class __F__TeRK__MotorException__Init
{
public:

    __F__TeRK__MotorException__Init()
    {
	::IceInternal::factoryTable->addExceptionFactory("::TeRK::MotorException", ::TeRK::MotorException::ice_factory());
    }

    ~__F__TeRK__MotorException__Init()
    {
	::IceInternal::factoryTable->removeExceptionFactory("::TeRK::MotorException");
    }
};

static __F__TeRK__MotorException__Init __F__TeRK__MotorException__i;

#ifdef __APPLE__
extern "C" { void __F__TeRK__MotorException__initializer() {} }
#endif

TeRK::VideoException::VideoException(const ::std::string& __ice_reason) :
#if (defined(_MSC_VER) && (_MSC_VER < 1300)) // VC++ 6 compiler bug
    TeRKException(__ice_reason)
#else
    ::TeRK::TeRKException(__ice_reason)
#endif
{
}

static const char* __TeRK__VideoException_name = "TeRK::VideoException";

const ::std::string
TeRK::VideoException::ice_name() const
{
    return __TeRK__VideoException_name;
}

::Ice::Exception*
TeRK::VideoException::ice_clone() const
{
    return new VideoException(*this);
}

void
TeRK::VideoException::ice_throw() const
{
    throw *this;
}

void
TeRK::VideoException::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(::std::string("::TeRK::VideoException"), false);
    __os->startWriteSlice();
    __os->endWriteSlice();
#if (defined(_MSC_VER) && (_MSC_VER < 1300)) // VC++ 6 compiler bug
    TeRKException::__write(__os);
#else
    ::TeRK::TeRKException::__write(__os);
#endif
}

void
TeRK::VideoException::__read(::IceInternal::BasicStream* __is, bool __rid)
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
TeRK::VideoException::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "exception TeRK::VideoException was not generated with stream support";
    throw ex;
}

void
TeRK::VideoException::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "exception TeRK::VideoException was not generated with stream support";
    throw ex;
}

struct __F__TeRK__VideoException : public ::IceInternal::UserExceptionFactory
{
    virtual void
    createAndThrow()
    {
	throw ::TeRK::VideoException();
    }
};

static ::IceInternal::UserExceptionFactoryPtr __F__TeRK__VideoException__Ptr = new __F__TeRK__VideoException;

const ::IceInternal::UserExceptionFactoryPtr&
TeRK::VideoException::ice_factory()
{
    return __F__TeRK__VideoException__Ptr;
}

class __F__TeRK__VideoException__Init
{
public:

    __F__TeRK__VideoException__Init()
    {
	::IceInternal::factoryTable->addExceptionFactory("::TeRK::VideoException", ::TeRK::VideoException::ice_factory());
    }

    ~__F__TeRK__VideoException__Init()
    {
	::IceInternal::factoryTable->removeExceptionFactory("::TeRK::VideoException");
    }
};

static __F__TeRK__VideoException__Init __F__TeRK__VideoException__i;

#ifdef __APPLE__
extern "C" { void __F__TeRK__VideoException__initializer() {} }
#endif

TeRK::ReadOnlyPropertyException::ReadOnlyPropertyException(const ::std::string& __ice_reason) :
#if (defined(_MSC_VER) && (_MSC_VER < 1300)) // VC++ 6 compiler bug
    TeRKException(__ice_reason)
#else
    ::TeRK::TeRKException(__ice_reason)
#endif
{
}

static const char* __TeRK__ReadOnlyPropertyException_name = "TeRK::ReadOnlyPropertyException";

const ::std::string
TeRK::ReadOnlyPropertyException::ice_name() const
{
    return __TeRK__ReadOnlyPropertyException_name;
}

::Ice::Exception*
TeRK::ReadOnlyPropertyException::ice_clone() const
{
    return new ReadOnlyPropertyException(*this);
}

void
TeRK::ReadOnlyPropertyException::ice_throw() const
{
    throw *this;
}

void
TeRK::ReadOnlyPropertyException::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(::std::string("::TeRK::ReadOnlyPropertyException"), false);
    __os->startWriteSlice();
    __os->endWriteSlice();
#if (defined(_MSC_VER) && (_MSC_VER < 1300)) // VC++ 6 compiler bug
    TeRKException::__write(__os);
#else
    ::TeRK::TeRKException::__write(__os);
#endif
}

void
TeRK::ReadOnlyPropertyException::__read(::IceInternal::BasicStream* __is, bool __rid)
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
TeRK::ReadOnlyPropertyException::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "exception TeRK::ReadOnlyPropertyException was not generated with stream support";
    throw ex;
}

void
TeRK::ReadOnlyPropertyException::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "exception TeRK::ReadOnlyPropertyException was not generated with stream support";
    throw ex;
}

struct __F__TeRK__ReadOnlyPropertyException : public ::IceInternal::UserExceptionFactory
{
    virtual void
    createAndThrow()
    {
	throw ::TeRK::ReadOnlyPropertyException();
    }
};

static ::IceInternal::UserExceptionFactoryPtr __F__TeRK__ReadOnlyPropertyException__Ptr = new __F__TeRK__ReadOnlyPropertyException;

const ::IceInternal::UserExceptionFactoryPtr&
TeRK::ReadOnlyPropertyException::ice_factory()
{
    return __F__TeRK__ReadOnlyPropertyException__Ptr;
}

class __F__TeRK__ReadOnlyPropertyException__Init
{
public:

    __F__TeRK__ReadOnlyPropertyException__Init()
    {
	::IceInternal::factoryTable->addExceptionFactory("::TeRK::ReadOnlyPropertyException", ::TeRK::ReadOnlyPropertyException::ice_factory());
    }

    ~__F__TeRK__ReadOnlyPropertyException__Init()
    {
	::IceInternal::factoryTable->removeExceptionFactory("::TeRK::ReadOnlyPropertyException");
    }
};

static __F__TeRK__ReadOnlyPropertyException__Init __F__TeRK__ReadOnlyPropertyException__i;

#ifdef __APPLE__
extern "C" { void __F__TeRK__ReadOnlyPropertyException__initializer() {} }
#endif

TeRK::CommandException::CommandException(const ::std::string& __ice_reason) :
#if (defined(_MSC_VER) && (_MSC_VER < 1300)) // VC++ 6 compiler bug
    TeRKException(__ice_reason)
#else
    ::TeRK::TeRKException(__ice_reason)
#endif
{
}

static const char* __TeRK__CommandException_name = "TeRK::CommandException";

const ::std::string
TeRK::CommandException::ice_name() const
{
    return __TeRK__CommandException_name;
}

::Ice::Exception*
TeRK::CommandException::ice_clone() const
{
    return new CommandException(*this);
}

void
TeRK::CommandException::ice_throw() const
{
    throw *this;
}

void
TeRK::CommandException::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(::std::string("::TeRK::CommandException"), false);
    __os->startWriteSlice();
    __os->endWriteSlice();
#if (defined(_MSC_VER) && (_MSC_VER < 1300)) // VC++ 6 compiler bug
    TeRKException::__write(__os);
#else
    ::TeRK::TeRKException::__write(__os);
#endif
}

void
TeRK::CommandException::__read(::IceInternal::BasicStream* __is, bool __rid)
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
TeRK::CommandException::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "exception TeRK::CommandException was not generated with stream support";
    throw ex;
}

void
TeRK::CommandException::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "exception TeRK::CommandException was not generated with stream support";
    throw ex;
}

struct __F__TeRK__CommandException : public ::IceInternal::UserExceptionFactory
{
    virtual void
    createAndThrow()
    {
	throw ::TeRK::CommandException();
    }
};

static ::IceInternal::UserExceptionFactoryPtr __F__TeRK__CommandException__Ptr = new __F__TeRK__CommandException;

const ::IceInternal::UserExceptionFactoryPtr&
TeRK::CommandException::ice_factory()
{
    return __F__TeRK__CommandException__Ptr;
}

class __F__TeRK__CommandException__Init
{
public:

    __F__TeRK__CommandException__Init()
    {
	::IceInternal::factoryTable->addExceptionFactory("::TeRK::CommandException", ::TeRK::CommandException::ice_factory());
    }

    ~__F__TeRK__CommandException__Init()
    {
	::IceInternal::factoryTable->removeExceptionFactory("::TeRK::CommandException");
    }
};

static __F__TeRK__CommandException__Init __F__TeRK__CommandException__i;

#ifdef __APPLE__
extern "C" { void __F__TeRK__CommandException__initializer() {} }
#endif

TeRK::AudioCommandException::AudioCommandException(const ::std::string& __ice_reason) :
#if (defined(_MSC_VER) && (_MSC_VER < 1300)) // VC++ 6 compiler bug
    CommandException(__ice_reason)
#else
    ::TeRK::CommandException(__ice_reason)
#endif
{
}

static const char* __TeRK__AudioCommandException_name = "TeRK::AudioCommandException";

const ::std::string
TeRK::AudioCommandException::ice_name() const
{
    return __TeRK__AudioCommandException_name;
}

::Ice::Exception*
TeRK::AudioCommandException::ice_clone() const
{
    return new AudioCommandException(*this);
}

void
TeRK::AudioCommandException::ice_throw() const
{
    throw *this;
}

void
TeRK::AudioCommandException::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(::std::string("::TeRK::AudioCommandException"), false);
    __os->startWriteSlice();
    __os->endWriteSlice();
#if (defined(_MSC_VER) && (_MSC_VER < 1300)) // VC++ 6 compiler bug
    CommandException::__write(__os);
#else
    ::TeRK::CommandException::__write(__os);
#endif
}

void
TeRK::AudioCommandException::__read(::IceInternal::BasicStream* __is, bool __rid)
{
    if(__rid)
    {
	::std::string myId;
	__is->read(myId, false);
    }
    __is->startReadSlice();
    __is->endReadSlice();
#if (defined(_MSC_VER) && (_MSC_VER < 1300)) // VC++ 6 compiler bug
    CommandException::__read(__is, true);
#else
    ::TeRK::CommandException::__read(__is, true);
#endif
}

void
TeRK::AudioCommandException::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "exception TeRK::AudioCommandException was not generated with stream support";
    throw ex;
}

void
TeRK::AudioCommandException::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "exception TeRK::AudioCommandException was not generated with stream support";
    throw ex;
}

struct __F__TeRK__AudioCommandException : public ::IceInternal::UserExceptionFactory
{
    virtual void
    createAndThrow()
    {
	throw ::TeRK::AudioCommandException();
    }
};

static ::IceInternal::UserExceptionFactoryPtr __F__TeRK__AudioCommandException__Ptr = new __F__TeRK__AudioCommandException;

const ::IceInternal::UserExceptionFactoryPtr&
TeRK::AudioCommandException::ice_factory()
{
    return __F__TeRK__AudioCommandException__Ptr;
}

class __F__TeRK__AudioCommandException__Init
{
public:

    __F__TeRK__AudioCommandException__Init()
    {
	::IceInternal::factoryTable->addExceptionFactory("::TeRK::AudioCommandException", ::TeRK::AudioCommandException::ice_factory());
    }

    ~__F__TeRK__AudioCommandException__Init()
    {
	::IceInternal::factoryTable->removeExceptionFactory("::TeRK::AudioCommandException");
    }
};

static __F__TeRK__AudioCommandException__Init __F__TeRK__AudioCommandException__i;

#ifdef __APPLE__
extern "C" { void __F__TeRK__AudioCommandException__initializer() {} }
#endif

TeRK::AudioCommandQueueFullException::AudioCommandQueueFullException(const ::std::string& __ice_reason) :
#if (defined(_MSC_VER) && (_MSC_VER < 1300)) // VC++ 6 compiler bug
    AudioCommandException(__ice_reason)
#else
    ::TeRK::AudioCommandException(__ice_reason)
#endif
{
}

static const char* __TeRK__AudioCommandQueueFullException_name = "TeRK::AudioCommandQueueFullException";

const ::std::string
TeRK::AudioCommandQueueFullException::ice_name() const
{
    return __TeRK__AudioCommandQueueFullException_name;
}

::Ice::Exception*
TeRK::AudioCommandQueueFullException::ice_clone() const
{
    return new AudioCommandQueueFullException(*this);
}

void
TeRK::AudioCommandQueueFullException::ice_throw() const
{
    throw *this;
}

void
TeRK::AudioCommandQueueFullException::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(::std::string("::TeRK::AudioCommandQueueFullException"), false);
    __os->startWriteSlice();
    __os->endWriteSlice();
#if (defined(_MSC_VER) && (_MSC_VER < 1300)) // VC++ 6 compiler bug
    AudioCommandException::__write(__os);
#else
    ::TeRK::AudioCommandException::__write(__os);
#endif
}

void
TeRK::AudioCommandQueueFullException::__read(::IceInternal::BasicStream* __is, bool __rid)
{
    if(__rid)
    {
	::std::string myId;
	__is->read(myId, false);
    }
    __is->startReadSlice();
    __is->endReadSlice();
#if (defined(_MSC_VER) && (_MSC_VER < 1300)) // VC++ 6 compiler bug
    AudioCommandException::__read(__is, true);
#else
    ::TeRK::AudioCommandException::__read(__is, true);
#endif
}

void
TeRK::AudioCommandQueueFullException::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "exception TeRK::AudioCommandQueueFullException was not generated with stream support";
    throw ex;
}

void
TeRK::AudioCommandQueueFullException::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "exception TeRK::AudioCommandQueueFullException was not generated with stream support";
    throw ex;
}

struct __F__TeRK__AudioCommandQueueFullException : public ::IceInternal::UserExceptionFactory
{
    virtual void
    createAndThrow()
    {
	throw ::TeRK::AudioCommandQueueFullException();
    }
};

static ::IceInternal::UserExceptionFactoryPtr __F__TeRK__AudioCommandQueueFullException__Ptr = new __F__TeRK__AudioCommandQueueFullException;

const ::IceInternal::UserExceptionFactoryPtr&
TeRK::AudioCommandQueueFullException::ice_factory()
{
    return __F__TeRK__AudioCommandQueueFullException__Ptr;
}

class __F__TeRK__AudioCommandQueueFullException__Init
{
public:

    __F__TeRK__AudioCommandQueueFullException__Init()
    {
	::IceInternal::factoryTable->addExceptionFactory("::TeRK::AudioCommandQueueFullException", ::TeRK::AudioCommandQueueFullException::ice_factory());
    }

    ~__F__TeRK__AudioCommandQueueFullException__Init()
    {
	::IceInternal::factoryTable->removeExceptionFactory("::TeRK::AudioCommandQueueFullException");
    }
};

static __F__TeRK__AudioCommandQueueFullException__Init __F__TeRK__AudioCommandQueueFullException__i;

#ifdef __APPLE__
extern "C" { void __F__TeRK__AudioCommandQueueFullException__initializer() {} }
#endif

TeRK::AudioFileTooLargeException::AudioFileTooLargeException(const ::std::string& __ice_reason) :
#if (defined(_MSC_VER) && (_MSC_VER < 1300)) // VC++ 6 compiler bug
    AudioCommandException(__ice_reason)
#else
    ::TeRK::AudioCommandException(__ice_reason)
#endif
{
}

static const char* __TeRK__AudioFileTooLargeException_name = "TeRK::AudioFileTooLargeException";

const ::std::string
TeRK::AudioFileTooLargeException::ice_name() const
{
    return __TeRK__AudioFileTooLargeException_name;
}

::Ice::Exception*
TeRK::AudioFileTooLargeException::ice_clone() const
{
    return new AudioFileTooLargeException(*this);
}

void
TeRK::AudioFileTooLargeException::ice_throw() const
{
    throw *this;
}

void
TeRK::AudioFileTooLargeException::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(::std::string("::TeRK::AudioFileTooLargeException"), false);
    __os->startWriteSlice();
    __os->endWriteSlice();
#if (defined(_MSC_VER) && (_MSC_VER < 1300)) // VC++ 6 compiler bug
    AudioCommandException::__write(__os);
#else
    ::TeRK::AudioCommandException::__write(__os);
#endif
}

void
TeRK::AudioFileTooLargeException::__read(::IceInternal::BasicStream* __is, bool __rid)
{
    if(__rid)
    {
	::std::string myId;
	__is->read(myId, false);
    }
    __is->startReadSlice();
    __is->endReadSlice();
#if (defined(_MSC_VER) && (_MSC_VER < 1300)) // VC++ 6 compiler bug
    AudioCommandException::__read(__is, true);
#else
    ::TeRK::AudioCommandException::__read(__is, true);
#endif
}

void
TeRK::AudioFileTooLargeException::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "exception TeRK::AudioFileTooLargeException was not generated with stream support";
    throw ex;
}

void
TeRK::AudioFileTooLargeException::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "exception TeRK::AudioFileTooLargeException was not generated with stream support";
    throw ex;
}

struct __F__TeRK__AudioFileTooLargeException : public ::IceInternal::UserExceptionFactory
{
    virtual void
    createAndThrow()
    {
	throw ::TeRK::AudioFileTooLargeException();
    }
};

static ::IceInternal::UserExceptionFactoryPtr __F__TeRK__AudioFileTooLargeException__Ptr = new __F__TeRK__AudioFileTooLargeException;

const ::IceInternal::UserExceptionFactoryPtr&
TeRK::AudioFileTooLargeException::ice_factory()
{
    return __F__TeRK__AudioFileTooLargeException__Ptr;
}

class __F__TeRK__AudioFileTooLargeException__Init
{
public:

    __F__TeRK__AudioFileTooLargeException__Init()
    {
	::IceInternal::factoryTable->addExceptionFactory("::TeRK::AudioFileTooLargeException", ::TeRK::AudioFileTooLargeException::ice_factory());
    }

    ~__F__TeRK__AudioFileTooLargeException__Init()
    {
	::IceInternal::factoryTable->removeExceptionFactory("::TeRK::AudioFileTooLargeException");
    }
};

static __F__TeRK__AudioFileTooLargeException__Init __F__TeRK__AudioFileTooLargeException__i;

#ifdef __APPLE__
extern "C" { void __F__TeRK__AudioFileTooLargeException__initializer() {} }
#endif

TeRK::DigitalOutCommandException::DigitalOutCommandException(const ::std::string& __ice_reason) :
#if (defined(_MSC_VER) && (_MSC_VER < 1300)) // VC++ 6 compiler bug
    CommandException(__ice_reason)
#else
    ::TeRK::CommandException(__ice_reason)
#endif
{
}

static const char* __TeRK__DigitalOutCommandException_name = "TeRK::DigitalOutCommandException";

const ::std::string
TeRK::DigitalOutCommandException::ice_name() const
{
    return __TeRK__DigitalOutCommandException_name;
}

::Ice::Exception*
TeRK::DigitalOutCommandException::ice_clone() const
{
    return new DigitalOutCommandException(*this);
}

void
TeRK::DigitalOutCommandException::ice_throw() const
{
    throw *this;
}

void
TeRK::DigitalOutCommandException::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(::std::string("::TeRK::DigitalOutCommandException"), false);
    __os->startWriteSlice();
    __os->endWriteSlice();
#if (defined(_MSC_VER) && (_MSC_VER < 1300)) // VC++ 6 compiler bug
    CommandException::__write(__os);
#else
    ::TeRK::CommandException::__write(__os);
#endif
}

void
TeRK::DigitalOutCommandException::__read(::IceInternal::BasicStream* __is, bool __rid)
{
    if(__rid)
    {
	::std::string myId;
	__is->read(myId, false);
    }
    __is->startReadSlice();
    __is->endReadSlice();
#if (defined(_MSC_VER) && (_MSC_VER < 1300)) // VC++ 6 compiler bug
    CommandException::__read(__is, true);
#else
    ::TeRK::CommandException::__read(__is, true);
#endif
}

void
TeRK::DigitalOutCommandException::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "exception TeRK::DigitalOutCommandException was not generated with stream support";
    throw ex;
}

void
TeRK::DigitalOutCommandException::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "exception TeRK::DigitalOutCommandException was not generated with stream support";
    throw ex;
}

struct __F__TeRK__DigitalOutCommandException : public ::IceInternal::UserExceptionFactory
{
    virtual void
    createAndThrow()
    {
	throw ::TeRK::DigitalOutCommandException();
    }
};

static ::IceInternal::UserExceptionFactoryPtr __F__TeRK__DigitalOutCommandException__Ptr = new __F__TeRK__DigitalOutCommandException;

const ::IceInternal::UserExceptionFactoryPtr&
TeRK::DigitalOutCommandException::ice_factory()
{
    return __F__TeRK__DigitalOutCommandException__Ptr;
}

class __F__TeRK__DigitalOutCommandException__Init
{
public:

    __F__TeRK__DigitalOutCommandException__Init()
    {
	::IceInternal::factoryTable->addExceptionFactory("::TeRK::DigitalOutCommandException", ::TeRK::DigitalOutCommandException::ice_factory());
    }

    ~__F__TeRK__DigitalOutCommandException__Init()
    {
	::IceInternal::factoryTable->removeExceptionFactory("::TeRK::DigitalOutCommandException");
    }
};

static __F__TeRK__DigitalOutCommandException__Init __F__TeRK__DigitalOutCommandException__i;

#ifdef __APPLE__
extern "C" { void __F__TeRK__DigitalOutCommandException__initializer() {} }
#endif

TeRK::LEDCommandException::LEDCommandException(const ::std::string& __ice_reason) :
#if (defined(_MSC_VER) && (_MSC_VER < 1300)) // VC++ 6 compiler bug
    CommandException(__ice_reason)
#else
    ::TeRK::CommandException(__ice_reason)
#endif
{
}

static const char* __TeRK__LEDCommandException_name = "TeRK::LEDCommandException";

const ::std::string
TeRK::LEDCommandException::ice_name() const
{
    return __TeRK__LEDCommandException_name;
}

::Ice::Exception*
TeRK::LEDCommandException::ice_clone() const
{
    return new LEDCommandException(*this);
}

void
TeRK::LEDCommandException::ice_throw() const
{
    throw *this;
}

void
TeRK::LEDCommandException::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(::std::string("::TeRK::LEDCommandException"), false);
    __os->startWriteSlice();
    __os->endWriteSlice();
#if (defined(_MSC_VER) && (_MSC_VER < 1300)) // VC++ 6 compiler bug
    CommandException::__write(__os);
#else
    ::TeRK::CommandException::__write(__os);
#endif
}

void
TeRK::LEDCommandException::__read(::IceInternal::BasicStream* __is, bool __rid)
{
    if(__rid)
    {
	::std::string myId;
	__is->read(myId, false);
    }
    __is->startReadSlice();
    __is->endReadSlice();
#if (defined(_MSC_VER) && (_MSC_VER < 1300)) // VC++ 6 compiler bug
    CommandException::__read(__is, true);
#else
    ::TeRK::CommandException::__read(__is, true);
#endif
}

void
TeRK::LEDCommandException::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "exception TeRK::LEDCommandException was not generated with stream support";
    throw ex;
}

void
TeRK::LEDCommandException::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "exception TeRK::LEDCommandException was not generated with stream support";
    throw ex;
}

struct __F__TeRK__LEDCommandException : public ::IceInternal::UserExceptionFactory
{
    virtual void
    createAndThrow()
    {
	throw ::TeRK::LEDCommandException();
    }
};

static ::IceInternal::UserExceptionFactoryPtr __F__TeRK__LEDCommandException__Ptr = new __F__TeRK__LEDCommandException;

const ::IceInternal::UserExceptionFactoryPtr&
TeRK::LEDCommandException::ice_factory()
{
    return __F__TeRK__LEDCommandException__Ptr;
}

class __F__TeRK__LEDCommandException__Init
{
public:

    __F__TeRK__LEDCommandException__Init()
    {
	::IceInternal::factoryTable->addExceptionFactory("::TeRK::LEDCommandException", ::TeRK::LEDCommandException::ice_factory());
    }

    ~__F__TeRK__LEDCommandException__Init()
    {
	::IceInternal::factoryTable->removeExceptionFactory("::TeRK::LEDCommandException");
    }
};

static __F__TeRK__LEDCommandException__Init __F__TeRK__LEDCommandException__i;

#ifdef __APPLE__
extern "C" { void __F__TeRK__LEDCommandException__initializer() {} }
#endif

TeRK::MotorCommandException::MotorCommandException(const ::std::string& __ice_reason) :
#if (defined(_MSC_VER) && (_MSC_VER < 1300)) // VC++ 6 compiler bug
    CommandException(__ice_reason)
#else
    ::TeRK::CommandException(__ice_reason)
#endif
{
}

static const char* __TeRK__MotorCommandException_name = "TeRK::MotorCommandException";

const ::std::string
TeRK::MotorCommandException::ice_name() const
{
    return __TeRK__MotorCommandException_name;
}

::Ice::Exception*
TeRK::MotorCommandException::ice_clone() const
{
    return new MotorCommandException(*this);
}

void
TeRK::MotorCommandException::ice_throw() const
{
    throw *this;
}

void
TeRK::MotorCommandException::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(::std::string("::TeRK::MotorCommandException"), false);
    __os->startWriteSlice();
    __os->endWriteSlice();
#if (defined(_MSC_VER) && (_MSC_VER < 1300)) // VC++ 6 compiler bug
    CommandException::__write(__os);
#else
    ::TeRK::CommandException::__write(__os);
#endif
}

void
TeRK::MotorCommandException::__read(::IceInternal::BasicStream* __is, bool __rid)
{
    if(__rid)
    {
	::std::string myId;
	__is->read(myId, false);
    }
    __is->startReadSlice();
    __is->endReadSlice();
#if (defined(_MSC_VER) && (_MSC_VER < 1300)) // VC++ 6 compiler bug
    CommandException::__read(__is, true);
#else
    ::TeRK::CommandException::__read(__is, true);
#endif
}

void
TeRK::MotorCommandException::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "exception TeRK::MotorCommandException was not generated with stream support";
    throw ex;
}

void
TeRK::MotorCommandException::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "exception TeRK::MotorCommandException was not generated with stream support";
    throw ex;
}

struct __F__TeRK__MotorCommandException : public ::IceInternal::UserExceptionFactory
{
    virtual void
    createAndThrow()
    {
	throw ::TeRK::MotorCommandException();
    }
};

static ::IceInternal::UserExceptionFactoryPtr __F__TeRK__MotorCommandException__Ptr = new __F__TeRK__MotorCommandException;

const ::IceInternal::UserExceptionFactoryPtr&
TeRK::MotorCommandException::ice_factory()
{
    return __F__TeRK__MotorCommandException__Ptr;
}

class __F__TeRK__MotorCommandException__Init
{
public:

    __F__TeRK__MotorCommandException__Init()
    {
	::IceInternal::factoryTable->addExceptionFactory("::TeRK::MotorCommandException", ::TeRK::MotorCommandException::ice_factory());
    }

    ~__F__TeRK__MotorCommandException__Init()
    {
	::IceInternal::factoryTable->removeExceptionFactory("::TeRK::MotorCommandException");
    }
};

static __F__TeRK__MotorCommandException__Init __F__TeRK__MotorCommandException__i;

#ifdef __APPLE__
extern "C" { void __F__TeRK__MotorCommandException__initializer() {} }
#endif

TeRK::ServoCommandException::ServoCommandException(const ::std::string& __ice_reason) :
#if (defined(_MSC_VER) && (_MSC_VER < 1300)) // VC++ 6 compiler bug
    CommandException(__ice_reason)
#else
    ::TeRK::CommandException(__ice_reason)
#endif
{
}

static const char* __TeRK__ServoCommandException_name = "TeRK::ServoCommandException";

const ::std::string
TeRK::ServoCommandException::ice_name() const
{
    return __TeRK__ServoCommandException_name;
}

::Ice::Exception*
TeRK::ServoCommandException::ice_clone() const
{
    return new ServoCommandException(*this);
}

void
TeRK::ServoCommandException::ice_throw() const
{
    throw *this;
}

void
TeRK::ServoCommandException::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(::std::string("::TeRK::ServoCommandException"), false);
    __os->startWriteSlice();
    __os->endWriteSlice();
#if (defined(_MSC_VER) && (_MSC_VER < 1300)) // VC++ 6 compiler bug
    CommandException::__write(__os);
#else
    ::TeRK::CommandException::__write(__os);
#endif
}

void
TeRK::ServoCommandException::__read(::IceInternal::BasicStream* __is, bool __rid)
{
    if(__rid)
    {
	::std::string myId;
	__is->read(myId, false);
    }
    __is->startReadSlice();
    __is->endReadSlice();
#if (defined(_MSC_VER) && (_MSC_VER < 1300)) // VC++ 6 compiler bug
    CommandException::__read(__is, true);
#else
    ::TeRK::CommandException::__read(__is, true);
#endif
}

void
TeRK::ServoCommandException::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "exception TeRK::ServoCommandException was not generated with stream support";
    throw ex;
}

void
TeRK::ServoCommandException::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "exception TeRK::ServoCommandException was not generated with stream support";
    throw ex;
}

struct __F__TeRK__ServoCommandException : public ::IceInternal::UserExceptionFactory
{
    virtual void
    createAndThrow()
    {
	throw ::TeRK::ServoCommandException();
    }
};

static ::IceInternal::UserExceptionFactoryPtr __F__TeRK__ServoCommandException__Ptr = new __F__TeRK__ServoCommandException;

const ::IceInternal::UserExceptionFactoryPtr&
TeRK::ServoCommandException::ice_factory()
{
    return __F__TeRK__ServoCommandException__Ptr;
}

class __F__TeRK__ServoCommandException__Init
{
public:

    __F__TeRK__ServoCommandException__Init()
    {
	::IceInternal::factoryTable->addExceptionFactory("::TeRK::ServoCommandException", ::TeRK::ServoCommandException::ice_factory());
    }

    ~__F__TeRK__ServoCommandException__Init()
    {
	::IceInternal::factoryTable->removeExceptionFactory("::TeRK::ServoCommandException");
    }
};

static __F__TeRK__ServoCommandException__Init __F__TeRK__ServoCommandException__i;

#ifdef __APPLE__
extern "C" { void __F__TeRK__ServoCommandException__initializer() {} }
#endif

bool
TeRK::MotorBuffer::operator==(const MotorBuffer& __rhs) const
{
    return !operator!=(__rhs);
}

bool
TeRK::MotorBuffer::operator!=(const MotorBuffer& __rhs) const
{
    if(this == &__rhs)
    {
	return false;
    }
    if(values != __rhs.values)
    {
	return true;
    }
    return false;
}

bool
TeRK::MotorBuffer::operator<(const MotorBuffer& __rhs) const
{
    if(this == &__rhs)
    {
	return false;
    }
    if(values < __rhs.values)
    {
	return true;
    }
    else if(__rhs.values < values)
    {
	return false;
    }
    return false;
}

void
TeRK::MotorBuffer::__write(::IceInternal::BasicStream* __os) const
{
    if(values.size() == 0)
    {
	__os->writeSize(0);
    }
    else
    {
	__os->write(&values[0], &values[0] + values.size());
    }
}

void
TeRK::MotorBuffer::__read(::IceInternal::BasicStream* __is)
{
    __is->read(values);
}

void
TeRK::__write(::IceInternal::BasicStream* __os, const ::TeRK::MotorBuffer* begin, const ::TeRK::MotorBuffer* end, ::TeRK::__U__MotorBufferArray)
{
    ::Ice::Int size = static_cast< ::Ice::Int>(end - begin);
    __os->writeSize(size);
    for(int i = 0; i < size; ++i)
    {
	begin[i].__write(__os);
    }
}

void
TeRK::__read(::IceInternal::BasicStream* __is, ::TeRK::MotorBufferArray& v, ::TeRK::__U__MotorBufferArray)
{
    ::Ice::Int sz;
    __is->readSize(sz);
    __is->startSeq(sz, 1);
    v.resize(sz);
    for(int i = 0; i < sz; ++i)
    {
	v[i].__read(__is);
	__is->checkSeq();
	__is->endElement();
    }
    __is->endSeq(sz);
}

bool
TeRK::Image::operator==(const Image& __rhs) const
{
    return !operator!=(__rhs);
}

bool
TeRK::Image::operator!=(const Image& __rhs) const
{
    if(this == &__rhs)
    {
	return false;
    }
    if(height != __rhs.height)
    {
	return true;
    }
    if(width != __rhs.width)
    {
	return true;
    }
    if(frameNum != __rhs.frameNum)
    {
	return true;
    }
    if(format != __rhs.format)
    {
	return true;
    }
    if(data != __rhs.data)
    {
	return true;
    }
    return false;
}

bool
TeRK::Image::operator<(const Image& __rhs) const
{
    if(this == &__rhs)
    {
	return false;
    }
    if(height < __rhs.height)
    {
	return true;
    }
    else if(__rhs.height < height)
    {
	return false;
    }
    if(width < __rhs.width)
    {
	return true;
    }
    else if(__rhs.width < width)
    {
	return false;
    }
    if(frameNum < __rhs.frameNum)
    {
	return true;
    }
    else if(__rhs.frameNum < frameNum)
    {
	return false;
    }
    if(format < __rhs.format)
    {
	return true;
    }
    else if(__rhs.format < format)
    {
	return false;
    }
    if(data < __rhs.data)
    {
	return true;
    }
    else if(__rhs.data < data)
    {
	return false;
    }
    return false;
}

void
TeRK::Image::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(height);
    __os->write(width);
    __os->write(frameNum);
    ::TeRK::__write(__os, format);
    if(data.size() == 0)
    {
	__os->writeSize(0);
    }
    else
    {
	__os->write(&data[0], &data[0] + data.size());
    }
}

void
TeRK::Image::__read(::IceInternal::BasicStream* __is)
{
    __is->read(height);
    __is->read(width);
    __is->read(frameNum);
    ::TeRK::__read(__is, format);
    ::std::pair<const ::Ice::Byte*, const ::Ice::Byte*> ___data;
    __is->read(___data);
    ::std::vector< ::Ice::Byte>(___data.first, ___data.second).swap(data);
}

bool
TeRK::AnalogInState::operator==(const AnalogInState& __rhs) const
{
    return !operator!=(__rhs);
}

bool
TeRK::AnalogInState::operator!=(const AnalogInState& __rhs) const
{
    if(this == &__rhs)
    {
	return false;
    }
    if(analogInValues != __rhs.analogInValues)
    {
	return true;
    }
    return false;
}

bool
TeRK::AnalogInState::operator<(const AnalogInState& __rhs) const
{
    if(this == &__rhs)
    {
	return false;
    }
    if(analogInValues < __rhs.analogInValues)
    {
	return true;
    }
    else if(__rhs.analogInValues < analogInValues)
    {
	return false;
    }
    return false;
}

void
TeRK::AnalogInState::__write(::IceInternal::BasicStream* __os) const
{
    if(analogInValues.size() == 0)
    {
	__os->writeSize(0);
    }
    else
    {
	__os->write(&analogInValues[0], &analogInValues[0] + analogInValues.size());
    }
}

void
TeRK::AnalogInState::__read(::IceInternal::BasicStream* __is)
{
    __is->read(analogInValues);
}

bool
TeRK::BatteryState::operator==(const BatteryState& __rhs) const
{
    return !operator!=(__rhs);
}

bool
TeRK::BatteryState::operator!=(const BatteryState& __rhs) const
{
    if(this == &__rhs)
    {
	return false;
    }
    if(batteryVoltage != __rhs.batteryVoltage)
    {
	return true;
    }
    return false;
}

bool
TeRK::BatteryState::operator<(const BatteryState& __rhs) const
{
    if(this == &__rhs)
    {
	return false;
    }
    if(batteryVoltage < __rhs.batteryVoltage)
    {
	return true;
    }
    else if(__rhs.batteryVoltage < batteryVoltage)
    {
	return false;
    }
    return false;
}

void
TeRK::BatteryState::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(batteryVoltage);
}

void
TeRK::BatteryState::__read(::IceInternal::BasicStream* __is)
{
    __is->read(batteryVoltage);
}

bool
TeRK::ButtonState::operator==(const ButtonState& __rhs) const
{
    return !operator!=(__rhs);
}

bool
TeRK::ButtonState::operator!=(const ButtonState& __rhs) const
{
    if(this == &__rhs)
    {
	return false;
    }
    if(buttonStates != __rhs.buttonStates)
    {
	return true;
    }
    return false;
}

bool
TeRK::ButtonState::operator<(const ButtonState& __rhs) const
{
    if(this == &__rhs)
    {
	return false;
    }
    if(buttonStates < __rhs.buttonStates)
    {
	return true;
    }
    else if(__rhs.buttonStates < buttonStates)
    {
	return false;
    }
    return false;
}

void
TeRK::ButtonState::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(buttonStates);
}

void
TeRK::ButtonState::__read(::IceInternal::BasicStream* __is)
{
    __is->read(buttonStates);
}

bool
TeRK::DigitalInState::operator==(const DigitalInState& __rhs) const
{
    return !operator!=(__rhs);
}

bool
TeRK::DigitalInState::operator!=(const DigitalInState& __rhs) const
{
    if(this == &__rhs)
    {
	return false;
    }
    if(digitalInStates != __rhs.digitalInStates)
    {
	return true;
    }
    return false;
}

bool
TeRK::DigitalInState::operator<(const DigitalInState& __rhs) const
{
    if(this == &__rhs)
    {
	return false;
    }
    if(digitalInStates < __rhs.digitalInStates)
    {
	return true;
    }
    else if(__rhs.digitalInStates < digitalInStates)
    {
	return false;
    }
    return false;
}

void
TeRK::DigitalInState::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(digitalInStates);
}

void
TeRK::DigitalInState::__read(::IceInternal::BasicStream* __is)
{
    __is->read(digitalInStates);
}

bool
TeRK::MotorState::operator==(const MotorState& __rhs) const
{
    return !operator!=(__rhs);
}

bool
TeRK::MotorState::operator!=(const MotorState& __rhs) const
{
    if(this == &__rhs)
    {
	return false;
    }
    if(motorVelocities != __rhs.motorVelocities)
    {
	return true;
    }
    if(motorPositions != __rhs.motorPositions)
    {
	return true;
    }
    if(motorCurrents != __rhs.motorCurrents)
    {
	return true;
    }
    if(motorDutyCycles != __rhs.motorDutyCycles)
    {
	return true;
    }
    if(motorDone != __rhs.motorDone)
    {
	return true;
    }
    return false;
}

bool
TeRK::MotorState::operator<(const MotorState& __rhs) const
{
    if(this == &__rhs)
    {
	return false;
    }
    if(motorVelocities < __rhs.motorVelocities)
    {
	return true;
    }
    else if(__rhs.motorVelocities < motorVelocities)
    {
	return false;
    }
    if(motorPositions < __rhs.motorPositions)
    {
	return true;
    }
    else if(__rhs.motorPositions < motorPositions)
    {
	return false;
    }
    if(motorCurrents < __rhs.motorCurrents)
    {
	return true;
    }
    else if(__rhs.motorCurrents < motorCurrents)
    {
	return false;
    }
    if(motorDutyCycles < __rhs.motorDutyCycles)
    {
	return true;
    }
    else if(__rhs.motorDutyCycles < motorDutyCycles)
    {
	return false;
    }
    if(motorDone < __rhs.motorDone)
    {
	return true;
    }
    else if(__rhs.motorDone < motorDone)
    {
	return false;
    }
    return false;
}

void
TeRK::MotorState::__write(::IceInternal::BasicStream* __os) const
{
    if(motorVelocities.size() == 0)
    {
	__os->writeSize(0);
    }
    else
    {
	__os->write(&motorVelocities[0], &motorVelocities[0] + motorVelocities.size());
    }
    if(motorPositions.size() == 0)
    {
	__os->writeSize(0);
    }
    else
    {
	__os->write(&motorPositions[0], &motorPositions[0] + motorPositions.size());
    }
    if(motorCurrents.size() == 0)
    {
	__os->writeSize(0);
    }
    else
    {
	__os->write(&motorCurrents[0], &motorCurrents[0] + motorCurrents.size());
    }
    if(motorDutyCycles.size() == 0)
    {
	__os->writeSize(0);
    }
    else
    {
	__os->write(&motorDutyCycles[0], &motorDutyCycles[0] + motorDutyCycles.size());
    }
    __os->write(motorDone);
}

void
TeRK::MotorState::__read(::IceInternal::BasicStream* __is)
{
    __is->read(motorVelocities);
    __is->read(motorPositions);
    __is->read(motorCurrents);
    __is->read(motorDutyCycles);
    __is->read(motorDone);
}

bool
TeRK::ServoState::operator==(const ServoState& __rhs) const
{
    return !operator!=(__rhs);
}

bool
TeRK::ServoState::operator!=(const ServoState& __rhs) const
{
    if(this == &__rhs)
    {
	return false;
    }
    if(servoPositions != __rhs.servoPositions)
    {
	return true;
    }
    return false;
}

bool
TeRK::ServoState::operator<(const ServoState& __rhs) const
{
    if(this == &__rhs)
    {
	return false;
    }
    if(servoPositions < __rhs.servoPositions)
    {
	return true;
    }
    else if(__rhs.servoPositions < servoPositions)
    {
	return false;
    }
    return false;
}

void
TeRK::ServoState::__write(::IceInternal::BasicStream* __os) const
{
    if(servoPositions.size() == 0)
    {
	__os->writeSize(0);
    }
    else
    {
	__os->write(&servoPositions[0], &servoPositions[0] + servoPositions.size());
    }
}

void
TeRK::ServoState::__read(::IceInternal::BasicStream* __is)
{
    __is->read(servoPositions);
}

bool
TeRK::QwerkState::operator==(const QwerkState& __rhs) const
{
    return !operator!=(__rhs);
}

bool
TeRK::QwerkState::operator!=(const QwerkState& __rhs) const
{
    if(this == &__rhs)
    {
	return false;
    }
    if(analogIn != __rhs.analogIn)
    {
	return true;
    }
    if(battery != __rhs.battery)
    {
	return true;
    }
    if(button != __rhs.button)
    {
	return true;
    }
    if(digitalIn != __rhs.digitalIn)
    {
	return true;
    }
    if(motor != __rhs.motor)
    {
	return true;
    }
    if(servo != __rhs.servo)
    {
	return true;
    }
    return false;
}

bool
TeRK::QwerkState::operator<(const QwerkState& __rhs) const
{
    if(this == &__rhs)
    {
	return false;
    }
    if(analogIn < __rhs.analogIn)
    {
	return true;
    }
    else if(__rhs.analogIn < analogIn)
    {
	return false;
    }
    if(battery < __rhs.battery)
    {
	return true;
    }
    else if(__rhs.battery < battery)
    {
	return false;
    }
    if(button < __rhs.button)
    {
	return true;
    }
    else if(__rhs.button < button)
    {
	return false;
    }
    if(digitalIn < __rhs.digitalIn)
    {
	return true;
    }
    else if(__rhs.digitalIn < digitalIn)
    {
	return false;
    }
    if(motor < __rhs.motor)
    {
	return true;
    }
    else if(__rhs.motor < motor)
    {
	return false;
    }
    if(servo < __rhs.servo)
    {
	return true;
    }
    else if(__rhs.servo < servo)
    {
	return false;
    }
    return false;
}

void
TeRK::QwerkState::__write(::IceInternal::BasicStream* __os) const
{
    analogIn.__write(__os);
    battery.__write(__os);
    button.__write(__os);
    digitalIn.__write(__os);
    motor.__write(__os);
    servo.__write(__os);
}

void
TeRK::QwerkState::__read(::IceInternal::BasicStream* __is)
{
    analogIn.__read(__is);
    battery.__read(__is);
    button.__read(__is);
    digitalIn.__read(__is);
    motor.__read(__is);
    servo.__read(__is);
}

bool
TeRK::AudioCommand::operator==(const AudioCommand& __rhs) const
{
    return !operator!=(__rhs);
}

bool
TeRK::AudioCommand::operator!=(const AudioCommand& __rhs) const
{
    if(this == &__rhs)
    {
	return false;
    }
    if(mode != __rhs.mode)
    {
	return true;
    }
    if(frequency != __rhs.frequency)
    {
	return true;
    }
    if(amplitude != __rhs.amplitude)
    {
	return true;
    }
    if(duration != __rhs.duration)
    {
	return true;
    }
    if(sound != __rhs.sound)
    {
	return true;
    }
    return false;
}

bool
TeRK::AudioCommand::operator<(const AudioCommand& __rhs) const
{
    if(this == &__rhs)
    {
	return false;
    }
    if(mode < __rhs.mode)
    {
	return true;
    }
    else if(__rhs.mode < mode)
    {
	return false;
    }
    if(frequency < __rhs.frequency)
    {
	return true;
    }
    else if(__rhs.frequency < frequency)
    {
	return false;
    }
    if(amplitude < __rhs.amplitude)
    {
	return true;
    }
    else if(__rhs.amplitude < amplitude)
    {
	return false;
    }
    if(duration < __rhs.duration)
    {
	return true;
    }
    else if(__rhs.duration < duration)
    {
	return false;
    }
    if(sound < __rhs.sound)
    {
	return true;
    }
    else if(__rhs.sound < sound)
    {
	return false;
    }
    return false;
}

void
TeRK::AudioCommand::__write(::IceInternal::BasicStream* __os) const
{
    ::TeRK::__write(__os, mode);
    __os->write(frequency);
    __os->write(amplitude);
    __os->write(duration);
    if(sound.size() == 0)
    {
	__os->writeSize(0);
    }
    else
    {
	__os->write(&sound[0], &sound[0] + sound.size());
    }
}

void
TeRK::AudioCommand::__read(::IceInternal::BasicStream* __is)
{
    ::TeRK::__read(__is, mode);
    __is->read(frequency);
    __is->read(amplitude);
    __is->read(duration);
    ::std::pair<const ::Ice::Byte*, const ::Ice::Byte*> ___sound;
    __is->read(___sound);
    ::std::vector< ::Ice::Byte>(___sound.first, ___sound.second).swap(sound);
}

bool
TeRK::DigitalOutCommand::operator==(const DigitalOutCommand& __rhs) const
{
    return !operator!=(__rhs);
}

bool
TeRK::DigitalOutCommand::operator!=(const DigitalOutCommand& __rhs) const
{
    if(this == &__rhs)
    {
	return false;
    }
    if(digitalOutMask != __rhs.digitalOutMask)
    {
	return true;
    }
    if(digitalOutValues != __rhs.digitalOutValues)
    {
	return true;
    }
    return false;
}

bool
TeRK::DigitalOutCommand::operator<(const DigitalOutCommand& __rhs) const
{
    if(this == &__rhs)
    {
	return false;
    }
    if(digitalOutMask < __rhs.digitalOutMask)
    {
	return true;
    }
    else if(__rhs.digitalOutMask < digitalOutMask)
    {
	return false;
    }
    if(digitalOutValues < __rhs.digitalOutValues)
    {
	return true;
    }
    else if(__rhs.digitalOutValues < digitalOutValues)
    {
	return false;
    }
    return false;
}

void
TeRK::DigitalOutCommand::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(digitalOutMask);
    __os->write(digitalOutValues);
}

void
TeRK::DigitalOutCommand::__read(::IceInternal::BasicStream* __is)
{
    __is->read(digitalOutMask);
    __is->read(digitalOutValues);
}

bool
TeRK::LEDCommand::operator==(const LEDCommand& __rhs) const
{
    return !operator!=(__rhs);
}

bool
TeRK::LEDCommand::operator!=(const LEDCommand& __rhs) const
{
    if(this == &__rhs)
    {
	return false;
    }
    if(ledMask != __rhs.ledMask)
    {
	return true;
    }
    if(ledModes != __rhs.ledModes)
    {
	return true;
    }
    return false;
}

bool
TeRK::LEDCommand::operator<(const LEDCommand& __rhs) const
{
    if(this == &__rhs)
    {
	return false;
    }
    if(ledMask < __rhs.ledMask)
    {
	return true;
    }
    else if(__rhs.ledMask < ledMask)
    {
	return false;
    }
    if(ledModes < __rhs.ledModes)
    {
	return true;
    }
    else if(__rhs.ledModes < ledModes)
    {
	return false;
    }
    return false;
}

void
TeRK::LEDCommand::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(ledMask);
    if(ledModes.size() == 0)
    {
	__os->writeSize(0);
    }
    else
    {
	::TeRK::__write(__os, &ledModes[0], &ledModes[0] + ledModes.size(), ::TeRK::__U__LEDModeArray());
    }
}

void
TeRK::LEDCommand::__read(::IceInternal::BasicStream* __is)
{
    __is->read(ledMask);
    ::TeRK::__read(__is, ledModes, ::TeRK::__U__LEDModeArray());
}

bool
TeRK::MotorCommand::operator==(const MotorCommand& __rhs) const
{
    return !operator!=(__rhs);
}

bool
TeRK::MotorCommand::operator!=(const MotorCommand& __rhs) const
{
    if(this == &__rhs)
    {
	return false;
    }
    if(motorMask != __rhs.motorMask)
    {
	return true;
    }
    if(motorModes != __rhs.motorModes)
    {
	return true;
    }
    if(motorPositions != __rhs.motorPositions)
    {
	return true;
    }
    if(motorVelocities != __rhs.motorVelocities)
    {
	return true;
    }
    if(motorAccelerations != __rhs.motorAccelerations)
    {
	return true;
    }
    return false;
}

bool
TeRK::MotorCommand::operator<(const MotorCommand& __rhs) const
{
    if(this == &__rhs)
    {
	return false;
    }
    if(motorMask < __rhs.motorMask)
    {
	return true;
    }
    else if(__rhs.motorMask < motorMask)
    {
	return false;
    }
    if(motorModes < __rhs.motorModes)
    {
	return true;
    }
    else if(__rhs.motorModes < motorModes)
    {
	return false;
    }
    if(motorPositions < __rhs.motorPositions)
    {
	return true;
    }
    else if(__rhs.motorPositions < motorPositions)
    {
	return false;
    }
    if(motorVelocities < __rhs.motorVelocities)
    {
	return true;
    }
    else if(__rhs.motorVelocities < motorVelocities)
    {
	return false;
    }
    if(motorAccelerations < __rhs.motorAccelerations)
    {
	return true;
    }
    else if(__rhs.motorAccelerations < motorAccelerations)
    {
	return false;
    }
    return false;
}

void
TeRK::MotorCommand::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(motorMask);
    if(motorModes.size() == 0)
    {
	__os->writeSize(0);
    }
    else
    {
	::TeRK::__write(__os, &motorModes[0], &motorModes[0] + motorModes.size(), ::TeRK::__U__MotorModeArray());
    }
    if(motorPositions.size() == 0)
    {
	__os->writeSize(0);
    }
    else
    {
	__os->write(&motorPositions[0], &motorPositions[0] + motorPositions.size());
    }
    if(motorVelocities.size() == 0)
    {
	__os->writeSize(0);
    }
    else
    {
	__os->write(&motorVelocities[0], &motorVelocities[0] + motorVelocities.size());
    }
    if(motorAccelerations.size() == 0)
    {
	__os->writeSize(0);
    }
    else
    {
	__os->write(&motorAccelerations[0], &motorAccelerations[0] + motorAccelerations.size());
    }
}

void
TeRK::MotorCommand::__read(::IceInternal::BasicStream* __is)
{
    __is->read(motorMask);
    ::TeRK::__read(__is, motorModes, ::TeRK::__U__MotorModeArray());
    __is->read(motorPositions);
    __is->read(motorVelocities);
    __is->read(motorAccelerations);
}

bool
TeRK::ServoCommand::operator==(const ServoCommand& __rhs) const
{
    return !operator!=(__rhs);
}

bool
TeRK::ServoCommand::operator!=(const ServoCommand& __rhs) const
{
    if(this == &__rhs)
    {
	return false;
    }
    if(servoMask != __rhs.servoMask)
    {
	return true;
    }
    if(servoModes != __rhs.servoModes)
    {
	return true;
    }
    if(servoPositions != __rhs.servoPositions)
    {
	return true;
    }
    if(servoSpeeds != __rhs.servoSpeeds)
    {
	return true;
    }
    return false;
}

bool
TeRK::ServoCommand::operator<(const ServoCommand& __rhs) const
{
    if(this == &__rhs)
    {
	return false;
    }
    if(servoMask < __rhs.servoMask)
    {
	return true;
    }
    else if(__rhs.servoMask < servoMask)
    {
	return false;
    }
    if(servoModes < __rhs.servoModes)
    {
	return true;
    }
    else if(__rhs.servoModes < servoModes)
    {
	return false;
    }
    if(servoPositions < __rhs.servoPositions)
    {
	return true;
    }
    else if(__rhs.servoPositions < servoPositions)
    {
	return false;
    }
    if(servoSpeeds < __rhs.servoSpeeds)
    {
	return true;
    }
    else if(__rhs.servoSpeeds < servoSpeeds)
    {
	return false;
    }
    return false;
}

void
TeRK::ServoCommand::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(servoMask);
    if(servoModes.size() == 0)
    {
	__os->writeSize(0);
    }
    else
    {
	::TeRK::__write(__os, &servoModes[0], &servoModes[0] + servoModes.size(), ::TeRK::__U__ServoModeArray());
    }
    if(servoPositions.size() == 0)
    {
	__os->writeSize(0);
    }
    else
    {
	__os->write(&servoPositions[0], &servoPositions[0] + servoPositions.size());
    }
    if(servoSpeeds.size() == 0)
    {
	__os->writeSize(0);
    }
    else
    {
	__os->write(&servoSpeeds[0], &servoSpeeds[0] + servoSpeeds.size());
    }
}

void
TeRK::ServoCommand::__read(::IceInternal::BasicStream* __is)
{
    __is->read(servoMask);
    ::TeRK::__read(__is, servoModes, ::TeRK::__U__ServoModeArray());
    __is->read(servoPositions);
    __is->read(servoSpeeds);
}

bool
TeRK::QwerkCommand::operator==(const QwerkCommand& __rhs) const
{
    return !operator!=(__rhs);
}

bool
TeRK::QwerkCommand::operator!=(const QwerkCommand& __rhs) const
{
    if(this == &__rhs)
    {
	return false;
    }
    if(audioCmd != __rhs.audioCmd)
    {
	return true;
    }
    if(digitalOutCmd != __rhs.digitalOutCmd)
    {
	return true;
    }
    if(ledCmd != __rhs.ledCmd)
    {
	return true;
    }
    if(motorCmd != __rhs.motorCmd)
    {
	return true;
    }
    if(servoCmd != __rhs.servoCmd)
    {
	return true;
    }
    return false;
}

bool
TeRK::QwerkCommand::operator<(const QwerkCommand& __rhs) const
{
    if(this == &__rhs)
    {
	return false;
    }
    if(audioCmd < __rhs.audioCmd)
    {
	return true;
    }
    else if(__rhs.audioCmd < audioCmd)
    {
	return false;
    }
    if(digitalOutCmd < __rhs.digitalOutCmd)
    {
	return true;
    }
    else if(__rhs.digitalOutCmd < digitalOutCmd)
    {
	return false;
    }
    if(ledCmd < __rhs.ledCmd)
    {
	return true;
    }
    else if(__rhs.ledCmd < ledCmd)
    {
	return false;
    }
    if(motorCmd < __rhs.motorCmd)
    {
	return true;
    }
    else if(__rhs.motorCmd < motorCmd)
    {
	return false;
    }
    if(servoCmd < __rhs.servoCmd)
    {
	return true;
    }
    else if(__rhs.servoCmd < servoCmd)
    {
	return false;
    }
    return false;
}

void
TeRK::QwerkCommand::__write(::IceInternal::BasicStream* __os) const
{
    audioCmd.__write(__os);
    digitalOutCmd.__write(__os);
    ledCmd.__write(__os);
    motorCmd.__write(__os);
    servoCmd.__write(__os);
}

void
TeRK::QwerkCommand::__read(::IceInternal::BasicStream* __is)
{
    audioCmd.__read(__is);
    digitalOutCmd.__read(__is);
    ledCmd.__read(__is);
    motorCmd.__read(__is);
    servoCmd.__read(__is);
}

void
TeRK::__write(::IceInternal::BasicStream* __os, const ::TeRK::AbstractCommandControllerPrx* begin, const ::TeRK::AbstractCommandControllerPrx* end, ::TeRK::__U__AbstractCommandControllerProxyArray)
{
    ::Ice::Int size = static_cast< ::Ice::Int>(end - begin);
    __os->writeSize(size);
    for(int i = 0; i < size; ++i)
    {
	::TeRK::__write(__os, begin[i]);
    }
}

void
TeRK::__read(::IceInternal::BasicStream* __is, ::TeRK::AbstractCommandControllerProxyArray& v, ::TeRK::__U__AbstractCommandControllerProxyArray)
{
    ::Ice::Int sz;
    __is->readSize(sz);
    __is->startSeq(sz, 2);
    v.resize(sz);
    for(int i = 0; i < sz; ++i)
    {
	::TeRK::__read(__is, v[i]);
	__is->checkSeq();
	__is->endElement();
    }
    __is->endSeq(sz);
}

void
TeRK::AMI_PropertyManager_getProperty::__invoke(const ::TeRK::PropertyManagerPrx& __prx, const ::std::string& key, const ::Ice::Context& __ctx)
{
    try
    {
	__prepare(__prx, __TeRK__PropertyManager__getProperty_name, ::Ice::Normal, __ctx);
	__os->write(key);
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
TeRK::AMI_PropertyManager_getProperty::__response(bool __ok)
{
    ::std::string __ret;
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
TeRK::AMI_PropertyManager_getProperties::__invoke(const ::TeRK::PropertyManagerPrx& __prx, const ::Ice::Context& __ctx)
{
    try
    {
	__prepare(__prx, __TeRK__PropertyManager__getProperties_name, ::Ice::Normal, __ctx);
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
TeRK::AMI_PropertyManager_getProperties::__response(bool __ok)
{
    ::TeRK::PropertyMap __ret;
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
	::TeRK::__read(__is, __ret, ::TeRK::__U__PropertyMap());
    }
    catch(const ::Ice::LocalException& __ex)
    {
	__finished(__ex);
	return;
    }
    ice_response(__ret);
}

void
TeRK::AMI_PropertyManager_getPropertyKeys::__invoke(const ::TeRK::PropertyManagerPrx& __prx, const ::Ice::Context& __ctx)
{
    try
    {
	__prepare(__prx, __TeRK__PropertyManager__getPropertyKeys_name, ::Ice::Normal, __ctx);
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
TeRK::AMI_PropertyManager_getPropertyKeys::__response(bool __ok)
{
    ::TeRK::StringArray __ret;
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
TeRK::AMI_PropertyManager_setProperty::__invoke(const ::TeRK::PropertyManagerPrx& __prx, const ::std::string& key, const ::std::string& value, const ::Ice::Context& __ctx)
{
    try
    {
	__prepare(__prx, __TeRK__PropertyManager__setProperty_name, ::Ice::Normal, __ctx);
	__os->write(key);
	__os->write(value);
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
TeRK::AMI_PropertyManager_setProperty::__response(bool __ok)
{
    try
    {
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::TeRK::ReadOnlyPropertyException& __ex)
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
TeRK::AMI_AnalogInController_getState::__invoke(const ::TeRK::AnalogInControllerPrx& __prx, const ::Ice::Context& __ctx)
{
    try
    {
	__prepare(__prx, __TeRK__AnalogInController__getState_name, ::Ice::Normal, __ctx);
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
TeRK::AMI_AnalogInController_getState::__response(bool __ok)
{
    ::TeRK::AnalogInState __ret;
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
	__ret.__read(__is);
    }
    catch(const ::Ice::LocalException& __ex)
    {
	__finished(__ex);
	return;
    }
    ice_response(__ret);
}

void
TeRK::AMI_AudioController_execute::__invoke(const ::TeRK::AudioControllerPrx& __prx, const ::TeRK::AudioCommand& command, const ::Ice::Context& __ctx)
{
    try
    {
	__prepare(__prx, __TeRK__AudioController__execute_name, ::Ice::Normal, __ctx);
	command.__write(__os);
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
TeRK::AMI_AudioController_execute::__response(bool __ok)
{
    try
    {
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::TeRK::AudioCommandException& __ex)
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
TeRK::AMI_DigitalInController_getState::__invoke(const ::TeRK::DigitalInControllerPrx& __prx, const ::Ice::Context& __ctx)
{
    try
    {
	__prepare(__prx, __TeRK__DigitalInController__getState_name, ::Ice::Normal, __ctx);
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
TeRK::AMI_DigitalInController_getState::__response(bool __ok)
{
    ::TeRK::DigitalInState __ret;
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
	__ret.__read(__is);
    }
    catch(const ::Ice::LocalException& __ex)
    {
	__finished(__ex);
	return;
    }
    ice_response(__ret);
}

void
TeRK::AMI_DigitalOutController_execute::__invoke(const ::TeRK::DigitalOutControllerPrx& __prx, const ::TeRK::DigitalOutCommand& command, const ::Ice::Context& __ctx)
{
    try
    {
	__prepare(__prx, __TeRK__DigitalOutController__execute_name, ::Ice::Normal, __ctx);
	command.__write(__os);
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
TeRK::AMI_DigitalOutController_execute::__response(bool __ok)
{
    try
    {
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::TeRK::DigitalOutCommandException& __ex)
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
TeRK::AMI_LEDController_execute::__invoke(const ::TeRK::LEDControllerPrx& __prx, const ::TeRK::LEDCommand& command, const ::Ice::Context& __ctx)
{
    try
    {
	__prepare(__prx, __TeRK__LEDController__execute_name, ::Ice::Normal, __ctx);
	command.__write(__os);
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
TeRK::AMI_LEDController_execute::__response(bool __ok)
{
    try
    {
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::TeRK::LEDCommandException& __ex)
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
TeRK::AMI_MotorController_execute::__invoke(const ::TeRK::MotorControllerPrx& __prx, const ::TeRK::MotorCommand& command, const ::Ice::Context& __ctx)
{
    try
    {
	__prepare(__prx, __TeRK__MotorController__execute_name, ::Ice::Normal, __ctx);
	command.__write(__os);
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
TeRK::AMI_MotorController_execute::__response(bool __ok)
{
    ::TeRK::MotorState __ret;
    try
    {
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::TeRK::MotorCommandException& __ex)
	    {
		ice_exception(__ex);
		return;
	    }
	    catch(const ::Ice::UserException& __ex)
	    {
		throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
	    }
	}
	__ret.__read(__is);
    }
    catch(const ::Ice::LocalException& __ex)
    {
	__finished(__ex);
	return;
    }
    ice_response(__ret);
}

void
TeRK::AMI_MotorController_getFrequency::__invoke(const ::TeRK::MotorControllerPrx& __prx, const ::Ice::Context& __ctx)
{
    try
    {
	__prepare(__prx, __TeRK__MotorController__getFrequency_name, ::Ice::Normal, __ctx);
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
TeRK::AMI_MotorController_getFrequency::__response(bool __ok)
{
    ::Ice::Int __ret;
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
TeRK::AMI_MotorController_startMotorBufferRecord::__invoke(const ::TeRK::MotorControllerPrx& __prx, const ::TeRK::BooleanArray& motorMask, const ::Ice::Context& __ctx)
{
    try
    {
	__prepare(__prx, __TeRK__MotorController__startMotorBufferRecord_name, ::Ice::Normal, __ctx);
	__os->write(motorMask);
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
TeRK::AMI_MotorController_startMotorBufferRecord::__response(bool __ok)
{
    try
    {
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::TeRK::MotorException& __ex)
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
TeRK::AMI_MotorController_stopMotorBufferRecord::__invoke(const ::TeRK::MotorControllerPrx& __prx, const ::TeRK::BooleanArray& motorMask, const ::Ice::Context& __ctx)
{
    try
    {
	__prepare(__prx, __TeRK__MotorController__stopMotorBufferRecord_name, ::Ice::Idempotent, __ctx);
	__os->write(motorMask);
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
TeRK::AMI_MotorController_stopMotorBufferRecord::__response(bool __ok)
{
    try
    {
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::TeRK::MotorException& __ex)
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
TeRK::AMI_MotorController_getMotorBuffers::__invoke(const ::TeRK::MotorControllerPrx& __prx, const ::TeRK::BooleanArray& motorMask, const ::Ice::Context& __ctx)
{
    try
    {
	__prepare(__prx, __TeRK__MotorController__getMotorBuffers_name, ::Ice::Nonmutating, __ctx);
	__os->write(motorMask);
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
TeRK::AMI_MotorController_getMotorBuffers::__response(bool __ok)
{
    ::TeRK::MotorBufferArray __ret;
    try
    {
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::TeRK::MotorException& __ex)
	    {
		ice_exception(__ex);
		return;
	    }
	    catch(const ::Ice::UserException& __ex)
	    {
		throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
	    }
	}
	::TeRK::__read(__is, __ret, ::TeRK::__U__MotorBufferArray());
    }
    catch(const ::Ice::LocalException& __ex)
    {
	__finished(__ex);
	return;
    }
    ice_response(__ret);
}

void
TeRK::AMI_MotorController_setMotorBuffer::__invoke(const ::TeRK::MotorControllerPrx& __prx, const ::TeRK::BooleanArray& motorMask, const ::TeRK::MotorBufferArray& motorBuffers, const ::Ice::Context& __ctx)
{
    try
    {
	__prepare(__prx, __TeRK__MotorController__setMotorBuffer_name, ::Ice::Idempotent, __ctx);
	__os->write(motorMask);
	if(motorBuffers.size() == 0)
	{
	    __os->writeSize(0);
	}
	else
	{
	    ::TeRK::__write(__os, &motorBuffers[0], &motorBuffers[0] + motorBuffers.size(), ::TeRK::__U__MotorBufferArray());
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
TeRK::AMI_MotorController_setMotorBuffer::__response(bool __ok)
{
    try
    {
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::TeRK::MotorException& __ex)
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
TeRK::AMI_MotorController_playMotorBuffer::__invoke(const ::TeRK::MotorControllerPrx& __prx, const ::TeRK::BooleanArray& motorMask, const ::Ice::Context& __ctx)
{
    try
    {
	__prepare(__prx, __TeRK__MotorController__playMotorBuffer_name, ::Ice::Normal, __ctx);
	__os->write(motorMask);
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
TeRK::AMI_MotorController_playMotorBuffer::__response(bool __ok)
{
    try
    {
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::TeRK::MotorException& __ex)
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
TeRK::AMI_ServoController_execute::__invoke(const ::TeRK::ServoControllerPrx& __prx, const ::TeRK::ServoCommand& command, const ::Ice::Context& __ctx)
{
    try
    {
	__prepare(__prx, __TeRK__ServoController__execute_name, ::Ice::Normal, __ctx);
	command.__write(__os);
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
TeRK::AMI_ServoController_execute::__response(bool __ok)
{
    ::TeRK::ServoState __ret;
    try
    {
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::TeRK::ServoCommandException& __ex)
	    {
		ice_exception(__ex);
		return;
	    }
	    catch(const ::Ice::UserException& __ex)
	    {
		throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
	    }
	}
	__ret.__read(__is);
    }
    catch(const ::Ice::LocalException& __ex)
    {
	__finished(__ex);
	return;
    }
    ice_response(__ret);
}

void
TeRK::AMI_VideoStreamerServer_startVideoStream::__invoke(const ::TeRK::VideoStreamerServerPrx& __prx, const ::Ice::Context& __ctx)
{
    try
    {
	__prepare(__prx, __TeRK__VideoStreamerServer__startVideoStream_name, ::Ice::Idempotent, __ctx);
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
TeRK::AMI_VideoStreamerServer_startVideoStream::__response(bool __ok)
{
    try
    {
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::TeRK::VideoException& __ex)
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
TeRK::AMI_VideoStreamerServer_stopVideoStream::__invoke(const ::TeRK::VideoStreamerServerPrx& __prx, const ::Ice::Context& __ctx)
{
    try
    {
	__prepare(__prx, __TeRK__VideoStreamerServer__stopVideoStream_name, ::Ice::Idempotent, __ctx);
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
TeRK::AMI_VideoStreamerServer_stopVideoStream::__response(bool __ok)
{
    try
    {
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::TeRK::VideoException& __ex)
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
TeRK::AMI_VideoStreamerServer_startCamera::__invoke(const ::TeRK::VideoStreamerServerPrx& __prx, const ::Ice::Context& __ctx)
{
    try
    {
	__prepare(__prx, __TeRK__VideoStreamerServer__startCamera_name, ::Ice::Idempotent, __ctx);
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
TeRK::AMI_VideoStreamerServer_startCamera::__response(bool __ok)
{
    ::Ice::Int __ret;
    try
    {
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::TeRK::VideoException& __ex)
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
TeRK::AMI_VideoStreamerServer_stopCamera::__invoke(const ::TeRK::VideoStreamerServerPrx& __prx, const ::Ice::Context& __ctx)
{
    try
    {
	__prepare(__prx, __TeRK__VideoStreamerServer__stopCamera_name, ::Ice::Idempotent, __ctx);
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
TeRK::AMI_VideoStreamerServer_stopCamera::__response(bool __ok)
{
    ::Ice::Int __ret;
    try
    {
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::TeRK::VideoException& __ex)
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
TeRK::AMI_VideoStreamerServer_getFrame::__invoke(const ::TeRK::VideoStreamerServerPrx& __prx, ::Ice::Int frameNumber, const ::Ice::Context& __ctx)
{
    try
    {
	__prepare(__prx, __TeRK__VideoStreamerServer__getFrame_name, ::Ice::Normal, __ctx);
	__os->write(frameNumber);
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
TeRK::AMI_VideoStreamerServer_getFrame::__response(bool __ok)
{
    ::TeRK::Image __ret;
    try
    {
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::TeRK::VideoException& __ex)
	    {
		ice_exception(__ex);
		return;
	    }
	    catch(const ::Ice::UserException& __ex)
	    {
		throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
	    }
	}
	__ret.__read(__is);
    }
    catch(const ::Ice::LocalException& __ex)
    {
	__finished(__ex);
	return;
    }
    ice_response(__ret);
}

void
TeRK::AMI_TerkUser_getSupportedServices::__invoke(const ::TeRK::TerkUserPrx& __prx, const ::Ice::Context& __ctx)
{
    try
    {
	__prepare(__prx, __TeRK__TerkUser__getSupportedServices_name, ::Ice::Nonmutating, __ctx);
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
TeRK::AMI_TerkUser_getSupportedServices::__response(bool __ok)
{
    ::TeRK::ProxyTypeIdToIdentityMap __ret;
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
	::TeRK::__read(__is, __ret, ::TeRK::__U__ProxyTypeIdToIdentityMap());
    }
    catch(const ::Ice::LocalException& __ex)
    {
	__finished(__ex);
	return;
    }
    ice_response(__ret);
}

void
TeRK::AMI_Qwerk_getState::__invoke(const ::TeRK::QwerkPrx& __prx, const ::Ice::Context& __ctx)
{
    try
    {
	__prepare(__prx, __TeRK__Qwerk__getState_name, ::Ice::Nonmutating, __ctx);
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
TeRK::AMI_Qwerk_getState::__response(bool __ok)
{
    ::TeRK::QwerkState __ret;
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
	__ret.__read(__is);
    }
    catch(const ::Ice::LocalException& __ex)
    {
	__finished(__ex);
	return;
    }
    ice_response(__ret);
}

void
TeRK::AMI_Qwerk_execute::__invoke(const ::TeRK::QwerkPrx& __prx, const ::TeRK::QwerkCommand& command, const ::Ice::Context& __ctx)
{
    try
    {
	__prepare(__prx, __TeRK__Qwerk__execute_name, ::Ice::Normal, __ctx);
	command.__write(__os);
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
TeRK::AMI_Qwerk_execute::__response(bool __ok)
{
    ::TeRK::QwerkState __ret;
    try
    {
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::TeRK::CommandException& __ex)
	    {
		ice_exception(__ex);
		return;
	    }
	    catch(const ::Ice::UserException& __ex)
	    {
		throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
	    }
	}
	__ret.__read(__is);
    }
    catch(const ::Ice::LocalException& __ex)
    {
	__finished(__ex);
	return;
    }
    ice_response(__ret);
}

void
TeRK::AMI_Qwerk_emergencyStop::__invoke(const ::TeRK::QwerkPrx& __prx, const ::Ice::Context& __ctx)
{
    try
    {
	__prepare(__prx, __TeRK__Qwerk__emergencyStop_name, ::Ice::Idempotent, __ctx);
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
TeRK::AMI_Qwerk_emergencyStop::__response(bool __ok)
{
    ::TeRK::QwerkState __ret;
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
	__ret.__read(__is);
    }
    catch(const ::Ice::LocalException& __ex)
    {
	__finished(__ex);
	return;
    }
    ice_response(__ret);
}

void
TeRK::AMI_Qwerk_getCommandControllerTypeToProxyIdentityMap::__invoke(const ::TeRK::QwerkPrx& __prx, const ::Ice::Context& __ctx)
{
    try
    {
	__prepare(__prx, __TeRK__Qwerk__getCommandControllerTypeToProxyIdentityMap_name, ::Ice::Nonmutating, __ctx);
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
TeRK::AMI_Qwerk_getCommandControllerTypeToProxyIdentityMap::__response(bool __ok)
{
    ::TeRK::ProxyTypeIdToIdentityMap __ret;
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
	::TeRK::__read(__is, __ret, ::TeRK::__U__ProxyTypeIdToIdentityMap());
    }
    catch(const ::Ice::LocalException& __ex)
    {
	__finished(__ex);
	return;
    }
    ice_response(__ret);
}

::std::string
IceProxy::TeRK::PropertyManager::getProperty(const ::std::string& key, const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__TeRK__PropertyManager__getProperty_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::TeRK::PropertyManager* __del = dynamic_cast< ::IceDelegate::TeRK::PropertyManager*>(__delBase.get());
	    return __del->getProperty(key, __ctx);
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
IceProxy::TeRK::PropertyManager::getProperty_async(const ::TeRK::AMI_PropertyManager_getPropertyPtr& __cb, const ::std::string& key)
{
    getProperty_async(__cb, key, __defaultContext());
}

void
IceProxy::TeRK::PropertyManager::getProperty_async(const ::TeRK::AMI_PropertyManager_getPropertyPtr& __cb, const ::std::string& key, const ::Ice::Context& __ctx)
{
    __cb->__invoke(this, key, __ctx);
}

::TeRK::PropertyMap
IceProxy::TeRK::PropertyManager::getProperties(const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__TeRK__PropertyManager__getProperties_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::TeRK::PropertyManager* __del = dynamic_cast< ::IceDelegate::TeRK::PropertyManager*>(__delBase.get());
	    return __del->getProperties(__ctx);
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
IceProxy::TeRK::PropertyManager::getProperties_async(const ::TeRK::AMI_PropertyManager_getPropertiesPtr& __cb)
{
    getProperties_async(__cb, __defaultContext());
}

void
IceProxy::TeRK::PropertyManager::getProperties_async(const ::TeRK::AMI_PropertyManager_getPropertiesPtr& __cb, const ::Ice::Context& __ctx)
{
    __cb->__invoke(this, __ctx);
}

::TeRK::StringArray
IceProxy::TeRK::PropertyManager::getPropertyKeys(const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__TeRK__PropertyManager__getPropertyKeys_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::TeRK::PropertyManager* __del = dynamic_cast< ::IceDelegate::TeRK::PropertyManager*>(__delBase.get());
	    return __del->getPropertyKeys(__ctx);
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
IceProxy::TeRK::PropertyManager::getPropertyKeys_async(const ::TeRK::AMI_PropertyManager_getPropertyKeysPtr& __cb)
{
    getPropertyKeys_async(__cb, __defaultContext());
}

void
IceProxy::TeRK::PropertyManager::getPropertyKeys_async(const ::TeRK::AMI_PropertyManager_getPropertyKeysPtr& __cb, const ::Ice::Context& __ctx)
{
    __cb->__invoke(this, __ctx);
}

void
IceProxy::TeRK::PropertyManager::setProperty(const ::std::string& key, const ::std::string& value, const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__TeRK__PropertyManager__setProperty_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::TeRK::PropertyManager* __del = dynamic_cast< ::IceDelegate::TeRK::PropertyManager*>(__delBase.get());
	    __del->setProperty(key, value, __ctx);
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
IceProxy::TeRK::PropertyManager::setProperty_async(const ::TeRK::AMI_PropertyManager_setPropertyPtr& __cb, const ::std::string& key, const ::std::string& value)
{
    setProperty_async(__cb, key, value, __defaultContext());
}

void
IceProxy::TeRK::PropertyManager::setProperty_async(const ::TeRK::AMI_PropertyManager_setPropertyPtr& __cb, const ::std::string& key, const ::std::string& value, const ::Ice::Context& __ctx)
{
    __cb->__invoke(this, key, value, __ctx);
}

const ::std::string&
IceProxy::TeRK::PropertyManager::ice_staticId()
{
    return ::TeRK::PropertyManager::ice_staticId();
}

::IceInternal::Handle< ::IceDelegateM::Ice::Object>
IceProxy::TeRK::PropertyManager::__createDelegateM()
{
    return ::IceInternal::Handle< ::IceDelegateM::Ice::Object>(new ::IceDelegateM::TeRK::PropertyManager);
}

::IceInternal::Handle< ::IceDelegateD::Ice::Object>
IceProxy::TeRK::PropertyManager::__createDelegateD()
{
    return ::IceInternal::Handle< ::IceDelegateD::Ice::Object>(new ::IceDelegateD::TeRK::PropertyManager);
}

bool
IceProxy::TeRK::operator==(const ::IceProxy::TeRK::PropertyManager& l, const ::IceProxy::TeRK::PropertyManager& r)
{
    return static_cast<const ::IceProxy::Ice::Object&>(l) == static_cast<const ::IceProxy::Ice::Object&>(r);
}

bool
IceProxy::TeRK::operator!=(const ::IceProxy::TeRK::PropertyManager& l, const ::IceProxy::TeRK::PropertyManager& r)
{
    return static_cast<const ::IceProxy::Ice::Object&>(l) != static_cast<const ::IceProxy::Ice::Object&>(r);
}

bool
IceProxy::TeRK::operator<(const ::IceProxy::TeRK::PropertyManager& l, const ::IceProxy::TeRK::PropertyManager& r)
{
    return static_cast<const ::IceProxy::Ice::Object&>(l) < static_cast<const ::IceProxy::Ice::Object&>(r);
}

bool
IceProxy::TeRK::operator<=(const ::IceProxy::TeRK::PropertyManager& l, const ::IceProxy::TeRK::PropertyManager& r)
{
    return l < r || l == r;
}

bool
IceProxy::TeRK::operator>(const ::IceProxy::TeRK::PropertyManager& l, const ::IceProxy::TeRK::PropertyManager& r)
{
    return !(l < r) && !(l == r);
}

bool
IceProxy::TeRK::operator>=(const ::IceProxy::TeRK::PropertyManager& l, const ::IceProxy::TeRK::PropertyManager& r)
{
    return !(l < r);
}

const ::std::string&
IceProxy::TeRK::AbstractCommandController::ice_staticId()
{
    return ::TeRK::AbstractCommandController::ice_staticId();
}

::IceInternal::Handle< ::IceDelegateM::Ice::Object>
IceProxy::TeRK::AbstractCommandController::__createDelegateM()
{
    return ::IceInternal::Handle< ::IceDelegateM::Ice::Object>(new ::IceDelegateM::TeRK::AbstractCommandController);
}

::IceInternal::Handle< ::IceDelegateD::Ice::Object>
IceProxy::TeRK::AbstractCommandController::__createDelegateD()
{
    return ::IceInternal::Handle< ::IceDelegateD::Ice::Object>(new ::IceDelegateD::TeRK::AbstractCommandController);
}

bool
IceProxy::TeRK::operator==(const ::IceProxy::TeRK::AbstractCommandController& l, const ::IceProxy::TeRK::AbstractCommandController& r)
{
    return static_cast<const ::IceProxy::Ice::Object&>(l) == static_cast<const ::IceProxy::Ice::Object&>(r);
}

bool
IceProxy::TeRK::operator!=(const ::IceProxy::TeRK::AbstractCommandController& l, const ::IceProxy::TeRK::AbstractCommandController& r)
{
    return static_cast<const ::IceProxy::Ice::Object&>(l) != static_cast<const ::IceProxy::Ice::Object&>(r);
}

bool
IceProxy::TeRK::operator<(const ::IceProxy::TeRK::AbstractCommandController& l, const ::IceProxy::TeRK::AbstractCommandController& r)
{
    return static_cast<const ::IceProxy::Ice::Object&>(l) < static_cast<const ::IceProxy::Ice::Object&>(r);
}

bool
IceProxy::TeRK::operator<=(const ::IceProxy::TeRK::AbstractCommandController& l, const ::IceProxy::TeRK::AbstractCommandController& r)
{
    return l < r || l == r;
}

bool
IceProxy::TeRK::operator>(const ::IceProxy::TeRK::AbstractCommandController& l, const ::IceProxy::TeRK::AbstractCommandController& r)
{
    return !(l < r) && !(l == r);
}

bool
IceProxy::TeRK::operator>=(const ::IceProxy::TeRK::AbstractCommandController& l, const ::IceProxy::TeRK::AbstractCommandController& r)
{
    return !(l < r);
}

::TeRK::AnalogInState
IceProxy::TeRK::AnalogInController::getState(const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__TeRK__AnalogInController__getState_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::TeRK::AnalogInController* __del = dynamic_cast< ::IceDelegate::TeRK::AnalogInController*>(__delBase.get());
	    return __del->getState(__ctx);
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
IceProxy::TeRK::AnalogInController::getState_async(const ::TeRK::AMI_AnalogInController_getStatePtr& __cb)
{
    getState_async(__cb, __defaultContext());
}

void
IceProxy::TeRK::AnalogInController::getState_async(const ::TeRK::AMI_AnalogInController_getStatePtr& __cb, const ::Ice::Context& __ctx)
{
    __cb->__invoke(this, __ctx);
}

const ::std::string&
IceProxy::TeRK::AnalogInController::ice_staticId()
{
    return ::TeRK::AnalogInController::ice_staticId();
}

::IceInternal::Handle< ::IceDelegateM::Ice::Object>
IceProxy::TeRK::AnalogInController::__createDelegateM()
{
    return ::IceInternal::Handle< ::IceDelegateM::Ice::Object>(new ::IceDelegateM::TeRK::AnalogInController);
}

::IceInternal::Handle< ::IceDelegateD::Ice::Object>
IceProxy::TeRK::AnalogInController::__createDelegateD()
{
    return ::IceInternal::Handle< ::IceDelegateD::Ice::Object>(new ::IceDelegateD::TeRK::AnalogInController);
}

bool
IceProxy::TeRK::operator==(const ::IceProxy::TeRK::AnalogInController& l, const ::IceProxy::TeRK::AnalogInController& r)
{
    return static_cast<const ::IceProxy::Ice::Object&>(l) == static_cast<const ::IceProxy::Ice::Object&>(r);
}

bool
IceProxy::TeRK::operator!=(const ::IceProxy::TeRK::AnalogInController& l, const ::IceProxy::TeRK::AnalogInController& r)
{
    return static_cast<const ::IceProxy::Ice::Object&>(l) != static_cast<const ::IceProxy::Ice::Object&>(r);
}

bool
IceProxy::TeRK::operator<(const ::IceProxy::TeRK::AnalogInController& l, const ::IceProxy::TeRK::AnalogInController& r)
{
    return static_cast<const ::IceProxy::Ice::Object&>(l) < static_cast<const ::IceProxy::Ice::Object&>(r);
}

bool
IceProxy::TeRK::operator<=(const ::IceProxy::TeRK::AnalogInController& l, const ::IceProxy::TeRK::AnalogInController& r)
{
    return l < r || l == r;
}

bool
IceProxy::TeRK::operator>(const ::IceProxy::TeRK::AnalogInController& l, const ::IceProxy::TeRK::AnalogInController& r)
{
    return !(l < r) && !(l == r);
}

bool
IceProxy::TeRK::operator>=(const ::IceProxy::TeRK::AnalogInController& l, const ::IceProxy::TeRK::AnalogInController& r)
{
    return !(l < r);
}

void
IceProxy::TeRK::AudioController::execute(const ::TeRK::AudioCommand& command, const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__TeRK__AudioController__execute_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::TeRK::AudioController* __del = dynamic_cast< ::IceDelegate::TeRK::AudioController*>(__delBase.get());
	    __del->execute(command, __ctx);
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
IceProxy::TeRK::AudioController::execute_async(const ::TeRK::AMI_AudioController_executePtr& __cb, const ::TeRK::AudioCommand& command)
{
    execute_async(__cb, command, __defaultContext());
}

void
IceProxy::TeRK::AudioController::execute_async(const ::TeRK::AMI_AudioController_executePtr& __cb, const ::TeRK::AudioCommand& command, const ::Ice::Context& __ctx)
{
    __cb->__invoke(this, command, __ctx);
}

const ::std::string&
IceProxy::TeRK::AudioController::ice_staticId()
{
    return ::TeRK::AudioController::ice_staticId();
}

::IceInternal::Handle< ::IceDelegateM::Ice::Object>
IceProxy::TeRK::AudioController::__createDelegateM()
{
    return ::IceInternal::Handle< ::IceDelegateM::Ice::Object>(new ::IceDelegateM::TeRK::AudioController);
}

::IceInternal::Handle< ::IceDelegateD::Ice::Object>
IceProxy::TeRK::AudioController::__createDelegateD()
{
    return ::IceInternal::Handle< ::IceDelegateD::Ice::Object>(new ::IceDelegateD::TeRK::AudioController);
}

bool
IceProxy::TeRK::operator==(const ::IceProxy::TeRK::AudioController& l, const ::IceProxy::TeRK::AudioController& r)
{
    return static_cast<const ::IceProxy::Ice::Object&>(l) == static_cast<const ::IceProxy::Ice::Object&>(r);
}

bool
IceProxy::TeRK::operator!=(const ::IceProxy::TeRK::AudioController& l, const ::IceProxy::TeRK::AudioController& r)
{
    return static_cast<const ::IceProxy::Ice::Object&>(l) != static_cast<const ::IceProxy::Ice::Object&>(r);
}

bool
IceProxy::TeRK::operator<(const ::IceProxy::TeRK::AudioController& l, const ::IceProxy::TeRK::AudioController& r)
{
    return static_cast<const ::IceProxy::Ice::Object&>(l) < static_cast<const ::IceProxy::Ice::Object&>(r);
}

bool
IceProxy::TeRK::operator<=(const ::IceProxy::TeRK::AudioController& l, const ::IceProxy::TeRK::AudioController& r)
{
    return l < r || l == r;
}

bool
IceProxy::TeRK::operator>(const ::IceProxy::TeRK::AudioController& l, const ::IceProxy::TeRK::AudioController& r)
{
    return !(l < r) && !(l == r);
}

bool
IceProxy::TeRK::operator>=(const ::IceProxy::TeRK::AudioController& l, const ::IceProxy::TeRK::AudioController& r)
{
    return !(l < r);
}

::TeRK::DigitalInState
IceProxy::TeRK::DigitalInController::getState(const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__TeRK__DigitalInController__getState_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::TeRK::DigitalInController* __del = dynamic_cast< ::IceDelegate::TeRK::DigitalInController*>(__delBase.get());
	    return __del->getState(__ctx);
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
IceProxy::TeRK::DigitalInController::getState_async(const ::TeRK::AMI_DigitalInController_getStatePtr& __cb)
{
    getState_async(__cb, __defaultContext());
}

void
IceProxy::TeRK::DigitalInController::getState_async(const ::TeRK::AMI_DigitalInController_getStatePtr& __cb, const ::Ice::Context& __ctx)
{
    __cb->__invoke(this, __ctx);
}

const ::std::string&
IceProxy::TeRK::DigitalInController::ice_staticId()
{
    return ::TeRK::DigitalInController::ice_staticId();
}

::IceInternal::Handle< ::IceDelegateM::Ice::Object>
IceProxy::TeRK::DigitalInController::__createDelegateM()
{
    return ::IceInternal::Handle< ::IceDelegateM::Ice::Object>(new ::IceDelegateM::TeRK::DigitalInController);
}

::IceInternal::Handle< ::IceDelegateD::Ice::Object>
IceProxy::TeRK::DigitalInController::__createDelegateD()
{
    return ::IceInternal::Handle< ::IceDelegateD::Ice::Object>(new ::IceDelegateD::TeRK::DigitalInController);
}

bool
IceProxy::TeRK::operator==(const ::IceProxy::TeRK::DigitalInController& l, const ::IceProxy::TeRK::DigitalInController& r)
{
    return static_cast<const ::IceProxy::Ice::Object&>(l) == static_cast<const ::IceProxy::Ice::Object&>(r);
}

bool
IceProxy::TeRK::operator!=(const ::IceProxy::TeRK::DigitalInController& l, const ::IceProxy::TeRK::DigitalInController& r)
{
    return static_cast<const ::IceProxy::Ice::Object&>(l) != static_cast<const ::IceProxy::Ice::Object&>(r);
}

bool
IceProxy::TeRK::operator<(const ::IceProxy::TeRK::DigitalInController& l, const ::IceProxy::TeRK::DigitalInController& r)
{
    return static_cast<const ::IceProxy::Ice::Object&>(l) < static_cast<const ::IceProxy::Ice::Object&>(r);
}

bool
IceProxy::TeRK::operator<=(const ::IceProxy::TeRK::DigitalInController& l, const ::IceProxy::TeRK::DigitalInController& r)
{
    return l < r || l == r;
}

bool
IceProxy::TeRK::operator>(const ::IceProxy::TeRK::DigitalInController& l, const ::IceProxy::TeRK::DigitalInController& r)
{
    return !(l < r) && !(l == r);
}

bool
IceProxy::TeRK::operator>=(const ::IceProxy::TeRK::DigitalInController& l, const ::IceProxy::TeRK::DigitalInController& r)
{
    return !(l < r);
}

void
IceProxy::TeRK::DigitalOutController::execute(const ::TeRK::DigitalOutCommand& command, const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__TeRK__DigitalOutController__execute_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::TeRK::DigitalOutController* __del = dynamic_cast< ::IceDelegate::TeRK::DigitalOutController*>(__delBase.get());
	    __del->execute(command, __ctx);
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
IceProxy::TeRK::DigitalOutController::execute_async(const ::TeRK::AMI_DigitalOutController_executePtr& __cb, const ::TeRK::DigitalOutCommand& command)
{
    execute_async(__cb, command, __defaultContext());
}

void
IceProxy::TeRK::DigitalOutController::execute_async(const ::TeRK::AMI_DigitalOutController_executePtr& __cb, const ::TeRK::DigitalOutCommand& command, const ::Ice::Context& __ctx)
{
    __cb->__invoke(this, command, __ctx);
}

const ::std::string&
IceProxy::TeRK::DigitalOutController::ice_staticId()
{
    return ::TeRK::DigitalOutController::ice_staticId();
}

::IceInternal::Handle< ::IceDelegateM::Ice::Object>
IceProxy::TeRK::DigitalOutController::__createDelegateM()
{
    return ::IceInternal::Handle< ::IceDelegateM::Ice::Object>(new ::IceDelegateM::TeRK::DigitalOutController);
}

::IceInternal::Handle< ::IceDelegateD::Ice::Object>
IceProxy::TeRK::DigitalOutController::__createDelegateD()
{
    return ::IceInternal::Handle< ::IceDelegateD::Ice::Object>(new ::IceDelegateD::TeRK::DigitalOutController);
}

bool
IceProxy::TeRK::operator==(const ::IceProxy::TeRK::DigitalOutController& l, const ::IceProxy::TeRK::DigitalOutController& r)
{
    return static_cast<const ::IceProxy::Ice::Object&>(l) == static_cast<const ::IceProxy::Ice::Object&>(r);
}

bool
IceProxy::TeRK::operator!=(const ::IceProxy::TeRK::DigitalOutController& l, const ::IceProxy::TeRK::DigitalOutController& r)
{
    return static_cast<const ::IceProxy::Ice::Object&>(l) != static_cast<const ::IceProxy::Ice::Object&>(r);
}

bool
IceProxy::TeRK::operator<(const ::IceProxy::TeRK::DigitalOutController& l, const ::IceProxy::TeRK::DigitalOutController& r)
{
    return static_cast<const ::IceProxy::Ice::Object&>(l) < static_cast<const ::IceProxy::Ice::Object&>(r);
}

bool
IceProxy::TeRK::operator<=(const ::IceProxy::TeRK::DigitalOutController& l, const ::IceProxy::TeRK::DigitalOutController& r)
{
    return l < r || l == r;
}

bool
IceProxy::TeRK::operator>(const ::IceProxy::TeRK::DigitalOutController& l, const ::IceProxy::TeRK::DigitalOutController& r)
{
    return !(l < r) && !(l == r);
}

bool
IceProxy::TeRK::operator>=(const ::IceProxy::TeRK::DigitalOutController& l, const ::IceProxy::TeRK::DigitalOutController& r)
{
    return !(l < r);
}

void
IceProxy::TeRK::LEDController::execute(const ::TeRK::LEDCommand& command, const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__TeRK__LEDController__execute_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::TeRK::LEDController* __del = dynamic_cast< ::IceDelegate::TeRK::LEDController*>(__delBase.get());
	    __del->execute(command, __ctx);
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
IceProxy::TeRK::LEDController::execute_async(const ::TeRK::AMI_LEDController_executePtr& __cb, const ::TeRK::LEDCommand& command)
{
    execute_async(__cb, command, __defaultContext());
}

void
IceProxy::TeRK::LEDController::execute_async(const ::TeRK::AMI_LEDController_executePtr& __cb, const ::TeRK::LEDCommand& command, const ::Ice::Context& __ctx)
{
    __cb->__invoke(this, command, __ctx);
}

const ::std::string&
IceProxy::TeRK::LEDController::ice_staticId()
{
    return ::TeRK::LEDController::ice_staticId();
}

::IceInternal::Handle< ::IceDelegateM::Ice::Object>
IceProxy::TeRK::LEDController::__createDelegateM()
{
    return ::IceInternal::Handle< ::IceDelegateM::Ice::Object>(new ::IceDelegateM::TeRK::LEDController);
}

::IceInternal::Handle< ::IceDelegateD::Ice::Object>
IceProxy::TeRK::LEDController::__createDelegateD()
{
    return ::IceInternal::Handle< ::IceDelegateD::Ice::Object>(new ::IceDelegateD::TeRK::LEDController);
}

bool
IceProxy::TeRK::operator==(const ::IceProxy::TeRK::LEDController& l, const ::IceProxy::TeRK::LEDController& r)
{
    return static_cast<const ::IceProxy::Ice::Object&>(l) == static_cast<const ::IceProxy::Ice::Object&>(r);
}

bool
IceProxy::TeRK::operator!=(const ::IceProxy::TeRK::LEDController& l, const ::IceProxy::TeRK::LEDController& r)
{
    return static_cast<const ::IceProxy::Ice::Object&>(l) != static_cast<const ::IceProxy::Ice::Object&>(r);
}

bool
IceProxy::TeRK::operator<(const ::IceProxy::TeRK::LEDController& l, const ::IceProxy::TeRK::LEDController& r)
{
    return static_cast<const ::IceProxy::Ice::Object&>(l) < static_cast<const ::IceProxy::Ice::Object&>(r);
}

bool
IceProxy::TeRK::operator<=(const ::IceProxy::TeRK::LEDController& l, const ::IceProxy::TeRK::LEDController& r)
{
    return l < r || l == r;
}

bool
IceProxy::TeRK::operator>(const ::IceProxy::TeRK::LEDController& l, const ::IceProxy::TeRK::LEDController& r)
{
    return !(l < r) && !(l == r);
}

bool
IceProxy::TeRK::operator>=(const ::IceProxy::TeRK::LEDController& l, const ::IceProxy::TeRK::LEDController& r)
{
    return !(l < r);
}

::TeRK::MotorState
IceProxy::TeRK::MotorController::execute(const ::TeRK::MotorCommand& command, const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__TeRK__MotorController__execute_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::TeRK::MotorController* __del = dynamic_cast< ::IceDelegate::TeRK::MotorController*>(__delBase.get());
	    return __del->execute(command, __ctx);
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
IceProxy::TeRK::MotorController::execute_async(const ::TeRK::AMI_MotorController_executePtr& __cb, const ::TeRK::MotorCommand& command)
{
    execute_async(__cb, command, __defaultContext());
}

void
IceProxy::TeRK::MotorController::execute_async(const ::TeRK::AMI_MotorController_executePtr& __cb, const ::TeRK::MotorCommand& command, const ::Ice::Context& __ctx)
{
    __cb->__invoke(this, command, __ctx);
}

::Ice::Int
IceProxy::TeRK::MotorController::getFrequency(const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__TeRK__MotorController__getFrequency_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::TeRK::MotorController* __del = dynamic_cast< ::IceDelegate::TeRK::MotorController*>(__delBase.get());
	    return __del->getFrequency(__ctx);
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
IceProxy::TeRK::MotorController::getFrequency_async(const ::TeRK::AMI_MotorController_getFrequencyPtr& __cb)
{
    getFrequency_async(__cb, __defaultContext());
}

void
IceProxy::TeRK::MotorController::getFrequency_async(const ::TeRK::AMI_MotorController_getFrequencyPtr& __cb, const ::Ice::Context& __ctx)
{
    __cb->__invoke(this, __ctx);
}

void
IceProxy::TeRK::MotorController::startMotorBufferRecord(const ::TeRK::BooleanArray& motorMask, const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__TeRK__MotorController__startMotorBufferRecord_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::TeRK::MotorController* __del = dynamic_cast< ::IceDelegate::TeRK::MotorController*>(__delBase.get());
	    __del->startMotorBufferRecord(motorMask, __ctx);
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
IceProxy::TeRK::MotorController::startMotorBufferRecord_async(const ::TeRK::AMI_MotorController_startMotorBufferRecordPtr& __cb, const ::TeRK::BooleanArray& motorMask)
{
    startMotorBufferRecord_async(__cb, motorMask, __defaultContext());
}

void
IceProxy::TeRK::MotorController::startMotorBufferRecord_async(const ::TeRK::AMI_MotorController_startMotorBufferRecordPtr& __cb, const ::TeRK::BooleanArray& motorMask, const ::Ice::Context& __ctx)
{
    __cb->__invoke(this, motorMask, __ctx);
}

void
IceProxy::TeRK::MotorController::stopMotorBufferRecord(const ::TeRK::BooleanArray& motorMask, const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__TeRK__MotorController__stopMotorBufferRecord_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::TeRK::MotorController* __del = dynamic_cast< ::IceDelegate::TeRK::MotorController*>(__delBase.get());
	    __del->stopMotorBufferRecord(motorMask, __ctx);
	    return;
	}
	catch(const ::IceInternal::LocalExceptionWrapper& __ex)
	{
	    __handleExceptionWrapperRelaxed(__ex, __cnt);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    __handleException(__ex, __cnt);
	}
    }
}

void
IceProxy::TeRK::MotorController::stopMotorBufferRecord_async(const ::TeRK::AMI_MotorController_stopMotorBufferRecordPtr& __cb, const ::TeRK::BooleanArray& motorMask)
{
    stopMotorBufferRecord_async(__cb, motorMask, __defaultContext());
}

void
IceProxy::TeRK::MotorController::stopMotorBufferRecord_async(const ::TeRK::AMI_MotorController_stopMotorBufferRecordPtr& __cb, const ::TeRK::BooleanArray& motorMask, const ::Ice::Context& __ctx)
{
    __cb->__invoke(this, motorMask, __ctx);
}

::TeRK::MotorBufferArray
IceProxy::TeRK::MotorController::getMotorBuffers(const ::TeRK::BooleanArray& motorMask, const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__TeRK__MotorController__getMotorBuffers_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::TeRK::MotorController* __del = dynamic_cast< ::IceDelegate::TeRK::MotorController*>(__delBase.get());
	    return __del->getMotorBuffers(motorMask, __ctx);
	}
	catch(const ::IceInternal::LocalExceptionWrapper& __ex)
	{
	    __handleExceptionWrapperRelaxed(__ex, __cnt);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    __handleException(__ex, __cnt);
	}
    }
}

void
IceProxy::TeRK::MotorController::getMotorBuffers_async(const ::TeRK::AMI_MotorController_getMotorBuffersPtr& __cb, const ::TeRK::BooleanArray& motorMask)
{
    getMotorBuffers_async(__cb, motorMask, __defaultContext());
}

void
IceProxy::TeRK::MotorController::getMotorBuffers_async(const ::TeRK::AMI_MotorController_getMotorBuffersPtr& __cb, const ::TeRK::BooleanArray& motorMask, const ::Ice::Context& __ctx)
{
    __cb->__invoke(this, motorMask, __ctx);
}

void
IceProxy::TeRK::MotorController::setMotorBuffer(const ::TeRK::BooleanArray& motorMask, const ::TeRK::MotorBufferArray& motorBuffers, const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__TeRK__MotorController__setMotorBuffer_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::TeRK::MotorController* __del = dynamic_cast< ::IceDelegate::TeRK::MotorController*>(__delBase.get());
	    __del->setMotorBuffer(motorMask, motorBuffers, __ctx);
	    return;
	}
	catch(const ::IceInternal::LocalExceptionWrapper& __ex)
	{
	    __handleExceptionWrapperRelaxed(__ex, __cnt);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    __handleException(__ex, __cnt);
	}
    }
}

void
IceProxy::TeRK::MotorController::setMotorBuffer_async(const ::TeRK::AMI_MotorController_setMotorBufferPtr& __cb, const ::TeRK::BooleanArray& motorMask, const ::TeRK::MotorBufferArray& motorBuffers)
{
    setMotorBuffer_async(__cb, motorMask, motorBuffers, __defaultContext());
}

void
IceProxy::TeRK::MotorController::setMotorBuffer_async(const ::TeRK::AMI_MotorController_setMotorBufferPtr& __cb, const ::TeRK::BooleanArray& motorMask, const ::TeRK::MotorBufferArray& motorBuffers, const ::Ice::Context& __ctx)
{
    __cb->__invoke(this, motorMask, motorBuffers, __ctx);
}

void
IceProxy::TeRK::MotorController::playMotorBuffer(const ::TeRK::BooleanArray& motorMask, const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__TeRK__MotorController__playMotorBuffer_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::TeRK::MotorController* __del = dynamic_cast< ::IceDelegate::TeRK::MotorController*>(__delBase.get());
	    __del->playMotorBuffer(motorMask, __ctx);
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
IceProxy::TeRK::MotorController::playMotorBuffer_async(const ::TeRK::AMI_MotorController_playMotorBufferPtr& __cb, const ::TeRK::BooleanArray& motorMask)
{
    playMotorBuffer_async(__cb, motorMask, __defaultContext());
}

void
IceProxy::TeRK::MotorController::playMotorBuffer_async(const ::TeRK::AMI_MotorController_playMotorBufferPtr& __cb, const ::TeRK::BooleanArray& motorMask, const ::Ice::Context& __ctx)
{
    __cb->__invoke(this, motorMask, __ctx);
}

void
IceProxy::TeRK::MotorController::driveForward(const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::TeRK::MotorController* __del = dynamic_cast< ::IceDelegate::TeRK::MotorController*>(__delBase.get());
	    __del->driveForward(__ctx);
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
IceProxy::TeRK::MotorController::driveBack(const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::TeRK::MotorController* __del = dynamic_cast< ::IceDelegate::TeRK::MotorController*>(__delBase.get());
	    __del->driveBack(__ctx);
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
IceProxy::TeRK::MotorController::spinLeft(const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::TeRK::MotorController* __del = dynamic_cast< ::IceDelegate::TeRK::MotorController*>(__delBase.get());
	    __del->spinLeft(__ctx);
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
IceProxy::TeRK::MotorController::spinRight(const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::TeRK::MotorController* __del = dynamic_cast< ::IceDelegate::TeRK::MotorController*>(__delBase.get());
	    __del->spinRight(__ctx);
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
IceProxy::TeRK::MotorController::setMotorVelocities(const ::TeRK::IntArray& motorValues, const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__TeRK__MotorController__setMotorVelocities_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::TeRK::MotorController* __del = dynamic_cast< ::IceDelegate::TeRK::MotorController*>(__delBase.get());
	    __del->setMotorVelocities(motorValues, __ctx);
	    return;
	}
	catch(const ::IceInternal::LocalExceptionWrapper& __ex)
	{
	    __handleExceptionWrapperRelaxed(__ex, __cnt);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    __handleException(__ex, __cnt);
	}
    }
}

void
IceProxy::TeRK::MotorController::stopMotors(const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::TeRK::MotorController* __del = dynamic_cast< ::IceDelegate::TeRK::MotorController*>(__delBase.get());
	    __del->stopMotors(__ctx);
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
IceProxy::TeRK::MotorController::ice_staticId()
{
    return ::TeRK::MotorController::ice_staticId();
}

::IceInternal::Handle< ::IceDelegateM::Ice::Object>
IceProxy::TeRK::MotorController::__createDelegateM()
{
    return ::IceInternal::Handle< ::IceDelegateM::Ice::Object>(new ::IceDelegateM::TeRK::MotorController);
}

::IceInternal::Handle< ::IceDelegateD::Ice::Object>
IceProxy::TeRK::MotorController::__createDelegateD()
{
    return ::IceInternal::Handle< ::IceDelegateD::Ice::Object>(new ::IceDelegateD::TeRK::MotorController);
}

bool
IceProxy::TeRK::operator==(const ::IceProxy::TeRK::MotorController& l, const ::IceProxy::TeRK::MotorController& r)
{
    return static_cast<const ::IceProxy::Ice::Object&>(l) == static_cast<const ::IceProxy::Ice::Object&>(r);
}

bool
IceProxy::TeRK::operator!=(const ::IceProxy::TeRK::MotorController& l, const ::IceProxy::TeRK::MotorController& r)
{
    return static_cast<const ::IceProxy::Ice::Object&>(l) != static_cast<const ::IceProxy::Ice::Object&>(r);
}

bool
IceProxy::TeRK::operator<(const ::IceProxy::TeRK::MotorController& l, const ::IceProxy::TeRK::MotorController& r)
{
    return static_cast<const ::IceProxy::Ice::Object&>(l) < static_cast<const ::IceProxy::Ice::Object&>(r);
}

bool
IceProxy::TeRK::operator<=(const ::IceProxy::TeRK::MotorController& l, const ::IceProxy::TeRK::MotorController& r)
{
    return l < r || l == r;
}

bool
IceProxy::TeRK::operator>(const ::IceProxy::TeRK::MotorController& l, const ::IceProxy::TeRK::MotorController& r)
{
    return !(l < r) && !(l == r);
}

bool
IceProxy::TeRK::operator>=(const ::IceProxy::TeRK::MotorController& l, const ::IceProxy::TeRK::MotorController& r)
{
    return !(l < r);
}

::TeRK::ServoState
IceProxy::TeRK::ServoController::execute(const ::TeRK::ServoCommand& command, const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__TeRK__ServoController__execute_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::TeRK::ServoController* __del = dynamic_cast< ::IceDelegate::TeRK::ServoController*>(__delBase.get());
	    return __del->execute(command, __ctx);
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
IceProxy::TeRK::ServoController::execute_async(const ::TeRK::AMI_ServoController_executePtr& __cb, const ::TeRK::ServoCommand& command)
{
    execute_async(__cb, command, __defaultContext());
}

void
IceProxy::TeRK::ServoController::execute_async(const ::TeRK::AMI_ServoController_executePtr& __cb, const ::TeRK::ServoCommand& command, const ::Ice::Context& __ctx)
{
    __cb->__invoke(this, command, __ctx);
}

void
IceProxy::TeRK::ServoController::cameraTiltUp(const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::TeRK::ServoController* __del = dynamic_cast< ::IceDelegate::TeRK::ServoController*>(__delBase.get());
	    __del->cameraTiltUp(__ctx);
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
IceProxy::TeRK::ServoController::cameraTiltDown(const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::TeRK::ServoController* __del = dynamic_cast< ::IceDelegate::TeRK::ServoController*>(__delBase.get());
	    __del->cameraTiltDown(__ctx);
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
IceProxy::TeRK::ServoController::cameraPanLeft(const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::TeRK::ServoController* __del = dynamic_cast< ::IceDelegate::TeRK::ServoController*>(__delBase.get());
	    __del->cameraPanLeft(__ctx);
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
IceProxy::TeRK::ServoController::cameraPanRight(const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::TeRK::ServoController* __del = dynamic_cast< ::IceDelegate::TeRK::ServoController*>(__delBase.get());
	    __del->cameraPanRight(__ctx);
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
IceProxy::TeRK::ServoController::setServoPositions(const ::TeRK::IntArray& servoPositions, const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::TeRK::ServoController* __del = dynamic_cast< ::IceDelegate::TeRK::ServoController*>(__delBase.get());
	    __del->setServoPositions(servoPositions, __ctx);
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
IceProxy::TeRK::ServoController::setServoVelocities(const ::TeRK::IntArray& servoVelocities, const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::TeRK::ServoController* __del = dynamic_cast< ::IceDelegate::TeRK::ServoController*>(__delBase.get());
	    __del->setServoVelocities(servoVelocities, __ctx);
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
IceProxy::TeRK::ServoController::stopServos(const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::TeRK::ServoController* __del = dynamic_cast< ::IceDelegate::TeRK::ServoController*>(__delBase.get());
	    __del->stopServos(__ctx);
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
IceProxy::TeRK::ServoController::ice_staticId()
{
    return ::TeRK::ServoController::ice_staticId();
}

::IceInternal::Handle< ::IceDelegateM::Ice::Object>
IceProxy::TeRK::ServoController::__createDelegateM()
{
    return ::IceInternal::Handle< ::IceDelegateM::Ice::Object>(new ::IceDelegateM::TeRK::ServoController);
}

::IceInternal::Handle< ::IceDelegateD::Ice::Object>
IceProxy::TeRK::ServoController::__createDelegateD()
{
    return ::IceInternal::Handle< ::IceDelegateD::Ice::Object>(new ::IceDelegateD::TeRK::ServoController);
}

bool
IceProxy::TeRK::operator==(const ::IceProxy::TeRK::ServoController& l, const ::IceProxy::TeRK::ServoController& r)
{
    return static_cast<const ::IceProxy::Ice::Object&>(l) == static_cast<const ::IceProxy::Ice::Object&>(r);
}

bool
IceProxy::TeRK::operator!=(const ::IceProxy::TeRK::ServoController& l, const ::IceProxy::TeRK::ServoController& r)
{
    return static_cast<const ::IceProxy::Ice::Object&>(l) != static_cast<const ::IceProxy::Ice::Object&>(r);
}

bool
IceProxy::TeRK::operator<(const ::IceProxy::TeRK::ServoController& l, const ::IceProxy::TeRK::ServoController& r)
{
    return static_cast<const ::IceProxy::Ice::Object&>(l) < static_cast<const ::IceProxy::Ice::Object&>(r);
}

bool
IceProxy::TeRK::operator<=(const ::IceProxy::TeRK::ServoController& l, const ::IceProxy::TeRK::ServoController& r)
{
    return l < r || l == r;
}

bool
IceProxy::TeRK::operator>(const ::IceProxy::TeRK::ServoController& l, const ::IceProxy::TeRK::ServoController& r)
{
    return !(l < r) && !(l == r);
}

bool
IceProxy::TeRK::operator>=(const ::IceProxy::TeRK::ServoController& l, const ::IceProxy::TeRK::ServoController& r)
{
    return !(l < r);
}

void
IceProxy::TeRK::VideoStreamerClient::newFrame(const ::TeRK::Image& frame, const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::TeRK::VideoStreamerClient* __del = dynamic_cast< ::IceDelegate::TeRK::VideoStreamerClient*>(__delBase.get());
	    __del->newFrame(frame, __ctx);
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
IceProxy::TeRK::VideoStreamerClient::ice_staticId()
{
    return ::TeRK::VideoStreamerClient::ice_staticId();
}

::IceInternal::Handle< ::IceDelegateM::Ice::Object>
IceProxy::TeRK::VideoStreamerClient::__createDelegateM()
{
    return ::IceInternal::Handle< ::IceDelegateM::Ice::Object>(new ::IceDelegateM::TeRK::VideoStreamerClient);
}

::IceInternal::Handle< ::IceDelegateD::Ice::Object>
IceProxy::TeRK::VideoStreamerClient::__createDelegateD()
{
    return ::IceInternal::Handle< ::IceDelegateD::Ice::Object>(new ::IceDelegateD::TeRK::VideoStreamerClient);
}

bool
IceProxy::TeRK::operator==(const ::IceProxy::TeRK::VideoStreamerClient& l, const ::IceProxy::TeRK::VideoStreamerClient& r)
{
    return static_cast<const ::IceProxy::Ice::Object&>(l) == static_cast<const ::IceProxy::Ice::Object&>(r);
}

bool
IceProxy::TeRK::operator!=(const ::IceProxy::TeRK::VideoStreamerClient& l, const ::IceProxy::TeRK::VideoStreamerClient& r)
{
    return static_cast<const ::IceProxy::Ice::Object&>(l) != static_cast<const ::IceProxy::Ice::Object&>(r);
}

bool
IceProxy::TeRK::operator<(const ::IceProxy::TeRK::VideoStreamerClient& l, const ::IceProxy::TeRK::VideoStreamerClient& r)
{
    return static_cast<const ::IceProxy::Ice::Object&>(l) < static_cast<const ::IceProxy::Ice::Object&>(r);
}

bool
IceProxy::TeRK::operator<=(const ::IceProxy::TeRK::VideoStreamerClient& l, const ::IceProxy::TeRK::VideoStreamerClient& r)
{
    return l < r || l == r;
}

bool
IceProxy::TeRK::operator>(const ::IceProxy::TeRK::VideoStreamerClient& l, const ::IceProxy::TeRK::VideoStreamerClient& r)
{
    return !(l < r) && !(l == r);
}

bool
IceProxy::TeRK::operator>=(const ::IceProxy::TeRK::VideoStreamerClient& l, const ::IceProxy::TeRK::VideoStreamerClient& r)
{
    return !(l < r);
}

void
IceProxy::TeRK::VideoStreamerServer::startVideoStream(const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__TeRK__VideoStreamerServer__startVideoStream_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::TeRK::VideoStreamerServer* __del = dynamic_cast< ::IceDelegate::TeRK::VideoStreamerServer*>(__delBase.get());
	    __del->startVideoStream(__ctx);
	    return;
	}
	catch(const ::IceInternal::LocalExceptionWrapper& __ex)
	{
	    __handleExceptionWrapperRelaxed(__ex, __cnt);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    __handleException(__ex, __cnt);
	}
    }
}

void
IceProxy::TeRK::VideoStreamerServer::startVideoStream_async(const ::TeRK::AMI_VideoStreamerServer_startVideoStreamPtr& __cb)
{
    startVideoStream_async(__cb, __defaultContext());
}

void
IceProxy::TeRK::VideoStreamerServer::startVideoStream_async(const ::TeRK::AMI_VideoStreamerServer_startVideoStreamPtr& __cb, const ::Ice::Context& __ctx)
{
    __cb->__invoke(this, __ctx);
}

void
IceProxy::TeRK::VideoStreamerServer::stopVideoStream(const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__TeRK__VideoStreamerServer__stopVideoStream_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::TeRK::VideoStreamerServer* __del = dynamic_cast< ::IceDelegate::TeRK::VideoStreamerServer*>(__delBase.get());
	    __del->stopVideoStream(__ctx);
	    return;
	}
	catch(const ::IceInternal::LocalExceptionWrapper& __ex)
	{
	    __handleExceptionWrapperRelaxed(__ex, __cnt);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    __handleException(__ex, __cnt);
	}
    }
}

void
IceProxy::TeRK::VideoStreamerServer::stopVideoStream_async(const ::TeRK::AMI_VideoStreamerServer_stopVideoStreamPtr& __cb)
{
    stopVideoStream_async(__cb, __defaultContext());
}

void
IceProxy::TeRK::VideoStreamerServer::stopVideoStream_async(const ::TeRK::AMI_VideoStreamerServer_stopVideoStreamPtr& __cb, const ::Ice::Context& __ctx)
{
    __cb->__invoke(this, __ctx);
}

::Ice::Int
IceProxy::TeRK::VideoStreamerServer::startCamera(const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__TeRK__VideoStreamerServer__startCamera_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::TeRK::VideoStreamerServer* __del = dynamic_cast< ::IceDelegate::TeRK::VideoStreamerServer*>(__delBase.get());
	    return __del->startCamera(__ctx);
	}
	catch(const ::IceInternal::LocalExceptionWrapper& __ex)
	{
	    __handleExceptionWrapperRelaxed(__ex, __cnt);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    __handleException(__ex, __cnt);
	}
    }
}

void
IceProxy::TeRK::VideoStreamerServer::startCamera_async(const ::TeRK::AMI_VideoStreamerServer_startCameraPtr& __cb)
{
    startCamera_async(__cb, __defaultContext());
}

void
IceProxy::TeRK::VideoStreamerServer::startCamera_async(const ::TeRK::AMI_VideoStreamerServer_startCameraPtr& __cb, const ::Ice::Context& __ctx)
{
    __cb->__invoke(this, __ctx);
}

::Ice::Int
IceProxy::TeRK::VideoStreamerServer::stopCamera(const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__TeRK__VideoStreamerServer__stopCamera_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::TeRK::VideoStreamerServer* __del = dynamic_cast< ::IceDelegate::TeRK::VideoStreamerServer*>(__delBase.get());
	    return __del->stopCamera(__ctx);
	}
	catch(const ::IceInternal::LocalExceptionWrapper& __ex)
	{
	    __handleExceptionWrapperRelaxed(__ex, __cnt);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    __handleException(__ex, __cnt);
	}
    }
}

void
IceProxy::TeRK::VideoStreamerServer::stopCamera_async(const ::TeRK::AMI_VideoStreamerServer_stopCameraPtr& __cb)
{
    stopCamera_async(__cb, __defaultContext());
}

void
IceProxy::TeRK::VideoStreamerServer::stopCamera_async(const ::TeRK::AMI_VideoStreamerServer_stopCameraPtr& __cb, const ::Ice::Context& __ctx)
{
    __cb->__invoke(this, __ctx);
}

::TeRK::Image
IceProxy::TeRK::VideoStreamerServer::getFrame(::Ice::Int frameNumber, const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__TeRK__VideoStreamerServer__getFrame_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::TeRK::VideoStreamerServer* __del = dynamic_cast< ::IceDelegate::TeRK::VideoStreamerServer*>(__delBase.get());
	    return __del->getFrame(frameNumber, __ctx);
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
IceProxy::TeRK::VideoStreamerServer::getFrame_async(const ::TeRK::AMI_VideoStreamerServer_getFramePtr& __cb, ::Ice::Int frameNumber)
{
    getFrame_async(__cb, frameNumber, __defaultContext());
}

void
IceProxy::TeRK::VideoStreamerServer::getFrame_async(const ::TeRK::AMI_VideoStreamerServer_getFramePtr& __cb, ::Ice::Int frameNumber, const ::Ice::Context& __ctx)
{
    __cb->__invoke(this, frameNumber, __ctx);
}

const ::std::string&
IceProxy::TeRK::VideoStreamerServer::ice_staticId()
{
    return ::TeRK::VideoStreamerServer::ice_staticId();
}

::IceInternal::Handle< ::IceDelegateM::Ice::Object>
IceProxy::TeRK::VideoStreamerServer::__createDelegateM()
{
    return ::IceInternal::Handle< ::IceDelegateM::Ice::Object>(new ::IceDelegateM::TeRK::VideoStreamerServer);
}

::IceInternal::Handle< ::IceDelegateD::Ice::Object>
IceProxy::TeRK::VideoStreamerServer::__createDelegateD()
{
    return ::IceInternal::Handle< ::IceDelegateD::Ice::Object>(new ::IceDelegateD::TeRK::VideoStreamerServer);
}

bool
IceProxy::TeRK::operator==(const ::IceProxy::TeRK::VideoStreamerServer& l, const ::IceProxy::TeRK::VideoStreamerServer& r)
{
    return static_cast<const ::IceProxy::Ice::Object&>(l) == static_cast<const ::IceProxy::Ice::Object&>(r);
}

bool
IceProxy::TeRK::operator!=(const ::IceProxy::TeRK::VideoStreamerServer& l, const ::IceProxy::TeRK::VideoStreamerServer& r)
{
    return static_cast<const ::IceProxy::Ice::Object&>(l) != static_cast<const ::IceProxy::Ice::Object&>(r);
}

bool
IceProxy::TeRK::operator<(const ::IceProxy::TeRK::VideoStreamerServer& l, const ::IceProxy::TeRK::VideoStreamerServer& r)
{
    return static_cast<const ::IceProxy::Ice::Object&>(l) < static_cast<const ::IceProxy::Ice::Object&>(r);
}

bool
IceProxy::TeRK::operator<=(const ::IceProxy::TeRK::VideoStreamerServer& l, const ::IceProxy::TeRK::VideoStreamerServer& r)
{
    return l < r || l == r;
}

bool
IceProxy::TeRK::operator>(const ::IceProxy::TeRK::VideoStreamerServer& l, const ::IceProxy::TeRK::VideoStreamerServer& r)
{
    return !(l < r) && !(l == r);
}

bool
IceProxy::TeRK::operator>=(const ::IceProxy::TeRK::VideoStreamerServer& l, const ::IceProxy::TeRK::VideoStreamerServer& r)
{
    return !(l < r);
}

::TeRK::ProxyTypeIdToIdentityMap
IceProxy::TeRK::TerkUser::getSupportedServices(const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__TeRK__TerkUser__getSupportedServices_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::TeRK::TerkUser* __del = dynamic_cast< ::IceDelegate::TeRK::TerkUser*>(__delBase.get());
	    return __del->getSupportedServices(__ctx);
	}
	catch(const ::IceInternal::LocalExceptionWrapper& __ex)
	{
	    __handleExceptionWrapperRelaxed(__ex, __cnt);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    __handleException(__ex, __cnt);
	}
    }
}

void
IceProxy::TeRK::TerkUser::getSupportedServices_async(const ::TeRK::AMI_TerkUser_getSupportedServicesPtr& __cb)
{
    getSupportedServices_async(__cb, __defaultContext());
}

void
IceProxy::TeRK::TerkUser::getSupportedServices_async(const ::TeRK::AMI_TerkUser_getSupportedServicesPtr& __cb, const ::Ice::Context& __ctx)
{
    __cb->__invoke(this, __ctx);
}

const ::std::string&
IceProxy::TeRK::TerkUser::ice_staticId()
{
    return ::TeRK::TerkUser::ice_staticId();
}

::IceInternal::Handle< ::IceDelegateM::Ice::Object>
IceProxy::TeRK::TerkUser::__createDelegateM()
{
    return ::IceInternal::Handle< ::IceDelegateM::Ice::Object>(new ::IceDelegateM::TeRK::TerkUser);
}

::IceInternal::Handle< ::IceDelegateD::Ice::Object>
IceProxy::TeRK::TerkUser::__createDelegateD()
{
    return ::IceInternal::Handle< ::IceDelegateD::Ice::Object>(new ::IceDelegateD::TeRK::TerkUser);
}

bool
IceProxy::TeRK::operator==(const ::IceProxy::TeRK::TerkUser& l, const ::IceProxy::TeRK::TerkUser& r)
{
    return static_cast<const ::IceProxy::Ice::Object&>(l) == static_cast<const ::IceProxy::Ice::Object&>(r);
}

bool
IceProxy::TeRK::operator!=(const ::IceProxy::TeRK::TerkUser& l, const ::IceProxy::TeRK::TerkUser& r)
{
    return static_cast<const ::IceProxy::Ice::Object&>(l) != static_cast<const ::IceProxy::Ice::Object&>(r);
}

bool
IceProxy::TeRK::operator<(const ::IceProxy::TeRK::TerkUser& l, const ::IceProxy::TeRK::TerkUser& r)
{
    return static_cast<const ::IceProxy::Ice::Object&>(l) < static_cast<const ::IceProxy::Ice::Object&>(r);
}

bool
IceProxy::TeRK::operator<=(const ::IceProxy::TeRK::TerkUser& l, const ::IceProxy::TeRK::TerkUser& r)
{
    return l < r || l == r;
}

bool
IceProxy::TeRK::operator>(const ::IceProxy::TeRK::TerkUser& l, const ::IceProxy::TeRK::TerkUser& r)
{
    return !(l < r) && !(l == r);
}

bool
IceProxy::TeRK::operator>=(const ::IceProxy::TeRK::TerkUser& l, const ::IceProxy::TeRK::TerkUser& r)
{
    return !(l < r);
}

::TeRK::QwerkState
IceProxy::TeRK::Qwerk::getState(const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__TeRK__Qwerk__getState_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::TeRK::Qwerk* __del = dynamic_cast< ::IceDelegate::TeRK::Qwerk*>(__delBase.get());
	    return __del->getState(__ctx);
	}
	catch(const ::IceInternal::LocalExceptionWrapper& __ex)
	{
	    __handleExceptionWrapperRelaxed(__ex, __cnt);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    __handleException(__ex, __cnt);
	}
    }
}

void
IceProxy::TeRK::Qwerk::getState_async(const ::TeRK::AMI_Qwerk_getStatePtr& __cb)
{
    getState_async(__cb, __defaultContext());
}

void
IceProxy::TeRK::Qwerk::getState_async(const ::TeRK::AMI_Qwerk_getStatePtr& __cb, const ::Ice::Context& __ctx)
{
    __cb->__invoke(this, __ctx);
}

::TeRK::QwerkState
IceProxy::TeRK::Qwerk::execute(const ::TeRK::QwerkCommand& command, const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__TeRK__Qwerk__execute_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::TeRK::Qwerk* __del = dynamic_cast< ::IceDelegate::TeRK::Qwerk*>(__delBase.get());
	    return __del->execute(command, __ctx);
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
IceProxy::TeRK::Qwerk::execute_async(const ::TeRK::AMI_Qwerk_executePtr& __cb, const ::TeRK::QwerkCommand& command)
{
    execute_async(__cb, command, __defaultContext());
}

void
IceProxy::TeRK::Qwerk::execute_async(const ::TeRK::AMI_Qwerk_executePtr& __cb, const ::TeRK::QwerkCommand& command, const ::Ice::Context& __ctx)
{
    __cb->__invoke(this, command, __ctx);
}

::TeRK::QwerkState
IceProxy::TeRK::Qwerk::emergencyStop(const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__TeRK__Qwerk__emergencyStop_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::TeRK::Qwerk* __del = dynamic_cast< ::IceDelegate::TeRK::Qwerk*>(__delBase.get());
	    return __del->emergencyStop(__ctx);
	}
	catch(const ::IceInternal::LocalExceptionWrapper& __ex)
	{
	    __handleExceptionWrapperRelaxed(__ex, __cnt);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    __handleException(__ex, __cnt);
	}
    }
}

void
IceProxy::TeRK::Qwerk::emergencyStop_async(const ::TeRK::AMI_Qwerk_emergencyStopPtr& __cb)
{
    emergencyStop_async(__cb, __defaultContext());
}

void
IceProxy::TeRK::Qwerk::emergencyStop_async(const ::TeRK::AMI_Qwerk_emergencyStopPtr& __cb, const ::Ice::Context& __ctx)
{
    __cb->__invoke(this, __ctx);
}

::TeRK::ProxyTypeIdToIdentityMap
IceProxy::TeRK::Qwerk::getCommandControllerTypeToProxyIdentityMap(const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__TeRK__Qwerk__getCommandControllerTypeToProxyIdentityMap_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::TeRK::Qwerk* __del = dynamic_cast< ::IceDelegate::TeRK::Qwerk*>(__delBase.get());
	    return __del->getCommandControllerTypeToProxyIdentityMap(__ctx);
	}
	catch(const ::IceInternal::LocalExceptionWrapper& __ex)
	{
	    __handleExceptionWrapperRelaxed(__ex, __cnt);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    __handleException(__ex, __cnt);
	}
    }
}

void
IceProxy::TeRK::Qwerk::getCommandControllerTypeToProxyIdentityMap_async(const ::TeRK::AMI_Qwerk_getCommandControllerTypeToProxyIdentityMapPtr& __cb)
{
    getCommandControllerTypeToProxyIdentityMap_async(__cb, __defaultContext());
}

void
IceProxy::TeRK::Qwerk::getCommandControllerTypeToProxyIdentityMap_async(const ::TeRK::AMI_Qwerk_getCommandControllerTypeToProxyIdentityMapPtr& __cb, const ::Ice::Context& __ctx)
{
    __cb->__invoke(this, __ctx);
}

const ::std::string&
IceProxy::TeRK::Qwerk::ice_staticId()
{
    return ::TeRK::Qwerk::ice_staticId();
}

::IceInternal::Handle< ::IceDelegateM::Ice::Object>
IceProxy::TeRK::Qwerk::__createDelegateM()
{
    return ::IceInternal::Handle< ::IceDelegateM::Ice::Object>(new ::IceDelegateM::TeRK::Qwerk);
}

::IceInternal::Handle< ::IceDelegateD::Ice::Object>
IceProxy::TeRK::Qwerk::__createDelegateD()
{
    return ::IceInternal::Handle< ::IceDelegateD::Ice::Object>(new ::IceDelegateD::TeRK::Qwerk);
}

bool
IceProxy::TeRK::operator==(const ::IceProxy::TeRK::Qwerk& l, const ::IceProxy::TeRK::Qwerk& r)
{
    return static_cast<const ::IceProxy::Ice::Object&>(l) == static_cast<const ::IceProxy::Ice::Object&>(r);
}

bool
IceProxy::TeRK::operator!=(const ::IceProxy::TeRK::Qwerk& l, const ::IceProxy::TeRK::Qwerk& r)
{
    return static_cast<const ::IceProxy::Ice::Object&>(l) != static_cast<const ::IceProxy::Ice::Object&>(r);
}

bool
IceProxy::TeRK::operator<(const ::IceProxy::TeRK::Qwerk& l, const ::IceProxy::TeRK::Qwerk& r)
{
    return static_cast<const ::IceProxy::Ice::Object&>(l) < static_cast<const ::IceProxy::Ice::Object&>(r);
}

bool
IceProxy::TeRK::operator<=(const ::IceProxy::TeRK::Qwerk& l, const ::IceProxy::TeRK::Qwerk& r)
{
    return l < r || l == r;
}

bool
IceProxy::TeRK::operator>(const ::IceProxy::TeRK::Qwerk& l, const ::IceProxy::TeRK::Qwerk& r)
{
    return !(l < r) && !(l == r);
}

bool
IceProxy::TeRK::operator>=(const ::IceProxy::TeRK::Qwerk& l, const ::IceProxy::TeRK::Qwerk& r)
{
    return !(l < r);
}

const ::std::string&
IceProxy::TeRK::TerkClient::ice_staticId()
{
    return ::TeRK::TerkClient::ice_staticId();
}

::IceInternal::Handle< ::IceDelegateM::Ice::Object>
IceProxy::TeRK::TerkClient::__createDelegateM()
{
    return ::IceInternal::Handle< ::IceDelegateM::Ice::Object>(new ::IceDelegateM::TeRK::TerkClient);
}

::IceInternal::Handle< ::IceDelegateD::Ice::Object>
IceProxy::TeRK::TerkClient::__createDelegateD()
{
    return ::IceInternal::Handle< ::IceDelegateD::Ice::Object>(new ::IceDelegateD::TeRK::TerkClient);
}

bool
IceProxy::TeRK::operator==(const ::IceProxy::TeRK::TerkClient& l, const ::IceProxy::TeRK::TerkClient& r)
{
    return static_cast<const ::IceProxy::Ice::Object&>(l) == static_cast<const ::IceProxy::Ice::Object&>(r);
}

bool
IceProxy::TeRK::operator!=(const ::IceProxy::TeRK::TerkClient& l, const ::IceProxy::TeRK::TerkClient& r)
{
    return static_cast<const ::IceProxy::Ice::Object&>(l) != static_cast<const ::IceProxy::Ice::Object&>(r);
}

bool
IceProxy::TeRK::operator<(const ::IceProxy::TeRK::TerkClient& l, const ::IceProxy::TeRK::TerkClient& r)
{
    return static_cast<const ::IceProxy::Ice::Object&>(l) < static_cast<const ::IceProxy::Ice::Object&>(r);
}

bool
IceProxy::TeRK::operator<=(const ::IceProxy::TeRK::TerkClient& l, const ::IceProxy::TeRK::TerkClient& r)
{
    return l < r || l == r;
}

bool
IceProxy::TeRK::operator>(const ::IceProxy::TeRK::TerkClient& l, const ::IceProxy::TeRK::TerkClient& r)
{
    return !(l < r) && !(l == r);
}

bool
IceProxy::TeRK::operator>=(const ::IceProxy::TeRK::TerkClient& l, const ::IceProxy::TeRK::TerkClient& r)
{
    return !(l < r);
}

::std::string
IceDelegateM::TeRK::PropertyManager::getProperty(const ::std::string& key, const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __TeRK__PropertyManager__getProperty_name, ::Ice::Normal, __context, __compress);
    try
    {
	::IceInternal::BasicStream* __os = __og.os();
	__os->write(key);
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
	::std::string __ret;
	__is->read(__ret);
	return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
	throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

::TeRK::PropertyMap
IceDelegateM::TeRK::PropertyManager::getProperties(const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __TeRK__PropertyManager__getProperties_name, ::Ice::Normal, __context, __compress);
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
	::TeRK::PropertyMap __ret;
	::TeRK::__read(__is, __ret, ::TeRK::__U__PropertyMap());
	return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
	throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

::TeRK::StringArray
IceDelegateM::TeRK::PropertyManager::getPropertyKeys(const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __TeRK__PropertyManager__getPropertyKeys_name, ::Ice::Normal, __context, __compress);
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
	::TeRK::StringArray __ret;
	__is->read(__ret);
	return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
	throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

void
IceDelegateM::TeRK::PropertyManager::setProperty(const ::std::string& key, const ::std::string& value, const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __TeRK__PropertyManager__setProperty_name, ::Ice::Normal, __context, __compress);
    try
    {
	::IceInternal::BasicStream* __os = __og.os();
	__os->write(key);
	__os->write(value);
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
	    catch(const ::TeRK::ReadOnlyPropertyException&)
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

::TeRK::AnalogInState
IceDelegateM::TeRK::AnalogInController::getState(const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __TeRK__AnalogInController__getState_name, ::Ice::Normal, __context, __compress);
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
	::TeRK::AnalogInState __ret;
	__ret.__read(__is);
	return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
	throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

void
IceDelegateM::TeRK::AudioController::execute(const ::TeRK::AudioCommand& command, const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __TeRK__AudioController__execute_name, ::Ice::Normal, __context, __compress);
    try
    {
	::IceInternal::BasicStream* __os = __og.os();
	command.__write(__os);
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
	    catch(const ::TeRK::AudioCommandException&)
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

::TeRK::DigitalInState
IceDelegateM::TeRK::DigitalInController::getState(const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __TeRK__DigitalInController__getState_name, ::Ice::Normal, __context, __compress);
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
	::TeRK::DigitalInState __ret;
	__ret.__read(__is);
	return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
	throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

void
IceDelegateM::TeRK::DigitalOutController::execute(const ::TeRK::DigitalOutCommand& command, const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __TeRK__DigitalOutController__execute_name, ::Ice::Normal, __context, __compress);
    try
    {
	::IceInternal::BasicStream* __os = __og.os();
	command.__write(__os);
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
	    catch(const ::TeRK::DigitalOutCommandException&)
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
IceDelegateM::TeRK::LEDController::execute(const ::TeRK::LEDCommand& command, const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __TeRK__LEDController__execute_name, ::Ice::Normal, __context, __compress);
    try
    {
	::IceInternal::BasicStream* __os = __og.os();
	command.__write(__os);
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
	    catch(const ::TeRK::LEDCommandException&)
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

::TeRK::MotorState
IceDelegateM::TeRK::MotorController::execute(const ::TeRK::MotorCommand& command, const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __TeRK__MotorController__execute_name, ::Ice::Normal, __context, __compress);
    try
    {
	::IceInternal::BasicStream* __os = __og.os();
	command.__write(__os);
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
	    catch(const ::TeRK::MotorCommandException&)
	    {
		throw;
	    }
	    catch(const ::Ice::UserException& __ex)
	    {
		throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
	    }
	}
	::TeRK::MotorState __ret;
	__ret.__read(__is);
	return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
	throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

::Ice::Int
IceDelegateM::TeRK::MotorController::getFrequency(const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __TeRK__MotorController__getFrequency_name, ::Ice::Normal, __context, __compress);
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
	::Ice::Int __ret;
	__is->read(__ret);
	return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
	throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

void
IceDelegateM::TeRK::MotorController::startMotorBufferRecord(const ::TeRK::BooleanArray& motorMask, const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __TeRK__MotorController__startMotorBufferRecord_name, ::Ice::Normal, __context, __compress);
    try
    {
	::IceInternal::BasicStream* __os = __og.os();
	__os->write(motorMask);
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
	    catch(const ::TeRK::MotorException&)
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
IceDelegateM::TeRK::MotorController::stopMotorBufferRecord(const ::TeRK::BooleanArray& motorMask, const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __TeRK__MotorController__stopMotorBufferRecord_name, ::Ice::Idempotent, __context, __compress);
    try
    {
	::IceInternal::BasicStream* __os = __og.os();
	__os->write(motorMask);
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
	    catch(const ::TeRK::MotorException&)
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

::TeRK::MotorBufferArray
IceDelegateM::TeRK::MotorController::getMotorBuffers(const ::TeRK::BooleanArray& motorMask, const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __TeRK__MotorController__getMotorBuffers_name, ::Ice::Nonmutating, __context, __compress);
    try
    {
	::IceInternal::BasicStream* __os = __og.os();
	__os->write(motorMask);
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
	    catch(const ::TeRK::MotorException&)
	    {
		throw;
	    }
	    catch(const ::Ice::UserException& __ex)
	    {
		throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
	    }
	}
	::TeRK::MotorBufferArray __ret;
	::TeRK::__read(__is, __ret, ::TeRK::__U__MotorBufferArray());
	return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
	throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

void
IceDelegateM::TeRK::MotorController::setMotorBuffer(const ::TeRK::BooleanArray& motorMask, const ::TeRK::MotorBufferArray& motorBuffers, const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __TeRK__MotorController__setMotorBuffer_name, ::Ice::Idempotent, __context, __compress);
    try
    {
	::IceInternal::BasicStream* __os = __og.os();
	__os->write(motorMask);
	if(motorBuffers.size() == 0)
	{
	    __os->writeSize(0);
	}
	else
	{
	    ::TeRK::__write(__os, &motorBuffers[0], &motorBuffers[0] + motorBuffers.size(), ::TeRK::__U__MotorBufferArray());
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
	    catch(const ::TeRK::MotorException&)
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
IceDelegateM::TeRK::MotorController::playMotorBuffer(const ::TeRK::BooleanArray& motorMask, const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __TeRK__MotorController__playMotorBuffer_name, ::Ice::Normal, __context, __compress);
    try
    {
	::IceInternal::BasicStream* __os = __og.os();
	__os->write(motorMask);
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
	    catch(const ::TeRK::MotorException&)
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
IceDelegateM::TeRK::MotorController::driveForward(const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __TeRK__MotorController__driveForward_name, ::Ice::Normal, __context, __compress);
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
IceDelegateM::TeRK::MotorController::driveBack(const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __TeRK__MotorController__driveBack_name, ::Ice::Normal, __context, __compress);
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
IceDelegateM::TeRK::MotorController::spinLeft(const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __TeRK__MotorController__spinLeft_name, ::Ice::Normal, __context, __compress);
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
IceDelegateM::TeRK::MotorController::spinRight(const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __TeRK__MotorController__spinRight_name, ::Ice::Normal, __context, __compress);
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
IceDelegateM::TeRK::MotorController::setMotorVelocities(const ::TeRK::IntArray& motorValues, const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __TeRK__MotorController__setMotorVelocities_name, ::Ice::Idempotent, __context, __compress);
    try
    {
	::IceInternal::BasicStream* __os = __og.os();
	if(motorValues.size() == 0)
	{
	    __os->writeSize(0);
	}
	else
	{
	    __os->write(&motorValues[0], &motorValues[0] + motorValues.size());
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
	    catch(const ::TeRK::GenericError&)
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
IceDelegateM::TeRK::MotorController::stopMotors(const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __TeRK__MotorController__stopMotors_name, ::Ice::Normal, __context, __compress);
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

::TeRK::ServoState
IceDelegateM::TeRK::ServoController::execute(const ::TeRK::ServoCommand& command, const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __TeRK__ServoController__execute_name, ::Ice::Normal, __context, __compress);
    try
    {
	::IceInternal::BasicStream* __os = __og.os();
	command.__write(__os);
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
	    catch(const ::TeRK::ServoCommandException&)
	    {
		throw;
	    }
	    catch(const ::Ice::UserException& __ex)
	    {
		throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
	    }
	}
	::TeRK::ServoState __ret;
	__ret.__read(__is);
	return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
	throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

void
IceDelegateM::TeRK::ServoController::cameraTiltUp(const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __TeRK__ServoController__cameraTiltUp_name, ::Ice::Normal, __context, __compress);
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
IceDelegateM::TeRK::ServoController::cameraTiltDown(const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __TeRK__ServoController__cameraTiltDown_name, ::Ice::Normal, __context, __compress);
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
IceDelegateM::TeRK::ServoController::cameraPanLeft(const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __TeRK__ServoController__cameraPanLeft_name, ::Ice::Normal, __context, __compress);
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
IceDelegateM::TeRK::ServoController::cameraPanRight(const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __TeRK__ServoController__cameraPanRight_name, ::Ice::Normal, __context, __compress);
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
IceDelegateM::TeRK::ServoController::setServoPositions(const ::TeRK::IntArray& servoPositions, const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __TeRK__ServoController__setServoPositions_name, ::Ice::Normal, __context, __compress);
    try
    {
	::IceInternal::BasicStream* __os = __og.os();
	if(servoPositions.size() == 0)
	{
	    __os->writeSize(0);
	}
	else
	{
	    __os->write(&servoPositions[0], &servoPositions[0] + servoPositions.size());
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
IceDelegateM::TeRK::ServoController::setServoVelocities(const ::TeRK::IntArray& servoVelocities, const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __TeRK__ServoController__setServoVelocities_name, ::Ice::Normal, __context, __compress);
    try
    {
	::IceInternal::BasicStream* __os = __og.os();
	if(servoVelocities.size() == 0)
	{
	    __os->writeSize(0);
	}
	else
	{
	    __os->write(&servoVelocities[0], &servoVelocities[0] + servoVelocities.size());
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
IceDelegateM::TeRK::ServoController::stopServos(const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __TeRK__ServoController__stopServos_name, ::Ice::Normal, __context, __compress);
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
IceDelegateM::TeRK::VideoStreamerClient::newFrame(const ::TeRK::Image& frame, const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __TeRK__VideoStreamerClient__newFrame_name, ::Ice::Normal, __context, __compress);
    try
    {
	::IceInternal::BasicStream* __os = __og.os();
	frame.__write(__os);
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
IceDelegateM::TeRK::VideoStreamerServer::startVideoStream(const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __TeRK__VideoStreamerServer__startVideoStream_name, ::Ice::Idempotent, __context, __compress);
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
	    catch(const ::TeRK::VideoException&)
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
IceDelegateM::TeRK::VideoStreamerServer::stopVideoStream(const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __TeRK__VideoStreamerServer__stopVideoStream_name, ::Ice::Idempotent, __context, __compress);
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
	    catch(const ::TeRK::VideoException&)
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

::Ice::Int
IceDelegateM::TeRK::VideoStreamerServer::startCamera(const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __TeRK__VideoStreamerServer__startCamera_name, ::Ice::Idempotent, __context, __compress);
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
	    catch(const ::TeRK::VideoException&)
	    {
		throw;
	    }
	    catch(const ::Ice::UserException& __ex)
	    {
		throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
	    }
	}
	::Ice::Int __ret;
	__is->read(__ret);
	return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
	throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

::Ice::Int
IceDelegateM::TeRK::VideoStreamerServer::stopCamera(const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __TeRK__VideoStreamerServer__stopCamera_name, ::Ice::Idempotent, __context, __compress);
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
	    catch(const ::TeRK::VideoException&)
	    {
		throw;
	    }
	    catch(const ::Ice::UserException& __ex)
	    {
		throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
	    }
	}
	::Ice::Int __ret;
	__is->read(__ret);
	return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
	throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

::TeRK::Image
IceDelegateM::TeRK::VideoStreamerServer::getFrame(::Ice::Int frameNumber, const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __TeRK__VideoStreamerServer__getFrame_name, ::Ice::Normal, __context, __compress);
    try
    {
	::IceInternal::BasicStream* __os = __og.os();
	__os->write(frameNumber);
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
	    catch(const ::TeRK::VideoException&)
	    {
		throw;
	    }
	    catch(const ::Ice::UserException& __ex)
	    {
		throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
	    }
	}
	::TeRK::Image __ret;
	__ret.__read(__is);
	return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
	throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

::TeRK::ProxyTypeIdToIdentityMap
IceDelegateM::TeRK::TerkUser::getSupportedServices(const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __TeRK__TerkUser__getSupportedServices_name, ::Ice::Nonmutating, __context, __compress);
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
	::TeRK::ProxyTypeIdToIdentityMap __ret;
	::TeRK::__read(__is, __ret, ::TeRK::__U__ProxyTypeIdToIdentityMap());
	return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
	throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

::TeRK::QwerkState
IceDelegateM::TeRK::Qwerk::getState(const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __TeRK__Qwerk__getState_name, ::Ice::Nonmutating, __context, __compress);
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
	::TeRK::QwerkState __ret;
	__ret.__read(__is);
	return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
	throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

::TeRK::QwerkState
IceDelegateM::TeRK::Qwerk::execute(const ::TeRK::QwerkCommand& command, const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __TeRK__Qwerk__execute_name, ::Ice::Normal, __context, __compress);
    try
    {
	::IceInternal::BasicStream* __os = __og.os();
	command.__write(__os);
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
	    catch(const ::TeRK::CommandException&)
	    {
		throw;
	    }
	    catch(const ::Ice::UserException& __ex)
	    {
		throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
	    }
	}
	::TeRK::QwerkState __ret;
	__ret.__read(__is);
	return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
	throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

::TeRK::QwerkState
IceDelegateM::TeRK::Qwerk::emergencyStop(const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __TeRK__Qwerk__emergencyStop_name, ::Ice::Idempotent, __context, __compress);
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
	::TeRK::QwerkState __ret;
	__ret.__read(__is);
	return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
	throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

::TeRK::ProxyTypeIdToIdentityMap
IceDelegateM::TeRK::Qwerk::getCommandControllerTypeToProxyIdentityMap(const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __TeRK__Qwerk__getCommandControllerTypeToProxyIdentityMap_name, ::Ice::Nonmutating, __context, __compress);
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
	::TeRK::ProxyTypeIdToIdentityMap __ret;
	::TeRK::__read(__is, __ret, ::TeRK::__U__ProxyTypeIdToIdentityMap());
	return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
	throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

::std::string
IceDelegateD::TeRK::PropertyManager::getProperty(const ::std::string& key, const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __TeRK__PropertyManager__getProperty_name, ::Ice::Normal, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::TeRK::PropertyManager* __servant = dynamic_cast< ::TeRK::PropertyManager*>(__direct.servant().get());
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
	    return __servant->getProperty(key, __current);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

::TeRK::PropertyMap
IceDelegateD::TeRK::PropertyManager::getProperties(const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __TeRK__PropertyManager__getProperties_name, ::Ice::Normal, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::TeRK::PropertyManager* __servant = dynamic_cast< ::TeRK::PropertyManager*>(__direct.servant().get());
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
	    return __servant->getProperties(__current);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

::TeRK::StringArray
IceDelegateD::TeRK::PropertyManager::getPropertyKeys(const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __TeRK__PropertyManager__getPropertyKeys_name, ::Ice::Normal, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::TeRK::PropertyManager* __servant = dynamic_cast< ::TeRK::PropertyManager*>(__direct.servant().get());
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
	    return __servant->getPropertyKeys(__current);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

void
IceDelegateD::TeRK::PropertyManager::setProperty(const ::std::string& key, const ::std::string& value, const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __TeRK__PropertyManager__setProperty_name, ::Ice::Normal, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::TeRK::PropertyManager* __servant = dynamic_cast< ::TeRK::PropertyManager*>(__direct.servant().get());
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
	    __servant->setProperty(key, value, __current);
	    return;
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

::TeRK::AnalogInState
IceDelegateD::TeRK::AnalogInController::getState(const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __TeRK__AnalogInController__getState_name, ::Ice::Normal, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::TeRK::AnalogInController* __servant = dynamic_cast< ::TeRK::AnalogInController*>(__direct.servant().get());
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
	    return __servant->getState(__current);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

void
IceDelegateD::TeRK::AudioController::execute(const ::TeRK::AudioCommand& command, const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __TeRK__AudioController__execute_name, ::Ice::Normal, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::TeRK::AudioController* __servant = dynamic_cast< ::TeRK::AudioController*>(__direct.servant().get());
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
	    __servant->execute(command, __current);
	    return;
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

::TeRK::DigitalInState
IceDelegateD::TeRK::DigitalInController::getState(const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __TeRK__DigitalInController__getState_name, ::Ice::Normal, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::TeRK::DigitalInController* __servant = dynamic_cast< ::TeRK::DigitalInController*>(__direct.servant().get());
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
	    return __servant->getState(__current);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

void
IceDelegateD::TeRK::DigitalOutController::execute(const ::TeRK::DigitalOutCommand& command, const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __TeRK__DigitalOutController__execute_name, ::Ice::Normal, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::TeRK::DigitalOutController* __servant = dynamic_cast< ::TeRK::DigitalOutController*>(__direct.servant().get());
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
	    __servant->execute(command, __current);
	    return;
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

void
IceDelegateD::TeRK::LEDController::execute(const ::TeRK::LEDCommand& command, const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __TeRK__LEDController__execute_name, ::Ice::Normal, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::TeRK::LEDController* __servant = dynamic_cast< ::TeRK::LEDController*>(__direct.servant().get());
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
	    __servant->execute(command, __current);
	    return;
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

::TeRK::MotorState
IceDelegateD::TeRK::MotorController::execute(const ::TeRK::MotorCommand& command, const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __TeRK__MotorController__execute_name, ::Ice::Normal, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::TeRK::MotorController* __servant = dynamic_cast< ::TeRK::MotorController*>(__direct.servant().get());
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
	    return __servant->execute(command, __current);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

::Ice::Int
IceDelegateD::TeRK::MotorController::getFrequency(const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __TeRK__MotorController__getFrequency_name, ::Ice::Normal, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::TeRK::MotorController* __servant = dynamic_cast< ::TeRK::MotorController*>(__direct.servant().get());
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
	    return __servant->getFrequency(__current);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

void
IceDelegateD::TeRK::MotorController::startMotorBufferRecord(const ::TeRK::BooleanArray& motorMask, const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __TeRK__MotorController__startMotorBufferRecord_name, ::Ice::Normal, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::TeRK::MotorController* __servant = dynamic_cast< ::TeRK::MotorController*>(__direct.servant().get());
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
	    __servant->startMotorBufferRecord(motorMask, __current);
	    return;
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

void
IceDelegateD::TeRK::MotorController::stopMotorBufferRecord(const ::TeRK::BooleanArray& motorMask, const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __TeRK__MotorController__stopMotorBufferRecord_name, ::Ice::Idempotent, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::TeRK::MotorController* __servant = dynamic_cast< ::TeRK::MotorController*>(__direct.servant().get());
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
	    __servant->stopMotorBufferRecord(motorMask, __current);
	    return;
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

::TeRK::MotorBufferArray
IceDelegateD::TeRK::MotorController::getMotorBuffers(const ::TeRK::BooleanArray& motorMask, const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __TeRK__MotorController__getMotorBuffers_name, ::Ice::Nonmutating, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::TeRK::MotorController* __servant = dynamic_cast< ::TeRK::MotorController*>(__direct.servant().get());
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
	    return __servant->getMotorBuffers(motorMask, __current);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

void
IceDelegateD::TeRK::MotorController::setMotorBuffer(const ::TeRK::BooleanArray& motorMask, const ::TeRK::MotorBufferArray& motorBuffers, const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __TeRK__MotorController__setMotorBuffer_name, ::Ice::Idempotent, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::TeRK::MotorController* __servant = dynamic_cast< ::TeRK::MotorController*>(__direct.servant().get());
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
	    __servant->setMotorBuffer(motorMask, motorBuffers, __current);
	    return;
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

void
IceDelegateD::TeRK::MotorController::playMotorBuffer(const ::TeRK::BooleanArray& motorMask, const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __TeRK__MotorController__playMotorBuffer_name, ::Ice::Normal, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::TeRK::MotorController* __servant = dynamic_cast< ::TeRK::MotorController*>(__direct.servant().get());
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
	    __servant->playMotorBuffer(motorMask, __current);
	    return;
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

void
IceDelegateD::TeRK::MotorController::driveForward(const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __TeRK__MotorController__driveForward_name, ::Ice::Normal, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::TeRK::MotorController* __servant = dynamic_cast< ::TeRK::MotorController*>(__direct.servant().get());
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
	    __servant->driveForward(__current);
	    return;
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

void
IceDelegateD::TeRK::MotorController::driveBack(const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __TeRK__MotorController__driveBack_name, ::Ice::Normal, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::TeRK::MotorController* __servant = dynamic_cast< ::TeRK::MotorController*>(__direct.servant().get());
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
	    __servant->driveBack(__current);
	    return;
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

void
IceDelegateD::TeRK::MotorController::spinLeft(const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __TeRK__MotorController__spinLeft_name, ::Ice::Normal, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::TeRK::MotorController* __servant = dynamic_cast< ::TeRK::MotorController*>(__direct.servant().get());
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
	    __servant->spinLeft(__current);
	    return;
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

void
IceDelegateD::TeRK::MotorController::spinRight(const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __TeRK__MotorController__spinRight_name, ::Ice::Normal, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::TeRK::MotorController* __servant = dynamic_cast< ::TeRK::MotorController*>(__direct.servant().get());
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
	    __servant->spinRight(__current);
	    return;
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

void
IceDelegateD::TeRK::MotorController::setMotorVelocities(const ::TeRK::IntArray& motorValues, const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __TeRK__MotorController__setMotorVelocities_name, ::Ice::Idempotent, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::TeRK::MotorController* __servant = dynamic_cast< ::TeRK::MotorController*>(__direct.servant().get());
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
	    __servant->setMotorVelocities(motorValues, __current);
	    return;
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

void
IceDelegateD::TeRK::MotorController::stopMotors(const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __TeRK__MotorController__stopMotors_name, ::Ice::Normal, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::TeRK::MotorController* __servant = dynamic_cast< ::TeRK::MotorController*>(__direct.servant().get());
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
	    __servant->stopMotors(__current);
	    return;
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

::TeRK::ServoState
IceDelegateD::TeRK::ServoController::execute(const ::TeRK::ServoCommand& command, const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __TeRK__ServoController__execute_name, ::Ice::Normal, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::TeRK::ServoController* __servant = dynamic_cast< ::TeRK::ServoController*>(__direct.servant().get());
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
	    return __servant->execute(command, __current);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

void
IceDelegateD::TeRK::ServoController::cameraTiltUp(const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __TeRK__ServoController__cameraTiltUp_name, ::Ice::Normal, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::TeRK::ServoController* __servant = dynamic_cast< ::TeRK::ServoController*>(__direct.servant().get());
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
	    __servant->cameraTiltUp(__current);
	    return;
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

void
IceDelegateD::TeRK::ServoController::cameraTiltDown(const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __TeRK__ServoController__cameraTiltDown_name, ::Ice::Normal, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::TeRK::ServoController* __servant = dynamic_cast< ::TeRK::ServoController*>(__direct.servant().get());
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
	    __servant->cameraTiltDown(__current);
	    return;
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

void
IceDelegateD::TeRK::ServoController::cameraPanLeft(const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __TeRK__ServoController__cameraPanLeft_name, ::Ice::Normal, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::TeRK::ServoController* __servant = dynamic_cast< ::TeRK::ServoController*>(__direct.servant().get());
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
	    __servant->cameraPanLeft(__current);
	    return;
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

void
IceDelegateD::TeRK::ServoController::cameraPanRight(const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __TeRK__ServoController__cameraPanRight_name, ::Ice::Normal, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::TeRK::ServoController* __servant = dynamic_cast< ::TeRK::ServoController*>(__direct.servant().get());
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
	    __servant->cameraPanRight(__current);
	    return;
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

void
IceDelegateD::TeRK::ServoController::setServoPositions(const ::TeRK::IntArray& servoPositions, const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __TeRK__ServoController__setServoPositions_name, ::Ice::Normal, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::TeRK::ServoController* __servant = dynamic_cast< ::TeRK::ServoController*>(__direct.servant().get());
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
	    __servant->setServoPositions(servoPositions, __current);
	    return;
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

void
IceDelegateD::TeRK::ServoController::setServoVelocities(const ::TeRK::IntArray& servoVelocities, const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __TeRK__ServoController__setServoVelocities_name, ::Ice::Normal, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::TeRK::ServoController* __servant = dynamic_cast< ::TeRK::ServoController*>(__direct.servant().get());
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
	    __servant->setServoVelocities(servoVelocities, __current);
	    return;
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

void
IceDelegateD::TeRK::ServoController::stopServos(const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __TeRK__ServoController__stopServos_name, ::Ice::Normal, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::TeRK::ServoController* __servant = dynamic_cast< ::TeRK::ServoController*>(__direct.servant().get());
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
	    __servant->stopServos(__current);
	    return;
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

void
IceDelegateD::TeRK::VideoStreamerClient::newFrame(const ::TeRK::Image& frame, const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __TeRK__VideoStreamerClient__newFrame_name, ::Ice::Normal, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::TeRK::VideoStreamerClient* __servant = dynamic_cast< ::TeRK::VideoStreamerClient*>(__direct.servant().get());
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
	    __servant->newFrame(frame, __current);
	    return;
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

void
IceDelegateD::TeRK::VideoStreamerServer::startVideoStream(const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __TeRK__VideoStreamerServer__startVideoStream_name, ::Ice::Idempotent, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::TeRK::VideoStreamerServer* __servant = dynamic_cast< ::TeRK::VideoStreamerServer*>(__direct.servant().get());
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
	    __servant->startVideoStream(__current);
	    return;
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

void
IceDelegateD::TeRK::VideoStreamerServer::stopVideoStream(const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __TeRK__VideoStreamerServer__stopVideoStream_name, ::Ice::Idempotent, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::TeRK::VideoStreamerServer* __servant = dynamic_cast< ::TeRK::VideoStreamerServer*>(__direct.servant().get());
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
	    __servant->stopVideoStream(__current);
	    return;
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

::Ice::Int
IceDelegateD::TeRK::VideoStreamerServer::startCamera(const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __TeRK__VideoStreamerServer__startCamera_name, ::Ice::Idempotent, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::TeRK::VideoStreamerServer* __servant = dynamic_cast< ::TeRK::VideoStreamerServer*>(__direct.servant().get());
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
	    return __servant->startCamera(__current);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

::Ice::Int
IceDelegateD::TeRK::VideoStreamerServer::stopCamera(const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __TeRK__VideoStreamerServer__stopCamera_name, ::Ice::Idempotent, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::TeRK::VideoStreamerServer* __servant = dynamic_cast< ::TeRK::VideoStreamerServer*>(__direct.servant().get());
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
	    return __servant->stopCamera(__current);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

::TeRK::Image
IceDelegateD::TeRK::VideoStreamerServer::getFrame(::Ice::Int frameNumber, const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __TeRK__VideoStreamerServer__getFrame_name, ::Ice::Normal, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::TeRK::VideoStreamerServer* __servant = dynamic_cast< ::TeRK::VideoStreamerServer*>(__direct.servant().get());
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
	    return __servant->getFrame(frameNumber, __current);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

::TeRK::ProxyTypeIdToIdentityMap
IceDelegateD::TeRK::TerkUser::getSupportedServices(const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __TeRK__TerkUser__getSupportedServices_name, ::Ice::Nonmutating, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::TeRK::TerkUser* __servant = dynamic_cast< ::TeRK::TerkUser*>(__direct.servant().get());
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
	    return __servant->getSupportedServices(__current);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

::TeRK::QwerkState
IceDelegateD::TeRK::Qwerk::getState(const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __TeRK__Qwerk__getState_name, ::Ice::Nonmutating, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::TeRK::Qwerk* __servant = dynamic_cast< ::TeRK::Qwerk*>(__direct.servant().get());
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
	    return __servant->getState(__current);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

::TeRK::QwerkState
IceDelegateD::TeRK::Qwerk::execute(const ::TeRK::QwerkCommand& command, const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __TeRK__Qwerk__execute_name, ::Ice::Normal, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::TeRK::Qwerk* __servant = dynamic_cast< ::TeRK::Qwerk*>(__direct.servant().get());
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
	    return __servant->execute(command, __current);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

::TeRK::QwerkState
IceDelegateD::TeRK::Qwerk::emergencyStop(const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __TeRK__Qwerk__emergencyStop_name, ::Ice::Idempotent, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::TeRK::Qwerk* __servant = dynamic_cast< ::TeRK::Qwerk*>(__direct.servant().get());
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
	    return __servant->emergencyStop(__current);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

::TeRK::ProxyTypeIdToIdentityMap
IceDelegateD::TeRK::Qwerk::getCommandControllerTypeToProxyIdentityMap(const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __TeRK__Qwerk__getCommandControllerTypeToProxyIdentityMap_name, ::Ice::Nonmutating, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::TeRK::Qwerk* __servant = dynamic_cast< ::TeRK::Qwerk*>(__direct.servant().get());
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
	    return __servant->getCommandControllerTypeToProxyIdentityMap(__current);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

::Ice::ObjectPtr
TeRK::PropertyManager::ice_clone() const
{
    throw ::Ice::CloneNotImplementedException(__FILE__, __LINE__);
    return 0; // to avoid a warning with some compilers
}

static const ::std::string __TeRK__PropertyManager_ids[2] =
{
    "::Ice::Object",
    "::TeRK::PropertyManager"
};

bool
TeRK::PropertyManager::ice_isA(const ::std::string& _s, const ::Ice::Current&) const
{
    return ::std::binary_search(__TeRK__PropertyManager_ids, __TeRK__PropertyManager_ids + 2, _s);
}

::std::vector< ::std::string>
TeRK::PropertyManager::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&__TeRK__PropertyManager_ids[0], &__TeRK__PropertyManager_ids[2]);
}

const ::std::string&
TeRK::PropertyManager::ice_id(const ::Ice::Current&) const
{
    return __TeRK__PropertyManager_ids[1];
}

const ::std::string&
TeRK::PropertyManager::ice_staticId()
{
    return __TeRK__PropertyManager_ids[1];
}

::IceInternal::DispatchStatus
TeRK::PropertyManager::___getProperty(::IceInternal::Incoming&__inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::std::string key;
    __is->read(key);
    ::std::string __ret = getProperty(key, __current);
    __os->write(__ret);
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
TeRK::PropertyManager::___getProperties(::IceInternal::Incoming&__inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __os = __inS.os();
    ::TeRK::PropertyMap __ret = getProperties(__current);
    ::TeRK::__write(__os, __ret, ::TeRK::__U__PropertyMap());
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
TeRK::PropertyManager::___getPropertyKeys(::IceInternal::Incoming&__inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __os = __inS.os();
    ::TeRK::StringArray __ret = getPropertyKeys(__current);
    if(__ret.size() == 0)
    {
	__os->writeSize(0);
    }
    else
    {
	__os->write(&__ret[0], &__ret[0] + __ret.size());
    }
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
TeRK::PropertyManager::___setProperty(::IceInternal::Incoming&__inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::std::string key;
    ::std::string value;
    __is->read(key);
    __is->read(value);
    try
    {
	setProperty(key, value, __current);
    }
    catch(const ::TeRK::ReadOnlyPropertyException& __ex)
    {
	__os->write(__ex);
	return ::IceInternal::DispatchUserException;
    }
    return ::IceInternal::DispatchOK;
}

static ::std::string __TeRK__PropertyManager_all[] =
{
    "getProperties",
    "getProperty",
    "getPropertyKeys",
    "ice_id",
    "ice_ids",
    "ice_isA",
    "ice_ping",
    "setProperty"
};

::IceInternal::DispatchStatus
TeRK::PropertyManager::__dispatch(::IceInternal::Incoming& in, const ::Ice::Current& current)
{
    ::std::pair< ::std::string*, ::std::string*> r = ::std::equal_range(__TeRK__PropertyManager_all, __TeRK__PropertyManager_all + 8, current.operation);
    if(r.first == r.second)
    {
	return ::IceInternal::DispatchOperationNotExist;
    }

    switch(r.first - __TeRK__PropertyManager_all)
    {
	case 0:
	{
	    return ___getProperties(in, current);
	}
	case 1:
	{
	    return ___getProperty(in, current);
	}
	case 2:
	{
	    return ___getPropertyKeys(in, current);
	}
	case 3:
	{
	    return ___ice_id(in, current);
	}
	case 4:
	{
	    return ___ice_ids(in, current);
	}
	case 5:
	{
	    return ___ice_isA(in, current);
	}
	case 6:
	{
	    return ___ice_ping(in, current);
	}
	case 7:
	{
	    return ___setProperty(in, current);
	}
    }

    assert(false);
    return ::IceInternal::DispatchOperationNotExist;
}

void
TeRK::PropertyManager::__write(::IceInternal::BasicStream* __os) const
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
TeRK::PropertyManager::__read(::IceInternal::BasicStream* __is, bool __rid)
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
TeRK::PropertyManager::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type TeRK::PropertyManager was not generated with stream support";
    throw ex;
}

void
TeRK::PropertyManager::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type TeRK::PropertyManager was not generated with stream support";
    throw ex;
}

void 
TeRK::__patch__PropertyManagerPtr(void* __addr, ::Ice::ObjectPtr& v)
{
    ::TeRK::PropertyManagerPtr* p = static_cast< ::TeRK::PropertyManagerPtr*>(__addr);
    assert(p);
    *p = ::TeRK::PropertyManagerPtr::dynamicCast(v);
    if(v && !*p)
    {
	::Ice::NoObjectFactoryException e(__FILE__, __LINE__);
	e.type = ::TeRK::PropertyManager::ice_staticId();
	throw e;
    }
}

bool
TeRK::operator==(const ::TeRK::PropertyManager& l, const ::TeRK::PropertyManager& r)
{
    return static_cast<const ::Ice::Object&>(l) == static_cast<const ::Ice::Object&>(r);
}

bool
TeRK::operator!=(const ::TeRK::PropertyManager& l, const ::TeRK::PropertyManager& r)
{
    return static_cast<const ::Ice::Object&>(l) != static_cast<const ::Ice::Object&>(r);
}

bool
TeRK::operator<(const ::TeRK::PropertyManager& l, const ::TeRK::PropertyManager& r)
{
    return static_cast<const ::Ice::Object&>(l) < static_cast<const ::Ice::Object&>(r);
}

bool
TeRK::operator<=(const ::TeRK::PropertyManager& l, const ::TeRK::PropertyManager& r)
{
    return l < r || l == r;
}

bool
TeRK::operator>(const ::TeRK::PropertyManager& l, const ::TeRK::PropertyManager& r)
{
    return !(l < r) && !(l == r);
}

bool
TeRK::operator>=(const ::TeRK::PropertyManager& l, const ::TeRK::PropertyManager& r)
{
    return !(l < r);
}

::Ice::ObjectPtr
TeRK::AbstractCommandController::ice_clone() const
{
    throw ::Ice::CloneNotImplementedException(__FILE__, __LINE__);
    return 0; // to avoid a warning with some compilers
}

static const ::std::string __TeRK__AbstractCommandController_ids[3] =
{
    "::Ice::Object",
    "::TeRK::AbstractCommandController",
    "::TeRK::PropertyManager"
};

bool
TeRK::AbstractCommandController::ice_isA(const ::std::string& _s, const ::Ice::Current&) const
{
    return ::std::binary_search(__TeRK__AbstractCommandController_ids, __TeRK__AbstractCommandController_ids + 3, _s);
}

::std::vector< ::std::string>
TeRK::AbstractCommandController::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&__TeRK__AbstractCommandController_ids[0], &__TeRK__AbstractCommandController_ids[3]);
}

const ::std::string&
TeRK::AbstractCommandController::ice_id(const ::Ice::Current&) const
{
    return __TeRK__AbstractCommandController_ids[1];
}

const ::std::string&
TeRK::AbstractCommandController::ice_staticId()
{
    return __TeRK__AbstractCommandController_ids[1];
}

static ::std::string __TeRK__AbstractCommandController_all[] =
{
    "getProperties",
    "getProperty",
    "getPropertyKeys",
    "ice_id",
    "ice_ids",
    "ice_isA",
    "ice_ping",
    "setProperty"
};

::IceInternal::DispatchStatus
TeRK::AbstractCommandController::__dispatch(::IceInternal::Incoming& in, const ::Ice::Current& current)
{
    ::std::pair< ::std::string*, ::std::string*> r = ::std::equal_range(__TeRK__AbstractCommandController_all, __TeRK__AbstractCommandController_all + 8, current.operation);
    if(r.first == r.second)
    {
	return ::IceInternal::DispatchOperationNotExist;
    }

    switch(r.first - __TeRK__AbstractCommandController_all)
    {
	case 0:
	{
	    return ___getProperties(in, current);
	}
	case 1:
	{
	    return ___getProperty(in, current);
	}
	case 2:
	{
	    return ___getPropertyKeys(in, current);
	}
	case 3:
	{
	    return ___ice_id(in, current);
	}
	case 4:
	{
	    return ___ice_ids(in, current);
	}
	case 5:
	{
	    return ___ice_isA(in, current);
	}
	case 6:
	{
	    return ___ice_ping(in, current);
	}
	case 7:
	{
	    return ___setProperty(in, current);
	}
    }

    assert(false);
    return ::IceInternal::DispatchOperationNotExist;
}

void
TeRK::AbstractCommandController::__write(::IceInternal::BasicStream* __os) const
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
TeRK::AbstractCommandController::__read(::IceInternal::BasicStream* __is, bool __rid)
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
TeRK::AbstractCommandController::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type TeRK::AbstractCommandController was not generated with stream support";
    throw ex;
}

void
TeRK::AbstractCommandController::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type TeRK::AbstractCommandController was not generated with stream support";
    throw ex;
}

void 
TeRK::__patch__AbstractCommandControllerPtr(void* __addr, ::Ice::ObjectPtr& v)
{
    ::TeRK::AbstractCommandControllerPtr* p = static_cast< ::TeRK::AbstractCommandControllerPtr*>(__addr);
    assert(p);
    *p = ::TeRK::AbstractCommandControllerPtr::dynamicCast(v);
    if(v && !*p)
    {
	::Ice::NoObjectFactoryException e(__FILE__, __LINE__);
	e.type = ::TeRK::AbstractCommandController::ice_staticId();
	throw e;
    }
}

bool
TeRK::operator==(const ::TeRK::AbstractCommandController& l, const ::TeRK::AbstractCommandController& r)
{
    return static_cast<const ::Ice::Object&>(l) == static_cast<const ::Ice::Object&>(r);
}

bool
TeRK::operator!=(const ::TeRK::AbstractCommandController& l, const ::TeRK::AbstractCommandController& r)
{
    return static_cast<const ::Ice::Object&>(l) != static_cast<const ::Ice::Object&>(r);
}

bool
TeRK::operator<(const ::TeRK::AbstractCommandController& l, const ::TeRK::AbstractCommandController& r)
{
    return static_cast<const ::Ice::Object&>(l) < static_cast<const ::Ice::Object&>(r);
}

bool
TeRK::operator<=(const ::TeRK::AbstractCommandController& l, const ::TeRK::AbstractCommandController& r)
{
    return l < r || l == r;
}

bool
TeRK::operator>(const ::TeRK::AbstractCommandController& l, const ::TeRK::AbstractCommandController& r)
{
    return !(l < r) && !(l == r);
}

bool
TeRK::operator>=(const ::TeRK::AbstractCommandController& l, const ::TeRK::AbstractCommandController& r)
{
    return !(l < r);
}

::Ice::ObjectPtr
TeRK::AnalogInController::ice_clone() const
{
    throw ::Ice::CloneNotImplementedException(__FILE__, __LINE__);
    return 0; // to avoid a warning with some compilers
}

static const ::std::string __TeRK__AnalogInController_ids[4] =
{
    "::Ice::Object",
    "::TeRK::AbstractCommandController",
    "::TeRK::AnalogInController",
    "::TeRK::PropertyManager"
};

bool
TeRK::AnalogInController::ice_isA(const ::std::string& _s, const ::Ice::Current&) const
{
    return ::std::binary_search(__TeRK__AnalogInController_ids, __TeRK__AnalogInController_ids + 4, _s);
}

::std::vector< ::std::string>
TeRK::AnalogInController::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&__TeRK__AnalogInController_ids[0], &__TeRK__AnalogInController_ids[4]);
}

const ::std::string&
TeRK::AnalogInController::ice_id(const ::Ice::Current&) const
{
    return __TeRK__AnalogInController_ids[2];
}

const ::std::string&
TeRK::AnalogInController::ice_staticId()
{
    return __TeRK__AnalogInController_ids[2];
}

::IceInternal::DispatchStatus
TeRK::AnalogInController::___getState(::IceInternal::Incoming&__inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __os = __inS.os();
    ::TeRK::AnalogInState __ret = getState(__current);
    __ret.__write(__os);
    return ::IceInternal::DispatchOK;
}

static ::std::string __TeRK__AnalogInController_all[] =
{
    "getProperties",
    "getProperty",
    "getPropertyKeys",
    "getState",
    "ice_id",
    "ice_ids",
    "ice_isA",
    "ice_ping",
    "setProperty"
};

::IceInternal::DispatchStatus
TeRK::AnalogInController::__dispatch(::IceInternal::Incoming& in, const ::Ice::Current& current)
{
    ::std::pair< ::std::string*, ::std::string*> r = ::std::equal_range(__TeRK__AnalogInController_all, __TeRK__AnalogInController_all + 9, current.operation);
    if(r.first == r.second)
    {
	return ::IceInternal::DispatchOperationNotExist;
    }

    switch(r.first - __TeRK__AnalogInController_all)
    {
	case 0:
	{
	    return ___getProperties(in, current);
	}
	case 1:
	{
	    return ___getProperty(in, current);
	}
	case 2:
	{
	    return ___getPropertyKeys(in, current);
	}
	case 3:
	{
	    return ___getState(in, current);
	}
	case 4:
	{
	    return ___ice_id(in, current);
	}
	case 5:
	{
	    return ___ice_ids(in, current);
	}
	case 6:
	{
	    return ___ice_isA(in, current);
	}
	case 7:
	{
	    return ___ice_ping(in, current);
	}
	case 8:
	{
	    return ___setProperty(in, current);
	}
    }

    assert(false);
    return ::IceInternal::DispatchOperationNotExist;
}

void
TeRK::AnalogInController::__write(::IceInternal::BasicStream* __os) const
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
TeRK::AnalogInController::__read(::IceInternal::BasicStream* __is, bool __rid)
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
TeRK::AnalogInController::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type TeRK::AnalogInController was not generated with stream support";
    throw ex;
}

void
TeRK::AnalogInController::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type TeRK::AnalogInController was not generated with stream support";
    throw ex;
}

void 
TeRK::__patch__AnalogInControllerPtr(void* __addr, ::Ice::ObjectPtr& v)
{
    ::TeRK::AnalogInControllerPtr* p = static_cast< ::TeRK::AnalogInControllerPtr*>(__addr);
    assert(p);
    *p = ::TeRK::AnalogInControllerPtr::dynamicCast(v);
    if(v && !*p)
    {
	::Ice::NoObjectFactoryException e(__FILE__, __LINE__);
	e.type = ::TeRK::AnalogInController::ice_staticId();
	throw e;
    }
}

bool
TeRK::operator==(const ::TeRK::AnalogInController& l, const ::TeRK::AnalogInController& r)
{
    return static_cast<const ::Ice::Object&>(l) == static_cast<const ::Ice::Object&>(r);
}

bool
TeRK::operator!=(const ::TeRK::AnalogInController& l, const ::TeRK::AnalogInController& r)
{
    return static_cast<const ::Ice::Object&>(l) != static_cast<const ::Ice::Object&>(r);
}

bool
TeRK::operator<(const ::TeRK::AnalogInController& l, const ::TeRK::AnalogInController& r)
{
    return static_cast<const ::Ice::Object&>(l) < static_cast<const ::Ice::Object&>(r);
}

bool
TeRK::operator<=(const ::TeRK::AnalogInController& l, const ::TeRK::AnalogInController& r)
{
    return l < r || l == r;
}

bool
TeRK::operator>(const ::TeRK::AnalogInController& l, const ::TeRK::AnalogInController& r)
{
    return !(l < r) && !(l == r);
}

bool
TeRK::operator>=(const ::TeRK::AnalogInController& l, const ::TeRK::AnalogInController& r)
{
    return !(l < r);
}

::Ice::ObjectPtr
TeRK::AudioController::ice_clone() const
{
    throw ::Ice::CloneNotImplementedException(__FILE__, __LINE__);
    return 0; // to avoid a warning with some compilers
}

static const ::std::string __TeRK__AudioController_ids[4] =
{
    "::Ice::Object",
    "::TeRK::AbstractCommandController",
    "::TeRK::AudioController",
    "::TeRK::PropertyManager"
};

bool
TeRK::AudioController::ice_isA(const ::std::string& _s, const ::Ice::Current&) const
{
    return ::std::binary_search(__TeRK__AudioController_ids, __TeRK__AudioController_ids + 4, _s);
}

::std::vector< ::std::string>
TeRK::AudioController::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&__TeRK__AudioController_ids[0], &__TeRK__AudioController_ids[4]);
}

const ::std::string&
TeRK::AudioController::ice_id(const ::Ice::Current&) const
{
    return __TeRK__AudioController_ids[2];
}

const ::std::string&
TeRK::AudioController::ice_staticId()
{
    return __TeRK__AudioController_ids[2];
}

::IceInternal::DispatchStatus
TeRK::AudioController::___execute(::IceInternal::Incoming&__inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::TeRK::AudioCommand command;
    command.__read(__is);
    try
    {
	execute(command, __current);
    }
    catch(const ::TeRK::AudioCommandException& __ex)
    {
	__os->write(__ex);
	return ::IceInternal::DispatchUserException;
    }
    return ::IceInternal::DispatchOK;
}

static ::std::string __TeRK__AudioController_all[] =
{
    "execute",
    "getProperties",
    "getProperty",
    "getPropertyKeys",
    "ice_id",
    "ice_ids",
    "ice_isA",
    "ice_ping",
    "setProperty"
};

::IceInternal::DispatchStatus
TeRK::AudioController::__dispatch(::IceInternal::Incoming& in, const ::Ice::Current& current)
{
    ::std::pair< ::std::string*, ::std::string*> r = ::std::equal_range(__TeRK__AudioController_all, __TeRK__AudioController_all + 9, current.operation);
    if(r.first == r.second)
    {
	return ::IceInternal::DispatchOperationNotExist;
    }

    switch(r.first - __TeRK__AudioController_all)
    {
	case 0:
	{
	    return ___execute(in, current);
	}
	case 1:
	{
	    return ___getProperties(in, current);
	}
	case 2:
	{
	    return ___getProperty(in, current);
	}
	case 3:
	{
	    return ___getPropertyKeys(in, current);
	}
	case 4:
	{
	    return ___ice_id(in, current);
	}
	case 5:
	{
	    return ___ice_ids(in, current);
	}
	case 6:
	{
	    return ___ice_isA(in, current);
	}
	case 7:
	{
	    return ___ice_ping(in, current);
	}
	case 8:
	{
	    return ___setProperty(in, current);
	}
    }

    assert(false);
    return ::IceInternal::DispatchOperationNotExist;
}

void
TeRK::AudioController::__write(::IceInternal::BasicStream* __os) const
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
TeRK::AudioController::__read(::IceInternal::BasicStream* __is, bool __rid)
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
TeRK::AudioController::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type TeRK::AudioController was not generated with stream support";
    throw ex;
}

void
TeRK::AudioController::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type TeRK::AudioController was not generated with stream support";
    throw ex;
}

void 
TeRK::__patch__AudioControllerPtr(void* __addr, ::Ice::ObjectPtr& v)
{
    ::TeRK::AudioControllerPtr* p = static_cast< ::TeRK::AudioControllerPtr*>(__addr);
    assert(p);
    *p = ::TeRK::AudioControllerPtr::dynamicCast(v);
    if(v && !*p)
    {
	::Ice::NoObjectFactoryException e(__FILE__, __LINE__);
	e.type = ::TeRK::AudioController::ice_staticId();
	throw e;
    }
}

bool
TeRK::operator==(const ::TeRK::AudioController& l, const ::TeRK::AudioController& r)
{
    return static_cast<const ::Ice::Object&>(l) == static_cast<const ::Ice::Object&>(r);
}

bool
TeRK::operator!=(const ::TeRK::AudioController& l, const ::TeRK::AudioController& r)
{
    return static_cast<const ::Ice::Object&>(l) != static_cast<const ::Ice::Object&>(r);
}

bool
TeRK::operator<(const ::TeRK::AudioController& l, const ::TeRK::AudioController& r)
{
    return static_cast<const ::Ice::Object&>(l) < static_cast<const ::Ice::Object&>(r);
}

bool
TeRK::operator<=(const ::TeRK::AudioController& l, const ::TeRK::AudioController& r)
{
    return l < r || l == r;
}

bool
TeRK::operator>(const ::TeRK::AudioController& l, const ::TeRK::AudioController& r)
{
    return !(l < r) && !(l == r);
}

bool
TeRK::operator>=(const ::TeRK::AudioController& l, const ::TeRK::AudioController& r)
{
    return !(l < r);
}

::Ice::ObjectPtr
TeRK::DigitalInController::ice_clone() const
{
    throw ::Ice::CloneNotImplementedException(__FILE__, __LINE__);
    return 0; // to avoid a warning with some compilers
}

static const ::std::string __TeRK__DigitalInController_ids[4] =
{
    "::Ice::Object",
    "::TeRK::AbstractCommandController",
    "::TeRK::DigitalInController",
    "::TeRK::PropertyManager"
};

bool
TeRK::DigitalInController::ice_isA(const ::std::string& _s, const ::Ice::Current&) const
{
    return ::std::binary_search(__TeRK__DigitalInController_ids, __TeRK__DigitalInController_ids + 4, _s);
}

::std::vector< ::std::string>
TeRK::DigitalInController::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&__TeRK__DigitalInController_ids[0], &__TeRK__DigitalInController_ids[4]);
}

const ::std::string&
TeRK::DigitalInController::ice_id(const ::Ice::Current&) const
{
    return __TeRK__DigitalInController_ids[2];
}

const ::std::string&
TeRK::DigitalInController::ice_staticId()
{
    return __TeRK__DigitalInController_ids[2];
}

::IceInternal::DispatchStatus
TeRK::DigitalInController::___getState(::IceInternal::Incoming&__inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __os = __inS.os();
    ::TeRK::DigitalInState __ret = getState(__current);
    __ret.__write(__os);
    return ::IceInternal::DispatchOK;
}

static ::std::string __TeRK__DigitalInController_all[] =
{
    "getProperties",
    "getProperty",
    "getPropertyKeys",
    "getState",
    "ice_id",
    "ice_ids",
    "ice_isA",
    "ice_ping",
    "setProperty"
};

::IceInternal::DispatchStatus
TeRK::DigitalInController::__dispatch(::IceInternal::Incoming& in, const ::Ice::Current& current)
{
    ::std::pair< ::std::string*, ::std::string*> r = ::std::equal_range(__TeRK__DigitalInController_all, __TeRK__DigitalInController_all + 9, current.operation);
    if(r.first == r.second)
    {
	return ::IceInternal::DispatchOperationNotExist;
    }

    switch(r.first - __TeRK__DigitalInController_all)
    {
	case 0:
	{
	    return ___getProperties(in, current);
	}
	case 1:
	{
	    return ___getProperty(in, current);
	}
	case 2:
	{
	    return ___getPropertyKeys(in, current);
	}
	case 3:
	{
	    return ___getState(in, current);
	}
	case 4:
	{
	    return ___ice_id(in, current);
	}
	case 5:
	{
	    return ___ice_ids(in, current);
	}
	case 6:
	{
	    return ___ice_isA(in, current);
	}
	case 7:
	{
	    return ___ice_ping(in, current);
	}
	case 8:
	{
	    return ___setProperty(in, current);
	}
    }

    assert(false);
    return ::IceInternal::DispatchOperationNotExist;
}

void
TeRK::DigitalInController::__write(::IceInternal::BasicStream* __os) const
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
TeRK::DigitalInController::__read(::IceInternal::BasicStream* __is, bool __rid)
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
TeRK::DigitalInController::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type TeRK::DigitalInController was not generated with stream support";
    throw ex;
}

void
TeRK::DigitalInController::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type TeRK::DigitalInController was not generated with stream support";
    throw ex;
}

void 
TeRK::__patch__DigitalInControllerPtr(void* __addr, ::Ice::ObjectPtr& v)
{
    ::TeRK::DigitalInControllerPtr* p = static_cast< ::TeRK::DigitalInControllerPtr*>(__addr);
    assert(p);
    *p = ::TeRK::DigitalInControllerPtr::dynamicCast(v);
    if(v && !*p)
    {
	::Ice::NoObjectFactoryException e(__FILE__, __LINE__);
	e.type = ::TeRK::DigitalInController::ice_staticId();
	throw e;
    }
}

bool
TeRK::operator==(const ::TeRK::DigitalInController& l, const ::TeRK::DigitalInController& r)
{
    return static_cast<const ::Ice::Object&>(l) == static_cast<const ::Ice::Object&>(r);
}

bool
TeRK::operator!=(const ::TeRK::DigitalInController& l, const ::TeRK::DigitalInController& r)
{
    return static_cast<const ::Ice::Object&>(l) != static_cast<const ::Ice::Object&>(r);
}

bool
TeRK::operator<(const ::TeRK::DigitalInController& l, const ::TeRK::DigitalInController& r)
{
    return static_cast<const ::Ice::Object&>(l) < static_cast<const ::Ice::Object&>(r);
}

bool
TeRK::operator<=(const ::TeRK::DigitalInController& l, const ::TeRK::DigitalInController& r)
{
    return l < r || l == r;
}

bool
TeRK::operator>(const ::TeRK::DigitalInController& l, const ::TeRK::DigitalInController& r)
{
    return !(l < r) && !(l == r);
}

bool
TeRK::operator>=(const ::TeRK::DigitalInController& l, const ::TeRK::DigitalInController& r)
{
    return !(l < r);
}

::Ice::ObjectPtr
TeRK::DigitalOutController::ice_clone() const
{
    throw ::Ice::CloneNotImplementedException(__FILE__, __LINE__);
    return 0; // to avoid a warning with some compilers
}

static const ::std::string __TeRK__DigitalOutController_ids[4] =
{
    "::Ice::Object",
    "::TeRK::AbstractCommandController",
    "::TeRK::DigitalOutController",
    "::TeRK::PropertyManager"
};

bool
TeRK::DigitalOutController::ice_isA(const ::std::string& _s, const ::Ice::Current&) const
{
    return ::std::binary_search(__TeRK__DigitalOutController_ids, __TeRK__DigitalOutController_ids + 4, _s);
}

::std::vector< ::std::string>
TeRK::DigitalOutController::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&__TeRK__DigitalOutController_ids[0], &__TeRK__DigitalOutController_ids[4]);
}

const ::std::string&
TeRK::DigitalOutController::ice_id(const ::Ice::Current&) const
{
    return __TeRK__DigitalOutController_ids[2];
}

const ::std::string&
TeRK::DigitalOutController::ice_staticId()
{
    return __TeRK__DigitalOutController_ids[2];
}

::IceInternal::DispatchStatus
TeRK::DigitalOutController::___execute(::IceInternal::Incoming&__inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::TeRK::DigitalOutCommand command;
    command.__read(__is);
    try
    {
	execute(command, __current);
    }
    catch(const ::TeRK::DigitalOutCommandException& __ex)
    {
	__os->write(__ex);
	return ::IceInternal::DispatchUserException;
    }
    return ::IceInternal::DispatchOK;
}

static ::std::string __TeRK__DigitalOutController_all[] =
{
    "execute",
    "getProperties",
    "getProperty",
    "getPropertyKeys",
    "ice_id",
    "ice_ids",
    "ice_isA",
    "ice_ping",
    "setProperty"
};

::IceInternal::DispatchStatus
TeRK::DigitalOutController::__dispatch(::IceInternal::Incoming& in, const ::Ice::Current& current)
{
    ::std::pair< ::std::string*, ::std::string*> r = ::std::equal_range(__TeRK__DigitalOutController_all, __TeRK__DigitalOutController_all + 9, current.operation);
    if(r.first == r.second)
    {
	return ::IceInternal::DispatchOperationNotExist;
    }

    switch(r.first - __TeRK__DigitalOutController_all)
    {
	case 0:
	{
	    return ___execute(in, current);
	}
	case 1:
	{
	    return ___getProperties(in, current);
	}
	case 2:
	{
	    return ___getProperty(in, current);
	}
	case 3:
	{
	    return ___getPropertyKeys(in, current);
	}
	case 4:
	{
	    return ___ice_id(in, current);
	}
	case 5:
	{
	    return ___ice_ids(in, current);
	}
	case 6:
	{
	    return ___ice_isA(in, current);
	}
	case 7:
	{
	    return ___ice_ping(in, current);
	}
	case 8:
	{
	    return ___setProperty(in, current);
	}
    }

    assert(false);
    return ::IceInternal::DispatchOperationNotExist;
}

void
TeRK::DigitalOutController::__write(::IceInternal::BasicStream* __os) const
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
TeRK::DigitalOutController::__read(::IceInternal::BasicStream* __is, bool __rid)
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
TeRK::DigitalOutController::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type TeRK::DigitalOutController was not generated with stream support";
    throw ex;
}

void
TeRK::DigitalOutController::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type TeRK::DigitalOutController was not generated with stream support";
    throw ex;
}

void 
TeRK::__patch__DigitalOutControllerPtr(void* __addr, ::Ice::ObjectPtr& v)
{
    ::TeRK::DigitalOutControllerPtr* p = static_cast< ::TeRK::DigitalOutControllerPtr*>(__addr);
    assert(p);
    *p = ::TeRK::DigitalOutControllerPtr::dynamicCast(v);
    if(v && !*p)
    {
	::Ice::NoObjectFactoryException e(__FILE__, __LINE__);
	e.type = ::TeRK::DigitalOutController::ice_staticId();
	throw e;
    }
}

bool
TeRK::operator==(const ::TeRK::DigitalOutController& l, const ::TeRK::DigitalOutController& r)
{
    return static_cast<const ::Ice::Object&>(l) == static_cast<const ::Ice::Object&>(r);
}

bool
TeRK::operator!=(const ::TeRK::DigitalOutController& l, const ::TeRK::DigitalOutController& r)
{
    return static_cast<const ::Ice::Object&>(l) != static_cast<const ::Ice::Object&>(r);
}

bool
TeRK::operator<(const ::TeRK::DigitalOutController& l, const ::TeRK::DigitalOutController& r)
{
    return static_cast<const ::Ice::Object&>(l) < static_cast<const ::Ice::Object&>(r);
}

bool
TeRK::operator<=(const ::TeRK::DigitalOutController& l, const ::TeRK::DigitalOutController& r)
{
    return l < r || l == r;
}

bool
TeRK::operator>(const ::TeRK::DigitalOutController& l, const ::TeRK::DigitalOutController& r)
{
    return !(l < r) && !(l == r);
}

bool
TeRK::operator>=(const ::TeRK::DigitalOutController& l, const ::TeRK::DigitalOutController& r)
{
    return !(l < r);
}

::Ice::ObjectPtr
TeRK::LEDController::ice_clone() const
{
    throw ::Ice::CloneNotImplementedException(__FILE__, __LINE__);
    return 0; // to avoid a warning with some compilers
}

static const ::std::string __TeRK__LEDController_ids[4] =
{
    "::Ice::Object",
    "::TeRK::AbstractCommandController",
    "::TeRK::LEDController",
    "::TeRK::PropertyManager"
};

bool
TeRK::LEDController::ice_isA(const ::std::string& _s, const ::Ice::Current&) const
{
    return ::std::binary_search(__TeRK__LEDController_ids, __TeRK__LEDController_ids + 4, _s);
}

::std::vector< ::std::string>
TeRK::LEDController::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&__TeRK__LEDController_ids[0], &__TeRK__LEDController_ids[4]);
}

const ::std::string&
TeRK::LEDController::ice_id(const ::Ice::Current&) const
{
    return __TeRK__LEDController_ids[2];
}

const ::std::string&
TeRK::LEDController::ice_staticId()
{
    return __TeRK__LEDController_ids[2];
}

::IceInternal::DispatchStatus
TeRK::LEDController::___execute(::IceInternal::Incoming&__inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::TeRK::LEDCommand command;
    command.__read(__is);
    try
    {
	execute(command, __current);
    }
    catch(const ::TeRK::LEDCommandException& __ex)
    {
	__os->write(__ex);
	return ::IceInternal::DispatchUserException;
    }
    return ::IceInternal::DispatchOK;
}

static ::std::string __TeRK__LEDController_all[] =
{
    "execute",
    "getProperties",
    "getProperty",
    "getPropertyKeys",
    "ice_id",
    "ice_ids",
    "ice_isA",
    "ice_ping",
    "setProperty"
};

::IceInternal::DispatchStatus
TeRK::LEDController::__dispatch(::IceInternal::Incoming& in, const ::Ice::Current& current)
{
    ::std::pair< ::std::string*, ::std::string*> r = ::std::equal_range(__TeRK__LEDController_all, __TeRK__LEDController_all + 9, current.operation);
    if(r.first == r.second)
    {
	return ::IceInternal::DispatchOperationNotExist;
    }

    switch(r.first - __TeRK__LEDController_all)
    {
	case 0:
	{
	    return ___execute(in, current);
	}
	case 1:
	{
	    return ___getProperties(in, current);
	}
	case 2:
	{
	    return ___getProperty(in, current);
	}
	case 3:
	{
	    return ___getPropertyKeys(in, current);
	}
	case 4:
	{
	    return ___ice_id(in, current);
	}
	case 5:
	{
	    return ___ice_ids(in, current);
	}
	case 6:
	{
	    return ___ice_isA(in, current);
	}
	case 7:
	{
	    return ___ice_ping(in, current);
	}
	case 8:
	{
	    return ___setProperty(in, current);
	}
    }

    assert(false);
    return ::IceInternal::DispatchOperationNotExist;
}

void
TeRK::LEDController::__write(::IceInternal::BasicStream* __os) const
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
TeRK::LEDController::__read(::IceInternal::BasicStream* __is, bool __rid)
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
TeRK::LEDController::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type TeRK::LEDController was not generated with stream support";
    throw ex;
}

void
TeRK::LEDController::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type TeRK::LEDController was not generated with stream support";
    throw ex;
}

void 
TeRK::__patch__LEDControllerPtr(void* __addr, ::Ice::ObjectPtr& v)
{
    ::TeRK::LEDControllerPtr* p = static_cast< ::TeRK::LEDControllerPtr*>(__addr);
    assert(p);
    *p = ::TeRK::LEDControllerPtr::dynamicCast(v);
    if(v && !*p)
    {
	::Ice::NoObjectFactoryException e(__FILE__, __LINE__);
	e.type = ::TeRK::LEDController::ice_staticId();
	throw e;
    }
}

bool
TeRK::operator==(const ::TeRK::LEDController& l, const ::TeRK::LEDController& r)
{
    return static_cast<const ::Ice::Object&>(l) == static_cast<const ::Ice::Object&>(r);
}

bool
TeRK::operator!=(const ::TeRK::LEDController& l, const ::TeRK::LEDController& r)
{
    return static_cast<const ::Ice::Object&>(l) != static_cast<const ::Ice::Object&>(r);
}

bool
TeRK::operator<(const ::TeRK::LEDController& l, const ::TeRK::LEDController& r)
{
    return static_cast<const ::Ice::Object&>(l) < static_cast<const ::Ice::Object&>(r);
}

bool
TeRK::operator<=(const ::TeRK::LEDController& l, const ::TeRK::LEDController& r)
{
    return l < r || l == r;
}

bool
TeRK::operator>(const ::TeRK::LEDController& l, const ::TeRK::LEDController& r)
{
    return !(l < r) && !(l == r);
}

bool
TeRK::operator>=(const ::TeRK::LEDController& l, const ::TeRK::LEDController& r)
{
    return !(l < r);
}

::Ice::ObjectPtr
TeRK::MotorController::ice_clone() const
{
    throw ::Ice::CloneNotImplementedException(__FILE__, __LINE__);
    return 0; // to avoid a warning with some compilers
}

static const ::std::string __TeRK__MotorController_ids[4] =
{
    "::Ice::Object",
    "::TeRK::AbstractCommandController",
    "::TeRK::MotorController",
    "::TeRK::PropertyManager"
};

bool
TeRK::MotorController::ice_isA(const ::std::string& _s, const ::Ice::Current&) const
{
    return ::std::binary_search(__TeRK__MotorController_ids, __TeRK__MotorController_ids + 4, _s);
}

::std::vector< ::std::string>
TeRK::MotorController::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&__TeRK__MotorController_ids[0], &__TeRK__MotorController_ids[4]);
}

const ::std::string&
TeRK::MotorController::ice_id(const ::Ice::Current&) const
{
    return __TeRK__MotorController_ids[2];
}

const ::std::string&
TeRK::MotorController::ice_staticId()
{
    return __TeRK__MotorController_ids[2];
}

::IceInternal::DispatchStatus
TeRK::MotorController::___execute(::IceInternal::Incoming&__inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::TeRK::MotorCommand command;
    command.__read(__is);
    try
    {
	::TeRK::MotorState __ret = execute(command, __current);
	__ret.__write(__os);
    }
    catch(const ::TeRK::MotorCommandException& __ex)
    {
	__os->write(__ex);
	return ::IceInternal::DispatchUserException;
    }
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
TeRK::MotorController::___getFrequency(::IceInternal::Incoming&__inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __os = __inS.os();
    ::Ice::Int __ret = getFrequency(__current);
    __os->write(__ret);
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
TeRK::MotorController::___startMotorBufferRecord(::IceInternal::Incoming&__inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::TeRK::BooleanArray motorMask;
    __is->read(motorMask);
    try
    {
	startMotorBufferRecord(motorMask, __current);
    }
    catch(const ::TeRK::MotorException& __ex)
    {
	__os->write(__ex);
	return ::IceInternal::DispatchUserException;
    }
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
TeRK::MotorController::___stopMotorBufferRecord(::IceInternal::Incoming&__inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Idempotent, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::TeRK::BooleanArray motorMask;
    __is->read(motorMask);
    try
    {
	stopMotorBufferRecord(motorMask, __current);
    }
    catch(const ::TeRK::MotorException& __ex)
    {
	__os->write(__ex);
	return ::IceInternal::DispatchUserException;
    }
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
TeRK::MotorController::___getMotorBuffers(::IceInternal::Incoming&__inS, const ::Ice::Current& __current) const
{
    __checkMode(::Ice::Nonmutating, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::TeRK::BooleanArray motorMask;
    __is->read(motorMask);
    try
    {
	::TeRK::MotorBufferArray __ret = getMotorBuffers(motorMask, __current);
	if(__ret.size() == 0)
	{
	    __os->writeSize(0);
	}
	else
	{
	    ::TeRK::__write(__os, &__ret[0], &__ret[0] + __ret.size(), ::TeRK::__U__MotorBufferArray());
	}
    }
    catch(const ::TeRK::MotorException& __ex)
    {
	__os->write(__ex);
	return ::IceInternal::DispatchUserException;
    }
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
TeRK::MotorController::___setMotorBuffer(::IceInternal::Incoming&__inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Idempotent, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::TeRK::BooleanArray motorMask;
    ::TeRK::MotorBufferArray motorBuffers;
    __is->read(motorMask);
    ::TeRK::__read(__is, motorBuffers, ::TeRK::__U__MotorBufferArray());
    try
    {
	setMotorBuffer(motorMask, motorBuffers, __current);
    }
    catch(const ::TeRK::MotorException& __ex)
    {
	__os->write(__ex);
	return ::IceInternal::DispatchUserException;
    }
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
TeRK::MotorController::___playMotorBuffer(::IceInternal::Incoming&__inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::TeRK::BooleanArray motorMask;
    __is->read(motorMask);
    try
    {
	playMotorBuffer(motorMask, __current);
    }
    catch(const ::TeRK::MotorException& __ex)
    {
	__os->write(__ex);
	return ::IceInternal::DispatchUserException;
    }
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
TeRK::MotorController::___driveForward(::IceInternal::Incoming&, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    driveForward(__current);
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
TeRK::MotorController::___driveBack(::IceInternal::Incoming&, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    driveBack(__current);
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
TeRK::MotorController::___spinLeft(::IceInternal::Incoming&, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    spinLeft(__current);
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
TeRK::MotorController::___spinRight(::IceInternal::Incoming&, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    spinRight(__current);
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
TeRK::MotorController::___setMotorVelocities(::IceInternal::Incoming&__inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Idempotent, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::TeRK::IntArray motorValues;
    __is->read(motorValues);
    try
    {
	setMotorVelocities(motorValues, __current);
    }
    catch(const ::TeRK::GenericError& __ex)
    {
	__os->write(__ex);
	return ::IceInternal::DispatchUserException;
    }
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
TeRK::MotorController::___stopMotors(::IceInternal::Incoming&, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    stopMotors(__current);
    return ::IceInternal::DispatchOK;
}

static ::std::string __TeRK__MotorController_all[] =
{
    "driveBack",
    "driveForward",
    "execute",
    "getFrequency",
    "getMotorBuffers",
    "getProperties",
    "getProperty",
    "getPropertyKeys",
    "ice_id",
    "ice_ids",
    "ice_isA",
    "ice_ping",
    "playMotorBuffer",
    "setMotorBuffer",
    "setMotorVelocities",
    "setProperty",
    "spinLeft",
    "spinRight",
    "startMotorBufferRecord",
    "stopMotorBufferRecord",
    "stopMotors"
};

::IceInternal::DispatchStatus
TeRK::MotorController::__dispatch(::IceInternal::Incoming& in, const ::Ice::Current& current)
{
    ::std::pair< ::std::string*, ::std::string*> r = ::std::equal_range(__TeRK__MotorController_all, __TeRK__MotorController_all + 21, current.operation);
    if(r.first == r.second)
    {
	return ::IceInternal::DispatchOperationNotExist;
    }

    switch(r.first - __TeRK__MotorController_all)
    {
	case 0:
	{
	    return ___driveBack(in, current);
	}
	case 1:
	{
	    return ___driveForward(in, current);
	}
	case 2:
	{
	    return ___execute(in, current);
	}
	case 3:
	{
	    return ___getFrequency(in, current);
	}
	case 4:
	{
	    return ___getMotorBuffers(in, current);
	}
	case 5:
	{
	    return ___getProperties(in, current);
	}
	case 6:
	{
	    return ___getProperty(in, current);
	}
	case 7:
	{
	    return ___getPropertyKeys(in, current);
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
	    return ___playMotorBuffer(in, current);
	}
	case 13:
	{
	    return ___setMotorBuffer(in, current);
	}
	case 14:
	{
	    return ___setMotorVelocities(in, current);
	}
	case 15:
	{
	    return ___setProperty(in, current);
	}
	case 16:
	{
	    return ___spinLeft(in, current);
	}
	case 17:
	{
	    return ___spinRight(in, current);
	}
	case 18:
	{
	    return ___startMotorBufferRecord(in, current);
	}
	case 19:
	{
	    return ___stopMotorBufferRecord(in, current);
	}
	case 20:
	{
	    return ___stopMotors(in, current);
	}
    }

    assert(false);
    return ::IceInternal::DispatchOperationNotExist;
}

void
TeRK::MotorController::__write(::IceInternal::BasicStream* __os) const
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
TeRK::MotorController::__read(::IceInternal::BasicStream* __is, bool __rid)
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
TeRK::MotorController::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type TeRK::MotorController was not generated with stream support";
    throw ex;
}

void
TeRK::MotorController::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type TeRK::MotorController was not generated with stream support";
    throw ex;
}

void 
TeRK::__patch__MotorControllerPtr(void* __addr, ::Ice::ObjectPtr& v)
{
    ::TeRK::MotorControllerPtr* p = static_cast< ::TeRK::MotorControllerPtr*>(__addr);
    assert(p);
    *p = ::TeRK::MotorControllerPtr::dynamicCast(v);
    if(v && !*p)
    {
	::Ice::NoObjectFactoryException e(__FILE__, __LINE__);
	e.type = ::TeRK::MotorController::ice_staticId();
	throw e;
    }
}

bool
TeRK::operator==(const ::TeRK::MotorController& l, const ::TeRK::MotorController& r)
{
    return static_cast<const ::Ice::Object&>(l) == static_cast<const ::Ice::Object&>(r);
}

bool
TeRK::operator!=(const ::TeRK::MotorController& l, const ::TeRK::MotorController& r)
{
    return static_cast<const ::Ice::Object&>(l) != static_cast<const ::Ice::Object&>(r);
}

bool
TeRK::operator<(const ::TeRK::MotorController& l, const ::TeRK::MotorController& r)
{
    return static_cast<const ::Ice::Object&>(l) < static_cast<const ::Ice::Object&>(r);
}

bool
TeRK::operator<=(const ::TeRK::MotorController& l, const ::TeRK::MotorController& r)
{
    return l < r || l == r;
}

bool
TeRK::operator>(const ::TeRK::MotorController& l, const ::TeRK::MotorController& r)
{
    return !(l < r) && !(l == r);
}

bool
TeRK::operator>=(const ::TeRK::MotorController& l, const ::TeRK::MotorController& r)
{
    return !(l < r);
}

::Ice::ObjectPtr
TeRK::ServoController::ice_clone() const
{
    throw ::Ice::CloneNotImplementedException(__FILE__, __LINE__);
    return 0; // to avoid a warning with some compilers
}

static const ::std::string __TeRK__ServoController_ids[4] =
{
    "::Ice::Object",
    "::TeRK::AbstractCommandController",
    "::TeRK::PropertyManager",
    "::TeRK::ServoController"
};

bool
TeRK::ServoController::ice_isA(const ::std::string& _s, const ::Ice::Current&) const
{
    return ::std::binary_search(__TeRK__ServoController_ids, __TeRK__ServoController_ids + 4, _s);
}

::std::vector< ::std::string>
TeRK::ServoController::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&__TeRK__ServoController_ids[0], &__TeRK__ServoController_ids[4]);
}

const ::std::string&
TeRK::ServoController::ice_id(const ::Ice::Current&) const
{
    return __TeRK__ServoController_ids[3];
}

const ::std::string&
TeRK::ServoController::ice_staticId()
{
    return __TeRK__ServoController_ids[3];
}

::IceInternal::DispatchStatus
TeRK::ServoController::___execute(::IceInternal::Incoming&__inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::TeRK::ServoCommand command;
    command.__read(__is);
    try
    {
	::TeRK::ServoState __ret = execute(command, __current);
	__ret.__write(__os);
    }
    catch(const ::TeRK::ServoCommandException& __ex)
    {
	__os->write(__ex);
	return ::IceInternal::DispatchUserException;
    }
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
TeRK::ServoController::___cameraTiltUp(::IceInternal::Incoming&, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    cameraTiltUp(__current);
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
TeRK::ServoController::___cameraTiltDown(::IceInternal::Incoming&, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    cameraTiltDown(__current);
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
TeRK::ServoController::___cameraPanLeft(::IceInternal::Incoming&, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    cameraPanLeft(__current);
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
TeRK::ServoController::___cameraPanRight(::IceInternal::Incoming&, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    cameraPanRight(__current);
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
TeRK::ServoController::___setServoPositions(::IceInternal::Incoming&__inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::TeRK::IntArray servoPositions;
    __is->read(servoPositions);
    setServoPositions(servoPositions, __current);
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
TeRK::ServoController::___setServoVelocities(::IceInternal::Incoming&__inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::TeRK::IntArray servoVelocities;
    __is->read(servoVelocities);
    setServoVelocities(servoVelocities, __current);
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
TeRK::ServoController::___stopServos(::IceInternal::Incoming&, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    stopServos(__current);
    return ::IceInternal::DispatchOK;
}

static ::std::string __TeRK__ServoController_all[] =
{
    "cameraPanLeft",
    "cameraPanRight",
    "cameraTiltDown",
    "cameraTiltUp",
    "execute",
    "getProperties",
    "getProperty",
    "getPropertyKeys",
    "ice_id",
    "ice_ids",
    "ice_isA",
    "ice_ping",
    "setProperty",
    "setServoPositions",
    "setServoVelocities",
    "stopServos"
};

::IceInternal::DispatchStatus
TeRK::ServoController::__dispatch(::IceInternal::Incoming& in, const ::Ice::Current& current)
{
    ::std::pair< ::std::string*, ::std::string*> r = ::std::equal_range(__TeRK__ServoController_all, __TeRK__ServoController_all + 16, current.operation);
    if(r.first == r.second)
    {
	return ::IceInternal::DispatchOperationNotExist;
    }

    switch(r.first - __TeRK__ServoController_all)
    {
	case 0:
	{
	    return ___cameraPanLeft(in, current);
	}
	case 1:
	{
	    return ___cameraPanRight(in, current);
	}
	case 2:
	{
	    return ___cameraTiltDown(in, current);
	}
	case 3:
	{
	    return ___cameraTiltUp(in, current);
	}
	case 4:
	{
	    return ___execute(in, current);
	}
	case 5:
	{
	    return ___getProperties(in, current);
	}
	case 6:
	{
	    return ___getProperty(in, current);
	}
	case 7:
	{
	    return ___getPropertyKeys(in, current);
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
	    return ___setProperty(in, current);
	}
	case 13:
	{
	    return ___setServoPositions(in, current);
	}
	case 14:
	{
	    return ___setServoVelocities(in, current);
	}
	case 15:
	{
	    return ___stopServos(in, current);
	}
    }

    assert(false);
    return ::IceInternal::DispatchOperationNotExist;
}

void
TeRK::ServoController::__write(::IceInternal::BasicStream* __os) const
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
TeRK::ServoController::__read(::IceInternal::BasicStream* __is, bool __rid)
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
TeRK::ServoController::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type TeRK::ServoController was not generated with stream support";
    throw ex;
}

void
TeRK::ServoController::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type TeRK::ServoController was not generated with stream support";
    throw ex;
}

void 
TeRK::__patch__ServoControllerPtr(void* __addr, ::Ice::ObjectPtr& v)
{
    ::TeRK::ServoControllerPtr* p = static_cast< ::TeRK::ServoControllerPtr*>(__addr);
    assert(p);
    *p = ::TeRK::ServoControllerPtr::dynamicCast(v);
    if(v && !*p)
    {
	::Ice::NoObjectFactoryException e(__FILE__, __LINE__);
	e.type = ::TeRK::ServoController::ice_staticId();
	throw e;
    }
}

bool
TeRK::operator==(const ::TeRK::ServoController& l, const ::TeRK::ServoController& r)
{
    return static_cast<const ::Ice::Object&>(l) == static_cast<const ::Ice::Object&>(r);
}

bool
TeRK::operator!=(const ::TeRK::ServoController& l, const ::TeRK::ServoController& r)
{
    return static_cast<const ::Ice::Object&>(l) != static_cast<const ::Ice::Object&>(r);
}

bool
TeRK::operator<(const ::TeRK::ServoController& l, const ::TeRK::ServoController& r)
{
    return static_cast<const ::Ice::Object&>(l) < static_cast<const ::Ice::Object&>(r);
}

bool
TeRK::operator<=(const ::TeRK::ServoController& l, const ::TeRK::ServoController& r)
{
    return l < r || l == r;
}

bool
TeRK::operator>(const ::TeRK::ServoController& l, const ::TeRK::ServoController& r)
{
    return !(l < r) && !(l == r);
}

bool
TeRK::operator>=(const ::TeRK::ServoController& l, const ::TeRK::ServoController& r)
{
    return !(l < r);
}

::Ice::ObjectPtr
TeRK::VideoStreamerClient::ice_clone() const
{
    throw ::Ice::CloneNotImplementedException(__FILE__, __LINE__);
    return 0; // to avoid a warning with some compilers
}

static const ::std::string __TeRK__VideoStreamerClient_ids[2] =
{
    "::Ice::Object",
    "::TeRK::VideoStreamerClient"
};

bool
TeRK::VideoStreamerClient::ice_isA(const ::std::string& _s, const ::Ice::Current&) const
{
    return ::std::binary_search(__TeRK__VideoStreamerClient_ids, __TeRK__VideoStreamerClient_ids + 2, _s);
}

::std::vector< ::std::string>
TeRK::VideoStreamerClient::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&__TeRK__VideoStreamerClient_ids[0], &__TeRK__VideoStreamerClient_ids[2]);
}

const ::std::string&
TeRK::VideoStreamerClient::ice_id(const ::Ice::Current&) const
{
    return __TeRK__VideoStreamerClient_ids[1];
}

const ::std::string&
TeRK::VideoStreamerClient::ice_staticId()
{
    return __TeRK__VideoStreamerClient_ids[1];
}

::IceInternal::DispatchStatus
TeRK::VideoStreamerClient::___newFrame(::IceInternal::Incoming&__inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::TeRK::Image frame;
    frame.__read(__is);
    newFrame(frame, __current);
    return ::IceInternal::DispatchOK;
}

static ::std::string __TeRK__VideoStreamerClient_all[] =
{
    "ice_id",
    "ice_ids",
    "ice_isA",
    "ice_ping",
    "newFrame"
};

::IceInternal::DispatchStatus
TeRK::VideoStreamerClient::__dispatch(::IceInternal::Incoming& in, const ::Ice::Current& current)
{
    ::std::pair< ::std::string*, ::std::string*> r = ::std::equal_range(__TeRK__VideoStreamerClient_all, __TeRK__VideoStreamerClient_all + 5, current.operation);
    if(r.first == r.second)
    {
	return ::IceInternal::DispatchOperationNotExist;
    }

    switch(r.first - __TeRK__VideoStreamerClient_all)
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
	    return ___newFrame(in, current);
	}
    }

    assert(false);
    return ::IceInternal::DispatchOperationNotExist;
}

void
TeRK::VideoStreamerClient::__write(::IceInternal::BasicStream* __os) const
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
TeRK::VideoStreamerClient::__read(::IceInternal::BasicStream* __is, bool __rid)
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
TeRK::VideoStreamerClient::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type TeRK::VideoStreamerClient was not generated with stream support";
    throw ex;
}

void
TeRK::VideoStreamerClient::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type TeRK::VideoStreamerClient was not generated with stream support";
    throw ex;
}

void 
TeRK::__patch__VideoStreamerClientPtr(void* __addr, ::Ice::ObjectPtr& v)
{
    ::TeRK::VideoStreamerClientPtr* p = static_cast< ::TeRK::VideoStreamerClientPtr*>(__addr);
    assert(p);
    *p = ::TeRK::VideoStreamerClientPtr::dynamicCast(v);
    if(v && !*p)
    {
	::Ice::NoObjectFactoryException e(__FILE__, __LINE__);
	e.type = ::TeRK::VideoStreamerClient::ice_staticId();
	throw e;
    }
}

bool
TeRK::operator==(const ::TeRK::VideoStreamerClient& l, const ::TeRK::VideoStreamerClient& r)
{
    return static_cast<const ::Ice::Object&>(l) == static_cast<const ::Ice::Object&>(r);
}

bool
TeRK::operator!=(const ::TeRK::VideoStreamerClient& l, const ::TeRK::VideoStreamerClient& r)
{
    return static_cast<const ::Ice::Object&>(l) != static_cast<const ::Ice::Object&>(r);
}

bool
TeRK::operator<(const ::TeRK::VideoStreamerClient& l, const ::TeRK::VideoStreamerClient& r)
{
    return static_cast<const ::Ice::Object&>(l) < static_cast<const ::Ice::Object&>(r);
}

bool
TeRK::operator<=(const ::TeRK::VideoStreamerClient& l, const ::TeRK::VideoStreamerClient& r)
{
    return l < r || l == r;
}

bool
TeRK::operator>(const ::TeRK::VideoStreamerClient& l, const ::TeRK::VideoStreamerClient& r)
{
    return !(l < r) && !(l == r);
}

bool
TeRK::operator>=(const ::TeRK::VideoStreamerClient& l, const ::TeRK::VideoStreamerClient& r)
{
    return !(l < r);
}

::Ice::ObjectPtr
TeRK::VideoStreamerServer::ice_clone() const
{
    throw ::Ice::CloneNotImplementedException(__FILE__, __LINE__);
    return 0; // to avoid a warning with some compilers
}

static const ::std::string __TeRK__VideoStreamerServer_ids[4] =
{
    "::Ice::Object",
    "::TeRK::AbstractCommandController",
    "::TeRK::PropertyManager",
    "::TeRK::VideoStreamerServer"
};

bool
TeRK::VideoStreamerServer::ice_isA(const ::std::string& _s, const ::Ice::Current&) const
{
    return ::std::binary_search(__TeRK__VideoStreamerServer_ids, __TeRK__VideoStreamerServer_ids + 4, _s);
}

::std::vector< ::std::string>
TeRK::VideoStreamerServer::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&__TeRK__VideoStreamerServer_ids[0], &__TeRK__VideoStreamerServer_ids[4]);
}

const ::std::string&
TeRK::VideoStreamerServer::ice_id(const ::Ice::Current&) const
{
    return __TeRK__VideoStreamerServer_ids[3];
}

const ::std::string&
TeRK::VideoStreamerServer::ice_staticId()
{
    return __TeRK__VideoStreamerServer_ids[3];
}

::IceInternal::DispatchStatus
TeRK::VideoStreamerServer::___startVideoStream(::IceInternal::Incoming&__inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Idempotent, __current.mode);
    ::IceInternal::BasicStream* __os = __inS.os();
    try
    {
	startVideoStream(__current);
    }
    catch(const ::TeRK::VideoException& __ex)
    {
	__os->write(__ex);
	return ::IceInternal::DispatchUserException;
    }
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
TeRK::VideoStreamerServer::___stopVideoStream(::IceInternal::Incoming&__inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Idempotent, __current.mode);
    ::IceInternal::BasicStream* __os = __inS.os();
    try
    {
	stopVideoStream(__current);
    }
    catch(const ::TeRK::VideoException& __ex)
    {
	__os->write(__ex);
	return ::IceInternal::DispatchUserException;
    }
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
TeRK::VideoStreamerServer::___startCamera(::IceInternal::Incoming&__inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Idempotent, __current.mode);
    ::IceInternal::BasicStream* __os = __inS.os();
    try
    {
	::Ice::Int __ret = startCamera(__current);
	__os->write(__ret);
    }
    catch(const ::TeRK::VideoException& __ex)
    {
	__os->write(__ex);
	return ::IceInternal::DispatchUserException;
    }
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
TeRK::VideoStreamerServer::___stopCamera(::IceInternal::Incoming&__inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Idempotent, __current.mode);
    ::IceInternal::BasicStream* __os = __inS.os();
    try
    {
	::Ice::Int __ret = stopCamera(__current);
	__os->write(__ret);
    }
    catch(const ::TeRK::VideoException& __ex)
    {
	__os->write(__ex);
	return ::IceInternal::DispatchUserException;
    }
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
TeRK::VideoStreamerServer::___getFrame(::IceInternal::Incoming&__inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::Ice::Int frameNumber;
    __is->read(frameNumber);
    try
    {
	::TeRK::Image __ret = getFrame(frameNumber, __current);
	__ret.__write(__os);
    }
    catch(const ::TeRK::VideoException& __ex)
    {
	__os->write(__ex);
	return ::IceInternal::DispatchUserException;
    }
    return ::IceInternal::DispatchOK;
}

static ::std::string __TeRK__VideoStreamerServer_all[] =
{
    "getFrame",
    "getProperties",
    "getProperty",
    "getPropertyKeys",
    "ice_id",
    "ice_ids",
    "ice_isA",
    "ice_ping",
    "setProperty",
    "startCamera",
    "startVideoStream",
    "stopCamera",
    "stopVideoStream"
};

::IceInternal::DispatchStatus
TeRK::VideoStreamerServer::__dispatch(::IceInternal::Incoming& in, const ::Ice::Current& current)
{
    ::std::pair< ::std::string*, ::std::string*> r = ::std::equal_range(__TeRK__VideoStreamerServer_all, __TeRK__VideoStreamerServer_all + 13, current.operation);
    if(r.first == r.second)
    {
	return ::IceInternal::DispatchOperationNotExist;
    }

    switch(r.first - __TeRK__VideoStreamerServer_all)
    {
	case 0:
	{
	    return ___getFrame(in, current);
	}
	case 1:
	{
	    return ___getProperties(in, current);
	}
	case 2:
	{
	    return ___getProperty(in, current);
	}
	case 3:
	{
	    return ___getPropertyKeys(in, current);
	}
	case 4:
	{
	    return ___ice_id(in, current);
	}
	case 5:
	{
	    return ___ice_ids(in, current);
	}
	case 6:
	{
	    return ___ice_isA(in, current);
	}
	case 7:
	{
	    return ___ice_ping(in, current);
	}
	case 8:
	{
	    return ___setProperty(in, current);
	}
	case 9:
	{
	    return ___startCamera(in, current);
	}
	case 10:
	{
	    return ___startVideoStream(in, current);
	}
	case 11:
	{
	    return ___stopCamera(in, current);
	}
	case 12:
	{
	    return ___stopVideoStream(in, current);
	}
    }

    assert(false);
    return ::IceInternal::DispatchOperationNotExist;
}

void
TeRK::VideoStreamerServer::__write(::IceInternal::BasicStream* __os) const
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
TeRK::VideoStreamerServer::__read(::IceInternal::BasicStream* __is, bool __rid)
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
TeRK::VideoStreamerServer::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type TeRK::VideoStreamerServer was not generated with stream support";
    throw ex;
}

void
TeRK::VideoStreamerServer::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type TeRK::VideoStreamerServer was not generated with stream support";
    throw ex;
}

void 
TeRK::__patch__VideoStreamerServerPtr(void* __addr, ::Ice::ObjectPtr& v)
{
    ::TeRK::VideoStreamerServerPtr* p = static_cast< ::TeRK::VideoStreamerServerPtr*>(__addr);
    assert(p);
    *p = ::TeRK::VideoStreamerServerPtr::dynamicCast(v);
    if(v && !*p)
    {
	::Ice::NoObjectFactoryException e(__FILE__, __LINE__);
	e.type = ::TeRK::VideoStreamerServer::ice_staticId();
	throw e;
    }
}

bool
TeRK::operator==(const ::TeRK::VideoStreamerServer& l, const ::TeRK::VideoStreamerServer& r)
{
    return static_cast<const ::Ice::Object&>(l) == static_cast<const ::Ice::Object&>(r);
}

bool
TeRK::operator!=(const ::TeRK::VideoStreamerServer& l, const ::TeRK::VideoStreamerServer& r)
{
    return static_cast<const ::Ice::Object&>(l) != static_cast<const ::Ice::Object&>(r);
}

bool
TeRK::operator<(const ::TeRK::VideoStreamerServer& l, const ::TeRK::VideoStreamerServer& r)
{
    return static_cast<const ::Ice::Object&>(l) < static_cast<const ::Ice::Object&>(r);
}

bool
TeRK::operator<=(const ::TeRK::VideoStreamerServer& l, const ::TeRK::VideoStreamerServer& r)
{
    return l < r || l == r;
}

bool
TeRK::operator>(const ::TeRK::VideoStreamerServer& l, const ::TeRK::VideoStreamerServer& r)
{
    return !(l < r) && !(l == r);
}

bool
TeRK::operator>=(const ::TeRK::VideoStreamerServer& l, const ::TeRK::VideoStreamerServer& r)
{
    return !(l < r);
}

::Ice::ObjectPtr
TeRK::TerkUser::ice_clone() const
{
    throw ::Ice::CloneNotImplementedException(__FILE__, __LINE__);
    return 0; // to avoid a warning with some compilers
}

static const ::std::string __TeRK__TerkUser_ids[6] =
{
    "::Ice::Object",
    "::TeRK::PropertyManager",
    "::TeRK::TerkUser",
    "::peer::ConnectionEventHandler",
    "::peer::PeerConnectionEventHandler",
    "::peer::UserConnectionEventHandler"
};

bool
TeRK::TerkUser::ice_isA(const ::std::string& _s, const ::Ice::Current&) const
{
    return ::std::binary_search(__TeRK__TerkUser_ids, __TeRK__TerkUser_ids + 6, _s);
}

::std::vector< ::std::string>
TeRK::TerkUser::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&__TeRK__TerkUser_ids[0], &__TeRK__TerkUser_ids[6]);
}

const ::std::string&
TeRK::TerkUser::ice_id(const ::Ice::Current&) const
{
    return __TeRK__TerkUser_ids[2];
}

const ::std::string&
TeRK::TerkUser::ice_staticId()
{
    return __TeRK__TerkUser_ids[2];
}

::IceInternal::DispatchStatus
TeRK::TerkUser::___getSupportedServices(::IceInternal::Incoming&__inS, const ::Ice::Current& __current) const
{
    __checkMode(::Ice::Nonmutating, __current.mode);
    ::IceInternal::BasicStream* __os = __inS.os();
    ::TeRK::ProxyTypeIdToIdentityMap __ret = getSupportedServices(__current);
    ::TeRK::__write(__os, __ret, ::TeRK::__U__ProxyTypeIdToIdentityMap());
    return ::IceInternal::DispatchOK;
}

static ::std::string __TeRK__TerkUser_all[] =
{
    "forcedLogoutNotification",
    "getProperties",
    "getProperty",
    "getPropertyKeys",
    "getSupportedServices",
    "ice_id",
    "ice_ids",
    "ice_isA",
    "ice_ping",
    "peerConnected",
    "peerConnectedNoProxy",
    "peerDisconnected",
    "setProperty"
};

::IceInternal::DispatchStatus
TeRK::TerkUser::__dispatch(::IceInternal::Incoming& in, const ::Ice::Current& current)
{
    ::std::pair< ::std::string*, ::std::string*> r = ::std::equal_range(__TeRK__TerkUser_all, __TeRK__TerkUser_all + 13, current.operation);
    if(r.first == r.second)
    {
	return ::IceInternal::DispatchOperationNotExist;
    }

    switch(r.first - __TeRK__TerkUser_all)
    {
	case 0:
	{
	    return ___forcedLogoutNotification(in, current);
	}
	case 1:
	{
	    return ___getProperties(in, current);
	}
	case 2:
	{
	    return ___getProperty(in, current);
	}
	case 3:
	{
	    return ___getPropertyKeys(in, current);
	}
	case 4:
	{
	    return ___getSupportedServices(in, current);
	}
	case 5:
	{
	    return ___ice_id(in, current);
	}
	case 6:
	{
	    return ___ice_ids(in, current);
	}
	case 7:
	{
	    return ___ice_isA(in, current);
	}
	case 8:
	{
	    return ___ice_ping(in, current);
	}
	case 9:
	{
	    return ___peerConnected(in, current);
	}
	case 10:
	{
	    return ___peerConnectedNoProxy(in, current);
	}
	case 11:
	{
	    return ___peerDisconnected(in, current);
	}
	case 12:
	{
	    return ___setProperty(in, current);
	}
    }

    assert(false);
    return ::IceInternal::DispatchOperationNotExist;
}

void
TeRK::TerkUser::__write(::IceInternal::BasicStream* __os) const
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
TeRK::TerkUser::__read(::IceInternal::BasicStream* __is, bool __rid)
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
TeRK::TerkUser::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type TeRK::TerkUser was not generated with stream support";
    throw ex;
}

void
TeRK::TerkUser::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type TeRK::TerkUser was not generated with stream support";
    throw ex;
}

void 
TeRK::__patch__TerkUserPtr(void* __addr, ::Ice::ObjectPtr& v)
{
    ::TeRK::TerkUserPtr* p = static_cast< ::TeRK::TerkUserPtr*>(__addr);
    assert(p);
    *p = ::TeRK::TerkUserPtr::dynamicCast(v);
    if(v && !*p)
    {
	::Ice::NoObjectFactoryException e(__FILE__, __LINE__);
	e.type = ::TeRK::TerkUser::ice_staticId();
	throw e;
    }
}

bool
TeRK::operator==(const ::TeRK::TerkUser& l, const ::TeRK::TerkUser& r)
{
    return static_cast<const ::Ice::Object&>(l) == static_cast<const ::Ice::Object&>(r);
}

bool
TeRK::operator!=(const ::TeRK::TerkUser& l, const ::TeRK::TerkUser& r)
{
    return static_cast<const ::Ice::Object&>(l) != static_cast<const ::Ice::Object&>(r);
}

bool
TeRK::operator<(const ::TeRK::TerkUser& l, const ::TeRK::TerkUser& r)
{
    return static_cast<const ::Ice::Object&>(l) < static_cast<const ::Ice::Object&>(r);
}

bool
TeRK::operator<=(const ::TeRK::TerkUser& l, const ::TeRK::TerkUser& r)
{
    return l < r || l == r;
}

bool
TeRK::operator>(const ::TeRK::TerkUser& l, const ::TeRK::TerkUser& r)
{
    return !(l < r) && !(l == r);
}

bool
TeRK::operator>=(const ::TeRK::TerkUser& l, const ::TeRK::TerkUser& r)
{
    return !(l < r);
}

::Ice::ObjectPtr
TeRK::Qwerk::ice_clone() const
{
    throw ::Ice::CloneNotImplementedException(__FILE__, __LINE__);
    return 0; // to avoid a warning with some compilers
}

static const ::std::string __TeRK__Qwerk_ids[7] =
{
    "::Ice::Object",
    "::TeRK::PropertyManager",
    "::TeRK::Qwerk",
    "::TeRK::TerkUser",
    "::peer::ConnectionEventHandler",
    "::peer::PeerConnectionEventHandler",
    "::peer::UserConnectionEventHandler"
};

bool
TeRK::Qwerk::ice_isA(const ::std::string& _s, const ::Ice::Current&) const
{
    return ::std::binary_search(__TeRK__Qwerk_ids, __TeRK__Qwerk_ids + 7, _s);
}

::std::vector< ::std::string>
TeRK::Qwerk::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&__TeRK__Qwerk_ids[0], &__TeRK__Qwerk_ids[7]);
}

const ::std::string&
TeRK::Qwerk::ice_id(const ::Ice::Current&) const
{
    return __TeRK__Qwerk_ids[2];
}

const ::std::string&
TeRK::Qwerk::ice_staticId()
{
    return __TeRK__Qwerk_ids[2];
}

::IceInternal::DispatchStatus
TeRK::Qwerk::___getState(::IceInternal::Incoming&__inS, const ::Ice::Current& __current) const
{
    __checkMode(::Ice::Nonmutating, __current.mode);
    ::IceInternal::BasicStream* __os = __inS.os();
    ::TeRK::QwerkState __ret = getState(__current);
    __ret.__write(__os);
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
TeRK::Qwerk::___execute(::IceInternal::Incoming&__inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::TeRK::QwerkCommand command;
    command.__read(__is);
    try
    {
	::TeRK::QwerkState __ret = execute(command, __current);
	__ret.__write(__os);
    }
    catch(const ::TeRK::CommandException& __ex)
    {
	__os->write(__ex);
	return ::IceInternal::DispatchUserException;
    }
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
TeRK::Qwerk::___emergencyStop(::IceInternal::Incoming&__inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Idempotent, __current.mode);
    ::IceInternal::BasicStream* __os = __inS.os();
    ::TeRK::QwerkState __ret = emergencyStop(__current);
    __ret.__write(__os);
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
TeRK::Qwerk::___getCommandControllerTypeToProxyIdentityMap(::IceInternal::Incoming&__inS, const ::Ice::Current& __current) const
{
    __checkMode(::Ice::Nonmutating, __current.mode);
    ::IceInternal::BasicStream* __os = __inS.os();
    ::TeRK::ProxyTypeIdToIdentityMap __ret = getCommandControllerTypeToProxyIdentityMap(__current);
    ::TeRK::__write(__os, __ret, ::TeRK::__U__ProxyTypeIdToIdentityMap());
    return ::IceInternal::DispatchOK;
}

static ::std::string __TeRK__Qwerk_all[] =
{
    "emergencyStop",
    "execute",
    "forcedLogoutNotification",
    "getCommandControllerTypeToProxyIdentityMap",
    "getProperties",
    "getProperty",
    "getPropertyKeys",
    "getState",
    "getSupportedServices",
    "ice_id",
    "ice_ids",
    "ice_isA",
    "ice_ping",
    "peerConnected",
    "peerConnectedNoProxy",
    "peerDisconnected",
    "setProperty"
};

::IceInternal::DispatchStatus
TeRK::Qwerk::__dispatch(::IceInternal::Incoming& in, const ::Ice::Current& current)
{
    ::std::pair< ::std::string*, ::std::string*> r = ::std::equal_range(__TeRK__Qwerk_all, __TeRK__Qwerk_all + 17, current.operation);
    if(r.first == r.second)
    {
	return ::IceInternal::DispatchOperationNotExist;
    }

    switch(r.first - __TeRK__Qwerk_all)
    {
	case 0:
	{
	    return ___emergencyStop(in, current);
	}
	case 1:
	{
	    return ___execute(in, current);
	}
	case 2:
	{
	    return ___forcedLogoutNotification(in, current);
	}
	case 3:
	{
	    return ___getCommandControllerTypeToProxyIdentityMap(in, current);
	}
	case 4:
	{
	    return ___getProperties(in, current);
	}
	case 5:
	{
	    return ___getProperty(in, current);
	}
	case 6:
	{
	    return ___getPropertyKeys(in, current);
	}
	case 7:
	{
	    return ___getState(in, current);
	}
	case 8:
	{
	    return ___getSupportedServices(in, current);
	}
	case 9:
	{
	    return ___ice_id(in, current);
	}
	case 10:
	{
	    return ___ice_ids(in, current);
	}
	case 11:
	{
	    return ___ice_isA(in, current);
	}
	case 12:
	{
	    return ___ice_ping(in, current);
	}
	case 13:
	{
	    return ___peerConnected(in, current);
	}
	case 14:
	{
	    return ___peerConnectedNoProxy(in, current);
	}
	case 15:
	{
	    return ___peerDisconnected(in, current);
	}
	case 16:
	{
	    return ___setProperty(in, current);
	}
    }

    assert(false);
    return ::IceInternal::DispatchOperationNotExist;
}

void
TeRK::Qwerk::__write(::IceInternal::BasicStream* __os) const
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
TeRK::Qwerk::__read(::IceInternal::BasicStream* __is, bool __rid)
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
TeRK::Qwerk::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type TeRK::Qwerk was not generated with stream support";
    throw ex;
}

void
TeRK::Qwerk::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type TeRK::Qwerk was not generated with stream support";
    throw ex;
}

void 
TeRK::__patch__QwerkPtr(void* __addr, ::Ice::ObjectPtr& v)
{
    ::TeRK::QwerkPtr* p = static_cast< ::TeRK::QwerkPtr*>(__addr);
    assert(p);
    *p = ::TeRK::QwerkPtr::dynamicCast(v);
    if(v && !*p)
    {
	::Ice::NoObjectFactoryException e(__FILE__, __LINE__);
	e.type = ::TeRK::Qwerk::ice_staticId();
	throw e;
    }
}

bool
TeRK::operator==(const ::TeRK::Qwerk& l, const ::TeRK::Qwerk& r)
{
    return static_cast<const ::Ice::Object&>(l) == static_cast<const ::Ice::Object&>(r);
}

bool
TeRK::operator!=(const ::TeRK::Qwerk& l, const ::TeRK::Qwerk& r)
{
    return static_cast<const ::Ice::Object&>(l) != static_cast<const ::Ice::Object&>(r);
}

bool
TeRK::operator<(const ::TeRK::Qwerk& l, const ::TeRK::Qwerk& r)
{
    return static_cast<const ::Ice::Object&>(l) < static_cast<const ::Ice::Object&>(r);
}

bool
TeRK::operator<=(const ::TeRK::Qwerk& l, const ::TeRK::Qwerk& r)
{
    return l < r || l == r;
}

bool
TeRK::operator>(const ::TeRK::Qwerk& l, const ::TeRK::Qwerk& r)
{
    return !(l < r) && !(l == r);
}

bool
TeRK::operator>=(const ::TeRK::Qwerk& l, const ::TeRK::Qwerk& r)
{
    return !(l < r);
}

::Ice::ObjectPtr
TeRK::TerkClient::ice_clone() const
{
    throw ::Ice::CloneNotImplementedException(__FILE__, __LINE__);
    return 0; // to avoid a warning with some compilers
}

static const ::std::string __TeRK__TerkClient_ids[8] =
{
    "::Ice::Object",
    "::TeRK::PropertyManager",
    "::TeRK::TerkClient",
    "::TeRK::TerkUser",
    "::TeRK::VideoStreamerClient",
    "::peer::ConnectionEventHandler",
    "::peer::PeerConnectionEventHandler",
    "::peer::UserConnectionEventHandler"
};

bool
TeRK::TerkClient::ice_isA(const ::std::string& _s, const ::Ice::Current&) const
{
    return ::std::binary_search(__TeRK__TerkClient_ids, __TeRK__TerkClient_ids + 8, _s);
}

::std::vector< ::std::string>
TeRK::TerkClient::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&__TeRK__TerkClient_ids[0], &__TeRK__TerkClient_ids[8]);
}

const ::std::string&
TeRK::TerkClient::ice_id(const ::Ice::Current&) const
{
    return __TeRK__TerkClient_ids[2];
}

const ::std::string&
TeRK::TerkClient::ice_staticId()
{
    return __TeRK__TerkClient_ids[2];
}

static ::std::string __TeRK__TerkClient_all[] =
{
    "forcedLogoutNotification",
    "getProperties",
    "getProperty",
    "getPropertyKeys",
    "getSupportedServices",
    "ice_id",
    "ice_ids",
    "ice_isA",
    "ice_ping",
    "newFrame",
    "peerConnected",
    "peerConnectedNoProxy",
    "peerDisconnected",
    "setProperty"
};

::IceInternal::DispatchStatus
TeRK::TerkClient::__dispatch(::IceInternal::Incoming& in, const ::Ice::Current& current)
{
    ::std::pair< ::std::string*, ::std::string*> r = ::std::equal_range(__TeRK__TerkClient_all, __TeRK__TerkClient_all + 14, current.operation);
    if(r.first == r.second)
    {
	return ::IceInternal::DispatchOperationNotExist;
    }

    switch(r.first - __TeRK__TerkClient_all)
    {
	case 0:
	{
	    return ___forcedLogoutNotification(in, current);
	}
	case 1:
	{
	    return ___getProperties(in, current);
	}
	case 2:
	{
	    return ___getProperty(in, current);
	}
	case 3:
	{
	    return ___getPropertyKeys(in, current);
	}
	case 4:
	{
	    return ___getSupportedServices(in, current);
	}
	case 5:
	{
	    return ___ice_id(in, current);
	}
	case 6:
	{
	    return ___ice_ids(in, current);
	}
	case 7:
	{
	    return ___ice_isA(in, current);
	}
	case 8:
	{
	    return ___ice_ping(in, current);
	}
	case 9:
	{
	    return ___newFrame(in, current);
	}
	case 10:
	{
	    return ___peerConnected(in, current);
	}
	case 11:
	{
	    return ___peerConnectedNoProxy(in, current);
	}
	case 12:
	{
	    return ___peerDisconnected(in, current);
	}
	case 13:
	{
	    return ___setProperty(in, current);
	}
    }

    assert(false);
    return ::IceInternal::DispatchOperationNotExist;
}

void
TeRK::TerkClient::__write(::IceInternal::BasicStream* __os) const
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
TeRK::TerkClient::__read(::IceInternal::BasicStream* __is, bool __rid)
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
TeRK::TerkClient::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type TeRK::TerkClient was not generated with stream support";
    throw ex;
}

void
TeRK::TerkClient::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type TeRK::TerkClient was not generated with stream support";
    throw ex;
}

void 
TeRK::__patch__TerkClientPtr(void* __addr, ::Ice::ObjectPtr& v)
{
    ::TeRK::TerkClientPtr* p = static_cast< ::TeRK::TerkClientPtr*>(__addr);
    assert(p);
    *p = ::TeRK::TerkClientPtr::dynamicCast(v);
    if(v && !*p)
    {
	::Ice::NoObjectFactoryException e(__FILE__, __LINE__);
	e.type = ::TeRK::TerkClient::ice_staticId();
	throw e;
    }
}

bool
TeRK::operator==(const ::TeRK::TerkClient& l, const ::TeRK::TerkClient& r)
{
    return static_cast<const ::Ice::Object&>(l) == static_cast<const ::Ice::Object&>(r);
}

bool
TeRK::operator!=(const ::TeRK::TerkClient& l, const ::TeRK::TerkClient& r)
{
    return static_cast<const ::Ice::Object&>(l) != static_cast<const ::Ice::Object&>(r);
}

bool
TeRK::operator<(const ::TeRK::TerkClient& l, const ::TeRK::TerkClient& r)
{
    return static_cast<const ::Ice::Object&>(l) < static_cast<const ::Ice::Object&>(r);
}

bool
TeRK::operator<=(const ::TeRK::TerkClient& l, const ::TeRK::TerkClient& r)
{
    return l < r || l == r;
}

bool
TeRK::operator>(const ::TeRK::TerkClient& l, const ::TeRK::TerkClient& r)
{
    return !(l < r) && !(l == r);
}

bool
TeRK::operator>=(const ::TeRK::TerkClient& l, const ::TeRK::TerkClient& r)
{
    return !(l < r);
}

#endif
