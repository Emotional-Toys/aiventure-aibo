//-*-c++-*-

#include <iostream>
#include <cmath>

#include "ShapeSpace.h"
#include "MapBuilder.h"
#include "PFShapeSLAM.h"
#include "ShapeLocalizationParticle.h"
#include "VRmixin.h"  // for mapBuilder

using namespace std;

namespace DualCoding {

  void SLAMParticleShapeEvaluator::evaluate(SLAMShapesParticle& part) {
    ParticleShapeEvaluator::evaluate(part);
    for(unsigned int i=0; i<localMatches.size(); i++)
      part.addLocal[i] = ( localMatches[i] == -1 && localLms[i]->mobile );
    if ( localMobile )
      determineAdditions(part);
    if ( worldMobile )
      determineDeletions(part);
    // cout << "computeParticleScores(): best particle = " << bestIndex
    // << ": " << bestProb << endl;
  }

  void SLAMParticleShapeEvaluator::determineAdditions(SLAMShapesParticle& part) {
    for (unsigned int j = 0; j<localLms.size(); j++) {
      if ( localLms[j]->mobile  ) {
	float const randval = float(rand()) / (float(RAND_MAX)*6);
	if (randval >= localScores[j]) {
	  part.addLocal[j] = true;
	  localScores[numMatches++] = ADDITION_PENALTY;
	  localMatches[j] = -1;
	  continue;
	}
	for (unsigned int j2 = (j+1); j2<localLms.size(); j2++) {
	  if (localMatches[j2] != localMatches[j] || localMatches[j2] == -1)
	    continue;
	  if (localScores[j2] > localScores[j]) {
	    part.addLocal[j] = true;
	    localScores[numMatches++] = ADDITION_PENALTY;
	    localMatches[j] = -1;
	  } else {
	    part.addLocal[j2] = true;
	    localScores[numMatches++] = ADDITION_PENALTY;
	    localMatches[j2] = -1;
	  }
	}
      }
    }
  }

  void SLAMParticleShapeEvaluator::determineDeletions(SLAMShapesParticle& part) {
    part.deleteWorld.assign(part.deleteWorld.size(),true);
	
    float minXLoc = HUGE_VAL;
    float minYLoc = HUGE_VAL;
    float maxXLoc = -HUGE_VAL;
    float maxYLoc = -HUGE_VAL;
    for (unsigned int j = 0; j<localLms.size(); j++) {
      if ( localMatches[j] != -1 )
	part.deleteWorld[localMatches[j]] = false;  // don't delete world LM if it matches
      if ( particleViewX[j] < minXLoc )
	minXLoc = particleViewX[j];
      else if (particleViewX[j] > maxXLoc)
	maxXLoc = particleViewX[j];
      if (particleViewY[j] < minYLoc)
	minYLoc = particleViewY[j];
      else if (particleViewY[j] > maxYLoc)
	maxYLoc = particleViewY[j];
    }
	
    for (unsigned int k = 0; k<worldLms.size(); k++)
      if ( ! worldLms[k]->mobile ||
	   !( worldLms[k]->x >= minXLoc && worldLms[k]->x <= maxXLoc &&
	      worldLms[k]->y >= minYLoc && worldLms[k]->y <= maxYLoc ) )
	part.deleteWorld[k] = false; // don't delete world LM if not mobile, or it was outside local view
  }


  void PFShapeSLAM::setAgent() const {
    const PFShapeSLAM::particle_type& best = particles[bestIndex];
    VRmixin::mapBuilder.setAgent(Point(best.x,best.y), best.theta);
  }

  void PFShapeSLAM::displayParticles(float const howmany) const {
    ShapeSpace &wShS = sensorModel->getWorldShS();
    wShS.deleteShapes<LocalizationParticleData>();
    NEW_SHAPE(best, LocalizationParticleData, new LocalizationParticleData(wShS,particles[bestIndex]));
    best->setColor(VRmixin::mapBuilder.getAgent()->getColor());
    if ( howmany <= 0 ) return;
    int increment;
    if ( howmany <= 1.0 )
      increment = (int)ceil(1.0/howmany);
    else
      increment = (int)ceil(particles.size()/howmany);
    for (unsigned int i=0; i<particles.size(); i+=increment)
      if ( i != bestIndex ) {
	NEW_SHAPE(pt, LocalizationParticleData, new LocalizationParticleData(wShS,particles[i]));
      }
  }

  ostream& operator << (ostream& os, const SLAMShapesParticle &p){
    os << "Particle(p=" << p.weight
       << ", dx=" << p.x
       << ", dy=" << p.y
       << ", th=" << p.theta;
	
    os << ", add=";
    for (unsigned int i = 0; i<p.addLocal.size(); i++)
      os << p.addLocal[i];
	
    os << ", del=";
    for (unsigned int i = 0; i<p.deleteWorld.size(); i++)
      os << p.deleteWorld[i];
	
    os << ")";
    return os;
  }

} // namespace
