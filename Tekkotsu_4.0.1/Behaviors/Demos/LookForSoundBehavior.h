//-*-c++-*-
#ifndef INCLUDED_LookForSoundBehavior_h_
#define INCLUDED_LookForSoundBehavior_h_

#include "Behaviors/BehaviorBase.h"
#include "Motion/HeadPointerMC.h"
#include "Events/DataEvent.h"
#include "Events/EventRouter.h"
#include "Shared/ODataFormats.h"
#include "Motion/MMAccessor.h"

//! Turns head to sound source, estimated by average volume difference between left and right ears
class LookForSoundBehavior : public BehaviorBase {
public:
	//! constructor
	LookForSoundBehavior() : BehaviorBase("LookForSoundBehavior"), mc_id(MotionManager::invalid_MC_ID){}

	virtual void DoStart()
	{
		BehaviorBase::DoStart();

		// You'll listen for sound and move your head
		mc_id = motman->addPersistentMotion(SharedObject<HeadPointerMC>());
		erouter->addListener(this,EventBase::micOSndEGID);
	}

	virtual void DoStop()
	{
		motman->removeMotion(mc_id);
		erouter->removeListener(this);
		BehaviorBase::DoStop();
	}

	virtual void processEvent(const EventBase& event)
	{
		if( event.getGeneratorID() == EventBase::micOSndEGID) {
			// Get to the sound buffer, inevitable warning on line 37
			// getData() is not specified for const data
			const DataEvent<const OSoundVectorData*> *de = 
				reinterpret_cast<const DataEvent<const OSoundVectorData*>*>( &event);

			OSoundVectorData *svd = const_cast<OSoundVectorData*>(de->getData());
			const short *d = ( const short *)svd->GetData(0);

			// Measure the energy of both channels
			// Samples are interleaved [l,r]
			double l = 0, r = 0;
			int sz = svd->GetInfo(0)->frameSize;
			for( int i = 0 ; i != sz ; i++){
				l += abs( d[2*i]);
				r += abs( d[2*i+1]);
			}

			// If there is sufficient energy coming in
			if( l+r > sz*1000.){
				MMAccessor<HeadPointerMC> mc(mc_id);
				double cur = state->outputs[HeadOffset+PanOffset];
				if( r > 1.3*l)
					// Move your head righward
					mc->setJoints( 0, cur-.2*M_PI/(r/l), 0);
				if( l > 1.3*r)
					// Move your head leftward
					mc->setJoints( 0, cur+.2*M_PI/(l/r), 0);
			}
		}
	}

	static std::string getClassDescription() { return "Turns head to sound source, estimated by average volume difference between left and right ears"; }
	virtual std::string getDescription() const { return getClassDescription(); }
	
protected:
	MotionManager::MC_ID mc_id; //!< the id of the HeadPointerMC which does the looking
};

/*! @file
 * @brief Defines LookForSoundBehavior, which turns head to sound source, estimated by average volume difference between left and right ears
 * @author Paris Smaragdis (paris AT media mit edu) (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.9 $
 * $State: Exp $
 * $Date: 2007/08/05 16:16:04 $
 */

#endif
