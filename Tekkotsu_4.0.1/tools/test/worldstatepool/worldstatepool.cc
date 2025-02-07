#define TK_ENABLE_THREADING
#include "local/minisim.h"

#include "Shared/WorldStatePool.h"
#include "Shared/MarkScope.h"
#include "IPC/PollThread.h"
#include "local/EntryPoint.h"
#include <string>
#include <iostream>
#include <iomanip>

using namespace std;
using namespace minisim;

void usage(char* p) {
	cerr << "Usage: " << p << " [-h|--help] [[read] freq1 freq2 ...] [write freq1 freq2 ...]" << endl;
}

WorldStatePool& getPool() {
	static WorldStatePool p;
	return p;
}
/*EntryPoint& getEntryPoint() {
	static EntryPoint e(getPool());
	return e;
}*/
unsigned int frame=0;

class ReadThread : public PollThread {
public:
	ReadThread(int myid, long freq) : PollThread(freq,freq,true,false), id(myid) {}
protected:
	virtual bool poll() {
		//cout << dec << setw(12) << get_time() << ' ' << id << " Read wait " << flush;
		//MarkScope l(getEntryPoint());
		EntryPoint::WorldStateRead wsr;
		MarkScope l(getPool(),wsr);
		cout << dec << setw(12) << get_time() << ' ' << id << " Read active " << state << ' ' << flush;
		TimeET t;
		//while(t.Age()<20L) {}
		usleep(20*1000);
		cout << "- " << id <<" done (" << get_time() << ")" << endl;
		return true;
	}
	int id;
};

class WriteThread : public PollThread {
public:
	WriteThread(int myid, long freq) : PollThread(freq,freq,true,false), id(myid) {}
protected:
	virtual bool poll() {
		cout << dec << setw(12) << get_time() << ' ' << id << " Write wait " << flush;
		EntryPoint::WorldStateWrite wsw(get_time()/10);
		//MarkScope l(getEntryPoint(),wsw);
		MarkScope l(getPool(),wsw);
		cout << dec << setw(12) << get_time() << ' ' << id << " Write active " << state << "/" << wsw.src << ' ' << flush;
		TimeET t;
		//while(t.Age()<20L) {}
		usleep(20*1000);
		cout << "- " << id <<" done (" << get_time() << ")" << endl;
		return true;
	}
	int id;
};

int main(unsigned argc, char** argv) {
	initialize();

	bool read=true;
	vector<PollThread*> threads;
	
	for(unsigned i=1; i<argc; i++) {
		if(strcmp(argv[i],"-h")==0 || strcmp(argv[i],"--help")==0) {
			usage(argv[0]);
			for(vector<PollThread*>::iterator it=threads.begin(); it!=threads.end(); ++it)
				delete *it;
			destruct();
			return 2;
		} else if(strcmp(argv[i],"read")==0) {
			read=true;
		} else if(strcmp(argv[i],"write")==0) {
			read=false;
		} else {
			unsigned t=atoi(argv[i]);
			threads.push_back(read ? static_cast<PollThread*>(new ReadThread(threads.size(),t)) : static_cast<PollThread*>(new WriteThread(threads.size(),t)));
		}
	}
	
	cout << setw(12) << get_time() << " starting " << threads.size() << " threads" << endl;
	for(vector<PollThread*>::iterator it=threads.begin(); it!=threads.end(); ++it)
		(*it)->start();
	
	/*string s;
	getline(cin,s);
	while(s=="i") {
		for(vector<PollThread*>::iterator it=threads.begin(); it!=threads.end(); ++it)
			(*it)->interrupt();
		getline(cin,s);
	}*/
	sleep(600);
	
	for(vector<PollThread*>::iterator it=threads.begin(); it!=threads.end(); ++it)
		delete *it;
	destruct();
	return 0;
}

