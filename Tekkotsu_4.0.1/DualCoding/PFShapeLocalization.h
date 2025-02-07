//-*-c++-*-

#ifndef _LOADED_PFShapeLocalization_h_
#define _LOADED_PFShapeLocalization_h_

#include <vector>
#include <iostream>
#include <cmath>
#include "Shared/LocalizationParticle.h"
#include "ParticleShapes.h"
#include "Behaviors/Services/DeadReckoningBehavior.h"
#include "DualCoding/ShapePolygon.h"

#ifndef USE_LOGWEIGHTS
#  define USE_LOGWEIGHTS 1
#endif

namespace DualCoding {
	
class ShapeSpace;

  //! provides evaluation of the matching between local and world given a candidate particle
  /*! The reason for separating ParticleShapeEvaluator and ShapeSensorModel?  Partly so the
   *  fairly lengthy evaluation code can go in the .cc file to avoid repeated recompilation, but also to
   *  allow inheritance (e.g. SLAMShapeParticleEvaluator) as a clean way to extend the 
   *  evaluation code for particle sub-types. */
class ParticleShapeEvaluator {
public:
    //! constructor, pass the local and world shape spaces, these will be used to initialize the appropriate particle-independent fields of the class
  ParticleShapeEvaluator(ShapeSpace &localShS, ShapeSpace &worldShS);
  virtual ~ParticleShapeEvaluator() {} //!< destructor
	
  //! the heart of the class, call with a particle, will adjust the weight
  void evaluate(LocalizationParticle& part);
  
  void setMaxDist(float const dist) { maxDist = dist; }
  const std::vector<float>& getLocalScores() const { return localScores; }
  unsigned int getNumMatches() const { return numMatches; }

  std::vector<PfRoot*> localLms; //!< a vector of the landmarks in the local space
  std::vector<PfRoot*> worldLms; //!< a vector of landmarks in the world space
  
  float maxDist; //!< maximum distance for a landmark to be useful in distance error calculation

  std::vector<int> localMatches; 		//!< Index of best matching world landmark for each local landmark according to the currently-selected particle
  unsigned int numMatches; //!< number of matches found
  std::vector<float> localScores;		//!< Match scores for local landmarks according to this particle, only first #numMatches are filled in (skip non-matching)
  
  //! helper function which calculates the distance between a point and a line along a perpendicular
  static float distanceFromLine(coordinate_t x0, coordinate_t y0, PfLine &wline);
  
  std::vector<float> particleViewX;		//!< x coords of local landmarks according to the currently-selected particle
  std::vector<float> particleViewY;		//!< y coords of local landmarks according to the currently-selected particle
  std::vector<float> particleViewX2;		//!< x coords of other point of local line
  std::vector<float> particleViewY2;		//!< y coords of other point of local line
};
  


//================ ShapeSensorModel ================

//! this wraps the ParticleShapeEvaluator in a ParticleFilter::SensorModel so it can be used by the particle filter
/*! The reason for separating ParticleShapeEvaluator and ShapeSensorModel?  Partly so the
 *  fairly length evaluation code can go in the .cc file to avoid repeated recompilation, but also to
 *  allow inheritance (e.g. SLAMShapesParticleEvaluator) as a clean way to extend the 
 *  evaluation code for particle sub-types.  Ideally, I'd like to combine these classes. */

template<typename ParticleT>
class ShapeSensorModel : public ParticleFilter<ParticleT>::SensorModel {
public:
  typedef typename ParticleFilter<ParticleT>::SensorModel::index_t index_t; //!< convenience typedef
  typedef typename ParticleFilter<ParticleT>::SensorModel::particle_collection particle_collection; //!< convenience typedef
  
  //! constructor, the standard deviation on matches defaults to 60, but you can always reassign #stdevSq directly
  ShapeSensorModel(ShapeSpace &localShS, ShapeSpace &worldShS) :
    stdevSq(60*60), lShS(localShS), wShS(worldShS)
  {}
  
  //! controls how much weight is given to "near-misses"
  float stdevSq;
  
  //! applies the ParticleShapeEvaluator across a collection of particles and tracks the best weight
  virtual void evaluate(particle_collection& particles, index_t& bestIndex) {
    float bestWeight=-FLT_MAX;
    ParticleShapeEvaluator eval(lShS,wShS);
    for(typename particle_collection::size_type p=0; p<particles.size(); ++p) {
      eval.evaluate(particles[p]);
      for(unsigned int i=0; i<eval.getNumMatches(); ++i) {
#if USE_LOGWEIGHTS
	particles[p].weight += -eval.getLocalScores()[i]/stdevSq;
#else
	particles[p].weight *= normpdf(eval.getLocalScores()[i]);
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
  
  //!< computes a (non-normalized) gaussian distribution
  /*! normalization isn't needed because the scale factor is constant across particles, and so
   *  doesn't matter for purposes of comparison between particles */
  inline float normpdf(float const distsq) { return std::exp(-distsq/stdevSq); }
};
  

//================ PFShapeDistributionPolicy ================

template<typename ParticleT>
class PFShapeDistributionPolicy : public LocalizationParticleDistributionPolicy<ParticleT> {
public:
  typedef ParticleT particle_type;  //!< just for convenience
  typedef typename ParticleFilter<particle_type>::index_t index_t; //!< just for convenience

  PFShapeDistributionPolicy() : LocalizationParticleDistributionPolicy<ParticleT>(), worldBounds() {}

  virtual void randomize(particle_type* begin, index_t num) {
    particle_type* end=&begin[num]; 
    while(begin!=end) { 
      while (1) { // loop until particle is acceptable
	begin->x = float(rand())/RAND_MAX * LocalizationParticleDistributionPolicy<ParticleT>::mapWidth + 
	  LocalizationParticleDistributionPolicy<ParticleT>::mapMinX;
	begin->y = float(rand())/RAND_MAX * LocalizationParticleDistributionPolicy<ParticleT>::mapHeight + 
	  LocalizationParticleDistributionPolicy<ParticleT>::mapMinY;
	if ( !worldBounds.isValid() || worldBounds->isInside(Point(begin->x,begin->y)) )
	  break;
      }
      begin->theta = float(rand())/RAND_MAX * 2 * M_PI;
      ++begin;
    }
  }

  virtual void jiggle(float var, particle_type* begin, index_t num) {
    if(var==0)
      return;
    particle_type* end=&begin[num]; 
    while(begin!=end) {
      float dx=0, dy=0;
      for (int i=0; i<4; i++) {
	float rx = DRanNormalZig32()*LocalizationParticleDistributionPolicy<ParticleT>::positionVariance*var;
	float ry = DRanNormalZig32()*LocalizationParticleDistributionPolicy<ParticleT>::positionVariance*var;
	if ( !worldBounds.isValid() || worldBounds->isInside(Point(begin->x+rx, begin->y+ry)) ) {
	  dx = rx;
	  dy = ry;
	  break;
	}
      }
      begin->x += dx;
      begin->y += dy;
      begin->theta+=DRanNormalZig32()*LocalizationParticleDistributionPolicy<ParticleT>::orientationVariance*var;
      ++begin;
    }
  }

  virtual void setWorldBounds(const Shape<PolygonData> bounds) {
    worldBounds = bounds;
    if ( worldBounds.isValid() ) {
      BoundingBox b(worldBounds->getBoundingBox());
      LocalizationParticleDistributionPolicy<ParticleT>::mapMinX = b.xmin;
      LocalizationParticleDistributionPolicy<ParticleT>::mapWidth = b.xmax - b.xmin;
      LocalizationParticleDistributionPolicy<ParticleT>::mapMinY = b.ymin;
      LocalizationParticleDistributionPolicy<ParticleT>::mapHeight = b.ymax - b.ymin;
    }
  }

protected:
  Shape<PolygonData> worldBounds;	//!< If valid shape, particles must lie inside it.
};

//================ PFShapeLocalization ================

//! bundles a DeadReckoning motion model and a ShapeSensorModel for easy use of a shape-based particle filter for localization

class PFShapeLocalization : public ParticleFilter<LocalizationParticle> {
public:
  //! constructor, must pass local and world shape spaces, which will be used in future calls to update()
  PFShapeLocalization(ShapeSpace &localShS, ShapeSpace &worldShS, unsigned int numParticles=2000)
    : ParticleFilter<LocalizationParticle>(numParticles, new DeadReckoningBehavior<LocalizationParticle>),
      sensorModel(new ShapeSensorModel<LocalizationParticle>(localShS,worldShS))
  {
    getResamplingPolicy()->setDistributionPolicy(new PFShapeDistributionPolicy<LocalizationParticle>);
    if(BehaviorBase* motBeh = dynamic_cast<BehaviorBase*>(motion))
      motBeh->DoStart();
  }

  //! destructor
  virtual ~PFShapeLocalization() { 
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
  virtual ShapeSensorModel<LocalizationParticle>& getSensorModel() const { return *sensorModel; }

  //! replaces the sensor model in use, the particle filter will take responsibility for deallocating the sensor model's memory when destructed or replaced
  virtual void setSensorModel(ShapeSensorModel<LocalizationParticle>* customSensorModel) {
    delete sensorModel; 
    sensorModel=customSensorModel;
  }

  //! updates the mapbuilder's agent's position on worldShS
  virtual void setAgent() const;
  
  //! resets particles to the specified position and orientation, and optionally jiggles them by variance
  virtual void setPosition(float const x, float const y, AngTwoPi const orientation, float variance=0);
  using ParticleFilter<LocalizationParticle>::setPosition;

  //! sets boundary within which particles should lie
  virtual void setWorldBounds(const Shape<PolygonData> &bounds);

  //! displays particles on the world map; howmany can either be a percentage (<= 1.0) or a whole number
  virtual void displayParticles(float const howmany=100) const;

protected:
  ShapeSensorModel<LocalizationParticle> * sensorModel; //!< provides evaluation of particles
  
private:
  PFShapeLocalization(const PFShapeLocalization&); //!< don't call (copy constructor)
  PFShapeLocalization& operator=(const PFShapeLocalization&); //!< don't call (assignment operator)
};

} // namespace

#endif
