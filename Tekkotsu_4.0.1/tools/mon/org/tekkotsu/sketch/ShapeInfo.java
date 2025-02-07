package org.tekkotsu.sketch;

import java.awt.Color;
import java.awt.Graphics2D;
import java.awt.Graphics;
import java.awt.Rectangle;
import java.awt.geom.*;

// stores info for a Shape, to use as UserObject for DefaultMutableTreeNode

public class ShapeInfo extends SketchOrShapeInfo {
    float centroidx, centroidy, centroidz;
    boolean useDefaultSelection, useDefaultInversion;
    final static String icon_path = "org/tekkotsu/sketch/icons/";
    
    public ShapeInfo(int _id, int _parentId, String _name, Color _color,
		     float _centroidx, float _centroidy, float _centroidz) {
	super(_id, _parentId, _name, _color);
	centroidx = _centroidx;
	centroidy = _centroidy;
	centroidz = _centroidz;
	useDefaultSelection = true;
	useDefaultInversion = true;
    }


    // if no specific renderer defined, just plot a point at centroid
    public void renderTo(Graphics2D graphics, float scaling) {
	graphics.setColor(color);
	graphics.drawOval((int)centroidx,(int)centroidy,5,5);
    }

    public void renderTo(Graphics graphics, Rectangle2D.Double r) {
	double xscale = r.width;
	double yscale = r.height;
	graphics.setColor(color);
	graphics.drawOval((int)(centroidx*xscale+r.x),(int)(centroidy*yscale+r.y),(int)(5*xscale),(int)(5*yscale));	
    }

    public float getCentroidX() { return centroidx; }
    public float getCentroidY() { return centroidy; }
    public float getCentroidZ() { return centroidz; }

    // Specific call to get the coordinates of where to display the shape's Id
    // defaults to the centroid, override for other behavior
    public float getIdX() { return centroidx; }
    public float getIdY() { return centroidy; }

    public void setUseDefaultSelection(boolean useSelection) { useDefaultSelection = useSelection; }
    public boolean getUseDefaultSelection() { return useDefaultSelection; }
    public void setUseDefaultInversion(boolean useInversion) { useDefaultInversion = useInversion; }
    public boolean getUseDefaultInversion() { return useDefaultInversion; }
}


