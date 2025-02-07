//-*-c++-*-

#include <math.h>
#include "susan.h"

#include "visops.h"

using namespace DualCoding;

namespace visops {

Sketch<bool> zeros(SketchSpace& space) {
  Sketch<bool> result(space,"zeros()");
  *result.pixels = 0;  // valarray assignment
  return result;
}

Sketch<bool> zeros(const SketchRoot& other) {
  const Sketch<bool>& fake = reinterpret_cast<const Sketch<bool>&>(other);
  Sketch<bool> result("zeros("+fake->getName()+")", fake);
  *result.pixels = 0;  // valarray assignment
  return result;
}

Sketch<bool> colormask(const Sketch<uchar>& src, const std::string &colorname) {
  return colormask(src, ProjectInterface::getColorIndex(colorname));
}

Sketch<bool> colormask(const Sketch<uchar>& src, color_index cindex) {
  Sketch<bool> result(src == cindex);
  result->setColor(ProjectInterface::getColorRGB(cindex));
  return result;
}

Sketch<uint> bdist(const Sketch<bool>& dest, 
			    const Sketch<bool>& obst, 
			    const uint maxdist) {
  SketchSpace &space = dest->getSpace();
  space.requireIdx4way();
  Sketch<uint> result("bdist("+dest->getName()+","+obst->getName()+")", dest);
  result = maxdist;
  result->setColorMap(jetMapScaled);
  
  // Start at the target and iteratively expand out the frontier
  SketchIndices frontier;
  frontier.addIndices(dest);
  result.setIndices(frontier, 0);
  SketchIndices obstInds;
  obstInds.addIndices(obst);
  SketchIndices newFrontier, oldFrontier;

  for (uint dist = 1; dist < maxdist; dist++) {
    // newFrontier should become all values adjacent to frontier, not
    // actually in the frontier, not a boundary, or in oldFrontier
    newFrontier = frontier[*space.idxN] + frontier[*space.idxS] 
      + frontier[*space.idxW] + frontier[*space.idxE]
      - (obstInds + frontier + oldFrontier);
    newFrontier.trimBounds(space);
    // if no more frontier, done
    if (newFrontier.table.empty())
      break;
    result.setIndices(newFrontier, dist);
    oldFrontier = frontier;
    frontier = newFrontier;
  }
  
  return result;
}

Sketch<uint> edist(const Sketch<bool>& target) {
  SketchSpace &space = target->getSpace();
  const int width = space.getWidth();
  const int height = space.getHeight();
  const uint maxdist = width + height + 1;
  // at the moment doing 4-pass linear Manhattan distance, but should
  // probably use linear-time Euclidean algorithm described in 
  Sketch<uint> dist("edist("+target->getName()+")", target);
  dist = maxdist;
  dist->setColorMap(jetMapScaled);
  
  // find min distances within each row
  for (int j = 0; j < height; j++) {
    for (int i = 0, cur_dist = maxdist; i < width; i++) {
      if (target(i,j) == 1)
				dist(i,j) = cur_dist = 0;
      else if (dist(i,j) < (uint)cur_dist)
				cur_dist = dist(i,j);
      else dist(i,j) = cur_dist;
      cur_dist++;
    }
    for (int i = width-1, cur_dist = maxdist; i >= 0; i--) {
      if (target(i,j) == true)
	dist(i,j) = cur_dist = 0;
      else if (dist(i,j) < (uint)cur_dist)
	cur_dist = dist(i,j);
      else
	dist(i,j) = cur_dist;
      cur_dist++;
    }
  }
  // find min distances within each column
  for (int i = 0; i < width; i++) {
    for (int j = 0, cur_dist = maxdist; j < height; j++) {
      if (target(i,j) == 1)
				dist(i,j) = cur_dist = 0;
      else if (dist(i,j) < (uint)cur_dist)
				cur_dist = dist(i,j);
      else
				dist(i,j) = cur_dist;
      cur_dist++;
    }
    for (int j = height-1, cur_dist = maxdist; j  >= 0; j--) {
      if (target(i,j) == 1)
				dist(i,j) = cur_dist = 0;
      else if (dist(i,j) < (uint)cur_dist)
				cur_dist = dist(i,j);
      else
				dist(i,j) = cur_dist;
      cur_dist++;
    }
  }
  return dist;
}

Sketch<uint> labelcc(const Sketch<bool>& sketch, int minarea) {
  Sketch<uchar> temp;
  uchar *pixels;
  if ( sizeof(bool) == sizeof(uchar) )
    pixels = reinterpret_cast<uchar*>(&((*sketch.pixels)[0]));
  else {
    temp.bind((Sketch<uchar>)sketch);
    pixels = &((*temp.pixels)[0]);
  }

  // convert pixel array to RLE
  int const maxRuns = (sketch.width * sketch.height) / 8;
  CMVision::run<uchar> *rle_buffer = new CMVision::run<uchar>[maxRuns];
  unsigned int const numRuns = CMVision::EncodeRuns(rle_buffer, pixels, sketch.width, sketch.height, maxRuns);

  // convert RLE to region list
  CMVision::ConnectComponents(rle_buffer,numRuns);
  int const maxRegions = (sketch.width * sketch.height) / 16;   // formula from RegionGenerator.h
  CMVision::region *regions = new CMVision::region[maxRegions];
  unsigned int numRegions = CMVision::ExtractRegions(regions, maxRegions, rle_buffer, numRuns);
  int const numColors = 2;  // only two colors in a Sketch<bool>: 0 and 1
  CMVision::color_class_state *ccs = new CMVision::color_class_state[numColors];
  unsigned int const maxArea = CMVision::SeparateRegions(ccs, numColors, regions, numRegions);
  CMVision::SortRegions(ccs, numColors, maxArea);
  CMVision::MergeRegions(ccs, numColors, rle_buffer);

  // extract regions from region list
  NEW_SKETCH_N(result, uint, visops::zeros(sketch));
  result->setColorMap(jetMapScaled);
  const CMVision::region* list_head = ccs[1].list;
  if ( list_head != NULL ) {
    for (int label=1; list_head!=NULL && list_head->area >= minarea;
	   list_head = list_head->next, label++) {
      // the first run might be array element 0, so use -1 as end of list test
      for (int runidx = list_head->run_start; runidx != -1;
	   runidx = rle_buffer[runidx].next ? rle_buffer[runidx].next : -1) {
	const CMVision::run<uchar> &this_run = rle_buffer[runidx];
	const int xstop = this_run.x + this_run.width;
	const int yi = this_run.y;
	for ( int xi = this_run.x; xi < xstop; xi++ )
	  result(xi,yi) = label * sketch(xi,yi); // the * undoes some of CMVision's noise removal
      }
    }
  }

  delete[] ccs;
  delete[] regions;
  delete[] rle_buffer;
  return result;
}

// written with guidance from this page: http://www.dai.ed.ac.uk/HIPR2/label.htm
Sketch<uint> oldlabelcc(const Sketch<bool>& source, Connectivity_t connectivity)
{
  bool conn8 = (connectivity == EightWayConnect);
  const int width = source.width;
  const int height = source.height;
  Sketch<uint> labels("oldlabelcc("+source->getName()+")",source);
  labels = 0;
  labels->setColorMap(jetMapScaled);
  
  // First scan: Give initial labels and sort connected label classes
  // into equivalence classes using UNION-FIND
  // Doing something similar to tree-based UNION-FIND, without the tree
  std::vector<int> eq_classes(500); // vector of equivalence classes for union-find
  eq_classes.clear();
  eq_classes.push_back(0); // added just so that indices match up with labels
  int highest_label = 0;
  int up_label = 0; // value above current pixel
  int left_label = 0; // value to left of current pixel
  int ul_label = 0; // value to upper-left of current pixel
  int ur_label = 0; // value to upper-right of current pixel
  for(int j = 0; j < height; j++) {
    for(int i = 0; i < width; i++) {
      if (source(i,j)) {
	up_label = (j == 0) ? 0 : labels(i,j-1);
	left_label = (i==0) ? 0 : labels(i-1,j); 
	ul_label = (i==0||j==0) ? 0 : labels(i-1,j-1);
	ur_label = (i==(width-1)||j==0) ? 0 : labels(i+1,j-1);
	if (up_label == 0 && left_label == 0
	    && (!conn8 || (ul_label == 0 && ur_label == 0))) {
	  labels(i,j) = ++highest_label;  // create new label
	  // push back a new root label
	  eq_classes.push_back(highest_label); // label value will be equal to index
	} else if (up_label && !left_label) {
	  labels(i,j) = up_label;
	} else if (conn8 && !up_label && ur_label) {
	  labels(i,j) = ur_label;
	} else if (left_label && !up_label) {
	  labels(i,j) = left_label;
	} else if (conn8 && !left_label && !up_label 
		   && ur_label && !ul_label) {
	  labels(i,j) = ur_label; 
	} else if (conn8 && !left_label && !up_label
		   && ul_label && !ur_label){
	  labels(i,j) = ul_label;	
	}
	
	if (up_label && left_label && (up_label != left_label)) {
	  // form union between two equivalence classes
	  // if upper-left, assume equivalence class already made
	  
	  int root = up_label;
	  while (eq_classes[root] != root) {
	    root = eq_classes[root]; // "FIND" of UNION-FIND
	  }
	  // should do path compression to make more efficient
	  int tmp_root = up_label, next_root;
	  while(eq_classes[tmp_root] != root) {
	    next_root = eq_classes[tmp_root];
	    eq_classes[tmp_root] = root; // compress
	    tmp_root = next_root;
	  }
	  
	  eq_classes[left_label] = root; // "UNION" of UNION-FIND
	  labels(i,j) = root; // not sure why putting this here works, but it does
	} else if (up_label && (up_label == left_label)) {
	  labels(i,j) = up_label;	
	} else if (conn8 && ur_label && left_label 
		   && (ur_label != left_label)) {
	  // form union between two equivalence classes
	  int root = ur_label;
	  while (eq_classes[root] != root) {
	    root = eq_classes[root]; // "FIND" of UNION-FIND
	  }
	  // should do path compression to make more efficient
	  int tmp_root = ur_label, next_root;
	  while(eq_classes[tmp_root] != root) {
	    next_root = eq_classes[tmp_root];
	    eq_classes[tmp_root] = root; // compress
	    tmp_root = next_root;
	  }
	  
	  eq_classes[left_label] = root; // "UNION" of UNION-FIND
	  labels(i,j) = root; // not sure why putting this here works, but it does
	}
      }
    }
  }
  
  // Second scan: 
  int cur_label;
  for(int j = 0; j < height; j++) {
    for(int i = 0; i < width; i++) {
      cur_label = labels(i,j);
      if (cur_label != 0) {
	while(eq_classes[cur_label] != cur_label) {
	  cur_label = eq_classes[cur_label];	
	}
	labels(i,j) = cur_label;
      }
      
    }
  }
  
  return labels;
}

Sketch<uint> areacc(const Sketch<bool>& source, Connectivity_t connectivity) {
  NEW_SKETCH_N(labels, uint, visops::oldlabelcc(source,connectivity));
  return visops::areacc(labels);
}

Sketch<uint> areacc(const Sketch<uint>& labels) {
  std::vector<int> areas(1+labels->max(), 0);
  for (uint i = 0; i<labels->getNumPixels(); i++)
    ++areas[labels[i]];
  areas[0] = 0;
  Sketch<uint> result("areacc("+labels->getName()+")",labels);
  for (uint i = 0; i<labels->getNumPixels(); i++)
    result[i] = areas[labels[i]];
  return result;
}

Sketch<bool> minArea(const Sketch<bool>& sketch, int minval) {
  NEW_SKETCH_N(labels, uint, visops::labelcc(sketch));
  NEW_SKETCH_N(areas, uint, visops::areacc(labels));
  NEW_SKETCH_N(result, bool, areas >= minval);
  return result;
}

Sketch<uchar> neighborSum(const Sketch<bool>& im, Connectivity_t connectivity) 
{
  // using index redirection method
  SketchSpace &space = im->getSpace();

  space.requireIdx4way();
  if (connectivity == EightWayConnect)
    space.requireIdx8way();

  Sketch<uchar> result("neighborSum("+im->getName()+")", im);
  result->setColorMap(jetMapScaled);
  for ( unsigned int i = 0; i < im->getNumPixels(); i++ ) {
    uchar cnt = (uchar)im[(*space.idxN)[i]] +
      (uchar)im[(*space.idxS)[i]] +
      (uchar)im[(*space.idxE)[i]] +
      (uchar)im[(*space.idxW)[i]];
    if (connectivity == EightWayConnect)
      cnt += (uchar)im[(*space.idxNE)[i]] +
	(uchar)im[(*space.idxNW)[i]] +
	(uchar)im[(*space.idxSE)[i]] +
	(uchar)im[(*space.idxSW)[i]];
    result[i] = cnt;
  }
  return result;
}

Sketch<bool> fillin(const Sketch<bool>& im, int iter, 
			    uchar min_thresh, uchar max_thresh, bool remove_only)
{
  Sketch<bool> result(im);
  if ( remove_only )
    result.bind(visops::copy(im));
  Sketch<uchar> neighborCount(im);
  if (remove_only) {
    neighborCount.bind(neighborSum(result,EightWayConnect));
    result &= (neighborCount <= max_thresh);
    for (int i = 0; i < iter; i++)
      result &= (neighborCount >= min_thresh);
  }
  else {
    for (int i = 0; i < iter; i++) {
      neighborCount.bind(neighborSum(result,EightWayConnect));
      result.bind((neighborCount >= min_thresh) & (neighborCount <= max_thresh));
    }
  }
  result->setName("fillin("+im->getName()+")");
  result->setColor(im->getColor());
  return result;
}

Sketch<bool> edge(const Sketch<bool> &im) {
  im->getSpace().requireIdx4way();
  SketchSpace &space = im->getSpace();
  return (im != im[*space.idxS] | im != im[*space.idxE]);
}


Sketch<bool> horsym(const Sketch<bool> &im, int minskip, int maxskip)
{
  NEW_SKETCH_N(result, bool, visops::zeros(im));
  result->setName("horsym("+im->getName()+")");
  const int width = im->getWidth();
  const int height = im->getHeight();

  for (int j = 0; j < height; j++) {
    for (int i = 0; i < width; i++) {
      while (i < width && !im(i,j)) i++; // skip over empty pixels
      for (int k = i+1; k <= width; k++) {
	  if ( k==width || !im(k,j)) {
	    if ( (k-i) >= minskip && (k-i) <= maxskip ) {
	      const int u = (i+k)/2;
	      result(u,j) = true;
	    }
	    i=k+1;
	    break;
	  }
      }
    }
  }
  return result;
}

Sketch<bool> versym(const Sketch<bool> &im, int minskip, int maxskip)
{
  NEW_SKETCH_N(result, bool, visops::zeros(im));
  result->setName("horsym("+im->getName()+")");
  const int width = im->getWidth();
  const int height = im->getHeight();

  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      while (j < height && !im(i,j)) j++; // skip over empty pixels
      for (int k = j+1; k <= height; k++) {
	  if ( k==height || !im(i,k)) {
	    if ( (k-j) >= minskip && (k-j) <= maxskip ) {
	      const int u = (j+k)/2;
	      result(i,u) = true;
	    }
	    j=k+1;
	    break;
	  }
      }
    }
  }
  return result;
}


/*
Sketch<bool> versym(const Sketch<bool>& im, int minskip, int maxskip)
{
  NEW_SKETCH_N(result, bool, visops::zeros(im));
  result->setName("versym("+im->getName()+")");
  int height = im->getWidth();
  int width = im->getHeight();
  
  for (int j = 0; j < height; j++) {
    for (int i = 0; i < width; i++) {
      if (im(j,i)) {
	while (i < width-1 && im(j,i+1)) {
	  i++; // skip over contiguous pixels
	}
	for (int k = i+1; k < width; k++) {
	  if (k-i > maxskip)
	    break;
	  else if (im(j,k)) {
	    if ((k-i) < minskip)
	      break;
	    int u = (i+k)/2;
	    if (!im(j,u))
	      result(j,u) = true; // was result(j,u) = k-i when this returned a Sketch<int>
	    break; // only works well for non-'donut' ellipses
	  }
	}
      }
    }
  }
  return result;
}
*/

Sketch<bool> skel(const Sketch<bool>& im) {
  NEW_SKETCH_N(result, bool, horsym(im) | versym(im));
  result->setName("skel("+im->getName()+")");
  return result;
}

Sketch<bool> seedfill(const Sketch<bool>& borders, size_t index) {
  // use four-way connect so thin diagonal line can function as a boundary
  NEW_SKETCH_N(regions, uint, oldlabelcc(! borders, visops::FourWayConnect));
  NEW_SKETCH_N(result, bool, regions == regions->at(index));  // use at() to do bounds checking
  return result;
}

// helper function called only inside visops::fillExterior
void fillExteriorHelper(const Sketch<uint> &regions, Sketch<bool> &result, std::vector<bool> &processed, 
			const int x, const int y) {
  const uint c = regions(x,y);
  if ( c > 0 && !processed[c] ) {
    result |= (regions == c);
    processed[c] = true;
  }
}

Sketch<bool> fillExterior(const Sketch<bool>& borders) {
  // use four-way connect so thin diagonal line can function as a boundary
  NEW_SKETCH_N(regions, uint, oldlabelcc(! borders, visops::FourWayConnect));
  const uint numreg = regions->max();
  std::vector<bool> processed(numreg+1,false);
  NEW_SKETCH_N(result, bool, visops::zeros(borders));
  for ( int x = 0; x < result.width; x++ ) {
    fillExteriorHelper(regions,result,processed,x,0);
    fillExteriorHelper(regions,result,processed,x,result.height-1);
  }
  for ( int y = 0; y < result.height; y++ ) {
    fillExteriorHelper(regions,result,processed,0,y);
    fillExteriorHelper(regions,result,processed,result.width-1,y);
  }
  return result;
}

Sketch<bool> fillInterior(const Sketch<bool>& borders) {
  return ! (borders | fillExterior(borders));
}

Sketch<bool> leftHalfPlane(const Shape<LineData> &ln) {
  SketchSpace &SkS = ln->getSpace().getDualSpace();
  //! @todo **** THIS visops::leftHalfPlane CODE NEEDS TO CHECK THE SketchSpace ReferenceFrameType **** BECAUSE "left" MEANS DIFFERENT THINGS IN DIFFERENT FRAMES 
  int const x1 = (int)ln->end1Pt().coordX();
  int const y1 = (int)ln->end1Pt().coordY();
  int const x2 = (int)ln->end2Pt().coordX();
  int const y2 = (int)ln->end2Pt().coordY();
  float const m = (x1 == x2) ? BIG_SLOPE : (y2-y1) / (x2-x1);
  int const b = (int) (y1 - x1*m);
  int seed;
  if ( x1 == x2 )
    seed = ( x1 <= 0 ) ? -1 : 0;
  else if ( ln->getOrientation() > M_PI/2 )
    seed =  ( b <= 0) ? -1 : 0;
  else {
		int const lim = SkS.getHeight() - 1;
    seed =  ( b < lim ) ? (int)(*SkS.idx)(0,lim) : -1;
	}
  if ( seed == -1 ) {
    NEW_SKETCH_N(result, bool, visops::zeros(SkS));
    result->inheritFrom(ln);
    return result;
  } else {
    NEW_SHAPE_N(line_copy, LineData, ln->copy());
    line_copy->setInfinite();
    NEW_SKETCH_N(bounds, bool, line_copy->getRendering());
    bounds->inheritFrom(ln);
    return visops::seedfill(bounds,seed);
  }
}

Sketch<bool> rightHalfPlane(const Shape<LineData> &ln) {
    NEW_SHAPE_N(line_copy, LineData, ln->copy());
    line_copy->setInfinite();
    NEW_SKETCH_N(bounds, bool, line_copy->getRendering());
    bounds->inheritFrom(ln);
    return ! (visops::leftHalfPlane(ln) | bounds);
}

Sketch<bool> topHalfPlane(const Shape<LineData> &ln) {
  SketchSpace &SkS = ln->getSpace().getDualSpace();
  //! @todo **** visops::topHalfPlane needs to check the SketchSpace ReferenceFrameType because "left" means different things in different reference frames 
  int const x1 = (int)ln->end1Pt().coordX();
  int const y1 = (int)ln->end1Pt().coordY();
  int const x2 = (int)ln->end2Pt().coordX();
  int const y2 = (int)ln->end2Pt().coordY();
  float const m = (x1 == x2) ? BIG_SLOPE : (y2-y1) / (x2-x1);
  int const b = (int) (y1 - x1*m);
  int seed;
  if ( x1 == x2 )
    seed = ( y1 <= 0 ) ? -1 : 0;
  else if ( ln->getOrientation() > M_PI/2 )
    seed =  ( b <= 0) ? -1 : 0;
  else {
		int const lim = SkS.getWidth() - 1;
    seed =  ( int(m*lim+b) > 0 ) ? (int)(*SkS.idx)(lim,0) : -1;
	}
  if ( seed == -1 ) {
    NEW_SKETCH_N(result, bool, visops::zeros(SkS));
    result->inheritFrom(ln);
    return result;
  } else {
    NEW_SHAPE_N(line_copy, LineData, ln->copy());
    line_copy->setInfinite();
    NEW_SKETCH_N(bounds, bool, line_copy->getRendering());
    bounds->inheritFrom(ln);
    return visops::seedfill(bounds,seed);
  }
}

Sketch<bool> bottomHalfPlane(const Shape<LineData> &ln) {
    NEW_SHAPE_N(line_copy, LineData, ln->copy());
    line_copy->setInfinite();
    NEW_SKETCH_N(bounds, bool, line_copy->getRendering());
    bounds->inheritFrom(ln);
    return ! (visops::topHalfPlane(ln) | bounds);
}

Sketch<bool> non_bounds(const Sketch<bool>& im, int offset) {
  const int width = im->getWidth();
  const int height = im->getHeight();
  NEW_SKETCH_N(nbresult,bool,visops::copy(im));
  nbresult->setName("non_bounds("+im->getName()+")");

  for (int i = 0; i < width; i++) {
    for (int j = 0; j < offset; j++) {
      nbresult(i,j) = false;
      nbresult(i,height-j-1) = false;
    }
  }
  for (int i = 0; i < offset; i++) {
    for (int j = offset; j < height-offset; j++) {
      nbresult(i,j) = false;
      nbresult(width-i-1,j) = false;
    }
  }
  return nbresult;
}


Sketch<uchar> susan_edges(const Sketch<uchar>& im, int brightness)
{
  const int width = im->getWidth();
  const int height = im->getHeight();
  unsigned char *bp;
  Sketch<uchar> edges(visops::copy(im));

  int *r = (int *)malloc(width*height*sizeof(int));

  unsigned char *mid = (unsigned char *)malloc(width*height);
  memset (mid,100,width * height); /* note not set to zero */

  setup_brightness_lut(&bp,brightness,6);

  // susan_principle(im->getRawPixels(),edges->getRawPixels(), &bp, 2650, width, height);

  susan_edges_internal(edges->getRawPixels(), r, mid, bp, 2650, width, height);

  susan_thin(r, mid, width, height);

  edge_draw(edges->getRawPixels(),mid,width,height,0);

   free(r);
   free(mid);
   free(bp-258);

  return edges;
}


// Default brightness was 20 for original algorithm
Sketch<bool> susan_edge_points(const Sketch<uchar>& im, int brightness)
{
  const int width = im->getWidth();
  const int height = im->getHeight();
  unsigned char *bp;
  Sketch<uchar> orig(im);
  Sketch<uchar> edges(visops::zeros(im));
  int *r = (int *)malloc(width*height*sizeof(int));
  unsigned char *mid = (unsigned char *)malloc(width*height);
  memset(mid,100,width * height); /* note not set to zero */
  setup_brightness_lut(&bp,brightness,6);
  susan_edges_internal(orig->getRawPixels(), r, mid, bp, 2650, width, height);
  susan_thin(r, mid, width, height);
  edge_draw(edges->getRawPixels(),mid,width,height,1);
  free(r);
  free(mid);
  Sketch<bool> result(edges);
  return result;
}

Sketch<uint> convolve(const Sketch<uchar> &sketch, Sketch<uchar> &kernel, 
		       int istart, int jstart, int width, int height) {
  Sketch<uint> result("convolve("+sketch->getName()+")",sketch);
  result->setColorMap(jetMapScaled);
  int const di = - (int)(width/2);
  int const dj = - (int)(height/2);
  for (int si=0; si<sketch.width; si++)
    for (int sj=0; sj<sketch.height; sj++) {
      int sum = 0;
      for (int ki=0; ki<width; ki++)
	for (int kj=0; kj<height; kj++)
	  if ( si+di+ki >= 0 && si+di+ki < sketch.width &&
	       sj+dj+kj >= 0 && sj+dj+kj < sketch.height )
	    sum += (uint)sketch(si+di+ki,sj+dj+kj) * (uint)kernel(istart+ki,jstart+kj);
      result(si,sj) = sum/(width*height);
    }
  return result;      
}

Sketch<uint> templateMatch(const Sketch<uchar> &sketch, Sketch<uchar> &kernel, 
		       int istart, int jstart, int width, int height) {
  Sketch<uint> result("convolve0("+sketch->getName()+")",sketch);
  result->setColorMap(jetMapScaled);
  int const npix = width * height;
  int const di = - (int)(width/2);
  int const dj = - (int)(height/2);
  for (int si=0; si<sketch.width; si++)
    for (int sj=0; sj<sketch.height; sj++) {
      int sum = 0;
      for (int ki=0; ki<width; ki++)
	for (int kj=0; kj<height; kj++) {
	  int k_pix = kernel(istart+ki,jstart+kj);
	  if ( si+di+ki >= 0 && si+di+ki < sketch.width &&
	       sj+dj+kj >= 0 && sj+dj+kj < sketch.height ) {
	    int s_pix = sketch(si+di+ki,sj+dj+kj);
	    sum +=  (s_pix - k_pix) * (s_pix - k_pix);
	  }
	  else
	    sum += k_pix * k_pix;
	}
      result(si,sj) =  65535 - uint(sqrt(sum/float(npix)));
    }
  result = result - result->min();
  return result;
}


} // namespace
