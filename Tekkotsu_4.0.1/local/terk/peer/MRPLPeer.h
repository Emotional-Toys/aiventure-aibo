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

#ifndef ___home_btsai_terk_source_TeRKPeerCommon_code_c____MRPLPeer_h__
#define ___home_btsai_terk_source_TeRKPeerCommon_code_c____MRPLPeer_h__

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
#include <Glacier2/Session.h>
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

namespace peer
{

class UserConnectionEventHandler;
bool operator==(const UserConnectionEventHandler&, const UserConnectionEventHandler&);
bool operator!=(const UserConnectionEventHandler&, const UserConnectionEventHandler&);
bool operator<(const UserConnectionEventHandler&, const UserConnectionEventHandler&);
bool operator<=(const UserConnectionEventHandler&, const UserConnectionEventHandler&);
bool operator>(const UserConnectionEventHandler&, const UserConnectionEventHandler&);
bool operator>=(const UserConnectionEventHandler&, const UserConnectionEventHandler&);

class PeerConnectionEventHandler;
bool operator==(const PeerConnectionEventHandler&, const PeerConnectionEventHandler&);
bool operator!=(const PeerConnectionEventHandler&, const PeerConnectionEventHandler&);
bool operator<(const PeerConnectionEventHandler&, const PeerConnectionEventHandler&);
bool operator<=(const PeerConnectionEventHandler&, const PeerConnectionEventHandler&);
bool operator>(const PeerConnectionEventHandler&, const PeerConnectionEventHandler&);
bool operator>=(const PeerConnectionEventHandler&, const PeerConnectionEventHandler&);

class ConnectionEventHandler;
bool operator==(const ConnectionEventHandler&, const ConnectionEventHandler&);
bool operator!=(const ConnectionEventHandler&, const ConnectionEventHandler&);
bool operator<(const ConnectionEventHandler&, const ConnectionEventHandler&);
bool operator<=(const ConnectionEventHandler&, const ConnectionEventHandler&);
bool operator>(const ConnectionEventHandler&, const ConnectionEventHandler&);
bool operator>=(const ConnectionEventHandler&, const ConnectionEventHandler&);

class PeerRegistrationHandler;
bool operator==(const PeerRegistrationHandler&, const PeerRegistrationHandler&);
bool operator!=(const PeerRegistrationHandler&, const PeerRegistrationHandler&);
bool operator<(const PeerRegistrationHandler&, const PeerRegistrationHandler&);
bool operator<=(const PeerRegistrationHandler&, const PeerRegistrationHandler&);
bool operator>(const PeerRegistrationHandler&, const PeerRegistrationHandler&);
bool operator>=(const PeerRegistrationHandler&, const PeerRegistrationHandler&);

class UserSession;
bool operator==(const UserSession&, const UserSession&);
bool operator!=(const UserSession&, const UserSession&);
bool operator<(const UserSession&, const UserSession&);
bool operator<=(const UserSession&, const UserSession&);
bool operator>(const UserSession&, const UserSession&);
bool operator>=(const UserSession&, const UserSession&);

}

}

namespace peer
{

class UserConnectionEventHandler;
bool operator==(const UserConnectionEventHandler&, const UserConnectionEventHandler&);
bool operator!=(const UserConnectionEventHandler&, const UserConnectionEventHandler&);
bool operator<(const UserConnectionEventHandler&, const UserConnectionEventHandler&);
bool operator<=(const UserConnectionEventHandler&, const UserConnectionEventHandler&);
bool operator>(const UserConnectionEventHandler&, const UserConnectionEventHandler&);
bool operator>=(const UserConnectionEventHandler&, const UserConnectionEventHandler&);

class PeerConnectionEventHandler;
bool operator==(const PeerConnectionEventHandler&, const PeerConnectionEventHandler&);
bool operator!=(const PeerConnectionEventHandler&, const PeerConnectionEventHandler&);
bool operator<(const PeerConnectionEventHandler&, const PeerConnectionEventHandler&);
bool operator<=(const PeerConnectionEventHandler&, const PeerConnectionEventHandler&);
bool operator>(const PeerConnectionEventHandler&, const PeerConnectionEventHandler&);
bool operator>=(const PeerConnectionEventHandler&, const PeerConnectionEventHandler&);

class ConnectionEventHandler;
bool operator==(const ConnectionEventHandler&, const ConnectionEventHandler&);
bool operator!=(const ConnectionEventHandler&, const ConnectionEventHandler&);
bool operator<(const ConnectionEventHandler&, const ConnectionEventHandler&);
bool operator<=(const ConnectionEventHandler&, const ConnectionEventHandler&);
bool operator>(const ConnectionEventHandler&, const ConnectionEventHandler&);
bool operator>=(const ConnectionEventHandler&, const ConnectionEventHandler&);

class PeerRegistrationHandler;
bool operator==(const PeerRegistrationHandler&, const PeerRegistrationHandler&);
bool operator!=(const PeerRegistrationHandler&, const PeerRegistrationHandler&);
bool operator<(const PeerRegistrationHandler&, const PeerRegistrationHandler&);
bool operator<=(const PeerRegistrationHandler&, const PeerRegistrationHandler&);
bool operator>(const PeerRegistrationHandler&, const PeerRegistrationHandler&);
bool operator>=(const PeerRegistrationHandler&, const PeerRegistrationHandler&);

class UserSession;
bool operator==(const UserSession&, const UserSession&);
bool operator!=(const UserSession&, const UserSession&);
bool operator<(const UserSession&, const UserSession&);
bool operator<=(const UserSession&, const UserSession&);
bool operator>(const UserSession&, const UserSession&);
bool operator>=(const UserSession&, const UserSession&);

}

namespace IceInternal
{

void incRef(::peer::UserConnectionEventHandler*);
void decRef(::peer::UserConnectionEventHandler*);

void incRef(::IceProxy::peer::UserConnectionEventHandler*);
void decRef(::IceProxy::peer::UserConnectionEventHandler*);

void incRef(::peer::PeerConnectionEventHandler*);
void decRef(::peer::PeerConnectionEventHandler*);

void incRef(::IceProxy::peer::PeerConnectionEventHandler*);
void decRef(::IceProxy::peer::PeerConnectionEventHandler*);

void incRef(::peer::ConnectionEventHandler*);
void decRef(::peer::ConnectionEventHandler*);

void incRef(::IceProxy::peer::ConnectionEventHandler*);
void decRef(::IceProxy::peer::ConnectionEventHandler*);

void incRef(::peer::PeerRegistrationHandler*);
void decRef(::peer::PeerRegistrationHandler*);

void incRef(::IceProxy::peer::PeerRegistrationHandler*);
void decRef(::IceProxy::peer::PeerRegistrationHandler*);

void incRef(::peer::UserSession*);
void decRef(::peer::UserSession*);

void incRef(::IceProxy::peer::UserSession*);
void decRef(::IceProxy::peer::UserSession*);

}

namespace peer
{

typedef ::IceInternal::Handle< ::peer::UserConnectionEventHandler> UserConnectionEventHandlerPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::peer::UserConnectionEventHandler> UserConnectionEventHandlerPrx;

void __write(::IceInternal::BasicStream*, const UserConnectionEventHandlerPrx&);
void __read(::IceInternal::BasicStream*, UserConnectionEventHandlerPrx&);
void __write(::IceInternal::BasicStream*, const UserConnectionEventHandlerPtr&);
void __patch__UserConnectionEventHandlerPtr(void*, ::Ice::ObjectPtr&);

typedef ::IceInternal::Handle< ::peer::PeerConnectionEventHandler> PeerConnectionEventHandlerPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::peer::PeerConnectionEventHandler> PeerConnectionEventHandlerPrx;

void __write(::IceInternal::BasicStream*, const PeerConnectionEventHandlerPrx&);
void __read(::IceInternal::BasicStream*, PeerConnectionEventHandlerPrx&);
void __write(::IceInternal::BasicStream*, const PeerConnectionEventHandlerPtr&);
void __patch__PeerConnectionEventHandlerPtr(void*, ::Ice::ObjectPtr&);

typedef ::IceInternal::Handle< ::peer::ConnectionEventHandler> ConnectionEventHandlerPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::peer::ConnectionEventHandler> ConnectionEventHandlerPrx;

void __write(::IceInternal::BasicStream*, const ConnectionEventHandlerPrx&);
void __read(::IceInternal::BasicStream*, ConnectionEventHandlerPrx&);
void __write(::IceInternal::BasicStream*, const ConnectionEventHandlerPtr&);
void __patch__ConnectionEventHandlerPtr(void*, ::Ice::ObjectPtr&);

typedef ::IceInternal::Handle< ::peer::PeerRegistrationHandler> PeerRegistrationHandlerPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::peer::PeerRegistrationHandler> PeerRegistrationHandlerPrx;

void __write(::IceInternal::BasicStream*, const PeerRegistrationHandlerPrx&);
void __read(::IceInternal::BasicStream*, PeerRegistrationHandlerPrx&);
void __write(::IceInternal::BasicStream*, const PeerRegistrationHandlerPtr&);
void __patch__PeerRegistrationHandlerPtr(void*, ::Ice::ObjectPtr&);

typedef ::IceInternal::Handle< ::peer::UserSession> UserSessionPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::peer::UserSession> UserSessionPrx;

void __write(::IceInternal::BasicStream*, const UserSessionPrx&);
void __read(::IceInternal::BasicStream*, UserSessionPrx&);
void __write(::IceInternal::BasicStream*, const UserSessionPtr&);
void __patch__UserSessionPtr(void*, ::Ice::ObjectPtr&);

}

namespace peer
{

enum PeerAccessLevel
{
    AccessLevelOwner,
    AccessLevelOwnerRestricted,
    AccessLevelNormalEnhanced,
    AccessLevelNormal,
    AccessLevelNormalRestricted,
    AccessLevelGuestEnhanced,
    AccessLevelGuest,
    AccessLevelGuestRestricted,
    AccessLevelNone
};

void __write(::IceInternal::BasicStream*, PeerAccessLevel);
void __read(::IceInternal::BasicStream*, PeerAccessLevel&);

typedef ::std::vector< ::Ice::Identity> IdentityArray;

class __U__IdentityArray { };
void __write(::IceInternal::BasicStream*, const ::Ice::Identity*, const ::Ice::Identity*, __U__IdentityArray);
void __read(::IceInternal::BasicStream*, IdentityArray&, __U__IdentityArray);

typedef ::std::vector< ::Ice::ObjectPrx> ObjectProxyArray;

class __U__ObjectProxyArray { };
void __write(::IceInternal::BasicStream*, const ::Ice::ObjectPrx*, const ::Ice::ObjectPrx*, __U__ObjectProxyArray);
void __read(::IceInternal::BasicStream*, ObjectProxyArray&, __U__ObjectProxyArray);

typedef ::std::map< ::Ice::Identity, ::Ice::ObjectPrx> IdentityToProxyMap;

class __U__IdentityToProxyMap { };
void __write(::IceInternal::BasicStream*, const IdentityToProxyMap&, __U__IdentityToProxyMap);
void __read(::IceInternal::BasicStream*, IdentityToProxyMap&, __U__IdentityToProxyMap);

struct PeerIdentifier
{
    ::std::string userId;
    ::std::string firstName;
    ::std::string lastName;

    bool operator==(const PeerIdentifier&) const;
    bool operator!=(const PeerIdentifier&) const;
    bool operator<(const PeerIdentifier&) const;
    bool operator<=(const PeerIdentifier& __rhs) const
    {
	return operator<(__rhs) || operator==(__rhs);
    }
    bool operator>(const PeerIdentifier& __rhs) const
    {
	return !operator<(__rhs) && !operator==(__rhs);
    }
    bool operator>=(const PeerIdentifier& __rhs) const
    {
	return !operator<(__rhs);
    }

    void __write(::IceInternal::BasicStream*) const;
    void __read(::IceInternal::BasicStream*);
};

class PeerException : public ::Ice::UserException
{
public:

    PeerException() {}
    explicit PeerException(const ::std::string&);

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

static PeerException __PeerException_init;

class InvalidIdentityException : public ::peer::PeerException
{
public:

    InvalidIdentityException() {}
    explicit InvalidIdentityException(const ::std::string&);

    virtual const ::std::string ice_name() const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;

    static const ::IceInternal::UserExceptionFactoryPtr& ice_factory();

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);

    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

class PeerAccessException : public ::peer::PeerException
{
public:

    PeerAccessException() {}
    explicit PeerAccessException(const ::std::string&);

    virtual const ::std::string ice_name() const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;

    static const ::IceInternal::UserExceptionFactoryPtr& ice_factory();

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);

    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

class PeerUnavailableException : public ::peer::PeerException
{
public:

    PeerUnavailableException() {}
    explicit PeerUnavailableException(const ::std::string&);

    virtual const ::std::string ice_name() const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;

    static const ::IceInternal::UserExceptionFactoryPtr& ice_factory();

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);

    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

class PeerConnectionFailedException : public ::peer::PeerException
{
public:

    PeerConnectionFailedException() {}
    explicit PeerConnectionFailedException(const ::std::string&);

    virtual const ::std::string ice_name() const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;

    static const ::IceInternal::UserExceptionFactoryPtr& ice_factory();

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);

    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

class DuplicateConnectionException : public ::peer::PeerException
{
public:

    DuplicateConnectionException() {}
    explicit DuplicateConnectionException(const ::std::string&);

    virtual const ::std::string ice_name() const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;

    static const ::IceInternal::UserExceptionFactoryPtr& ice_factory();

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);

    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

class RegistrationException : public ::peer::PeerException
{
public:

    RegistrationException() {}
    explicit RegistrationException(const ::std::string&);

    virtual const ::std::string ice_name() const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;

    static const ::IceInternal::UserExceptionFactoryPtr& ice_factory();

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);

    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

typedef ::std::vector< ::peer::PeerIdentifier> PeerIdentifierSet;

class __U__PeerIdentifierSet { };
void __write(::IceInternal::BasicStream*, const ::peer::PeerIdentifier*, const ::peer::PeerIdentifier*, __U__PeerIdentifierSet);
void __read(::IceInternal::BasicStream*, PeerIdentifierSet&, __U__PeerIdentifierSet);

}

namespace peer
{

class AMI_UserConnectionEventHandler_forcedLogoutNotification : public ::IceInternal::OutgoingAsync
{
public:

    virtual void ice_response() = 0;
    virtual void ice_exception(const ::Ice::Exception&) = 0;

    void __invoke(const ::peer::UserConnectionEventHandlerPrx&, const ::Ice::Context&);

protected:

    virtual void __response(bool);
};

typedef ::IceUtil::Handle< ::peer::AMI_UserConnectionEventHandler_forcedLogoutNotification> AMI_UserConnectionEventHandler_forcedLogoutNotificationPtr;

class AMI_PeerConnectionEventHandler_peerConnected : public ::IceInternal::OutgoingAsync
{
public:

    virtual void ice_response() = 0;
    virtual void ice_exception(const ::Ice::Exception&) = 0;

    void __invoke(const ::peer::PeerConnectionEventHandlerPrx&, const ::std::string&, ::peer::PeerAccessLevel, const ::Ice::ObjectPrx&, const ::Ice::Context&);

protected:

    virtual void __response(bool);
};

typedef ::IceUtil::Handle< ::peer::AMI_PeerConnectionEventHandler_peerConnected> AMI_PeerConnectionEventHandler_peerConnectedPtr;

class AMI_PeerConnectionEventHandler_peerConnectedNoProxy : public ::IceInternal::OutgoingAsync
{
public:

    virtual void ice_response() = 0;
    virtual void ice_exception(const ::Ice::Exception&) = 0;

    void __invoke(const ::peer::PeerConnectionEventHandlerPrx&, const ::std::string&, ::peer::PeerAccessLevel, const ::Ice::Context&);

protected:

    virtual void __response(bool);
};

typedef ::IceUtil::Handle< ::peer::AMI_PeerConnectionEventHandler_peerConnectedNoProxy> AMI_PeerConnectionEventHandler_peerConnectedNoProxyPtr;

class AMI_PeerConnectionEventHandler_peerDisconnected : public ::IceInternal::OutgoingAsync
{
public:

    virtual void ice_response() = 0;
    virtual void ice_exception(const ::Ice::Exception&) = 0;

    void __invoke(const ::peer::PeerConnectionEventHandlerPrx&, const ::std::string&, const ::Ice::Context&);

protected:

    virtual void __response(bool);
};

typedef ::IceUtil::Handle< ::peer::AMI_PeerConnectionEventHandler_peerDisconnected> AMI_PeerConnectionEventHandler_peerDisconnectedPtr;

}

namespace IceProxy
{

namespace peer
{

class UserConnectionEventHandler : virtual public ::IceProxy::Ice::Object
{
public:

    void forcedLogoutNotification()
    {
	forcedLogoutNotification(__defaultContext());
    }
    void forcedLogoutNotification(const ::Ice::Context&);
    void forcedLogoutNotification_async(const ::peer::AMI_UserConnectionEventHandler_forcedLogoutNotificationPtr&);
    void forcedLogoutNotification_async(const ::peer::AMI_UserConnectionEventHandler_forcedLogoutNotificationPtr&, const ::Ice::Context&);
    
    static const ::std::string& ice_staticId();

private: 

    virtual ::IceInternal::Handle< ::IceDelegateM::Ice::Object> __createDelegateM();
    virtual ::IceInternal::Handle< ::IceDelegateD::Ice::Object> __createDelegateD();
};

class PeerConnectionEventHandler : virtual public ::IceProxy::Ice::Object
{
public:

    void peerConnected(const ::std::string& peerUserId, ::peer::PeerAccessLevel accessLevel, const ::Ice::ObjectPrx& peerProxy)
    {
	peerConnected(peerUserId, accessLevel, peerProxy, __defaultContext());
    }
    void peerConnected(const ::std::string&, ::peer::PeerAccessLevel, const ::Ice::ObjectPrx&, const ::Ice::Context&);
    void peerConnected_async(const ::peer::AMI_PeerConnectionEventHandler_peerConnectedPtr&, const ::std::string&, ::peer::PeerAccessLevel, const ::Ice::ObjectPrx&);
    void peerConnected_async(const ::peer::AMI_PeerConnectionEventHandler_peerConnectedPtr&, const ::std::string&, ::peer::PeerAccessLevel, const ::Ice::ObjectPrx&, const ::Ice::Context&);

    void peerConnectedNoProxy(const ::std::string& peerUserId, ::peer::PeerAccessLevel accessLevel)
    {
	peerConnectedNoProxy(peerUserId, accessLevel, __defaultContext());
    }
    void peerConnectedNoProxy(const ::std::string&, ::peer::PeerAccessLevel, const ::Ice::Context&);
    void peerConnectedNoProxy_async(const ::peer::AMI_PeerConnectionEventHandler_peerConnectedNoProxyPtr&, const ::std::string&, ::peer::PeerAccessLevel);
    void peerConnectedNoProxy_async(const ::peer::AMI_PeerConnectionEventHandler_peerConnectedNoProxyPtr&, const ::std::string&, ::peer::PeerAccessLevel, const ::Ice::Context&);

    void peerDisconnected(const ::std::string& peerUserId)
    {
	peerDisconnected(peerUserId, __defaultContext());
    }
    void peerDisconnected(const ::std::string&, const ::Ice::Context&);
    void peerDisconnected_async(const ::peer::AMI_PeerConnectionEventHandler_peerDisconnectedPtr&, const ::std::string&);
    void peerDisconnected_async(const ::peer::AMI_PeerConnectionEventHandler_peerDisconnectedPtr&, const ::std::string&, const ::Ice::Context&);
    
    static const ::std::string& ice_staticId();

private: 

    virtual ::IceInternal::Handle< ::IceDelegateM::Ice::Object> __createDelegateM();
    virtual ::IceInternal::Handle< ::IceDelegateD::Ice::Object> __createDelegateD();
};

class ConnectionEventHandler : virtual public ::IceProxy::peer::UserConnectionEventHandler,
			       virtual public ::IceProxy::peer::PeerConnectionEventHandler
{
public:
    
    static const ::std::string& ice_staticId();

private: 

    virtual ::IceInternal::Handle< ::IceDelegateM::Ice::Object> __createDelegateM();
    virtual ::IceInternal::Handle< ::IceDelegateD::Ice::Object> __createDelegateD();
};

class PeerRegistrationHandler : virtual public ::IceProxy::Ice::Object
{
public:

    void registerCallbacks(const ::Ice::ObjectPrx& selfCallbackProxy, const ::peer::ConnectionEventHandlerPrx& connectionEventHandlerProxy)
    {
	registerCallbacks(selfCallbackProxy, connectionEventHandlerProxy, __defaultContext());
    }
    void registerCallbacks(const ::Ice::ObjectPrx&, const ::peer::ConnectionEventHandlerPrx&, const ::Ice::Context&);

    void registerProxy(const ::Ice::ObjectPrx& proxy)
    {
	registerProxy(proxy, __defaultContext());
    }
    void registerProxy(const ::Ice::ObjectPrx&, const ::Ice::Context&);

    void registerProxies(const ::peer::ObjectProxyArray& proxies)
    {
	registerProxies(proxies, __defaultContext());
    }
    void registerProxies(const ::peer::ObjectProxyArray&, const ::Ice::Context&);

    ::Ice::ObjectPrx getPeerProxy(const ::std::string& peerUserId, const ::Ice::Identity& privateProxyIdentity)
    {
	return getPeerProxy(peerUserId, privateProxyIdentity, __defaultContext());
    }
    ::Ice::ObjectPrx getPeerProxy(const ::std::string&, const ::Ice::Identity&, const ::Ice::Context&);

    ::peer::IdentityToProxyMap getPeerProxies(const ::std::string& peerUserId, const ::peer::IdentityArray& privateProxyIdentities)
    {
	return getPeerProxies(peerUserId, privateProxyIdentities, __defaultContext());
    }
    ::peer::IdentityToProxyMap getPeerProxies(const ::std::string&, const ::peer::IdentityArray&, const ::Ice::Context&);
    
    static const ::std::string& ice_staticId();

private: 

    virtual ::IceInternal::Handle< ::IceDelegateM::Ice::Object> __createDelegateM();
    virtual ::IceInternal::Handle< ::IceDelegateD::Ice::Object> __createDelegateD();
};

class UserSession : virtual public ::IceProxy::Glacier2::Session,
		    virtual public ::IceProxy::peer::PeerRegistrationHandler
{
public:

    ::peer::PeerIdentifierSet getMyAvailablePeers()
    {
	return getMyAvailablePeers(__defaultContext());
    }
    ::peer::PeerIdentifierSet getMyAvailablePeers(const ::Ice::Context&);

    ::Ice::ObjectPrx connectToPeer(const ::std::string& peerUserId)
    {
	return connectToPeer(peerUserId, __defaultContext());
    }
    ::Ice::ObjectPrx connectToPeer(const ::std::string&, const ::Ice::Context&);

    ::peer::PeerIdentifierSet getConnectedPeers()
    {
	return getConnectedPeers(__defaultContext());
    }
    ::peer::PeerIdentifierSet getConnectedPeers(const ::Ice::Context&);

    void disconnectFromPeer(const ::std::string& peerUserId)
    {
	disconnectFromPeer(peerUserId, __defaultContext());
    }
    void disconnectFromPeer(const ::std::string&, const ::Ice::Context&);

    void disconnectFromPeers()
    {
	disconnectFromPeers(__defaultContext());
    }
    void disconnectFromPeers(const ::Ice::Context&);
    
    static const ::std::string& ice_staticId();

private: 

    virtual ::IceInternal::Handle< ::IceDelegateM::Ice::Object> __createDelegateM();
    virtual ::IceInternal::Handle< ::IceDelegateD::Ice::Object> __createDelegateD();
};

}

}

namespace IceDelegate
{

namespace peer
{

class UserConnectionEventHandler : virtual public ::IceDelegate::Ice::Object
{
public:

    virtual void forcedLogoutNotification(const ::Ice::Context&) = 0;
};

class PeerConnectionEventHandler : virtual public ::IceDelegate::Ice::Object
{
public:

    virtual void peerConnected(const ::std::string&, ::peer::PeerAccessLevel, const ::Ice::ObjectPrx&, const ::Ice::Context&) = 0;

    virtual void peerConnectedNoProxy(const ::std::string&, ::peer::PeerAccessLevel, const ::Ice::Context&) = 0;

    virtual void peerDisconnected(const ::std::string&, const ::Ice::Context&) = 0;
};

class ConnectionEventHandler : virtual public ::IceDelegate::peer::UserConnectionEventHandler,
			       virtual public ::IceDelegate::peer::PeerConnectionEventHandler
{
public:
};

class PeerRegistrationHandler : virtual public ::IceDelegate::Ice::Object
{
public:

    virtual void registerCallbacks(const ::Ice::ObjectPrx&, const ::peer::ConnectionEventHandlerPrx&, const ::Ice::Context&) = 0;

    virtual void registerProxy(const ::Ice::ObjectPrx&, const ::Ice::Context&) = 0;

    virtual void registerProxies(const ::peer::ObjectProxyArray&, const ::Ice::Context&) = 0;

    virtual ::Ice::ObjectPrx getPeerProxy(const ::std::string&, const ::Ice::Identity&, const ::Ice::Context&) = 0;

    virtual ::peer::IdentityToProxyMap getPeerProxies(const ::std::string&, const ::peer::IdentityArray&, const ::Ice::Context&) = 0;
};

class UserSession : virtual public ::IceDelegate::Glacier2::Session,
		    virtual public ::IceDelegate::peer::PeerRegistrationHandler
{
public:

    virtual ::peer::PeerIdentifierSet getMyAvailablePeers(const ::Ice::Context&) = 0;

    virtual ::Ice::ObjectPrx connectToPeer(const ::std::string&, const ::Ice::Context&) = 0;

    virtual ::peer::PeerIdentifierSet getConnectedPeers(const ::Ice::Context&) = 0;

    virtual void disconnectFromPeer(const ::std::string&, const ::Ice::Context&) = 0;

    virtual void disconnectFromPeers(const ::Ice::Context&) = 0;
};

}

}

namespace IceDelegateM
{

namespace peer
{

class UserConnectionEventHandler : virtual public ::IceDelegate::peer::UserConnectionEventHandler,
				   virtual public ::IceDelegateM::Ice::Object
{
public:

    virtual void forcedLogoutNotification(const ::Ice::Context&);
};

class PeerConnectionEventHandler : virtual public ::IceDelegate::peer::PeerConnectionEventHandler,
				   virtual public ::IceDelegateM::Ice::Object
{
public:

    virtual void peerConnected(const ::std::string&, ::peer::PeerAccessLevel, const ::Ice::ObjectPrx&, const ::Ice::Context&);

    virtual void peerConnectedNoProxy(const ::std::string&, ::peer::PeerAccessLevel, const ::Ice::Context&);

    virtual void peerDisconnected(const ::std::string&, const ::Ice::Context&);
};

class ConnectionEventHandler : virtual public ::IceDelegate::peer::ConnectionEventHandler,
			       virtual public ::IceDelegateM::peer::UserConnectionEventHandler,
			       virtual public ::IceDelegateM::peer::PeerConnectionEventHandler
{
public:
};

class PeerRegistrationHandler : virtual public ::IceDelegate::peer::PeerRegistrationHandler,
				virtual public ::IceDelegateM::Ice::Object
{
public:

    virtual void registerCallbacks(const ::Ice::ObjectPrx&, const ::peer::ConnectionEventHandlerPrx&, const ::Ice::Context&);

    virtual void registerProxy(const ::Ice::ObjectPrx&, const ::Ice::Context&);

    virtual void registerProxies(const ::peer::ObjectProxyArray&, const ::Ice::Context&);

    virtual ::Ice::ObjectPrx getPeerProxy(const ::std::string&, const ::Ice::Identity&, const ::Ice::Context&);

    virtual ::peer::IdentityToProxyMap getPeerProxies(const ::std::string&, const ::peer::IdentityArray&, const ::Ice::Context&);
};

class UserSession : virtual public ::IceDelegate::peer::UserSession,
		    virtual public ::IceDelegateM::Glacier2::Session,
		    virtual public ::IceDelegateM::peer::PeerRegistrationHandler
{
public:

    virtual ::peer::PeerIdentifierSet getMyAvailablePeers(const ::Ice::Context&);

    virtual ::Ice::ObjectPrx connectToPeer(const ::std::string&, const ::Ice::Context&);

    virtual ::peer::PeerIdentifierSet getConnectedPeers(const ::Ice::Context&);

    virtual void disconnectFromPeer(const ::std::string&, const ::Ice::Context&);

    virtual void disconnectFromPeers(const ::Ice::Context&);
};

}

}

namespace IceDelegateD
{

namespace peer
{

class UserConnectionEventHandler : virtual public ::IceDelegate::peer::UserConnectionEventHandler,
				   virtual public ::IceDelegateD::Ice::Object
{
public:

    virtual void forcedLogoutNotification(const ::Ice::Context&);
};

class PeerConnectionEventHandler : virtual public ::IceDelegate::peer::PeerConnectionEventHandler,
				   virtual public ::IceDelegateD::Ice::Object
{
public:

    virtual void peerConnected(const ::std::string&, ::peer::PeerAccessLevel, const ::Ice::ObjectPrx&, const ::Ice::Context&);

    virtual void peerConnectedNoProxy(const ::std::string&, ::peer::PeerAccessLevel, const ::Ice::Context&);

    virtual void peerDisconnected(const ::std::string&, const ::Ice::Context&);
};

class ConnectionEventHandler : virtual public ::IceDelegate::peer::ConnectionEventHandler,
			       virtual public ::IceDelegateD::peer::UserConnectionEventHandler,
			       virtual public ::IceDelegateD::peer::PeerConnectionEventHandler
{
public:
};

class PeerRegistrationHandler : virtual public ::IceDelegate::peer::PeerRegistrationHandler,
				virtual public ::IceDelegateD::Ice::Object
{
public:

    virtual void registerCallbacks(const ::Ice::ObjectPrx&, const ::peer::ConnectionEventHandlerPrx&, const ::Ice::Context&);

    virtual void registerProxy(const ::Ice::ObjectPrx&, const ::Ice::Context&);

    virtual void registerProxies(const ::peer::ObjectProxyArray&, const ::Ice::Context&);

    virtual ::Ice::ObjectPrx getPeerProxy(const ::std::string&, const ::Ice::Identity&, const ::Ice::Context&);

    virtual ::peer::IdentityToProxyMap getPeerProxies(const ::std::string&, const ::peer::IdentityArray&, const ::Ice::Context&);
};

class UserSession : virtual public ::IceDelegate::peer::UserSession,
		    virtual public ::IceDelegateD::Glacier2::Session,
		    virtual public ::IceDelegateD::peer::PeerRegistrationHandler
{
public:

    virtual ::peer::PeerIdentifierSet getMyAvailablePeers(const ::Ice::Context&);

    virtual ::Ice::ObjectPrx connectToPeer(const ::std::string&, const ::Ice::Context&);

    virtual ::peer::PeerIdentifierSet getConnectedPeers(const ::Ice::Context&);

    virtual void disconnectFromPeer(const ::std::string&, const ::Ice::Context&);

    virtual void disconnectFromPeers(const ::Ice::Context&);
};

}

}

namespace peer
{

class UserConnectionEventHandler : virtual public ::Ice::Object
{
public:

    typedef UserConnectionEventHandlerPrx ProxyType;
    typedef UserConnectionEventHandlerPtr PointerType;
    
    virtual ::Ice::ObjectPtr ice_clone() const;

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();

    virtual void forcedLogoutNotification(const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___forcedLogoutNotification(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::IceInternal::DispatchStatus __dispatch(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);
    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

void __patch__UserConnectionEventHandlerPtr(void*, ::Ice::ObjectPtr&);

class PeerConnectionEventHandler : virtual public ::Ice::Object
{
public:

    typedef PeerConnectionEventHandlerPrx ProxyType;
    typedef PeerConnectionEventHandlerPtr PointerType;
    
    virtual ::Ice::ObjectPtr ice_clone() const;

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();

    virtual void peerConnected(const ::std::string&, ::peer::PeerAccessLevel, const ::Ice::ObjectPrx&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___peerConnected(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void peerConnectedNoProxy(const ::std::string&, ::peer::PeerAccessLevel, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___peerConnectedNoProxy(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void peerDisconnected(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___peerDisconnected(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::IceInternal::DispatchStatus __dispatch(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);
    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

void __patch__PeerConnectionEventHandlerPtr(void*, ::Ice::ObjectPtr&);

class ConnectionEventHandler : virtual public ::peer::UserConnectionEventHandler,
			       virtual public ::peer::PeerConnectionEventHandler
{
public:

    typedef ConnectionEventHandlerPrx ProxyType;
    typedef ConnectionEventHandlerPtr PointerType;
    
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

void __patch__ConnectionEventHandlerPtr(void*, ::Ice::ObjectPtr&);

class PeerRegistrationHandler : virtual public ::Ice::Object
{
public:

    typedef PeerRegistrationHandlerPrx ProxyType;
    typedef PeerRegistrationHandlerPtr PointerType;
    
    virtual ::Ice::ObjectPtr ice_clone() const;

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();

    virtual void registerCallbacks(const ::Ice::ObjectPrx&, const ::peer::ConnectionEventHandlerPrx&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___registerCallbacks(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void registerProxy(const ::Ice::ObjectPrx&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___registerProxy(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void registerProxies(const ::peer::ObjectProxyArray&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___registerProxies(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::Ice::ObjectPrx getPeerProxy(const ::std::string&, const ::Ice::Identity&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___getPeerProxy(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::peer::IdentityToProxyMap getPeerProxies(const ::std::string&, const ::peer::IdentityArray&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___getPeerProxies(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::IceInternal::DispatchStatus __dispatch(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);
    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

void __patch__PeerRegistrationHandlerPtr(void*, ::Ice::ObjectPtr&);

class UserSession : virtual public ::Glacier2::Session,
		    virtual public ::peer::PeerRegistrationHandler
{
public:

    typedef UserSessionPrx ProxyType;
    typedef UserSessionPtr PointerType;
    
    virtual ::Ice::ObjectPtr ice_clone() const;

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();

    virtual ::peer::PeerIdentifierSet getMyAvailablePeers(const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___getMyAvailablePeers(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::Ice::ObjectPrx connectToPeer(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___connectToPeer(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::peer::PeerIdentifierSet getConnectedPeers(const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___getConnectedPeers(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void disconnectFromPeer(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___disconnectFromPeer(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void disconnectFromPeers(const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___disconnectFromPeers(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::IceInternal::DispatchStatus __dispatch(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);
    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

void __patch__UserSessionPtr(void*, ::Ice::ObjectPtr&);

}

#endif
