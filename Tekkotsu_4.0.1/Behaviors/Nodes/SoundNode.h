//-*-c++-*-
#ifndef INCLUDED_SoundNode_h_
#define INCLUDED_SoundNode_h_

#include "Behaviors/StateNode.h"
#include "Events/EventRouter.h"
#include "Sound/SoundManager.h"

//! A simple StateNode that plays a sound upon startup and throws a status event on completion
/*! Doesn't automatically preload the sound buffer - if you want the sound file
 *  to be preloaded, you'll have to make the
 *  SoundManager::loadFile() / SoundManager::releaseFile() calls yourself.
 *  
 *  By default, sound playback will continue even after this node has been deactivated.
 *  If this is not the behavior you desire, set the #autostop flag (through setAutoStop())
 */
class SoundNode : public StateNode {
protected:
  std::string filename; //!< filename of sound to play, accessed through setFileName() and getFileName()
  SoundManager::Play_ID curplay_id; //!< holds the playback identification so it can be halted any time
	bool autostop; //!< if set to true by setAutoStop(), when this node is deactivated, playback will be halted.  Otherwise, playback will continue even after the node is deactivated

public:
  //! constructor, specify a sound file to play
  SoundNode(const std::string& soundfilename="") : 
    StateNode("SoundNode","SoundNode"), filename(soundfilename), curplay_id(SoundManager::invalid_Play_ID), autostop(false) {}

  //! constructor, specify instance name and sound file to play
  SoundNode(const std::string& nodename, const std::string& soundfilename) : 
    StateNode("SoundNode",nodename), filename(soundfilename), curplay_id(SoundManager::invalid_Play_ID), autostop(false) {}

  //! activate the node, starts playing the sound
  virtual void DoStart() {
    StateNode::DoStart();  // don't activate transitions until our listener has been added
		if(filename.size()>0) {
			curplay_id = sndman->playFile(filename);
			erouter->addListener(this,EventBase::audioEGID,curplay_id,EventBase::deactivateETID);
		}
  }

  //! deactivate the node, doesn't stop the sound playback unless the #autostop flag has been set
  virtual void DoStop() {
		if(autostop)
			stopPlay();
    erouter->removeListener(this);
    StateNode::DoStop();
  }

  //! receive audioEGID status event and throw stateMachineEGID status event
  virtual void processEvent(const EventBase&) {
		curplay_id = SoundManager::invalid_Play_ID;
		postCompletionEvent();
  }

  //! interrupts playing of the current sound
  void stopPlay() {
    sndman->stopPlay(curplay_id);
    curplay_id = SoundManager::invalid_Play_ID;
  }

  //! returns the name of the sound file associated with this node
  std::string getFileName() { return filename; }

  //! sets the name of the sound file associated with this node
  void setFileName(std::string &soundfilename) { filename = soundfilename; }

	//! returns the current status of the #autostop flag
	bool getAutoStop() { return autostop; }

	//! sets the #autostop flag
	void setAutoStop(bool stop) { autostop=stop; }

protected:
  //! constructor
  SoundNode(const std::string &classname, const std::string &nodename, const std::string &soundfilename) : 
    StateNode(classname,nodename), filename(soundfilename), curplay_id(SoundManager::invalid_Play_ID), autostop(false) {}
};

/*! @file
 * @brief Defines SoundNode, a simple StateNode that plays a sound and throws a status event upon completion
 * @author dst (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.13 $
 * $State: Exp $
 * $Date: 2006/09/18 18:07:59 $
 */

#endif
