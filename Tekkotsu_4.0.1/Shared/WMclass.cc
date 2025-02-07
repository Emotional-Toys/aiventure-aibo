// Watchable Memory (WM)
//
// David S. Touretzky
// (Insert standard Tekkotsu copyright stuff here.)

#include "WMclass.h"

//================================================================
//
// toString() methods for various classes
//

#ifndef __CYGWIN__

template<>
std::string WMitem<int>::toString(void) const {
  char print_buffer[30];
  sprintf(print_buffer,"%d",get_value());
  return std::string(print_buffer);
}

template<>
std::string WMitem<float>::toString(void) const {
  char print_buffer[30];
  sprintf(print_buffer,"%g",get_value());
  return std::string(print_buffer);
}

template<>
std::string WMitem<double>::toString(void) const {
  char print_buffer[30];
  sprintf(print_buffer,"%g",get_value());
  return std::string(print_buffer);
}

template<>
std::string WMitem<bool>::toString(void) const {
  char print_buffer[30];
  sprintf(print_buffer,"%d",get_value());
  return std::string(print_buffer);
}

template<>
std::string WMitem<char>::toString(void) const {
  char print_buffer[30];
  sprintf(print_buffer,"'%c'",get_value());
  return std::string(print_buffer);
}

template<>
std::string WMitem<char*>::toString(void) const {
  char buff[1+WM_max_stringlen] = "";
  strncpy(buff,get_value(),WM_max_stringlen);
  char *p = get_value();
  char *q = p + WM_max_stringlen + 1;
  for (; p != q ; p++)
    if ( *p == 0 ) break;
  std::string result = std::string("\"") + buff + ((p==q) ? "..." : "") + "\"";
  return result;
}

template<>
std::string WMitem<std::string>::toString(void) const {
  return std::string("\"") + 
    ((get_value().length() <= WM_max_stringlen) ?
     get_value() : get_value().substr(0,WM_max_stringlen) + "...") + "\"";
}

template<>
std::string WMitem<WMregistry>::toString(void) const {
  char print_buffer[300];
  sprintf(print_buffer,"<%s, %lu entries>",
	  get_value().name.data(),(unsigned long)get_value().entries.size());
  return std::string(print_buffer);
}

#endif //CYGWIN-compiler incompatable

WMregistry GlobalWM("GlobalWM");

WMitem<WMregistry> lookup_reg(std::string const &name, WMregistry &registry) {
  for (std::vector<WMentry*>::const_iterator it = registry.entries.begin(); 
       it != registry.entries.end(); it++) {
    if ( (*it)->item_name == name )
      return *static_cast<WMitem<WMregistry> const*>((*it)->item);
  };
  return create_WMentry(name, "WMregistry", new WMregistry(name, &registry),
                        registry);
}

void WMregistry::dump(void) const {
  std::cout << std::endl << "Registry " << name << ":" << std::endl;
  for (std::vector<WMentry*>::const_iterator it = entries.begin();
       it != entries.end();
       it++)
    std::cout << "  " << (*it)->type_name << " " << (*it)->item_name
	 << " = " << (*it)->item->toString() << std::endl;
}

void dump(WMitem<WMregistry> const &wmreg) {
  wmreg.get_value().dump();
}
