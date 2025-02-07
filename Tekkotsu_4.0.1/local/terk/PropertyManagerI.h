#ifndef _PROPERTYMANAGERI_H_
#define _PROPERTYMANAGERI_H_

#include <vector>
#include <map>

#include "TeRKPeerCommon.h"

class PropertyManagerI : virtual public ::TeRK::PropertyManager
{
 public:
	PropertyManagerI():PropertyManager(), properties(){};

  std::string getProperty(const std::string &key, const Ice::Current&);
  void setProperty(const std::string &key, const std::string &value, const Ice::Current&);
  ::TeRK::StringArray getPropertyKeys(const Ice::Current&);

 private:
	std::map<std::string, std::string> properties;
};

#endif /* _PROPERTYMANAGERI_H_ */

