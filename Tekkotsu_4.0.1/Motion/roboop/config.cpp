/*
Copyright (C) 2003-2004  Etienne Lachance

This library is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as
published by the Free Software Foundation; either version 2.1 of the
License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA


Report problems and direct all questions to:

email: etienne.lachance@polytml.ca or richard.gourdeau@polymtl.ca

-------------------------------------------------------------------------------
Revision_history:

2004/06/10: Etienne Lachance
    -Added doxygen documentation.

2004/07/01: Ethan Tira-Thompson
    -Added support for newmat's use_namespace #define, using ROBOOP namespace

2004/07/13: Ethan Tira-Thompson
    -Added a select_real and add_real function for type indepence of Real
    -Added functions to test for sections and parameters existance

2004/07/23: Ethan Tira-Thompson
    -Fixed potentially uninitialized variables and some other warnings

2004/09/01: Ethan Tira-Thompson
    -Added optional parameter to constructor so you can automatically read_conf
    -select_* functions are now const

2005/04/08: Ethan Tira-Thompson
    -Switched data storage to STL map, replacing the previous O(n^2) config file
     parsing, now approx O(n lg n) (actually a little better - O((s*p) lg (s+p)))
-------------------------------------------------------------------------------
*/

/*! 
  @file config.cpp
  @brief Configuration class functions.
*/


#include "config.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#ifdef __WATCOMC__
#include <strstrea.h>
#else
#include <sstream>
#endif

using namespace std;

#ifdef use_namespace
namespace ROBOOP {
  using namespace NEWMAT;
#endif
	
//! @brief RCS/CVS version.
static const char rcsid[] __UNUSED__ = "$Id: config.cpp,v 1.14 2007/11/11 23:57:24 ejt Exp $";

Config::Config(const string & filename_, bool doRead/*=false*/)
//! @brief Constructor.
	: conf(), filename(filename_)
{
   if(doRead)
      read_conf();
}

Config::Config(const Config & x)
//! @brief Constructor.
	: conf(x.conf), filename(x.filename)
{
}

Config & Config::operator=(const Config & x)
//! @brief Overload = operator.
{
   conf = x.conf;
   filename = x.filename;
   return *this;
}

short Config::read_conf()
/*!
  @brief Read a configuration file.

  This function reads the configuration file specified in 
  the constructor parameter. The information is stored in the
  variable conf.

  A configuration file contains section(s) (between [ ]), and the 
  section(s) contains parameter(s) with there respective value(s). 
  The section and the parameter are always access via a string. Below 
  is an exemple: one section named PUMA560_mDH, and two parameters.

  [PUMA560_mDH]
  DH:         0
  dof:        6
*/
{
   const char *ptr_filename = filename.c_str(); //transform string to *char
   std::ifstream inconffile(ptr_filename, std::ios::in);

   string temp;
#ifdef __WATCOMC__
   char tempo[256], tempo2[256];
   inconffile.getline(tempo,sizeof(tempo));
   temp = tempo;
#else
   getline(inconffile, temp);
#endif
   if(inconffile)
   {
      int tmpPos;
      map<string,string>* cursection=NULL;
      string parameter, value;

      while( inconffile )
      {
	 // Is-it comment line?
         if(temp[0] != '#')
         {
	    // Is-it a section name?
            if(temp[0] == '[') // [section]
            {
	       // Search for the end of the section name and ignore the rest of the line.
 	       tmpPos = temp.find("]");
	       if (tmpPos != (int)string::npos)
		 {
		   cursection = &conf[temp.substr(1, tmpPos-1)]; // remove []
            
		   // Read the next line, it's should be a parameter.
#ifdef __WATCOMC__
		   inconffile.getline(tempo,sizeof(tempo));
		   temp = tempo;
#else
		   getline(inconffile, temp);
#endif
		   // Be sure that is not another section name.
		   while( (temp[0] != '[') &&
			  (!inconffile.eof()) )
		     {
		       if(temp[0] != '#') // ignore comments
			 {
			   if(temp.find(":") != string::npos)
			     {
					 try{
#ifdef __WATCOMC__
			       istrstream inputString(tempo);
			       inputString >> tempo2;
			       parameter = tempo2;
			       inputString >> tempo2;
			       value = tempo2;
#else
//						 while(foo) ;
//						 std::cout << "Config Da " << temp << std::endl;
			       istringstream inputString(temp);
//						 std::cout << "Config Db " << temp << std::endl;
			       inputString >> parameter >> value;
#endif
//						 std::cout << "Config Dc" << std::endl;
			       // Find ":" in parameter.
			       tmpPos = parameter.find(":");
			       if (tmpPos != (int)string::npos)
				 // remove ":" a the end of parameter
				 parameter = parameter.erase(tmpPos);
			       else
				 {
#ifdef __WATCOMC__
				   inputString >> tempo2;
				   value = tempo2;
#else
				   inputString >> value;
#endif
				 }
						 } catch(std::exception e) { std::cout << "caught " << e.what() << ' ' << parameter << ' ' << value << endl; }
					 catch(...) { std::cout << "caught " << parameter << ' ' << value << endl; }

			       // Add data to the config vector
            if(cursection==NULL)
               std::cout << "parameter without section: " << parameter << " == " << value << endl;
            else
					 (*cursection)[parameter]=value;
			     }

			   // Read the next line.
#ifdef __WATCOMC__
			   inconffile.getline(tempo,sizeof(tempo));
			   temp = tempo;
#else
			   getline(inconffile, temp);
#endif
			 }
		       else
			 {
			   // Ignore comments and read the next line.
#ifdef __WATCOMC__
			   inconffile.getline(tempo,sizeof(tempo));
			   temp = tempo;
#else
			   getline(inconffile, temp);
#endif
			 }
		     }
		 }
            }
         }
         if(temp.substr(0,1) != "[") {
#ifdef __WATCOMC__
            inconffile.getline(tempo,sizeof(tempo));
            temp = tempo;
#else
            getline(inconffile, temp);
#endif
         }
      }
   }
   else
   {
      cerr << "Config::read_conf: can not open file " << filename.c_str() << endl;
      return CAN_NOT_OPEN_FILE;
   }
   return 0;
}

void Config::print()
//! @brief Print the configuration data.
{
	for(confdata_t::const_iterator it=conf.begin(); it!=conf.end(); ++it) {
		cout << '[' << it->first << "]\n";
		for(map<string,string>::const_iterator vit=it->second.begin(); vit!=it->second.end(); ++vit)
			cout << setw(15-vit->first.size()) << vit->first+":" << " " << vit->second << '\n';
	}
	cout << flush;
}

bool Config::section_exists(const string& section) const
/*!
  @brief Test to see if a section exists
  @return true if @a section is found
*/
{
	return conf.find(section)!=conf.end();
}

bool Config::parameter_exists(const string& section, const string& parameter) const
/*!
  @brief Test to see if a parameter exists within a section
  @return true if @a parameter is found within @a section
*/
{
	confdata_t::const_iterator it=conf.find(section);
	if(it==conf.end())
		return false;
	return it->second.find(parameter)!=it->second.end();
}

short Config::select_string(const string section, const string parameter, string & value) const
/*!
  @brief Get a parameter data, of a certain section, into the string value.
  @return 0 or SECTION_OR_PARAMETER_DOES_NOT_EXIST if the data can not be found.
*/
{
	confdata_t::const_iterator it=conf.find(section);
	if(it!=conf.end()) {
		map<string,string>::const_iterator vit=it->second.find(parameter);
		if(vit!=it->second.end()) {
			value=vit->second;
			return 0;
		}
	}
   value = "";
   cerr << "Config::select_string: section " << section.c_str() << " or parameter "
   << parameter.c_str() << " does not exist." << endl;
   return SECTION_OR_PARAMETER_DOES_NOT_EXIST;
}

short Config::select_bool(const string section, const string parameter, bool & value) const
/*!
  @brief Get a parameter data, of a certain section, into the bool value.
  @return 0 or SECTION_OR_PARAMETER_DOES_NOT_EXIST if the data can not be found.
*/
{
   string value_string;
   if(!select_string(section, parameter, value_string))
   {
#ifdef __WATCOMC__
      char temp[256];
      strcpy(temp,value_string.c_str());
      istrstream istr(temp);
      value = istr;
#else
      istringstream istr(value_string);
      istr >> value;
#endif
      return 0;
   }
   cerr << "Config::select_bool: section " << section.c_str() << " or parameter "
   << parameter.c_str() << " does not exist" << endl;
   value = false;
   return SECTION_OR_PARAMETER_DOES_NOT_EXIST;
}

short Config::select_int(const string section, const string parameter, int & value) const
/*!
  @brief Get a parameter data, of a certain section, into the int value.
  @return 0 or SECTION_OR_PARAMETER_DOES_NOT_EXIST if the data can not be found.
*/
{
   string value_string;
   if(!select_string(section, parameter, value_string))
   {
#ifdef __WATCOMC__
      char temp[256];
      strcpy(temp,value_string.c_str());
      istrstream istr(temp);
      istr >> value;
#else
      istringstream istr(value_string);
      istr >> value;
#endif
      return 0;
   }
   cerr << "Config::select_int: section " << section.c_str() << " or parameter "
   << parameter.c_str() << " does not exist" << endl;
   value = 0;
   return SECTION_OR_PARAMETER_DOES_NOT_EXIST;
}

short Config::select_short(const string section, const string parameter, short & value) const
/*!
  @brief Get a parameter data, of a certain section, into the short value.
  @return 0 or SECTION_OR_PARAMETER_DOES_NOT_EXIST if the data can not be found.
*/
{
   string value_string;
   if(!select_string(section, parameter, value_string))
   {
#ifdef __WATCOMC__
      char temp[256];
      strcpy(temp,value_string.c_str());
      istrstream istr(temp);
      istr >> value;
#else
      istringstream istr(value_string);
      istr >> value;
#endif
      return 0;
   }
   cerr << "Config::select_short: section " << section.c_str() << " or parameter "
   << parameter.c_str() << " does not exist" << endl;
   value = 0;
   return SECTION_OR_PARAMETER_DOES_NOT_EXIST;
}

short Config::select_float(const string section, const string parameter, float & value) const
/*!
  @brief Get a parameter data, of a certain section, into the float value.
  @return 0 or SECTION_OR_PARAMETER_DOES_NOT_EXIST if the data can not be found.
*/
{
   string value_string;
   if(!select_string(section, parameter, value_string))
   {
#ifdef __WATCOMC__
      char temp[256];
      strcpy(temp,value_string.c_str());
      istrstream istr(temp);
      istr >> value;
#else
      istringstream istr(value_string);
      istr >> value;
#endif
      return 0;
   }
   cerr << "Config::select_float: section " << section.c_str() << " or parameter "
   << parameter.c_str() << " does not exist" << endl;
   value = 0.0;
   return SECTION_OR_PARAMETER_DOES_NOT_EXIST;
}

short Config::select_double(const string section, const string parameter, double & value) const
/*!
  @brief Get a parameter data, of a certain section, into the double value.
  @return 0 or SECTION_OR_PARAMETER_DOES_NOT_EXIST if the data can not be found.
*/
{
   string value_string;
   if(!select_string(section, parameter, value_string))
   {
#ifdef __WATCOMC__
      char temp[256];
      strcpy(temp,value_string.c_str());
      istrstream istr(temp);
      istr >> value;
#else
      istringstream istr(value_string);
      istr >> value;
#endif
      return 0;
   }
   cerr << "Config::select_double: section " << section.c_str() << " or parameter "
   << parameter.c_str() << " does not exist" << endl;
   value = 0.0;
   return SECTION_OR_PARAMETER_DOES_NOT_EXIST;
}

short Config::select_real(const string section, const string parameter, Real & value) const
/*!
  @brief Get a parameter data, of a certain section, into the Real value.
  @return 0 or SECTION_OR_PARAMETER_DOES_NOT_EXIST if the data can not be found.
*/
{
   string value_string;
   if(!select_string(section, parameter, value_string))
   {
#ifdef __WATCOMC__
      char temp[256];
      strcpy(temp,value_string.c_str());
      istrstream istr(temp);
      istr >> value;
#else
      istringstream istr(value_string);
      istr >> value;
#endif
      return 0;
   }
   cerr << "Config::select_real: section " << section.c_str() << " or parameter "
   << parameter.c_str() << " does not exist" << endl;
   value = 0.0;
   return SECTION_OR_PARAMETER_DOES_NOT_EXIST;
}

short Config::write_conf(const string name, const string file_title,
                         const int space_between_column)
/*!
  @brief Write the configuration information, contained in conf, on disk.
  @param name: Configuration file name.
  @param file_title: Title in the configuration file header.
  @param space_between_column: Number of blanks between : (of a parameter) and it's value.
*/
{
   const char *ptr_filename = name.c_str(); //transform string to *char
   std::ofstream outconffile(ptr_filename, std::ios::out);

   if(outconffile)
   {                     // file header
      outconffile << "# ---------------------------------------------------" << endl;
      outconffile << "# " << file_title << endl;
      outconffile << "# ---------------------------------------------------" << endl;
      outconffile << endl;

		for(confdata_t::const_iterator it=conf.begin(); it!=conf.end(); ++it) {
			outconffile << '[' << it->first << "]\n";
			for(map<string,string>::const_iterator vit=it->second.begin(); vit!=it->second.end(); ++vit)
				outconffile << setw(space_between_column-vit->first.size()) << vit->first + ": " << vit->second << '\n';
		}
      return 0;
   }
   else
   {
      cerr << "Config::write_conf: can not create file " << name.c_str() << endl;
      return CAN_NOT_CREATE_FILE;
   }
}

void Config::add_string(const string section, const string parameter, const string value)
/*!
  @brief Added the value(string) of the parameter in the section in the configuration data.

  The functions will added the parameter and the section if it does not already exist.
*/
{
	conf[section][parameter]=value;
}

void Config::add_bool(const string section, const string parameter,
                      const bool value)
/*!
  @brief Added the value (bool) of the parameter in the section in the configuration data.

  The functions use add_string by first converting the value (bool) in value (string) 
*/
{
   string value_tmp;
#ifdef __WATCOMC__
   ostrstream ostr;
#else
   ostringstream ostr;
#endif
   ostr << value;
   value_tmp = ostr.str();

   add_string(section, parameter, value_tmp);
}

void Config::add_int(const string section, const string parameter,
                     const int value)
/*!
  @brief Added the value (int) of the parameter in the section in the configuration data.

  The functions use add_string by first converting the value (int) in value (string) 
*/
{
   string value_tmp;
#ifdef __WATCOMC__
   ostrstream ostr;
#else
   ostringstream ostr;
#endif
   ostr << value;
   value_tmp = ostr.str();

   add_string(section, parameter, value_tmp);
}

void Config::add_float(const string section, const string parameter,
		       const float value)
/*!
  @brief Added the value (float) of the parameter in the section in the configuration data.

  The functions use add_string by first converting the value (float) in value (string) 
*/
{
   string value_tmp;
#ifdef __WATCOMC__
   ostrstream ostr;
#else
   ostringstream ostr;
#endif
#ifdef _MSC_VER  
   ostr << value; // need to be fixed !
#else
   ostr << setprecision(13) << value;
#endif
   value_tmp = ostr.str();

   add_string(section, parameter, value_tmp);
}

void Config::add_double(const string section, const string parameter,
                        const double value)
/*!
  @brief Added the value (double) of the parameter in the section in the configuration data.

  The functions use add_string by first converting the value (double) in value (string) 
*/
{
   string value_tmp;
#ifdef __WATCOMC__
   ostrstream ostr;
#else
   ostringstream ostr;
#endif
#ifdef _MSC_VER  
   ostr << value; // need to be fixed !
#else
   ostr << setprecision(13) << value;
#endif
   value_tmp = ostr.str();

   add_string(section, parameter, value_tmp);
}

void Config::add_real(const string section, const string parameter,
                        const Real value)
/*!
  @brief Added the value (Real) of the parameter in the section in the configuration data.

  The functions use add_string by first converting the value (Real) in value (string) 
*/
{
   string value_tmp;
#ifdef __WATCOMC__
   ostrstream ostr;
#else
   ostringstream ostr;
#endif
#ifdef _MSC_VER  
   ostr << value; // need to be fixed !
#else
   ostr << setprecision(13) << value;
#endif
   value_tmp = ostr.str();

   add_string(section, parameter, value_tmp);
}


#ifdef use_namespace
}
#endif
