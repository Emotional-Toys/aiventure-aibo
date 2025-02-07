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

2003/02/03: Etienne Lachance
   -Added functions set_plot2d and classe IO_matrix_file.

2003/29/04: Etienne Lachance
   -Class definitions, functions prototype, ... now in gnugraph.h
   -Improved class IO_matrix_file.
   -Class Plot2d, GNUcurve are now using STL string instead of char*.
   -Added class Plot_graph, to create a graph from a data file.
   -Replaced all NULL by 0.
   -Use mkstemp instead of tmpnam in void Plot2d::gnuplot(void).

2003/15/08: Etienne Lachance
   -The member function IO_matrix_file::write writes data in column of each 
    variables, instead of only one. This way it is possible to load a dat file
    in Matlab.

2004/07/01: Etienne Lachance
   -Added doxygen documentation.

2004/07/01: Ethan Tira-Thompson
    -Added support for newmat's use_namespace #define, using ROBOOP namespace
-------------------------------------------------------------------------------
*/

/*!
  @file gnugraph.cpp
  @brief Graphics functions.
*/

#include "gnugraph.h"
#include <fstream>

using namespace std;

#ifdef use_namespace
namespace ROBOOP {
  using namespace NEWMAT;
#endif
	//! @brief RCS/CVS version.
	static const char rcsid[] __UNUSED__ = "$Id: gnugraph.cpp,v 1.6 2007/11/14 02:38:24 ejt Exp $";

char *curvetype[] =
   {"lines",
    "points",
    "linespoints",
    "impulses",
    "dots",
    "steps",
    "boxes"};


GNUcurve::GNUcurve(void)
//!  @brief Constructor.
{
   ctype = LINES;
   clabel = "";
}


GNUcurve::GNUcurve(const Matrix & data, const string & label, int type)
//!  @brief Constructor.
{
   xy = data;
   if(data.Ncols() > 2) 
      cerr << "GNUcurve::Warning: Only the first two columns are used in GNUcurve\n";

   ctype = type;
   clabel = label;
}


GNUcurve::GNUcurve(const GNUcurve & gnuc)
//!  @brief Copy Constructor.
{
   xy = gnuc.xy;
   ctype = gnuc.ctype;
   clabel = gnuc.clabel;
}


GNUcurve & GNUcurve::operator=(const GNUcurve & gnuc)
//!  @brief Overload = operator.
{
   xy = gnuc.xy;
   ctype = gnuc.ctype;
   clabel = gnuc.clabel;

   return *this;
}


void GNUcurve::dump(void)
//!  @brief Method to dump the content of a curve to stdout
{
   cout << "Curve label: " << clabel.c_str() << "\n";
   cout << "Curve type:  " << curvetype[ctype] << "\n";
   cout << "Curve data points: \n";
   cout << xy;
   cout << "\n\n";
}


Plot2d::Plot2d(void)
//!  @brief Constructor.
{
   ncurves = 0;
   try
   {
      curves = new GNUcurve[NCURVESMAX];
   }
   catch(bad_alloc ex)
   {
      cerr << "Plot2d::Plot2d:: new ran out of memory" << endl;
   }

}


Plot2d::~Plot2d(void)
//!  @brief  Destructor.
{
   delete [] curves;
}


void Plot2d::gnuplot(void)
//!  @brief Creates a GNUplot graphic.
{
   int i, strl;
#if defined(__BCPLUSPLUS__) || defined(_MSC_VER) || defined(__WATCOMC__)
   char filename[L_tmpnam];
#else
   char filename[] = "tmpfileXXXXXX";
#endif
   char * filedata=0;
   char * wibsl;
   char bsl = '\\';

#if defined(__BCPLUSPLUS__) || defined(_MSC_VER) || defined(__WATCOMC__)
   tmpnam(filename); /* generate a temporary file name */
#else
   mkstemp(filename);
#endif
   /* replacing \ by / */
   while((wibsl = strchr(filename,bsl)) != 0) {
      wibsl[0] = '/';
   }

   {
      ofstream fileout(filename); /* write the command file */
      fileout << gnucommand.c_str();
      fileout << "set title \"" << title << "\"\n";
      fileout << "set xlabel \"" << xlabel << "\"\n";
      fileout << "set ylabel \"" << ylabel << "\"\n";
      fileout << "plot \\\n";
      for(i = 0; i < ncurves; i++) {
         fileout << "\"" << filename << "." << i << "\" ";
         fileout << "title \"" << curves[i].clabel << "\" ";
         fileout << "with " << curvetype[curves[i].ctype] << " ";
         if( i+1 < ncurves){
            fileout << ", \\\n";
         }
      }
      fileout << "\n";
   }
   try
   {
      filedata = new char[strlen(filename)+3];
   }
   catch(bad_alloc ex)
   {
      cerr << "Plot2d::gnuplot:: new ran out of memory" << endl;
   }
   strcpy(filedata,filename);
   strcat(filedata,".");
   strl = strlen(filedata);
   for(i = 0; i < ncurves; i++) { /* write the data files */
      sprintf(&filedata[strl],"%d",i);
      ofstream fileout(filedata);
#ifndef _MSC_VER // MSVC++ chokes on the next line !
      fileout << curves[i].xy;
#else
      for(int j = 1; j <= curves[i].xy.Nrows(); j++) {
         for(int k = 1; k <= curves[i].xy.Ncols(); k++) {
            fileout << curves[i].xy(j,k) << " ";
         }
         fileout << "\n";
      }
#endif
   }
   /* all command and data files are ready for the call to gnuplot */
#if defined(__WIN32__) || defined(_WIN32) || defined(__NT__) || defined(__CYGWIN__)
   /* Windows 95/98/NT/2000 etc */
   char c[L_tmpnam+15];
   char *d;
   HWND hwndm, hwnd;
   if (WinExec(GNUPLOT, SW_SHOWNORMAL) <= 32) { /* start gnuplot */
      /* failed */
      cout << "Cannot find the gnuplot application\n";
      cout << "Press Enter to continue" << endl;
      getchar();
      remove(filename); /* clean up the files and return */
      for(i = 0; i < ncurves; i++) {
         sprintf(&filedata[strl],"%d",i);
         remove(filedata);
      }
      delete filedata;
      return;
   } else { /* succeed */
      /* get gnuplot main window handle */
      hwndm = FindWindow((LPSTR) 0, (LPSTR) "gnuplot");
   }
   hwnd= GetWindow(hwndm, GW_CHILD); /* get gnuplot command area handle */
   if(hwnd == 0) cout << "OUPS!!!\n"; /* something wrong happened */
   sprintf(c,"load \"%s\" \n",filename); /* load command for the plot */

#ifdef __GNUG__        /* Cygnus Gnu C++ for win32*/
   char ccygnus[] = "cd \"c:\"\n"; /* this string should reflect
                                      the drive used to mount / 
                                      where /tmp is located */
   d = ccygnus;
   while(*d != '\0') { /* sending the command through windows messages */
      SendMessage(hwnd,WM_CHAR,*d,1L);
      d++;
   }
#endif
   d = c;
   while(*d != '\0') { /* sending the command through windows messages */
      SendMessage(hwnd,WM_CHAR,*d,1L);
      d++;
   }
   cout << "Press Enter to continue..." << endl;
   getchar();
#else      /*  using a pipe under Unix */
   FILE *command;

   command = popen(GNUPLOT,"w");
   fprintf(command,"load \"%s\"\n",filename); fflush(command);
   fprintf(stderr,"Press Enter to continue...\n"); fflush(stderr);
   getchar();
   pclose(command);
#endif
   remove(filename);
   for(i = 0; i < ncurves; i++) {
      sprintf(&filedata[strl],"%d",i);
      remove(filedata);
   }
   delete filedata;
}


void Plot2d::addcurve(const Matrix & data, const string & label, int type)
//!  @brief Add a curve on the graphic.
{
   ncurves++;
   if(ncurves <= NCURVESMAX) {
      curves[ncurves-1] = GNUcurve(data,label,type);
   } else {
      cout << "Too many curves in your Plot2d (maximum 10)\n";
      exit(1);
   }
}

void Plot2d::addcommand(const string & gcom)
//!  @brief Add GNUplot command.
{
   gnucommand += gcom;
}

void Plot2d::settitle(const string & t)
//!  @brief Set the title.
{
   title = t;
}

void Plot2d::setxlabel(const string & t)
//!  @brief Set the x axis name.
{
   xlabel = t;
}

void Plot2d::setylabel(const string & t)
//!  @brief Set the y axis name.
{
   ylabel = t;
}

void Plot2d::dump(void)
//!  @brief Method to dump the content of Plot2d to stdout
{
   cout << "gnuplot commands:\n" << gnucommand.c_str();
   cout << "Plot title: " << title.c_str() << "\n";
   cout << "X label:    " << xlabel.c_str() << "\n";
   cout << "Y label:    " << ylabel.c_str() << "\n";
   for (int i = 0; i < ncurves; i++) {
      cout << "\nCurve #" << i << "\n";
      curves[i].dump();
   }
}


// ---------------------------------------------------------------------------------------

IO_matrix_file::IO_matrix_file(const string & filename_)
//!  @brief  Constructor.
{
   filename = filename_;
   position_read = 0;
   nb_iterations_write = 0;
   nb_iterations_read = 0;
   nb_element = 0;
}


short IO_matrix_file::write(const vector<Matrix> & data)
//!  @brief Write data on disk using a default data name..
{
   vector<string> title;
   string tmp;
   for(int i = 1; i <= (int)data.size(); i++)
   {
      tmp = "data#";  // Provide a default name
      tmp += i;
      title.push_back(tmp);
   }

   return IO_matrix_file::write(data, title);
}


short IO_matrix_file::write(const std::vector<Matrix> & data, const std::vector<std::string> & title)
/*!
  @brief Write data on disk.
  @param data: Data.
  @param title: Name of each data member (ie: speed, position, ...)
*/  
{
   /*
   If the file "filename" does not exist yet, created it. The first lines of the file
   contain the following informations (for each line):
      1) the number of iterations.
      2) second line is empty.
      3) the number(n) of matrix/iteration
      4) number of rows and number of columns of Matrix 1.
      5)  "                                         "   i.
      6)  "                                         "   n.
      7)---------------------------------   (end of header file)

   example of header file;
   1120
       
   2
   6 1 titre#1
   6 1 titre#1
   ---------------------------------
   */
   const char *ptr_filename = filename.c_str(); // transform string to *char
   if(data.size())
   {
      if(!nb_iterations_write)
      {
         struct stat buf;
         if(stat(ptr_filename, &buf) )  // File does not exist
         {
            ofstream outvecfile(ptr_filename);
            if(outvecfile)
            {
               outvecfile << "nd_iterations " << nb_iterations_write
               << "        " << endl;
               outvecfile << "nb_vector " << data.size() << endl;
               for(int i = 0; i < (int)data.size(); i++)
                  outvecfile << "nb_rows " << data[i].Nrows() << "  "
                  << "nb_cols " << data[i].Ncols() <<  "  "
                  << title[i] << endl;
               outvecfile << "---------------------------------\n";
            }
            else
            {
               cerr << "IO_matrix_file::write: can not open file " << filename.c_str() << endl;
               return IO_COULD_NOT_OPEN_FILE;
            }
         }
         else
         {
            ifstream invecfile(ptr_filename, ios::in);
            if(invecfile)
               invecfile >> nb_iterations_write;
         }
      }

      ofstream outvecfile(ptr_filename, ios::in | ios::out);
      if(outvecfile)
      {
         outvecfile.seekp(strlen("nb_iterations ")); // position at start of fileObject
         outvecfile << ++nb_iterations_write << endl;
         outvecfile.seekp(0, std::ios::end); // position at end of fileObject
         for(int i = 0; i < (int)data.size(); i++)
         {
            for(int j = 1; j <= data[i].Nrows(); j++) {
               for(int k = 1; k <= data[i].Ncols(); k++) {
                  outvecfile << data[i](j,k) << " ";
               }
            }
         }
         outvecfile << endl;
         outvecfile << endl;
      }
      else
      {
         cerr << "IO_matrix_file::write: can not open file " << filename.c_str() << endl;
         return IO_COULD_NOT_OPEN_FILE;
      }
   }
   else
   {
      cerr << "IO_matrix_file::write: vector data is empty" << endl;
      return IO_DATA_EMPTY;
   }

   return 0;
}


short IO_matrix_file::read(vector<Matrix> & data)
//! @brief Read one sequence of data per call.
{
   vector<string> data_title;
   string tmp;
   for(int i = 1; i <= (int)data.size(); i++)
   {
      tmp = "data#";  // Provide a default name
      tmp += i;
      data_title.push_back(tmp);
   }

   return IO_matrix_file::read(data, data_title);
}


short IO_matrix_file::read(std::vector<Matrix> & data, std::vector<std::string> & data_title)
//! @brief Read one sequence of data per call.
{
   /*
   If the file "filename does not exist yet, created it and fill the first line
   with the number of rows and columns for each element of "data".
   ex: 6x1;3x1;3x3;
   This line indidate that data has 3 elements Matrix. The first one has 6 rows and
   1 columns, the second one has 3 rows and 1 columns ...
   */
   static const char *ptr_filename = filename.c_str(); // transform string to *char
   ifstream invecfile(ptr_filename, ios::in);

   if(invecfile)
   {
      if(!position_read)
      {
#ifdef __WATCOMC__
         char temp[256];
#else
         string temp;
#endif
         int nbcol = 0, nbrow = 0;
         invecfile >> temp >> nb_iterations_read;
         invecfile >> temp >> nb_element;
         Matrix mat_tmp;
         data.clear();
         data_title.clear();
         for(int i = 1; i <= nb_element; i++)
         {
            data.push_back(mat_tmp);
            data_title.push_back(temp);
         }
         for(int j = 0; j < nb_element; j++)
         {
            invecfile >> temp >> nbrow;
            invecfile >> temp >> nbcol;
#ifdef __WATCOMC__
            invecfile >> temp >> data_title[j];
#else
            getline(invecfile,data_title[j]);
#endif
            if( (nbrow != data[j].Nrows()) ||
                  (nbcol != data[j].Ncols()) )
               data[j] = Matrix(nbrow, nbcol);
         }
         invecfile >> temp;  //------------------------
         position_read = invecfile.tellg();
      }

      if(position_read > 0)
      {
         invecfile.seekg(position_read); // position for reading
         for(int ii = 0; ii < (int)data.size(); ii++)
            for(int jj = 1; jj <= data[ii].Nrows(); jj++)
               for(int kk = 1; kk <= data[ii].Ncols(); kk++)
                  invecfile >> data[ii](jj,kk);

         position_read = invecfile.tellg(); // position for next reading
      }
   }
   else
   {
      cerr << "IO_matrix_file::read, can not open file" << filename.c_str() << endl;
      return IO_COULD_NOT_OPEN_FILE;
   }
   return 0;
}


short IO_matrix_file::read_all(std::vector<Matrix> & data, std::vector<std::string> & data_title)
/*!
  @brief Reads all sequences of data.
  
   If the file "filename does not exist yet, created it and fill the first line
   with the number of rows and columns for each element of "data".
   ex: 6x1;3x1;3x3;
   This line indidate that data has 3 elements Matrix. The first one has 6 rows and
   1 columns, the second one has 3 rows and 1 columns ...
*/
{
   static const char *ptr_filename = filename.c_str(); // transform string to *char
   ifstream invecfile(ptr_filename, ios::in);

   if(invecfile)
   {
#ifdef __WATCOMC__
      char temp[256];
#else
      string temp;
#endif
      int nbcol = 0, nbrow = 0;
      invecfile >> temp >> nb_iterations_read;
      invecfile >> temp >> nb_element;

      Matrix mat_tmp;
      data.clear();
      data_title.clear();
      for(int i = 1; i <= nb_element; i++)
      {
         data.push_back(mat_tmp);
         data_title.push_back(" ");
      }

      for(int j = 0; j < nb_element; j++)
      {
         invecfile >> temp >> nbrow;
         invecfile >> temp >> nbcol;
         if(nbcol >1)
            return IO_MISMATCH_SIZE;

#ifdef __WATCOMC__
         invecfile >> temp >> data_title[j];
#else
         getline(invecfile,data_title[j]);
#endif
         if( (nbrow != data[j].Nrows()) ||
               (nbcol != data[j].Ncols()) )
            data[j] = Matrix(nbrow, nbcol*nb_iterations_read);
      }
      invecfile >> temp;  //---------------------------------

      for(int k = 1; k <= nb_iterations_read; k++)
         for(int ii = 0; ii < (int)data.size(); ii++)
            for(int jj = 1; jj <= data[ii].Nrows(); jj++)
               invecfile >> data[ii](jj,k);
   }
   else
   {
      cerr << "IO_matrix_file::read_all, can not open file " << filename.c_str() << endl;
      return IO_COULD_NOT_OPEN_FILE;
   }
   return 0;
}

// ---------------------------------------------------------------------------------------

/*!
  @fn Plot_file::Plot_file(const string & filename)
  @brief Constructor.

  Reads all the data of the file filename.
*/
Plot_file::Plot_file(const string & filename) : IO_matrix_file(filename), Plot2d()
{
   //clear the buffers in case of error while reading the file
   if(read_all(data_from_file, data_title))
   {
      data_from_file.clear();
      data_title.clear();
      cerr << "Plot_file::Plot_file: problem in reading file " << filename.c_str() << "." << endl;
   }
}


short Plot_file::graph(const string & title_graph, const string & label, const short x,
                       const short y, const short x_start, const short y_start,
                       const short y_end)
//!  @brief Creates a graphic.
{
   if(data_from_file.size())
   {
      if(data_from_file[x].Ncols() != data_from_file[y].Ncols())
      {
         cerr << "Plot_file::graph: number of rows of xdata and ydata does not match" << endl;
         return X_Y_DATA_NO_MATCH;
      }

      settitle(title_graph.c_str());
      setxlabel(data_title[x]);
      setylabel(data_title[y]);

      string legend;
      for(int i = y_start; i <= y_end; i++)
      {
#ifdef __WATCOMC__
         ostrstream istr;
         {
            char temp[256];
            sprintf(temp,"%d",i-y_start+1);
            istr << temp;
         }
#else
         ostringstream istr;
         istr << label << i-y_start+1;
#endif
         legend = istr.str();

         addcurve((data_from_file[x].SubMatrix(x_start,x_start,1,data_from_file[x].Ncols())
                   & data_from_file[y].SubMatrix(i,i,1,data_from_file[y].Ncols())).t(),
                  legend, POINTS);
      }
      gnuplot();
      return 0;
   }
   else
   {
      cerr << "Plot_file::graph: data file buffer is empty." << endl;
      return PROBLEM_FILE_READING;
   }
}

// ---------------------------------------------------------------------------------------------

short set_plot2d(const char *title_graph, const char *x_axis_title, const char *y_axis_title,
                 const char *label, int type, const Matrix &xdata, const Matrix &ydata,
                 int start_y, int end_y)
{

   Plot2d plotgraph;
   char *legend=0;
   try
   {
      legend = new char[strlen(label)+1];
   }
   catch(bad_alloc ex)
   {
      cerr << "set_plot2d:: new ran out of memory" << endl;
      return OUT_OF_MEMORY;
   }

   if(xdata.Ncols() != ydata.Ncols())
   {
      cerr << "set_plot2d:: number of rows of xdata and ydata does not match" << endl;
      return X_Y_DATA_NO_MATCH;
   }

   plotgraph.settitle(title_graph);
   plotgraph.setxlabel(x_axis_title);
   plotgraph.setylabel(y_axis_title);

   for(int i = start_y; i <= end_y; i++)
   {
      snprintf(legend, sizeof(legend), "%s%d", label, i-start_y+1);
      plotgraph.addcurve((xdata & ydata.SubMatrix(i,i,1,ydata.Ncols())).t(), legend, type);
   }
   plotgraph.gnuplot();

   delete [] legend;

   return 0;
}

short set_plot2d(const char *title_graph, const char *x_axis_title, const char *y_axis_title,
                 const vector<char *> label, int type, const Matrix &xdata, const Matrix &ydata,
                 const vector<int> & data_select)
{
   Plot2d plotgraph;

   plotgraph.settitle(title_graph);
   plotgraph.setxlabel(x_axis_title);
   plotgraph.setylabel(y_axis_title);

   if(xdata.Ncols() != ydata.Ncols())
   {
      cerr << "set_plot2d:: number of rows of xdata and ydata does not match" << endl;
      return X_Y_DATA_NO_MATCH;
   }
   if(data_select.size() != label.size())
   {
      cerr << "set_plot2d:: number of labels does not match" << endl;
      return LABELS_NBR_NO_MATCH;
   }

   for(int i = 0; i < (int)data_select.size(); i++)
      plotgraph.addcurve((xdata & ydata.SubMatrix(data_select[i],data_select[i],
                          1,ydata.Ncols())).t(), label[i], type);
   plotgraph.gnuplot();

   return 0;
}

#ifdef use_namespace
}
#endif













