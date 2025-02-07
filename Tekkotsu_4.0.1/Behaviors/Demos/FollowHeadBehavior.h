//-*-c++-*-
#if !defined(INCLUDED_FollowHeadBehavior_h_) && defined(TGT_HAS_BUTTONS)
#define INCLUDED_FollowHeadBehavior_h_

#include "Behaviors/BehaviorBase.h"
#include "Motion/MotionManager.h"

//! Will walk where the head is pointing
/*! Press the chin button to loosen the head to point it, release the
 *  button to lock it again
 *
 *  Tilt of head determines x axis (forward/backward)<br>
 *  Roll of head determines y axis (sideways strafing)<br>
 *  Pan of head determines z axis (rotational)
 *
 *  The zero point of joint position is zero motion.  Since the tilt
 *  is asymmetric (can tilt down farther than it can tilt up), the
 *  full range of the down tilt isn't used - if you tilt down farther
 *  than you could tilt it back, it'll just clip the speed.  Besides,
 *  if the head is all the way down, it screws up the walk because
 *  the center of balance is changed.
 */
class FollowHeadBehavior : public BehaviorBase {
 public:

	//! just sets up the variables
	FollowHeadBehavior();

	//! calls DoStop() if isActive()
	virtual ~FollowHeadBehavior();

	//! Register for events and creates and adds two motion commands - a walker and a head pointer
	virtual void DoStart();

	//! Removes its two motion commands
	virtual void DoStop();

	//! Handles event processing
	/*! After every clock pulse, sets walk in direction of head */
	virtual void processEvent(const EventBase& e);

	static std::string getClassDescription() { return "Walks whereever you point the head - press the chin button to loosen the head, release to lock it"; }
	virtual std::string getDescription() const { return getClassDescription(); }

 protected:
	const EventBase head_release; //!< event mask for releasing head (chin button down)
	const EventBase head_lock;    //!< event mask for locking head (chin button up)
	const EventBase clock;        //!< event mask for updating walk direction (every 150 ms)
	MotionManager::MC_ID walker_id;      //!< MC_ID for walker
};

/*! @file
 * @brief Describes FollowHeadBehavior, walks where the head is pointing
 * @author ejt (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.6 $
 * $State: Exp $
 * $Date: 2007/03/05 01:19:08 $
 */

#endif
