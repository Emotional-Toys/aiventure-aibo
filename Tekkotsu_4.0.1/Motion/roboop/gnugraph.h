/*
ROBOOP -- A robotics object oriented package in C++
Copyright (C) 1996-2004  Richard Gourdeau

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

Richard Gourdeau
Professeur Agrege
Departement de genie electrique
Ecole Polytechnique de Montreal
C.P. 6079, Succ. Centre-Ville
Montreal, Quebec, H3C 3A7

email: richard.gourdeau@polymtl.ca
-------------------------------------------------------------------------------
Revision_history:

2004/07/01: Etienne Lachance
   -Added doxygen documentation.

2004/07/01: Ethan Tira-Thompson
    -Added support for newmat's use_namespace #define, using ROBOOP namespace
-------------------------------------------------------------------------------
*/

#ifndef GNUGRAPH_H
#define GNUGRAPH_H


/*!
  @file gnugraph.h
  @brief Header file for graphics definitions.
*/

#if defined(__WIN32__) || defined(_WIN32) || defined(__NT__)  || defined(__CYGWIN__)      /* Windows 95/NT */

#define GNUPLOT "wgnuplot.exe"
#define STRICT
#include <windows.h>

#ifdef _MSC_VER 
#define snprintf	_snprintf
#endif

#else // Unix 

#define GNUPLOT "gnuplot"
#include <sys/types.h>
#include <unistd.h>
#endif

#include "robot.h"
#include <sys/stat.h>

#ifdef __WATCOMC__
#include <strstrea.h>
#else
#include <sstream>
#endif
#include <vector>

#ifdef use_namespace
namespace ROBOOP {
  using namespace NEWMAT;
#endif
	//! @brief RCS/CVS version.
	static const char header_gnugraph_rcsid[] __UNUSED__ = "$Id: gnugraph.h,v 1.3 2007/11/11 23:57:24 ejt Exp $";

#define OUT_OF_MEMORY       -1
#define X_Y_DATA_NO_MATCH   -2
#define LABELS_NBR_NO_MATCH -3

short set_plot2d(const char *title_graph, const char *x_axis_title, const char *y_axis_title,
                 const char *label, int type, const Matrix &xdata, const Matrix &ydata,
                 int start_y, int end_y);

short set_plot2d(const char *title_graph, const char *x_axis_title, const char *y_axis_title,
                 const std::vector<char *>label, int type, const Matrix &xdata, const Matrix &ydata,
                 const std::vector<int> & data_select);


//  interface class for gnuplot

#define LINES       0
#define POINTS      1
#define LINESPOINTS 2
#define IMPULSES    3
#define DOTS        4
#define STEPS       5
#define BOXES       6

#define NCURVESMAX  10  // maximum number of curves in the same Plot2d 

class Plot2d;

/*!
  @class GNUcurve
  @brief Object for one curve.
 */
class GNUcurve {
   friend class Plot2d; //!< Plot2d need access to private data
   Matrix xy;           //!< n x 2 matrix defining the curve */
   std::string clabel;       //!< string defining the curve label for the legend
   int ctype;           //!< curve type: lines, points, linespoints, impulses,
                        //!< dots, steps, boxes
public:
   GNUcurve(const Matrix & data, const std::string & label = "", int type = LINES);
   GNUcurve(void);
   GNUcurve(const GNUcurve & gnuc);
   GNUcurve & operator=(const GNUcurve & gnuc);
   void dump(void);
};


/*!
  @class Plot2d
  @brief 2d plot object.
*/
class Plot2d {
   std::string 
     title,           //!< Graph title.
     xlabel,          //!< Graph x axis.
     ylabel;          //!< Graph y axis.
   std::string gnucommand; //!< GNU plot command.
   int ncurves;       //!< Number of curves.
   GNUcurve *curves;  //!< Pointer to GNUcurve object.
public:
   Plot2d(void);
   ~Plot2d(void);
   void dump(void);
   void settitle(const std::string & t);
   void setxlabel(const std::string & t);
   void setylabel(const std::string & t);
   void addcurve(const Matrix & data, const std::string & label = "", int type = LINES);
   void gnuplot(void);
   void addcommand(const std::string & gcom);
};

#define IO_COULD_NOT_OPEN_FILE  -1
#define IO_MISMATCH_SIZE        -2
#define IO_DATA_EMPTY           -3
#define IO_MISMATCH_ELEMENT_NBR -4
#define PROBLEM_FILE_READING    -5


/*!
  @class IO_matrix_file.
  @brief Read and write data at every iterations in a file.
*/
class IO_matrix_file {
public:
   IO_matrix_file(const std::string & filename);
   short write(const std::vector<Matrix> & data);
   short write(const std::vector<Matrix> & data, const std::vector<std::string> & title);
   short read(std::vector<Matrix> & data);
   short read(std::vector<Matrix> & data, std::vector<std::string> & title);
   short read_all(std::vector<Matrix> & data, std::vector<std::string> & data_title);
private:
   int 
     position_read,       //!< Position to read the file.
     nb_iterations_write, //!< Number of iterations in writing mode.
     nb_iterations_read,  //!< Number of iterations in reading mode.
     nb_element;          //!< Number of elements to read or write.
   std::string filename;       //!< File name.
};


/*!
  @class Plot_file
  @brief Creates a graphic from a data file.
*/
class Plot_file : public IO_matrix_file, Plot2d
{
public:
   Plot_file(const std::string & filename);
   short graph(const std::string & title_graph, const std::string & label, const short x,
               const short y, const short x_start, const short y_start,
               const short y_end);
private:
   std::vector<Matrix> data_from_file;  //!< Data file.
   std::vector<std::string> data_title;      //!< Data file title.
};

#ifdef use_namespace
}
#endif

#endif

