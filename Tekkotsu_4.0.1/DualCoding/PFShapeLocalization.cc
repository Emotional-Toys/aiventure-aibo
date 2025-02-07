//-*-c++-*-

#include <cmath>
#include <iostream>

#include "MapBuilder.h"
#include "PFShapeLocalization.h"
#include "ShapeLocalizationParticle.h"
#include "ShapeSpace.h"
#include "ShapeFuns.h"
#include "VRmixin.h"  // for mapBuilder

using namespace std;

namespace DualCoding {

  ParticleShapeEvaluator::ParticleShapeEvaluator(ShapeSpace &localShS, ShapeSpace &worldShS) : 
    localLms(), worldLms(),
    maxDist(0),
    localMatches(), numMatches(0), localScores(),
    particleViewX(), particleViewY(), particleViewX2(), particleViewY2()
  {
    PfRoot::loadLms(localShS.allShapes(), false, localLms);
    PfRoot::loadLms(worldShS.allShapes(), true, worldLms);
    if ( localLms.size()==0 || worldLms.size()==0 ) {
      cout << "ParticleFilter::loadLms found " << localLms.size() << " local and "
	   << worldLms.size() << " world landmarks: can't localize!" << endl;
      return;
    }
    unsigned int nlocal = localLms.size();
    localScores.resize(nlocal);
    localMatches.resize(nlocal);
    particleViewX.resize(nlocal);
    particleViewY.resize(nlocal);
    particleViewX2.resize(nlocal);
    particleViewY2.resize(nlocal);
  }

  void ParticleShapeEvaluator::evaluate(LocalizationParticle& part) {
    // determine position of local space landmark in world given the current particle
    float const cosT = cos(-part.theta);
    float const sinT = sin(-part.theta);
    float const negSinT = -sinT;
    // constructor has already verified that localLms > 0 and worldLms > 0
    for ( unsigned int j=0; j < localLms.size(); j++ ) {
      PfRoot &landmark = *(localLms[j]);
      particleViewX[j] = landmark.x * cosT + landmark.y * sinT + part.x; 
      particleViewY[j] = landmark.x * negSinT + landmark.y * cosT + part.y;
      if ( landmark.type == lineDataType ) {
	const PfLine &line = static_cast<PfLine&>(landmark);
	particleViewX2[j] = line.x2 * cosT + line.y2 * sinT + part.x;
	particleViewY2[j] = line.x2 * negSinT + line.y2 * cosT + part.y;
      }
    }
	
    // now compute score for the particle by finding matches between local landmarks and world landmarks
    numMatches=0;
    for ( unsigned int j = 0; j<localLms.size(); j++ ) {
      float distsq = HUGE_VAL; // HUGE_VAL is defined in math.h
      localMatches[j] = -1;
      for ( unsigned int k=0; k<worldLms.size(); k++ ) {
	if ( localLms[j]->type == worldLms[k]->type &&
	     localLms[j]->color == worldLms[k]->color ) {
	  float const lx = particleViewX[j];
	  float const ly = particleViewY[j];
	  float const wx = worldLms[k]->x;
	  float const wy = worldLms[k]->y;
	  float tempDistsq;
	  switch ( localLms[j]->type ) {
	  case lineDataType: {
	    PfLine &localLine = *static_cast<PfLine*>(localLms[j]);
	    PfLine &worldLine = *static_cast<PfLine*>(worldLms[k]);
	    float tempDistsq1, tempDistsq2;
	    // If endpoints are valid, compare distance between endpoints.
	    // If not valid, measure perpendicular distance from the local endpoint
	    // to the world line segment, if the projection of the endpoint onto the
	    // segment occurs within the segment, not beyond it.  Instead of calculating
	    // the projection we use a heuristic test: either the x or y endpoint value must
	    // lie within the range of the line segment.
	    if ( (localLine.valid1 && worldLine.valid1) ||
		 !( lx >= min(worldLine.x,worldLine.x2) &&
		    lx <= max(worldLine.x,worldLine.x2) ||
		    ly >= min(worldLine.y,worldLine.y2) &&
		    ly <= max(worldLine.y,worldLine.y2) ) )
	      tempDistsq1 = (lx-wx)*(lx-wx) + (ly-wy)*(ly-wy);
	    else {
	      float const tempDist1 = distanceFromLine(lx,ly,worldLine);
	      tempDistsq1 = tempDist1 * tempDist1;
	    }
	    float const lx2 = particleViewX2[j];
	    float const ly2 = particleViewY2[j];
	    float const wx2 = worldLine.x2;
	    float const wy2 = worldLine.y2;
	    if ( (localLine.valid2 && worldLine.valid2) ||
		 !( lx2 >= min(worldLine.x,worldLine.x2) &&
		    lx2 <= max(worldLine.x,worldLine.x2) ||
		    ly2 >= min(worldLine.y,worldLine.y2) &&
		    ly2 <= max(worldLine.y,worldLine.y2) ) )
	      tempDistsq2 = (lx2-wx2)*(lx2-wx2) + (ly2-wy2)*(ly2-wy2);
	    else {
	      float const tempDist2 = distanceFromLine(lx2,ly2,worldLine);
	      tempDistsq2 = tempDist2 * tempDist2;
	    }
	    AngPi const localOrient = localLine.orientation + part.theta;
	    AngPi const odiff = worldLine.orientation - localOrient;
	    float const odist = 500 * sin(odiff);
	    float const odistsq = odist * odist;
	    tempDistsq = tempDistsq1 + tempDistsq2 + odistsq; // plus orientation match term?
	  }
	    break;
	  case ellipseDataType:
	  case pointDataType:
	  case blobDataType: {
	    tempDistsq = (lx-wx)*(lx-wx) + (ly-wy)*(ly-wy);
	    break;
	  }
	  default:
	    std::cout << "ParticleFilter::computeMatchScore() can't match landmark type "
		      << localLms[j]->type << std::endl;
	    return;
	  }
	  if ( tempDistsq < distsq ) {
	    distsq = tempDistsq;
	    localMatches[j] = k;
	  }
	}
      }
		
      if ( localMatches[j] != -1 || !localLms[j]->mobile )
	localScores[numMatches++]=distsq;
    }
  }

  float ParticleShapeEvaluator::distanceFromLine(coordinate_t x0, coordinate_t y0, PfLine &wline) {
    float const &x1 = wline.x;
    float const &y1 = wline.y;
    float const &x2 = wline.x2;
    float const &y2 = wline.y2;
    float const &length = wline.length;
    float const result = fabs((x2-x1)*(y1-y0) - (x1-x0)*(y2-y1)) / length;
    return result;
  }

  void PFShapeLocalization::setAgent() const {
    const PFShapeLocalization::particle_type& best = particles[bestIndex];
    VRmixin::mapBuilder.setAgent(Point(best.x,best.y,0,allocentric), best.theta);
  }

  void PFShapeLocalization::setPosition(float const x, float const y, AngTwoPi const orientation, float variance) {
    LocalizationParticle part(x, y, orientation);
    setPosition(part, variance);
  }

  void PFShapeLocalization::setWorldBounds(const Shape<PolygonData> &bounds) {
    PFShapeDistributionPolicy<LocalizationParticle> *dist =
      dynamic_cast<PFShapeDistributionPolicy<LocalizationParticle> *>(&(getResamplingPolicy()->getDistributionPolicy()));
    if ( dist != NULL )
      dist->setWorldBounds(bounds);
    else
      cout << "Error: setWorldBounds found wrong type of DistributionPolicy" << endl;
  }

  void PFShapeLocalization::displayParticles(float const howmany) const {
    ShapeSpace &wShS = sensorModel->getWorldShS();
    wShS.deleteShapes<LocalizationParticleData>();
    NEW_SHAPE(best, LocalizationParticleData, new LocalizationParticleData(wShS,particles[bestIndex]));
    best->setColor(VRmixin::mapBuilder.getAgent()->getColor());
    if ( howmany <= 0 ) return;
    unsigned int increment;
    if ( howmany <= 1.0 )
      increment = (unsigned int)ceil(1.0/howmany);
    else
      increment = (unsigned int)ceil(particles.size()/howmany);
    for (unsigned int i=0; i<particles.size(); i+=increment)
      if ( i != bestIndex ) {
	NEW_SHAPE(pt, LocalizationParticleData, new LocalizationParticleData(wShS,particles[i]));
      }
  }

} // namespace
