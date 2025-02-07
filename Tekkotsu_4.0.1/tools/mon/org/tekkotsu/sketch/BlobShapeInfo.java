package org.tekkotsu.sketch;

import java.awt.Color;
import java.awt.Font;
import java.awt.Graphics2D;
import java.awt.Graphics;
import javax.swing.Icon;
import javax.swing.ImageIcon;
import java.awt.Rectangle;
import java.awt.geom.*;
import java.awt.BasicStroke;

// stores info for a BlobShape
public class BlobShapeInfo extends ShapeInfo {
    static Icon icon = new ImageIcon(icon_path+"blob.png");
    static Icon inv_icon = new ImageIcon(icon_path+"blobinv.png");

    float area;
    int orient;
    float topLeft_x, topLeft_y, topLeft_z,
	  topRight_x, topRight_y, topRight_z,
	  bottomLeft_x, bottomLeft_y, bottomLeft_z,
	  bottomRight_x, bottomRight_y, bottomRight_z;


    public BlobShapeInfo(int _id, int _parentId, String _name, Color _color,
			 float _centroidx, float _centroidy, float _centroidz,
			 float _area, int _orient,
			 float _topLeft_x, float _topLeft_y, float _topLeft_z,
			 float _topRight_x, float _topRight_y, float _topRight_z,
			 float _bottomLeft_x, float _bottomLeft_y, float _bottomLeft_z,
			 float _bottomRight_x, float _bottomRight_y, float _bottomRight_z) {
	super(_id, _parentId, _name, _color, _centroidx, _centroidy, _centroidz);
	area = _area;
	orient = _orient;
	topLeft_x = _topLeft_x;
	topLeft_y = _topLeft_y;
	topLeft_z = _topLeft_z;
	topRight_x = _topRight_x;
	topRight_y = _topRight_y;
	topRight_z = _topRight_z;
	bottomLeft_x = _bottomLeft_x;
	bottomLeft_y = _bottomLeft_y;
	bottomLeft_z = _bottomLeft_z;
	bottomRight_x = _bottomRight_x;
	bottomRight_y = _bottomRight_y;
	bottomRight_z = _bottomRight_z;
    }

	// should adjust these to take into account the crossbar height for pillar/poster blobs in worldspace
	public float getLeft() { return java.lang.Math.min(topLeft_x,bottomLeft_x); }
	public float getRight() { return java.lang.Math.max(topRight_x,bottomRight_x); }
	public float getTop() { return  java.lang.Math.min(topLeft_y,topRight_y); }
	public float getBottom() { return java.lang.Math.max(bottomLeft_y,bottomRight_y); }


	public String toString() {
	    String _orient = (orient==0) ? "ground" : (orient==1) ? "pillar" : "poster";
	    String _center = "center=(" + centroidx + " " + centroidy + " " + centroidz + ")";
	    return super.toString() + " " + _orient + " area=" + area + " " + _center;
	}

	public Icon getIcon() { 
	    if (inverted)
		return inv_icon;
	    else
		return icon; 
	}

    public void renderTo(Graphics2D graphics, float scaling) {
	if ( orient == 0 ) {
	    graphics.setStroke(new BasicStroke(0.5f));
	    graphics.drawLine((int)topLeft_x,(int)topLeft_y,(int)topRight_x,(int)topRight_y);
	    graphics.drawLine((int)bottomLeft_x,(int)bottomLeft_y,(int)bottomRight_x,(int)bottomRight_y);
	    graphics.drawLine((int)topLeft_x,(int)topLeft_y,(int)bottomLeft_x,(int)bottomLeft_y);
	    graphics.drawLine((int)topRight_x,(int)topRight_y,(int)bottomRight_x,(int)bottomRight_y);
	}
	else {
	    graphics.setStroke(new BasicStroke(3.0f));
	    graphics.drawLine((int)bottomLeft_x,(int)bottomLeft_y,(int)bottomRight_x,(int)bottomRight_y);
	    int w = java.lang.Math.max(12,(int)java.lang.Math.abs(bottomRight_y - bottomLeft_y));
	    int midx = (int)((bottomLeft_x + bottomRight_x) / 2);
	    int midy = (int)((bottomLeft_y + bottomRight_y) / 2);
	    graphics.setStroke(new BasicStroke(3.0f));
	    graphics.drawLine(midx-w/4,midy,midx+w/4,midy);
	};
    }

}
