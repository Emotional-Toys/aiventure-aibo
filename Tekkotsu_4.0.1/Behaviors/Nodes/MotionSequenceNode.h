//-*-c++-*-
#ifndef INCLUDED_MotionSequenceNode_h_
#define INCLUDED_MotionSequenceNode_h_

#include "Behaviors/StateNode.h"
#include "Motion/MotionManager.h"
#include "Motion/MotionSequenceMC.h"
#include "Motion/MMAccessor.h"
#include "Shared/debuget.h"

//! A StateNode for playing a MotionSequence (and looping it if desired)
/*! Eventually, i'd like to just build the looping functionality into
 *  MotionSequence, but in the mean time we have this. */
template<unsigned int SIZE>
class MotionSequenceNode : public StateNode {
public:
	//!constructor
	MotionSequenceNode()
		: StateNode("MotionSequenceNode","MotionSequenceNode"), msid(MotionManager::invalid_MC_ID), msidIsMine(false), looping(false), filename()
	{}

	//!constructor
	MotionSequenceNode(const std::string& nm, const std::string& file, bool loop=false)
		: StateNode("MotionSequenceNode",nm), msid(MotionManager::invalid_MC_ID), msidIsMine(false), looping(false), filename(file)
	{
		setLooping(loop);
	}

	//!destructor, check if we need to call our teardown
	~MotionSequenceNode() {
		if(issetup)
			teardown();
	}

	virtual void DoStart() {
		//std::cout << "MotionSequenceNode::DoStart(); " << std::endl;
		updateMS(filename);
		erouter->addListener(this, EventBase::motmanEGID);//, msid, EventBase::deactivateETID);
		StateNode::DoStart();
	}

	virtual void DoStop() {
		//std::cout << "MotionSequenceNode::DoStop(); " << std::endl;
		erouter->removeListener(this);
		motman->removeMotion(msid);
		msid=MotionManager::invalid_MC_ID;
		StateNode::DoStop();
	}

	virtual void teardown() {
		if(msidIsMine) {
			motman->removeMotion(msid);
			msid=MotionManager::invalid_MC_ID;
		}
		StateNode::teardown();
	}

	/* not ready yet
	// ! use this to force the MotionSequenceNode to use a shared MS - set to MotionManager::invalid_MC_ID to reset to internally generated MS
	virtual void setMSid(MotionManager::MC_ID id) {
	if(msidIsMine) {
	motman->removeMotion(msid);
	msid=MotionManager::invalid_MC_ID;
	}
	msid=id;
	msidIsMine=(id==MotionManager::invalid_MC_ID);
	}
	*/

	//! sets the file to play
	virtual void setFile(const std::string& file) {
		if(isActive())
			updateMS(file);
		else
			filename=file;
	}

	//! turns looping on or off
	virtual void setLooping(bool loop) { looping=loop; }

	virtual void processEvent(const EventBase& e) {
		ASSERTRET(e.getGeneratorID()==EventBase::motmanEGID,"Unknown event");
		if(e==EventBase(EventBase::motmanEGID,msid,EventBase::deactivateETID)) {
			msid=MotionManager::invalid_MC_ID;
			if(looping) {
				updateMS(filename);
			}
			postCompletionEvent(looping);
		}
	}

	//! returns true if currently looping
	virtual bool getLooping() { return looping; }

	//! use this to access the MS that the MotionSequenceNode is using
	virtual MotionManager::MC_ID getMSid() { return msid; }

	//! use this to access the MS that the MotionSequenceNode is using
	virtual MMAccessor<MotionSequenceMC<SIZE> > getMSAccessor() { return MMAccessor<MotionSequenceMC<SIZE> >(msid); }

	//! returns true if #msid was created (and will be destroyed) by this MotionSequenceNode - false if assigned by setMsid()
	virtual bool ownsMSid() { return msidIsMine; }

protected:
	//! resets the motion command and starts it playing
	void updateMS(const std::string& file) {
		if(msid==MotionManager::invalid_MC_ID) {
			msid=motman->addPrunableMotion(SharedObject<MotionSequenceMC<SIZE> >(file.c_str()));
			msidIsMine=true;
		} else {
			MMAccessor<MotionSequenceMC<SIZE> > ms(msid);
			ms->clear();
			ms->loadFile(file.c_str());
			ms->setTime(1);
		}
		filename=file;
	}

	MotionManager::MC_ID msid; //!< id of the motion command
	bool msidIsMine; //!< true if this class created the current motion command (and therefore should delete it when done)
	bool looping; //!< true if we should loop
	std::string filename; //!< filename of current motion sequence
};

typedef MotionSequenceNode<TinyMotionSequenceMC::CAPACITY> TinyMotionSequenceNode; //!< streamlined access to the standard template sizes
typedef MotionSequenceNode<SmallMotionSequenceMC::CAPACITY> SmallMotionSequenceNode; //!< streamlined access to the standard template sizes
typedef MotionSequenceNode<MediumMotionSequenceMC::CAPACITY> MediumMotionSequenceNode; //!< streamlined access to the standard template sizes
typedef MotionSequenceNode<LargeMotionSequenceMC::CAPACITY> LargeMotionSequenceNode; //!< streamlined access to the standard template sizes
typedef MotionSequenceNode<XLargeMotionSequenceMC::CAPACITY> XLargeMotionSequenceNode; //!< streamlined access to the standard template sizes

/*! @file
 * @brief Describes MotionSequenceNode, a StateNode for playing a MotionSequence (and looping it if desired)
 * @author ejt (Creator)
 *
 * $Author: dst $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.5 $
 * $State: Exp $
 * $Date: 2007/04/07 02:03:43 $
 */

#endif
