#include "Graphics.h"
#include "FilterBankGenerator.h"

using namespace std;

//! empty definition of macro for checking for any exceeding of image's memory boundaries
//#define CHKBOUNDS(p) {}

// ! check memory boundaries of image, first that we are after the start of the image, second that we are before the end of the image, and third that we are within a valid row (in case of interleaved rows), fourth that we are within a valid column (interleaved pixels)
#define CHKBOUNDS(p,ERR_F) { \
	unsigned int rowoffset=(p-img)-(p-img)/yInc*yInc; \
	if(p<img || p>=img+h*yInc || rowoffset>=w*xInc || rowoffset/xInc*xInc!=rowoffset) { \
		cout << "Graphics Bad draw! line:" << __LINE__ << " frame=" << (gen!=NULL?gen->getFrameNumber():0) << ' '  /*<< (void*)p << '-' << (void*)img << '='*/ << (int)(p-img) << " w=" << w << " xInc=" << xInc << " h=" << h << " yInc=" << yInc << endl; \
		ERR_F; \
	} \
}


Graphics::Graphics(FilterBankGenerator& fbg, unsigned int layer, unsigned int channel)
	: gen(&fbg), genLayer(layer), genChan(channel), img(NULL), w(0), h(0), xInc(0), yInc(0), c(0)
{
	updateFBG();
}

Graphics::Graphics(unsigned char * base, unsigned int width, unsigned int height, unsigned int interval, unsigned int stride)
	: gen(NULL), genLayer(), genChan(), img(base), w(width), h(height), xInc(interval), yInc(stride), c(0)
{}

void Graphics::updateFBG() {
	if(gen==NULL)
		return;
	img=gen->getImage(genLayer,genChan);
	w=gen->getWidth(genLayer);
	h=gen->getHeight(genLayer);
	xInc=gen->getIncrement(genLayer);
	yInc=gen->getStride(genLayer);
}

void Graphics::drawRect(int x, int y, int width, int height) {
	if(img==NULL)
		return;
	if(width<0) {
		x+=width;
		width=-width;
	}
	if(height<0) {
		y+=height;
		height=-height;
	}
	if(x>=(int)w || y>=(int)h || x+width<0 || y+height<0) //completely out of bounds
		return;
	if(x<0 && y<0 && x+width>=(int)w && y+height>=(int)h) //out of bounds (circumscribed)
		return;
	unsigned int left=x>0?x:0;
	unsigned int top=y>0?y:0;
	unsigned int right=( (x+width>=(int)w) ? w : x+width); //not inclusive
	unsigned int bot=( (y+height>=(int)h) ? h : y+height); //not inclusive
	//left vertical
	if(x>=0) {
		unsigned char * p=img+top*yInc+left*xInc;
		unsigned char * end=img+bot*yInc+left*xInc;
		while(p!=end) {
			CHKBOUNDS(p,return);
			*p=c;
			p+=yInc;
		}
	}		
	//top horizontal
	if(y>=0) {
		unsigned char * p=img+top*yInc+left*xInc;
		unsigned char * end=img+top*yInc+right*xInc;
		while(p!=end) {
			CHKBOUNDS(p,return);
			*p=c;
			p+=xInc;
		}
	}
	//right vertical
	if(right<w && left!=right) {
		unsigned char * p=img+top*yInc+right*xInc;
		unsigned char * end=img+bot*yInc+right*xInc;
		while(p!=end) {
			CHKBOUNDS(p,return);
			*p=c;
			p+=yInc;
		}
	}		
	//bottom horizontal
	if(bot<h && top!=bot) {
		unsigned char * p=img+bot*yInc+left*xInc;
		unsigned char * end=img+bot*yInc+right*xInc;
		while(p!=end) {
			CHKBOUNDS(p,return);
			*p=c;
			p+=xInc;
		}
	}
	//bottom right corner
	if(right<w && bot<h) {
		unsigned char * p=img+bot*yInc+right*xInc;
		CHKBOUNDS(p,return);
		*p=c;
		return;
	}
}
void Graphics::drawRect(float x, float y, float width, float height) {
	unsigned int left,top,right,bot;
	getPixelCoordinates(left,top,x,y);
	getPixelCoordinates(right,bot,x+width,y+height);
	drawRect((int)left,(int)top,(int)(right-left),(int)(bot-top));
}

/*! @todo I think this could be a little faster by writing two cases -- one that
 *  handles mostly-vertical lines, and one that handles mostly-horizontal ones.
 *  Then the primary loop could be over integer coordinates along that axis, and
 *  only the position along the other axis would have to be calculated as
 *  floating point */
void Graphics::drawLine(int ix1, int iy1, int ix2, int iy2) {
	// the "right" way, allows full range of unsigned int for width
	//if(ix1<0 && ix2<0 || iy1<0 && iy2<0 || ix1>0 && ix2>0 && (unsigned int)ix1>=w && (unsigned int)ix2>=w || iy1>0 && iy2>0 && (unsigned int)iy1>=h && (unsigned int)iy2>=h)
	//  return; //completely outside visible region

	// the "realistic way" saves some CPU
	if(ix1<0 && ix2<0 || iy1<0 && iy2<0 || ix1>=(int)w && ix2>=(int)w || iy1>=(int)h && iy2>=(int)h)
		return; //completely outside visible region

	float x1=ix1, y1=iy1, x2=ix2, y2=iy2;
	float width=x2-x1;
	float height=y2-y1;
	bool clipped=false;
	if(width!=0) {
		float slope=height/width;
		if(x1<x2) {
			if(x1<0) {
				y1-=x1*slope;
				x1=0;
				clipped=true;
			}
			if(x2>=w) {
				y2-=(x2-(w-1))*slope;
				x2=w-1;
				clipped=true;
			}
		} else {
			if(x2<0) {
				y2-=x2*slope;
				x2=0;
				clipped=true;
			}
			if(x1>=w) {
				y1-=(x1-(w-1))*slope;
				x1=w-1;
				clipped=true;
			}
		}
	}
	if(clipped) {
		if(x1<0 && x2<0 || y1<0 && y2<0 || x1>=w && x2>=w || y1>=h && y2>=h)
			return; //completely outside visible region
		clipped=false;
	}
	if(height!=0) {
		float invslope=width/height;
		if(y1<y2) {
			if(y1<0) {
				x1-=y1*invslope;
				y1=0;
				clipped=true;
			}
			if(y2>=h) {
				x2-=(y2-(h-1))*invslope;
				y2=h-1;
				clipped=true;
			}
		} else {
			if(y2<0) {
				x2-=y2*invslope;
				y2=0;
				clipped=true;
			}
			if(y1>=h) {
				x1-=(y1-(h-1))*invslope;
				y1=h-1;
				clipped=true;
			}
		}
	}
	if(clipped) {
		if(x1<0 && x2<0 || y1<0 && y2<0 || x1>=w && x2>=w || y1>=h && y2>=h)
			return; //completely outside visible region
		clipped=false;
	}
	width=x2-x1;
	height=y2-y1;
	int aw=abs((int)width);
	int ah=abs((int)height);
	int d=aw>ah?aw:ah;
	float dx=width/d;
	float dy=height/d;
	for(float x=x1,y=y1;d>0;d--,x+=dx,y+=dy) {
		unsigned char * p=(img+((int)y)*yInc+((int)x)*xInc);
		CHKBOUNDS(p,continue);
		*p=c;
	}
	unsigned char * p=(img+((int)y2)*yInc+((int)x2)*xInc);
	CHKBOUNDS(p,return);
	*p=c;
}
void Graphics::drawLine(float x1, float y1, float x2, float y2) {
	unsigned int px1,py1,px2,py2;
	getPixelCoordinates(px1,py1,x1,y1);
	getPixelCoordinates(px2,py2,x2,y2);
	drawLine((int)px1,(int)py1,(int)px2,(int)py2);
}

void Graphics::getPixelCoordinates(unsigned int& px, unsigned int& py, float x, float y) const {
	if(gen!=NULL) {
		gen->getPixelCoordinates(px,py,x,y,genLayer);
	} else {
		//note width sets the scale for both, so coordinate system is square... is good? I'm up for debate.
		px=(unsigned int)((w-1)*(x+1)/2+.5f); //+.5 to round to nearest
		float aspect=h/(float)w;
		py=(unsigned int)((h-1)*(y+aspect)/(aspect*2)+.5f); //+.5 to round to nearest
	}
}
	
void Graphics::getRealCoordinates(float& x, float& y, unsigned int px, unsigned int py) const {
	if(gen!=NULL) {
		gen->getRealCoordinates(x,y,px,py,genLayer);
	} else {
		//note width sets the scale for both, so coordinate system is square... is good? I'm up for debate.
		x=px/(float)(w-1)*2-1;
		float aspect=h/(float)w;
		y=py/(float)(h-1)*aspect*2-aspect;
	}
}

/*! @file
 * @brief 
 * @author ejt (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.5 $
 * $State: Exp $
 * $Date: 2006/07/03 01:54:58 $
 */
