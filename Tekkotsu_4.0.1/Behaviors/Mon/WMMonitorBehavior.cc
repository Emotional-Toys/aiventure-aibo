#include "WMMonitorBehavior.h"
#include "Shared/WMclass.h"

//! so wmmonitorcmd_callback knows where to send the input from the GUI
WMMonitorBehavior *wmMonitorBehavior = NULL;

void
WMMonitorBehavior::registerData(const std::string& s) {
  if (s.size()==0) return;
  unsigned int pos;

  pos=s.find(' ');
  std::string cmd=s.substr(0,pos);
  std::string var=s.substr(pos+1, s.length());

  if (cmd[0]=='w') {           // enable watch on WMitem
    WMitem_base* wmitem=find (var);
    if (wmitem!=NULL) wmitem->watch();
  } else if (cmd[0]=='s') {    // disable watch on WMitem
    WMitem_base* wmitem=find (var);
    if (wmitem!=NULL) wmitem->unwatch();
  } else if (cmd[0]=='x') {    // examing a WMitem
    WMitem_base* wmitem=find (var);
    if (wmitem!=NULL)
      report(wmitem->entry->type_name, wmitem->entry->item_name,
          wmitem->toString());
  } else if (cmd[0]=='r') {
    WMregistry* wmreg=NULL;
    if (var.length()==0)
			wmreg=&GlobalWM;
    else {
      WMitem<WMregistry>* wmitem=static_cast<WMitem<WMregistry> *> (find (var));
      if (wmitem!=NULL)
        wmreg=&wmitem->get_value();
			else
				serr->printf("WMMonitorBehavior: Could not find '%s'\n",var.c_str());
    }
    if (wmreg==NULL)
			serr->printf("WMMonitorBehavior: wmreg is NULL\n");
		else {
			//sout->printf("Reporting:\n");
      for (std::vector<WMentry*>::const_iterator it = wmreg->entries.begin(); it != wmreg->entries.end(); it++) {
        WMentry* entry=*it;
        std::string sn(entry->item_name);
        WMregistry *temp=entry->registry;
        while (temp!=&GlobalWM && temp!=NULL) {
          sn=temp->name + "." + sn;
          temp=temp->parent;
        }
				//sout->printf("Reporting %s %s %s\n",entry->type_name.c_str(),sn.c_str(),entry->item->toString().c_str());
        report(entry->type_name, sn, entry->item->toString());
      }
		}
  } else if (cmd[0]=='d') {    // set debug mode (blocking/nonblocking)
    // implement within this class
  }
}


WMitem_base*
WMMonitorBehavior::find (std::string& s) {
  WMregistry* wmreg=&GlobalWM;
  std::string::size_type pos=s.find('.');
  while (pos!=std::string::npos) {
    bool changed=false;
    std::string subreg=s.substr(0, pos);
    s=s.substr(pos+1);
    for (std::vector<WMentry*>::const_iterator it = wmreg->entries.begin(); it != wmreg->entries.end(); it++)
      if ( (*it)->item_name == subreg) {
        WMitem<WMregistry> const* wmitem=static_cast<WMitem<WMregistry> const*>((*it)->item);
        wmreg=&(wmitem->get_value());
        changed=true;
				break;
      }
    if (!changed) return NULL;

    pos=s.find('.');
  }

  for (std::vector<WMentry*>::const_iterator it = wmreg->entries.begin(); it != wmreg->entries.end(); it++)
    if ( (*it)->item_name == s)
      return (*it)->item;
  return NULL;
}

void
WMMonitorBehavior::report (const std::string& var_type,
                           const std::string& var_name,
                           const std::string& value) {
  report (var_type.c_str(), var_type.length(),
          var_name.c_str(), var_name.length(),
          value.c_str(), value.length());
}

void
WMMonitorBehavior::report (const std::string& var_type,
                           const std::string& var_name,
                           const char* value, int value_length) {
  report (var_type.c_str(), var_type.length(),
          var_name.c_str(), var_name.length(),
          value, value_length);
}

void
WMMonitorBehavior::report (const char* var_type, int var_type_length,
                           const char* var_name, int var_name_length,
                           const char* value, int value_length) {
  char *buf=(char*)cmdsock->getWriteBuffer(5*sizeof(int)+var_type_length+var_name_length+value_length);
  if (buf) {
    encodeHeader(&buf, packet_wmclass);
    encode(&buf, var_type_length);
    encode(&buf, var_type, var_type_length);
    encode(&buf, var_name_length);
    encode(&buf, var_name, var_name_length);
    encode(&buf, value_length);
    encode(&buf, value, value_length);
    cmdsock->write(5*sizeof(int)+var_type_length+var_name_length+value_length);
  } else
		serr->printf("WMMonitorBehavior: Failed to get write buffer\n");
}

int wmmonitorcmd_callback(char *buf, int bytes) {
  std::string s(buf, bytes);
  if (wmMonitorBehavior==NULL) return 0;
  static std::string incomplete;
                                                                                
  //pass a line at a time to the controller
  while(s.size()>0) {
    std::string::size_type endline=s.find('\n');
    if(endline==std::string::npos) {
      incomplete+=s;
      return 0;
    }
    incomplete+=s.substr(0,endline);
    //is now complete:
    wmMonitorBehavior->registerData(incomplete);
    incomplete.erase();
    s=s.substr(endline+1);
  }
                                                                                
  return 0;
}

/*! @file
 * @brief Defines WMMonitorBehavior, which listens to commands from the Aibo3D gui and shows current state
 * @author alokl (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.4 $
 * $State: Exp $
 * $Date: 2005/11/08 21:36:08 $
 */
