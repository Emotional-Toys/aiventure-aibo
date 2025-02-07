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

// stores info for a EllipseShape
// note that ellipse center is same as centroid
public class EllipseShapeInfo extends ShapeInfo {
    static Icon icon = new ImageIcon(icon_path+"ellipse.png");
    static Icon inv_icon = new ImageIcon(icon_path+"ellipseinv.png");
    float semimajor, semiminor; // length of semimajor axes
    float orientation; // orientation of principal axis

    public EllipseShapeInfo(int _id, int _parentId, String _name, Color _color,
			float _centroidx, float _centroidy, float _centroidz,
			float _semimajor, float _semiminor,
			float _orientation) {
		super(_id, _parentId, _name, _color, _centroidx, _centroidy, _centroidz);
		semimajor = _semimajor;
		semiminor = _semiminor;
		orientation = _orientation;
	}

	// returns left-most coordinate of object
	public float getLeft() { return centroidx-semimajor; }
	// returns right-most coordinate of object
	public float getRight() { return centroidx+semimajor; }
	// returns top-most coordinate of object
	public float getTop() { return centroidy-semiminor; }
	// returns bottom-most coordinate of object
	public float getBottom() { return centroidy+semiminor; }


	public String toString() {
		return (super.toString() + " center=(" + centroidx + " " + centroidy + " " + centroidz + ")"
			+ " smaj=" + semimajor 
			+ ", smin=" + semiminor 
			+ ", orient=" + orientation);
	}

	public Icon getIcon() { 
	    if (inverted)
		return inv_icon;
	    return icon; 
	}

	// rough rendering which doesn't take ellipse orientation into account
        // maybe do a hack-ish check if abs(orientation) more than pi/4?
    public void renderTo(Graphics2D graphics, float scaling) {
	// System.out.println("Drawing ellipse");

	graphics.setStroke(new BasicStroke(1.0f));

	// for proper orientation, rotate the coordinate because we cannot rotate the oval itself
	graphics.transform(AffineTransform.getRotateInstance(getOrientation(), getCentroidX(), getCentroidY())); // forward rotation
	graphics.drawOval((int)(getCentroidX()-semimajor+1), 
			  (int)(getCentroidY()-semiminor+1),
			  (int)(semimajor*2), (int)(semiminor*2));
	graphics.transform(AffineTransform.getRotateInstance(-getOrientation(), getCentroidX(), getCentroidY())); // backward rotation
    }

	public float getSemiMajor() { return semimajor; }
	public float getSemiMinor() { return semiminor; }
	public float getOrientation() { return orientation; }
}

