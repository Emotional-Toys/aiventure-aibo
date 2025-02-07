#include "PostureEngine.h"
#include "Shared/WorldState.h"
#include "Motion/roboop/robot.h"
#include "Shared/Config.h"
#include "Shared/string_util.h"
#include <stdio.h>
#include <iostream>
#include <regex.h>
#include <map>

using namespace std;

PostureEngine::~PostureEngine() {}

void PostureEngine::takeSnapshot() {
	takeSnapshot(*WorldState::getCurrent());
}

void PostureEngine::takeSnapshot(const WorldState& st) {
	for(unsigned int i=0; i<NumOutputs; i++)
		cmds[i].value=st.outputs[i];
}

void PostureEngine::setWeights(float w, unsigned int lowjoint, unsigned int highjoint) {
	for(unsigned int i=lowjoint; i<highjoint; i++)
		cmds[i].weight=w;
}

void PostureEngine::clear() {
	for(unsigned int i=0; i<NumOutputs; i++)
		cmds[i].unset();
}

PostureEngine& PostureEngine::setOverlay(const PostureEngine& pe) {
	for(unsigned int i=0; i<NumOutputs; i++)
		if(pe.cmds[i].weight>0)
			cmds[i]=pe.cmds[i];
	return *this;
}
PostureEngine PostureEngine::createOverlay(const PostureEngine& pe) const {
	PostureEngine tmp(*this);
	return tmp.setOverlay(pe);
}
PostureEngine& PostureEngine::setUnderlay(const PostureEngine& pe) {
	for(unsigned int i=0; i<NumOutputs; i++)
		if(cmds[i].weight<=0)
			cmds[i]=pe.cmds[i];
	return *this;
}
PostureEngine PostureEngine::createUnderlay(const PostureEngine& pe) const {
	PostureEngine tmp(*this);
	return tmp.setUnderlay(pe);
}
/*! joints being averaged with unused joints have their weights averaged, but not their values (so an output can crossfade properly)\n
 *	@param pe the other PostureEngine
 *	@param w amount to weight towards @a pe
 *	- if @a w < .001, nothing is done
 *	- if @a w > .999, a straight copy of @a pe occurs (sets joints to unused properly at end of fade)
 *	- .001 and .999 is used instead of 0 and 1 to allow for slight addition errors in a loop (if
 *		using repeated additions of a delta value instead of repeated divisions)
 *	@return @c *this, stores results into this */
PostureEngine& PostureEngine::setAverage(const PostureEngine& pe, float w) {
	if(w<0.001)
		return *this;
	if(w>0.999)
		return (*this=pe);
	float wp=1-w;
	for(unsigned int i=0; i<NumOutputs; i++)
		if(cmds[i].weight>0) {
			if(pe.cmds[i].weight>0)
				cmds[i].set(cmds[i].value*wp+pe.cmds[i].value*w,cmds[i].weight*wp+pe.cmds[i].weight*w);
			else
				cmds[i].weight*=wp;
		} else
			cmds[i].set(pe.cmds[i].value,pe.cmds[i].weight*w);
	return *this;
}
/*! joints being averaged with weight<=0 have their weights averaged, but not their values (so an output can crossfade properly)\n
 *	@param pe the other PostureEngine
 *	@param w amount to weight towards @a pe
 *	- if @a w < .001, nothing is done
 *	- if @a w > .999, a straight copy of @a pe occurs (sets joints to unused properly at end of fade)
 *	- .001 and .999 is used instead of 0 and 1 to allow for slight addition errors in a loop (if
 *		using repeated additions of a delta value instead of repeated divisions)
 *	@return a new posture containing the results */
PostureEngine PostureEngine::createAverage(const PostureEngine& pe, float w) const {
	PostureEngine tmp(*this);
	return tmp.setAverage(pe,w);
}
PostureEngine& PostureEngine::setCombine(const PostureEngine& pe) {
	for(unsigned int i=0; i<NumOutputs; i++) {
		float total=cmds[i].weight+pe.cmds[i].weight;
		cmds[i].set((cmds[i].value*cmds[i].weight+pe.cmds[i].value*pe.cmds[i].weight)/total,total);
	}
	return *this;
}
PostureEngine PostureEngine::createCombine(const PostureEngine& pe) const {
	PostureEngine tmp(*this);
	return tmp.setCombine(pe);
}

float PostureEngine::diff(const PostureEngine& pe) const {
	float ans=0;
	for(unsigned int i=0; i<NumOutputs; i++)
		if(cmds[i].weight>0 && pe.cmds[i].weight>0) {
			float dif=cmds[i].value-pe.cmds[i].value;
			ans+=dif*dif;
		}
	return ans;
}

float PostureEngine::avgdiff(const PostureEngine& pe) const {
	float ans=0;
	unsigned int cnt=0;
	for(unsigned int i=0; i<NumOutputs; i++)
		if(cmds[i].weight>0 && pe.cmds[i].weight>0) {
			float dif=cmds[i].value-pe.cmds[i].value;
			ans+=dif*dif;
			cnt++;
		}
	return ans/cnt;
}

float PostureEngine::maxdiff(const PostureEngine& pe) const {
	float max=0;
	for(unsigned int i=0; i<NumOutputs; i++)
		if(cmds[i].weight>0 && pe.cmds[i].weight>0) {
			float dif=cmds[i].value-pe.cmds[i].value;
			if(dif>max)
				max=dif;
		}
	return max;
}

void PostureEngine::setSaveFormat(bool condensed, WorldState* ws) {
	saveFormatCondensed=condensed;
	loadSaveSensors=ws;
}

unsigned int PostureEngine::getBinSize() const {
	unsigned int written=11; //accounts for initial #POS\n and final #END\n, plus 1
	const unsigned int len=0;
	char buf[len];
	if(saveFormatCondensed) {
#ifndef PLATFORM_APERIOS
		written+=snprintf(buf,len,"condensed %s\n",RobotName);
#else
		written+=snprintf(buf,len,"condensed %s\n",RobotName.c_str());
#endif
		if(loadSaveSensors!=NULL)
			written+=snprintf(buf,len,"meta-info = %u %u\n",loadSaveSensors->lastSensorUpdateTime,loadSaveSensors->frameNumber);
		bool weightsAllEqual=true;
		float weightsVal=cmds[0].weight;
		for(unsigned int i=1; i<NumOutputs && weightsAllEqual; i++)
			weightsAllEqual=(cmds[i].weight==weightsVal);
		if(!weightsAllEqual || weightsVal!=0) { //if they're all 0, skip outputs and weights
			written+=snprintf(buf,len,"outputs =");
			for(unsigned int i=0; i<NumOutputs; i++) {
				written+=snprintf(buf,len," %g",cmds[i].value);
			}
			if(!weightsAllEqual || weightsVal!=1) { //if they're all 1, skip weights
				written+=snprintf(buf,len,"\nweights =");
				for(unsigned int i=0; i<NumOutputs; i++) {
					written+=snprintf(buf,len," %g",cmds[i].weight);
				}
			}
			written+=snprintf(buf,len,"\n");
		}
		if(loadSaveSensors!=NULL) {
			written+=snprintf(buf,len,"buttons =");
			for(unsigned int i=0; i<NumButtons; i++) {
				written+=snprintf(buf,len," %g",loadSaveSensors->buttons[i]);
			}
			written+=snprintf(buf,len,"\nsensors =");
			for(unsigned int i=0; i<NumSensors; i++) {
				written+=snprintf(buf,len," %g",loadSaveSensors->sensors[i]);
			}
			written+=snprintf(buf,len,"\npidduties =");
			for(unsigned int i=0; i<NumPIDJoints; i++) {
				written+=snprintf(buf,len," %g",loadSaveSensors->pidduties[i]);
			}
			written+=snprintf(buf,len,"\n");
		}		
	} else {
		if(loadSaveSensors!=NULL)
			written+=snprintf(buf,len,"<meta-info>\n	timestamp\t%u\n	 framenumber\t%u\n</meta-info>\n",loadSaveSensors->lastSensorUpdateTime,loadSaveSensors->frameNumber);
		for(unsigned int i=0; i<NumOutputs; i++)
			if(cmds[i].weight>0) {
				if(cmds[i].weight==1)
					written+=snprintf(buf,len,"%s\t% .4f\n",outputNames[i],cmds[i].value);
				else
					written+=snprintf(buf,len,"%s\t% .4f\t% .4f\n",outputNames[i],cmds[i].value,cmds[i].weight);
			}
		if(loadSaveSensors!=NULL) {
			written+=snprintf(buf,len,"<buttons>\n");
			for(unsigned int i=0; i<NumButtons; i++) {
				written+=snprintf(buf,len,"	 %s\t% .4f\t\n",buttonNames[i],loadSaveSensors->buttons[i]);
			}
			written+=snprintf(buf,len,"</buttons><sensors>\n");
			for(unsigned int i=0; i<NumSensors; i++) {
				written+=snprintf(buf,len,"	 %s\t% .4f\t\n",sensorNames[i],loadSaveSensors->sensors[i]);
			}
			written+=snprintf(buf,len,"</sensors><pidduties>\n");
			for(unsigned int i=0; i<NumPIDJoints; i++) {
				written+=snprintf(buf,len,"	 duty-%s\t% .4f\t\n",outputNames[i],loadSaveSensors->pidduties[i]);
			}
			written+=snprintf(buf,len,"</pidduties>\n");
		}
	}
	return written;
}

unsigned int PostureEngine::loadBuffer(const char buf[], unsigned int len) {
	unsigned int origlen=len;
	clear();
	if(buf==NULL || len==0) {
		cerr << "*** ERROR: PostureEngine::loadBuffer(" << static_cast<const void*>(buf) << ',' << len << ")" << endl;
		return 0;
	}
	if(strncmp("#POS",buf,4)!=0) {
		// we don't want to display an error here because we may be only testing file type,
		// so it's up to the caller to decide if it's necessarily an error if the file isn't
		// a posture
		//std::cout << "ERROR PostureEngine load corrupted - expected #POS header" << std::endl;
		return 0;
	}
	saveFormatCondensed=false;
	Capabilities* caps = &capabilities;
	unsigned int linenum=0;
	bool filtered=false; // true if in a specialization section for a different model
	section_t curSection=SECTION_OUTPUTS;
	map<string,section_t> sectionMap;
	sectionMap["meta-info"]=SECTION_METAINFO;
	sectionMap["outputs"]=SECTION_OUTPUTS;
	sectionMap["buttons"]=SECTION_BUTTONS;
	sectionMap["sensors"]=SECTION_SENSORS;
	sectionMap["pidduties"]=SECTION_PIDDUTIES;
	
	
	while(len<=origlen && len>0) {
		// extract a line from the buffer (line endings could be \r, \n, or \r\n )
		string line;
		for(unsigned int lineend=0; lineend<len; ++lineend) {
			bool isreturn = (buf[lineend]=='\r');
			if(isreturn || buf[lineend]=='\n') {
				line.assign(buf,lineend);
				++linenum;
				++lineend;
				buf+=lineend;
				len-=lineend;
				if(len==0) // end of buffer, don't check for \n
					break;
				if(isreturn && buf[0]=='\n') { // indicates an \r\n
					++buf;
					--len;
				}
				break;
			}
		}
		
		// strip comment
		string::size_type commentPos = line.find('#');
		if(commentPos!=string::npos) {
			if(line.substr(commentPos)=="#END")
				return origlen-len;
			line = line.substr(0,commentPos);
		}
		
		// tokenize
		// whitespace and '=' are used as delimiters
		vector<string> words;
		for(unsigned int i=0; i<line.size(); ++i) {
			if(isspace(line[i]) || line[i]=='=')
				continue;
			unsigned int j=i+1;
			while(j<line.size() && !isspace(line[j]) && line[j]!='=')
				++j;
			words.push_back(line.substr(i,j-i));
			i=j;
		}
		if(words.size()==0)
			continue;
		
		// debugging output
		/*cout << "LINE " << linenum << endl;
		for(unsigned int i=0; i<words.size(); ++i)
			cout << '\t' << words[i] << endl;*/
		
		// process the line!
		if(!loadLine(linenum,sectionMap,words,curSection,caps,filtered))
			return 0;
		
	} // end of process-line loop 'while'
		
	// shouldn't get here, proper file ends with '#END', which will trigger a 'return' within the loop
	std::cout << "*** WARNING PostureEngine load missing #END" << std::endl;
	return origlen-len;
}

bool PostureEngine::loadLine(unsigned int linenum, const std::map<std::string,section_t>& sectionMap, std::vector<std::string>& words, section_t& curSection, Capabilities*& caps, bool& filtered) {
	if(words[0]=="specialize") {
		if(words.size()>2)
			cerr << "*** Warning line " << linenum << ", extra arguments to 'specialize'" << endl;
		filtered = false;
		if(words.size()>1) try {
			filtered = !string_util::reMatch(RobotName,words[1],REG_EXTENDED);
		} catch(const std::string& err) {
			cerr << "*** ERROR line " << linenum << ", bad regular expression: " << err << endl;
			return false;
		}
		
	} else if(words[0]=="condensed") {
		if(words.size()>2)
			cerr << "*** Warning line " << linenum << ", extra arguments to 'condensed'" << endl;
		if(words.size()==1) {
			cerr << "*** ERROR line " << linenum << ", missing model name for 'condensed'" << endl;
			return false;
		} else {
			caps = getCapabilities(words[1]);
			if(caps==NULL) {
				cerr << "*** ERROR line " << linenum << ", unknown model for 'condensed': " << words[1] << endl;
				return false;
			}
			saveFormatCondensed=true;
		}
		
	} else if(words[0]=="verbose") {
		if(words.size()>1)
			cerr << "*** Warning line " << linenum << ", extra arguments to 'verbose'" << endl;
		saveFormatCondensed=false;
		
	} else if(words[0][0]=='<' && words[0][words[0].size()-1]=='>') {
		if(words.size()>1)
			cerr << "*** Warning line " << linenum << ", extra arguments to section tag (this isn't a real XML format, tag must be on line by itself)" << endl;
		bool isend = (words[0][1]=='/');
		const string name = words[0].substr(isend?1:2, words[0].size()-(isend?2:3));
		map<string,section_t>::const_iterator it = sectionMap.find(name);
		if(it==sectionMap.end()) {
			cerr << "*** ERROR line " << linenum << ", unknown section '" << name << "'" << endl;
			return false;
		}
		section_t section = it->second;
		if(isend) {
			if(curSection!=section)
				cerr << "*** Warning line " << linenum << ", mismatched close tag " << name << endl;
			curSection=SECTION_OUTPUTS;
		} else {
			if(curSection!=SECTION_OUTPUTS)
				cerr << "*** Warning line " << linenum << ", nested tags not supported, or missing close tag" << endl;
			curSection=section;
		}
		
	} else if(filtered) {
		// no-op
		
	} else {
		// either a section name (condensed form) or an output/button/sensor name (verbose form)
		map<string,section_t>::const_iterator it = sectionMap.find(words[0]);
		if(it==sectionMap.end()) {
			// not an entry in sectionMap...
			if(words[0]=="weights") { // weights is a "condensed-only" section, handled a little specially since it's not in sectionMap...
				if(caps==&capabilities) {
					// we're using the "native" target... make things a bit quicker
					for(unsigned int i=1; i<words.size(); ++i)
						cmds[i-1].weight = atof(words[i].c_str());
				} else {
					// we're using another target, map to the host
					for(unsigned int i=1; i<words.size(); ++i) {
						const char * name = caps->getOutputName(i-1);
						unsigned int off = capabilities.findOutputOffset(name);
						if(off==-1U) {
							cerr << "*** Warning line " << linenum << ", output '" << name << "' from robot " << caps->getRobotName() << " cannot be mapped to host " << RobotName << endl;
						} else {
							cmds[off].weight = atof(words[i].c_str());
						}
					}
				}
			} else {
				// line is not a section declaration -- is it an 'verbose' output/sensor/button/etc. name within the current section??
				if(words.size()<2) {
					cerr << "*** ERROR line " << linenum << ", no value supplied for " << words[0] << endl;
					return false;
				}
				switch(curSection) {
					case SECTION_METAINFO: {
						if(words[0]=="timestamp") {
							if(loadSaveSensors!=NULL)
								loadSaveSensors->lastSensorUpdateTime = atoi(words[1].c_str());
						} else if(words[0]=="framenumber") {
							if(loadSaveSensors!=NULL)
								loadSaveSensors->frameNumber = atoi(words[1].c_str());
						} else {
							cerr << "*** Warning line " << linenum << ", '" << words[0] << "' is not a valid meta-info" << endl;
						}
					} break;
					case SECTION_OUTPUTS: {
						stripTildes(words[0]);
						unsigned int off = capabilities.findOutputOffset(words[0].c_str()), loff=-1U;
						if(off==-1U) {
							// try again with L/R prefixes
							loff = capabilities.findOutputOffset(('L'+words[0]).c_str());
							if(loff!=-1U)
								off = capabilities.findOutputOffset(('R'+words[0]).c_str());
						}
						if(off==-1U)
							cerr << "*** Warning line " << linenum << ", '" << words[0] << "' is not a valid output on this model (" << RobotName << ")" << endl;
						else {
							float value = atof(words[1].c_str());
							float weight=1;
							if(words.size()>2)
								weight = atof(words[2].c_str());
							cmds[off].set(value,weight);
							if(loff!=-1U)
								cmds[loff].set(value,weight);
						}
					} break;
					case SECTION_BUTTONS: {
						unsigned int off = capabilities.findButtonOffset(words[0].c_str());
						if(off==-1U) {
							cerr << "*** Warning line " << linenum << ", '" << words[0] << "' is not a valid button on this model (" << RobotName << ")" << endl;
						} else if(loadSaveSensors!=NULL) {
							loadSaveSensors->buttons[off] = atof(words[1].c_str());
						}
					} break;
					case SECTION_SENSORS: {
						unsigned int off = capabilities.findSensorOffset(words[0].c_str());
						if(off==-1U) {
							cerr << "*** Warning line " << linenum << ", '" << words[0] << "' is not a valid sensor on this model (" << RobotName << ")" << endl;
						} else if(loadSaveSensors!=NULL) {
							loadSaveSensors->sensors[off] = atof(words[1].c_str());
						}
					} break;
					case SECTION_PIDDUTIES: {
						stripTildes(words[0]);
						unsigned int off = capabilities.findOutputOffset(words[0].c_str());
						if(off==-1U) {
							cerr << "*** Warning line " << linenum << ", '" << words[0] << "' is not a valid output on this model (" << RobotName << ")" << endl;
						} else if(off<PIDJointOffset || off>=PIDJointOffset+NumPIDJoints) {
							cerr << "*** Warning line " << linenum << ", output '" << words[0] << "' from robot " << caps->getRobotName() << " does not map to a PID joint on the local host " << RobotName << endl;
						} else if(loadSaveSensors!=NULL) {
							loadSaveSensors->pidduties[off-PIDJointOffset] = atof(words[1].c_str());
						}
					} break;
				} // end of section 'switch'
			} // end of condensed 'weight' section vs. verbose entry 'else'
			
		} else {
			// is a section name (aka condensed entry)
			switch(it->second) {
				case SECTION_METAINFO: {
					if(loadSaveSensors==NULL)
						break;
					if(words.size()>1)
						loadSaveSensors->lastSensorUpdateTime = atoi(words[1].c_str());
					if(words.size()>1)
						loadSaveSensors->frameNumber = atoi(words[2].c_str());
				} break;
				case SECTION_OUTPUTS: {
					unsigned int size=words.size();
					if(size-1!=caps->getNumOutputs()) {
						cerr << "*** ERROR line " << linenum << ", " << caps->getRobotName() << " expected " << caps->getNumOutputs() << " values, got " << (size-1) << endl;
						return 0;
					}
					if(caps==&capabilities) {
						// we're using the "native" target... make things a bit quicker
						for(unsigned int i=1; i<size; ++i)
							cmds[i-1].set(atof(words[i].c_str()));
					} else {
						// we're using another target, map to the host
						for(unsigned int i=1; i<size; ++i) {
							const char * name = caps->getOutputName(i-1);
							unsigned int off = capabilities.findOutputOffset(name);
							if(off==-1U) {
								cerr << "*** Warning line " << linenum << ", output '" << name << "' from robot " << caps->getRobotName() << " cannot be mapped to host " << RobotName << endl;
							} else {
								cmds[off].set(atof(words[i].c_str()));
							}
						}
					}
				} break;
				case SECTION_BUTTONS: {
					if(loadSaveSensors==NULL)
						break;
					unsigned int size=words.size();
					if(size-1!=caps->getNumButtons()) {
						cerr << "*** ERROR line " << linenum << ", " << caps->getRobotName() << " expected " << caps->getNumButtons() << " values, got " << (size-1) << endl;
						return 0;
					}
					if(caps==&capabilities) {
						// we're using the "native" target... make things a bit quicker
						for(unsigned int i=1; i<size; ++i)
							loadSaveSensors->buttons[i-1] = atof(words[i].c_str());
					} else {
						// we're using another target, map to the host
						for(unsigned int i=1; i<size; ++i) {
							const char * name = caps->getButtonName(i-1);
							unsigned int off = capabilities.findButtonOffset(name);
							if(off==-1U) {
								cerr << "*** Warning line " << linenum << ", button '" << name << "' from robot " << caps->getRobotName() << " cannot be mapped to host " << RobotName << endl;
							} else {
								loadSaveSensors->buttons[off] = atof(words[i].c_str());
							}
						}
					}
				} break;
				case SECTION_SENSORS: {
					if(loadSaveSensors==NULL)
						break;
					unsigned int size=words.size();
					if(size-1!=caps->getNumSensors()) {
						cerr << "*** ERROR line " << linenum << ", " << caps->getRobotName() << " expected " << caps->getNumSensors() << " values, got " << (size-1) << endl;
						return 0;
					}
					if(caps==&capabilities) {
						// we're using the "native" target... make things a bit quicker
						for(unsigned int i=1; i<size; ++i)
							loadSaveSensors->sensors[i-1] = atof(words[i].c_str());
					} else {
						// we're using another target, map to the host
						for(unsigned int i=1; i<size; ++i) {
							const char * name = caps->getSensorName(i-1);
							unsigned int off = capabilities.findSensorOffset(name);
							if(off==-1U) {
								cerr << "*** Warning line " << linenum << ", sensor '" << name << "' from robot " << caps->getRobotName() << " cannot be mapped to host " << RobotName << endl;
							} else {
								loadSaveSensors->sensors[off] = atof(words[i].c_str());
							}
						}
					}
				} break;
				case SECTION_PIDDUTIES: {
					if(loadSaveSensors==NULL)
						break;
					unsigned int size=words.size();
					if(size-1!=caps->getNumPIDJoints()) {
						cerr << "*** ERROR line " << linenum << ", " << caps->getRobotName() << " expected " << caps->getNumPIDJoints() << " values, got " << (size-1) << endl;
						return 0;
					}
					if(caps==&capabilities) {
						for(unsigned int i=1; i<size; ++i)
							loadSaveSensors->pidduties[i-1] = atof(words[i].c_str());
					} else {
						// we're using another target, map to the host
						unsigned int pidoff = caps->getPIDJointOffset();
						for(unsigned int i=1; i<size; ++i) {
							const char * name = caps->getOutputName(pidoff+i-1);
							unsigned int off = capabilities.findOutputOffset(name);
							if(off==-1U) {
								cerr << "*** Warning line " << linenum << ", output '" << name << "' from robot " << caps->getRobotName() << " cannot be mapped to host " << RobotName << endl;
							} else if(off<PIDJointOffset || off>=PIDJointOffset+NumPIDJoints) {
								cerr << "*** Warning line " << linenum << ", output '" << name << "' from robot " << caps->getRobotName() << " does not map to a PID joint on the local host " << RobotName << endl;
							} else {
								loadSaveSensors->pidduties[off-PIDJointOffset] = atof(words[i].c_str());
							}
						}
					}
				} break;
			} // end of section name 'switch'
		} // end of section name found 'else'
	} // end of not-a-keyword 'else'
	return true;
}
			
//Why do i need a cast to const char**??? It doesn't work without, should be automatic... grrrr
unsigned int PostureEngine::saveBuffer(char buf[], unsigned int len) const {
	unsigned int origlen=len;
	int written=snprintf(buf,len,"#POS\n");
	if(!checkInc(written,buf,len,"*** ERROR PostureEngine save failed on header\n")) return 0;
	if(saveFormatCondensed) {
#ifndef PLATFORM_APERIOS
		written=snprintf(buf,len,"condensed %s\n",RobotName);
#else
		written=snprintf(buf,len,"condensed %s\n",RobotName.c_str());
#endif
		if(!checkInc(written,buf,len,"*** ERROR PostureEngine save condensed header failed\n")) return 0;
		if(loadSaveSensors!=NULL) {
			written=snprintf(buf,len,"meta-info = %u %u\n",loadSaveSensors->lastSensorUpdateTime,loadSaveSensors->frameNumber);
			if(!checkInc(written,buf,len,"*** ERROR PostureEngine save pidduty failed\n")) return 0;
		}
		bool weightsAllEqual=true;
		float weightsVal=cmds[0].weight;
		for(unsigned int i=1; i<NumOutputs && weightsAllEqual; i++)
			weightsAllEqual=(cmds[i].weight==weightsVal);
		if(!weightsAllEqual || weightsVal!=0) { //if they're all 0, skip outputs and weights
			written=snprintf(buf,len,"outputs =");
			if(!checkInc(written,buf,len,"*** ERROR PostureEngine save outputs header failed\n")) return 0;
			for(unsigned int i=0; i<NumOutputs; i++) {
				written=snprintf(buf,len," %g",cmds[i].value);
				if(!checkInc(written,buf,len,"*** ERROR PostureEngine save output failed\n")) return 0;
			}
			if(!weightsAllEqual || weightsVal!=1) { //if they're all 1, skip weights
				written=snprintf(buf,len,"\nweights =");
				if(!checkInc(written,buf,len,"*** ERROR PostureEngine save weights header failed\n")) return 0;
				for(unsigned int i=0; i<NumOutputs; i++) {
					written=snprintf(buf,len," %g",cmds[i].weight);
					if(!checkInc(written,buf,len,"*** ERROR PostureEngine save weight failed\n")) return 0;
				}
			}
			written=snprintf(buf,len,"\n");
			if(!checkInc(written,buf,len,"*** ERROR PostureEngine save final newline failed\n")) return 0;
		}
		if(loadSaveSensors!=NULL) {
			written=snprintf(buf,len,"buttons =");
			if(!checkInc(written,buf,len,"*** ERROR PostureEngine save buttons header failed\n")) return 0;
			for(unsigned int i=0; i<NumButtons; i++) {
				written=snprintf(buf,len," %g",loadSaveSensors->buttons[i]);
				if(!checkInc(written,buf,len,"*** ERROR PostureEngine save button failed\n")) return 0;
			}
			written=snprintf(buf,len,"\nsensors =");
			if(!checkInc(written,buf,len,"*** ERROR PostureEngine save sensors header failed\n")) return 0;
			for(unsigned int i=0; i<NumSensors; i++) {
				written=snprintf(buf,len," %g",loadSaveSensors->sensors[i]);
				if(!checkInc(written,buf,len,"*** ERROR PostureEngine save sensor failed\n")) return 0;
			}
			written=snprintf(buf,len,"\npidduties =");
			if(!checkInc(written,buf,len,"*** ERROR PostureEngine save pidduties header failed\n")) return 0;
			for(unsigned int i=0; i<NumPIDJoints; i++) {
				written=snprintf(buf,len," %g",loadSaveSensors->pidduties[i]);
				if(!checkInc(written,buf,len,"*** ERROR PostureEngine save pidduty failed\n")) return 0;
			}
			written=snprintf(buf,len,"\n");
			if(!checkInc(written,buf,len,"*** ERROR PostureEngine save final newline failed\n")) return 0;
		}		
	} else {
		if(loadSaveSensors!=NULL) {
			written=snprintf(buf,len,"<meta-info>\n	 timestamp\t%u\n	framenumber\t%u\n</meta-info>\n",loadSaveSensors->lastSensorUpdateTime,loadSaveSensors->frameNumber);
			if(!checkInc(written,buf,len,"*** ERROR PostureEngine sensor begin save failed\n")) return 0;
		}
		for(unsigned int i=0; i<NumOutputs; i++) {
			if(cmds[i].weight>0) {
				if(cmds[i].weight==1)
					written=snprintf(buf,len,"%s\t% .4f\n",outputNames[i],cmds[i].value);
				else
					written=snprintf(buf,len,"%s\t% .4f\t% .4f\n",outputNames[i],cmds[i].value,cmds[i].weight);
				if(!checkInc(written,buf,len,"*** ERROR PostureEngine save failed\n")) return 0;
			}
		}
		if(loadSaveSensors!=NULL) {
			written=snprintf(buf,len,"<buttons>\n");
			if(!checkInc(written,buf,len,"*** ERROR PostureEngine sensor begin save failed\n")) return 0;
			for(unsigned int i=0; i<NumButtons; i++) {
				written=snprintf(buf,len,"	%s\t% .4f\t\n",buttonNames[i],loadSaveSensors->buttons[i]);
				if(!checkInc(written,buf,len,"*** ERROR PostureEngine save button failed\n")) return 0;
			}
			written=snprintf(buf,len,"</buttons><sensors>\n");
			if(!checkInc(written,buf,len,"*** ERROR PostureEngine sensor end save failed\n")) return 0;
			for(unsigned int i=0; i<NumSensors; i++) {
				written=snprintf(buf,len,"	%s\t% .4f\t\n",sensorNames[i],loadSaveSensors->sensors[i]);
				if(!checkInc(written,buf,len,"*** ERROR PostureEngine save sensor failed\n")) return 0;
			}
			written=snprintf(buf,len,"</sensors><pidduties>\n");
			if(!checkInc(written,buf,len,"*** ERROR PostureEngine sensor end save failed\n")) return 0;
			for(unsigned int i=0; i<NumPIDJoints; i++) {
				written=snprintf(buf,len,"	%s\t% .4f\t\n",outputNames[i],loadSaveSensors->pidduties[i]);
				if(!checkInc(written,buf,len,"*** ERROR PostureEngine save pidduties failed\n")) return 0;
			}
			written=snprintf(buf,len,"</pidduties>\n");
			if(!checkInc(written,buf,len,"*** ERROR PostureEngine sensor end save failed\n")) return 0;
		}
	}
	written=snprintf(buf,len,"#END\n");
	if(!checkInc(written,buf,len,"*** ERROR PostureEngine save failed on #END\n")) return 0;
	return origlen-len;
}
				
unsigned int PostureEngine::loadFile(const char filename[]) {
	return LoadSave::loadFile(config->motion.makePath(filename).c_str());
}
unsigned int PostureEngine::saveFile(const char filename[]) const {
	return LoadSave::saveFile(config->motion.makePath(filename).c_str());
}

/*
 NEWMAT::ReturnMatrix
 Kinematics::getOrientation(unsigned int j) {
	 unsigned int c=-1U,l=-1U;
	 if(!lookup(j,c,l)) {
		 NEWMAT::Matrix A(4,4);
		 A<<ROBOOP::fourbyfourident;
		 A.Release(); return A;
	 }
	 update(c,l);
	 NEWMAT::Matrix R;
	 NEWMAT::ColumnVector p;
	 chains[c]->kine(R,p,j);
	 R.Release(); return R;
 }
 
 NEWMAT::ReturnMatrix
 Kinematics::getPosition(unsigned int j) {
	 unsigned int c=-1U,l=-1U;
	 if(!lookup(j,c,l)) {
		 NEWMAT::Matrix A(4,4);
		 A<<ROBOOP::fourbyfourident;
		 A.Release(); return A;
	 }
	 update(c,l);
	 NEWMAT::Matrix R;
	 NEWMAT::ColumnVector p;
	 chains[c]->kine(R,p,j);
	 p.Release(); return p;
 }
 */
				
bool
PostureEngine::solveLinkPosition(const NEWMAT::ColumnVector& Pobj, unsigned int j, const NEWMAT::ColumnVector& Plink) {
	unsigned int c=-1U,l=-1U;
	if(!lookup(j,c,l))
		return false;
	update(c,l);
	bool conv=false;
	NEWMAT::ColumnVector q=chains[c]->inv_kin_pos(Pobj,0,l,Plink,conv);
	for(unsigned int i=1; i<=l && i<=chainMaps[c].size(); i++)
		if(chainMaps[c][i]<NumOutputs)
			setOutputCmd(chainMaps[c][i],chains[c]->get_q(i));
	return conv;
}

bool
PostureEngine::solveLinkVector(const NEWMAT::ColumnVector& Pobj, unsigned int j, const NEWMAT::ColumnVector& Plink) {
	solveLinkPosition(Pobj,j,Plink);
	//This method is an approximation, not entirely precise or fast as it could be
	//Something to work on some more down the road! :)
	//(this method is shared with HeadPointerMC::lookAtPoint(x,y,z))
	NEWMAT::ColumnVector poE=baseToLink(j)*Pobj;
	if(poE.nrows()>3 && poE(4)!=0) {
		poE/=poE(4);
	}
	poE=poE.SubMatrix(1,3,1,1);
	NEWMAT::ColumnVector plE=Plink.SubMatrix(1,3,1,1);
	if(Plink.nrows()>3 && Plink(4)!=0)
		plE/=Plink(4);
	float plE2=plE.SumSquare();
	float plE_len=sqrt(plE2);
	float obj_comp_link=NEWMAT::DotProduct(plE,poE)/plE_len;
	if(obj_comp_link<plE_len)
		obj_comp_link=obj_comp_link*.975; //.975 is a bit of fudge - accounts for joints moving Plink when adjusting
	else
		obj_comp_link=obj_comp_link/.975; //.975 is a bit of fudge - accounts for joints moving Plink when adjusting
	NEWMAT::ColumnVector obj_proj_link(4);
	obj_proj_link.SubMatrix(1,3,1,1)=obj_comp_link*plE/plE_len;
	obj_proj_link(4)=1;
	return solveLinkPosition(Pobj,j,obj_proj_link);
}

void
PostureEngine::update(unsigned int c, unsigned int l) {
	for(unsigned int j=1; j<=l; j++) {
		unsigned int tkout=chainMaps[c][j];
		if(tkout<NumOutputs)
			chains[c]->set_q(getOutputCmd(tkout).value,j);
	}
}

/*! @file
 * @brief Implements PostureEngine, a base class for managing the values and weights of all the outputs
 * @author ejt (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.41 $
 * $State: Exp $
 * $Date: 2007/11/11 23:57:23 $
 */

