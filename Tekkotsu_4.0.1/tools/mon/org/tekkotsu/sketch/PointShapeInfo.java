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
public class PointShapeInfo extends ShapeInfo {
    static Icon icon = new ImageIcon(icon_path+"point.png");
    static Icon inv_icon = new ImageIcon(icon_path+"pointinv.png");
    
    public PointShapeInfo(int _id, int _parentId, String _name, Color _color,
			float _centroidx, float _centroidy, float _centroidz) {
	super(_id, _parentId, _name, _color, _centroidx, _centroidy, _centroidz);
    }
	
    // returns left-most coordinate of object
    public float getLeft() { return centroidx; }
    // returns right-most coordinate of object
    public float getRight() { return centroidx; }
    // returns top-most coordinate of object
    public float getTop() { return centroidy; }
    // returns bottom-most coordinate of object
    public float getBottom() { return centroidy; }
    
    public String toString() {
	String _point = "point=(" + centroidx + " " + centroidy + " " + centroidz + ")";
	return (super.toString() + " " + _point); }

    public Icon getIcon() { 
	if (inverted)
	    return inv_icon;
	else
	    return icon; 
    }

    public void renderTo(Graphics2D graphics, float scaling) {
	Rectangle bounds = graphics.getClipBounds();
	int radius;
	if (bounds != null)
	    radius= java.lang.Math.max(4,java.lang.Math.max(bounds.width,bounds.height)/100);
	else
	    radius = 4;
	graphics.fillOval((int)centroidx-radius/2, (int)centroidy-radius/2, radius, radius);
    }

}

