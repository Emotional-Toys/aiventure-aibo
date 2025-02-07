#include "Vision/colors.h"

//! displays an rgb value in the form '[r,g,b]'
std::ostream& operator<<(std::ostream &os, const rgb &rgbval) {
  os << "[" << (unsigned int)rgbval.red
     << "," << (unsigned int)rgbval.green
     << "," << (unsigned int)rgbval.blue
     << "]";
  return os;
}

//! returns @a rgbval in the form 'r g b'
std::string toString(const rgb &rgbval) {
  char buff[15];
  snprintf(buff,15,"%d %d %d",rgbval.red,rgbval.green,rgbval.blue);
  return buff;
}


