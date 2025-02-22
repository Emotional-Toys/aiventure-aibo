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
   -Merge classes Link and mLink into Link.
   -Added Robot_basic class, which is base class of Robot and mRobot.
   -Use try/catch statement in dynamic memory allocation.
   -Added member functions set_qp and set_qpp in Robot_basic.
   -It is now possible to specify a min and max value of joint angle in
    all the robot constructor.

2003/05/18: Etienne Lachance
   -Initialize the following vectors to zero in Ro_basic constructors: w, wp, vp, dw, dwp, dvp.
   -Added vector z0 in robot_basic.

2004/01/23: Etienne Lachance
   -Added const in non reference argument for all functions.

2004/03/01: Etienne Lachance
   -Added non member function perturb_robot.

2004/03/21: Etienne Lachance
   -Added the following member functions: set_mc, set_r, set_I.

2004/04/19: Etienne Lachane
   -Added and fixed Robot::robotType_inv_kin(). First version of this member function
    has been done by Vincent Drolet.

2004/04/26: Etienne Lachance
   -Added member functions Puma_DH, Puma_mDH, Rhino_DH, Rhino_mDH.

2004/05/21: Etienne Lachance
   -Added Doxygen comments.

2004/07/01: Ethan Tira-Thompson
    -Added support for newmat's use_namespace #define, using ROBOOP namespace

2004/07/02: Etienne Lachance
    -Modified Link constructor, Robot_basic constructor, Link::transform and
     Link::get_q functions in order to added joint_offset.

2004/07/13: Ethan Tira-Thompson
    -if joint_offset isn't defined in the config file, it is set to theta

2004/07/16: Ethan Tira-Thompson
    -Added Link::immobile flag, accessor functions, and initialization code
    -Added get_available_q* functions, modified set_q so it can take a list of 
     length `dof' or `get_available_dof()'.

2004/08/04: Ethan Tira-Thompson
    -Added check to Link::transform() so it shortcuts math if q is unchanged
    
2004/09/01: Ethan Tira-Thompson
    -Added constructor for instantiation from already-read config file.
     This saves time when having to reconstruct repeatedly with slow disks
-------------------------------------------------------------------------------
*/

/*!
  @file robot.cpp
  @brief Initialisation of differents robot class.
*/

#include "robot.h"
#include <time.h>
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
static const char rcsid[] __UNUSED__ = "$Id: robot.cpp,v 1.25 2007/11/11 23:57:24 ejt Exp $";

//! @brief Used to initialize a \f$4\times 4\f$ matrix.
Real fourbyfourident[] = {1.0,0.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,0.0,1.0};

//! @brief Used to initialize a \f$3\times 3\f$ matrix.
Real threebythreeident[] ={1.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,1.0};

/*!
  @fn Link::Link(const int jt, const Real it, const Real id,
        const Real ia, const Real ial, const Real theta_min,
        const Real theta_max, const Real it_off, const Real mass,
        const Real cmx, const Real cmy, const Real cmz,
        const Real ixx, const Real ixy, const Real ixz,
        const Real iyy, const Real iyz, const Real izz,
        const Real iIm, const Real iGr, const Real iB,
        const Real iCf, const bool dh, const bool min_inertial_para)
  @brief Constructor.
*/
Link::Link(const int jt, const Real it, const Real id, const Real ia, const Real ial,
           const Real it_min, const Real it_max, const Real it_off, const Real mass, 
	   const Real cmx, const Real cmy, const Real cmz, const Real ixx, const Real ixy, 
	   const Real ixz, const Real iyy, const Real iyz, const Real izz, const Real iIm, 
	   const Real iGr, const Real iB, const Real iCf, const bool dh, 
	   const bool min_inertial_parameters):
      R(3,3),
      qp(0),
      qpp(0),
      joint_type(jt),
      theta(it),
      d(id),
      a(ia),
      alpha(ial),
      theta_min(it_min),
      theta_max(it_max),
      joint_offset(it_off),
      DH(dh),
      min_para(min_inertial_parameters),
      r(),
      p(3),
      m(mass),
      Im(iIm),
      Gr(iGr),
      B(iB),
      Cf(iCf),
      mc(),
      I(3,3),
      immobile(false)
{
    if (joint_type == 0)
	theta += joint_offset;  
    else
	d += joint_offset;
   Real ct, st, ca, sa;
   ct = cos(theta);
   st = sin(theta);
   ca = cos(alpha);
   sa = sin(alpha);

   qp = qpp = 0.0;

   if (DH)
   {
      R(1,1) = ct;
      R(2,1) = st;
      R(3,1) = 0.0;
      R(1,2) = -ca*st;
      R(2,2) = ca*ct;
      R(3,2) = sa;
      R(1,3) = sa*st;
      R(2,3) = -sa*ct;
      R(3,3) = ca;

      p(1) = a*ct;
      p(2) = a*st;
      p(3) = d;
   }
   else     // modified DH notation
   {
      R(1,1) = ct;
      R(2,1) = st*ca;
      R(3,1) = st*sa;
      R(1,2) = -st;
      R(2,2) = ca*ct;
      R(3,2) = sa*ct;
      R(1,3) = 0;
      R(2,3) = -sa;
      R(3,3) = ca;

      p(1) = a;
      p(2) = -sa*d;
      p(3) = ca*d;
   }

   if (min_para)
   {
      mc = ColumnVector(3);
      mc(1) = cmx;
      mc(2) = cmy;  // mass * center of gravity
      mc(3) = cmz;
   }
   else
   {
      r = ColumnVector(3);
      r(1) = cmx;   // center of gravity
      r(2) = cmy;
      r(3) = cmz;
   }

   I(1,1) = ixx;
   I(1,2) = I(2,1) = ixy;
   I(1,3) = I(3,1) = ixz;
   I(2,2) = iyy;
   I(2,3) = I(3,2) = iyz;
   I(3,3) = izz;
}

Link::Link(const Link & x)
//! @brief Copy constructor.
  : 
    R(x.R),
    qp(x.qp),
    qpp(x.qpp),
    joint_type(x.joint_type),
    theta(x.theta),
    d(x.d),
    a(x.a),
    alpha(x.alpha),
    theta_min(x.theta_min),
    theta_max(x.theta_max),
    joint_offset(x.joint_offset),
    DH(x.DH),
    min_para(x.min_para),
    r(x.r),
    p(x.p),
    m(x.m),
    Im(x.Im),
    Gr(x.Gr),
    B(x.B),
    Cf(x.Cf),
    mc(x.mc),
    I(x.I),
    immobile(x.immobile)
{
}

Link & Link::operator=(const Link & x)
//! @brief Overload = operator.
{
   joint_type = x.joint_type;
   theta = x.theta;
   qp = x.qp;
   qpp = x.qpp;
   d = x.d;
   a = x.a;
   alpha = x.alpha;
   theta_min = x.theta_min;
   theta_max = x.theta_max;
   joint_offset = x.joint_offset;
   R = x.R;
   p = x.p;
   m = x.m;
   r = x.r;
   mc = x.mc;
   I = x.I;
   Im = x.Im;
   Gr = x.Gr;
   B = x.B;
   Cf = x.Cf;
   DH = x.DH;
   min_para = x.min_para;
   immobile = x.immobile;
   return *this;
}

void Link::transform(const Real q)
//! @brief Set the rotation matrix R and the vector p.
{
   if (DH)
   {
      if(joint_type == 0)
      {
         Real ct, st, ca, sa;
				 Real nt=q + joint_offset;
				 if(theta==nt)
					 return;
				 theta=nt;
         ct = cos(theta);
         st = sin(theta);
         ca = R(3,3);
         sa = R(3,2);

         R(1,1) = ct;
         R(2,1) = st;
         R(1,2) = -ca*st;
         R(2,2) = ca*ct;
         R(1,3) = sa*st;
         R(2,3) = -sa*ct;
         p(1) = a*ct;
         p(2) = a*st;
      }
      else // prismatic joint
         p(3) = d = q + joint_offset;
   }
   else  // modified DH notation
   {
      Real ca, sa;
      ca = R(3,3);
      sa = -R(2,3);
      if(joint_type == 0)
      {
         Real ct, st;
				 Real nt=q + joint_offset;
				 if(theta==nt)
					 return;
				 theta=nt;
         ct = cos(theta);
         st = sin(theta);

         R(1,1) = ct;
         R(2,1) = st*ca;
         R(3,1) = st*sa;
         R(1,2) = -st;
         R(2,2) = ca*ct;
         R(3,2) = sa*ct;
         R(1,3) = 0;
      }
      else
      {
         d = q + joint_offset;
         p(2) = -sa*d;
         p(3) = ca*d;
      }
   }
}

Real Link::get_q(void) const 
/*!
  @brief Return joint position (theta if joint type is rotoide, d otherwise).

  The joint offset is removed from the value.
*/
{
    if(joint_type == 0) 
	return theta - joint_offset;
      else 
	  return d - joint_offset;
}


void Link::set_r(const ColumnVector & r_)
//! @brief Set the center of gravity position.
{
   if(r_.Nrows() == 3)
      r = r_;
   else
      cerr << "Link::set_r: wrong size in input vector." << endl;
}

void Link::set_mc(const ColumnVector & mc_)
//! @brief Set the mass \f$\times\f$ center of gravity position.
{
   if(mc_.Nrows() == 3)
      mc = mc_;
   else
      cerr << "Link::set_mc: wrong size in input vector." << endl;
}

/*!
  @fn void Link::set_I(const Matrix & I)
  @brief Set the inertia matrix.
*/
void Link::set_I(const Matrix & I_)
{
   if( (I_.Nrows() == 3) && (I_.Ncols() == 3) )
      I = I_;
   else
      cerr << "Link::set_r: wrong size in input vector." << endl;
}

Robot_basic::Robot_basic(const Matrix & dhinit, const bool dh_parameter, 
			 const bool min_inertial_para)
/*!
  @brief Constructor.
  @param dhinit: Robot initialization matrix.
  @param dh_parameter: true if DH notation, false if modified DH notation.
  @param min_inertial_para: true inertial parameter are in minimal form.

  Allocate memory for vectors and matrix pointers. Initialize all the Links
  instance. 
*/
  : w(NULL), wp(NULL), vp(NULL), a(NULL), f(NULL), f_nv(NULL), n(NULL), n_nv(NULL),
  F(NULL), N(NULL), p(NULL), pp(NULL), dw(NULL), dwp(NULL), dvp(NULL), da(NULL),
  df(NULL), dn(NULL), dF(NULL), dN(NULL), dp(NULL), z0(3), gravity(3), R(NULL),
  links(NULL), robotType(DEFAULT), dof(0), fix(0)
{
   int ndof=0, i;

   gravity = 0.0;
   gravity(3) = 9.81;
   z0(1) = z0(2) = 0.0; z0(3) = 1.0;
   fix = 0;
   for(i = 1; i <= dhinit.Nrows(); i++)
      if(dhinit(i,1) == 2)
      {
         if (i == dhinit.Nrows())
            fix = 1;
         else
            error("Fix link can only be on the last one");
      }
      else
         ndof++;

   if(ndof < 1)
      error("Number of degree of freedom must be greater or equal to 1");

   dof = ndof;

   try
   {
      links = new Link[dof+fix];
      links = links-1;
      w    = new ColumnVector[dof+1];
      wp   = new ColumnVector[dof+1];
      vp   = new ColumnVector[dof+fix+1];
      a    = new ColumnVector[dof+1];
      f    = new ColumnVector[dof+1];
      f_nv = new ColumnVector[dof+1];
      n    = new ColumnVector[dof+1];
      n_nv = new ColumnVector[dof+1];
      F    = new ColumnVector[dof+1];
      N    = new ColumnVector[dof+1];
      p    = new ColumnVector[dof+fix+1];
      pp   = new ColumnVector[dof+fix+1];
      dw   = new ColumnVector[dof+1];
      dwp  = new ColumnVector[dof+1];
      dvp  = new ColumnVector[dof+1];
      da   = new ColumnVector[dof+1];
      df   = new ColumnVector[dof+1];
      dn   = new ColumnVector[dof+1];
      dF   = new ColumnVector[dof+1];
      dN   = new ColumnVector[dof+1];
      dp   = new ColumnVector[dof+1];
      R    = new Matrix[dof+fix+1];
   }
   catch(bad_alloc ex)
   {
      cerr << "Robot_basic constructor : new ran out of memory" << endl;
      exit(1);
   }

   for(i = 0; i <= dof; i++)
   {
      w[i] = ColumnVector(3);
      w[i] = 0.0;
      wp[i] = ColumnVector(3);
      wp[i] = 0.0;
      vp[i] = ColumnVector(3);
      dw[i] = ColumnVector(3);
      dw[i] = 0.0;
      dwp[i] = ColumnVector(3);
      dwp[i] = 0.0;
      dvp[i] = ColumnVector(3);
      dvp[i] = 0.0;
   }
   for(i = 0; i <= dof+fix; i++)
   {
      R[i] = Matrix(3,3);
      R[i] << threebythreeident;
      p[i] = ColumnVector(3);
      p[i] = 0.0;
      pp[i] = p[i];
   }

   switch (dhinit.Ncols()){
   case 5:                  // No inertia, no motor
      for(i = 1; i <= dof+fix; i++)
      {
         links[i] = Link((int) dhinit(i,1), dhinit(i,2), dhinit(i,3),
                         dhinit(i,4), dhinit(i,5), 0, 0, 0, 0, 0, 0, 0,
                         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, dh_parameter, min_inertial_para);
      }
      break;
   case 7:                  // min and max joint angle, no inertia, no motor
      for(i = 1; i <= dof+fix; i++)
      {
         links[i] = Link((int) dhinit(i,1), dhinit(i,2), dhinit(i,3),
                         dhinit(i,4), dhinit(i,5), dhinit(i,6), dhinit(i,7),
                         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
			 dh_parameter, min_inertial_para);
      }
      break;
   case 15:                // inertia, no motor
      for(i = 1; i <= dof+fix; i++)
      {
         links[i] = Link((int) dhinit(i,1), dhinit(i,2), dhinit(i,3),
                         dhinit(i,4), dhinit(i,5), 0, 0, 0, dhinit(i,6), dhinit(i,7),
                         dhinit(i,8), dhinit(i,9), dhinit(i,10), dhinit(i,11),
                         dhinit(i,12), dhinit(i,13), dhinit(i,14), dhinit(i,15),
                         0, 0, 0, 0, dh_parameter, min_inertial_para);
      }
      break;
   case 18:                // min and max joint angle, inertia, no motor
      for(i = 1; i <= dof+fix; i++)
      {
         links[i] = Link((int) dhinit(i,1), dhinit(i,2), dhinit(i,3),
                         dhinit(i,4), dhinit(i,5), dhinit(i,6), dhinit(i,7),
                         dhinit(i,8), dhinit(i,9), dhinit(i,10), dhinit(i,11),
                         dhinit(i,12), dhinit(i,13), dhinit(i,14), dhinit(i,15),
                         dhinit(i,16), dhinit(i,17), dhinit(i,18), 0, 0, 0, 0, 
			 dh_parameter, min_inertial_para);
      }
      break;
   case 20:                // inertia + motor
      for(i = 1; i <= dof+fix; i++)
      {
         links[i] = Link((int) dhinit(i,1), dhinit(i,2), dhinit(i,3),
                         dhinit(i,4), dhinit(i,5), 0, 0, dhinit(i,6), dhinit(i,7),
                         dhinit(i,8), dhinit(i,9), dhinit(i,10), dhinit(i,11),
                         dhinit(i,12), dhinit(i,13), dhinit(i,14), dhinit(i,15),
                         dhinit(i,16), dhinit(i,17), dhinit(i,18), dhinit(i,19),
                         dhinit(i,20), dh_parameter, min_inertial_para);
      }
      break;
   case 22:                // inertia + motor
      for(i = 1; i <= dof+fix; i++)
      {
         links[i] = Link((int) dhinit(i,1), dhinit(i,2), dhinit(i,3),
                         dhinit(i,4), dhinit(i,5), dhinit(i,6), dhinit(i,7),
                         dhinit(i,8), dhinit(i,9), dhinit(i,10), dhinit(i,11),
                         dhinit(i,12), dhinit(i,13), dhinit(i,14), dhinit(i,15),
                         dhinit(i,16), dhinit(i,17), dhinit(i,18), dhinit(i,19),
                         dhinit(i,20), dhinit(i,21), dhinit(i,22), 
			 dh_parameter, min_inertial_para);
      }
      break;
   default:
      error("Initialisation Matrix does not have 5, ,7, 16 18, 20 or 22 columns.");
   }

}

Robot_basic::Robot_basic(const Matrix & initrobot, const Matrix & initmotor,
                         const bool dh_parameter, const bool min_inertial_para)
/*!
  @brief Constructor.
  @param initrobot: Robot initialization matrix.
  @param initmotor: Motor initialization matrix.
  @param dh_parameter: true if DH notation, false if modified DH notation.
  @param min_inertial_para: true inertial parameter are in minimal form.

  Allocate memory for vectors and matrix pointers. Initialize all the Links
  instance.
*/
  : w(NULL), wp(NULL), vp(NULL), a(NULL), f(NULL), f_nv(NULL), n(NULL), n_nv(NULL),
  F(), N(NULL), p(NULL), pp(NULL), dw(NULL), dwp(NULL), dvp(NULL), da(NULL),
  df(NULL), dn(NULL), dF(NULL), dN(NULL), dp(NULL), z0(3), gravity(3), R(NULL),
  links(NULL), robotType(DEFAULT), dof(0), fix(0)
{
   int ndof=0, i;

   gravity = 0.0;
   gravity(3) = 9.81;
   z0(1) = z0(2) = 0.0; z0(3) = 1.0;

   for(i = 1; i <= initrobot.Nrows(); i++)
      if(initrobot(i,1) == 2)
      {
         if (i == initrobot.Nrows())
            fix = 1;
         else
            error("Fix link can only be on the last one");
      }
      else
         ndof++;

   if(ndof < 1)
      error("Number of degree of freedom must be greater or equal to 1");
   dof = ndof;

   try
   {
      links = new Link[dof+fix];
      links = links-1;
      w    = new ColumnVector[dof+1];
      wp   = new ColumnVector[dof+1];
      vp   = new ColumnVector[dof+fix+1];
      a    = new ColumnVector[dof+1];
      f    = new ColumnVector[dof+1];
      f_nv = new ColumnVector[dof+1];
      n    = new ColumnVector[dof+1];
      n_nv = new ColumnVector[dof+1];
      F    = new ColumnVector[dof+1];
      N    = new ColumnVector[dof+1];
      p    = new ColumnVector[dof+fix+1];
      pp   = new ColumnVector[dof+fix+1];
      dw   = new ColumnVector[dof+1];
      dwp  = new ColumnVector[dof+1];
      dvp  = new ColumnVector[dof+1];
      da   = new ColumnVector[dof+1];
      df   = new ColumnVector[dof+1];
      dn   = new ColumnVector[dof+1];
      dF   = new ColumnVector[dof+1];
      dN   = new ColumnVector[dof+1];
      dp   = new ColumnVector[dof+1];
      R    = new Matrix[dof+fix+1];
   }
   catch(bad_alloc ex)
   {
      cerr << "Robot_basic constructor : new ran out of memory" << endl;
      exit(1);
   }

   for(i = 0; i <= dof; i++)
   {
      w[i] = ColumnVector(3);
      w[i] = 0.0;
      wp[i] = ColumnVector(3);
      wp[i] = 0.0;
      vp[i] = ColumnVector(3);
      dw[i] = ColumnVector(3);
      dw[i] = 0.0;
      dwp[i] = ColumnVector(3);
      dwp[i] = 0.0;
      dvp[i] = ColumnVector(3);
      dvp[i] = 0.0;
   }
   for(i = 0; i <= dof+fix; i++)
   {
      R[i] = Matrix(3,3);
      R[i] << threebythreeident;
      p[i] = ColumnVector(3);
      p[i] = 0.0;
      pp[i] = p[i];
   }

   if ( initrobot.Nrows() == initmotor.Nrows() )
   {
      if(initmotor.Ncols() == 4)
      {
         switch(initrobot.Ncols()){
         case 15:   // inertia + motor
            for(i = 1; i <= dof+fix; i++)
            {
               links[i] = Link((int) initrobot(i,1), initrobot(i,2), initrobot(i,3),
                               initrobot(i,4), initrobot(i,5), 0, 0, 0, initrobot(i,6),
                               initrobot(i,7), initrobot(i,8),  initrobot(i,9),
                               initrobot(i,10),initrobot(i,11), initrobot(i,12),
                               initrobot(i,13),initrobot(i,14), initrobot(i,15), 
			       initmotor(i,1), initmotor(i,2),  initmotor(i,3), 
			       initmotor(i,4), dh_parameter, min_inertial_para);
            }
            break;
         case 18:    // min and max joint angle, inertia,  motor
            for(i = 1; i <= dof+fix; i++)
            {
               links[i] = Link((int) initrobot(i,1), initrobot(i,2), initrobot(i,3),
                               initrobot(i,4), initrobot(i,5),  initrobot(i,6),
                               initrobot(i,7), initrobot(i,8),  initrobot(i,9),
                               initrobot(i,10),initrobot(i,11), initrobot(i,12),
                               initrobot(i,13),initrobot(i,14), initrobot(i,15),
                               initrobot(i,16),initrobot(i,17), initrobot(i,18),
			       initmotor(i,1), initmotor(i,2), initmotor(i,3),  
			       initmotor(i,4), dh_parameter, min_inertial_para);
            }
            break;
         default:
            error("Initialisation robot Matrix does not have 16 or 18 columns.");
         }
      }
      else
         error("Initialisation robot motor Matrix does not have 4 columns.");

   }
   else
      error("Initialisation robot and motor matrix does not have same numbers of Rows.");
}

Robot_basic::Robot_basic(const int ndof, const bool /*dh_parameter*/, 
                         const bool /*min_inertial_para*/)
/*!
  @brief Constructor.
  @param ndof: Robot degree of freedom.
  @param dh_parameter: true if DH notation, false if modified DH notation.
  @param min_inertial_para: true inertial parameter are in minimal form.

  Allocate memory for vectors and matrix pointers. Initialize all the Links
  instance.
*/
  : w(NULL), wp(NULL), vp(NULL), a(NULL), f(NULL), f_nv(NULL), n(NULL), n_nv(NULL),
  F(NULL), N(NULL), p(NULL), pp(NULL), dw(NULL), dwp(NULL), dvp(NULL), da(NULL),
  df(NULL), dn(NULL), dF(NULL), dN(NULL), dp(NULL), z0(3), gravity(3), R(NULL),
  links(NULL), robotType(DEFAULT), dof(ndof), fix(0)
{
   int i = 0;
   gravity = 0.0;
   gravity(3) = 9.81;
   z0(1) = z0(2) = 0.0; z0(3) = 1.0;

   try
   {
      links = new Link[dof];
      links = links-1;
      w    = new ColumnVector[dof+1];
      wp   = new ColumnVector[dof+1];
      vp   = new ColumnVector[dof+1];
      a    = new ColumnVector[dof+1];
      f    = new ColumnVector[dof+1];
      f_nv = new ColumnVector[dof+1];
      n    = new ColumnVector[dof+1];
      n_nv = new ColumnVector[dof+1];
      F    = new ColumnVector[dof+1];
      N    = new ColumnVector[dof+1];
      p    = new ColumnVector[dof+1];
      pp   = new ColumnVector[dof+fix+1];
      dw   = new ColumnVector[dof+1];
      dwp  = new ColumnVector[dof+1];
      dvp  = new ColumnVector[dof+1];
      da   = new ColumnVector[dof+1];
      df   = new ColumnVector[dof+1];
      dn   = new ColumnVector[dof+1];
      dF   = new ColumnVector[dof+1];
      dN   = new ColumnVector[dof+1];
      dp   = new ColumnVector[dof+1];
      R    = new Matrix[dof+1];
   }
   catch(bad_alloc ex)
   {
      cerr << "Robot_basic constructor : new ran out of memory" << endl;
      exit(1);
   }

   for(i = 0; i <= dof; i++)
   {
      w[i] = ColumnVector(3);
      w[i] = 0.0;
      wp[i] = ColumnVector(3);
      wp[i] = 0.0;
      vp[i] = ColumnVector(3);
      dw[i] = ColumnVector(3);
      dw[i] = 0.0;
      dwp[i] = ColumnVector(3);
      dwp[i] = 0.0;
      dvp[i] = ColumnVector(3);
      dvp[i] = 0.0;
   }
   for(i = 0; i <= dof+fix; i++)
   {
      R[i] = Matrix(3,3);
      R[i] << threebythreeident;
      p[i] = ColumnVector(3);
      p[i] = 0.0;
      pp[i] = p[i];
   }
}

Robot_basic::Robot_basic(const Robot_basic & x)
//! @brief Copy constructor.
  : w(NULL), wp(NULL), vp(NULL), a(NULL), f(NULL), f_nv(NULL), n(NULL), n_nv(NULL),
  F(NULL), N(NULL), p(NULL), pp(NULL), dw(NULL), dwp(NULL), dvp(NULL), da(NULL),
  df(NULL), dn(NULL), dF(NULL), dN(NULL), dp(NULL), z0(x.z0), gravity(x.gravity), R(NULL),
  links(NULL), robotType(x.robotType), dof(x.dof), fix(x.fix)
{
   int i = 0;

   try
   {
      links = new Link[dof+fix];
      links = links-1;
      w    = new ColumnVector[dof+1];
      wp   = new ColumnVector[dof+1];
      vp   = new ColumnVector[dof+1];
      a    = new ColumnVector[dof+1];
      f    = new ColumnVector[dof+1];
      f_nv = new ColumnVector[dof+1];
      n    = new ColumnVector[dof+1];
      n_nv = new ColumnVector[dof+1];
      F    = new ColumnVector[dof+1];
      N    = new ColumnVector[dof+1];
      p    = new ColumnVector[dof+fix+1];
      pp   = new ColumnVector[dof+fix+1];
      dw   = new ColumnVector[dof+1];
      dwp  = new ColumnVector[dof+1];
      dvp  = new ColumnVector[dof+1];
      da   = new ColumnVector[dof+1];
      df   = new ColumnVector[dof+1];
      dn   = new ColumnVector[dof+1];
      dF   = new ColumnVector[dof+1];
      dN   = new ColumnVector[dof+1];
      dp   = new ColumnVector[dof+1];
      R    = new ColumnVector[dof+fix+1];
   }
   catch(bad_alloc ex)
   {
      cerr << "Robot_basic constructor : new ran out of memory" << endl;
      exit(1);
   }

   for(i = 0; i <= dof; i++)
   {
      w[i] = ColumnVector(3);
      w[i] = 0.0;
      wp[i] = ColumnVector(3);
      wp[i] = 0.0;
      vp[i] = ColumnVector(3);
      dw[i] = ColumnVector(3);
      dw[i] = 0.0;
      dwp[i] = ColumnVector(3);
      dwp[i] = 0.0;
      dvp[i] = ColumnVector(3);
      dvp[i] = 0.0;
   }
   for(i = 0; i <= dof+fix; i++)
   {
      R[i] = Matrix(3,3);
      R[i] << threebythreeident;
      p[i] = ColumnVector(3);
      p[i] = 0.0;
      pp[i] = p[i];
   }

   for(i = 1; i <= dof+fix; i++)
      links[i] = x.links[i];
}

Robot_basic::Robot_basic(const Config& robData, const string & robotName,
                         const bool dh_parameter, const bool min_inertial_para)
/*!
  @brief Constructor.
  @param robData: Robot configuration file.
  @param robotName: Basic section name of the configuration file.
  @param dh_parameter: DH notation (True) or modified DH notation.
  @param min_inertial_para: Minimum inertial parameters (True).

  Initialize a new robot from a configuration file.
*/
  : w(NULL), wp(NULL), vp(NULL), a(NULL), f(NULL), f_nv(NULL), n(NULL), n_nv(NULL),
  F(NULL), N(NULL), p(NULL), pp(NULL), dw(NULL), dwp(NULL), dvp(NULL), da(NULL),
  df(NULL), dn(NULL), dF(NULL), dN(NULL), dp(NULL), z0(3), gravity(3), R(NULL),
  links(NULL), robotType(DEFAULT), dof(0), fix(0)
{
   int i = 0;
   gravity = 0.0;
   gravity(3) = 9.81;
   z0(1) = z0(2) = 0.0; z0(3) = 1.0;
   
   bool motor;

   if(!robData.select_int(robotName, "dof", dof))
   {
      if(dof < 0)
         error("Robot_basic::Robot_basic: dof is less then zero.");
   }
   else
      error("Robot_basic::Robot_basic: error in extracting dof from conf file.");

   if(robData.select_int(robotName, "Fix", fix))
      fix = 0;
   if(robData.select_bool(robotName, "Motor", motor))
      motor = false;

   try
   {
      links = new Link[dof+fix];
      links = links-1;
      w    = new ColumnVector[dof+1];
      wp   = new ColumnVector[dof+1];
      vp   = new ColumnVector[dof+fix+1];
      a    = new ColumnVector[dof+1];
      f    = new ColumnVector[dof+1];
      f_nv = new ColumnVector[dof+1];
      n    = new ColumnVector[dof+1];
      n_nv = new ColumnVector[dof+1];
      F    = new ColumnVector[dof+1];
      N    = new ColumnVector[dof+1];
      p    = new ColumnVector[dof+fix+1];
      pp   = new ColumnVector[dof+fix+1];
      dw   = new ColumnVector[dof+1];
      dwp  = new ColumnVector[dof+1];
      dvp  = new ColumnVector[dof+1];
      da   = new ColumnVector[dof+1];
      df   = new ColumnVector[dof+1];
      dn   = new ColumnVector[dof+1];
      dF   = new ColumnVector[dof+1];
      dN   = new ColumnVector[dof+1];
      dp   = new ColumnVector[dof+1];
      R    = new Matrix[dof+fix+1];
   }
   catch(bad_alloc ex)
   {
      cerr << "Robot_basic constructor : new ran out of memory" << endl;
      exit(1);
   }

   for(i = 0; i <= dof; i++)
   {
      w[i] = ColumnVector(3);
      w[i] = 0.0;
      wp[i] = ColumnVector(3);
      wp[i] = 0.0;
      vp[i] = ColumnVector(3);
      dw[i] = ColumnVector(3);
      dw[i] = 0.0;
      dwp[i] = ColumnVector(3);
      dwp[i] = 0.0;
      dvp[i] = ColumnVector(3);
      dvp[i] = 0.0;
   }
   for(i = 0; i <= dof+fix; i++)
   {
      R[i] = Matrix(3,3);
      R[i] << threebythreeident;
      p[i] = ColumnVector(3);
      p[i] = 0.0;
      pp[i] = p[i];
   }

   for(int j = 1; j <= dof+fix; j++)
   {
      int joint_type =0;
      double theta=0, d=0, a=0, alpha=0, theta_min=0, theta_max=0, joint_offset=0,
         m=0, cx=0, cy=0, cz=0, Ixx=0, Ixy=0, Ixz=0, Iyy=0, Iyz=0, Izz=0,
         Im=0, Gr=0, B=0, Cf=0;
      bool immobile=false;

      string robotName_link;
#ifdef __WATCOMC__
      ostrstream ostr;
      {
         char temp[256];
         sprintf(temp,"_LINK%d",j);
         robotName_link = robotName;
         robotName_link.append(temp);
      }
#else
      ostringstream ostr;
      ostr << robotName << "_LINK" << j;
      robotName_link = ostr.str();
#endif

      robData.select_int(robotName_link, "joint_type", joint_type);
      robData.select_double(robotName_link, "theta", theta);
      robData.select_double(robotName_link, "d", d);
      robData.select_double(robotName_link, "a", a);
      robData.select_double(robotName_link, "alpha", alpha);
      robData.select_double(robotName_link, "theta_max", theta_max);
      robData.select_double(robotName_link, "theta_min", theta_min);
			if(robData.parameter_exists(robotName_link, "joint_offset"))
				robData.select_double(robotName_link, "joint_offset", joint_offset);
			else if(joint_type==0) {
				joint_offset=theta;
				theta=0;
			} else {
				joint_offset=d;
				d=0;
			}
      robData.select_double(robotName_link, "m", m);
      robData.select_double(robotName_link, "cx", cx);
      robData.select_double(robotName_link, "cy", cy);
      robData.select_double(robotName_link, "cz", cz);
      robData.select_double(robotName_link, "Ixx", Ixx);
      robData.select_double(robotName_link, "Ixy", Ixy);
      robData.select_double(robotName_link, "Ixz", Ixz);
      robData.select_double(robotName_link, "Iyy", Iyy);
      robData.select_double(robotName_link, "Iyz", Iyz);
      robData.select_double(robotName_link, "Izz", Izz);
      if(robData.parameter_exists(robotName_link,"immobile"))
        robData.select_bool(robotName_link,"immobile", immobile);

      if(motor)
      {
         robData.select_double(robotName_link, "Im", Im);
         robData.select_double(robotName_link, "Gr", Gr);
         robData.select_double(robotName_link, "B", B);
         robData.select_double(robotName_link, "Cf", Cf);
      }

      links[j] = Link(joint_type, theta, d, a, alpha, theta_min, theta_max,
                      joint_offset, m, cx, cy, cz, Ixx, Ixy, Ixz, Iyy, Iyz, 
		      Izz, Im, Gr, B, Cf, dh_parameter, min_inertial_para);
      links[j].set_immobile(immobile);
   }

   if(fix)
      links[dof+fix] = Link(2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                            0, 0, 0, 0, 0, 0, 0, 0, 0, dh_parameter, min_inertial_para);
}

Robot_basic::~Robot_basic() 
/*!
  @brief Destructor.
  
  Free all vectors and matrix memory.
*/
{
   links = links+1;
   delete []links;
   delete []R;
   delete []dp;
   delete []dN;
   delete []dF;
   delete []dn;
   delete []df;
   delete []da;
   delete []dvp;
   delete []dwp;
   delete []dw;
   delete []pp;
   delete []p;
   delete []N;
   delete []F;
   delete []n_nv;
   delete []n;
   delete []f_nv;
   delete []f;
   delete []a;
   delete []vp;
   delete []wp;
   delete []w;
}

Robot_basic & Robot_basic::operator=(const Robot_basic & x)
//! @brief Overload = operator.
{
   int i = 0;
   if ( (dof != x.dof) || (fix != x.fix) )
   {
      links = links+1;
      delete []links;
      delete []R;
      delete []dp;
      delete []dN;
      delete []dF;
      delete []dn;
      delete []df;
      delete []da;
      delete []dvp;
      delete []dwp;
      delete []dw;
      delete []pp;
      delete []p;
      delete []N;
      delete []F;
      delete []n_nv;
      delete []n;
      delete []f_nv;
      delete []f;
      delete []a;
      delete []vp;
      delete []wp;
      delete []w;
      dof = x.dof;
      fix = x.fix;
      gravity = x.gravity;
      z0 = x.z0;

      try
      {
         links = new Link[dof+fix];
         links = links-1;
         w     = new ColumnVector[dof+1];
         wp    = new ColumnVector[dof+1];
         vp    = new ColumnVector[dof+fix+1];
         a     = new ColumnVector[dof+1];
         f     = new ColumnVector[dof+1];
         f_nv  = new ColumnVector[dof+1];
         n     = new ColumnVector[dof+1];
         n_nv  = new ColumnVector[dof+1];
         F     = new ColumnVector[dof+1];
         N     = new ColumnVector[dof+1];
         p     = new ColumnVector[dof+fix+1];
         pp   = new ColumnVector[dof+fix+1];
         dw    = new ColumnVector[dof+1];
         dwp   = new ColumnVector[dof+1];
         dvp   = new ColumnVector[dof+1];
         da    = new ColumnVector[dof+1];
         df    = new ColumnVector[dof+1];
         dn    = new ColumnVector[dof+1];
         dF    = new ColumnVector[dof+1];
         dN    = new ColumnVector[dof+1];
         dp    = new ColumnVector[dof+1];
         R     = new Matrix[dof+fix+1];
      }
      catch(bad_alloc ex)
      {
         cerr << "Robot_basic::operator= : new ran out of memory" << endl;
         exit(1);
      }
   }
   for(i = 0; i <= dof; i++)
   {
      w[i] = ColumnVector(3);
      w[i] = 0.0;
      wp[i] = ColumnVector(3);
      wp[i] = 0.0;
      vp[i] = ColumnVector(3);
      dw[i] = ColumnVector(3);
      dw[i] = 0.0;
      dwp[i] = ColumnVector(3);
      dwp[i] = 0.0;
      dvp[i] = ColumnVector(3);
      dvp[i] = 0.0;
   }
   for(i = 0; i <= dof+fix; i++)
   {
      R[i] = Matrix(3,3);
      R[i] << threebythreeident;
      p[i] = ColumnVector(3);
      p[i] = 0.0;
      pp[i] = p[i];
   }
   for(i = 1; i <= dof+fix; i++)
      links[i] = x.links[i];
   
   robotType = x.robotType;

   return *this;
}

void Robot_basic::error(const string & msg1) const
//! @brief Print the message msg1 on the console.
{
   cerr << endl << "Robot error: " << msg1.c_str() << endl;
   //   exit(1);
}

int Robot_basic::get_available_dof(const int endlink)const
//! @brief Counts number of currently non-immobile links up to and including @a endlink
{
  int ans=0;
  for(int i=1; i<=endlink; i++)
    if(!links[i].immobile)
      ans++;
  return ans;
}

ReturnMatrix Robot_basic::get_q(void)const
//! @brief Return the joint position vector.
{
   ColumnVector q(dof);

   for(int i = 1; i <= dof; i++)
      q(i) = links[i].get_q();
   q.Release(); return q;
}

ReturnMatrix Robot_basic::get_qp(void)const
//! @brief Return the joint velocity vector.
{
   ColumnVector qp(dof);

   for(int i = 1; i <= dof; i++)
      qp(i) = links[i].qp;
   qp.Release(); return qp;
}

ReturnMatrix Robot_basic::get_qpp(void)const
//! @brief Return the joint acceleration vector.
{
   ColumnVector qpp(dof);

   for(int i = 1; i <= dof; i++)
      qpp(i) = links[i].qpp;
   qpp.Release(); return qpp;
}

ReturnMatrix Robot_basic::get_available_q(const int endlink)const
//! @brief Return the joint position vector of available (non-immobile) joints up to and including @a endlink.
{
   ColumnVector q(get_available_dof(endlink));

   int j=1;
   for(int i = 1; i <= endlink; i++)
      if(!links[i].immobile)
         q(j++) = links[i].get_q();
   q.Release(); return q;
}

ReturnMatrix Robot_basic::get_available_qp(const int endlink)const
//! @brief Return the joint velocity vector of available (non-immobile) joints up to and including @a endlink.
{
   ColumnVector qp(get_available_dof(endlink));

   int j=1;
   for(int i = 1; i <= endlink; i++)
      if(!links[i].immobile)
         qp(j++) = links[i].qp;
   qp.Release(); return qp;
}

ReturnMatrix Robot_basic::get_available_qpp(const int endlink)const
//! @brief Return the joint acceleration vector of available (non-immobile) joints up to and including @a endlink.
{
   ColumnVector qpp(get_available_dof(endlink));

   int j=1;
   for(int i = 1; i <= endlink; i++)
      if(!links[i].immobile)
         qpp(j) = links[i].qpp;
   qpp.Release(); return qpp;
}

void Robot_basic::set_q(const Matrix & q)
/*!
  @brief Set the joint position vector.

  Set the joint position vector and recalculate the 
  orientation matrix R and the position vector p (see 
  Link class). Print an error if the size of q is incorrect.
*/
{
   int adof=get_available_dof();
   if(q.Nrows() == dof && q.Ncols() == 1) {
      for(int i = 1; i <= dof; i++)
      {
         links[i].transform(q(i,1));
         if(links[1].DH)
         {
            p[i](1) = links[i].get_a();
            p[i](2) = links[i].get_d() * links[i].R(3,2);
            p[i](3) = links[i].get_d() * links[i].R(3,3);
         }
         else
            p[i] = links[i].p;
      }
   } else if(q.Nrows() == 1 && q.Ncols() == dof) {
      for(int i = 1; i <= dof; i++)
      {
         links[i].transform(q(1,i));
         if(links[1].DH)
         {
            p[i](1) = links[i].get_a();
            p[i](2) = links[i].get_d() * links[i].R(3,2);
            p[i](3) = links[i].get_d() * links[i].R(3,3);
         }
         else
            p[i] = links[i].p;
      }
   } else if(q.Nrows() == adof && q.Ncols() == 1) {
      int j=1;
      for(int i = 1; i <= dof; i++)
         if(!links[i].immobile) {
            links[i].transform(q(j++,1));
            if(links[1].DH)
            {
               p[i](1) = links[i].get_a();
               p[i](2) = links[i].get_d() * links[i].R(3,2);
               p[i](3) = links[i].get_d() * links[i].R(3,3);
            }
            else
               p[i] = links[i].p;
         }
   } else if(q.Nrows() == 1 && q.Ncols() == adof) {
      int j=1;
      for(int i = 1; i <= dof; i++)
         if(!links[i].immobile) {
            links[i].transform(q(1,j++));
            if(links[1].DH)
            {
               p[i](1) = links[i].get_a();
               p[i](2) = links[i].get_d() * links[i].R(3,2);
               p[i](3) = links[i].get_d() * links[i].R(3,3);
            }
            else
               p[i] = links[i].p;
         }
   } else error("q has the wrong dimension in set_q()");
}

void Robot_basic::set_q(const ColumnVector & q)
/*!
  @brief Set the joint position vector.

  Set the joint position vector and recalculate the 
  orientation matrix R and the position vector p (see 
  Link class). Print an error if the size of q is incorrect.
*/
{
   if(q.Nrows() == dof) {
      for(int i = 1; i <= dof; i++)
      {
         links[i].transform(q(i));
         if(links[1].DH)
         {
            p[i](1) = links[i].get_a();
            p[i](2) = links[i].get_d() * links[i].R(3,2);
            p[i](3) = links[i].get_d() * links[i].R(3,3);
         }
         else
            p[i] = links[i].p;
      }
   } else if(q.Nrows() == get_available_dof()) {
      int j=1;
      for(int i = 1; i <= dof; i++)
         if(!links[i].immobile) {
            links[i].transform(q(j++));
            if(links[1].DH)
            {
               p[i](1) = links[i].get_a();
               p[i](2) = links[i].get_d() * links[i].R(3,2);
               p[i](3) = links[i].get_d() * links[i].R(3,3);
            }
            else
               p[i] = links[i].p;
         }
   } else error("q has the wrong dimension in set_q()");
}

void Robot_basic::set_qp(const ColumnVector & qp)
//! @brief Set the joint velocity vector.
{
   if(qp.Nrows() == dof)
      for(int i = 1; i <= dof; i++)
         links[i].qp = qp(i);
   else if(qp.Nrows() == get_available_dof()) {
      int j=1;
      for(int i = 1; i <= dof; i++)
         if(!links[i].immobile)
            links[i].qp = qp(j++);
   } else
      error("qp has the wrong dimension in set_qp()");
}

void Robot_basic::set_qpp(const ColumnVector & qpp)
//! @brief Set the joint acceleration vector.
{
   if(qpp.Nrows() == dof)
      for(int i = 1; i <= dof; i++)
         links[i].qpp = qpp(i);
   else
      error("qpp has the wrong dimension in set_qpp()");
}

/*!
  @fn Robot::Robot(const int ndof)
  @brief Constructor
*/
Robot::Robot(const int ndof): Robot_basic(ndof, true, false)
{
    robotType_inv_kin();
}

/*!
  @fn Robot::Robot(const Matrix & dhinit)
  @brief Constructor
*/
Robot::Robot(const Matrix & dhinit): Robot_basic(dhinit, true, false)
{
    robotType_inv_kin();
}

/*!
  @fn Robot::Robot(const Matrix & initrobot, const Matrix & initmotor)
  @brief Constructor
*/
Robot::Robot(const Matrix & initrobot, const Matrix & initmotor):
      Robot_basic(initrobot, initmotor, true, false)
{
    robotType_inv_kin();
}

/*!
  @fn Robot::Robot(const string & filename, const string & robotName)
  @brief Constructor
*/
Robot::Robot(const string & filename, const string & robotName):
      Robot_basic(Config(filename,true), robotName, true, false)
{
    robotType_inv_kin();
}

/*!
  @fn Robot::Robot(const string & robData, const string & robotName)
  @brief Constructor
*/
Robot::Robot(const Config & robData, const string & robotName):
      Robot_basic(robData, robotName, true, false)
{
    robotType_inv_kin();
}

/*!
  @fn Robot::Robot(const Robot & x)
  @brief Copy constructor
*/
Robot::Robot(const Robot & x) : Robot_basic(x)
{
}

Robot & Robot::operator=(const Robot & x)
//! @brief Overload = operator.
{
   Robot_basic::operator=(x);
   return *this;
}

void Robot::robotType_inv_kin()
/*!
  @brief Identify inverse kinematics familly.

  Identify the inverse kinematics analytic solution
  based on the similarity of the robot DH parameters and
  the DH parameters of known robots (ex: Puma, Rhino, ...). 
  The inverse kinematics will be based on a numerical
  alogorithm if there is no match .
*/
{
	if ( Puma_DH(this) )
		robotType = PUMA;
	else if ( Rhino_DH(this) )
		robotType = RHINO;
	else if ( ERS_Leg_DH(this) )
		robotType = ERS_LEG;
	else if ( ERS2xx_Head_DH(this) )
		robotType = ERS2XX_HEAD;
	else if ( ERS7_Head_DH(this) )
		robotType = ERS7_HEAD;
	else if ( PanTilt_DH(this) )
		robotType = PANTILT;
	else if ( Goose_Neck_DH(this) )
		robotType = GOOSENECK;
	else if ( Crab_Arm_DH(this) )
		robotType = CRABARM;
	else
		robotType = DEFAULT;
}

// ----------------- M O D I F I E D  D H  N O T A T I O N -------------------

/*!
  @fn mRobot::mRobot(const int ndof)
  @brief Constructor.
*/
mRobot::mRobot(const int ndof) : Robot_basic(ndof, false, false)
{
    robotType_inv_kin();
}

/*!
  @fn mRobot::mRobot(const Matrix & dhinit)
  @brief Constructor.
*/
mRobot::mRobot(const Matrix & dhinit) : Robot_basic(dhinit, false, false)
{
    robotType_inv_kin();
}

/*!
  @fn mRobot::mRobot(const Matrix & initrobot, const Matrix & initmotor)
  @brief Constructor.
*/
mRobot::mRobot(const Matrix & initrobot, const Matrix & initmotor) :
    Robot_basic(initrobot, initmotor, false, false)
{
    robotType_inv_kin();
}

/*!
  @fn mRobot::mRobot(const string & filename, const string & robotName)
  @brief Constructor.
*/
mRobot::mRobot(const string & filename, const string & robotName):
      Robot_basic(Config(filename,true), robotName, false, false)
{
    robotType_inv_kin();
}

/*!
  @fn mRobot::mRobot(const string & robData, const string & robotName)
  @brief Constructor.
*/
mRobot::mRobot(const Config & robData, const string & robotName):
      Robot_basic(robData, robotName, false, false)
{
    robotType_inv_kin();
}

/*!
  @fn mRobot::mRobot(const mRobot & x)
  @brief Copy constructor.
*/
mRobot::mRobot(const mRobot & x) : Robot_basic(x)
{
    robotType_inv_kin();
}

mRobot & mRobot::operator=(const mRobot & x)
//! @brief Overload = operator.
{
   Robot_basic::operator=(x);
   return *this;
}

void mRobot::robotType_inv_kin()
/*!
  @brief Identify inverse kinematics familly.

  Identify the inverse kinematics analytic solution
  based on the similarity of the robot DH parameters and
  the DH parameters of know robots (ex: Puma, Rhino, ...). 
  The inverse kinematics will be based on a numerical
  alogorithm if there is no match .
*/
{
	if ( Puma_mDH(this))
		robotType = PUMA;
	else if ( Rhino_mDH(this))
		robotType = RHINO;
	else
		robotType = DEFAULT;
}

/*!
  @fn mRobot_min_para::mRobot_min_para(const int ndof)
  @brief Constructor.
*/
mRobot_min_para::mRobot_min_para(const int ndof) : Robot_basic(ndof, false, true)
{
    robotType_inv_kin();
}

/*!
  @fn mRobot_min_para::mRobot_min_para(const Matrix & dhinit)
  @brief Constructor.
*/
mRobot_min_para::mRobot_min_para(const Matrix & dhinit):
      Robot_basic(dhinit, false, true)
{
    robotType_inv_kin();
}

/*!
  @fn mRobot_min_para::mRobot_min_para(const Matrix & initrobot, const Matrix & initmotor)
  @brief Constructor.
*/
mRobot_min_para::mRobot_min_para(const Matrix & initrobot, const Matrix & initmotor) :
      Robot_basic(initrobot, initmotor, false, true)
{
    robotType_inv_kin();
}

/*!
  @fn mRobot_min_para::mRobot_min_para(const mRobot_min_para & x)
  @brief Copy constructor
*/
mRobot_min_para::mRobot_min_para(const mRobot_min_para & x) : Robot_basic(x)
{
    robotType_inv_kin();
}

/*!
  @fn mRobot_min_para::mRobot_min_para(const string & filename, const string & robotName)
  @brief Constructor.
*/
mRobot_min_para::mRobot_min_para(const string & filename, const string & robotName):
      Robot_basic(Config(filename,true), robotName, false, true)
{
    robotType_inv_kin();
}

/*!
  @fn mRobot_min_para::mRobot_min_para(const string & robData, const string & robotName)
  @brief Constructor.
*/
mRobot_min_para::mRobot_min_para(const Config & robData, const string & robotName):
      Robot_basic(robData, robotName, false, true)
{
    robotType_inv_kin();
}

mRobot_min_para & mRobot_min_para::operator=(const mRobot_min_para & x)
//! @brief Overload = operator.
{
   Robot_basic::operator=(x);
   return *this;
}

void mRobot_min_para::robotType_inv_kin()
/*!
  @brief Identify inverse kinematics familly.

  Identify the inverse kinematics analytic solution
  based on the similarity of the robot DH parameters and
  the DH parameters of know robots (ex: Puma, Rhino, ...). 
  The inverse kinematics will be based on a numerical
  alogorithm if there is no match .
*/
{
	if ( Puma_mDH(this))
		robotType = PUMA;
	else if ( Rhino_mDH(this))
		robotType = RHINO;
	else
		robotType = DEFAULT;
}

// ---------------------- Non Member Functions -------------------------------

void perturb_robot(Robot_basic & robot, const double f)
/*!
  @brief  Modify a robot.
  @param robot: Robot_basic reference.
  @param f: Percentage of erreur between 0 and 1.

  f represents an error to added on the robot inertial parameter.
  f is between 0 (no error) and 1 (100% error).
*/
{
   if( (f < 0) || (f > 1) )
      cerr << "perturb_robot: f is not between 0 and 1" << endl;

   double fact;
   srand(clock());
   for(int i = 1; i <= robot.get_dof()+robot.get_fix(); i++)
   {
      fact = (2.0*rand()/RAND_MAX - 1)*f + 1;
      robot.links[i].set_Im(robot.links[i].get_Im()*fact);
      fact = (2.0*rand()/RAND_MAX - 1)*f + 1;
      robot.links[i].set_B(robot.links[i].get_B()*fact);
      fact = (2.0*rand()/RAND_MAX - 1)*f + 1;
      robot.links[i].set_Cf(robot.links[i].get_Cf()*fact);
      fact = (2.0*rand()/RAND_MAX - 1)*f + 1;
      robot.links[i].set_m(robot.links[i].get_m()*fact);
      //    fact = (2.0*rand()/RAND_MAX - 1)*f + 1;
      //    robot.links[i].mc = robot.links[i].mc*fact;
      fact = (2.0*rand()/RAND_MAX - 1)*f + 1;
      Matrix I = robot.links[i].get_I()*fact;
      robot.links[i].set_I(I);
   }
}

bool Puma_DH(const Robot_basic *robot)
/*!
  @brief  Return true if the robot is like a Puma on DH notation.

  Compare the robot DH table with the Puma DH table. The function 
  return true if the tables are similar (same alpha and similar a 
  and d parameters).
*/
{
    if (robot->get_dof() == 6) 
    {
	double a[7], d[7], alpha[7];
	for (int j = 1; j <= 6; j++)      
	{
	    if (robot->links[j].get_joint_type())  // All joints are rotoide
		return false;
	    a[j] = robot->links[j].get_a();
	    d[j] = robot->links[j].get_d();
	    alpha[j] = robot->links[j].get_alpha();
	}

// comparaison pour alpha de 90 a faire.
	if( !a[1] && !a[4] && !a[5] && !a[6] && !d[1] && !d[5] && 
	    !alpha[2] && !alpha[6] && (a[2]>=0) && (a[3]>=0) && (d[2]+d[3]>=0) 
	    && (d[4]>=0) && (d[6]>=0) )
	{
	    return true;
	}

    }

    return false;
}

bool Rhino_DH(const Robot_basic *robot)
/*!
  @brief  Return true if the robot is like a Rhino on DH notation.

  Compare the robot DH table with the Puma DH table. The function 
  return true if the tables are similar (same alpha and similar a 
  and d parameters).
*/
{
    if (robot->get_dof() == 5)
    {
	double a[6], d[6], alpha[6];
	for (int j = 1; j <= 5; j++)      
	{
	    if (robot->links[j].get_joint_type())  // All joints are rotoide
		return false;
	    a[j] = robot->links[j].get_a();
	    d[j] = robot->links[j].get_d();
	    alpha[j] = robot->links[j].get_alpha();
	}

	if ( !a[1] && !a[5] && !d[2] && !d[3] && !d[4] &&
	     !alpha[2] && !alpha[3] && !alpha[5] &&
	     (a[2]>=0) && (a[3]>=0) && (a[4]>=0) && (d[1]>=0) && (d[5]>=0) )
	{
	    return true;
	}

    }
    
    return false;
}

bool ERS_Leg_DH(const Robot_basic *robot)
/*! @brief  Return true if the robot is like the leg chain of an AIBO on DH notation. */
{
	if(robot->get_dof()==5) {
		Real alpha[6], theta[6];
		for(unsigned int i=1; i<=5; i++) {
			if(robot->links[i].get_joint_type()!=0)
				return false;
			alpha[i]=robot->links[i].get_alpha();
			theta[i]=robot->links[i].get_theta();
		}
		return (theta[1]==0 && alpha[1]!=0 && theta[2]!=0 && alpha[2]!=0 && theta[3]==0 && alpha[3]!=0);
	}
	return false;
}

bool ERS2xx_Head_DH(const Robot_basic *robot)
/*! @brief  Return true if the robot is like the camera chain of a 200 series AIBO on DH notation. */
{
	if(robot->get_dof()==5) {
		Real alpha[6], theta[6];
		bool revolute[6];
		for(unsigned int i=1; i<=5; i++) {
			alpha[i]=robot->links[i].get_alpha();
			theta[i]=robot->links[i].get_theta();
			revolute[i]=robot->links[i].get_joint_type()==0;
		}
		return (theta[1]==0 && alpha[1]!=0 &&
		        theta[2]==0 && alpha[2]!=0 && revolute[2] &&
		        theta[3]!=0 && alpha[3]!=0 && revolute[3] &&
		        revolute[4]);
	}
	return false;
}

bool ERS7_Head_DH(const Robot_basic *robot)
/*! @brief  Return true if the robot is like the camera or mouth chain of a 7 model AIBO on DH notation. */
{
	if(robot->get_dof()==5 || robot->get_dof()==6) {
		Real alpha[6], theta[6];
		bool revolute[6];
		for(unsigned int i=1; i<=5; i++) {
			alpha[i]=robot->links[i].get_alpha();
			theta[i]=robot->links[i].get_theta();
			revolute[i]=robot->links[i].get_joint_type()==0;
		}
		return (theta[1]==0 && alpha[1]!=0 &&
		        theta[2]==0 && alpha[2]!=0 && revolute[2] &&
		        theta[3]==0 && alpha[3]!=0 && revolute[3] &&
		        revolute[4]);
	}
	return false;
}

bool PanTilt_DH(const Robot_basic *robot)
/*
	@brief Return true if robot is like PanTilt on DH notation
	
	Compare the robot DH table with the PanTilt DH table. The function returns
	true if the two are similar.
*/
{
	if(robot->get_available_dof()==2)
		for(int i=1; i < robot->get_dof(); i++)
			if( robot->links[i].get_immobile()==false &&
			    robot->links[i+1].get_immobile()==false &&
			    robot->links[i+1].get_alpha() != 0 )
				return true;
	return false;
}

bool Goose_Neck_DH(const Robot_basic *robot)
/*
*/
{
	if(robot->get_dof()==5)
		if( robot->links[1].get_alpha() != 0 &&
		    robot->links[2].get_alpha() == 0 &&
		    robot->links[3].get_alpha() == 0 &&
		    robot->links[4].get_immobile() == 1 &&
		    robot->links[5].get_theta() > 0 )
			return true;
	return false;
}

bool Crab_Arm_DH(const Robot_basic *robot)
{
	if(robot->get_available_dof()==5)
		if( robot->links[1].get_immobile() == 1 && 
				robot->links[1].get_theta() > 0 && 
				robot->links[1].get_alpha() > 0 &&
				robot->links[2].get_alpha() > 0 &&
				robot->links[3].get_theta() > 0 &&
				robot->links[4].get_alpha() == 0 && robot->links[4].get_theta() == 0 &&
				robot->links[5].get_theta() < 0 && robot->links[5].get_alpha() < 0 &&
				robot->links[6].get_theta() < 0 )
			return true;
	return false;
}

bool Puma_mDH(const Robot_basic *robot)
/*!
  @brief  Return true if the robot is like a Puma on modified DH notation.

  Compare the robot DH table with the Puma DH table. The function 
  return true if the tables are similar (same alpha and similar a 
  and d parameters).
*/
{
    if (robot->get_dof() == 6) 
    {
	double a[7], d[7], alpha[7];
	for (int j = 1; j <= 6; j++)      
	{
	    if (robot->links[j].get_joint_type())  // All joints are rotoide
				return false;
	    a[j] = robot->links[j].get_a();
	    d[j] = robot->links[j].get_d();
	    alpha[j] = robot->links[j].get_alpha();
	}

// comparaison pour alpha de 90.

	if ( !a[1] && !a[2] && !a[5] && !a[6] && !d[1] && !d[5] &&
	     !alpha[1] && !alpha[3] && (a[3] >= 0) && (a[4] >= 0) && 
	     (d[2] + d[3] >=0) && (d[4]>=0) && (d[6]>=0) ) 
	{
	    return true;
	}

    }
    return false;
}

bool Rhino_mDH(const Robot_basic *robot)
/*!
  @brief  Return true if the robot is like a Rhino on modified DH notation.

  Compare the robot DH table with the Puma DH table. The function 
  return true if the tables are similar (same alpha and similar a 
  and d parameters).
*/
{
    if (robot->get_dof() == 5) 
    {
	double a[6], d[6], alpha[6];
	for (int j = 1; j <= 5; j++)      
	{
	    if (robot->links[j].get_joint_type())  // All joints are rotoide
		return false;
	    a[j] = robot->links[j].get_a();
	    d[j] = robot->links[j].get_d();
	    alpha[j] = robot->links[j].get_alpha();
	}
// comparaison pour alpha de 90 a ajouter
	if ( !a[1] && !a[2] && !d[2] && !d[3] && !d[4] && !alpha[1] && 
	     !alpha[3] && !alpha[4] && (d[1]>=0) && (a[3]>=0) && 
	     (a[4]>=0) && (a[5]>=0) && (d[5]>=0) )
	{
	    return true;
	}

    }    
    return false;
}

#ifdef use_namespace
}
#endif









