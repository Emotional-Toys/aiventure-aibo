//-*-c++-*-
#ifndef INCLUDED_PathPlanner_h_
#define INCLUDED_PathPlanner_h_

#include <map>
#include <iostream>
#include <queue>
#include <map>
#include <bitset>
#include "DualCoding.h"

namespace DualCoding {

// plans path given start and goal locations, landmarks, obstacles and area allowed to be in.
// path is a set of waypoints which consist of position, heading and landmark(s).
// path is actually computed from goal to start, which I guess could have been other way around
class PathPlanner {
public:
  PathPlanner(coordinate_t xMax, coordinate_t xMin, coordinate_t yMax, coordinate_t yMin);
  virtual ~PathPlanner() { 
    delete [] unreached; 
    delete [] costMap; 
  }

  //{ data structures used to represent the map and path
  struct direction {
    std::bitset<2> bitVal; // 00=N, 01=W, 10=S, 11=E
    direction (unsigned int i) : bitVal(i) {}
    operator unsigned int() const { return (unsigned int) bitVal.to_ulong(); }
    enum directions { N=0,W=1,S=2,E=3 };// dir;
    direction cw() const { direction cwd(bitVal.to_ulong()+1); return cwd; }
    direction ccw() const { direction ccwd(bitVal.to_ulong()-1); return ccwd; }
  };
  struct location {
    unsigned int pos;
    direction dir;
    location()
      : pos(0), dir(direction::N) {}
    location(unsigned int _pos, direction _dir)
      : pos(_pos), dir(_dir) {}
    location(const location& l)
      : pos(l.pos), dir(l.dir) {}
    bool operator==(const location& l) const { return pos==l.pos && dir==l.dir; }
  };
  struct state { // structure representing each node 
    location loc;
    unsigned int cost;
    std::pair<int,int> lms;
    state* dst; // optimal transition to be found
    state(location _loc, int lm1, int lm2)
      : loc(_loc), cost(0), lms(lm1, lm2), dst(NULL) {} //, links() {}
    state(location _loc, std::pair<int,int> _lms)
      : loc(_loc), cost(0), lms(_lms), dst(NULL) {} //, links() {}
    state(const state& s)
      : loc(s.loc), cost(s.cost), lms(s.lms), dst(&(*s.dst)) {} //, links(s.links) {}
    state& operator=(const state& s) 
    { if (this!=&s) { loc = s.loc; cost = s.cost; lms = s.lms; dst = s.dst; } return *this; }
  };
  struct edge { // structure representing transition b/w two states
    state *from, *to;
    unsigned int cost; // cost of transition
    edge(state& _from, state& _to, unsigned int _cost) : from(&_from), to(&_to), cost(_cost) {}
    edge(const edge& l) : from(&(*l.from)), to(&(*l.to)), cost(l.cost) {}
    edge& operator=(const edge& l) 
    { if (this!=&l) { from = &(*l.from); to = &(*l.to); cost = l.cost; } return *this; }
  };
  //}

protected:
  std::queue<state> allStates; // all possible states
  std::vector<state*> *unreached; // collection of states for which optimal action is not determined yet
  // priority queue containing edges evaluated so far in order of increasing cost, core of this algorithm
  struct lessCost { bool operator()(const edge& e1, const edge& e2) const { return e1.cost > e2.cost; }};
  std::priority_queue<edge,std::vector<edge>,lessCost> reached;
  static const int size=300; // length of dog
  unsigned int numX, numY;
  float minX, minY, dX, dY;
  location start, goal;
public:
  unsigned int maxDistance;
  enum Cost { transF, transB, transY, tranB, rotate, noLM, oneLM };
  std::map <Cost, unsigned int> costs; // all costs must be positive for this algorithm to work
  Point startPt, goalPt;
  AngTwoPi startOrientation, goalOrientation;
  
protected:
  unsigned int *costMap; // measure of how close you are to obstacles
  // maps b/w shape id and location/color info
  std::map<unsigned int, PointData> landmarks; 
  // maps b/w shape ids and their score(distinguishability from other lms) 
  std::map<unsigned int, unsigned int> lmCosts; 
  std::vector<std::pair<Point, unsigned int> > obstacles; //<! position of obstacles and their relative costs

public:
  void findPath(); // finds path from goal to start using Dijkstra's algorithm
  void addLandmark(const ShapeRoot& lm) { //adds landmark
    landmarks.insert(std::make_pair<unsigned int,PointData>
		     (lm->getId(),PointData(VRmixin::worldShS,Point(lm->getCentroid()))));
  }
  void addLandmarkAsObstacle(const ShapeRoot& lm, unsigned int cost) { // adds landmark also as obstacle
    addLandmark(lm);
    obstacles.push_back(std::pair<Point,unsigned int>(lm->getCentroid(),cost));
  }

protected:
  bool isLMVisible(location loc, const Point& lm); // finds if landmark is visible from the location
  void initialize(); // defines all possible states (vertices)
  void fillState(unsigned int low, unsigned int high);
  void findLinks(state& s); // find all possible links for given state and store them inside the state
  std::vector<std::pair<int,int> > findLMs(location loc);
  state* thereIs(unsigned int pos, direction dir, std::pair<int,int> lms);
  void computeLandmarkCosts();
  Point findWorldCoords(unsigned int pos);
  std::string toString(const PathPlanner::state &s);

private:
  PathPlanner& operator=(const PathPlanner&);
  PathPlanner(const PathPlanner&);

};
  std::ostream& operator<<(std::ostream& out, const PathPlanner::direction& d);

} // namespace

#endif
