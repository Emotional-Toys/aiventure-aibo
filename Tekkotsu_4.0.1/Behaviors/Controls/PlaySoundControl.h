//-*-c++-*-
#ifndef INCLUDED_PlaySoundControl_h_
#define INCLUDED_PlaySoundControl_h_

#include "FileBrowserControl.h"
#include "Sound/SoundManager.h"

//! Upon activation, loads a position from a file name read from cin (stored in ms/data/motion...)
class PlaySoundControl : public FileBrowserControl {
 public:
	//! Constructor
	PlaySoundControl(const std::string& n)
		: FileBrowserControl(n,"Plays a sound from a user specified sound file",config->sound.root)
	{
		setFilter("*.wav");
	}
	//! Destructor
	virtual ~PlaySoundControl() {}

protected:
	//!does the actual loading of the MotionSequence
	virtual ControlBase* selectedFile(const std::string& f) {
		sndman->stopPlay();
		if(sndman)
			sndman->playFile(f.c_str());
		return this;
	}
};

/*! @file
 * @brief Defines PlaySoundControl, which when activated, plays a sound selected from the memory stick
 * @author ejt (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.10 $
 * $State: Exp $
 * $Date: 2007/01/29 01:16:57 $
 */

#endif
