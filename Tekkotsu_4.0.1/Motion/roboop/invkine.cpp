/*
ROBOOP -- A robotics object oriented package in C++
Copyright (C) 2004  Etienne Lachance

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

2004/04/19: Vincent Drolet
   -Added Robot::inv_kin_rhino and Robot::inv_kin_puma member functions.

2004/04/20: Etienne Lachance
   -Added try, throw, catch statement in Robot::inv_kin_rhino and 
    Robot::inv_kin_puma in order to avoid singularity.

2004/05/21: Etienne Lachance
   -Added Doxygen documentation.

2004/07/01: Ethan Tira-Thompson
    -Added support for newmat's use_namespace #define, using ROBOOP namespace
    -Fixed warnings regarding atan2 when using float as Real type

2004/07/16: Ethan Tira-Thompson
    -If USING_FLOAT is set from newmat's include.h, ITOL is 1e-4 instead of 1e-6
     Motivation was occasional failures to converge when requiring 1e-6
     precision from floats using prismatic joints with ranges to 100's
    -A few modifications to support only solving for mobile joints in chain
    -Can now do inverse kinematics for frames other than end effector

2004/07/21: Ethan Tira-Thompson
    -Added inv_kin_pos() for times when you only care about position
    -Added inv_kin_orientation() for times when you only care about orientation
-------------------------------------------------------------------------------
*/

/*!
  @file invkine.cpp
  @brief Inverse kinematics solutions.
*/

#include "robot.h"
#include <sstream>

using namespace std;

#ifdef use_namespace
namespace ROBOOP {
  using namespace NEWMAT;
#endif
	//! @brief RCS/CVS version.
	static const char rcsid[] __UNUSED__ = "$Id: invkine.cpp,v 1.29 2007/11/16 16:46:45 ejt Exp $";

#define NITMAX 1000  //!< def maximum number of iterations in inv_kin 
#ifdef USING_FLOAT //from newmat's include.h
#  define ITOL   1e-4f //!< def tolerance for the end of iterations in inv_kin 
#else
#  define ITOL   1e-6  //!< def tolerance for the end of iterations in inv_kin 
#endif

ReturnMatrix Robot_basic::inv_kin(const Matrix & Tobj, const int mj)
//!  @brief Overload inv_kin function.
{
   bool converge = false;
   return inv_kin(Tobj, mj, dof, converge);
}


ReturnMatrix Robot_basic::inv_kin(const Matrix & Tobj, const int mj, const int endlink, bool & converge)
/*!
  @brief Numerical inverse kinematics.

  @param Tobj: Transformation matrix expressing the desired end effector pose.
  @param mj: Select algorithm type, 0: based on Jacobian, 1: based on derivative of T.
  @param endlink: the link to pretend is the end effector 
  @param converge: Indicate if the algorithm converge.

  The joint position vector before the inverse kinematics is returned if the 
  algorithm does not converge.
*/
{
   ColumnVector qout, dq, q_tmp;
   UpperTriangularMatrix U;

   qout = get_available_q();
   if(qout.nrows()==0) {
      converge=true;
      return qout;
   }
   q_tmp = qout;

   converge = false;
   if(mj == 0) {  // Jacobian based
      Matrix Ipd, A, B(6,1), M;
      for(int j = 1; j <= NITMAX; j++) {
         Ipd = (kine(endlink)).i()*Tobj;
         B(1,1) = Ipd(1,4);
         B(2,1) = Ipd(2,4);
         B(3,1) = Ipd(3,4);
         B(4,1) = Ipd(3,2);
         B(5,1) = Ipd(1,3);
         B(6,1) = Ipd(2,1);
         A = jacobian(endlink,endlink);
         QRZ(A,U);
         QRZ(A,B,M);
         dq = U.i()*M;

         while(dq.MaximumAbsoluteValue() > 1)
            dq /= 10;

         for(int k = 1; k<= dq.nrows(); k++)
            qout(k)+=dq(k);
         set_q(qout);

         if (dq.MaximumAbsoluteValue() < ITOL)
         {
            converge = true;
            break;
         }
      }
   } else {  // using partial derivative of T
      int adof=get_available_dof(endlink);
      Matrix A(12,adof),B,M;
      for(int j = 1; j <= NITMAX; j++) {
         B = (Tobj-kine(endlink)).SubMatrix(1,3,1,4).AsColumn();
         int k=1;
         for(int i = 1; i<=dof && k<=adof; i++) {
            if(links[i].immobile)
               continue;
            A.SubMatrix(1,12,k,k) = dTdqi(i,endlink).SubMatrix(1,3,1,4).AsColumn();
            k++;
         }
         if(A.ncols()==0) {
            converge=true;
            break;
         }
         QRZ(A,U);
         QRZ(A,B,M);
         dq = U.i()*M;

         while(dq.MaximumAbsoluteValue() > 1)
            dq /= 10;

         for(k = 1; k<=adof; k++)
            qout(k)+=dq(k);
         set_q(qout);
         if (dq.MaximumAbsoluteValue() < ITOL)
         {
            converge = true;
            break;
         }
      }
   }

   if(converge)
   {
      // Make sure that: -pi < qout <= pi for revolute joints
      int j=1;
      for(int i = 1; i <= dof; i++)
      {
         if(links[i].immobile)
            continue;
         if(links[i].get_joint_type() == 0) {
            while(qout(j) >= M_PI)
               qout(j) -= 2*M_PI;
            while(qout(j) <= -M_PI)
               qout(j) += 2*M_PI;
         }
         j++;
      }
      set_q(qout);
      qout.Release();
      return qout;
   }
   else
   {
      set_q(q_tmp);
      q_tmp.Release();
      return q_tmp;
   }
}


//void serrprintf(const char *, int a, int b, int c);
//#define DEBUG_ET { int debl=deb++; for(int a=0; a<50; a++) serrprintf("%d: %d %d\n",a,debl,__LINE__); }
#define DEBUG_ET ;

//cerr << a << ": " << debl << ' ' << __LINE__ << endl; }

ReturnMatrix Robot_basic::inv_kin_pos(const ColumnVector & Pobj, const int mj, const int endlink, const ColumnVector& /*Plink*/, bool & converge)
/*!
  @brief Numerical inverse kinematics.

  @param Pobj: Vector expressing the desired end effector position; can be homogenous
  @param mj: Select algorithm type, 0: based on Jacobian, 1: based on derivative of T.
  @param endlink: the link to pretend is the end effector 
  @param Plink: ignored for now
  @param converge: Indicate if the algorithm converge.

  The joint position vector before the inverse kinematics is returned if the 
  algorithm does not converge.
*/
{	
   ColumnVector qout, dq, q_tmp;
   UpperTriangularMatrix U;
   //int deb=0;
	 DEBUG_ET;

   qout = get_available_q();
   if(qout.nrows()==0) {
      converge=true;
      return qout;
   }
   q_tmp = qout;

   ColumnVector PHobj(4);
   if(Pobj.nrows()!=4) {
      PHobj.SubMatrix(1,Pobj.nrows(),1,1)=Pobj;
      PHobj.SubMatrix(Pobj.nrows()+1,4,1,1)=1;
   } else {
      PHobj=Pobj;
   }

	 DEBUG_ET;
   converge = false;
   if(mj == 0) {  // Jacobian based
   DEBUG_ET;
      Matrix Ipd, A, B(3,1),M;
      for(int j = 1; j <= NITMAX; j++) {
         Ipd = (kine(endlink)).i()*PHobj;
         B(1,1) = Ipd(1,1);
         B(2,1) = Ipd(2,1);
         B(3,1) = Ipd(3,1);
         A = jacobian(endlink,endlink);
         A = A.SubMatrix(1,3,1,A.ncols());
         //need to remove any joints which do not affect position
         //otherwise those joints's q go nuts
         int removeCount=0;
         for(int k=1; k<= A.ncols(); k++)
            if(A.SubMatrix(1,3,k,k).MaximumAbsoluteValue()<ITOL)
               removeCount++;
         Matrix A2(3,A.ncols()-removeCount);
         if(removeCount==0)
            A2=A;
         else
            for(int k=1,m=1; k<= A.ncols(); k++) {
               if(A.SubMatrix(1,3,k,k).MaximumAbsoluteValue()<ITOL)
                  continue;
               A2.SubMatrix(1,3,m,m)=A.SubMatrix(1,3,k,k);
               m++;
            }
         //ok... on with the show, using A2 now
         if(A2.ncols()==0) {
            converge=true;
            break;
         }
				 {
					 stringstream ss;
					 ss << "A2-pre:\n";
					 for(int r=1; r<=A2.nrows(); r++) {
						 for(int c=1; c<=A2.ncols(); c++) {
							 ss << A2(r,c) << ' ';
						 }
						 ss << '\n';
					 }
         QRZ(A2,U);
				 /*	 ss << "A2-mid:\n";
	 for(int r=1; r<=A2.nrows(); r++) {
		 for(int c=1; c<=A2.ncols(); c++) {
			 ss << A2(r,c) << ' ';
		 }
		 ss << '\n';
		 }*/
         QRZ(A2,B,M);
         //serrprintf(ss.str().c_str(),0,0,0);
			}
   DEBUG_ET;
   /*stringstream ss;
	 ss << "A2-post:\n";
	 for(int r=1; r<=A2.nrows(); r++) {
		 for(int c=1; c<=A2.ncols(); c++) {
			 ss << A2(r,c) << ' ';
		 }
		 ss << '\n';
	 }
	 ss << "U:\n";
	 for(int r=1; r<=4; r++) {
		 for(int c=r; c<=4; c++) {
			 ss << U(r,c) << ' ';
		 }
		 ss << '\n';
	 }
	 ss << "M: ";
	 for(int r=1; r<=M.nrows(); r++) {
		 ss << M(r,1) << ' ';
	 }
	 ss << '\n';*/
	 //serrprintf(ss.str().c_str(),0,0,0);

   DEBUG_ET;
         dq = U.i()*M;

   DEBUG_ET;
         while(dq.MaximumAbsoluteValue() > 1)
            dq /= 10;

         for(int k = 1,m=1; m<= dq.nrows(); k++)
            if(A.SubMatrix(1,3,k,k).MaximumAbsoluteValue()>=ITOL)
               qout(k)+=dq(m++);
         set_q(qout);

         if (dq.MaximumAbsoluteValue() < ITOL)
         {
            converge = true;
            break;
         }
      }
   } else {  // using partial derivative of T
      int adof=get_available_dof(endlink);
      Matrix A(3,adof),Rcur,B,M;
      ColumnVector pcur;
      bool used[adof];
      for(int j = 1; j <= NITMAX; j++) {
         kine(Rcur,pcur,endlink);
         B = (PHobj.SubMatrix(1,3,1,1)-pcur);
         int k=1,m=1;
         for(int i = 1; m<=adof; i++) {
            if(links[i].immobile)
               continue;
            Matrix Atmp=dTdqi(i,endlink).SubMatrix(1,3,4,4);
            used[m]=(Atmp.MaximumAbsoluteValue()>=ITOL);
            if(!used[m++])
               continue;
            A.SubMatrix(1,3,k,k) = Atmp;
            k++;
         }
         Matrix A2=A.SubMatrix(1,3,1,k-1);
         if(A2.ncols()==0) {
            converge=true;
            break;
         }
         QRZ(A2,U);
         QRZ(A2,B,M);
         dq = U.i()*M;

         while(dq.MaximumAbsoluteValue() > 1)
            dq /= 10;

         for(k = m = 1; k<=adof; k++)
            if(used[k])
               qout(k)+=dq(m++);
         set_q(qout);

         if (dq.MaximumAbsoluteValue() < ITOL)
         {
            converge = true;
            break;
         }
      }
   }
	 DEBUG_ET;

   if(converge)
   {
	 DEBUG_ET;
      // Make sure that: -pi < qout <= pi for revolute joints
      int j=1;
      for(int i = 1; i <= dof; i++)
      {
         if(links[i].immobile)
            continue;
				 unsigned int * test=(unsigned int*)&qout(j);
				 if(((*test)&(255U<<23))==(255U<<23)) {
				   //serrprintf("qout %d is not-finite\n",j,0,0);
					 set_q(q_tmp);
					 q_tmp.Release();
					 return q_tmp;
				 }
				 /*
         if(links[i].get_joint_type() == 0 && finite(qout(j))) {
            while(qout(j) >= M_PI)
               qout(j) -= 2*M_PI;
            while(qout(j) <= -M_PI)
               qout(j) += 2*M_PI;
         }
				 */
         j++;
      }
	 DEBUG_ET;
      set_q(qout);
      qout.Release();
	 DEBUG_ET;
      return qout;
   }
   else
   {
	 DEBUG_ET;
      set_q(q_tmp);
      q_tmp.Release();
	 DEBUG_ET;
      return q_tmp;
   }
}

ReturnMatrix Robot_basic::inv_kin_orientation(const Matrix & Robj, const int mj, const int endlink, bool & converge)
/*!
  @brief Numerical inverse kinematics.

  @param Robj: Rotation matrix expressing the desired end effector orientation w.r.t base frame
  @param mj: Select algorithm type, 0: based on Jacobian, 1: based on derivative of T.
  @param endlink: the link to pretend is the end effector 
  @param converge: Indicate if the algorithm converge.

  The joint position vector before the inverse kinematics is returned if the 
  algorithm does not converge.
*/
{
   ColumnVector qout, dq, q_tmp;
   UpperTriangularMatrix U;

   qout = get_available_q();
   if(qout.nrows()==0) {
      converge=true;
      return qout;
   }
   q_tmp = qout;

   Matrix RHobj(4,3);
   RHobj.SubMatrix(1,3,1,3)=Robj;
   RHobj.SubMatrix(4,4,1,3)=0;

   converge = false;
   if(mj == 0) {  // Jacobian based
      Matrix Ipd, A, B(3,1),M;
      for(int j = 1; j <= NITMAX; j++) {
         Ipd = kine(endlink).i()*RHobj;
         B(1,1) = Ipd(3,2);
         B(2,1) = Ipd(1,3);
         B(3,1) = Ipd(2,1);
         A = jacobian(endlink,endlink);
         A = A.SubMatrix(4,6,1,A.ncols());
         //need to remove any joints which do not affect position
         //otherwise those joints's q go nuts
         int removeCount=0;
         for(int k=1; k<= A.ncols(); k++)
            if(A.SubMatrix(1,3,k,k).MaximumAbsoluteValue()<ITOL)
               removeCount++;
         Matrix A2(3,A.ncols()-removeCount);
         if(removeCount==0)
            A2=A;
         else
            for(int k=1,m=1; k<= A.ncols(); k++) {
               if(A.SubMatrix(1,3,k,k).MaximumAbsoluteValue()<ITOL)
                  continue;
               A2.SubMatrix(1,3,m,m)=A.SubMatrix(1,3,k,k);
               m++;
            }
         //ok... on with the show, using A2 now
         if(A2.ncols()==0) {
            converge=true;
            break;
         }
         QRZ(A2,U);
         QRZ(A2,B,M);
         dq = U.i()*M;

         while(dq.MaximumAbsoluteValue() > 1)
            dq /= 10;

         for(int k = 1,m=1; m<= dq.nrows(); k++)
            if(A.SubMatrix(1,3,k,k).MaximumAbsoluteValue()>=ITOL)
               qout(k)+=dq(m++);
         set_q(qout);

         if (dq.MaximumAbsoluteValue() < ITOL)
         {
            converge = true;
            break;
         }
      }
   } else {  // using partial derivative of T
      int adof=get_available_dof(endlink);
      Matrix A(9,adof),Rcur,B,M;
      ColumnVector pcur;
      bool used[adof];
      for(int j = 1; j <= NITMAX; j++) {
         kine(Rcur,pcur,endlink);
         B = (Robj-Rcur).AsColumn();
         int k=1,m=1;
         for(int i = 1; m<=adof; i++) {
            if(links[i].immobile)
               continue;
            Matrix Atmp=dTdqi(i,endlink).SubMatrix(1,3,1,3).AsColumn();
            used[m]=(Atmp.MaximumAbsoluteValue()>=ITOL);
            if(!used[m++])
               continue;
            A.SubMatrix(1,9,k,k) = Atmp;
            k++;
         }
         Matrix A2=A.SubMatrix(1,9,1,k-1);
         if(A2.ncols()==0) {
            converge=true;
            break;
         }
         QRZ(A2,U);
         QRZ(A2,B,M);
         dq = U.i()*M;

         while(dq.MaximumAbsoluteValue() > 1)
            dq /= 10;

         for(k = m = 1; k<=adof; k++)
            if(used[k])
               qout(k)+=dq(m++);
         set_q(qout);
         
         if (dq.MaximumAbsoluteValue() < ITOL)
         {
            converge = true;
            break;
         }
      }
   }

   if(converge)
   {
      // Make sure that: -pi < qout <= pi for revolute joints
      int j=1;
      for(int i = 1; i <= dof; i++)
      {
         if(links[i].immobile)
            continue;
         if(links[i].get_joint_type() == 0) {
            while(qout(j) >= M_PI)
               qout(j) -= 2*M_PI;
            while(qout(j) <= -M_PI)
               qout(j) += 2*M_PI;
         }
         j++;
      }
      set_q(qout);
      qout.Release();
      return qout;
   }
   else
   {
      set_q(q_tmp);
      q_tmp.Release();
      return q_tmp;
   }
}

// ---------------------  R O B O T   DH   N O T A T I O N  --------------------------

ReturnMatrix Robot::inv_kin(const Matrix & Tobj, const int mj)
//!  @brief Overload inv_kin function.
{
   bool converge = false;
   return inv_kin(Tobj, mj, dof, converge);
}


ReturnMatrix Robot::inv_kin(const Matrix & Tobj, const int mj, const int endlink, bool & converge)
/*!
  @brief Inverse kinematics solutions.

  The solution is based on the analytic inverse kinematics if robot type (familly)
  is Rhino or Puma, otherwise used the numerical algoritm defined in Robot_basic
  class.
*/
{
   switch (robotType) {
      case RHINO:
         return inv_kin_rhino(Tobj, converge);
      case PUMA:
         return inv_kin_puma(Tobj, converge);
      case ERS_LEG:
      case ERS2XX_HEAD:
      case ERS7_HEAD:
         //no specializations yet... :(
      case PANTILT:
      case GOOSENECK:
      	return inv_kin_goose_neck(Tobj);
      case CRABARM:
      default:
         return Robot_basic::inv_kin(Tobj, mj, endlink, converge);
   }
}

ReturnMatrix Robot::inv_kin_pos(const ColumnVector & Pobj, const int mj, const int endlink, const ColumnVector& Plink, bool & converge)
/*!
  @brief Inverse kinematics solutions.

  The solution is based on the analytic inverse kinematics if robot type (familly)
  is Rhino or Puma, otherwise used the numerical algoritm defined in Robot_basic
  class.
*/
{
   switch (robotType) {
      case ERS_LEG:
      case ERS2XX_HEAD:
      case ERS7_HEAD:
         return inv_kin_ers_pos(Pobj, endlink, Plink, converge);
      case RHINO:
      case PUMA:
         //no specializations yet... :(
       case PANTILT:
         return inv_kin_pan_tilt(Pobj, converge);
      case GOOSENECK:
      	return inv_kin_goose_neck_pos(Pobj);
      case CRABARM:
      default:
         return Robot_basic::inv_kin_pos(Pobj, mj, endlink, Plink, converge);
   }
}

ReturnMatrix Robot::inv_kin_orientation(const Matrix & Robj, const int mj, const int endlink, bool & converge)
/*!
  @brief Inverse kinematics solutions.

  The solution is based on the analytic inverse kinematics if robot type (familly)
  is Rhino or Puma, otherwise used the numerical algoritm defined in Robot_basic
  class.
*/
{
   switch (robotType) {
      case ERS_LEG:
      case ERS2XX_HEAD:
      case ERS7_HEAD:
      case RHINO:
      case PUMA:
         //no specializations yet... :(
      case PANTILT:
      case GOOSENECK:
//       return inv_kin_goose_neck_orientation(Robj)
	  case CRABARM:
      default:
         return Robot_basic::inv_kin_orientation(Robj, mj, endlink, converge);
   }
}

ReturnMatrix Robot::inv_kin_rhino(const Matrix & Tobj, bool & converge)
/*!
  @brief Analytic Rhino inverse kinematics.

  converge will be false if the desired end effector pose is outside robot range.
*/
{
    ColumnVector qout(5), q_actual;    
    q_actual = get_q();

    try
    {
	Real theta[6] , diff1, diff2, tmp,
	     angle , L=0.0 , M=0.0 , K=0.0 , H=0.0 , Gl=0.0 ;
    
	// Calcul les deux angles possibles
	theta[0] = atan2(Tobj(2,4),
			 Tobj(1,4));
	
	theta[1] = atan2(-Tobj(2,4),
			 -Tobj(1,4))  ;
	
	diff1 = fabs(q_actual(1)-theta[0]) ;		
	if (diff1 > M_PI)
	    diff1 = 2*M_PI - diff1;
	
	diff2 = fabs(q_actual(1)-theta[1]);
	if (diff2 > M_PI)
	    diff1 = 2*M_PI - diff2 ;
	
	// Prend l'angle le plus proche de sa position actuel
	if (diff1 < diff2)		   
	    theta[1] = theta[0] ;
	
	theta[5] = atan2(sin(theta[1])*Tobj(1,1) - cos(theta[1])*Tobj(2,1), 
			 sin(theta[1])*Tobj(1,2) - cos(theta[1])*Tobj(2,2));
	
	// angle = theta1 +theta2 + theta3
	angle = atan2(-1*cos(theta[1])*Tobj(1,3) - sin(theta[1])*Tobj(2,3),
		      -1*Tobj(3,3));
	
	L = cos(theta[1])*Tobj(1,4) + 
	    sin(theta[1])*Tobj(2,4) + 
	    links[5].d*sin(angle) - 
	    links[4].a*cos(angle);
	M = links[1].d - 
	    Tobj(3,4) - 
	    links[5].d*cos(angle) - 
	    links[4].a*sin(angle);
	K = (L*L + M*M - links[3].a*links[3].a - 
	     links[2].a*links[2].a) / 
	    (2 * links[3].a * links[2].a);
	
	tmp = 1-K*K;
	if (tmp < 0)
	    throw std::out_of_range("sqrt of negative number not allowed.");

	theta[0] = atan2( sqrt(tmp) , K );
	theta[3] = atan2( -sqrt(tmp) , K );	
	
	diff1 = fabs(q_actual(3)-theta[0]) ;
	if (diff1 > M_PI)
	    diff1 = 2*M_PI - diff1 ;
	
	diff2 = fabs(q_actual(3)-theta[3]);
	if (diff2 > M_PI)
	    diff1 = 2*M_PI - diff2 ;
	
	if (diff1 < diff2)
	    theta[3] = theta[0] ;
	
	H = cos(theta[3]) * links[3].a + links[2].a;
	Gl = sin(theta[3]) * links[3].a;
	
	theta[2] = atan2( M , L ) - atan2( Gl , H );
	theta[4] = atan2( -1*cos(theta[1])*Tobj(1,3) - sin(theta[1])*Tobj(2,3) , 
			  -1*Tobj(3,3)) - theta[2] - theta[3] ;
	
	qout(1) = theta[1];
	qout(2) = theta[2];
	qout(3) = theta[3];
	qout(4) = theta[4];
	qout(5) = theta[5];
	set_q(qout);
	
	converge = true;
    }
    catch(std::out_of_range & e)
    {
	converge = false; 
	set_q(q_actual);
	qout = q_actual;
    }

    qout.Release();
    return qout;
}


ReturnMatrix Robot::inv_kin_puma(const Matrix & Tobj, bool & converge)
/*!
  @brief Analytic Puma inverse kinematics.

  converge will be false if the desired end effector pose is outside robot range.
*/
{
    ColumnVector qout(6), q_actual;
    q_actual = get_q();

    try
    {  
	Real theta[7] , diff1, diff2, tmp,
	     A = 0.0 , B = 0.0 , Cl = 0.0 , D =0.0, Ro = 0.0,
	     H = 0.0 , L = 0.0 , M = 0.0;
	
// Removed d6 component because in the Puma inverse kinematics solution 
// d6 = 0. 
	if (links[6].d)
	{
	    ColumnVector tmpd6(3);
	    tmpd6(1)=0; tmpd6(2)=0; tmpd6(3)=links[6].d;
	    tmpd6 = Tobj.SubMatrix(1,3,1,3)*tmpd6;
	    Tobj.SubMatrix(1,3,4,4) = Tobj.SubMatrix(1,3,4,4) - tmpd6;
	}

	tmp = Tobj(2,4)*Tobj(2,4) + Tobj(1,4)*Tobj(1,4);
	if (tmp < 0)
	    throw std::out_of_range("sqrt of negative number not allowed.");

	Ro = sqrt(tmp);
	D = (links[2].d+links[3].d) / Ro;
	
	tmp = 1-D*D;
	if (tmp < 0)
	    throw std::out_of_range("sqrt of negative number not allowed.");
	
	//Calcul les deux angles possibles
	theta[0] =  atan2(Tobj(2,4),Tobj(1,4)) - atan2(D, sqrt(tmp));	 
	//Calcul les deux angles possibles
	theta[1] =  atan2(Tobj(2,4),Tobj(1,4)) - atan2(D , -sqrt(tmp));
	
	diff1 = fabs(q_actual(1)-theta[0]);
	if (diff1 > M_PI)
	    diff1 = 2*M_PI - diff1;
	
	diff2 = fabs(q_actual(1)-theta[1]);
	if (diff2 > M_PI)
	    diff1 = 2*M_PI - diff2;
	
	// Prend l'angle le plus proche de sa position actuel
	if (diff1 < diff2)
	    theta[1] = theta[0];
	
	tmp = links[3].a*links[3].a + links[4].d*links[4].d;
	if (tmp < 0)
	    throw std::out_of_range("sqrt of negative number not allowed.");
	
	Ro = sqrt(tmp);
	B = atan2(links[4].d,links[3].a);
	Cl = Tobj(1,4)*Tobj(1,4) + 
	     Tobj(2,4)*Tobj(2,4) + 
	     Tobj(3,4)*Tobj(3,4) - 
	     (links[2].d + links[3].d)*(links[2].d + links[3].d) - 
	     links[2].a*links[2].a - 
	     links[3].a*links[3].a - 
	     links[4].d*links[4].d; 
	A = Cl / (2*links[2].a);
	
	tmp = 1-A/Ro*A/Ro;
	if (tmp < 0)
	    throw std::out_of_range("sqrt of negative number not allowed.");
	
	theta[0] = atan2(sqrt(tmp) , A/Ro) + B;
	theta[3] = atan2(-sqrt(tmp) , A/Ro) + B;
	
	diff1 = fabs(q_actual(3)-theta[0]);
	if (diff1 > M_PI)
	    diff1 = 2*M_PI - diff1 ;
	
	diff2 = fabs(q_actual(3)-theta[3]);
	if (diff2 > M_PI)
	    diff1 = 2*M_PI - diff2;
	
	//Prend l'angle le plus proche de sa position actuel
	if (diff1 < diff2)
	    theta[3] = theta[0];
	
	H = cos(theta[1])*Tobj(1,4) + sin(theta[1])*Tobj(2,4);
	L = sin(theta[3])*links[4].d + cos(theta[3])*links[3].a + links[2].a;
	M = cos(theta[3])*links[4].d - sin(theta[3])*links[3].a;
	
	theta[2] = atan2( M , L ) - atan2(Tobj(3,4) , H );
	
	theta[0] = atan2( -sin(theta[1])*Tobj(1,3) + cos(theta[1])*Tobj(2,3) , 
			  cos(theta[2] + theta[3]) * 
			  (cos(theta[1]) * Tobj(1,3) + sin(theta[1])*Tobj(2,3)) 
			  - (sin(theta[2]+theta[3])*Tobj(3,3)) );
	
	theta[4] = atan2(-1*(-sin(theta[1])*Tobj(1,3) + cos(theta[1])*Tobj(2,3)), 
			 -cos(theta[2] + theta[3]) * 
			 (cos(theta[1]) * Tobj(1,3) + sin(theta[1])*Tobj(2,3)) 
			 + (sin(theta[2]+theta[3])*Tobj(3,3)) );
	
	diff1 = fabs(q_actual(4)-theta[0]);
	if (diff1 > M_PI)
	    diff1 = 2*M_PI - diff1;
	
	diff2 = fabs(q_actual(4)-theta[4]);
	if (diff2 > M_PI)
	    diff1 = 2*M_PI - diff2;
	
	// Prend l'angle le plus proche de sa position actuel
	if (diff1 < diff2)
	    theta[4] = theta[0];
	
	theta[5] = atan2( cos(theta[4]) * 
			  ( cos(theta[2] + theta[3]) * 
			    (cos(theta[1]) * Tobj(1,3) 
			     + sin(theta[1])*Tobj(2,3)) 
			    - (sin(theta[2]+theta[3])*Tobj(3,3)) ) + 
			  sin(theta[4])*(-sin(theta[1])*Tobj(1,3) 
					 + cos(theta[1])*Tobj(2,3)) , 
			  sin(theta[2]+theta[3]) * (cos(theta[1]) * Tobj(1,3) 
						    + sin(theta[1])*Tobj(2,3) ) 
			  + (cos(theta[2]+theta[3])*Tobj(3,3)) );
	
	theta[6] = atan2( -sin(theta[4]) 
			  * ( cos(theta[2] + theta[3]) * 
			      (cos(theta[1]) * Tobj(1,1) 
			       + sin(theta[1])*Tobj(2,1)) 
			      - (sin(theta[2]+theta[3])*Tobj(3,1))) + 
			  cos(theta[4])*(-sin(theta[1])*Tobj(1,1) 
					 + cos(theta[1])*Tobj(2,1)), 
			  -sin(theta[4]) * ( cos(theta[2] + theta[3]) * 
					     (cos(theta[1]) * Tobj(1,2) 
					      + sin(theta[1])*Tobj(2,2)) 
					     - (sin(theta[2]+theta[3])*Tobj(3,2))) +
			  cos(theta[4])*(-sin(theta[1])*Tobj(1,2) 
					 + cos(theta[1])*Tobj(2,2)) );
	
	qout(1) = theta[1];
	qout(2) = theta[2];
	qout(3) = theta[3];
	qout(4) = theta[4];
	qout(5) = theta[5];
	qout(6) = theta[6];
	set_q(qout);
	
	converge = true; 
    }
    catch(std::out_of_range & e)
    {
	converge = false; 
	set_q(q_actual);
	qout = q_actual;
    }

    qout.Release();
    return qout;
}

ReturnMatrix Robot::inv_kin_pan_tilt(const ColumnVector & Pobj, bool & converge)
/*
	@brief Inverse Kinematics of Pan Tilt Robot
	@param Pobj: Vector expressing the desired end effector position
	@param converge: Indicate if the algorithm converge.
*/
{
	//std::cout << "Inv_Kine Pan Tilt!" << std::endl;
	ColumnVector qout, qtmp,  /* Joint angle vectors */
				 xyz1, xyz2,  /* Frame 1 and 2 coordinates */
				 xyz0 = Pobj; /* Base frame coordinates */
	Matrix 	M01, /* Matrix to convert coordinates from base (frame 0) to frame 1 */
			M12; /* Matrix to convert coordinates from frame 1 to frame 2 */

	qtmp = get_q();
	M01 = convertFrame(1,2);
	//std::cout << "Matrix M01" << std::endl << setw(5) << M01 << std::endl;
	xyz1 = M01*xyz0;
	//std::cout << xyz1(1) << " " << xyz1(2) << " " << xyz1(3) << std::endl;
	qtmp(2)=atan2(xyz1(2),xyz1(1));
	set_q(qtmp);
	M12 = convertFrame(2,3);
	//std::cout << "Matrix M12" << std::endl << setw(5) << M12 << std::endl;
	xyz2 = M12*xyz1;
	//std::cout << xyz2(1) << " " << xyz2(2) << " " << xyz2(3) << std::endl;
	qtmp(3)=atan2(xyz2(2),xyz2(1));
	set_q(qtmp);

//Now I have to check to see if the angles lie within the ranges of the pan and tilt	
//If not, set angles to closest min/max value.
	if( qtmp(2) < links[2].get_theta_min() )
		qtmp(2) = links[2].get_theta_min();
	if( qtmp(2) > links[2].get_theta_max() )
		qtmp(2) = links[2].get_theta_max();
	if( qtmp(3) < links[3].get_theta_min() )
		qtmp(3) = links[3].get_theta_min();
	if( qtmp(3) > links[3].get_theta_max() )
		qtmp(3) = links[3].get_theta_max();
	//std::cout << "inv_kin_pan_tilt computed the values:" << endl;	
	//std::cout << "Pan: "<< qtmp(2) << " Tilt: " << qtmp(3) << std::endl;
	qout = qtmp;
	converge=true;
	return qout;
}

ReturnMatrix Robot::inv_kin_goose_neck(const Matrix & Tobj)
/*
*/
{
	//std::cout << "inv_kin_goose_neck was called!" << std::endl;
	ColumnVector qout, Pobj(3);
	Real phi;
	Pobj(1) = Tobj(1,4);
	Pobj(2) = Tobj(2,4);
	Pobj(3) = Tobj(3,4);
	phi = atan2(Tobj(3,3),Tobj(1,3));
	qout = goose_neck_angles(Pobj, phi);
	return qout;
}

ReturnMatrix Robot::inv_kin_goose_neck_pos(const ColumnVector & Pobj)
/*
*/
{
		//std::cout << "inv_kin_goose_neck_pos was called!" << std::endl;
	ColumnVector qout;
	qout = goose_neck_angles(Pobj, -1.57079632679);
	return qout;
}

/*ReturnMatrix Robot::inv_kin_goose_neck_orientation(const Matrix & Robj)
{
	
}
*/

ReturnMatrix Robot::goose_neck_angles(const ColumnVector & Pobj, Real phi)
/*
*/
{
	Real 	c2, 
			qp, q1, q2, q3, //phi=-1.57079632679, //phi is the Tool Orientation
			                                    //this phi puts the tool above the target
			K1, K2,
			L1=links[2].get_a(),//Planar Link Lengths
			L2=links[3].get_a(),
			L3=links[5].get_d();
	ColumnVector 	qtmp, qout,
					xyz0 = Pobj,
					xyz_t,xyz_w;
	Matrix 	M01, M12;
	bool valid;
	
	qout = get_q();
	qtmp = qout;
	qp = atan2(xyz0(2),xyz0(1));
	//std::cout << "Neck Pan: " << qp << std::endl;
	qtmp(2)=qp;
	set_q(qtmp);	//This will make the base pan
	
	M01 = convertFrame(1,2);

	//std::cout << "Matrix M01" << std::endl << setw(5) << M01 << std::endl;
	xyz_t = M01 * xyz0;  //xyz_t are the tool tip coordinates
	//std::cout << xyz_t(1) << " " << xyz_t(2) << " " << xyz_t(3) << std::endl;
	//Compute the wrist position
	xyz_w = xyz_t; //This makes xyz_w a 4x1 matrix
	xyz_w(1)=xyz_t(1)-L3*cos(phi);
	xyz_w(2)=xyz_t(2)-L3*sin(phi);
	
	//std::cout << "The wrist is to be placed at (" << xyz_w(1) << "," << xyz_w(2) << ")." << std::endl;
	
	c2 = ( ( (xyz_w(1)*xyz_w(1)) + (xyz_w(2)*xyz_w(2)) - ( (L1*L1) + (L2*L2) ) ) / ( 2 * L1 * L2 ) );
	//std::cout << "Cosine Theta2 is " << c2 << std::endl;
	
	if ( (c2*c2) > 1){
		c2 = sign(c2);
		valid = false;
	}
	else
		valid = true;
/*		
	s2plus = sqrt(1-(c2*c2));
	std::cout << "Sine Theta2 is " << s2plus << std::endl;
	q2 = atan2(s2plus,c2);  //Compute q2
*/
	q2 = -acos(c2);
		
//	K1 = L1 + (L2 * c2);
//	K2 = L2 * s2plus;
	
	K1 = L1 + (L2*cos(q2));
	K2 = L2 * sin(q2);
	
	q1 = atan2(xyz_w(2),xyz_w(1)) - atan2(K2,K1);  //Compute q1
	
	q3 = phi - q1 - q2;  //Compute q3
	//std::cout << "The angles are " << qp << " " << q1 << " " << q2 << " " << q3 << std::endl;
	qtmp(2)=qp; qtmp(3)=q1; qtmp(4)=q2; qtmp(5)=q3;
	set_q(qtmp);
	
	if(valid)
		return qtmp;
	else
		return qout;
}

ReturnMatrix Robot::inv_kin_ers_pos(const ColumnVector & Pobj, const int endlink, const ColumnVector& Plink, bool & converge) {
	//std::cout << "Called inv_kin_ers_pos!" << std::endl;
   bool converges[3];
   
   bool third_invert=(robotType==ERS7_HEAD || robotType==ERS2XX_HEAD);
   bool second_invert=false;

   if(endlink>=2) {
      if(endlink>=3) {
         if(endlink>=4)
            set_q(computeThirdERSLink(4,Pobj,endlink,Plink,third_invert,links[4].get_theta_min(),links[4].get_theta_max(),converges[2]),4);
         set_q(computeSecondERSLink(3,Pobj,endlink,Plink,second_invert,links[3].get_theta_min(),links[3].get_theta_max(),converges[1]),3);
      }
      set_q(computeFirstERSLink(2,Pobj,endlink,Plink,links[2].get_theta_min(),links[2].get_theta_max(),converges[0]),2);
   }
   
   //check if link 2 is maxed out
   if(!converges[0]) {
      //redo links 3 and 4 since link 2 was limited
      if(endlink>=3) {
         if(endlink>=4)
            set_q(computeSecondERSLink(4,Pobj,endlink,Plink,second_invert,links[4].get_theta_min(),links[4].get_theta_max(),converges[2]),4);
         set_q(computeFirstERSLink(3,Pobj,endlink,Plink,links[3].get_theta_min(),links[3].get_theta_max(),converges[1]),3);
      }
   }
   
   //check again, maybe now link 3 is maxed out as well
   if(!converges[1] && endlink>=4) {
      //redo link 4 since link 3 was limited
      set_q(computeFirstERSLink(4,Pobj,endlink,Plink,links[4].get_theta_min(),links[4].get_theta_max(),converges[2]),4);
   }
   
   converge=converges[0] && converges[1] && converges[2];
   return get_q();
}

Real Robot::computeFirstERSLink(int curlink, const ColumnVector & Pobj, const int endlink, const ColumnVector& Plink, Real min, Real max, bool & converge) {
   //Convert Pobj and Plink to be frame 'curlink' relative
   ColumnVector po=convertFrame(0,curlink)*Pobj;
   ColumnVector pl=convertLink(endlink,curlink)*Plink;
	 if(fabs(pl(1))<ITOL && fabs(pl(2))<ITOL) {
		 //Plink is along axis of rotation - nothing we do is going to move it, so don't move at all
		 converge=false; //debatable
		 return links[curlink].get_q();
	 }
   Real to=atan2(po(2),po(1));
   Real tl=atan2(pl(2),pl(1));
   Real qtgt=normalize_angle(to-tl);
   Real qans=limit_angle(qtgt,min,max);
   converge=(qtgt==qans);
   return qans;
}

Real Robot::computeSecondERSLink(int curlink, const ColumnVector & Pobj, const int endlink, const ColumnVector& Plink, bool invert, Real min, Real max, bool & converge) {
	 if(Plink(4)==0) //infinite ray
		 return computeFirstERSLink(curlink,Pobj,endlink,Plink,min,max,converge);
   //Convert Pobj, Plink, z3 to be frame 'curlink' relative
   ColumnVector po=convertFrame(0,curlink)*Pobj;
   ColumnVector pl=convertLink(endlink,curlink)*Plink;
	 if(fabs(pl(1))<ITOL && fabs(pl(2))<ITOL) {
		 //Plink is along axis of rotation - nothing we do is going to move it, so don't move at all
		 converge=false; //debatable
		 return links[curlink].get_q();
	 }
   Matrix Rp;
   ColumnVector pp;
   convertFrame(Rp,pp,curlink-1,curlink);
   ColumnVector zp=Rp.SubMatrix(1,3,3,3);
   Real dot_zppo=zp(1)*po(1)+zp(2)*po(2)+zp(3)*po(3);
   Real ao=M_PI_2-acos(dot_zppo/sqrt(zp.SumSquare()*po.SumSquare()));
   Real r=(pl(1)*pl(1)+pl(2)*pl(2))/(pl(4)*pl(4));
   Real tao=tan(ao);
   tao*=tao;
   Real tors=(r-pl(3)*pl(3)*tao)/(r+r*tao);
   Real sign;
   if(dot_zppo>0)
      sign=(DotProduct(zp,pp)<0)?1:-1;
   else
      sign=(DotProduct(zp,pp)<0)?-1:1;
   if(tors<0) {
      //disp('out of reach')
      converge=false;
      return limit_angle(sign*M_PI_2,min,max);
   } else {
      Real to=sign*acos(sqrt(tors));
      if(invert)
         to=M_PI-to;
      Real tl=atan2(pl(2),pl(1));
      Real qtgt=normalize_angle(to-tl);
      Real qans=limit_angle(qtgt,min,max);
      converge=(qtgt==qans);
      return qans;
   }
}

Real Robot::computeThirdERSLink(int curlink, const ColumnVector & Pobj, const int endlink, const ColumnVector& Plink, bool invert, Real min, Real max, bool & converge) {
   //We'll compute the knee angle first, using the
   //  length of the thigh
   //  distance from knee (curlink) to Plink
   //  distance from shoulder (previous link) to Pobj
   //use law of cosines to find angle at knee of Pobj and Plink, and the difference is the amount to move
   Matrix Rp;
   ColumnVector pp;
   convertFrame(Rp,pp,curlink-1,curlink);
   //cout << "Rp:\n" << Rp;
   //cout << "pp:\n" << pp;
   Real previous_to_cur_len=sqrt(pp(1)*pp(1)+pp(2)*pp(2));
   //cout << "previous_to_cur_len==" << previous_to_cur_len <<endl;
   ColumnVector pl=convertLink(endlink,curlink)*Plink;
   //cout << "pl:\n" << pl;
	 if(fabs(pl(1))<ITOL && fabs(pl(2))<ITOL) {
		 //Plink is along axis of rotation - nothing we do is going to move it, so don't move at all
		 converge=true; //debatable
		 return links[curlink].get_q();
	 }
   Real tl=atan2(pl(2),pl(1));
   Real tp=atan2(pp(2),pp(1));
   //cout << "tl==" << tl << "  tp==" << tp << endl;
   if(Plink(4)==0) {
      //We're dealing with an infinite ray
      //disp('out of reach (positive infinity)');
		 Real qtgt=normalize_angle(tp-tl);
		 Real qans=limit_angle(qtgt,min,max);
		 converge=(qtgt==qans);
		 return qans;
   } else {
      //Real cur_to_plink_xyz_len=homog_norm(pl);
      Real plz=pl(3)/pl(4);
      pl(3)=0;
      //cout << "plz==" << plz << endl;
      //cout << "pl:\n" << pl;
      Real cur_to_plink_len=homog_norm(pl);
      //cout << "cur_to_plink_len==" << cur_to_plink_len << endl;
      ColumnVector prev_to_pobj=convertFrame(0,curlink-1)*Pobj;
      //cout << "prev_to_pobj\n" << prev_to_pobj;
      Real prev_to_pobj_xyz_len=homog_norm(prev_to_pobj);
      //cout << "prev_to_pobj_xyz_len==" << prev_to_pobj_xyz_len << endl;
      prev_to_pobj(3)=0;
      //cout << "prev_to_pobj\n" << prev_to_pobj;
			if(plz>prev_to_pobj_xyz_len) {
				//Pobj is too close to center of rotation - fold up
				converge=false;
				return limit_angle(normalize_angle(tp-tl),min,max);
			}
			Real tgt_len=sqrt(prev_to_pobj_xyz_len*prev_to_pobj_xyz_len-plz*plz);
			//cout << "tgt_len==" << tgt_len << endl;
			Real aor_d=(2*cur_to_plink_len*previous_to_cur_len);
			//cout << "aor_d==" << aor_d << endl;
			//have to check if Pobj is within reach
			if(fabs(aor_d)<=ITOL) {
				//Plink is along axis of rotation - nothing we do is going to move it, so don't move at all
				//this should never be seen...
				cout << "ASSERTION FAILED: " << __FILE__ << ':' << __LINE__ << endl;
				converge=false;
				return links[curlink].get_q();
			} else {
				Real aor=(cur_to_plink_len*cur_to_plink_len+previous_to_cur_len*previous_to_cur_len-tgt_len*tgt_len)/aor_d;
				//cout << "aor=="<<aor<<endl;
				if(aor<-1) { //Pobj is too far away - straighten out
					//disp('out of reach (negative)');
					converge=false;
					return limit_angle(normalize_angle(M_PI+tp-tl),min,max);
				} else if(aor>1) { //Pobj is too close to center of rotation - fold up
					//disp('out of reach (positive)');
					converge=false;
					return limit_angle(normalize_angle(tp-tl),min,max);
				} else {
					Real ao=-acos(aor);
					//cout << "ao=="<<ao<<endl;
					if(invert)
						ao=-ao;
					//cout << "ao=="<<ao<<endl;
					Real qtgt=normalize_angle(ao+tp-tl);
					//cout << "qtgt=="<<qtgt<<endl;
					Real qans=limit_angle(qtgt,min,max);
					//cout << "qans=="<<qans<<endl;
					converge=(qtgt==qans);
					return qans;
				}
			}
   }
}



// ----------------- M O D I F I E D  D H  N O T A T I O N ------------------


ReturnMatrix mRobot::inv_kin(const Matrix & Tobj, const int mj)
//!  @brief Overload inv_kin function.
{
   bool converge = false;
   return inv_kin(Tobj, mj, dof, converge);
}


ReturnMatrix mRobot::inv_kin(const Matrix & Tobj, const int mj, const int endlink, bool & converge)
/*!
  @brief Inverse kinematics solutions.

  The solution is based on the analytic inverse kinematics if robot type (familly)
  is Rhino or Puma, otherwise used the numerical algoritm defined in Robot_basic
  class.
*/
{
    switch (robotType) {
	case RHINO:
	    return inv_kin_rhino(Tobj, converge);
	    break;
	case PUMA:
	    return inv_kin_puma(Tobj, converge);
	    break;
	default:
	    return Robot_basic::inv_kin(Tobj, mj, endlink, converge);
    }
}


ReturnMatrix mRobot::inv_kin_rhino(const Matrix & Tobj, bool & converge)
/*!
  @brief Analytic Rhino inverse kinematics.

  converge will be false if the desired end effector pose is outside robot range.
*/
{
    ColumnVector qout(5), q_actual;    
    q_actual = get_q();

    try
    {
	Real theta[6] , diff1, diff2, tmp,
	     angle , L=0.0 , M=0.0 , K=0.0 , H=0.0 , Gl=0.0 ;

	if (links[6].d > 0)
	{
	    ColumnVector tmpd6(3); 
	    tmpd6(1)=0; tmpd6(2)=0; tmpd6(3)=links[6].d;
	    tmpd6 = Tobj.SubMatrix(1,3,1,3)*tmpd6;
	    Tobj.SubMatrix(1,3,4,4) = Tobj.SubMatrix(1,3,4,4) - tmpd6;
	}
    
	// Calcul les deux angles possibles
	theta[0] = atan2(Tobj(2,4),
			 Tobj(1,4));
	
	theta[1] = atan2(-Tobj(2,4),
			 -Tobj(1,4))  ;
	
	diff1 = fabs(q_actual(1)-theta[0]) ;		
	if (diff1 > M_PI)
	    diff1 = 2*M_PI - diff1;
	
	diff2 = fabs(q_actual(1)-theta[1]);
	if (diff2 > M_PI)
	    diff1 = 2*M_PI - diff2 ;
	
	// Prend l'angle le plus proche de sa position actuel
	if (diff1 < diff2)		   
	    theta[1] = theta[0] ;
	
	theta[5] = atan2(sin(theta[1])*Tobj(1,1) - cos(theta[1])*Tobj(2,1), 
			 sin(theta[1])*Tobj(1,2) - cos(theta[1])*Tobj(2,2));
	
	// angle = theta1 +theta2 + theta3
	angle = atan2(-1*cos(theta[1])*Tobj(1,3) - sin(theta[1])*Tobj(2,3),
		      -1*Tobj(3,3));
	
	L = cos(theta[1])*Tobj(1,4) + 
	    sin(theta[1])*Tobj(2,4) + 
	    links[5].d*sin(angle) - 
	    links[5].a*cos(angle);
	M = links[1].d - 
	    Tobj(3,4) - 
	    links[5].d*cos(angle) - 
	    links[5].a*sin(angle);
	K = (L*L + M*M - links[4].a*links[4].a - 
	     links[3].a*links[3].a) / 
	    (2 * links[4].a * links[4].a);
	
	tmp = 1-K*K;
	if (tmp < 0)
	    throw std::out_of_range("sqrt of negative number not allowed.");

	theta[0] = atan2( sqrt(tmp) , K );
	theta[3] = atan2( -sqrt(tmp) , K );	
	
	diff1 = fabs(q_actual(3)-theta[0]) ;
	if (diff1 > M_PI)
	    diff1 = 2*M_PI - diff1 ;
	
	diff2 = fabs(q_actual(3)-theta[3]);
	if (diff2 > M_PI)
	    diff1 = 2*M_PI - diff2 ;
	
	if (diff1 < diff2)
	    theta[3] = theta[0] ;
	
	H = cos(theta[3]) * links[4].a + links[3].a;
	Gl = sin(theta[3]) * links[4].a;
	
	theta[2] = atan2( M , L ) - atan2( Gl , H );
	theta[4] = atan2( -1*cos(theta[1])*Tobj(1,3) - sin(theta[1])*Tobj(2,3) , 
			  -1*Tobj(3,3)) - theta[2] - theta[3] ;
	
	qout(1) = theta[1];
	qout(2) = theta[2];
	qout(3) = theta[3];
	qout(4) = theta[4];
	qout(5) = theta[5];
	set_q(qout);
	
	converge = true;
    }
    catch(std::out_of_range & e)
    {
	converge = false; 
	set_q(q_actual);
	qout = q_actual;
    }

    qout.Release();
    return qout;
}


ReturnMatrix mRobot::inv_kin_puma(const Matrix & Tobj, bool & converge)
/*!
  @brief Analytic Puma inverse kinematics.

  converge will be false if the desired end effector pose is outside robot range.
*/
{
    ColumnVector qout(6), q_actual;
    q_actual = get_q();

    try
    {  
	Real theta[7] , diff1, diff2, tmp,
	     A = 0.0 , B = 0.0 , Cl = 0.0 , D =0.0, Ro = 0.0,
	     H = 0.0 , L = 0.0 , M = 0.0;

// Removed d6 component because in the Puma inverse kinematics solution 
// d6 = 0. 
	if (links[6].d)
	{
	    ColumnVector tmpd6(3);
	    tmpd6(1)=0; tmpd6(2)=0; tmpd6(3)=links[6].d;
	    tmpd6 = Tobj.SubMatrix(1,3,1,3)*tmpd6;
	    Tobj.SubMatrix(1,3,4,4) = Tobj.SubMatrix(1,3,4,4) - tmpd6;
	}
	
	tmp = Tobj(2,4)*Tobj(2,4) + Tobj(1,4)*Tobj(1,4);
	if (tmp < 0)
	    throw std::out_of_range("sqrt of negative number not allowed.");

	Ro = sqrt(tmp);
	D = (links[2].d+links[3].d) / Ro;
	
	tmp = 1-D*D;
	if (tmp < 0)
	    throw std::out_of_range("sqrt of negative number not allowed.");
	
	//Calcul les deux angles possibles
	theta[0] =  atan2(Tobj(2,4),Tobj(1,4)) - atan2(D, sqrt(tmp));	 
	//Calcul les deux angles possibles
	theta[1] =  atan2(Tobj(2,4),Tobj(1,4)) - atan2(D , -sqrt(tmp));
	
	diff1 = fabs(q_actual(1)-theta[0]);
	if (diff1 > M_PI)
	    diff1 = 2*M_PI - diff1;
	
	diff2 = fabs(q_actual(1)-theta[1]);
	if (diff2 > M_PI)
	    diff1 = 2*M_PI - diff2;
	
	// Prend l'angle le plus proche de sa position actuel
	if (diff1 < diff2)
	    theta[1] = theta[0];
	
	tmp = links[4].a*links[4].a + links[4].d*links[4].d;
	if (tmp < 0)
	    throw std::out_of_range("sqrt of negative number not allowed.");
	
	Ro = sqrt(tmp);
	B = atan2(links[4].d,links[4].a);
	Cl = Tobj(1,4)*Tobj(1,4) + 
	     Tobj(2,4)*Tobj(2,4) + 
	     Tobj(3,4)*Tobj(3,4) - 
	     (links[2].d + links[3].d)*(links[2].d + links[3].d) - 
	     links[3].a*links[3].a - 
	     links[4].a*links[4].a - 
	     links[4].d*links[4].d; 
	A = Cl / (2*links[3].a);
	
	tmp = 1-A/Ro*A/Ro;
	if (tmp < 0)
	    throw std::out_of_range("sqrt of negative number not allowed.");
	
	theta[0] = atan2(sqrt(tmp) , A/Ro) + B;
	theta[3] = atan2(-sqrt(tmp) , A/Ro) + B;
	
	diff1 = fabs(q_actual(3)-theta[0]);
	if (diff1 > M_PI)
	    diff1 = 2*M_PI - diff1 ;
	
	diff2 = fabs(q_actual(3)-theta[3]);
	if (diff2 > M_PI)
	    diff1 = 2*M_PI - diff2;
	
	//Prend l'angle le plus proche de sa position actuel
	if (diff1 < diff2)
	    theta[3] = theta[0];
	
	H = cos(theta[1])*Tobj(1,4) + sin(theta[1])*Tobj(2,4);
	L = sin(theta[3])*links[4].d + cos(theta[3])*links[4].a + links[3].a;
	M = cos(theta[3])*links[4].d - sin(theta[3])*links[4].a;
	
	theta[2] = atan2( M , L ) - atan2(Tobj(3,4) , H );
	
	theta[0] = atan2( -sin(theta[1])*Tobj(1,3) + cos(theta[1])*Tobj(2,3) , 
			  cos(theta[2] + theta[3]) * 
			  (cos(theta[1]) * Tobj(1,3) + sin(theta[1])*Tobj(2,3)) 
			  - (sin(theta[2]+theta[3])*Tobj(3,3)) );
	
	theta[4] = atan2(-1*(-sin(theta[1])*Tobj(1,3) + cos(theta[1])*Tobj(2,3)), 
			 -cos(theta[2] + theta[3]) * 
			 (cos(theta[1]) * Tobj(1,3) + sin(theta[1])*Tobj(2,3)) 
			 + (sin(theta[2]+theta[3])*Tobj(3,3)) );
	
	diff1 = fabs(q_actual(4)-theta[0]);
	if (diff1 > M_PI)
	    diff1 = 2*M_PI - diff1;
	
	diff2 = fabs(q_actual(4)-theta[4]);
	if (diff2 > M_PI)
	    diff1 = 2*M_PI - diff2;
	
	// Prend l'angle le plus proche de sa position actuel
	if (diff1 < diff2)
	    theta[4] = theta[0];
	
	theta[5] = atan2( cos(theta[4]) * 
			  ( cos(theta[2] + theta[3]) * 
			    (cos(theta[1]) * Tobj(1,3) 
			     + sin(theta[1])*Tobj(2,3)) 
			    - (sin(theta[2]+theta[3])*Tobj(3,3)) ) + 
			  sin(theta[4])*(-sin(theta[1])*Tobj(1,3) 
					 + cos(theta[1])*Tobj(2,3)) , 
			  sin(theta[2]+theta[3]) * (cos(theta[1]) * Tobj(1,3) 
						    + sin(theta[1])*Tobj(2,3) ) 
			  + (cos(theta[2]+theta[3])*Tobj(3,3)) );
	
	theta[6] = atan2( -sin(theta[4]) 
			  * ( cos(theta[2] + theta[3]) * 
			      (cos(theta[1]) * Tobj(1,1) 
			       + sin(theta[1])*Tobj(2,1)) 
			      - (sin(theta[2]+theta[3])*Tobj(3,1))) + 
			  cos(theta[4])*(-sin(theta[1])*Tobj(1,1) 
					 + cos(theta[1])*Tobj(2,1)), 
			  -sin(theta[4]) * ( cos(theta[2] + theta[3]) * 
					     (cos(theta[1]) * Tobj(1,2) 
					      + sin(theta[1])*Tobj(2,2)) 
					     - (sin(theta[2]+theta[3])*Tobj(3,2))) +
			  cos(theta[4])*(-sin(theta[1])*Tobj(1,2) 
					 + cos(theta[1])*Tobj(2,2)) );
	
	qout(1) = theta[1];
	qout(2) = theta[2];
	qout(3) = theta[3];
	qout(4) = theta[4];
	qout(5) = theta[5];
	qout(6) = theta[6];
	set_q(qout);
	
	converge = true; 
    }
    catch(std::out_of_range & e)
    {
	converge = false; 
	set_q(q_actual);
	qout = q_actual;
    }

    qout.Release();
    return qout;
}


ReturnMatrix mRobot_min_para::inv_kin(const Matrix & Tobj, const int mj)
//!  @brief Overload inv_kin function.
{
   bool converge = false;
   return inv_kin(Tobj, mj, dof, converge);
}


ReturnMatrix mRobot_min_para::inv_kin(const Matrix & Tobj, const int mj, const int endlink, bool & converge)
/*!
  @brief Inverse kinematics solutions.

  The solution is based on the analytic inverse kinematics if robot type (familly)
  is Rhino or Puma, otherwise used the numerical algoritm defined in Robot_basic
  class.
*/
{
    switch (robotType) {
	case RHINO:
	    return inv_kin_rhino(Tobj, converge);
	    break;
	case PUMA:
	    return inv_kin_puma(Tobj, converge);
	    break;
	default:
	    return Robot_basic::inv_kin(Tobj, mj, endlink, converge);
    }
}


ReturnMatrix mRobot_min_para::inv_kin_rhino(const Matrix & Tobj, bool & converge)
/*!
  @brief Analytic Rhino inverse kinematics.

  converge will be false if the desired end effector pose is outside robot range.
*/
{
    ColumnVector qout(5), q_actual;    
    q_actual = get_q();

    try
    {
	Real theta[6] , diff1, diff2, tmp,
	     angle , L=0.0 , M=0.0 , K=0.0 , H=0.0 , Gl=0.0 ;
    
	// Calcul les deux angles possibles
	theta[0] = atan2(Tobj(2,4),
			 Tobj(1,4));
	
	theta[1] = atan2(-Tobj(2,4),
			 -Tobj(1,4))  ;
	
	diff1 = fabs(q_actual(1)-theta[0]) ;		
	if (diff1 > M_PI)
	    diff1 = 2*M_PI - diff1;
	
	diff2 = fabs(q_actual(1)-theta[1]);
	if (diff2 > M_PI)
	    diff1 = 2*M_PI - diff2 ;
	
	// Prend l'angle le plus proche de sa position actuel
	if (diff1 < diff2)		   
	    theta[1] = theta[0] ;
	
	theta[5] = atan2(sin(theta[1])*Tobj(1,1) - cos(theta[1])*Tobj(2,1), 
			 sin(theta[1])*Tobj(1,2) - cos(theta[1])*Tobj(2,2));
	
	// angle = theta1 +theta2 + theta3
	angle = atan2(-1*cos(theta[1])*Tobj(1,3) - sin(theta[1])*Tobj(2,3),
		      -1*Tobj(3,3));
	
	L = cos(theta[1])*Tobj(1,4) + 
	    sin(theta[1])*Tobj(2,4) + 
	    links[5].d*sin(angle) - 
	    links[5].a*cos(angle);
	M = links[1].d - 
	    Tobj(3,4) - 
	    links[5].d*cos(angle) - 
	    links[5].a*sin(angle);
	K = (L*L + M*M - links[4].a*links[4].a - 
	     links[3].a*links[3].a) / 
	    (2 * links[4].a * links[4].a);
	
	tmp = 1-K*K;
	if (tmp < 0)
	    throw std::out_of_range("sqrt of negative number not allowed.");

	theta[0] = atan2( sqrt(tmp) , K );
	theta[3] = atan2( -sqrt(tmp) , K );	
	
	diff1 = fabs(q_actual(3)-theta[0]) ;
	if (diff1 > M_PI)
	    diff1 = 2*M_PI - diff1 ;
	
	diff2 = fabs(q_actual(3)-theta[3]);
	if (diff2 > M_PI)
	    diff1 = 2*M_PI - diff2 ;
	
	if (diff1 < diff2)
	    theta[3] = theta[0] ;
	
	H = cos(theta[3]) * links[4].a + links[3].a;
	Gl = sin(theta[3]) * links[4].a;
	
	theta[2] = atan2( M , L ) - atan2( Gl , H );
	theta[4] = atan2( -1*cos(theta[1])*Tobj(1,3) - sin(theta[1])*Tobj(2,3) , 
			  -1*Tobj(3,3)) - theta[2] - theta[3] ;
	
	qout(1) = theta[1];
	qout(2) = theta[2];
	qout(3) = theta[3];
	qout(4) = theta[4];
	qout(5) = theta[5];
	set_q(qout);
	
	converge = true;
    }
    catch(std::out_of_range & e)
    {
	converge = false; 
	set_q(q_actual);
	qout = q_actual;
    }

    qout.Release();
    return qout;
}


ReturnMatrix mRobot_min_para::inv_kin_puma(const Matrix & Tobj, bool & converge)
/*!
  @brief Analytic Puma inverse kinematics.

  converge will be false if the desired end effector pose is outside robot range.
*/
{
    ColumnVector qout(6), q_actual;
    q_actual = get_q();

    try
    {  
	Real theta[7] , diff1, diff2, tmp,
	     A = 0.0 , B = 0.0 , Cl = 0.0 , D =0.0, Ro = 0.0,
	     H = 0.0 , L = 0.0 , M = 0.0;

// Removed d6 component because in the Puma inverse kinematics solution 
// d6 = 0. 
	if (links[6].d > 0)
	{
	    ColumnVector tmpd6(3);
	    tmpd6(1)=0; tmpd6(2)=0; tmpd6(3)=links[6].d;
	    tmpd6 = Tobj.SubMatrix(1,3,1,3)*tmpd6;
	    Tobj.SubMatrix(1,3,4,4) = Tobj.SubMatrix(1,3,4,4) - tmpd6;
	}
	
	tmp = Tobj(2,4)*Tobj(2,4) + Tobj(1,4)*Tobj(1,4);
	if (tmp < 0)
	    throw std::out_of_range("sqrt of negative number not allowed.");

	Ro = sqrt(tmp);
	D = (links[2].d+links[3].d) / Ro;
	
	tmp = 1-D*D;
	if (tmp < 0)
	    throw std::out_of_range("sqrt of negative number not allowed.");
	
	//Calcul les deux angles possibles
	theta[0] =  atan2(Tobj(2,4),Tobj(1,4)) - atan2(D, sqrt(tmp));	 
	//Calcul les deux angles possibles
	theta[1] =  atan2(Tobj(2,4),Tobj(1,4)) - atan2(D , -sqrt(tmp));
	
	diff1 = fabs(q_actual(1)-theta[0]);
	if (diff1 > M_PI)
	    diff1 = 2*M_PI - diff1;
	
	diff2 = fabs(q_actual(1)-theta[1]);
	if (diff2 > M_PI)
	    diff1 = 2*M_PI - diff2;
	
	// Prend l'angle le plus proche de sa position actuel
	if (diff1 < diff2)
	    theta[1] = theta[0];
	
	tmp = links[4].a*links[4].a + links[4].d*links[4].d;
	if (tmp < 0)
	    throw std::out_of_range("sqrt of negative number not allowed.");
	
	Ro = sqrt(tmp);
	B = atan2(links[4].d,links[4].a);
	Cl = Tobj(1,4)*Tobj(1,4) + 
	     Tobj(2,4)*Tobj(2,4) + 
	     Tobj(3,4)*Tobj(3,4) - 
	     (links[2].d + links[3].d)*(links[2].d + links[3].d) - 
	     links[3].a*links[3].a - 
	     links[4].a*links[4].a - 
	     links[4].d*links[4].d; 
	A = Cl / (2*links[3].a);
	
	tmp = 1-A/Ro*A/Ro;
	if (tmp < 0)
	    throw std::out_of_range("sqrt of negative number not allowed.");
	
	theta[0] = atan2(sqrt(tmp) , A/Ro) + B;
	theta[3] = atan2(-sqrt(tmp) , A/Ro) + B;
	
	diff1 = fabs(q_actual(3)-theta[0]);
	if (diff1 > M_PI)
	    diff1 = 2*M_PI - diff1 ;
	
	diff2 = fabs(q_actual(3)-theta[3]);
	if (diff2 > M_PI)
	    diff1 = 2*M_PI - diff2;
	
	//Prend l'angle le plus proche de sa position actuel
	if (diff1 < diff2)
	    theta[3] = theta[0];
	
	H = cos(theta[1])*Tobj(1,4) + sin(theta[1])*Tobj(2,4);
	L = sin(theta[3])*links[4].d + cos(theta[3])*links[4].a + links[3].a;
	M = cos(theta[3])*links[4].d - sin(theta[3])*links[4].a;
	
	theta[2] = atan2( M , L ) - atan2(Tobj(3,4) , H );
	
	theta[0] = atan2( -sin(theta[1])*Tobj(1,3) + cos(theta[1])*Tobj(2,3) , 
			  cos(theta[2] + theta[3]) * 
			  (cos(theta[1]) * Tobj(1,3) + sin(theta[1])*Tobj(2,3)) 
			  - (sin(theta[2]+theta[3])*Tobj(3,3)) );
	
	theta[4] = atan2(-1*(-sin(theta[1])*Tobj(1,3) + cos(theta[1])*Tobj(2,3)), 
			 -cos(theta[2] + theta[3]) * 
			 (cos(theta[1]) * Tobj(1,3) + sin(theta[1])*Tobj(2,3)) 
			 + (sin(theta[2]+theta[3])*Tobj(3,3)) );
	
	diff1 = fabs(q_actual(4)-theta[0]);
	if (diff1 > M_PI)
	    diff1 = 2*M_PI - diff1;
	
	diff2 = fabs(q_actual(4)-theta[4]);
	if (diff2 > M_PI)
	    diff1 = 2*M_PI - diff2;
	
	// Prend l'angle le plus proche de sa position actuel
	if (diff1 < diff2)
	    theta[4] = theta[0];
	
	theta[5] = atan2( cos(theta[4]) * 
			  ( cos(theta[2] + theta[3]) * 
			    (cos(theta[1]) * Tobj(1,3) 
			     + sin(theta[1])*Tobj(2,3)) 
			    - (sin(theta[2]+theta[3])*Tobj(3,3)) ) + 
			  sin(theta[4])*(-sin(theta[1])*Tobj(1,3) 
					 + cos(theta[1])*Tobj(2,3)) , 
			  sin(theta[2]+theta[3]) * (cos(theta[1]) * Tobj(1,3) 
						    + sin(theta[1])*Tobj(2,3) ) 
			  + (cos(theta[2]+theta[3])*Tobj(3,3)) );
	
	theta[6] = atan2( -sin(theta[4]) 
			  * ( cos(theta[2] + theta[3]) * 
			      (cos(theta[1]) * Tobj(1,1) 
			       + sin(theta[1])*Tobj(2,1)) 
			      - (sin(theta[2]+theta[3])*Tobj(3,1))) + 
			  cos(theta[4])*(-sin(theta[1])*Tobj(1,1) 
					 + cos(theta[1])*Tobj(2,1)), 
			  -sin(theta[4]) * ( cos(theta[2] + theta[3]) * 
					     (cos(theta[1]) * Tobj(1,2) 
					      + sin(theta[1])*Tobj(2,2)) 
					     - (sin(theta[2]+theta[3])*Tobj(3,2))) +
			  cos(theta[4])*(-sin(theta[1])*Tobj(1,2) 
					 + cos(theta[1])*Tobj(2,2)) );
	
	qout(1) = theta[1];
	qout(2) = theta[2];
	qout(3) = theta[3];
	qout(4) = theta[4];
	qout(5) = theta[5];
	qout(6) = theta[6];
	set_q(qout);
	
	converge = true; 
    }
    catch(std::out_of_range & e)
    {
	converge = false; 
	set_q(q_actual);
	qout = q_actual;
    }

    qout.Release();
    return qout;
}

#ifdef use_namespace
}
#endif
