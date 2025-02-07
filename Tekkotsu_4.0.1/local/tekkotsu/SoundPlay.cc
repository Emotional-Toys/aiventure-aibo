#include "SoundPlay.h"
#include "Simulator.h"
#include "SharedGlobals.h"
#include "Main.h"
#include "IPC/MessageReceiver.h"
#include "IPC/RegionRegistry.h"
#include "Events/EventRouter.h"
#include "SimConfig.h"

SoundPlayThread * SoundPlay::player;

SoundPlay::SoundPlay()
	: Process(getID(),getClassName()),
		requests(ipc_setup->registerRegion(getSoundPlayID(),sizeof(sim::SoundPlayQueue_t))),
		events(ipc_setup->registerRegion(Main::getEventsID(),sizeof(sim::EventQueue_t))),
		statusRequest(ipc_setup->registerRegion(Simulator::getStatusRequestID(),sizeof(sim::StatusRequest_t))),
		soundmanager(ipc_setup->registerRegion(getSoundManagerID(),sizeof(SoundManager))),
		soundProf(ipc_setup->registerRegion(getSoundProfilerID(),sizeof(soundProfiler_t))),
		etrans(NULL), sndrecv(NULL), statusrecv(NULL)
{
	new (&(*requests)) sim::SoundPlayQueue_t;
	new (&(*soundmanager)) SoundManager;
	new (&(*soundProfiler)) soundProfiler_t;
	sndman=&(*soundmanager);
	::soundProfiler=&(*soundProf);
}

SoundPlay::~SoundPlay() {
	delete etrans;
	etrans=NULL;
	MotionManager::setTranslator(NULL);
}

void SoundPlay::DoStart() {
	Process::DoStart();
	//These are constructed by other processes, so need to wait
	//until the construction runlevel is complete before we access them
	if(!sim::config.multiprocess) {
		// don't use our own etrans here, because erouter will delete it for us, don't want a double-delete in our destructor...
		EventTranslator * forwardTrans = new IPCEventTranslator(*events);
		forwardTrans->setTrapEventValue(true);
		erouter->setForwardingAgent(getID(),forwardTrans);
	} else {
		etrans=new IPCEventTranslator(*events);
		MotionManager::setTranslator(etrans); //although SoundPlay shouldn't use any motions...

		// Set up Event Translator to trap and send events to main process
		//send everything over except erouter events
		for(unsigned int i=0; i<EventBase::numEGIDs; i++)
			if(i!=EventBase::erouterEGID)
				erouter->addTrapper(etrans,static_cast<EventBase::EventGeneratorID_t>(i));
	}
	
	player = new SoundPlayThread;
	player->start();
	sndrecv=new MessageReceiver(*requests,gotSnd);
	statusrecv=new MessageReceiver(*statusRequest,statusReport);
}

void SoundPlay::DoStop() {
	sndrecv->finish();
	statusrecv->finish();
	
	delete sndrecv;
	sndrecv=NULL;
	delete statusrecv;
	statusrecv=NULL;
	delete player;
	player=NULL;
	
	sndman->stopPlay();
	if(sim::config.multiprocess) {
		erouter->removeTrapper(etrans);
		delete etrans;
		etrans=NULL;
	}
	Process::DoStop();
}


/*! @file
 * @brief 
 * @author ejt (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Date: 2007/11/10 22:58:13 $
 */

