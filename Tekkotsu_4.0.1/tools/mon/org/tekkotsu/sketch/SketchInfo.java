package org.tekkotsu.sketch;

import java.awt.Color;
import java.awt.Graphics2D;
import java.awt.image.BufferedImage;
import javax.swing.Icon;
import javax.swing.ImageIcon;

import java.util.Hashtable;
import java.awt.image.IndexColorModel;
import java.awt.Graphics;

// stores info for a Sketch, to use as UserObject for DefaultMutableTreeNode
public class SketchInfo extends SketchOrShapeInfo {
    static Icon bool_icon = new ImageIcon(icon_path+"sketchbool.png");
    static Icon inverted_bool_icon = new ImageIcon(icon_path+"sketchboolinv.png");;

    static Icon uchar_icon = new ImageIcon(icon_path+"sketchuchar.png");
    static Icon inverted_uchar_icon = new ImageIcon(icon_path+"sketchucharinv.png");


    static Icon int_icon = new ImageIcon(icon_path+"sketchint.png");
    static Icon inverted_int_icon = new ImageIcon(icon_path+"sketchintinv.png");


    public static final int SKETCH_BOOL_TYPE  = 1;
    public static final int SKETCH_UCHAR_TYPE  = 2;
    public static final int SKETCH_USINT_TYPE   = 3;
    public static final int SKETCH_FLOAT_TYPE = 4;
    

    public static final int COLORMAP_SEG_TYPE = 0;
    public static final int COLORMAP_GRAY_TYPE = 1;
    public static final int COLORMAP_JET_TYPE = 2;
    public static final int COLORMAP_JET_SCALED_TYPE = 3;

    boolean imageLoaded;
    BufferedImage img;
    int sketchType;
    int grayColor = Color.GRAY.getRGB();
    int colormap;
    int width;
    int height;


    byte jetred[]; 
    byte jetgreen[];
    byte jetblue[];

    float grayMax, grayMin;


    public SketchInfo(int _id, int _parentId, String _name, Color _color, 
		      int _colormap, int _sketchType, int _width, int _height) {
	super(_id, _parentId, _name, _color);
	colormap = _colormap;
	width = _width;
	height = _height;
	imageLoaded = false;
	img = null;
	sketchType = _sketchType;

	if (sketchType == SKETCH_FLOAT_TYPE)
	    System.out.println("Warning, got a SketchFloat, treating it like a SketchUsint");
	makeJetMap();
    }
    
    public Icon getIcon() { 
	if (inverted) {
	    if (sketchType == SKETCH_BOOL_TYPE) return inverted_bool_icon;
	    else if (sketchType == SKETCH_UCHAR_TYPE)  return inverted_uchar_icon;
	    else return inverted_int_icon;
	}
	else {
	    if (sketchType == SKETCH_BOOL_TYPE) return bool_icon;
	    else if (sketchType == SKETCH_UCHAR_TYPE)  return uchar_icon;
	    else return int_icon;
	}
    }

    public boolean isImageLoaded() { return imageLoaded; }

    public BufferedImage getImage() { return img; }

    public int getSketchType() { return sketchType; }

    public void unloadImage() {
	imageLoaded = false;
	img = null;
    }

    // Deep image copying. Called when the image is first loaded to cache it
    public void copyImage(BufferedImage _img)
    {
	if (_img == null) {
	    System.err.println("Tried to copy a null image");
	    return;
	}

	if (colormap == COLORMAP_SEG_TYPE || sketchType == SKETCH_BOOL_TYPE )
	    {
		System.out.println("Type = "+_img.getType()+" RGB_Type="+BufferedImage.TYPE_INT_RGB);
		if ( _img.getColorModel() instanceof IndexColorModel )
		    img = new BufferedImage(_img.getWidth(), _img.getHeight(),
					    _img.getType(),
					    ((IndexColorModel) _img.getColorModel()));
		else {
		    // If the sketch is a usint but it has COLORMAP_SEG_TYPE, it has the wrong ColorModel.
		    // The user probably converted a bool or uchar sketch to usint.
		    // So use the jet colormap.
		    // *** THIS IS SORT OF A KLUDGE ***
		    // Note that usint can have values > 256 so we might still get an array bounds error
		    // if the sketch contains large values.
		    IndexColorModel cmodel = new IndexColorModel(8,256,jetred,jetgreen,jetblue);
		    img = new BufferedImage(_img.getWidth(), _img.getHeight(),
					    BufferedImage.TYPE_BYTE_INDEXED,
					    cmodel);
		}
		img.getRaster().setRect(_img.getRaster());

	    }
	else
	    {
		img = new BufferedImage(_img.getWidth(), _img.getHeight(), BufferedImage.TYPE_USHORT_GRAY);
		grayMax = -Float.MAX_VALUE;
		grayMin = Float.MAX_VALUE;

		for (int i=0; i<_img.getWidth()*_img.getHeight(); i++)
		    {
			int x = i % _img.getWidth();
			int y = i / _img.getWidth();
			int cur = _img.getRaster().getSample(x,y,0);
			img.getRaster().setSample(x,y,0,cur);
			if (cur > grayMax) 
			    grayMax = cur;
			else if (cur < grayMin)
			    grayMin = cur;
		    }
		System.out.println("Grayscale image: "+img.getType()+" "+colormap+" ("+
				   grayMin+","+grayMax+")");
		if (grayMax == grayMin) ++grayMax;
	    }
	imageLoaded = true;
    }


    public void renderToArrays(int r[], int g[], int b[], int counts[])
    {
	int curR = getColor().getRed();
	int curG = getColor().getGreen();
	int curB = getColor().getBlue();
	if (img!= null) {
	    for (int y = 0; y < img.getHeight(); y++) {
		for (int x = 0; x < img.getWidth(); x++) {
		    int pos = y*img.getWidth()+x;
		    int cur = img.getRGB(x,y);
		    if (colormap == COLORMAP_SEG_TYPE || sketchType == SKETCH_BOOL_TYPE) {
			if (cur != grayColor) {
			    if (sketchType != SKETCH_BOOL_TYPE) {
				Color c = new Color(cur);
				curR = c.getRed();
				curG = c.getGreen();
				curB = c.getBlue();
				if (inverted) {
				    curR = 255 - curR;
				    curG = 255 - curG;
				    curB = 255 - curB;
				}
			    }
			    
			    r[pos] += curR;
			    g[pos] += curG;
			    b[pos] += curB;
			    counts[pos]++;
			}
		    }
		    else if (colormap == COLORMAP_GRAY_TYPE) {
			cur = img.getRaster().getSample(x,y,0);
			if (cur>255) cur = 255;
			else if (cur<0) cur = 0;
			if (inverted)
			    cur = 255 - cur;
			r[pos]+=cur;
			g[pos]+=cur;
			b[pos]+=cur;
			counts[pos]++;
		    }
		    else if (colormap == COLORMAP_JET_TYPE || 
			     colormap == COLORMAP_JET_SCALED_TYPE) {
			cur = img.getRaster().getSample(x,y,0);
			if (colormap == COLORMAP_JET_SCALED_TYPE) {
			    cur = (int)Math.ceil((cur - grayMin)*255/(grayMax - grayMin));
			}
			else {
			    if (cur > 255) cur = 255;
			    else if (cur < 0) cur = 0;
			}
			if (inverted)
			    cur = 255 - cur;
			r[pos] += 0x00FF & (int)jetred[cur];
			g[pos] += 0x00FF & (int)jetgreen[cur];
			b[pos] += 0x00FF & (int)jetblue[cur];
			counts[pos]++;
		    }
		}
	    }
	}
    }

    public void makeJetMap()
    {

	jetred = new byte[256];
	jetgreen = new byte[256];
	jetblue = new byte[256];

	for (int i=0;i<256; i++) {
	    if (i < 32)
		jetgreen[i] = 0;
	    else if (i < 96)
		jetgreen[i] = (byte) ((i-32)*4);
	    else if (i <= 160)
		jetgreen[i] = (byte)255;
	    else if (i < 224)
		jetgreen[i] = (byte) ((224-i)*4);
	    else
		jetgreen[i] = 0;
	}

	for (int i=0; i<256; i++) {
	    if (i < 192)
		jetblue[i] = jetgreen[i+64];
	    else
		jetblue[i] = 0;
		
	    if (i >= 64)
		jetred[i] = jetgreen[i-64];
	    else
		jetred[i] = 0;
	}

	// manually set color 0 to background gray
	jetred[0] = (byte)128;
	jetgreen[0] = (byte)128;
	jetblue[0] = (byte)128;
    }
    
}




