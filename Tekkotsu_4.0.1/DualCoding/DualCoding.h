//-*-c++-*-
// Master include file list to be used by user behaviors.

#ifndef LOADED_DualCoding_h_
#define LOADED_DualCoding_h_

//! Dual coding vision representations (Sketches and Shapes)
/*! For tutorial, see http://www.cs.cmu.edu/~dst/Tekkotsu/Tutorial/vr-intro.shtml */
namespace DualCoding {}

#include "ShapeTypes.h"
#include "SketchTypes.h"

#include "SketchSpace.h"
#include "ShapeSpace.h"
#include "ShapeFuns.h"

#include "Sketch.h"
#include "SketchIndices.h"
#include "Region.h"
#include "visops.h"

#include "ShapeRoot.h"
#include "ShapeLine.h"
#include "ShapeEllipse.h"
#include "ShapePoint.h"
#include "ShapeAgent.h"
#include "ShapeSphere.h"
#include "ShapeBlob.h"
#include "ShapePolygon.h"
#include "ShapeBrick.h"
#include "ShapePyramid.h"
#include "ShapeLocalizationParticle.h"

#include "VisualRoutinesStateNode.h"
#include "VisualRoutinesBehavior.h" // this must precede mapbuilders, pilot and lookout

#include "Lookout.h"
#include "MapBuilder.h"
#include "Pilot.h"

#include "PFShapeSLAM.h"

#endif
