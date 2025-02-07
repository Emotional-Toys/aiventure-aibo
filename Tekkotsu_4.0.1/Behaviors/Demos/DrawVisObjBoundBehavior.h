//-*-c++-*-
#ifndef INCLUDED_DrawVisObjBoundBehavior_h_
#define INCLUDED_DrawVisObjBoundBehavior_h_

#include "Behaviors/BehaviorBase.h"
#include "Events/EventRouter.h"
#include "Shared/ProjectInterface.h"
#include "Events/VisionObjectEvent.h"
#include "Events/FilterBankEvent.h"
#include "Vision/Graphics.h"
#include "Shared/Config.h"
#include "Vision/RawCameraGenerator.h"

//! Draws a boundary box in camera frame around all detected vision object events
class DrawVisObjBoundBehavior : public BehaviorBase {
public:
	//! constructor
	DrawVisObjBoundBehavior() : BehaviorBase("DrawVisObjBoundBehavior"), objs(), drawn(0) {}

	virtual void DoStart() {
		BehaviorBase::DoStart(); // do this first (required)
		erouter->addListener(this, EventBase::visObjEGID);
		erouter->addListener(this, EventBase::visRawCameraEGID,ProjectInterface::visRawCameraSID,EventBase::statusETID);
		erouter->addListener(this, EventBase::visSegmentEGID,ProjectInterface::visSegmentSID,EventBase::statusETID);
		//This part isn't done -- we can't draw into RLE images until RLEGraphics is written
		//  erouter->addListener(this, EventBase::visRLEEGID,ProjectInterface::visRLESID,EventBase::statusETID);
	}

	virtual void DoStop() {
		erouter->removeListener(this); //generally a good idea, unsubscribe all
		BehaviorBase::DoStop(); // do this last (required)
	}

	virtual void processEvent(const EventBase& e) {
		if(e.getGeneratorID()==EventBase::visObjEGID) {
			if(drawn>=2) {
				objs.clear();
				drawn=0;
			}
			if(e.getTypeID()!=EventBase::deactivateETID) {
				const VisionObjectEvent& vis=dynamic_cast<const VisionObjectEvent&>(e);
				Rect r={vis.getLeft(),vis.getTop(),vis.getWidth(),vis.getHeight()};
				objs.push_back(r);
			}
		} else {
			//cache current layer, channel, and pen color (depends on whether we're handling the seg cam or the raw cam
			unsigned int layer, chan;
			unsigned char color;
			const FilterBankEvent& fbe=dynamic_cast<const FilterBankEvent&>(e);
			if(e.getGeneratorID()==EventBase::visRawCameraEGID) {
				layer=fbe.getNumLayers()-1-config->vision.rawcam.y_skip;
				chan=RawCameraGenerator::CHAN_Y;
				color=255;
			} else if(e.getGeneratorID()==EventBase::visSegmentEGID) {
				layer=fbe.getNumLayers()-1-config->vision.segcam.skip;
				chan=config->vision.segcam.channel;
				color=7;
			} else {
				std::cerr << "WARNING: " << getClassName() << " received vision event from unknown generator" << std::endl;
				return;
			}

			//do the drawing
			Graphics g(*fbe.getSource(),layer,chan);
			g.setColor(color);
			for(std::vector<Rect>::const_iterator it=objs.begin(); it!=objs.end(); ++it)
				g.drawRect(it->x,it->y,it->w,it->h);
			
			//this is only needed until RLEGraphics is in place
			//  in the mean time, we trigger the RLE generator to recompress the modified seg cam image
			if(config->vision.segcam.compression==Config::vision_config::SegCamConfig::COMPRESS_RLE)
				ProjectInterface::defRLEGenerator->invalidateCaches();
			
			drawn++;
		}
	}

	static std::string getClassDescription() { return "Draws a boundary box in camera frame around all detected vision object events"; }
	virtual std::string getDescription() const { return getClassDescription(); }
	

protected:
	//! a simple structure for storing bounding box info for each detected object
	struct Rect {
		float x; //!< left edge, in resolution-independent coordinates
		float y; //!< top edge, in resolution-independent coordinates
		float w; //!< width, in resolution-independent coordinates
		float h; //!< height, in resolution-independent coordinates
	};

	std::vector<Rect> objs; //!< vector of boundary boxes from the current frame
	unsigned int drawn; //!< a counter of the number of frames that have been updated, when this hits 2, the next vision object will clear the list
	
private:
	DrawVisObjBoundBehavior(const DrawVisObjBoundBehavior&); //!< don't call (copy constructor)
	DrawVisObjBoundBehavior& operator=(const DrawVisObjBoundBehavior&); //!< don't call (assignment operator)
};

/*! @file
 * @brief Defines DrawVisObjBoundBehavior, which draws a boundary box in camera frame around all detected vision object events
 * @author ejt (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.7 $
 * $State: Exp $
 * $Date: 2007/01/30 22:56:19 $
 */

#endif
