#include "Kinematics.h"
#include "Shared/Config.h"
#include "roboop/robot.h"
#include "roboop/config.h"
#include "Shared/WorldState.h"
#include <sstream>
#include <float.h>
#include <iostream>

using namespace std;

Kinematics * kine = NULL;
ROBOOP::Config * Kinematics::roconfig = NULL;
Kinematics::InterestPointMap * Kinematics::ips = NULL;
std::vector< std::vector<unsigned int> > Kinematics::chainMaps;
Kinematics::JointMap Kinematics::jointMaps[NumReferenceFrames];
bool Kinematics::staticsInited=false;
#ifndef THREADSAFE_KINEMATICS
std::vector<ROBOOP::Robot*> Kinematics::chains;
#endif

void
Kinematics::init() {
#ifdef THREADSAFE_KINEMATICS
	unsigned int nchains=::config->motion.kinematic_chains.size();
	chains.resize(nchains);
	for(unsigned int i=0; i<nchains; i++) {
		chains[i]=newChain(i);
		//cout << "Created " << chains[i] << " for " << this << endl;
	}
#endif
	checkStatics();
}

void Kinematics::initStatics() {
	if(!staticsInited) {
		unsigned int nchains=::config->motion.kinematic_chains.size();
		if(nchains==0) {
			serr->printf("ERROR Kinematics::init(): no kinematic chains were selected\n");
			return;
		}
		jointMaps[BaseFrameOffset]=JointMap(0,0);
		
#ifndef THREADSAFE_KINEMATICS
		chains.resize(nchains);
		for(unsigned int i=0; i<nchains; i++) {
			chains[i]=newChain(i);
			//cout << "Created " << chains[i] << " for " << this << endl;
		}
#endif
		
		if(ips==NULL) {
			int numIP=0;
			roconfig->select_int("InterestPoints","Length",numIP);
			ips=new InterestPointMap(numIP);
			pair<string,InterestPoint> ip;
			string chain;
			for(int i=1; i<=numIP; i++) {
				char section[100];
				snprintf(section,100,"InterestPoint%d",i);
				roconfig->select_float(section,"x",ip.second.x);
				roconfig->select_float(section,"y",ip.second.y);
				roconfig->select_float(section,"z",ip.second.z);
				roconfig->select_string(section,"chain",chain);
				roconfig->select_int(section,"link",ip.second.link);
				roconfig->select_string(section,"name",ip.first);
				ip.first.erase(ip.first.find_last_not_of('~')+1); // strip old-style '~' padding at end of output names
				for(ip.second.chain=0; ip.second.chain<(::config->motion.kinematic_chains.size()); ip.second.chain++)
					if(::config->motion.kinematic_chains[ip.second.chain]==chain)
						break;
				if(ip.second.chain==::config->motion.kinematic_chains.size())
					serr->printf("WARNING: Chain %s is not recognized for interest point %d\n",chain.c_str(),i);
				else
					ips->insert(ip);
			}
		}
		staticsInited=true;
#ifndef THREADSAFE_KINEMATICS
		delete roconfig; // static chains are initialized, don't need to keep this around...
#endif
	}
	/*cout << "Joint Maps" << endl;
	for(unsigned int i=0; i<NumOutputs; i++)
		cout << outputNames[i] << ": " << jointMaps[i].chain << ' ' << jointMaps[i].link << endl;
	for(unsigned int i=NumOutputs; i<NumReferenceFrames; i++)
		cout << i << ": " << jointMaps[i].chain << ' ' << jointMaps[i].link << endl;
	cout << "Chain Maps" << endl;
	for(unsigned int i=0; i<chains.size(); i++)
		for(unsigned int j=1; j<chainMaps[i].size(); j++)
		cout << "chainMaps["<<i<<"]["<<j<<"] == " << chainMaps[i][j] << endl;*/
}

ROBOOP::Robot* Kinematics::newChain(unsigned int chainIdx) {
	if(roconfig==NULL) {
		unsigned int nchains=::config->motion.kinematic_chains.size();
		chainMaps.resize(nchains);
		roconfig=new ROBOOP::Config(::config->motion.makePath(::config->motion.kinematics),true);
		for(unsigned int i=0; i<nchains; i++) {
			string section=::config->motion.kinematic_chains[i];
			int dof=0;
			if(roconfig->select_int(section,"dof",dof)!=0) {
				serr->printf("ERROR Kinematics::init(): unable to find 'dof' in chain %d (%s)\n",i,section.c_str());
				chainMaps[i].resize(0);
				continue;
			}
			chainMaps[i].resize(dof+1);
			for(int l=0; l<=dof; l++)
				chainMaps[i][l]=-1U;
			for(int l=1; l<=dof; l++) {
				ostringstream ostr;
				ostr << section << "_LINK" << l;
				string link = ostr.str();
				if(roconfig->parameter_exists(link,"tekkotsu_output")) {
					int tkout=-1U;
					roconfig->select_int(link,"tekkotsu_output",tkout);
					if((unsigned int)tkout>=NumOutputs) {
						serr->printf("WARNING Kinematics::init(): invalid tekkotsu_output %d on chain %d (%s), link %d (%s)\n",tkout,i,section.c_str(),l,link.c_str());
					} else {
						jointMaps[tkout]=JointMap(i,l);
						chainMaps[i][l]=tkout;
					}
				}
				if(roconfig->parameter_exists(link,"tekkotsu_frame")) {
					int tkout=-1U;
					roconfig->select_int(link,"tekkotsu_frame",tkout);
					tkout+=NumOutputs;
					if((unsigned int)tkout>=NumReferenceFrames)
						serr->printf("WARNING Kinematics::init(): invalid tekkotsu_frame %d on chain %d (%s), link %d (%s)\n",tkout,i,section.c_str(),l,link.c_str());
					else {
						jointMaps[tkout]=JointMap(i,l);
						chainMaps[i][l]=tkout;
					}
				}
			}
		}
	}
	if(chainMaps[chainIdx].size()==0)
		return NULL;
	return new ROBOOP::Robot(*roconfig,::config->motion.kinematic_chains[chainIdx]);
}

Kinematics::~Kinematics() {
#ifdef THREADSAFE_KINEMATICS
	for(unsigned int i=0; i<chains.size(); i++) {
		if(chains[i]==NULL)
			cerr << "Warning: Kinematics chains appear to already be deleted! (multiple free?)" << endl;
		delete chains[i];
		chains[i]=NULL;
	}
#endif
}

NEWMAT::ReturnMatrix
Kinematics::jointToBase(unsigned int j) {
#ifndef THREADSAFE_KINEMATICS
	checkStatics();
#endif
	unsigned int c=-1U,l=-1U;
	if(!lookup(j,c,l)) {
		NEWMAT::Matrix A(4,4);
		A<<ROBOOP::fourbyfourident;
		A.Release(); return A;
	}
	update(c,l);
	return chains[c]->convertFrame(l,0);
}

NEWMAT::ReturnMatrix
Kinematics::linkToBase(unsigned int j) {
#ifndef THREADSAFE_KINEMATICS
	checkStatics();
#endif
	unsigned int c=-1U,l=-1U;
	if(!lookup(j,c,l)) {
		NEWMAT::Matrix A(4,4);
		A<<ROBOOP::fourbyfourident;
		A.Release(); return A;
	}
	update(c,l);
	return chains[c]->convertLink(l,0);
}

NEWMAT::ReturnMatrix
Kinematics::baseToJoint(unsigned int j) {
#ifndef THREADSAFE_KINEMATICS
	checkStatics();
#endif
	unsigned int c=-1U,l=-1U;
	if(!lookup(j,c,l)) {
		NEWMAT::Matrix A(4,4);
		A<<ROBOOP::fourbyfourident;
		A.Release(); return A;
	}
	update(c,l);
	return chains[c]->convertFrame(0,l);
}

NEWMAT::ReturnMatrix
Kinematics::baseToLink(unsigned int j) {
#ifndef THREADSAFE_KINEMATICS
	checkStatics();
#endif
	unsigned int c=-1U,l=-1U;
	if(!lookup(j,c,l)) {
		NEWMAT::Matrix A(4,4);
		A<<ROBOOP::fourbyfourident;
		A.Release(); return A;
	}
	update(c,l);
	return chains[c]->convertLink(0,l);
}

NEWMAT::ReturnMatrix
Kinematics::jointToJoint(unsigned int ij, unsigned int oj) {
#ifndef THREADSAFE_KINEMATICS
	checkStatics();
#endif
	unsigned int ci=-1U,li=-1U,co=-1U,lo=-1U;
	if(ij==oj || !lookup(ij,ci,li) || !lookup(oj,co,lo)) {
		NEWMAT::Matrix A(4,4);
		A<<ROBOOP::fourbyfourident;
		A.Release(); return A;
	}
	if(ci==co) {
		update(ci,li>lo?li:lo);
		return chains[ci]->convertFrame(li,lo);
	} else if(li==0) {
		update(co,lo);
		return chains[co]->convertFrame(0,lo);
	} else if(lo==0) {
		update(ci,li);
		return chains[ci]->convertFrame(li,0);
	} else {
		update(ci,li);
		update(co,lo);
		NEWMAT::Matrix ans=chains[co]->convertFrame(0,lo)*chains[ci]->convertFrame(li,0);
		ans.Release(); return ans;
	}
}

NEWMAT::ReturnMatrix
Kinematics::jointToLink(unsigned int ij, unsigned int oj) {
#ifndef THREADSAFE_KINEMATICS
	checkStatics();
#endif
	unsigned int ci=-1U,li=-1U,co=-1U,lo=-1U;
	if(!lookup(ij,ci,li) || !lookup(oj,co,lo)) {
		NEWMAT::Matrix A(4,4);
		A<<ROBOOP::fourbyfourident;
		A.Release(); return A;
	}
	if(ci==co) {
		update(ci,li>lo?li:lo);
		return chains[ci]->convertFrameToLink(li,lo);
	} else if(li==0) {
		update(co,lo);
		return chains[co]->convertFrameToLink(0,lo);
	} else if(lo==0) {
		update(ci,li);
		return chains[ci]->convertFrameToLink(li,0);
	} else {
		update(ci,li);
		update(co,lo);
		NEWMAT::Matrix ans=chains[co]->convertLink(0,lo)*chains[ci]->convertFrame(li,0);
		ans.Release(); return ans;
	}
}

NEWMAT::ReturnMatrix
Kinematics::linkToJoint(unsigned int ij, unsigned int oj) {
#ifndef THREADSAFE_KINEMATICS
	checkStatics();
#endif
	unsigned int ci=-1U,li=-1U,co=-1U,lo=-1U;
	if(!lookup(ij,ci,li) || !lookup(oj,co,lo)) {
		NEWMAT::Matrix A(4,4);
		A<<ROBOOP::fourbyfourident;
		A.Release(); return A;
	}
	if(ci==co) {
		update(ci,li>lo?li:lo);
		return chains[ci]->convertLinkToFrame(li,lo);
	} else if(li==0) {
		update(co,lo);
		return chains[co]->convertLinkToFrame(0,lo);
	} else if(lo==0) {
		update(ci,li);
		return chains[ci]->convertLinkToFrame(li,0);
	} else {
		update(ci,li);
		update(co,lo);
		NEWMAT::Matrix ans=chains[co]->convertFrame(0,lo)*chains[ci]->convertLink(li,0);
		ans.Release(); return ans;
	}
}

NEWMAT::ReturnMatrix
Kinematics::linkToLink(unsigned int ij, unsigned int oj) {
#ifndef THREADSAFE_KINEMATICS
	checkStatics();
#endif
	unsigned int ci=-1U,li=-1U,co=-1U,lo=-1U;
	if(ij==oj || !lookup(ij,ci,li) || !lookup(oj,co,lo)) {
		NEWMAT::Matrix A(4,4);
		A<<ROBOOP::fourbyfourident;
		A.Release(); return A;
	}
	if(ci==co) {
		update(ci,li>lo?li:lo);
		return chains[ci]->convertLink(li,lo);
	} else if(li==0) {
		update(co,lo);
		return chains[co]->convertLink(0,lo);
	} else if(lo==0) {
		update(ci,li);
		return chains[ci]->convertLink(li,0);
	} else {
		update(ci,li);
		update(co,lo);
		NEWMAT::Matrix ans=chains[co]->convertLink(0,lo)*chains[ci]->convertLink(li,0);
		ans.Release(); return ans;
	}
}

void
Kinematics::getInterestPoint(const std::string& name, unsigned int& j, NEWMAT::Matrix& ip) {
#ifndef THREADSAFE_KINEMATICS
	checkStatics();
#endif
	unsigned int c=-1U,l=-1U;
	getInterestPoint(name,c,l,ip);
	j=chainMaps[c][l];
}

void
Kinematics::getInterestPoint(const std::string& name, unsigned int& c, unsigned int& l, NEWMAT::Matrix& ip) {
#ifndef THREADSAFE_KINEMATICS
	checkStatics();
#endif
	InterestPointMap::iterator it=ips->find(name);
	ip=NEWMAT::ColumnVector(4);
	if(it==ips->end()) {
		serr->printf("ERROR: '%s' is not a recognized interest point\n",name.c_str());
		ip=0;
		c=l=-1U;
	}
	c=(*it).second.chain;
	l=(*it).second.link;
	ip=pack((*it).second.x,(*it).second.y,(*it).second.z);
	//cout << ci << ' ' << li << ' ' << co << ' ' << lo << endl;
}

NEWMAT::ReturnMatrix
Kinematics::getJointInterestPoint(unsigned int joint, const std::string& name) {
#ifndef THREADSAFE_KINEMATICS
	checkStatics();
#endif
	NEWMAT::ColumnVector ans(4);
	ans=0;
	unsigned int co=-1U,lo=-1U;
	if(!lookup(joint,co,lo)) {
		ans.Release(); return ans;
	}
	for(std::string::size_type pos=0,len=0; pos<name.size(); pos+=len+1) {
		len=name.find(',',pos);
		if(len==string::npos)
			len=name.size();
		len-=pos;
		unsigned int ci=-1U,li=-1U;
		NEWMAT::ColumnVector ip;
		getInterestPoint(name.substr(pos,len),ci,li,ip);
		if(ci==-1U) {
			ip.Release(); return ip;
		}
		if(ci==co) {
			update(ci,li>lo?li:lo);
			ans+=chains[ci]->convertLinkToFrame(li,lo)*ip;
		} else if(li==0) {
			update(co,lo);
			ans+=chains[co]->convertLinkToFrame(0,lo)*ip;
		} else if(lo==0) {
			update(ci,li);
			ans+=chains[ci]->convertLinkToFrame(li,0)*ip;
		} else {
			update(ci,li);
			update(co,lo);
			ans+=chains[co]->convertFrame(0,lo)*chains[ci]->convertLink(li,0)*ip;
		}
	}
	ans/=ans(4); //not strictly necessary, but may save some people headaches
	ans.Release(); return ans;
}

#ifdef TGT_HAS_LEGS
void
Kinematics::calcLegHeights(const NEWMAT::ColumnVector& down, float heights[NumLegs]) {
#ifndef THREADSAFE_KINEMATICS
	checkStatics();
#endif
	//initialize to the height of the ball of the foot
	for(unsigned int i=0; i<NumLegs; i++) {
		NEWMAT::ColumnVector ip_b=jointToBase(PawFrameOffset+i).SubMatrix(1,3,4,4);
		float h=-ip_b(1)*down(1) - ip_b(2)*down(2) - ip_b(3)*down(3);
		h-=BallOfFootRadius; //add the ball's radius
		heights[i]=h;
	}
	//see if any interest points are lower
	for(InterestPointMap::const_iterator it=ips->begin(); it!=ips->end(); ++it) {
		unsigned int c=(*it).second.chain;
		LegOrder_t leg;
		if(config->motion.kinematic_chains[c]=="LFr")
			leg=LFrLegOrder;
		else if(config->motion.kinematic_chains[c]=="RFr")
			leg=RFrLegOrder;
		else if(config->motion.kinematic_chains[c]=="LBk")
			leg=LBkLegOrder;
		else if(config->motion.kinematic_chains[c]=="RBk")
			leg=RBkLegOrder;
		else
			continue;
		unsigned int l=(*it).second.link;
		NEWMAT::Matrix ip=pack((*it).second.x,(*it).second.y,(*it).second.z);
		update(c,l);
		NEWMAT::ColumnVector ip_b=chains[c]->convertLinkToFrame(l,0)*ip;
		float h=-ip_b(1)*down(1) - ip_b(2)*down(2) - ip_b(3)*down(3);
		if(h<heights[leg])
			heights[leg]=h;
	}
}

LegOrder_t
Kinematics::findUnusedLeg(const NEWMAT::ColumnVector& down) {
#ifndef THREADSAFE_KINEMATICS
	checkStatics();
#endif
	float heights[NumLegs];
	calcLegHeights(down,heights);
	//Find the highest foot
	unsigned int highleg=0;
	for(unsigned int i=1; i<NumLegs; i++)
		if(heights[i]>heights[highleg])
			highleg=i;
	//cout << "High: " << highleg << endl;
	return static_cast<LegOrder_t>(highleg);
}

NEWMAT::ReturnMatrix
Kinematics::calculateGroundPlane() {
	NEWMAT::ColumnVector down=pack(state->sensors[BAccelOffset],-state->sensors[LAccelOffset],state->sensors[DAccelOffset]);
	if(down.SumSquare()<1.01) //1 because homogenous coord is 1
		down=pack(0,0,-1); //default to a down vector if sensors don't give a significant indication of gravity
	return calculateGroundPlane(down);
}

// TODO comment out name tracking (and this following #include) once we're sure it's working
#include "PostureEngine.h"
NEWMAT::ReturnMatrix
Kinematics::calculateGroundPlane(const NEWMAT::ColumnVector& down) {
#ifndef THREADSAFE_KINEMATICS
	checkStatics();
#endif
	NEWMAT::Matrix lowip(3,3); //3 points define a plane
	float heights[3];
	unsigned int legs[3];
	std::string names[3];
	//initialize to max float
	for(unsigned int i=0; i<3; i++) {
		heights[i]=FLT_MAX;
		legs[i]=-1U;
	}
	//Check the balls of the foot
	for(unsigned int i=0; i<NumLegs; i++) {
		NEWMAT::ColumnVector ip_b=jointToBase(PawFrameOffset+i).SubMatrix(1,3,4,4);
		float h=-ip_b(1)*down(1) - ip_b(2)*down(2) - ip_b(3)*down(3);
		h-=BallOfFootRadius; //add the ball's radius
		if(h<heights[0]) {
			if(h<heights[1]) {
				heights[0]=heights[1];
				lowip.SubMatrix(1,3,1,1)=lowip.SubMatrix(1,3,2,2);
				legs[0]=legs[1];
				names[0]=names[1];
				if(h<heights[2]) {
					heights[1]=heights[2];
					lowip.SubMatrix(1,3,2,2)=lowip.SubMatrix(1,3,3,3);
					legs[1]=legs[2];
					names[1]=names[2];
					
					heights[2]=h;
					lowip.SubMatrix(1,3,3,3)=ip_b;
					legs[2]=i;
					names[2]="paw"; names[2]+=(char)('0'+i);
				} else {
					heights[1]=h;
					lowip.SubMatrix(1,3,2,2)=ip_b;
					legs[1]=i;
					names[1]="paw"; names[1]+=(char)('0'+i);
				}
			} else {
				heights[0]=h;
				lowip.SubMatrix(1,3,1,1)=ip_b;
				legs[0]=i;
				names[0]="paw"; names[0]+=(char)('0'+i);
			}
		}
	}
	//cout << "Ground plane initial: " << names[0] <<" ("<<heights[0]<<") " << names[1] << " ("<<heights[1]<<") " << names[2] << " ("<<heights[2]<<")"<< endl;
	
	//now check interest points
	for(InterestPointMap::const_iterator it=ips->begin(); it!=ips->end(); ++it) {
		if((*it).first.substr(0,3)=="Toe")
			continue;
		unsigned int c=(*it).second.chain;
		unsigned int l=(*it).second.link;
		NEWMAT::Matrix ip=pack((*it).second.x,(*it).second.y,(*it).second.z);
		update(c,l);
		NEWMAT::ColumnVector ip_b=chains[c]->convertLinkToFrame(l,0)*ip;
		float h=-ip_b(1)*down(1) - ip_b(2)*down(2) - ip_b(3)*down(3);
		if(h<heights[0]) {
			unsigned int leg;
			if(config->motion.kinematic_chains[c]=="LFr")
				leg=LFrLegOrder;
			else if(config->motion.kinematic_chains[c]=="RFr")
				leg=RFrLegOrder;
			else if(config->motion.kinematic_chains[c]=="LBk")
				leg=LBkLegOrder;
			else if(config->motion.kinematic_chains[c]=="RBk")
				leg=RBkLegOrder;
			else
				leg=-1U;
			
			if(h<heights[1]) {
				if(h<heights[2]) {
					if(leg==-1U || legs[1]!=leg && legs[2]!=leg) {
						heights[0]=heights[1];
						lowip.SubMatrix(1,3,1,1)=lowip.SubMatrix(1,3,2,2);
						legs[0]=legs[1];
						names[0]=names[1];
					}
					if(leg==-1U || legs[2]!=leg) {
						heights[1]=heights[2];
						lowip.SubMatrix(1,3,2,2)=lowip.SubMatrix(1,3,3,3);
						if(legs[2]!=leg)
							legs[1]=legs[2];
						names[1]=names[2];
					}
					
					heights[2]=h;
					lowip.SubMatrix(1,3,3,3)=ip_b.SubMatrix(1,3,1,1);
					legs[2]=leg;
					names[2]=(*it).first;
				} else {
					if(leg!=-1U && legs[2]==leg)
						continue;
					if(leg==-1U || legs[1]!=leg) {
						heights[0]=heights[1];
						lowip.SubMatrix(1,3,1,1)=lowip.SubMatrix(1,3,2,2);
						legs[0]=legs[1];
						names[0]=names[1];
					}
					heights[1]=h;
					lowip.SubMatrix(1,3,2,2)=ip_b.SubMatrix(1,3,1,1);
					legs[1]=leg;
					names[1]=(*it).first;
				}
			} else {
				if(leg!=-1U && (legs[1]==leg || legs[2]==leg))
					continue;
				heights[0]=h;
				lowip.SubMatrix(1,3,1,1)=ip_b.SubMatrix(1,3,1,1);
				legs[0]=leg;
				names[0]=(*it).first;
			}
		}
	}
	
	//Fit a plane to the remaining 3 feet
	NEWMAT::ColumnVector ones(3); ones=1;
	NEWMAT::ColumnVector p(4);
	try {
		p.SubMatrix(1,3,1,1) = lowip.t().i()*ones;
	} catch(...) {
		std::cout << "Exception during ground plane processing, saving current posture..." << std::flush;
		if(PostureEngine * tpose=dynamic_cast<PostureEngine*>(this)) {
			tpose->setSaveFormat(true,state);
			tpose->saveFile("/error.pos");
		} else {
			PostureEngine pose;
			pose.takeSnapshot();
			pose.setWeights(1);
			pose.setSaveFormat(true,state);
			pose.saveFile("/error.pos");
		}
		std::cout << "Wrote current sensor info \"error.pos\" on memstick" << std::endl;
		cout << "Ground plane was using " << names[0] <<" ("<<heights[0]<<") " << names[1] << " ("<<heights[1]<<") " << names[2] << " ("<<heights[2]<<")"<< endl;
		cout << lowip;
		throw;
	}
	p(4)=1;
	p.Release(); return p;
}

#else // NO LEGS -- constant ground plane

NEWMAT::ReturnMatrix
Kinematics::calculateGroundPlane() {
	return calculateGroundPlane(pack(0,0,-1));
}

NEWMAT::ReturnMatrix
Kinematics::calculateGroundPlane(const NEWMAT::ColumnVector& /*down*/) {
	return pack(0,0,1,0); // flat x-y plane through the origin
}

#endif

NEWMAT::ReturnMatrix
Kinematics::projectToPlane(unsigned int j, const NEWMAT::ColumnVector& r_j,
                           unsigned int b, const NEWMAT::ColumnVector& p_b,
                           unsigned int f)
{
	/*! Mathematical implementation:
	 *  
	 *  Need to convert @a p_b to @a p_j
	 *  
	 *  Once we have the transformation Tb_j from b to j, we need:\n
	 *    T2=inv(Tb_j)'; T2(3,1:end-1)=-T2(3,1:end-1);\n
	 *  but since we know a few things about the structure of T,
	 *  we don't have to explicitly calculate that inverse. */
	NEWMAT::Matrix T2=linkToLink(b,j);
	//cout << "Transform b->j:\n"<<T2;
	T2.SubMatrix(4,4,1,3)=T2.SubMatrix(1,3,4,4).t()*T2.SubMatrix(1,3,1,3);
	T2.SubMatrix(1,3,4,4)=0;
	//cout << "Transform plane b->j:\n"<<T2;
	NEWMAT::ColumnVector p_j=T2*p_b;
	//cout << "p_j:\n"<<p_j.t();

	
	/*! After we obtain @a p_j, we can find the point of intersection of
	 *  @a r_j and @a p_j using:
	 *  @f[ \frac{p_d}{p_{xyz} \cdot r}r @f]
	 *  Where @f$p_{xyz}@f$ is the first three elemnts of @a p_j, and
	 *  @f$p_d@f$ is the fourth (hopefully last) element of p_j.
	 *
	 *  Of course, if @f$p_{xyz} \cdot r@f$ is 0, then r and p are parallel
	 *  (since @a p_j is the normal of the plane, so a line perpendicular to
	 *  the normal is parallel to the plane), so we set the resulting
	 *  homogeneous coordinates accordingly to represent an interesection at
	 *  infinity. */

	float denom=0;
	for(unsigned int i=1; i<=3; i++)
		denom+=r_j(i)*p_j(i);
	NEWMAT::ColumnVector intersect=r_j;
	if(denom==0)
		intersect(4)=0;
	else {
		float s=p_j(4)/denom;
		for(unsigned int i=1; i<=3; i++)
			intersect(i)*=s;
		intersect(4)=1;
	}
	//cout << "Intersect_j:\n"<<intersect.t();
	NEWMAT::Matrix ans=linkToLink(j,f)*intersect;
	ans.Release(); return ans;
}

void
Kinematics::update(unsigned int c, unsigned int l) {
	for(unsigned int j=1; j<=l; j++) {
		unsigned int tkout=chainMaps[c][j];
		if(tkout<NumOutputs)
			chains[c]->set_q(state->outputs[tkout],j);
	}
}


/*! @file
 * @brief 
 * @author ejt (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.37 $
 * $State: Exp $
 * $Date: 2007/09/26 23:12:05 $
 */

