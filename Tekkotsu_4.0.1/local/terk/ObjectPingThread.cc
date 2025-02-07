#ifdef HAVE_ICE

#include "ObjectPingThread.h"

using namespace std;

ObjectPingThread::ObjectPingThread(const Ice::ObjectPrx& objectPrx):
  _objectPrx(objectPrx),
  _timeout(IceUtil::Time::seconds(5)),
  _destroy(false)
{

}

void ObjectPingThread::run(){
  cout << "running session run()" << endl;
  Lock sync(*this);
  while(!_destroy){
    timedWait(_timeout);
    if(_destroy){
      break;
    }
    try{
      // cout << "Pinging objectPrx" << endl;
      _objectPrx->ice_ping();
    }
    catch(const Ice::Exception&){
      break;
    }
  }
}

bool ObjectPingThread::destroyed(){
  return _destroy;
}

void ObjectPingThread::destroy(){
  Lock sync(*this);
  _destroy = true;
  notify();
}

void TeRKCallbackI::message(const string& data, const Ice::Current&){
  cout << data << endl;
}


#endif
