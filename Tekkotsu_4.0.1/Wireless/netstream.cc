#ifndef PLATFORM_APERIOS
#include "netstream.h"
#include <iostream>

using namespace std;

const unsigned int IPaddr::maxHostNameLen = 256;

IPaddr::IPaddr() : server(), ipname(), ipport(0) { Init(); }

IPaddr::IPaddr(const ipnum_t& num) : server(), ipname(), ipport(0) { Init(); set_num(num); }

IPaddr::IPaddr(const ipname_t& name) : server(), ipname(), ipport(0) { Init(); set_name(name); }

IPaddr::IPaddr(const ipnum_t& num, const ipport_t& port) : server(), ipname(), ipport(0) { Init(); set_addr(num,port); }

IPaddr::IPaddr(const ipname_t& name, const ipport_t& port) : server(), ipname(), ipport(0) { Init(); set_addr(name,port); }

bool IPaddr::set_num(const ipnum_t& num) {
	if(get_num()!=num) {
		Init();
		server.sin_family      = AF_INET;
		server.sin_addr.s_addr = htonl(num);
		server.sin_port        = htons(ipport);
		struct in_addr a;
		a.s_addr = server.sin_addr.s_addr;
		char addrname[maxHostNameLen];
		inet_ntop(AF_INET,&a,addrname,maxHostNameLen);
		ipname=addrname;
	}
	return true;
}

bool IPaddr::set_name(const ipname_t& name) {
	Init();
	if(!isalpha(name[0])) {
		// in case the string holds a dotted decimal we can convert directly
		struct in_addr a;
		if(inet_pton(AF_INET,name.c_str(),&a)<=0) {
			cerr << "IPaddr error: inet_pton failed: " << name << endl;
			return false;
		}
		return set_num(ntohl(a.s_addr));
	} else {
		Init();
		struct hostent * data = gethostbyname(name.c_str());
		if(data == NULL) {
			cerr << "IPaddr error: gethostbyname failed: " << name << endl;
			return false;
		}
		ipname=name;

		memcpy((char *) &server.sin_addr, data->h_addr_list[0], data->h_length);
		server.sin_family = data->h_addrtype;
		server.sin_port = htons(ipport);
	}
	return true;
}

IPaddr::ipname_t IPaddr::get_display_num() const {
	struct in_addr a;
	a.s_addr = server.sin_addr.s_addr;
	char addrname[maxHostNameLen];
	inet_ntop(AF_INET,&a,addrname,maxHostNameLen);
	return addrname;
}

IPaddr::ipname_t IPaddr::get_rname() const {
	struct in_addr a;
	a.s_addr = server.sin_addr.s_addr;
	struct hostent * data = gethostbyaddr((char*)&a,sizeof(a),AF_INET);
	if(data==NULL) {
		cerr << "IPaddr error: gethostbyaddr failed: " << ipname << endl;
		return "";
	}
	return data->h_name;
}

void IPaddr::Init() {
	ipname="";
	memset((char *) &server, sizeof(server), 0);
}

#endif
