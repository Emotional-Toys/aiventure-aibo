#include "Config.h"
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <cerrno>
#include <cstring>
#include <cctype>
#include <string>
#ifdef PLATFORM_APERIOS
#  include <OPENR/OPENRAPI.h>
#else
#include <sys/param.h>
#include <unistd.h>
#endif
#include "Shared/ProjectInterface.h"
#include "Shared/string_util.h"
//#include "Behaviors/Mon/RawCamBehavior.h"
#include "Vision/RawCameraGenerator.h"
//#include "Vision/SegmentedColorGenerator.h"
//#include "Wireless/Socket.h"
#include "Shared/debuget.h"

/*! Explicitly declaring these values allows us to relegate storage to a 
*  single translation unit instead of having it reallocated every time
*  the class is referenced.  This actually adds up to megabytes of
*  symbols (especially with debugging info). */
//! @name plist::NamedEnumeration statics
//! provides string names for enumeration values (see plist::NamedEnumeration and #INSTANTIATE_NAMEDENUMERATION_STATICS)
INSTANTIATE_NAMEDENUMERATION_STATICS(Config::main_config::consoleMode_t);
INSTANTIATE_NAMEDENUMERATION_STATICS(Config::sound_config::volume_levels);
INSTANTIATE_NAMEDENUMERATION_STATICS(Config::transports);
INSTANTIATE_NAMEDENUMERATION_STATICS(Config::vision_config::RawCamConfig::compression_t);
INSTANTIATE_NAMEDENUMERATION_STATICS(Config::vision_config::RawCamConfig::encoding_t);
INSTANTIATE_NAMEDENUMERATION_STATICS(Config::vision_config::SegCamConfig::compression_t);
INSTANTIATE_NAMEDENUMERATION_STATICS(Config::vision_config::gain_levels);
INSTANTIATE_NAMEDENUMERATION_STATICS(Config::vision_config::shutter_speeds);
INSTANTIATE_NAMEDENUMERATION_STATICS(Config::vision_config::white_balance_levels);
INSTANTIATE_NAMEDENUMERATION_STATICS(J_DCT_METHOD);
//@}

Config* config=NULL;
const std::locale& Config::curLocale=std::locale::classic();

const std::string ConfigDictionary::msPrefix="Model";
const std::string ConfigDictionary::msSep=":";
//const std::string ConfigDictionary::msNum="#";
std::string ConfigDictionary::curModel;

const char * Config::xmlIntro1="<?xml version";
const char * Config::xmlIntro2="<!DOCTYPE ";
const char * Config::xmlIntro3="<plist";

const char * Config::transport_names[] = { "UDP", "TCP", "" };

const char * Config::vision_config::dct_method_names[4] = { "islow", "ifast", "float", "" };
const char * Config::vision_config::RawCamConfig::encoding_names[Config::vision_config::RawCamConfig::NUM_ENCODINGS+1] = { "color", "grayscale", "" };
const char * Config::vision_config::RawCamConfig::compression_names[Config::vision_config::RawCamConfig::NUM_COMPRESSIONS+1] = { "none", "jpeg", "" };
const char * Config::vision_config::SegCamConfig::compression_names[Config::vision_config::SegCamConfig::NUM_COMPRESSIONS+1] = { "none", "rle", "" };

const char * Config::main_config::consoleModeNames[Config::main_config::NUM_CONSOLE_MODES+1] = { "controller", "textmsg", "auto", "" };

bool ConfigDictionary::loadXMLNode(const std::string& key, xmlNode* val, const std::string& comment) {
	// if not a model specific key, just load it normally
	if(key.substr(0,msPrefix.size())!=msPrefix)
		return plist::Dictionary::loadXMLNode(key,val,comment);
	
	//otherwise, strip off enumeration serial number (if any)
	//std::string k=key.substr(0,key.find(msNum));
	const std::string& k=key;
	
	//see if additional parameters in the key
	if(k.size()<=msPrefix.size() || k.substr(msPrefix.size(),msSep.size())!=msSep) {
		// not a valid model specific string, treat as normal key
		return plist::Dictionary::loadXMLNode(key,val,comment);
	}
	
	//prefix plus pattern (at least) -- check pattern
	std::string::size_type patStart=msPrefix.size()+msSep.size();
	std::string::size_type patEnd=k.find(msSep,patStart);
	std::string pattern=k.substr(patStart,patEnd-patStart);
	if(!matchNoCase(curModel,pattern))
		return false; //doesn't apply to the current model
	if(patEnd==std::string::npos) {
		// prefix plus pattern only -- load subnodes
		loadXML(val);
		return true;
	} else {
		// prefix plus pattern plus single item -- load it
		return plist::Dictionary::loadXMLNode(k.substr(patEnd+1),val,comment);
	}
}
bool ConfigDictionary::saveOverXMLNode(xmlNode* k, xmlNode* val, const std::string& key, std::string comment, const std::string& indentStr, std::set<std::string>& seen) const {
	// if not a model specific key, just load it normally
	if(key.substr(0,msPrefix.size())!=msPrefix)
		return plist::Dictionary::saveOverXMLNode(k,val,key,comment,indentStr,seen);
	
	//otherwise, strip off enumeration serial number (if any)
	//std::string ks=key.substr(0,key.find(msNum));
	const std::string& ks=key;
	
	//see if additional parameters in the key
	if(ks.size()<=msPrefix.size() || ks.substr(msPrefix.size(),msSep.size())!=msSep) {
		// not a valid model specific string, treat as normal key
		return plist::Dictionary::saveOverXMLNode(k,val,key,comment,indentStr,seen);
	}
	
	//prefix plus pattern (at least) -- check pattern
	std::string::size_type patStart=msPrefix.size()+msSep.size();
	std::string::size_type patEnd=ks.find(msSep,patStart);
	std::string pattern=ks.substr(patStart,patEnd-patStart);
	if(!matchNoCase(curModel,pattern))
		return true; //doesn't apply to the current model (still return true though because it's not an error, just a no-op)
	if(patEnd==std::string::npos) {
		// prefix plus pattern only -- save subnodes
		saveXML(val,true,seen);
		return true;
	} else {
		// prefix plus pattern plus single item -- save over it
		const std::string skey=ks.substr(patEnd+1);
		return plist::Dictionary::saveOverXMLNode(k,val,skey,comment,indentStr,seen);
	}
}

bool ConfigDictionary::matchNoCase(const std::string& model, const std::string& pattern) {
	unsigned int i=0;
	if(i==pattern.size() && i==model.size())
		return true;
	if(i==pattern.size() || i==model.size())
		return false;
	while(pattern[i]!='*') {
		if(toupper(pattern[i])!=toupper(model[i]))
			return false;
		i++;
		if(i==pattern.size() && i==model.size())
			return true;
		if(i==pattern.size() || i==model.size())
			return false;
	}
	i=pattern.size()-1;
	unsigned int j=model.size()-1;
	while(pattern[i]!='*') {
		if(toupper(pattern[i])!=toupper(model[j]))
			return false;
		i--; j--;
	}
	return true;
}

void Config::saveXML(xmlNode* node, bool onlyOverwrite, std::set<std::string>& seen) const {
	if(node==NULL)
		return;
	if(node->children==NULL) {
		std::string indentStr=getIndentationPrefix(node);
		const char* headerComment="\n"
			"##################################################################\n"
			"######################   Tekkotsu config   #######################\n"
			"##################################################################\n"
			//note the use of "s to break the string to avoid keyword substitution in the header...
			"#####################   $" "Revision:  $   ######################\n"
			"################   $" "Date:  $   ################\n"
			"##################################################################\n"
			"\n"
			"This is an XML-based format using the Property List (plist) layout.\n"
			"\n"
			"As a slight extension to standard plists, you can specify\n"
			"model-specific settings by prepending a key with:\n"
			"   Model:MODEL_PATTERN:KEY_NAME\n"
			"For example, to use different 'thresh' settings on the ERS-2xx\n"
			"series vs. the ERS-7 model, you would use the keys:\n"
			"   Model:ERS-2*:thresh\n"
			"   Model:ERS-7:thresh\n"
			"You can filter several items in a group by leaving off the second\n"
			"':' and name, and providing a dictionary as the value.  If the\n"
			"model matches, all entries from the dictionary are imported into\n"
			"the parent dictionary.\n"
			"\n"
			"You can change these settings at run time from the Controller\n"
			"by entering the command:\n"
			"   !set section_name.variable=value\n"
			"or by using the configuration editor found in the \"File Access\"\n"
			"menu.  Paths are assumed to be relative to the 'project/ms/'\n"
			"directory.  For example, the primary configuration file would be\n"
			"referenced as 'config/tekkotsu.xml'\n"
			"\n"
			"See also the 'simulator.xml' file, which provides you the ability\n"
			"to override settings when running in the simulator\n";
		xmlAddChild(node,xmlNewComment((const xmlChar*)headerComment));
		xmlAddChild(node,xmlNewText((const xmlChar*)("\n"+indentStr).c_str()));
	}
	ConfigDictionary::saveXML(node,onlyOverwrite,seen);
}

unsigned int Config::loadBuffer(const char buf[], unsigned int len) {
	if(strncmp(buf,xmlIntro1,strlen(xmlIntro1))==0 || strncmp(buf,xmlIntro2,strlen(xmlIntro2))==0 || strncmp(buf,xmlIntro3,strlen(xmlIntro3))==0)
		return plist::Dictionary::loadBuffer(buf,len);
	return loadOldFormat(buf,len);
}

unsigned int Config::loadFile(const char* filename) {
	std::string path=portPath(filename);
	bool isXML;
	{
		std::ifstream in(path.c_str());
		std::string line;
		std::getline(in,line);
		isXML = (strncmp(line.c_str(),xmlIntro1,strlen(xmlIntro1))==0 || strncmp(line.c_str(),xmlIntro2,strlen(xmlIntro2))==0 || strncmp(line.c_str(),xmlIntro3,strlen(xmlIntro3))==0);
	}
	if(isXML)
		return plist::Dictionary::loadFile(path.c_str());
	FILE* fp = fopen(path.c_str(), "r");
	if (fp==NULL) {
		std::cerr << "ERROR: Config could not open file '" << path.c_str() << "' for loading." << std::endl;
		return 0;
	}
	try {
		unsigned int ans=loadOldFormat(fp);
		fclose(fp);
		return ans;
	} catch(...) {
		fclose(fp);
		throw;
	}
}

unsigned int Config::loadFileStream(FILE* f) {
	std::string line;
	char c=fgetc(f);
	while(c!='\n' && c!=EOF) {
		line+=c;
		c=fgetc(f);
	}
	bool isXML = (strncmp(line.c_str(),xmlIntro1,strlen(xmlIntro1))==0 || strncmp(line.c_str(),xmlIntro2,strlen(xmlIntro2))==0 || strncmp(line.c_str(),xmlIntro3,strlen(xmlIntro3))==0);
	if(fseek(f,line.size()+1,SEEK_CUR)!=0) {
#ifdef PLATFORM_APERIOS
		int merrno=errno;
		std::cerr << "Warning: error on seek: " << strerror(merrno) << std::endl;
#else
		char err[100];
		strerror_r(errno,err,100);
		std::cerr << "Warning: error on seek: " << err << std::endl;
#endif
	}
	if(isXML)
		return plist::Dictionary::loadFileStream(f);
	return loadOldFormat(f);
}

void Config::setFileSystemRoot(const std::string& fsr) {
#ifdef PLATFORM_APERIOS
	fsRoot=fsr;
#else
	char buf[MAXPATHLEN+2];
	if(getcwd(buf,MAXPATHLEN+2)==NULL)
		perror("Config::setFileSystemRoot(): getcwd");
	buf[MAXPATHLEN+1]='\0';
	std::string sbuf(buf);
	if(sbuf[sbuf.size()-1]!='/' && fsr[0]!='/')
		fsRoot=sbuf+'/'+fsr;
	else if(sbuf[sbuf.size()-1]=='/' && fsr[0]=='/')
		fsRoot=sbuf+fsr.substr(1);
	else
		fsRoot=sbuf+fsr;
#endif
}


std::string Config::portPath(const std::string& path) const {
	if(fsRoot.size()==0)
		return path;
	if(path.size()==0)
		return fsRoot;
	if(path.substr(0,fsRoot.size())==fsRoot)
		return path;
	else if(fsRoot[fsRoot.size()-1]=='/') {
	  if(path[0]!='/')
	    return fsRoot+path;
	  else
	    return fsRoot+path.substr(1);
	} else {
	  if(path[0]!='/')
	    return fsRoot+'/'+path;
	  else
	    return fsRoot+path;
	}
}

void* Config::setValue(const std::string& section, std::string key, const std::string& value) {
	if(section=="vision") {
		if (key=="resolution") {
			if (value=="full") {
				vision.resolution=1;
			} else if (value=="half") {
				vision.resolution=2;
			} else if (value=="quarter") {
				vision.resolution=3;
			}
			return &vision.resolution;
		} else if (key=="rawcam_encoding") {
			if (value=="color") {
				vision.rawcam.encoding=vision_config::RawCamConfig::ENCODE_COLOR;
				vision.rawcam.channel=RawCameraGenerator::CHAN_Y;
			} else if (value=="y_only") {
				vision.rawcam.encoding=vision_config::RawCamConfig::ENCODE_SINGLE_CHANNEL;
				vision.rawcam.channel=RawCameraGenerator::CHAN_Y;
			} else if (value=="uv_only") {
				vision.rawcam.encoding=vision_config::RawCamConfig::ENCODE_COLOR;
				vision.rawcam.channel=-1;
			} else if (value=="u_only") {
				vision.rawcam.encoding=vision_config::RawCamConfig::ENCODE_SINGLE_CHANNEL;
				vision.rawcam.channel=RawCameraGenerator::CHAN_U;
			} else if (value=="v_only") {
				vision.rawcam.encoding=vision_config::RawCamConfig::ENCODE_SINGLE_CHANNEL;
				vision.rawcam.channel=RawCameraGenerator::CHAN_V;
			} else if (value=="y_dx_only") {
				vision.rawcam.encoding=vision_config::RawCamConfig::ENCODE_SINGLE_CHANNEL;
				vision.rawcam.channel=RawCameraGenerator::CHAN_Y_DX;
			} else if (value=="y_dy_only") {
				vision.rawcam.encoding=vision_config::RawCamConfig::ENCODE_SINGLE_CHANNEL;
				vision.rawcam.channel=RawCameraGenerator::CHAN_Y_DY;
			} else if (value=="y_dxdy_only") {
				vision.rawcam.encoding=vision_config::RawCamConfig::ENCODE_SINGLE_CHANNEL;
				vision.rawcam.channel=RawCameraGenerator::CHAN_Y_DXDY;
			}
			return &vision.rawcam.encoding;
		} else if (key=="focal_len_x" || key=="focal_len_y"
			 || key=="principle_point_x" || key=="principle_point_y"
			 || key=="skew" || key=="kc1_r2" || key=="kc2_r4"
			 || key=="kc5_r6" || key=="kc3_tan1" || key=="kc4_tan2"
			 || key=="calibration_res_x" || key=="calibration_res_y" )
		{
			key="calibration."+key;
		} else if (key.compare(0,7,"rawcam_")==0) {
			key[6]='.';
		} else if (key.compare(0,7,"segcam_")==0) {
			key[6]='.';
		} else if (key.compare(0,10,"regioncam_")==0) {
			key[9]='.';
		} else if (key=="region_port" || key=="region_transport") {
			key.replace(0,7,"regioncam.");
		}
	} else if(section=="motion") {
		std::string calibratePrefix="calibrate:";
		if (key.substr(0,calibratePrefix.size())==calibratePrefix) {
			std::string keyval = key.substr(calibratePrefix.size());
			OutputConfig<plist::Primitive<float> >::const_iterator it = motion.calibration_scale.findEntry(keyval);
			plist::Primitive<float>* prim = NULL;
			if(it!=motion.calibration_scale.end())
				prim = dynamic_cast<plist::Primitive<float> *>(it->second);
			if(prim==NULL) {
				std::cout << "WARNING: Could not match '" << key.substr(10) << "' as calibration parameter" << std::endl;
				return NULL;
			}
			//std::cout << "setting " << key.substr(10) << " to " << value << std::endl;
			prim->set(value);
			*prim = 1 / *(prim);
			return prim;
		}
	} else if(section=="sound") {
		if (key=="streaming.mic_bits") {
			key="streaming.mic_sample_bits";
		} else if (key=="streaming.speaker_frame_len") {
			key="streaming.speaker_frame_length";
		}
	} else if(section=="main") {
		if (key=="debug_level" || key=="error_level" || key=="verbose_level")
			return NULL; //drop, unused items
	}
	
	plist::Collection * sct=dynamic_cast<plist::Collection*>(resolveEntry(section));
	if(sct==NULL)
		return NULL;
	plist::ObjectBase * entry=sct->resolveEntry(key);
	if(entry==NULL) {
		std::cerr << "Unknown " << section << " key '" << key << "' for value " << value << std::endl;
		return NULL;
	} else if(plist::PrimitiveBase * prim=dynamic_cast<plist::PrimitiveBase*>(entry)) {
		try {
			prim->set(value);
		} catch(...) {
			std::cerr << "Error setting " << section << " key '" << key << "' to value " << value << std::endl;
			return NULL;
		}
		return prim;
	} else if(plist::ArrayBase::StringConversion * arr=dynamic_cast<plist::ArrayBase::StringConversion*>(entry)) {
		try {
			arr->addValue(value);
		} catch(...) {
			std::cerr << "Error setting " << section << " key '" << key << "' to value " << value << std::endl;
			return NULL;
		}
		return arr;
	} else {
		std::cerr << "Unknown type of " << section << " key '" << key << "' for value " << value << std::endl;
		return NULL;
	}
}

unsigned int Config::loadOldFormat(const char buf[], unsigned int len) {
	if (buf==NULL) return 0;
	unsigned int numRead=0, used=0;
	char line[256];
	unsigned int lineno=0;
	std::vector<std::string> modelStack;
	bool ignoring=false;
	std::string section="invalid";
	while (numRead<len && (used=sscanf(&buf[numRead],"%255[^\n]\n", line))!=(unsigned int)EOF) {
		numRead+=used;
		parseLine(line,++lineno,modelStack,ignoring,section);
	}
	return numRead;
}

unsigned int Config::loadOldFormat(FILE* fp) {
	if (fp==NULL) return 0;
	unsigned int numRead=0;
	char line[256];
	unsigned int lineno=0;
	std::vector<std::string> modelStack;
	bool ignoring=false;
	std::string section="invalid";
	while (fgets(line,256,fp)!=NULL) {
		numRead+=strlen(line);
		parseLine(line,++lineno,modelStack,ignoring,section);
	}
	return numRead;
}

void Config::parseLine(const char buf[], unsigned int lineno, std::vector<std::string>& modelStack, bool& ignoring, std::string& section) {
	char key[30], value[50];
	while(std::isspace(*buf))
		buf++;
	if(buf[0]=='#')
		return;
	if (sscanf(buf,"<%29[^>]>",key)>0) {
		if(key[0]=='/') {
			if(modelStack.size()==0) {
				printf("WARNING: not in a model specific section, line %d\n",lineno);
				return;
			}
			bool subset=matchNoCase(&key[1],modelStack.back());
			bool superset=matchNoCase(modelStack.back(),&key[1]);
			if(subset && superset) {
				//printf("leaving modelsection %s\n",modelStack.back().c_str());
				modelStack.pop_back();
			} else if(superset) {
				while(modelStack.size()>0) {
					//printf("leaving modelsection %s (==%s)\n",modelStack.back().c_str(),&key[1]);
					modelStack.pop_back();
					if(!matchNoCase(modelStack.back(),&key[1]))
						break;
				}
			} else
				printf("WARNING: config model mismatch, line %d\n",lineno);
			
			ignoring=false; //scan through current model selection stack, see if we're still ignoring
			for(unsigned int i=0; i<modelStack.size(); i++)
				if(!matchNoCase(curModel,modelStack[i])) {
					ignoring=true;
					break;
				}
					//printf("ignoring==%d\n",ignoring);
					
		} else {
			modelStack.push_back(key);
			//printf("entering section %s\n",modelStack.back().c_str());
			ignoring=ignoring || !matchNoCase(curModel,key);
			//printf("ignoring==%d\n",ignoring);
		}
	} else if(!ignoring) {
		if (sscanf(buf,"[%29[^]]]",key)>0) {
			section=key;
			std::transform(section.begin(), section.end(), section.begin(), (int(*)(int)) std::tolower);
			//std::cout << "now parsing section " << section << std::endl;
			plist::Collection * sct=dynamic_cast<plist::Collection*>(resolveEntry(section));
			if(sct==NULL)
				std::cerr << "ERROR: Unknown configuration section " << section << std::endl;
		} else if (sscanf(buf,"%29[^ =] =%49s",key,value)>1) {
			//printf("setValue(%d,'%s','%s');\n",section,key,value);
			//void * var=
			setValue(section, string_util::trim(key), string_util::trim(value));
			/*if(var==NULL) {
				std::cerr << "WARNING: Config::setValue(\"" << section << "\", \"" << string_util::trim(key) << "\", \"" << string_util::trim(key) << "\") returned NULL" << std::endl;
				std::cerr << "         This probably indicates the section or key is invalid." << std::endl;
			}*/
		}
	}
}			

void Config::vision_config::computeRay(float x, float y, float& r_x, float& r_y, float& r_z) {
	x=(x+1)*calibration.calibration_res_x/2;
	y=(y+1)*calibration.calibration_res_y/2;
	float yd=(y-calibration.principle_point_y)/calibration.focal_len_y;
	float xd=(x-calibration.principle_point_x)/calibration.focal_len_x-calibration.skew*yd;
	float r2=xd*xd+yd*yd; //estimate of original
	float radial=(1 + calibration.kc1_r2*r2 + calibration.kc2_r4*r2*r2 + calibration.kc5_r6*r2*r2*r2);
	r_x=(xd - 2*calibration.kc3_tan1*xd*yd - calibration.kc4_tan2*(r2+2*xd*xd))/radial; //estimating tangential component
	r_y=(yd - calibration.kc3_tan1*(r2+2*yd*yd) - 2*calibration.kc4_tan2*xd*yd)/radial; //estimating tangential component
	r_z=1;
}

//!provides a pixel hit in image by a ray going through the camera frame
/*! @param[in] r_x x value of the ray
*  @param[in] r_y y value of the ray
*  @param[in] r_z z value of the ray
*  @param[out] x x position in range [-1,1]
*  @param[out] y y position in range [-1,1] */
void Config::vision_config::computePixel(float r_x, float r_y, float r_z, float& x, float& y) {
	if(r_z==0) {
		x=y=0;
		return;
	}
	r_x/=r_z;
	r_y/=r_z;
	float r2 = r_x*r_x + r_y*r_y; //'r2' for 'radius squared', not 'ray number 2'
	float radial=(1 + calibration.kc1_r2*r2 + calibration.kc2_r4*r2*r2 + calibration.kc5_r6*r2*r2*r2);
	float xd = radial*r_x + 2*calibration.kc3_tan1*r_x*r_y + calibration.kc4_tan2*(r2+2*r_x*r_x);
	float yd = radial*r_y + calibration.kc3_tan1*(r2+2*r_y*r_y) + 2*calibration.kc4_tan2*r_x*r_y;
	x=calibration.focal_len_x*(xd+calibration.skew*yd)+calibration.principle_point_x;
	y=calibration.focal_len_y*yd+calibration.principle_point_y;
	x=2*x/calibration.calibration_res_x-1;
	y=2*y/calibration.calibration_res_y-1;
}

/*! @file
 * @brief Implements Config, which provides global access to system configuration information
 * @author ejt (Creator)
 * @author alokl (Original configuration system)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.71 $
 * $State: Exp $
 * $Date: 2007/11/16 15:53:49 $
 */

