#ifndef INCLUDED_qwerkserialstream_h_
#define INCLUDED_qwerkserialstream_h_

#include <cstdio>
#include <cstring>
#include <iostream>
#include <iosfwd>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <stdexcept>

#include <errno.h>
#include <signal.h>
//#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <cstring>
#include <string>
#include <poll.h>

#include "TeRKPeerCommon.h"
#include "peer/MRPLPeer.h"
#include "SerialIO.h"

template <class charT, class traits=std::char_traits<charT> >
class basic_QwerkSerialBuffer : public std::basic_streambuf<charT,traits> {
	public:	
	//  Types:
	typedef charT                     char_type;
	typedef typename traits::int_type int_type;
	typedef typename traits::pos_type pos_type;
	typedef typename traits::off_type off_type;
	typedef traits                    traits_type;
	
	//Constructors/Destructors:
	basic_QwerkSerialBuffer(TeRK::SerialIOServicePrx proxy, std::string port);

	virtual ~basic_QwerkSerialBuffer();

	virtual void open(TeRK::SerialIOServicePrx proxy, std::string port);
	
//Inherited Functions:
public:
	virtual void			in_sync(); //users shouldn't need to call this directly... but can if want to
	virtual void			out_flush();

protected:
	using std::basic_streambuf<charT,traits>::eback;
	using std::basic_streambuf<charT,traits>::gptr;
	using std::basic_streambuf<charT,traits>::egptr;
	using std::basic_streambuf<charT,traits>::gbump;
	using std::basic_streambuf<charT,traits>::pbase;
	using std::basic_streambuf<charT,traits>::pptr;
	using std::basic_streambuf<charT,traits>::epptr;
	using std::basic_streambuf<charT,traits>::pbump;
	
	//  lib.streambuf.virt.get Get area:
	virtual std::streamsize	showmanyc();
	//	virtual streamsize xsgetn(char_type* s, streamsize n);
	virtual int_type		underflow();
	virtual int_type		uflow();
	
	//  lib.streambuf.virt.pback Putback:
	//	virtual int_type pbackfail(int_type c = traits::eof() );
	//  lib.streambuf.virt.put Put area:
	//	virtual streamsize xsputn(const char_type* s, streamsize n);
	virtual int_type		overflow(int_type c  = traits::eof());
	
	//  lib.streambuf.virt.buffer Buffer management and positioning:
	//	virtual _Myt basic_netbuf<char_type, traits_type>* setbuf(char_type* s, streamsize n);
	//	virtual pos_type seekoff(off_type off, ios_base::seekdir way, ios_base::openmode which = ios_base::in | ios_base::out);
	//	virtual pos_type seekpos(pos_type sp, ios_base::openmode which = ios_base::in | ios_base::out);
	virtual int			sync();
	//  lib.streambuf.virt.locales Locales:
	//	virtual void imbue(const locale &loc);
	
//Data:
protected:
	charT *buf_in, *buf_out;
	bool using_buf_in, using_buf_out;
	static const size_t def_buf_in_size;
	static const size_t def_buf_out_size;

	TeRK::SerialIOServicePrx prx;
	std::string terminal;
	
private:
	//basic_QwerkSerialBuffer() = 0;
	basic_QwerkSerialBuffer(const basic_QwerkSerialBuffer&); // copy constructor, don't call
	basic_QwerkSerialBuffer& operator=(const basic_QwerkSerialBuffer&); //!< assignment, don't call
};
template <class charT, class traits>
const size_t basic_QwerkSerialBuffer<charT,traits>::def_buf_in_size=(1<<14)-28;
template <class charT, class traits>
const size_t basic_QwerkSerialBuffer<charT,traits>::def_buf_out_size=(1<<14)-28;

/*
template<class T>
class char_traits {
public:
  typedef T char_type;
  typedef int int_type;
  typedef T* pos_type;
  typedef unsigned int off_type;
  static void copy(pos_type dst, pos_type src, off_type size) {
    memcpy(dst,src,size);
  }
  static void move(pos_type dst, pos_type src, off_type size) {
    memmove(dst,src,size);
  }
  static int to_int_type(T c) { return c; }
  static int eof() { return EOF; }
};*/

typedef basic_QwerkSerialBuffer<char, std::char_traits<char> > QwerkSerialBuffer;

template <class charT, class traits>
basic_QwerkSerialBuffer<charT,traits>::basic_QwerkSerialBuffer(TeRK::SerialIOServicePrx proxy, std::string port)
  : std::basic_streambuf<charT,traits>(), buf_in(NULL), buf_out(NULL), using_buf_in(false), using_buf_out(false), prx(proxy), terminal(port) {
	  buf_in = new charT[def_buf_in_size];
	  buf_out = new charT[def_buf_out_size];
	  using_buf_in = using_buf_out = true;
	  setg(buf_in,buf_in+def_buf_in_size,buf_in+def_buf_in_size);
	  setp(buf_out,buf_out+def_buf_out_size);
  }

template <class charT, class traits>
basic_QwerkSerialBuffer<charT,traits>::~basic_QwerkSerialBuffer() {
	if(using_buf_in)
		delete [] buf_in;
	if(using_buf_out)
		delete [] buf_out;
}

template <class charT, class traits>
void
basic_QwerkSerialBuffer<charT,traits>::open(TeRK::SerialIOServicePrx proxy, std::string port){
	prx = proxy;
	terminal = port;
}



template <class charT, class traits>
void
basic_QwerkSerialBuffer<charT,traits>::in_sync() {
	if(gptr()>egptr())
		gbump( egptr()-gptr() );
	if(gptr()==eback()) {
		std::cout << "qwerk serial buffer error: in_sync without room in input buffer" << std::endl;
		return;
	}

	unsigned long n = gptr()-eback()-1;
	
	//std::cout << "in sync called!" << std::endl;
	//typedef ::std::vector< ::Ice::Byte> ByteArray;
	while(!prx->isDataAvailable(terminal)){
		//std::cout << "waiting for input to be available.." << std::endl;
		std::cout << "." << std::flush;
		//usleep(5000);
		usleep(20000);
	}
	::TeRK::ByteArray arr = prx->read(terminal, n);
	
	charT * temp_buf = (charT *) &(arr[0]);
	ssize_t used = arr.size();

	//TODO - what if sizeof(charT)>1?  We might need to worry about getting/storing partial char
	used/=sizeof(charT);

	size_t remain = egptr()-eback()-used;
	traits::move(eback(),egptr()-remain,remain);
	traits::copy(egptr()-used,temp_buf,used);
	gbump( -used );
}

template <class charT, class traits>
void
basic_QwerkSerialBuffer<charT,traits>::out_flush() {
	::TeRK::ByteArray arr(pbase(), pptr());
	prx->write(terminal, arr);
	pbump( -((pptr()-pbase())*sizeof(charT)) );
}


template <class charT, class traits>
inline std::streamsize
basic_QwerkSerialBuffer<charT, traits>::showmanyc() {
	return 1;//(is_open() && canRead) ? 1 : 0;
}

template <class charT, class traits>
inline typename basic_QwerkSerialBuffer<charT, traits>::int_type
basic_QwerkSerialBuffer<charT, traits>::underflow() {
	in_sync();
	if(gptr()<egptr())
		return traits::to_int_type(*gptr());
//	cout << "UNDERFLOW" << endl;
	return traits::eof();
}

template <class charT, class traits>
inline typename basic_QwerkSerialBuffer<charT, traits>::int_type
basic_QwerkSerialBuffer<charT, traits>::uflow() {
	in_sync();
	if(gptr()<egptr()) {
		int_type ans = traits::to_int_type(*gptr());
		gbump(1);
		return ans;
	}
//	cout << "UNDERFLOW" << endl;
	return traits::eof();
}

template <class charT, class traits>
inline typename basic_QwerkSerialBuffer<charT, traits>::int_type
basic_QwerkSerialBuffer<charT, traits>::overflow(int_type c) { 
	out_flush();
	if(!traits::eq_int_type(c, traits::eof())) {
		*pptr() = c;
		pbump(1);
	}
	return traits::not_eof(c);
}

template <class charT, class traits>
inline int
basic_QwerkSerialBuffer<charT, traits>::sync() {
	out_flush();
	//in_sync();
	return 0;
}


#endif
