package org.tekkotsu.sketch;

import java.awt.Graphics2D;
import java.awt.Graphics;
import java.awt.Color;
import java.awt.Font;
import javax.swing.Icon;
import javax.swing.ImageIcon;
import java.awt.Rectangle;
import java.awt.geom.*;
import java.awt.BasicStroke;

// stores info for a LineShape
public class LineShapeInfo extends ShapeInfo {
    static Icon icon = new ImageIcon(icon_path+"line.png");
    static Icon inv_icon = new ImageIcon(icon_path+"lineinv.png");
    // x/y coordinates of endpoints, with variances
    float e1x, e1y, e1v;
    float e2x, e2y, e2v;
    float r, theta;
    boolean end1_valid, end1_active, end2_valid, end2_active;
    
    public LineShapeInfo(int _id, int _parentId, String _name, Color _color,
			 float _centroidx, float _centroidy, float _centroidz,
			 float _e1x, float _e1y, float _e1v, 
			 float _e2x, float _e2y, float _e2v,
			 float _r, float _theta, 
			 boolean _end1_valid, boolean _end1_active,
			 boolean _end2_valid, boolean _end2_active) {
		
	super(_id, _parentId, _name, _color, _centroidx, _centroidy, _centroidz);
	e1x = _e1x;
	e1y = _e1y;
	e1v = _e1v;
	e2x = _e2x;
	e2y = _e2y;
	e2v = _e2v;
	r = _r;
	theta = _theta;
	end1_valid = _end1_valid;
	end1_active = _end1_active;
	end2_valid = _end2_valid;
	end2_active = _end2_active;
    }
	
    // returns left-most coordinate of object
    public float getLeft() { return java.lang.Math.min(e1x,e2x); }
    // returns right-most coordinate of object
    public float getRight() { return java.lang.Math.max(e1x,e2x); }
    // returns top-most coordinate of object
    public float getTop() { return java.lang.Math.min(e1y,e2y); }
    // returns bottom-most coordinate of object
    public float getBottom() { return java.lang.Math.max(e1y,e2y); }

    public String toString() {
	return (super.toString()
		+ " (" + e1x + " " + e1y + " " + e1v + ")---(" + e2x + " " + e2y + " " + e2v + ")  "
		+ "r=" + r + " theta=" + theta); }

    public Icon getIcon() { 
	if (inverted)
	    return inv_icon;
	return icon; 
    }

    public void renderTo(Graphics2D graphics, float scaling) {
	final int radius = (int) (10/scaling);
	    final float big_slope = (float)1e5;
	    if ( e1x > e2x ) {
		boolean t_valid = end1_valid;
		boolean t_active = end1_active;
		end1_valid = end2_valid;
		end1_active = end2_active;
		end2_valid = t_valid;
		end2_active = t_active;
		float tx=e1x;
		float ty=e1y;
		e1x=e2x;
		e1y=e2y;
		e2x=tx;
		e2y=ty;}
	    float m, b;
	    if ( e2x-e1x > 1/big_slope ) {
		m = (e2y-e1y)/(e2x-e1x);
		b = e1y-e1x*m;
	    } else {
		m = big_slope;
		b = 0;
	    }
	    int e1xi, e1yi, e2xi, e2yi;
	    if (end1_active) {
		e1xi = (int)e1x; e1yi = (int)e1y;
	    } else {
		e1xi = -10000; e1yi = (int)(e1xi*m+b);
	    };
	    if (end2_active) {
		e2xi = (int)e2x; e2yi = (int)e2y;
	    } else {
		e2xi = 10000; e2yi = (int)(e2xi*m+b);
	    };
	    graphics.setStroke(new BasicStroke(1.0f));
	    graphics.drawLine(e1xi,e1yi,e2xi,e2yi);
	    final float dl = 5/scaling;
	    if (end1_valid) {
		int v1xi = (int)e1x, v1yi = (int)e1y;
		graphics.drawLine((int)(v1xi+dl*Math.cos(theta)+.5), (int)(v1yi+dl*Math.sin(theta)+.5),
				  (int)(v1xi-dl*Math.cos(theta)+.5), (int)(v1yi-dl*Math.sin(theta)+.5));
		//	    graphics.fillOval((int)v1x-radius/2, (int)v1y-radius/2, radius, radius);
	    }
	    if (end2_valid) {
		int v2xi = (int)e2x, v2yi = (int)e2y;
		graphics.drawLine((int)(v2xi+dl*Math.cos(theta)+.5), (int)(v2yi+dl*Math.sin(theta)+.5),
				  (int)(v2xi-dl*Math.cos(theta)+.5), (int)(v2yi-dl*Math.sin(theta)+.5));
		//	    graphics.fillOval((int)e2x-radius/2, (int)e2y-radius/2, radius, radius);
	    }
	    
	// draw circles for variance
	// graphics.drawOval((int)e1x,(int)e1y,(int)(e1v/2),(int)(e1v/2));
	// graphics.drawOval((int)e2x,(int)e2y,(int)(e2v/2),(int)(e2v/2));
    }

    public float getE1X() { return e1x; }
    public float getE1Y() { return e1y; }
    public float getE1V() { return e1v; }
    public float getE2X() { return e2x; }
    public float getE2Y() { return e2y; }
    public float getE2V() { return e2v; }
    public float getR() { return r; }
    public float getTheta() { return theta; }

    public float getIdX() { return getE1X(); }
    public float getIdY() { return getE1Y(); }
    
}

