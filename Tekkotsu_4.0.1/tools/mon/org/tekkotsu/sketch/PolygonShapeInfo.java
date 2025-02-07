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

// stores info for a PolygonShape
public class PolygonShapeInfo extends ShapeInfo {
	static Icon icon = new ImageIcon(icon_path+"poly.png");
	static Icon inv_icon = new ImageIcon(icon_path+"polyinv.png");
	// x/y coordinates of endpoints, with variances
    int num_vertices;
    float[][] vertices;
    boolean end1_valid, end2_valid;
    
    public PolygonShapeInfo(int _id, int _parentId, String _name, Color _color,
			    float _centroidx, float _centroidy, float _centroidz,
			    int _num_vertices, float[][] _vertices,
			    boolean _end1_valid, boolean _end2_valid) {			
	super(_id, _parentId, _name, _color, _centroidx, _centroidy, _centroidz);
	num_vertices = _num_vertices;
	vertices = _vertices;
	end1_valid = _end1_valid;
	end2_valid = _end2_valid;
    }
	
    // returns left-most coordinate of object
    public float getLeft() { 
	float min_x = vertices[0][0];
	for (int i = 1; i < num_vertices; i++)
	    if (min_x > vertices[i][0])
		min_x = vertices[i][0];
	return min_x; 
    }
    // returns right-most coordinate of object
    public float getRight() {
	float max_x = vertices[0][0];
	for (int i = 1; i < num_vertices; i++)
	    if (max_x < vertices[i][0])
		max_x = vertices[i][0];
	return max_x; 
    }
    // returns top-most coordinate of object
    public float getTop() { 
	float min_y = vertices[0][1];
	for (int i = 1; i < num_vertices; i++)
	    if (min_y > vertices[i][1])
		min_y = vertices[i][1];
	return min_y;     
    }
    // returns bottom-most coordinate of object
    public float getBottom() { 
	float max_y = vertices[0][1];
	for (int i = 1; i < num_vertices; i++)
	    if (max_y < vertices[i][1])
		max_y = vertices[i][1];
	return max_y; 
    }
    public String toString() {
	String vtx_coords = "(" + vertices[0][0] + "," + vertices[0][1] + ")";
	for (int i = 1; i < num_vertices; i++)
	    vtx_coords += "->(" + vertices[i][0] + "," + vertices[i][1] + ")";
	return (super.toString() + " " +  vtx_coords);
    }    
    public Icon getIcon() { 
	if (inverted)
	    return inv_icon;
	return icon; 
    }
    
    public void renderTo(Graphics2D graphics, float scaling) {
	final int radius = (int) (10.0/scaling);
	for (int i = 0; i < num_vertices-1; i++)
	    graphics.drawLine((int)vertices[i][0],(int)vertices[i][1],(int)vertices[i+1][0],(int)vertices[i+1][1]);

	if (end1_valid)
	    graphics.fillOval((int)vertices[0][0]-radius/2, (int)vertices[0][1]-radius/2, radius, radius);

	if (end2_valid)
	    graphics.fillOval((int)vertices[num_vertices-1][0]-radius/2, (int)vertices[num_vertices-1][1]-radius/2, radius, radius);
    }

    public float[] getFirstVertex() {
	return vertices[0];
    }
    
    public float getIdX() { return vertices[0][0]; }
    public float getIdY() { return vertices[0][1]; }
}

