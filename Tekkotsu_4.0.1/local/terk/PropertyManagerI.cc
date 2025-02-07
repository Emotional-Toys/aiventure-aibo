#ifdef HAVE_ICE

#include <vector>

#include "TeRKPeerCommon.h"
#include "PropertyManagerI.h"

using namespace std;
using namespace TeRK;
using namespace peer;

string PropertyManagerI::getProperty(const string &key, const Ice::Current&)
{
  return properties[key];
}
  
void PropertyManagerI::setProperty(const string &key, const string &value, const Ice::Current&)
{ 
  properties[key] = value;
}
  
StringArray PropertyManagerI::getPropertyKeys(const Ice::Current&)
{ 
  vector<string> keys;
  map<string, string>::iterator propiter;
  
  for(propiter = properties.begin(); propiter != properties.end(); propiter++) {
    keys.push_back((*propiter).first);
  }

  return keys;
}

#endif
