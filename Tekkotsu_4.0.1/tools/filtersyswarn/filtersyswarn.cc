#include <iostream>
#include <string>
#include <vector>
#include <regex.h>
#include <sys/types.h>
#include <cstdlib>

using namespace std;

//you can pass a replacement path to override OPENRSDK_ROOT

int main(int argc, const char** argv) {
	regex_t sysmsg, instbegin, inststat, ignorebegin, warnstat, inclbegin, inclstat;
	
	string OPENR="^";
	if(argc>1) {
		OPENR+=argv[1];
	} else {
		char* OPENRSDK_ROOT=getenv("OPENRSDK_ROOT");
		OPENR+=(OPENRSDK_ROOT!=NULL)?OPENRSDK_ROOT:"/usr/local/OPEN_R_SDK/";
	}
	OPENR="("+OPENR+"|Shared/newmat/)";
	string clip="  [...]";
	
	regcomp(&inclbegin,"^In file included from",REG_EXTENDED|REG_NOSUB);
	regcomp(&inclstat, "^                 from",REG_EXTENDED|REG_NOSUB);

	//string instbeginstr=".*: (At global scope:|In instantiation of |In method |In member function|In destructor |In constructor |In function |In static )";
	// problems with line wrapping on long paths... but does this filter too much now?

	string instbeginstr=".*: (At|In)";
	string inststatstr=instbeginstr+"|   instantiated from |^   ";
	regcomp(&sysmsg,OPENR.c_str(),REG_EXTENDED|REG_NOSUB);
	regcomp(&instbegin,instbeginstr.c_str(),REG_EXTENDED|REG_NOSUB);
	regcomp(&inststat,inststatstr.c_str(),REG_EXTENDED|REG_NOSUB);
	string pliststr_no_vdtor=".*/plist[a-zA-Z]*\\.h:[0-9]*: warning: base ";
	string dualcoding_no_vdtor=".*DualCoding/(PolygonData\\.cc|ShapeFuns\\.h):[0-9]*: warning: base ";
	regcomp(&ignorebegin,(OPENR+".*: warning: |"+pliststr_no_vdtor+"|"+dualcoding_no_vdtor+"|.*: Warning: Macro instruction expanded|.*: warning: suggest |.*local/terk/(TeRKPeerCommon|peer/MRPLPeer|SerialIO).h:.*: warning:").c_str(),REG_EXTENDED|REG_NOSUB);
	regcomp(&warnstat,"^   ",REG_EXTENDED|REG_NOSUB);

	string file, instant;

	string s;
	getline(cin,s);
	bool outfile=false,outinstant=false;
	bool fileclipped=false,instantclipped=false,warnclipped=false;
	while(cin) {
		if(regexec(&inclbegin,s.c_str(),0,NULL,0)==0) {
			file="";
			do {
				file+=s+'\n';
				getline(cin,s);
			} while(regexec(&inclstat,s.c_str(),0,NULL,0)==0);
			outfile=false;
			fileclipped=warnclipped;
			instantclipped=warnclipped=false;
		}
		if(regexec(&instbegin,s.c_str(),0,NULL,0)==0) {
			instant="";
			do {
				instant+=s+'\n';
				getline(cin,s);
			} while(regexec(&inststat,s.c_str(),0,NULL,0)==0);
			outinstant=false;
			instantclipped=warnclipped;
			warnclipped=false;
		}
		if(regexec(&ignorebegin,s.c_str(),0,NULL,0)==0) {
			instant="";
			do {
				getline(cin,s);
			} while(regexec(&warnstat,s.c_str(),0,NULL,0)==0);
			warnclipped=true;
			continue;
		}
		if(!outfile) {
			if(fileclipped) {
				cout << clip << endl;
				fileclipped=false;
			}
			cout << file;
			outfile=true;
		}
		if(!outinstant) {
			if(instantclipped) {
				cout << clip << endl;
				instantclipped=false;
			}
			cout << instant;
			outinstant=true;
		}
		if(warnclipped) {
			cout << clip << endl;
			warnclipped=false;
		}
		cout << s << endl;
		getline(cin,s);
	}
	if(warnclipped)
		cout << clip << endl;
}
