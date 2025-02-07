//-*-c++-*-
#ifndef INCLUDED_ViewerConnection_h_
#define INCLUDED_ViewerConnection_h_

#include <string>

#include "Shared/ProjectInterface.h"
#include "Wireless/Wireless.h"

namespace DualCoding {

static const unsigned int DIALOG_BUFFER_SIZE=20000;

#if defined(TGT_ERS7) || defined(TGT_ERS2xx) || defined(TGT_ERS210) || defined(TGT_ERS220)
static const unsigned int RLE_BUFFER_SIZE=70000; // Raised to 120000 for sketch<usint> in world space; then lowered to 70000 due to memory shortage
#else
static const unsigned int RLE_BUFFER_SIZE=500000;
#endif

static const unsigned int CAM_LAYER=ProjectInterface::fullLayer; //!< the full resolution layer in the filter bank
static const unsigned int CAM_CHANNEL=0;  //!< corresponds to appropriate thresholding listed in tekkotsu.xml
static const unsigned int WORLD_WIDTH=225;
static const unsigned int WORLD_HEIGHT=225;

static const unsigned short CAM_DIALOG_PORT   = 5800;
static const unsigned short CAM_RLE_PORT      = 5801;
static const unsigned short LOCAL_DIALOG_PORT = 5802;
static const unsigned short LOCAL_RLE_PORT    = 5803;
static const unsigned short WORLD_DIALOG_PORT = 5804;
static const unsigned short WORLD_RLE_PORT    = 5805;

class ViewerConnection {

 private:
  Socket *dialogSock, *rleSock;
  int dialogPort, rlePort;

 public:
  ViewerConnection(void) : dialogSock(NULL), rleSock(NULL), dialogPort(0), rlePort(0) {}

  Socket *getDialogSocket() const { return dialogSock; }
  Socket *getRleSocket() const { return rleSock; }

  void setDialogSocket(Socket* sock, int const port) { dialogSock = sock; dialogPort = port; }
  void setRleSocket(Socket* sock, int const port) { rleSock = sock; rlePort = port; }

  void writeBigString(std::string const &msg);

 private:
  ViewerConnection(const ViewerConnection&); //!< never call this
  ViewerConnection& operator=(const ViewerConnection&); //!< never call this

};

} // namespace

#endif
