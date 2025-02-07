//This code uses portions of CMPack'02 for loading their motion files.
//See license below.

/*LICENSE:
  =========================================================================
    CMPack'02 Source Code Release for OPEN-R SDK v1.0
    Copyright (C) 2002 Multirobot Lab [Project Head: Manuela Veloso]
    School of Computer Science, Carnegie Mellon University
  -------------------------------------------------------------------------
    This software is distributed under the GNU General Public License,
    version 2.  If you do not have a copy of this licence, visit
    www.gnu.org, or write: Free Software Foundation, 59 Temple Place,
    Suite 330 Boston, MA 02111-1307 USA.  This program is distributed
    in the hope that it will be useful, but WITHOUT ANY WARRANTY,
    including MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  -------------------------------------------------------------------------
    Additionally licensed to Sony Corporation under the following terms:

    This software is provided by the copyright holders AS IS and any
    express or implied warranties, including, but not limited to, the
    implied warranties of merchantability and fitness for a particular
    purpose are disclaimed.  In no event shall authors be liable for
    any direct, indirect, incidental, special, exemplary, or consequential
    damages (including, but not limited to, procurement of substitute
    goods or services; loss of use, data, or profits; or business
    interruption) however caused and on any theory of liability, whether
    in contract, strict liability, or tort (including negligence or
    otherwise) arising in any way out of the use of this software, even if
    advised of the possibility of such damage.
  ========================================================================= */


#include <iostream>
#include <stdio.h>
#include <string>
#include <algorithm>
#include "Motion/DynamicMotionSequence.h"
#include "Motion/Geometry.h"
#include "Motion/OldKinematics.h"
#include "Shared/Config.h"

#include <sys/types.h>
#include <sys/stat.h>

using namespace std;

unsigned int simulator_time;

#define ATTR_ANGLES   1
#define ATTR_POSITION 2

struct LegState{
  long attr,reserved;
  point3d pos;
  double angles[3];
};

struct HeadState{
  long attr,reserved;
  vector3d target;
  double angles[3];
};

struct BodyState{
  BodyPosition pos;
  LegState leg[4];
  HeadState head;
};

struct BodyStateMotion{
  BodyState body;
  long time; // ms
  long reserved;
};

int usage(unsigned int argc, const char** argv) {
	const char* name;
	if(strchr(argv[0],'/')!=NULL)
		name=strrchr(argv[0],'/')+1;
	else
		name=argv[0];
	cerr << "Usage:\t" << argv[0] << " [-rad] [-deg] [-compress] [-conv] [-skip <n>] <in-file> <out-file>" << endl;
	cerr << "\t" << argv[0] << " [-rad] [-deg] [-compress] [-conv] [-skip <n>] <in-files> <out-directory>" << endl;
	cerr << endl;
	cerr << '`' <<name << "' converts CM-Pack'02 (and possibly newer) posture sequences into " << endl;
	cerr << "Tekkotsu format motion scripts.  Unfortunately, both types use .mot extensions, " << endl;
	cerr << "so you'll need to do some renaming/relocating." << endl;
	cerr << endl;
	cerr << "The first usage converts one single file.  With the second usage, `" << name << "'" << endl;
	cerr << "expects the last argument to be the directory to store the converted files " << endl;
	cerr << "into. (with the same name)" << endl;
	cerr << endl;
	cerr << "-deg will cause files to save using degrees.  Default is radians." << endl;
	cerr << endl;
	cerr << "-compress will call MotionSequence's compress() function before saving." << endl;
	cerr << endl;
	cerr << "-conv will read buggy motion files which were written using degrees instead of " << endl;
	cerr << "radians.  Shouldn't be necessary, but just in case." << endl;
	cerr << endl;
	cerr << "-skip <n> will skip the first n frames of the input. (default 1)" << endl;
	cerr << endl;
	cerr << "Be aware the resulting MotionSequence files may need a little tweaking by hand " << endl;
	cerr << "to remove \"unused\" joints since this can't be determined from the CM-Pack'02 " << endl;
	cerr << "posture sequence file." << endl;
	return 2;
}

int load_cmpack_mot(const char* filename, MotionSequenceEngine& ms);
void Complete(BodyState &body);
void convert(const BodyStateMotion* frame, unsigned int num_frames, MotionSequenceEngine& ms);

bool convertDeg=false;
bool compress=false;
unsigned int skip=1;

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
		if(strcmp("-conv",argv[used])==0) {
			used++;
			convertDeg=true;
			cout << "converting degrees..." << endl;
			continue;
		}
		if(strcmp("-compress",argv[used])==0) {
			used++;
			compress=true;
			cout << "compression on..." << endl;
			continue;
		}
		if(strcmp("-skip",argv[used])==0) {
			used++;
			char * end;
			skip=strtol(argv[used],&end,0);
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
			load_cmpack_mot(argv[i],ms);
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
		load_cmpack_mot(argv[used],ms);
		if(compress)
			ms.compress();
		ms.saveFile(argv[used+1]);
		used=argc;
	} else
		return usage(argc,argv);
	cout << endl;
	return 0;
}


int load_cmpack_mot(const char* filename, MotionSequenceEngine& ms) {
	unsigned int num;
	FILE * in = fopen(filename,"rb");
	if(!in) {
		cout << '\'' << filename << "' could not be opened." << endl;
		return 0;
	}
	
	BodyStateMotion * frame;
	fread(&num,sizeof(int),1,in);
	if(num<=0 || num>=64) return(false);

	frame = new BodyStateMotion[num];//(BodyStateMotion*)malloc(sizeof(BodyStateMotion)*num);
	if(!frame) {
		cout << "Memory allocation error" << endl;
		return 0;
	}
	fread(frame,sizeof(BodyStateMotion),num,in);
	fclose(in);
	for(uint i=0; i<num; i++){
		Complete(frame[i].body);
	}
	printf("Loaded Motion '%s'\t frames=%d\n",filename,num);

	convert(frame,num,ms);
	printf("Converted\n");
	return 1;
}


void Complete(BodyState &body) {
	int attr,i;

	// legs
	for(i=0; i<4; i++){
		attr = body.leg[i].attr;
		if(!(attr & ATTR_ANGLES) && (attr & ATTR_POSITION)){
			GetLegAngles(body.leg[i].angles,body.leg[i].pos,body.pos,i);
			body.leg[i].attr |= ATTR_ANGLES;

			/*
			printf("%d (%8.2f %8.2f %8.2f)\n",i,
						 body.leg[i].pos.x,
						 body.leg[i].pos.y,
						 body.leg[i].pos.z);
			*/
		}
	}

	// head
	attr = body.head.attr;
	if(!(attr & ATTR_ANGLES) && (attr & ATTR_POSITION)){
		GetHeadAngles(body.head.angles,body.head.target,
									body.pos.angle.y,body.pos.loc.z);
		body.head.attr |= ATTR_ANGLES;
	}
}

void convert(const BodyStateMotion* frame, unsigned int num, MotionSequenceEngine& ms) {
	//	long initdelay=frame[0].time;
	ms.setTime(0);
	for(unsigned int f=skip; f<num; f++) {
		int c=0;
		ms.setTime(ms.getTime()+std::max(frame[f].time,1l));
		for(int leg=0; leg<4; leg++) //4 legs, 3 joints
			for(int a=0; a<3; a++)
				ms.setOutputCmd(c++,frame[f].body.leg[leg].angles[a]*(convertDeg?M_PI/180:1));
		for(int a=0; a<3; a++) // 1 head, 3 joints
			ms.setOutputCmd(c++,frame[f].body.head.angles[a]*(convertDeg?M_PI/180:1));
		
		//The following aren't actually recorded in source file
		//included below just for completeness
/*		for(int a=0; a<2; a++) // 1 tail, 2 joints
			ms.setOutputCmd(c++,OutputCmd(0,0));
		ms.setOutputCmd(c++,OutputCmd(0,0)); // 1 mouth, 1 joint
		for(uint a=0; a<NumLEDs; a++)
			ms.setOutputCmd(c++,OutputCmd(0,0));
		for(int a=0; a<2; a++) // 2 ears, 1 joint
			ms.setOutputCmd(c++,OutputCmd(0,0)); */
	}
}

