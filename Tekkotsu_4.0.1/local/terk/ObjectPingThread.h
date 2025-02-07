#ifndef OBJECT_PING_THREAD
#define OBJECT_PING_THREAD 1

#include <Ice/Application.h>
#include <Glacier2/Router.h>
#include "TeRKPeerCommon.h"
#include "peer/MRPLPeer.h"
#include "PropertyManagerI.h"

#include <unistd.h>
#include <map>
#include <iostream>
#include <fstream>

class ObjectPingThread : public IceUtil::Thread, public IceUtil::Monitor<IceUtil::Mutex>
{
public:

  ObjectPingThread(const Ice::ObjectPrx& objectPrx);
  virtual void run();
  
  void  destroy();
  bool destroyed();

 private:
  const Ice::ObjectPrx _objectPrx;
  const IceUtil::Time _timeout;
  bool _destroy;
};
typedef IceUtil::Handle<ObjectPingThread> ObjectPingThreadPtr;

class TeRKCallbackI 
{
public:
	virtual ~TeRKCallbackI() {}
	virtual void message(const std::string& data, const Ice::Current&);
};

#endif
