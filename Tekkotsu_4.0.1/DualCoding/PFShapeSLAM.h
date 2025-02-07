//-*-c++-*-

#ifndef _LOADED_PFShapeSLAM_h_
#define _LOADED_PFShapeSLAM_h_

#include <vector>
#include <iostream>
#include <cmath>
#include "Behaviors/Services/DeadReckoningBehavior.h"
#include "Shared/LocalizationParticle.h"

#include "PFShapeLocalization.h"

#ifndef USE_LOGWEIGHTS
#  define USE_LOGWEIGHTS 1
#endif

namespace DualCoding {
	
  class ShapeSpace;
  class SLAMShapesParticleDistributionPolicy;

  //! Each Particle represents a hypothesis about the match of the local map to the world map, considering changes to the map
  /*! Only handles adding or removing landmarks, not moving existing landmarks.  (A move
   *  can be modeled as an addition plus a deletion, but there will be two penalties to pay.)
   *
   *  Note that the particle evaluation doesn't modify the world map, only
   *  tracks suggestions about what @e should be changed.
   *
   *  The size of addLocal and deleteWorld may not exactly match the number
   *  of current local or world landmarks -- the SensorModel is lazy about resizing
   *  for efficiency reasons. */
  class SLAMShapesParticle : public LocalizationParticle {
  public:
    //! Defines a default DistributionPolicy for the particle type (just reuse the same one as LocalizationParticle)
    typedef LocalizationParticleDistributionPolicy<SLAMShapesParticle> DistributionPolicy;

    //! Constructor
    SLAMShapesParticle() : LocalizationParticle(), addLocal(), deleteWorld() {}
	
    std::vector<bool> addLocal;	//!< true for local landmarks missing from the world map
    std::vector<bool> deleteWorld;	//!< true for world landmarks missing from the local map
  };

  //! extends ParticleShapeEvaluator to handle the addition and removal of landmarks as necessary
  class SLAMParticleShapeEvaluator : public ParticleShapeEvaluator {
  public:
    //! constructor, addPenalty specifies how much to add/multiply (logspace/linear space) weight when considering a landmark as an addition
    SLAMParticleShapeEvaluator(ShapeSpace &localShS, ShapeSpace &worldShS, float addPenalty) :
      ParticleShapeEvaluator(localShS,worldShS), localMobile(false), worldMobile(false), ADDITION_PENALTY(addPenalty)
    {
      for ( unsigned int j=0; j<localLms.size(); j++ )
	localMobile |= localLms[j]->mobile;
      for ( unsigned int k=0; k<worldLms.size(); k++ )
	worldMobile |= worldLms[k]->mobile;
    }
    using ParticleShapeEvaluator::evaluate;
    void evaluate(SLAMShapesParticle& part); //!< provides evaluation of SLAM-particles
  protected:
    void determineAdditions(SLAMShapesParticle& part); //!< may mark landmarks for addition which don't appear in the world map
    void determineDeletions(SLAMShapesParticle& part); //!< may mark landmarks for removal which don't appear in the world map
    bool localMobile; //!< set to true if *any* landmarks are marked as "mobile"
    bool worldMobile; //!< set to true if *any* landmarks are marked as "mobile"
    const float ADDITION_PENALTY; //!< the value passed to the constructor, limits how readily landmarks are added to the map
  };


//================ SLAMShapesSensorModel ================

  //! this wraps the SLAMParticleShapeEvaluator in a ParticleFilter::SensorModel so it can be used by the particle filter
  /*! see ShapeSensorModel for discussion of architectural issues vs separation of SLAMParticleShapeEvaluator */
  template<typename ParticleT>
  class SLAMShapesSensorModel : public ParticleFilter<ParticleT>::SensorModel {
  public:
    typedef typename ParticleFilter<ParticleT>::SensorModel::index_t index_t; //!< convenience typedef
    typedef typename ParticleFilter<ParticleT>::SensorModel::particle_collection particle_collection; //!< convenience typedef
	
    //! constructor, the standard deviation on matches defaults to 60, but you can always reassign #stdevSq directly
    SLAMShapesSensorModel(ShapeSpace &localShS, ShapeSpace &worldShS) :
      stdevSq(60*60), addPenalty(50), lShS(localShS), wShS(worldShS),
      particleLocalLandmarks(0), particleWorldLandmarks(0)
    {}
	
    //! controls how much weight is given to "near-misses"
    float stdevSq;
    //! controls how readily new landmarks are added to the map, vs. penalizing the particle for a bad match
    float addPenalty;
	
    //! applies the SLAMParticleShapeEvaluator across a collection of particles and tracks the best weight
    virtual void evaluate(particle_collection& particles, index_t& bestIndex) {
      float bestWeight=-FLT_MAX;
      SLAMParticleShapeEvaluator eval(lShS,wShS,addPenalty);
		
      if(eval.localLms.size()>particleLocalLandmarks || eval.localLms.size()<particleLocalLandmarks/2)
	for(typename particle_collection::iterator it=particles.begin(); it!=particles.end(); ++it)
	  it->addLocal.resize(particleLocalLandmarks);
		
      if(eval.worldLms.size()>particleWorldLandmarks || eval.worldLms.size()<particleWorldLandmarks/2)
	for(typename particle_collection::iterator it=particles.begin(); it!=particles.end(); ++it)
	  it->deleteWorld.resize(particleWorldLandmarks);
		
      for(typename particle_collection::size_type p=0; p<particles.size(); ++p) {
	eval.evaluate(particles[p]);
	for(unsigned int i=0; i<eval.numMatches; ++i) {
#if USE_LOGWEIGHTS
	  particles[p].weight += -eval.localScores[i]/stdevSq;
#else
	  particles[p].weight *= normpdf(eval.localScores[i]);
#endif
	}
	if(particles[p].weight>bestWeight) {
	  bestWeight=particles[p].weight;
	  bestIndex=p;
	}
      }
    }
	
  ShapeSpace& getLocalShS() const { return lShS; }
  ShapeSpace& getWorldShS() const { return wShS; }

  protected:
    ShapeSpace &lShS;			//!< Local shape space
    ShapeSpace &wShS;			//!< World shape space

    unsigned int particleLocalLandmarks; //!< number of entries in particles' individual addLocal (so we know if we need to resize it in all particles)
    unsigned int particleWorldLandmarks; //!<  number of entries in particles' individual deleteWorld (so we know if we need to resize it in all particles)

    //! computes a (non-normalized) gaussian distribution
    /*! normalization doesn't matter because it's constant across particles, and so
     *  doesn't matter for purposes of comparison between particles */
    inline float normpdf(float const distsq) { return std::exp(-distsq/stdevSq); }
  };


//================ PFShapeSLAM ================

  //! bundles a DeadReckoning motion model and a SLAMShapesSensorModel for easy use of a shape-based particle filter for mapping and localization
  class PFShapeSLAM : public ParticleFilter<SLAMShapesParticle> {
  public:
    //! constructor, must pass local and world shape spaces, which will be used in future calls to update()
    PFShapeSLAM(ShapeSpace &localShS, ShapeSpace &worldShS, unsigned int numParticles=2000)
      : ParticleFilter<SLAMShapesParticle>(numParticles, new DeadReckoningBehavior<SLAMShapesParticle>),
	sensorModel(new SLAMShapesSensorModel<SLAMShapesParticle>(localShS,worldShS))
    {
      if(BehaviorBase* motBeh = dynamic_cast<BehaviorBase*>(motion))
	motBeh->DoStart();
    }
    //! destructor
    virtual ~PFShapeSLAM() {
      if(BehaviorBase* motBeh = dynamic_cast<BehaviorBase*>(motion)) {
	motBeh->DoStop();
	// behaviors are reference counted, stopping removes our reference, set to NULL to avoid call to delete in ParticleFilter
	motion=NULL;
      }
      delete sensorModel;
    }
	
    //! update, triggers a particle filter update using the embedded #sensorModel
    virtual void update(bool updateMot=true, bool doResample=true) { updateSensors(*sensorModel,updateMot,doResample); }
	
    //! accessor for #sensorModel
    virtual SLAMShapesSensorModel<SLAMShapesParticle>& getSensorModel() const { return *sensorModel; }

    //! replaces the sensor model in use, the particle filter will take responsibility for deallocating the sensor model's memory when destructed or replaced
    virtual void setSensorModel(SLAMShapesSensorModel<SLAMShapesParticle>* customSensorModel) { delete sensorModel; sensorModel=customSensorModel; }

    //! updates the mapbuilder's agent's position on worldShS
    virtual void setAgent() const;
  
    //! displays particles on the world map; howmany can either be a percentage (<= 1.0) or a whole number
    virtual void displayParticles(float const howmany=100) const;

  protected:
    SLAMShapesSensorModel<SLAMShapesParticle> * sensorModel; //!< provides evaluation of particles

  private:
    PFShapeSLAM(const PFShapeSLAM&); //!< don't call (copy constructor)
    PFShapeSLAM& operator=(const PFShapeSLAM&); //!< don't call (assignment operator)
  };

  //! allows display of particles on console
  std::ostream& operator<< (std::ostream& os, const SLAMShapesParticle &p);

} // namespace

#endif
