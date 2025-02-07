#ifndef TERK_USER_I
#define TERK_USER_I

#include <Ice/Application.h>
#include <Glacier2/Router.h>
#include "TeRKPeerCommon.h"
#include "peer/MRPLPeer.h"
#include "PropertyManagerI.h"
#include "ImageCache.h"

#include <unistd.h>
#include <map>
#include <iostream>
#include <fstream>

class TerkUserI : public PropertyManagerI, public ::TeRK::TerkClient
{
public:
  int frameCount;
  TerkUserI();
  void forcedLogoutNotification(const Ice::Current&);
  void peerConnected(const ::std::string& peerUserId,
		     ::peer::PeerAccessLevel accessLevel,
		     const ::Ice::ObjectPrx& peerProxy,
		     const Ice::Current& current);
  void peerConnectedNoProxy(const ::std::string& peerUserId, 
			    ::peer::PeerAccessLevel accessLevel, 
			    const Ice::Current& current);
  void peerDisconnected(const ::std::string& peerUserId, const Ice::Current& current);
  //void bindFrameEvent(ImageCache ic);
  ::TeRK::ProxyTypeIdToIdentityMap getSupportedServices(const Ice::Current&) const;
  virtual void newFrame(const ::TeRK::Image& img, const ::Ice::Current& current);
  ::TeRK::PropertyMap getProperties(const Ice::Current&);
  virtual void setImageCache(ImageCache* ic);
 private:
  ImageCache* ic;

	TerkUserI(const TerkUserI&); //!< do not call
	TerkUserI& operator=(const TerkUserI&); //!< do not call
};



#endif
