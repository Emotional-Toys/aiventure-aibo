package org.tekkotsu.sketch;

import java.awt.Color;
import java.awt.Graphics2D;
import java.awt.Rectangle;
import javax.swing.Icon;
import javax.swing.ImageIcon;

public class AgentShapeInfo extends ShapeInfo {
	static Icon icon = new ImageIcon(icon_path+"agent.png");
	float orientation; // orientation of agent

    public AgentShapeInfo(int _id, int _parentId, String _name, Color _color,
			float _centroidx, float _centroidy, float _centroidz,
			float _orientation) {
		super(_id, _parentId, _name, _color, _centroidx,_centroidy, _centroidz);
		orientation = _orientation;
	}

	// returns left-most coordinate of object (want some buffer)
	public float getLeft() { return centroidx-2; }
	// returns right-most coordinate of object
	public float getRight() { return centroidx+2; }
	// returns top-most coordinate of object
	public float getTop() { return centroidy-2; }
	// returns bottom-most coordinate of object
	public float getBottom() { return centroidy+2; }

	public String toString() {
		return (super.toString() + " (x=" + centroidx + ", y=" + centroidy + ", theta=" + orientation + ")");
	}	
	
	public Icon getIcon() { return icon; }

    public void renderTo(Graphics2D graphics, float scaling) {
	    //		graphics.drawOval((int)(getCentroidX()+1),
	    //	(int)(getCentroidY()+1), 5, 5);
		int [] coordsX, coordsY;
		coordsX = new int[3];
		coordsY = new int[3];
		
		//represent the dog as a triangle which has base length of 10cm, height of 20cm
		//top
		coordsX[0] = (int) (centroidx + 100 * Math.cos(orientation));
		coordsY[0] = (int) (centroidy + 100 * Math.sin(orientation));

		//left
		coordsX[1] = (int) (centroidx + 120 * Math.cos(-2.618+orientation));
		coordsY[1] = (int) (centroidy + 120 * Math.sin(-2.618+orientation));

		//right
		coordsX[2] = (int) (centroidx + 120 * Math.cos(2.618+orientation));
		coordsY[2] = (int) (centroidy + 120 * Math.sin(2.618+orientation));
		graphics.drawPolygon(coordsX,coordsY, 3);

		//draw a point at the exact agent location
		Rectangle bounds = graphics.getClipBounds();
		int radius;
		if (bounds != null)
		    radius= java.lang.Math.max(4,java.lang.Math.max(bounds.width,bounds.height)/100);
		else
		    radius = 4;
		graphics.fillOval((int)centroidx-radius/2, (int)centroidy-radius/2, radius, radius);
	}

	public float getOrientation() { return orientation; }
}

