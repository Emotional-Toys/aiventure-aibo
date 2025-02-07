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

2004/07/01: Etienne Lachance
   -Added doxygen documentation.

2004/07/01: Ethan Tira-Thompson
    -Added support for newmat's use_namespace #define, using ROBOOP namespace
    -Added dependance on utils.h because we need to get the use_namespace setting

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

#ifndef CONFIG_H
#define CONFIG_H

/*!
  @file config.h
  @brief Header file for Config class definitions.
*/

#ifdef _MSC_VER                  // Microsoft
#pragma warning (disable:4786)  // Disable decorated name truncation warnings 
#pragma warning (disable:4503)  // Disable decorated name truncation warnings 
#endif

#include <string>
#include <map>

#include "utils.h"
#ifdef use_namespace
namespace ROBOOP {	
  using namespace NEWMAT;
#endif
  //! @brief RCS/CVS version.
  static const char header_config_rcsid[] __UNUSED__ = "$Id: config.h,v 1.15 2007/11/11 23:57:24 ejt Exp $";

//! @brief Return when can not open file.
#define CAN_NOT_OPEN_FILE                     -1

//! @brief Return when can not create a file.
#define CAN_NOT_CREATE_FILE                   -2

//! @brief Return when a section or parameter does not exist.
#define SECTION_OR_PARAMETER_DOES_NOT_EXIST   -3


//! @brief Handle configuration files.
/*! The file syntax used is:
 *  - <tt>[</tt><i>section_name</i><tt>]</tt>
 *  - one or more entries for that section:<br>
 *    <i>field_name</i><tt>:</tt> <i>value</i>
 *
 *  White space is ignored, but a newline must follow each section or field entry
 *
 *  The minimal entries required to avoid warnings are (can be any order):
 *  - <tt>[</tt><i>chain_name</i><tt>]</tt><br>
 *    - <tt>Name: </tt> - a string identifier
 *    - <tt>DH: </tt> - whether or not the chain specification is standard Denavit-Hartenberg (non-zero), or modified Denavit-Hartenberg (0)
 *    - <tt>Fix: </tt> - ?
 *    - <tt>MinPara: </tt> - ?
 *    - <tt>dof: </tt> - number of links in the chain
 *    - <tt>Motor: </tt> - ?
 *    - <tt>Stl: </tt> - ?
 *  - For each link <i>N</i>: 1..<i>dof</i><br>
 *    <tt>[</tt><i>chain_name</i><tt>_LINK</tt><i>N</i><tt>]</tt>
 *    - <tt>joint_type: </tt> - revolute (0), prismatic (1)
 *    - <tt>theta: </tt> - DH theta parameter
 *    - <tt>d: </tt> - DH d parameter
 *    - <tt>a: </tt> - DH a parameter
 *    - <tt>alpha: </tt> - DH alpha parameter
 *    - <tt>theta_min: </tt> - minimum achievable joint position
 *    - <tt>theta_max: </tt> - maximum achievable joint position
 *    - <tt>m: </tt> - mass of link
 *    - <tt>cx: </tt> - x position of center of gravity for link
 *    - <tt>cy: </tt> - y position of center of gravity for link
 *    - <tt>cz: </tt> - z position of center of gravity for link
 *    - <tt>Ixx: </tt> - xx entry of inertia matrix
 *    - <tt>Ixy: </tt> - xy entry of inertia matrix
 *    - <tt>Ixz: </tt> - xz entry of inertia matrix
 *    - <tt>Iyy: </tt> - yy entry of inertia matrix
 *    - <tt>Iyz: </tt> - yz entry of inertia matrix
 *    - <tt>Izz: </tt> - zz entry of inertia matrix
 *
 *  Additional entries may be defined, but will be silently ignored.
 */
class Config {
public:
   Config() : conf(), filename() {}
   Config(const std::string & filename_,bool doRead=false);
   Config(const Config & x);
   Config & operator=(const Config & x);
   short read_conf();
   void print();

	 bool section_exists(const std::string& section) const;
	 bool parameter_exists(const std::string& section, const std::string& parameter) const;

   short select_string(const std::string section, const std::string parameter,
                       std::string & value) const;
   short select_bool(const std::string section, const std::string parameter,
                     bool & value) const;
   short select_short(const std::string section, const std::string parameter,
                      short & value) const;
   short select_int(const std::string section, const std::string parameter,
                    int & value) const;
   short select_float(const std::string section, const std::string parameter,
		      float & value) const;
   short select_double(const std::string section, const std::string parameter,
                       double & value) const;
   short select_real(const std::string section, const std::string parameter,
                       Real & value) const;

   short write_conf(const std::string name, const std::string file_title,
                    const int space_between_column);
   void add_string(const std::string section, const std::string parameter,
                   const std::string value);
   void add_bool(const std::string section, const std::string parameter,
                 const bool value);
   void add_int(const std::string section, const std::string parameter,
                const int value);
   void add_float(const std::string section, const std::string parameter,
                   const float value);
   void add_double(const std::string section, const std::string parameter,
                   const double value);
   void add_real(const std::string section, const std::string parameter,
                   const Real value);

private:
	//! holds configuration data - a map from section name to sub-map of parameters to values
	typedef std::map<std::string, std::map<std::string, std::string> > confdata_t;
	 
   confdata_t conf;   //!< Data store from/to configuration file.
   std::string filename;  //!< Configuration file name.
};

#ifdef use_namespace
}
#endif

#endif 


















