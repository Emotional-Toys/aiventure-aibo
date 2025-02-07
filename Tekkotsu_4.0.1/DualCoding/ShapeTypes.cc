//-*-c++-*-

#include "ShapeTypes.h"

namespace DualCoding {

const char* data_name(int data_type)
{
  switch(data_type) {
  case lineDataType:
    return("LineData");
    break;
  case ellipseDataType:
    return("EllipseData");
    break;
  case pointDataType:
    return("PointData");
    break;
  case agentDataType:
    return("AgentData");
    break;
  case sphereDataType:
    return("SphereData");
    break;
  case polygonDataType:
    return("PolygonData");
    break;
  case blobDataType:
    return("BlobData");
    break;
  case brickDataType:
    return("BrickData");
    break;
  case pyramidDataType:
    return("PyramidData");
    break;
  case localizationParticleDataType:
    return("LocalizationParticleData");
    break;
  case targetDataType:
    return("TargetData");
    break;
  case unknownDataType:
  default:
    return("*Unknown*");
    break;
  }
}

} // namespace
