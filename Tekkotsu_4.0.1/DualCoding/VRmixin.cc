#include <sstream>

#include "Vision/RawCameraGenerator.h"
#include "Vision/RLEGenerator.h"
#include "Vision/RegionGenerator.h"
#include "Vision/SegmentedColorGenerator.h"

#include "SketchData.h"
#include "ShapeBlob.h"
#include "Sketch.h"

#include "Lookout.h"
#include "MapBuilder.h"
#include "Pilot.h"

#include "ViewerConnection.h"  // for port numbers and buffer sizes
#include "VRmixin.h"

using namespace std;

namespace DualCoding {

//----------------------------------------------------------------

VRmixin* VRmixin::theOne=NULL;

//! static function allows us to specify intialization order because static within a function isn't created until the function is called
template<ReferenceFrameType_t _refFrameType,int const init_id, size_t const _width, size_t const _height>
static SketchSpace& createStaticSkS(const std::string& _name) {
  static SketchSpace SkS(_name,_refFrameType,init_id,_width,_height);
  //  cout << _name << " space is constructed\n";
  return SkS;
}

SketchSpace& VRmixin::getCamSkS() {
  return createStaticSkS<camcentric,10000,CameraResolutionX,CameraResolutionY>("cam");
}
SketchSpace& VRmixin::getLocalSkS() {
  return createStaticSkS<egocentric,20000,WORLD_WIDTH,WORLD_HEIGHT>("local"); 
}
SketchSpace& VRmixin::getWorldSkS() {
  return createStaticSkS<allocentric,30000,WORLD_WIDTH,WORLD_HEIGHT>("world");
}
ShapeSpace& VRmixin::getGroundShS() {
  static ShapeSpace ShS(&VRmixin::getCamSkS(),90000,"ground",egocentric);
  return ShS;
}

SketchSpace& VRmixin::camSkS=VRmixin::getCamSkS();
ShapeSpace& VRmixin::camShS=VRmixin::getCamSkS().getDualSpace();

ShapeSpace& VRmixin::groundShS=VRmixin::getGroundShS();

SketchSpace& VRmixin::localSkS=VRmixin::getLocalSkS();
ShapeSpace& VRmixin::localShS=VRmixin::getLocalSkS().getDualSpace();

SketchSpace& VRmixin::worldSkS=VRmixin::getWorldSkS();
ShapeSpace& VRmixin::worldShS=VRmixin::getWorldSkS().getDualSpace();

Shape<AgentData> VRmixin::theAgent;

Socket *VRmixin::camDialogSock=NULL;
Socket *VRmixin::camRleSock=NULL;
Socket *VRmixin::localDialogSock=NULL;
Socket *VRmixin::localRleSock=NULL;
Socket *VRmixin::worldDialogSock=NULL;
Socket *VRmixin::worldRleSock=NULL;

MapBuilder& VRmixin::getMapBuilder() {
  static MapBuilder mapbuilderInstance;
  return mapbuilderInstance;
}
MapBuilder& VRmixin::mapBuilder = VRmixin::getMapBuilder();

Lookout& VRmixin::getLookout() {
  static Lookout lookoutInstance;
  return lookoutInstance;
}
Lookout& VRmixin::lookout = VRmixin::getLookout();

Pilot& VRmixin::getPilot() {
  static Pilot pilotInstance;
  return pilotInstance;
}
Pilot& VRmixin::pilot = VRmixin::getPilot();

PFShapeLocalization* VRmixin::particleFilter = NULL; // will be set by startCrew

unsigned int VRmixin::instanceCount = 0;
unsigned int VRmixin::crewCount = 0;

VRmixin::VRmixin() {
  if(instanceCount++==0) {
    // only want to do the following once
    //cout << "Initializing VRmixin statics" << endl;
    if (theOne != NULL) {
      if ( ! mapBuilder.isRetained() )
	cerr << "VRmixin statics already constructed!?!?!" << endl;
      return;
    }
    theOne=this;
    camSkS.requireIdx();
    localSkS.requireIdx();
    worldSkS.requireIdx();
		
    camDialogSock=wireless->socket(Socket::SOCK_STREAM, 1024, DIALOG_BUFFER_SIZE);
    camRleSock=wireless->socket(Socket::SOCK_STREAM, 1024, RLE_BUFFER_SIZE);
    worldDialogSock=wireless->socket(Socket::SOCK_STREAM, 1024, DIALOG_BUFFER_SIZE);
    worldRleSock=wireless->socket(Socket::SOCK_STREAM, 1024, RLE_BUFFER_SIZE);
    localDialogSock=wireless->socket(Socket::SOCK_STREAM, 1024, DIALOG_BUFFER_SIZE);
    localRleSock=wireless->socket(Socket::SOCK_STREAM, 1024, RLE_BUFFER_SIZE);
		
    wireless->setReceiver(camDialogSock->sock, &camDialogSockCallback);
    wireless->setReceiver(worldDialogSock->sock, &worldDialogSockCallback);
    wireless->setReceiver(localDialogSock->sock, &localDialogSockCallback);
		
    wireless->setDaemon(camDialogSock,   true);
    wireless->setDaemon(camRleSock,      true);
    wireless->setDaemon(worldDialogSock, true);
    wireless->setDaemon(worldRleSock,    true);
    wireless->setDaemon(localDialogSock, true);
    wireless->setDaemon(localRleSock,    true);
		
    wireless->listen(camDialogSock,   CAM_DIALOG_PORT);
    wireless->listen(camRleSock,      CAM_RLE_PORT);
    wireless->listen(worldDialogSock, WORLD_DIALOG_PORT);
    wireless->listen(worldRleSock,    WORLD_RLE_PORT);
    wireless->listen(localDialogSock, LOCAL_DIALOG_PORT);
    wireless->listen(localRleSock,    LOCAL_RLE_PORT);
		
    camSkS.viewer->setDialogSocket(camDialogSock,     CAM_DIALOG_PORT);
    camSkS.viewer->setRleSocket(camRleSock,           CAM_RLE_PORT);
    localSkS.viewer->setDialogSocket(localDialogSock, LOCAL_DIALOG_PORT);
    localSkS.viewer->setRleSocket(localRleSock,       LOCAL_RLE_PORT);
    worldSkS.viewer->setDialogSocket(worldDialogSock, WORLD_DIALOG_PORT);
    worldSkS.viewer->setRleSocket(worldRleSock,       WORLD_RLE_PORT);

    theAgent = Shape<AgentData>(worldShS);
    theAgent->setColor(rgb(0,0,255));

    mapBuilder.SetAutoDelete(false);
    pilot.SetAutoDelete(false);
    lookout.SetAutoDelete(false);
  }
}

VRmixin::~VRmixin() {
  if(--instanceCount==0) {
    if ( mapBuilder.isRetained() ) return;
    // cout << "Destructing VRmixin statics" << endl;
    if (theOne == NULL) {
      cerr << "VRmixin statics already destructed!?!?!" << endl;
      return;
    }
    
    wireless->setDaemon(camDialogSock,  false);
    wireless->setDaemon(camRleSock,     false);
    wireless->setDaemon(localDialogSock,false);
    wireless->setDaemon(localRleSock,   false);
    wireless->setDaemon(worldDialogSock,false);
    wireless->setDaemon(worldRleSock,   false);
    
    wireless->close(camRleSock->sock);
    wireless->close(camDialogSock->sock);
    wireless->close(localRleSock->sock);
    wireless->close(localDialogSock->sock);
    wireless->close(worldRleSock->sock);
    wireless->close(worldDialogSock->sock);
		
    theOne=NULL;
    
    // clear ShapeSpace first because it may contain rendering links to SketchSpace
    camShS.clear();
    camSkS.bumpRefreshCounter(); // release visible sketches
    camSkS.clear();
    
    localShS.clear();
    localSkS.bumpRefreshCounter(); // release visible sketches
    localSkS.clear();
    
    theAgent = Shape<AgentData>();
    worldShS.clear();
    worldSkS.bumpRefreshCounter(); // release visible sketches
    worldSkS.clear();
		
    camSkS.freeIndexes();
    localSkS.freeIndexes();
    worldSkS.freeIndexes();
  }
}

void VRmixin::startCrew() {
  if ( crewCount++ == 0 ) {
    //cout << "Starting crew" << endl;
    mapBuilder.SetAutoDelete(false);
    mapBuilder.DoStart();
    lookout.SetAutoDelete(false);
    lookout.DoStart();
    pilot.SetAutoDelete(false);
    pilot.DoStart();
    if ( particleFilter == NULL )
      particleFilter = new PFShapeLocalization(localShS,worldShS);
  }
}

void VRmixin::stopCrew() {
  if ( --crewCount == 0 ) {
    //cout << "Stopping crew" << endl;
    pilot.DoStop();
    lookout.DoStop();
    mapBuilder.DoStop();
    delete particleFilter;
    particleFilter = NULL;
  }
}

void VRmixin::projectToGround(const NEWMAT::Matrix& camToBase,
			      const NEWMAT::ColumnVector& ground_plane) {
  groundShS.clear();
  groundShS.importShapes(camShS.allShapes());
  const vector<ShapeRoot> &groundShapes_vec = groundShS.allShapes();
  for(size_t i = 0; i < groundShapes_vec.size(); i++)
    groundShapes_vec[i]->projectToGround(camToBase, ground_plane);
}

int VRmixin::camDialogSockCallback(char *buf, int bytes) {
  static std::string incomplete;
  dialogCallback(buf, bytes, incomplete, theOne->camSkS, theOne->camShS);
  return 0;
}

int VRmixin::localDialogSockCallback(char *buf, int bytes) {
  static std::string incomplete;
  dialogCallback(buf, bytes, incomplete, theOne->localSkS, theOne->localShS);
  return 0;
}

int VRmixin::worldDialogSockCallback(char *buf, int bytes) {
  static std::string incomplete;
  dialogCallback(buf, bytes, incomplete, theOne->worldSkS, theOne->worldShS);
  return 0;
}

void VRmixin::dialogCallback(char* buf, int bytes, std::string& incomplete,
			     SketchSpace& SkS, ShapeSpace& ShS) {
  std::string s(buf,bytes);
  while(s.size()>0) {
    size_t endline=s.find('\n');
    if(endline==std::string::npos) {
      incomplete+=s;
      return;
    }
    else {
      incomplete+=s.substr(0,endline);
      theOne->processSketchRequest(incomplete,SkS,ShS);
      incomplete.erase();
      s=s.substr(endline+1);
    }
  }
  return;
}

bool VRmixin::rleEncodeSketch(const SketchDataRoot& image)
{
  unsigned int avail = RLE_BUFFER_SIZE-1;
  Socket* rleSock = image.getSpace().viewer->getRleSocket();
  char* buf=(char*)rleSock->getWriteBuffer(avail);
  ASSERTRETVAL(buf!=NULL,"could not get buffer",false);
  unsigned int used = image.saveBuffer(buf, avail);
  rleSock->write(used);
  return true;
}

//! Import a color-segmented image as a Sketch<uchar>
Sketch<uchar> VRmixin::sketchFromSeg() {
  Sketch<uchar> cam(camSkS, "camimage");
  cam->setColorMap(segMap);
  size_t const npixels = cam->getNumPixels();
  cmap_t* seg_image = ProjectInterface::defSegmentedColorGenerator->getImage(CAM_LAYER,CAM_CHANNEL);
  for(size_t i = 0; i < npixels; i++)
    cam[i] = seg_image[i];
  return cam;
}

//! Import channel n as a Sketch<uchar>
Sketch<uchar> VRmixin::sketchFromChannel(const RawCameraGenerator::channel_id_t chan) {
  const RawCameraGenerator::channel_id_t the_chan =
    (chan >= 0 && chan < RawCameraGenerator::NUM_CHANNELS) ? chan : RawCameraGenerator::CHAN_Y;
  Sketch<uchar> cam(camSkS,"sketchFromChannel");
  cam->setColorMap(grayMap);
  uchar* campixels = cam->getRawPixels();
  int const incr = ProjectInterface::defRawCameraGenerator->getIncrement(CAM_LAYER);
  int const skip = ProjectInterface::defRawCameraGenerator->getSkip(CAM_LAYER);
  uchar* chan_ptr = ProjectInterface::defRawCameraGenerator->getImage(CAM_LAYER,the_chan);
  if(chan_ptr==NULL) {
    for (unsigned int row = 0; row < cam->getHeight(); row++)
      for (unsigned int col = 0; col < cam->getWidth(); col++)
        *campixels++ = 0;
  } else {
    chan_ptr -= incr;  // back up by one pixel to prepare for loop
    for (unsigned int row = 0; row < cam->getHeight(); row++) {
      for (unsigned int col = 0; col < cam->getWidth(); col++)
        *campixels++ = *(chan_ptr += incr);
      chan_ptr += skip;
    }
  }
  return cam;
}

Sketch<uchar> VRmixin::sketchFromRawY() {
  return sketchFromChannel(RawCameraGenerator::CHAN_Y);
}


//! Import the results of the region generator as a vector of Shape<BlobData>
vector<Shape<BlobData> >
VRmixin::getBlobsFromRegionGenerator(const color_index color, 
				     const int minarea,
				     const BlobData::BlobOrientation_t orient, 
				     const coordinate_t height,
				     const int maxblobs) {
  vector<Shape<BlobData> > result;
  const CMVision::run<uchar> *rle_buffer = reinterpret_cast<const CMVision::run<uchar>*>
    (ProjectInterface::defRLEGenerator->getImage(CAM_LAYER,CAM_CHANNEL));
  const CMVision::color_class_state* ccs = reinterpret_cast<const CMVision::color_class_state*>
    (ProjectInterface::defRegionGenerator->getImage(CAM_LAYER,CAM_CHANNEL));
  //  cout << "Color " << color << " name '" << ccs[color].name 
  //   << "' has " << ccs[color].num << " regions." << endl;
  const rgb rgbvalue = ProjectInterface::getColorRGB(color);
  const CMVision::region* list_head = ccs[color].list;
  for (int i=0; list_head!=NULL && i<maxblobs && list_head->area >= minarea; list_head = list_head->next, i++) {
    BlobData* blobdat = BlobData::new_blob(camShS,*list_head, rle_buffer, orient, height, rgbvalue);
    result.push_back(Shape<BlobData>(blobdat));
  }
  return result;
}

void VRmixin::processSketchRequest(const std::string &line,
				   SketchSpace& SkS, 
				   ShapeSpace& ShS)
{
  Socket* dialogSock = SkS.viewer->getDialogSocket();
  if(line.compare(0,strlen("size"),"size")==0) {
    dialogSock->printf("size begin\n");
    dialogSock->printf("width %d\nheight %d\n",int(SkS.getWidth()),int(SkS.getHeight()));
    dialogSock->printf("size end\n");
  }
  else if(line.compare(0,strlen("list"),"list")==0) {
    dialogSock->printf("list begin\n");
    SkS.viewer->writeBigString(SkS.getTmatForGUI());	
    SkS.viewer->writeBigString(SkS.getSketchListForGUI());	
    SkS.viewer->writeBigString(ShS.getShapeListForGUI());	
    dialogSock->printf("list end\n");
  } else if(line.compare(0,strlen("get"),"get")==0) {
    dialogSock->printf("get begin\n");
    std::string tempstring = line.substr(strlen("get"),
					 line.length()-strlen("get"));
    std::istringstream ist(tempstring);
    int requested_id = -1;
    ist >> requested_id;
    dialogSock->printf("get read:%d\n",requested_id);
    SketchDataRoot* sketchptr=(SkS.retrieveSketch(requested_id));
    if(sketchptr != NULL)
      rleEncodeSketch(*sketchptr);
    dialogSock->printf("get end\n");
  } else {
    dialogSock->printf("Invalid command\n");
  }
}

} // namespace
