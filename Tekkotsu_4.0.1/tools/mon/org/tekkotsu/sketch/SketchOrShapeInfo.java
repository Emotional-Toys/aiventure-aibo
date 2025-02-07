package org.tekkotsu.sketch;

import java.awt.Graphics2D;
import java.awt.Graphics;
import java.awt.Color;
import javax.swing.Icon;
import javax.swing.ImageIcon;
import java.awt.PopupMenu;

import java.awt.Rectangle;
import java.awt.geom.*;

// SketchOrShapeInfo is the parent class for SketchInfo and ShapeInfo

public class SketchOrShapeInfo {
    final static String icon_path = "org/tekkotsu/sketch/icons/";

    static Icon icon = new ImageIcon(icon_path+"unknown.png");
    static Icon inverted_icon = null;
    int id;
    int parentId;
    String name;
    Color color;
    boolean isVisible;
    boolean inverted;
    int width;
    int height;

    public SketchOrShapeInfo(int _id, int _parentId, String _name, Color _color){
	id = _id;
	parentId = _parentId;
	name = _name;
	color = _color;
    }

    public int getId() { return id; }
    public int getParentId() { return parentId; }
    public String getName() { return name; }

    public float getLeft() { return 0; }
    public float getRight() { return 0; }
    public float getTop() { return 0; }
    public float getBottom() { return 0; }

    public Color getColor() 
    { 
	if (inverted)
	    return new Color(255-color.getRed(),
				 255-color.getGreen(),
				 255-color.getBlue());
	else
	    return color; 
    }

    public Color getColorInverted()
    {
	return new Color(255-color.getRed(),
			 255-color.getGreen(),
			 255-color.getBlue());
    }

    public void setVisible(boolean vis) {
	isVisible = vis;
    }

    public boolean getVisible() {
	return isVisible;
    }
    
    public String toString() {
	//	return (name + "(id " + id + ", parentId " + parentId + ")");
	return name + " " + id;
    }

    public Icon getIcon() { 
	if (!inverted || inverted_icon == null)
	    return icon;
	else
	    return inverted_icon;
    }

    public void renderTo(Graphics2D graphics, float scaling) {
	System.out.println("Rendering "+id);
    }
    
    public void renderTo(Graphics graphics, Rectangle2D.Double r) {}

    public void invert() {
	inverted = !inverted;
    }

    public void setInverted(boolean _inverted) {
	inverted = _inverted;
    }
    
}


