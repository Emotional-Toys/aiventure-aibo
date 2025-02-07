/*
Copyright (C) 2002-2004  Etienne Lachance

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

email: etienne.lachance@polymtl.ca or richard.gourdeau@polymtl.ca

-------------------------------------------------------------------------------
Revision_history:

2004/07/13: Ethan Tira-Thompson
    -Added support for newmat's use_namespace #define, using ROBOOP namespace
    -Using config::select_real instead of select_double
-------------------------------------------------------------------------------
*/

/*!
  @file control_select.cpp
  @brief Controller selection class.
*/

#include "control_select.h"

using namespace std;

#ifdef use_namespace
namespace ROBOOP {
  using namespace NEWMAT;
#endif
	//! @brief RCS/CVS version.
	static const char rcsid[] __UNUSED__ = "$Id: control_select.cpp,v 1.5 2007/11/11 23:57:24 ejt Exp $";

Control_Select::Control_Select()
//! @brief Constructor.
{
  type = NONE;
  space_type = NONE;
  dof = 0;
}

Control_Select::Control_Select(const string & filename)
/*!
  @brief Constructor.
  @param filename: configuration file (path+name).
*/
{
    set_control(filename);
}

Control_Select::Control_Select(const Control_Select & x)
//! @brief Copy constructor.
{
    type = x.type;
    space_type = x.space_type;
    dof = x.dof;
    pd = x.pd;
    ctm = x.ctm;
    rra = x.rra;
    impedance = x.impedance;
}

Control_Select & Control_Select::operator=(const Control_Select & x)
//! @brief Overload = operator.
{
    type = x.type;
    space_type = x.space_type;
    dof = x.dof;
    pd = x.pd;
    ctm = x.ctm;
    rra = x.rra;
    impedance = x.impedance;

    return *this;
}

int Control_Select::get_dof()
//! @brief Return the degree of freedom.
{ 
    return dof; 
}

void Control_Select::set_control(const string & filename)
//! @brief Select the proper controller from filename. 
{
    Config conf(filename);
    conf.read_conf();

    conf.select_string(CONTROLLER, "type", ControllerName);

    if (ControllerName == PROPORTIONAL_DERIVATIVE)
    {
	type = PD;
	space_type = JOINT_SPACE;
    }
    else if(ControllerName == COMPUTED_TORQUE_METHOD)
    {
	type = CTM;
	space_type = JOINT_SPACE;
    }
    else if(ControllerName == RESOLVED_RATE_ACCELERATION)
    {
	type = RRA;
	space_type = CARTESIAN_SPACE;
    }
    else if(ControllerName == IMPEDANCE)
    {
	type = IMP;
	space_type = CARTESIAN_SPACE;
    }
    else 
    {
	ControllerName = "";
	type = 0;
	space_type = 0;
    }
    
    conf.select_int(CONTROLLER, "dof", dof);

    switch (type) {
	case PD:
	{	    
	    pd = Proportional_Derivative(dof);
	    DiagonalMatrix Kp(dof), Kd(dof);
	    for(int i = 1; i <= dof; i++)
	    {
#ifdef __WATCOMC__
		ostrstream Kp_ostr, Kd_ostr;
		Kp_ostr << "Kp_" << i;
		string temp = Kp_ostr.str();
		temp[Kp_ostr.pcount()] = 0;
		conf.select_real("GAINS", temp.c_str(), Kp(i));
		Kd_ostr << "Kd_" << i;
		temp = Kd_ostr.str();
		temp[Kd_ostr.pcount()] = 0;
		conf.select_real("GAINS", temp.c_str(), Kd(i));
#else
		ostringstream Kp_ostr, Kd_ostr;
		Kp_ostr << "Kp_" << i;
		conf.select_real("GAINS", Kp_ostr.str(), Kp(i));
		Kd_ostr << "Kd_" << i;
		conf.select_real("GAINS", Kd_ostr.str(), Kd(i));
#endif
	    }
	    pd.set_Kp(Kp);
	    pd.set_Kd(Kd);	    
	}
	break;
	
	case CTM:
	{	    
	    ctm = Computed_torque_method(dof);
	    DiagonalMatrix Kp(dof), Kd(dof);
	    for(int i = 1; i <= dof; i++)
	    {
#ifdef __WATCOMC__
		ostrstream Kp_ostr, Kd_ostr;
		Kp_ostr << "Kp_" << i;
		string temp = Kp_ostr.str();
		temp[Kp_ostr.pcount()] = 0;
		conf.select_real("GAINS", temp.c_str(), Kp(i));
		Kd_ostr << "Kd_" << i;
		temp = Kd_ostr.str();
		temp[Kd_ostr.pcount()] = 0;
		conf.select_real("GAINS", temp.c_str(), Kd(i));
#else
		ostringstream Kp_ostr, Kd_ostr;
		Kp_ostr << "Kp_" << i;
		conf.select_real("GAINS", Kp_ostr.str(), Kp(i));
		Kd_ostr << "Kd_" << i;
		conf.select_real("GAINS", Kd_ostr.str(), Kd(i));
#endif
	    }
	    ctm.set_Kp(Kp);
	    ctm.set_Kd(Kd);
	}
	break;
	    
	case RRA:
	{
	    rra = Resolved_acc(dof);
	    Real Kvp, Kpp, Kvo, Kpo;
	    conf.select_real("GAINS", "Kvp", Kvp);
	    conf.select_real("GAINS", "Kpp", Kpp);
	    conf.select_real("GAINS", "Kvo", Kvo);
	    conf.select_real("GAINS", "Kpo", Kpo);
	    rra.set_Kvp( Kvp );
	    rra.set_Kpp( Kpp );
	    rra.set_Kvo( Kvo );
	    rra.set_Kpo( Kpo );
	}
	break;

	case IMP:
	{
	}
	break;
	
	default:
	    break;
    }
}

#ifdef use_namespace
}
#endif

