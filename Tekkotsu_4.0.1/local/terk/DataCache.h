#ifndef DATA_CACHE
#define DATA_CACHE

#include <unistd.h>
#include <map>
#include <iostream>
#include <fstream>

#include "TeRKPeerCommon.h"
#include "Shared/get_time.h"
#include "IPC/Thread.h"

#include "local/DataSource.h"

class QwerkBot;

class DataCache:public DataSource{
 public:
  DataCache(QwerkBot* qb);
  virtual ~DataCache() {}
  virtual unsigned int nextTimestamp();
  virtual const std::string& nextName();
  virtual unsigned int getData(const char *& payload, unsigned int& size, unsigned int& timestamp, std::string& name);
  virtual void setDataSourceThread(LoadDataThread* p);

 protected:
  unsigned int number;
  QwerkBot* qb;
  std::ostringstream state;
  std::string states[2];
  int cstate;

  ThreadNS::Lock frameLock;

  virtual void doFreeze() {} //!< user hook for when #frozen is set to true
  virtual void doUnfreeze() {} //!< user hook for when #frozen is set to false

 private:
	DataCache(const DataCache&); //!< do not call
	DataCache& operator=(const DataCache&); //!< do not call
};

#endif
