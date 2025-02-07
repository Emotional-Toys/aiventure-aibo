#include <iostream>
#include <stdio.h>
#include <string>
#include <algorithm>
#include "Motion/DynamicMotionSequence.h"
#include "Shared/Config.h"

#include <sys/types.h>
#include <sys/stat.h>

using namespace std;

unsigned int simulator_time;

int usage(unsigned int argc, const char** argv) {
	const char* name;
	if(strchr(argv[0],'/')!=NULL)
		name=strrchr(argv[0],'/')+1;
	else
		name=argv[0];
	cerr << "Usage:\t" << argv[0] << " [-rad ] [ -deg ] [ -safe <margin> ] <in-file> <out-file>" << endl;
	cerr << "\t" << argv[0] << " [-rad ] [ -deg ] [ -safe <margin> ] <in-files> <out-directory>" << endl;
	cerr << endl;
	cerr << '`' <<name << "' will insert time between keyframes so that the resulting" << endl;
	cerr << "motion can be played back without missing frames or violating Sony's joint" << endl;
	cerr << "speed guidelines." << endl;
	cerr << endl;
	cerr << "The first usage converts one single file.  With the second usage, `" << name << "'" << endl;
	cerr << "expects the last argument to be the directory to store the converted files " << endl;
	cerr << "into. (with the same name)" << endl;
	cerr << endl;
	cerr << "-deg will cause files to save using degrees.  Default is radians." << endl;
	cerr << endl;
	cerr << "-safe <margin> will add (or subtract) a safety margin.  The argument is the" << endl;
	cerr << "percent of the published guideline to actually use.  Default is 1." << endl;
	return 2;
}

bool compress=false;
float margin=1;

int main(unsigned int argc, const char** argv) {
	if(argc<3)
		return usage(argc,argv);
	::config = new Config();
	::config->setFileSystemRoot("");
	if(::config->loadFile("tekkotsu.xml")==0) {
		if(::config->loadFile("tekkotsu.cfg")==0)
			std::cerr << std::endl << " *** ERROR: Could not load configuration file tekkotsu.xml *** " << std::endl << std::endl;
		else
			std::cerr << "Successfully imported settings from old-format tekkotsu.cfg" << std::endl;
	}
	unsigned int used=1;
	bool isRad=true;
	while(used<argc) {
		if(strcmp("-deg",argv[used])==0) {
			used++;
			isRad=false;
			cout << "storing as degrees..." << endl;
			continue;
		}
		if(strcmp("-rad",argv[used])==0) {
			used++;
			isRad=true;
			cout << "storing as radians..." << endl;
			continue;
		}
		if(strcmp("-compress",argv[used])==0) {
			used++;
			compress=true;
			cout << "compression on..." << endl;
			continue;
		}
		if(strcmp("-safe",argv[used])==0) {
			used++;
			char * end;
			margin=strtod(argv[used],&end);
			if(*end!='\0')
				return usage(argc,argv);
			used++;
		}
		break;
	}
	struct stat s;
	int ret=stat(argv[argc-1],&s);
	//cout << argv[argc-1] << ' ' << ret << ' ' << (void*)s.st_mode << ' ' << (void*)s.st_rdev << endl;
	if(ret==0 && s.st_mode&S_IFDIR) {
		for(unsigned int i=used; i<argc-1; i++) {
			string path=argv[argc-1];
			if(strchr(argv[i],'/')!=NULL)
				path+=strrchr(argv[i],'/');
			else {
				path+="/";
				path+=argv[i];
			}
			DynamicMotionSequence ms;
			if(isRad)
				ms.setSaveRadians();
			else
				ms.setSaveDegrees();
			ms.loadFile(argv[i]);
			ms.makeSafe(MaxOutputSpeed,margin);
			if(compress)
				ms.compress();
			ms.saveFile(path.c_str());
		}
		used=argc;
	} else if(argc-used==2) {
		DynamicMotionSequence ms;
		if(isRad)
			ms.setSaveRadians();
		else
			ms.setSaveDegrees();
		ms.loadFile(argv[used]);
		ms.makeSafe(MaxOutputSpeed,margin);
		if(compress)
			ms.compress();
		ms.saveFile(argv[used+1]);
		used=argc;
	} else
		return usage(argc,argv);
	cout << endl;
	return 0;
}

