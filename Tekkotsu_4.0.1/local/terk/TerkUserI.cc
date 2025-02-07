#ifdef HAVE_ICE

#include <Ice/Application.h>
#include <Glacier2/Router.h>
#include "TeRKPeerCommon.h"
#include "peer/MRPLPeer.h"
#include "TerkUserI.h"

#include <unistd.h>
#include <map>
#include <iostream>
#include <fstream>

using namespace std;
using namespace peer;
using namespace TeRK;


TerkUserI::TerkUserI():frameCount(0),ic(0){

}

void TerkUserI::forcedLogoutNotification(const Ice::Current&){
  printf("forced Logout!\n");
  exit(1);
}

void TerkUserI::peerConnected(const ::std::string& peerUserId,
			      ::peer::PeerAccessLevel accessLevel,
			      const ::Ice::ObjectPrx& peerProxy,
			      const Ice::Current& current){
  printf("Peer connected\n");
}

void TerkUserI::peerConnectedNoProxy(const ::std::string& peerUserId, 
				     ::peer::PeerAccessLevel accessLevel, 
				     const Ice::Current& current){
  printf("Peer connected without a proxy\n");
}

void TerkUserI::peerDisconnected(const ::std::string& peerUserId, const Ice::Current& current){
  printf("Peer Disconnected");
}

ProxyTypeIdToIdentityMap TerkUserI::getSupportedServices(const Ice::Current&) const{
  return ProxyTypeIdToIdentityMap();
}

void TerkUserI::newFrame(const ::TeRK::Image& img, const ::Ice::Current& current){
	if(ic)
		ic->newImage(img.data);
}


void TerkUserI::setImageCache(ImageCache* in_ic){
  this->ic = in_ic;
}

PropertyMap TerkUserI::getProperties(const Ice::Current& current){
  PropertyMap p;
  return p;
}

#endif
