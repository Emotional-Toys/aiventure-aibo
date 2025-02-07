#include "RawCamBehavior.h"
#include "Wireless/Wireless.h"
#include "Events/EventRouter.h"
#include "Vision/RawCameraGenerator.h"
#include "Vision/JPEGGenerator.h"
#include "Events/FilterBankEvent.h"
#include "Behaviors/Controller.h"
#include "Shared/ProjectInterface.h"

RawCamBehavior* RawCamBehavior::theOne=NULL;

RawCamBehavior::RawCamBehavior()
	: CameraStreamBehavior("RawCamBehavior",visRaw), visRaw(NULL), packet(NULL), cur(NULL), avail(0), max_buf(0), lastProcessedTime(0)
{
	ASSERT(theOne==NULL,"there was already a RawCamBehavior running!");
	theOne=this;
}

void
RawCamBehavior::DoStart() {
	BehaviorBase::DoStart();
	setupServer();
	erouter->addListener(this,EventBase::visRawCameraEGID,ProjectInterface::visRawCameraSID,EventBase::deactivateETID);
	erouter->addListener(this,EventBase::visJPEGEGID,ProjectInterface::visColorJPEGSID,EventBase::deactivateETID);
	erouter->addListener(this,EventBase::visJPEGEGID,ProjectInterface::visGrayscaleJPEGSID,EventBase::deactivateETID);
}

void
RawCamBehavior::DoStop() {
	erouter->removeListener(this);
	closeServer();
	BehaviorBase::DoStop();
}

void
RawCamBehavior::processEvent(const EventBase& e) {
	if(!wireless->isConnected(visRaw->sock))
		return;
	if(config->vision.rawcam.transport==0 && visRaw->getTransport()==Socket::SOCK_STREAM
		 || config->vision.rawcam.transport==1 && visRaw->getTransport()==Socket::SOCK_DGRAM) {
		//reset the socket
		closeServer();
		setupServer();
		return;
	}
	try {
		const FilterBankEvent* fbke=dynamic_cast<const FilterBankEvent*>(&e);
		if(fbke==NULL) {
			CameraStreamBehavior::processEvent(e);
			return;
		}
		if ((get_time() - lastProcessedTime) < config->vision.rawcam.interval) {// not enough time has gone by
			return;
		}
		/* // turning these off enables individual channel compression
			if(config->vision.rawcam.compression==Config::vision_config::COMPRESS_NONE && e.getGeneratorID()!=EventBase::visRawCameraEGID)
			return;
			if(config->vision.rawcam.compression==Config::vision_config::COMPRESS_JPEG && e.getGeneratorID()!=EventBase::visJPEGEGID)
			return; */
		if(config->vision.rawcam.encoding==Config::vision_config::RawCamConfig::ENCODE_COLOR) {
			if(!writeColor(*fbke)) {
				if(packet) { //packet was opened, need to close it
					cur=packet; // don't send anything
					closePacket();
				}
				//error message should already be printed in writeColor
				//ASSERTRET(false,"serialization failed");
			}
		} else if(config->vision.rawcam.encoding==Config::vision_config::RawCamConfig::ENCODE_SINGLE_CHANNEL) {
			if(!writeSingleChannel(*fbke)) {
				if(packet) { //packet was opened, need to close it
					cur=packet; // don't send anything
					closePacket();
				}
				//error message should already be printed in writeSingleChannel
				//ASSERTRET(false,"serialization failed");
			}
		} else {
			serr->printf("%s: Bad rawcam.encoding setting\n",getName().c_str());
		}
	} catch(...) {
		if(packet) { //packet was opened, need to close it
			cur=packet; // don't send anything
			closePacket();
		}
		// typically this is a per-frame recurring error, so let's just stop now
		serr->printf("%s: exception generated during image serialization, stopping stream.\n",getName().c_str());
		DoStop();
		throw;
	}
}

unsigned int RawCamBehavior::getSourceLayer(unsigned int chan, unsigned int numLayers) {
	if(config->vision.rawcam.encoding==Config::vision_config::RawCamConfig::ENCODE_SINGLE_CHANNEL) {
		if(config->vision.rawcam.channel!=(int)chan)
			return -1U;
		return numLayers-1-config->vision.rawcam.y_skip;
	}
	// must be full-color
	switch(chan) {
	case RawCameraGenerator::CHAN_Y:
		if(config->vision.rawcam.compression==Config::vision_config::RawCamConfig::COMPRESS_JPEG) {
			if(config->vision.rawcam.y_skip-config->vision.rawcam.uv_skip == 1)
				return numLayers-1-config->vision.rawcam.uv_skip;
		}
		return numLayers-1-config->vision.rawcam.y_skip;
	case RawCameraGenerator::CHAN_U:
	case RawCameraGenerator::CHAN_V:
		if(config->vision.rawcam.compression==Config::vision_config::RawCamConfig::COMPRESS_JPEG) {
			if(config->vision.rawcam.uv_skip-config->vision.rawcam.y_skip == 1)
				return numLayers-1-config->vision.rawcam.y_skip;
		}
		return numLayers-1-config->vision.rawcam.uv_skip;
	default: // other channels, i.e. Y-derivatives
		return -1U;
	}
}
unsigned int RawCamBehavior::getSourceYLayer(unsigned int numLayers) {
	return getSourceLayer(RawCameraGenerator::CHAN_Y,numLayers);
}
unsigned int RawCamBehavior::getSourceULayer(unsigned int numLayers) {
	return getSourceLayer(RawCameraGenerator::CHAN_U,numLayers);
}
unsigned int RawCamBehavior::getSourceVLayer(unsigned int numLayers) {
	return getSourceLayer(RawCameraGenerator::CHAN_V,numLayers);
}

void
RawCamBehavior::closeServer() {
	if(wireless->isConnected(visRaw->sock))
		sendCloseConnectionPacket();
	Controller::closeGUI("RawVisionGUI");
	
	// this could be considered a bug in our wireless - if we don't setDaemon(...,false)
	// it will try to listen again even though we explicitly closed the server socket...
	wireless->setDaemon(visRaw,false);
	wireless->close(visRaw->sock);
}

void
RawCamBehavior::setupServer() {
	std::vector<std::string> args;
	args.push_back("raw");
	char port[50];
	snprintf(port,50,"%d",*config->vision.rawcam.port);
	args.push_back(port);
	if(config->vision.rawcam.transport==0) {
		max_buf=UDP_WIRELESS_BUFFER_SIZE;
		visRaw=wireless->socket(Socket::SOCK_DGRAM, 1024, max_buf);
		args.push_back("udp");
	} else if(config->vision.rawcam.transport==1) {
		max_buf=TCP_WIRELESS_BUFFER_SIZE;
		visRaw=wireless->socket(Socket::SOCK_STREAM, 1024, max_buf);
		args.push_back("tcp");
	} else {
		serr->printf("ERROR: Invalid Config::vision.rawcam.transport: %d\n",*config->vision.rawcam.transport);
		return;
	}
	wireless->setDaemon(visRaw,true);
	wireless->setReceiver(visRaw,networkCallback);
	wireless->listen(visRaw,config->vision.rawcam.port);
	
	Controller::loadGUI("org.tekkotsu.mon.VisionGUI","RawVisionGUI",*config->vision.rawcam.port,args);
}

bool
RawCamBehavior::openPacket(FilterBankGenerator& fbkgen, unsigned int time, unsigned int layer) {
	if(packet!=NULL)
		return false;

	avail=max_buf-1; //not sure why -1, but Alok had it, so i will too
	ASSERT(cur==NULL,"cur non-NULL");
	cur=NULL;
	char * buf=packet=(char*)visRaw->getWriteBuffer(avail);
	ASSERT(packet!=NULL,"dropped frame, network bandwidth is saturated (reduce frame rate or size)");
	if(packet==NULL)
		return false;
	
	if(!LoadSave::encodeInc("TekkotsuImage",buf,avail,"ran out of space %s:%un",__FILE__,__LINE__)) return false;;
	if(!LoadSave::encodeInc(*config->vision.rawcam.encoding,buf,avail,"ran out of space %s:%un",__FILE__,__LINE__)) return false;;
	if(!LoadSave::encodeInc(*config->vision.rawcam.compression,buf,avail,"ran out of space %s:%un",__FILE__,__LINE__)) return false;;

	if(!LoadSave::encodeInc(fbkgen.getWidth(layer),buf,avail,"ran out of space %s:%un",__FILE__,__LINE__)) return false;;
	if(!LoadSave::encodeInc(fbkgen.getHeight(layer),buf,avail,"ran out of space %s:%un",__FILE__,__LINE__)) return false;;
	if(!LoadSave::encodeInc(time,buf,avail,"ran out of space %s:%un",__FILE__,__LINE__)) return false;;
	if(!LoadSave::encodeInc(fbkgen.getFrameNumber(),buf,avail,"ran out of space %s:%un",__FILE__,__LINE__)) return false;;

	cur=buf;
	return true;
}

bool
RawCamBehavior::writeColor(const FilterBankEvent& e) {
	FilterBankGenerator& fbkgen=*e.getSource();

	unsigned int y_layer=fbkgen.getNumLayers()-1-config->vision.rawcam.y_skip;
	unsigned int uv_layer=fbkgen.getNumLayers()-1-config->vision.rawcam.uv_skip;

	if(config->vision.rawcam.channel==-1) {
		if(e.getGeneratorID()==EventBase::visRawCameraEGID && config->vision.rawcam.compression==Config::vision_config::RawCamConfig::COMPRESS_NONE
			 || e.getGeneratorID()==EventBase::visJPEGEGID && config->vision.rawcam.compression==Config::vision_config::RawCamConfig::COMPRESS_JPEG) {
			if(const JPEGGenerator* jgen=dynamic_cast<const JPEGGenerator*>(&fbkgen))
				if(jgen->getCurrentSourceFormat()==JPEGGenerator::SRC_COLOR)
					return true;
			openPacket(fbkgen,e.getTimeStamp(),uv_layer);
			if(cur==NULL) //error should have been displayed by openPacket
				return false;
			
			if(!LoadSave::encodeInc("FbkImage",cur,avail,"ran out of space %s:%un",__FILE__,__LINE__)) return false;;
			if(!LoadSave::encodeInc(0,cur,avail,"ran out of space %s:%un",__FILE__,__LINE__)) return false;;
			if(!LoadSave::encodeInc(0,cur,avail,"ran out of space %s:%un",__FILE__,__LINE__)) return false;;
			if(!LoadSave::encodeInc(-1,cur,avail,"ran out of space %s:%un",__FILE__,__LINE__)) return false;;
			if(!LoadSave::encodeInc(RawCameraGenerator::CHAN_Y,cur,avail,"ran out of space %s:%un",__FILE__,__LINE__)) return false;;
			if(!LoadSave::encodeInc("blank",cur,avail,"ran out of space %s:%un",__FILE__,__LINE__)) return false;;

			fbkgen.selectSaveImage(uv_layer,RawCameraGenerator::CHAN_U);
			if(!LoadSave::checkInc(fbkgen.saveBuffer(cur,avail),cur,avail,"image size too large -- may need to set Config::vision.rawcam.transport to TCP and reopen raw cam")) return false;
			
			fbkgen.selectSaveImage(uv_layer,RawCameraGenerator::CHAN_V);
			if(!LoadSave::checkInc(fbkgen.saveBuffer(cur,avail),cur,avail,"image size too large -- may need to set Config::vision.rawcam.transport to TCP and reopen raw cam")) return false;

			closePacket();
		}
		return true;
	}

	unsigned int big_layer=y_layer;
	unsigned int small_layer=uv_layer;
	if(y_layer<uv_layer) { 
		big_layer=uv_layer;
		small_layer=y_layer;
	}
	if(const JPEGGenerator* jgen=dynamic_cast<const JPEGGenerator*>(&fbkgen)) {
		if(config->vision.rawcam.compression!=Config::vision_config::RawCamConfig::COMPRESS_JPEG)
			return true;
		if(jgen->getCurrentSourceFormat()==JPEGGenerator::SRC_COLOR && big_layer-small_layer<2) {
			openPacket(fbkgen,e.getTimeStamp(),big_layer);
			if(cur==NULL) //error should have been displayed by openPacket
				return false;
			
			fbkgen.selectSaveImage(big_layer,0);
			if(!LoadSave::checkInc(fbkgen.saveBuffer(cur,avail),cur,avail,"image size too large -- may need to set Config::vision.rawcam.transport to TCP and reopen raw cam")) return false;
			
			closePacket();
		} else if(jgen->getCurrentSourceFormat()==JPEGGenerator::SRC_GRAYSCALE && big_layer-small_layer>=2) {
			bool opened=openPacket(fbkgen,e.getTimeStamp(),big_layer);
			if(cur==NULL) //error should have been displayed by openPacket
				return false;
			
			if(big_layer==y_layer) {
				fbkgen.selectSaveImage(y_layer,RawCameraGenerator::CHAN_Y);
				if(!LoadSave::checkInc(fbkgen.saveBuffer(cur,avail),cur,avail,"image size too large -- may need to set Config::vision.rawcam.transport to TCP and reopen raw cam")) return false;
			} else {
				fbkgen.selectSaveImage(uv_layer,RawCameraGenerator::CHAN_U);
				if(!LoadSave::checkInc(fbkgen.saveBuffer(cur,avail),cur,avail,"image size too large -- may need to set Config::vision.rawcam.transport to TCP and reopen raw cam")) return false;
				fbkgen.selectSaveImage(uv_layer,RawCameraGenerator::CHAN_V);
				if(!LoadSave::checkInc(fbkgen.saveBuffer(cur,avail),cur,avail,"image size too large -- may need to set Config::vision.rawcam.transport to TCP and reopen raw cam")) return false;
			}
			
			if(!opened)
				closePacket();
		}
	} else {
		bool opened=false;
		
		if(config->vision.rawcam.compression==Config::vision_config::RawCamConfig::COMPRESS_NONE || big_layer-small_layer>=2 && big_layer==uv_layer) {
			opened=openPacket(fbkgen,e.getTimeStamp(),big_layer);
			if(cur==NULL) //error should have been displayed by openPacket
				return false;
			fbkgen.selectSaveImage(y_layer,RawCameraGenerator::CHAN_Y);
			if(!LoadSave::checkInc(fbkgen.saveBuffer(cur,avail),cur,avail,"image size too large -- may need to set Config::vision.rawcam.transport to TCP and reopen raw cam")) return false;
		}
		
		if(config->vision.rawcam.compression==Config::vision_config::RawCamConfig::COMPRESS_NONE || big_layer-small_layer>=2 && big_layer==y_layer) {
			opened=openPacket(fbkgen,e.getTimeStamp(),big_layer);
			if(cur==NULL) //error should have been displayed by openPacket
				return false;
			fbkgen.selectSaveImage(uv_layer,RawCameraGenerator::CHAN_U);
			if(!LoadSave::checkInc(fbkgen.saveBuffer(cur,avail),cur,avail,"image size too large -- may need to set Config::vision.rawcam.transport to TCP and reopen raw cam")) return false;
			fbkgen.selectSaveImage(uv_layer,RawCameraGenerator::CHAN_V);
			if(!LoadSave::checkInc(fbkgen.saveBuffer(cur,avail),cur,avail,"image size too large -- may need to set Config::vision.rawcam.transport to TCP and reopen raw cam")) return false;
		}
		
		if(config->vision.rawcam.compression==Config::vision_config::RawCamConfig::COMPRESS_NONE || !opened)
			closePacket();
	}
	
	return true;
}

bool
RawCamBehavior::writeSingleChannel(const FilterBankEvent& e) {
	FilterBankGenerator& fbkgen=*e.getSource();
	if( config->vision.rawcam.compression==Config::vision_config::RawCamConfig::COMPRESS_NONE && e.getGeneratorID()==EventBase::visRawCameraEGID
			|| config->vision.rawcam.compression==Config::vision_config::RawCamConfig::COMPRESS_JPEG && e.getGeneratorID()==EventBase::visJPEGEGID )
		{
			if(const JPEGGenerator * jgen=dynamic_cast<const JPEGGenerator*>(&fbkgen))
				if(jgen->getCurrentSourceFormat()!=JPEGGenerator::SRC_GRAYSCALE)
					return true;
			unsigned int layer=fbkgen.getNumLayers()-1-config->vision.rawcam.y_skip;
			
			openPacket(fbkgen,e.getTimeStamp(),layer);
			if(cur==NULL) //error should have been displayed by openPacket
				return false;
			
			fbkgen.selectSaveImage(layer,config->vision.rawcam.channel);
			if(!LoadSave::checkInc(fbkgen.saveBuffer(cur,avail),cur,avail,"image size too large -- may need to set Config::vision.rawcam.transport to TCP and reopen raw cam")) return false;
			
			closePacket();
		}
	return true;
}

//#include "Shared/WorldState.h"

void
RawCamBehavior::closePacket() {
	if(packet==NULL)
		return;
	visRaw->write(cur-packet);
	/*	cout << "used=" << (cur-packet) << "\tavail==" << avail;
	if(RobotName == ERS7Info::TargetName)
		cout << "\tmax bandwidth=" << (cur-packet)/1024.f*30 << "KB/sec" << endl;
	else
		cout << "\tmax bandwidth=" << (cur-packet)/1024.f*24 << "KB/sec" << endl;
	*/
	packet=cur=NULL;
	avail=0;
	lastProcessedTime = get_time();
}

bool
RawCamBehavior::sendCloseConnectionPacket() {
	char msg[]="CloseConnection";
	unsigned int len=strlen(msg)+LoadSave::stringpad;
	char * buf = (char*)visRaw->getWriteBuffer(len);
	if(buf==NULL) {
		std::cerr << "Could not get buffer for closing packet" << std::endl;
		return false;
	}
	unsigned int used=LoadSave::encode(msg,buf,len);
	if(used==0)
		std::cerr << "Could not write close packet" << std::endl;
	visRaw->write(used);
	return true;
}


/*! @file
 * @brief Implements RawCamBehavior, which forwards images from camera over wireless
 * @author ejt (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.37 $
 * $State: Exp $
 * $Date: 2007/06/26 04:27:44 $
 */

