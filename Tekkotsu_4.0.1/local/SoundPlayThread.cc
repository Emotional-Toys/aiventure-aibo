#include "SoundPlayThread.h"
#include "Shared/Config.h"
#include "Sound/SoundManager.h"

//better to put this here instead of the header
using namespace std; 

void SoundPlayThread::reset() {
	if(sndman->getNumPlaying()<=0 && isStarted())
		stop();
	else if(sndman->getNumPlaying()>0 && !isStarted())
		start();
}

bool SoundPlayThread::launched() {
	buffersInFlight=0;
	//cout << "SoundPlayThread launched " << buffersInFlight << ' ' << sndman->getNumPlaying() << endl;
	return sndman->getNumPlaying()>0;
}

bool SoundPlayThread::poll() {
	if(buffersInFlight>0)
		buffersInFlight--; // poll has timed out, remove a buffer from in-flight
	size_t tgtsize = BUFFER_TIME*(config->sound.sample_bits/8)*config->sound.sample_rate/1000;
	if(bufsize!=tgtsize) {
		delete buf;
		buf = new char[bufsize=tgtsize];
	}
	while(buffersInFlight<NUM_BUFFERS) {
		sndman->CopyTo(buf,bufsize);
		// interface with host hardware to play the sound...?
		buffersInFlight++;
	}
	//cout << "SoundPlayThread polled " << buffersInFlight << ' ' << sndman->getNumPlaying() << endl;
	return sndman->getNumPlaying()>0;
}

/*! @file
 * @brief 
 * @author Ethan Tira-Thompson (ejt) (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Date: 2007/10/12 16:55:04 $
 */
