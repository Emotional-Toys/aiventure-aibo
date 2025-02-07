#include "TestBehaviors.h"
#include "Vision/PNGGenerator.h"
#include "Sound/SoundManager.h"
#include <errno.h>

//better to put this here instead of the header
using namespace std; 

void SaveImagePyramidBehavior::saveImage(unsigned int layer, unsigned int chan, const std::string& name) {
	const unsigned int MAXPATH=128;
	char fname[MAXPATH];
	snprintf(fname,MAXPATH,name.c_str(),layer);
	FILE * f=fopen(fname,"w+");
	if(f==NULL) {
		int err=errno;
		serr->printf("Error opening file %s: %s\n",fname,strerror(err));
		sndman->playFile(config->controller.error_snd);
		return;
	}
	PNGGenerator * gen=dynamic_cast<PNGGenerator*>(ProjectInterface::defGrayscalePNGGenerator);
	unsigned char * img=gen->getImage(layer,chan);
	if(!gen->getImageCached(layer,chan)) {
		serr->printf("A PNG encoding error occurred whiel saving file %s\n",fname);
		sndman->playFile(config->controller.error_snd);
		//return;
	}
	unsigned int size=gen->getImageSize(layer,chan);
	unsigned int writ=fwrite(img,size,1,f);
	if(writ==0) {
		int err=errno;
		serr->printf("Error saving file %s: %s\n",fname,strerror(err));
		sndman->playFile(config->controller.error_snd);
		return;
	}
	if(fclose(f)!=0) {
		int err=errno;
		serr->printf("Error closing file %s: %s\n",fname,strerror(err));
		sndman->playFile(config->controller.error_snd);
		return;
	}
	sout->printf("Saved %s\n",fname);
}

/*! @file
 * @brief 
 * @author Ethan Tira-Thompson (ejt) (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.3 $
 * $State: Exp $
 * $Date: 2006/09/18 18:07:57 $
 */
