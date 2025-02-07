//-*-c++-*-
#include "PathPlanner.h"
#include "Shared/ProjectInterface.h"
#include <iostream>
#include <ctime>
#include <vector>
#include <queue>
#include <map>
#include <math.h>

using namespace std;

namespace DualCoding {

using ProjectInterface::defSegmentedColorGenerator;

PathPlanner::PathPlanner(coordinate_t xMax, coordinate_t xMin, coordinate_t yMax, coordinate_t yMin)
  : allStates(), unreached(NULL), reached(), 
    numX((unsigned int) ((xMax-xMin)/size)+1), numY((unsigned int) ((yMax-yMin)/size)+1), 
    minX(xMin), minY(yMin), dX((xMax-xMin)/numX), dY((yMax-yMin)/numY), 
    start(), goal(), maxDistance(800), costs(), 
    startPt(), goalPt(),startOrientation(), goalOrientation(),
    costMap(NULL), landmarks(), lmCosts(), obstacles()
{
  unreached = new vector<state*>[numX*numY*4];
  costMap = new unsigned int [numX*numY];
  // default costs; these can be modefied anytime before calling findPath()
  costs[transF] = 1;
  costs[transB] = 2;
  costs[transY] = 4;
  costs[rotate] = 3;
  costs[noLM] = 20;
  costs[oneLM] = 10;
}

void PathPlanner::findPath() {
  initialize();
  while (!reached.empty()) {
    edge top = reached.top();
    if (top.from != NULL && top.from->dst == NULL) { // best action not determined yet for this state
      top.from->cost = top.cost;
      top.from->dst = top.to;
      vector<state*>& lms = unreached[top.from->loc.pos*4 + top.from->loc.dir];
      for (vector<state*>::iterator it = lms.begin();
	   it != lms.end(); it++)
	if ((*it)->lms == top.from->lms) {
	  lms.erase(it);
	  break;
	}
      if (top.from->loc==start) {
	cout << "best path:\n";
	state* s = top.from;
	while(s) {
	  cout << toString(*s)  << endl;
	  s = s->dst;
	}
	cout << endl;
	break;
      }
      else
	findLinks(*top.from);
    }
    reached.pop();
  }
  cout << "done: " << reached.empty() << endl;
}

void PathPlanner::findLinks(state& s) {
  unsigned int newCostMotion = s.cost+lmCosts[s.lms.first]+lmCosts[s.lms.second]+1;
  if (s.lms.first < 0)
    if (s.lms.second < 0)
      newCostMotion += costs[noLM];
    else
      newCostMotion += costs[oneLM];
  { //translate
    if (s.loc.pos%numY+1 < numY) // go west
      if (state* it = thereIs(s.loc.pos+1,s.loc.dir,s.lms))
	if (s.loc.dir==direction::E)
	  reached.push(edge(*it,s,newCostMotion+costMap[s.loc.pos]+costs[transB]));
	else if (s.loc.dir == direction::W)
	  reached.push(edge(*it,s,newCostMotion+costMap[s.loc.pos]+costs[transF]));
	else
	  reached.push(edge(*it,s,newCostMotion+costMap[s.loc.pos]+costs[transY]));
    if (s.loc.pos%numY > 0) // go east
      if (state* it = thereIs(s.loc.pos-1,s.loc.dir,s.lms))
	if (s.loc.dir==direction::E)
	  reached.push(edge(*it,s,newCostMotion+costMap[s.loc.pos]+costs[transF]));
	else if (s.loc.dir == direction::W)
	  reached.push(edge(*it,s,newCostMotion+costMap[s.loc.pos]+costs[transB]));
	else
	  reached.push(edge(*it,s,newCostMotion+costMap[s.loc.pos]+costs[transY]));
    if (s.loc.pos < numY*(numX-1)) // go north
      if (state* it = thereIs(s.loc.pos+numY,s.loc.dir,s.lms))
	if (s.loc.dir==direction::N)
	  reached.push(edge(*it,s,newCostMotion+costMap[s.loc.pos]+costs[transF]));
	else if (s.loc.dir == direction::S)
	  reached.push(edge(*it,s,newCostMotion+costMap[s.loc.pos]+costs[transB]));
	else
	  reached.push(edge(*it,s,newCostMotion+costMap[s.loc.pos]+costs[transY]));
    if (s.loc.pos >= numY) // go south
      if (state* it = thereIs(s.loc.pos-numY,s.loc.dir,s.lms))
	if (s.loc.dir==direction::N)
	  reached.push(edge(*it,s,newCostMotion+costMap[s.loc.pos]+costs[transB]));
	else if (s.loc.dir == direction::S)
	  reached.push(edge(*it,s,newCostMotion+costMap[s.loc.pos]+costs[transF]));
	else
	  reached.push(edge(*it,s,newCostMotion+costMap[s.loc.pos]+costs[transY]));
  }
  { //rotate
    const int newRotateCost = newCostMotion+costs[rotate];
    if (state* it = thereIs(s.loc.pos,s.loc.dir.cw(),s.lms))
      reached.push(edge(*it,s,newRotateCost+costMap[s.loc.pos]));
    if (state* it = thereIs(s.loc.pos,s.loc.dir.ccw(),s.lms))
      reached.push(edge(*it,s,newRotateCost+costMap[s.loc.pos]));
  }
  { //switch landmarks
    vector<pair<int,int> > lms = findLMs(s.loc);
    for (vector<pair<int,int> >::const_iterator it = lms.begin();
	 it != lms.end(); it++)
      if (*it != s.lms)
	reached.push(edge(*(thereIs(s.loc.pos,s.loc.dir,*it)),s,s.cost+1));
  }
}


Point PathPlanner::findWorldCoords(unsigned int pos) {
  return Point (minX+dX*(pos/numY),minY+dY*(pos%numY),0);
}


void PathPlanner::computeLandmarkCosts() {
  map<unsigned int, float> distSum; // maps id of landmark against sum of distance with other landmarks of same color
  map<unsigned int, vector<unsigned int> > lmsSortedByColor; // maps color index against vector of idlandmarks' id having the color
  for (map<unsigned int, PointData>::const_iterator it1 = landmarks.begin();
       it1 != landmarks.end(); it1++) {
    map<unsigned int, PointData>::const_iterator it2 = it1; // cannot write it2 = it1+1?
    it2++;
    for (; it2 != landmarks.end(); it2++)
      if (it1->second.getColor() == it2->second.getColor()) {
	float dist = it1->second.getCentroid().xyDistanceFrom(it2->second);
	float cost = 10000/(dist*dist);
	distSum[it1->first] += cost;
	distSum[it2->first] += cost;
      }
    lmsSortedByColor[defSegmentedColorGenerator->getColorIndex(it1->second.getColor())].push_back(it1->first);
  }
  for (map<unsigned int, vector<unsigned int> >::const_iterator col_it = lmsSortedByColor.begin();
       col_it != lmsSortedByColor.end(); col_it++)
    for (vector<unsigned int>::const_iterator lm_it = col_it->second.begin();
	 lm_it != col_it->second.end(); lm_it++)
      lmCosts[*lm_it] = (unsigned int) distSum[*lm_it];
}

void PathPlanner::initialize() {
  // set start and goal
  float minStartDist = findWorldCoords(0).distanceFrom(startPt);
  float minGoalDist = findWorldCoords(0).distanceFrom(goalPt);
  unsigned int startIdx=0, goalIdx=0;
  for (unsigned int pos = 1; pos < numX*numY; pos++) {
    float startDist = findWorldCoords(pos).distanceFrom(startPt);
    float goalDist = findWorldCoords(pos).distanceFrom(goalPt);
    if (startDist < minStartDist) {
      minStartDist = startDist;
      startIdx = pos;
    }
    if (goalDist < minGoalDist) {
      minGoalDist = goalDist;
      goalIdx = pos;
    }
  }
  start.pos = startIdx;
  goal.pos = goalIdx;
  
  if (startOrientation > M_PI/4) {
    if (startOrientation < M_PI*3/4)
      start.dir = direction::W;
    else if (startOrientation < M_PI*5/4)
      start.dir = direction::S;
    else if (startOrientation < M_PI*7/4)
      start.dir = direction::E;
  }
  else
    start.dir = direction::N;

  if (goalOrientation > M_PI/4) {
    if (goalOrientation < M_PI*3/4)
      goal.dir = direction::W;
    else if (goalOrientation < M_PI*5/4)
      goal.dir = direction::S;
    else if (goalOrientation < M_PI*7/4)
      goal.dir = direction::E;
  }
  else
    goal.dir = direction::N;


  // initialize cost map
  for (unsigned int pos = 0; pos < numX*numY; pos++) {
    costMap[pos] = 0;
    for (vector<pair<Point, unsigned int> >::const_iterator it = obstacles.begin();
	 it != obstacles.end(); it++) {
      Point vec = findWorldCoords(pos)-it->first;
      float distSq = (vec.coordX()*vec.coordX() + vec.coordY()*vec.coordY())/10000; // cm^2
      costMap[pos] += (unsigned int) ((distSq > 0) ? it->second/distSq : -1U);
    }
  }
  computeLandmarkCosts();
  cout << "costs:\n";
  cout << " costs[transF]: " << costs[transF] << endl;
  cout << " costs[transB]: " << costs[transB] << endl;
  cout << " costs[transY]: " << costs[transY] << endl;
  cout << " costs[rotate]: " << costs[rotate] << endl;
  cout << " costs[noLM]: " << costs[noLM] << endl;
  cout << " costs[oneLM]: " << costs[oneLM] << endl;
  
  cout << "landmarks:\n"; 
  for (map<unsigned int, PointData>::const_iterator it = landmarks.begin();
       it != landmarks.end(); it++)
    cout<< "lm id " << it->first << " at " << it->second
      //	<< ", color index=" << defSegmentedColorGenerator->getColorIndex(it->second.getColor())
	<< ", cost=" << lmCosts[it->first] << endl;
  
  cout << "obstacles:\n";
  for (vector<pair<Point, unsigned int> >::const_iterator it = obstacles.begin();
       it != obstacles.end(); it++)
    cout << " " << it->first << ": " << it->second << endl;

  // print map
  cout << "costMap:\n";
  for (int j = numX-1; j >= 0; j--) {
    for (int i = numY-1; i >= 0; i--)
      if (i+j*numY == start.pos)
	cout << " S";
      else if (i+j*numY == goal.pos)
	cout << " G";
      else
	cout << " " << costMap[i+j*numY];
    cout << endl;
  }

  while (!reached.empty()) 
    reached.pop();
  for (unsigned int i = 0; i < numX*numY*4; i++)
    unreached[i].clear();
  while (!allStates.empty())
    allStates.pop();

  // find all possible vertices (states)
  vector<state*> goals;
  fillState(0,goal.pos);
  for (unsigned int i = 0; i < 4; i++) {
    location loc (goal.pos,i);
    vector<pair<int,int> > lms = findLMs(loc);
    for (vector<pair<int,int> >::const_iterator it = lms.begin();
	 it < lms.end(); it++) {
      allStates.push(state(loc,*it));
      if (i == goal.dir)
	goals.push_back(&allStates.back());
      else
	unreached[loc.pos*4 + loc.dir].push_back(&(allStates.back()));
    }
  }
  fillState(goal.pos+1,numX*numY);

  cout << "start state:\n";
  cout << " " << findWorldCoords(start.pos) << endl;
  //  for (vector<
  //	 cout << " is lm 1 visible ? " << isLMVisible(start,pt) << endl;

  cout << "goal state(s):\n";
  for (vector<state*>::iterator it = goals.begin();
       it != goals.end(); it++) {
    (*it)->cost = 0;
    findLinks(**it);
    cout << " " << toString(**it) <<  " => " << findWorldCoords((*it)->loc.pos) << endl;
  }
}

void PathPlanner::fillState(unsigned int low, unsigned int high) {
  for (unsigned int pos = low; pos < high; pos++) {
    for (unsigned int i = 0; i < 4; i++) {
      location loc(pos,i);
      vector<pair<int,int> > lms = findLMs(loc);
      for (vector<pair<int,int> >::const_iterator it = lms.begin();
	   it < lms.end(); it++) {
	allStates.push(state(loc,*it));
	unreached[loc.pos*4 + loc.dir].push_back(&(allStates.back()));
      }
    }
  }
}
  
PathPlanner::state* PathPlanner::thereIs(unsigned int pos, PathPlanner::direction dir, std::pair<int,int> lms) {
  //  cout << "thereIs: " << pos << ',' << dir << ',' << (pos+dir) << endl;
  const vector<state*>& states = unreached[pos*4 + (unsigned int) dir];
  for (vector<state*>::const_iterator it = states.begin();
       it < states.end(); it++)
    if ((*it)->lms == lms)
      return *it;
  return NULL;
}


std::vector<std::pair<int,int> > PathPlanner::findLMs(location loc) {
  vector<int> lms;
  lms.push_back(-1); // no landmark is also an option
  for (map<unsigned int, PointData>::const_iterator it = landmarks.begin();
       it != landmarks.end(); it++)
    if (isLMVisible(loc,it->second)) lms.push_back(it->first);
  vector<pair<int,int> > lmPairs;
  lmPairs.push_back(pair<int,int>(-1,-1));
  for(vector<int>::const_iterator it1 = lms.begin();
      it1 != lms.end(); it1++)
    for(vector<int>::const_iterator it2 = it1+1;
	it2 != lms.end(); it2++)
      lmPairs.push_back(pair<int,int>(*it1,*it2));
  return lmPairs;
}

bool PathPlanner::isLMVisible(location l, const Point& lm) {
  Point agent = findWorldCoords(l.pos);
  float dx = lm.coordX()-agent.coordX();
  float dy = lm.coordY()-agent.coordY();
  float distance = sqrt(dx*dx + dy*dy);
  //  cout << "distance: " << distance << endl;
  if (distance > maxDistance || distance < 200) return false; // too far or too close
  AngTwoPi angle = atan2(dy,dx);
  //  cout << "angle: " << angle << endl;
  switch (l.dir) {
  case direction::N: return (angle < M_PI/3 || angle > M_PI*5/3);
  case direction::W: return (angle > M_PI/6 && angle < M_PI*5/6);
  case direction::S: return (angle > M_PI*2/3 && angle < M_PI*4/3);
  case direction::E: return (angle > M_PI*7/6 && angle < M_PI*11/6);
  };
  return false;
}

std::string PathPlanner::toString(const PathPlanner::state &s) {
  stringstream ss;
  ss << "[(" << (s.loc.pos/numY) << ',' << (s.loc.pos%numY) << ','
     << s.loc.dir << "), (" << s.lms.first << ',' << s.lms.second << "), " << s.cost << ']';
  return ss.str();
}

std::ostream& operator<<(std::ostream& out, const PathPlanner::direction& d) {
  switch (d) {
  case PathPlanner::direction::N: out << "N"; break;
  case PathPlanner::direction::W: out << "W"; break;
  case PathPlanner::direction::S: out << "S"; break;
  default: out << "E"; break;
  };
  return out;
}


}
