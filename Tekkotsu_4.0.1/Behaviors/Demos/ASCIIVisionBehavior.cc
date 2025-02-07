#include "ASCIIVisionBehavior.h"
#include "Events/FilterBankEvent.h"
#include "Vision/RawCameraGenerator.h"
#include "Wireless/Socket.h"

const char ASCIIVisionBehavior::charMap[ASCIIVisionBehavior::charMapSize+1] = 
" .,-+=ioxwfkHN8M#@";


void
ASCIIVisionBehavior::processEvent(const EventBase& e) {
	const FilterBankEvent& fbkevt=dynamic_cast<const FilterBankEvent&>(e);

	unsigned int layer = 2;

	char charimg[(fbkevt.getWidth(layer)+1)*fbkevt.getHeight(layer)+1];
	char * curchar=charimg;
	unsigned char * image = fbkevt.getImage(layer, RawCameraGenerator::CHAN_Y);
	for (unsigned int y = 0; y < fbkevt.getHeight(layer); y+=2) {
		//notice 'y+=2' above -- skip every other row, makes "image" look more square (letters are taller than wide)
		//normally, if you want to process every pixel, use y++ instead ;)
		unsigned char * row = image + (y * fbkevt.getStride(layer));
		for (unsigned int x = 0; x < fbkevt.getWidth(layer); x++) {
			unsigned char * pixel = row + (x * fbkevt.getIncrement(layer));
			*curchar++=charMap[(pixel[0]*charMapSize)/256];
		}
		*curchar++='\n';
	}
	*curchar='\0';

	sout->printf("\n\n%s",charimg);
}

/*! @file
 * @brief Implements ASCIIVisionBehavior, which streams low-resolution ASCII-art of the camera image to sout
 * @author ejt (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.4 $
 * $State: Exp $
 * $Date: 2005/06/01 05:47:45 $
 */
