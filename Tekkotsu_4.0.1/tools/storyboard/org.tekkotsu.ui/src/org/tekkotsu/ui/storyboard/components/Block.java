/*
 * Created on Nov 11, 2004
 */
package org.tekkotsu.ui.storyboard.components;

import org.eclipse.draw2d.ColorConstants;
import org.eclipse.draw2d.Figure;
import org.eclipse.draw2d.Graphics;
import org.eclipse.draw2d.IFigure;
import org.eclipse.draw2d.Label;
import org.eclipse.draw2d.geometry.Point;
import org.eclipse.draw2d.geometry.PointList;
import org.eclipse.draw2d.geometry.Rectangle;
import org.eclipse.jface.resource.ColorRegistry;
import org.eclipse.swt.graphics.Color;
import org.eclipse.swt.graphics.RGB;
import org.eclipse.swt.widgets.Display;
import org.tekkotsu.ui.editor.model.StateNodeModel;
import org.tekkotsu.ui.storyboard.model.BlockModel;

/**
 * @author asangpet
 *
 */
public class Block extends Figure {	
	boolean active = false;
	ColorRegistry colReg;
	static final String COLOR_BASE = "_color_base";
	static final String COLOR_FADE = "_color_fade";
	static final String COLOR_BORDER="_color_border";
	Color baseColor, fgColor, bgColor;
	
	BlockModel model;
	StateNodeModel state;
	Rectangle constraint;
	TimeLine timeline;
	int offset = TimeLine.xoffset;
	int metric = TimeLine.gap;
	
	/*
	public Block() {
		super();
		colReg = new ColorRegistry();
		setColor(new RGB(220,220,220));
	}*/
	
	public Block(Display display, IFigure parent, BlockModel model, StateNodeModel state, TimeLine timeLine) {
		timeline = timeLine;
		colReg = new ColorRegistry(display);
		this.state = state;
		setModel(model);
		Color c = state.getColor();
		setColor(new RGB(c.getRed(), c.getGreen(), c.getBlue()));
		this.setToolTip(new Label(state.getLabel()));
	}
	
	private void setModel(BlockModel model) {
		this.model = model;
		Rectangle bound = state.getConstraint();
		
		int xoff = timeline.getOffset(model.getStart());
		int xwidth = timeline.getOffset(model.getEnd())-xoff;

		if ((xwidth<TimeLine.gap) && (model.isFinish())) {
		    timeline.getMarker().addMarker(model.getStart(),xoff);
		    timeline.getMarker().addMarker(model.getEnd(),xoff+state.getConstraint().width);
		    xwidth = timeline.getOffset(model.getEnd())-xoff;
		}
		
		setConstraint(new Rectangle(xoff,bound.y,xwidth,bound.height));
	}
	
	public void extendTo(int value) {
		Rectangle bound = state.getConstraint();
		int xoff = timeline.getOffset(model.getStart());
		int xwidth = timeline.getOffset(value)-xoff;
		setConstraint(new Rectangle(xoff,bound.y,xwidth,bound.height));		
	}
	
	private void setConstraint(Rectangle r) {
		constraint = r;
	}
	
	public Rectangle getConstraint() {
		return constraint;
	}
	
	private int colorMod(int oldcol, int delta) {
		oldcol = ((int)Math.round((double)oldcol*0.2) + delta);
		if (oldcol<0) return 0;
		if (oldcol>255) return 255;
		return oldcol;
	}
	
	public void setColor(RGB c) {		
		colReg.put(COLOR_BASE, c);		
		colReg.put(COLOR_FADE, new RGB(colorMod(c.red,200),colorMod(c.green,200),colorMod(c.blue,200)));
		colReg.put(COLOR_BORDER, new RGB(colorMod(c.red,100),colorMod(c.green,100),colorMod(c.blue,100)));
		setActive(active);
	}

	public void setActive(boolean b) {
		active = b;		
		Color c = colReg.get(COLOR_BASE);
		if (active) {
			bgColor = colReg.get(COLOR_BASE);
			if (c.getRed()+c.getGreen()+c.getBlue()>250)
				fgColor = ColorConstants.black;
			else 
				fgColor = ColorConstants.white;
		} else {
			fgColor = colReg.get(COLOR_BORDER);
			bgColor = colReg.get(COLOR_FADE);			
		}
	}

	protected void paintFigure(Graphics g2d) {
		//Graphics2D g2d = (Graphics2D)gr;		
		//Rectangle bound = (Rectangle)getParent().getLayoutManager().getConstraint(this);
		Rectangle clArea = getClientArea();
		Rectangle bound = getConstraint();		

		//g2d.setBackgroundColor(ColorConstants.cyan);
		//g2d.fillRectangle(0,0,400,400);

		//g2d.clearRect(0,0,bound.width,bound.height);		
		g2d.setBackgroundColor(bgColor);
		g2d.setForegroundColor(fgColor);
		
		String shape = state.getShape();
		if (shape.equals(StateNodeModel.P_SHAPE_STYLE_ELLIPSE)) {
		    int hbound = bound.height;
		    if (bound.width<bound.height) hbound = bound.width;
		    g2d.fillOval(clArea.x,clArea.y,hbound,bound.height);
		    g2d.fillOval(clArea.x+bound.width-hbound,clArea.y,hbound,bound.height);		    
		    g2d.drawArc(clArea.x,clArea.y,hbound,bound.height,90,180);
		    g2d.drawArc(clArea.x+bound.width-hbound,clArea.y,hbound,bound.height,270,180);		    
		    int xoff = clArea.x+hbound/2;
		    g2d.fillRectangle(xoff,clArea.y,bound.width-hbound,bound.height);
		    g2d.drawLine(xoff,clArea.y,xoff+bound.width-hbound,clArea.y);
		    g2d.drawLine(xoff-2,clArea.y+bound.height-1,xoff+bound.width-hbound+2,clArea.y+bound.height-1);
		} else if (shape.equals(StateNodeModel.P_SHAPE_STYLE_HEXAGON)){
		    int hbound = state.getConstraint().width/6;
		    if (2*hbound>bound.width) hbound = bound.width/6;

		    PointList plist = new PointList();
			plist.addPoint(new Point(clArea.x+hbound,clArea.y));
			plist.addPoint(new Point(clArea.x+bound.width-hbound,clArea.y));
			plist.addPoint(new Point(clArea.x+bound.width-1,clArea.y+clArea.height/2));
			plist.addPoint(new Point(clArea.x+bound.width-hbound,clArea.y+clArea.height-1));
			plist.addPoint(new Point(clArea.x+hbound,clArea.y+clArea.height-1));
			plist.addPoint(new Point(clArea.x,clArea.y+clArea.height/2));
			g2d.fillPolygon(plist);
			g2d.drawPolygon(plist);
		}  else if (shape.equals(StateNodeModel.P_SHAPE_STYLE_ROUNDED_RECTANGLE)){
		    Rectangle r = new Rectangle(clArea.x,clArea.y,bound.width,bound.height);
		    g2d.fillRoundRectangle(r,8,8);		
		    r = new Rectangle(clArea.x,clArea.y,bound.width-1,bound.height-1);
			g2d.drawRoundRectangle(r,8,8);
		} else {
		    //if (shape.equals(StateNodeModel.P_SHAPE_STYLE_RECTANGLE)){
		    // default style is RECTANGLE
			g2d.fillRectangle(clArea.x,clArea.y,bound.width,bound.height);		
			g2d.drawRectangle(clArea.x,clArea.y,bound.width-1,bound.height-1);		    
		}
		
		String label = state.getLabel();
		if (label==null) label = state.getId();
		int swidth = g2d.getFontMetrics().getAverageCharWidth()*label.length();
		int sheight = g2d.getFontMetrics().getHeight();
		if (swidth<bound.width) {
			g2d.drawString(label,clArea.x+(bound.width-swidth)/2,clArea.y+(bound.height-sheight)/2);
		} else {
			g2d.drawString(label,clArea.x+2,clArea.y+(bound.height-sheight)/2);
		}
	}
	/**
	 * @return Returns the model.
	 */
	public BlockModel getModel() {
		return model;
	}
}
