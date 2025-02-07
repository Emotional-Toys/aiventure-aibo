/*
 * Created on Nov 17, 2004
 */
package org.tekkotsu.ui.storyboard.components;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

import org.eclipse.draw2d.ColorConstants;
import org.eclipse.draw2d.Figure;
import org.eclipse.draw2d.Graphics;
import org.eclipse.draw2d.Label;
import org.eclipse.draw2d.Polygon;
import org.eclipse.draw2d.geometry.PointList;
import org.eclipse.draw2d.geometry.Rectangle;
import org.eclipse.jface.resource.ColorRegistry;
import org.eclipse.swt.graphics.RGB;
import org.eclipse.swt.widgets.Display;
import org.tekkotsu.ui.editor.model.MultiTransitionModel;
import org.tekkotsu.ui.editor.model.SourceTransitionModel;
import org.tekkotsu.ui.editor.model.StateNodeModel;
import org.tekkotsu.ui.editor.model.ViewModel;
import org.tekkotsu.ui.storyboard.model.TransitionModel;

/**
 * @author asangpet
 *
 */
public class Transition extends Figure {
	static final String COLOR_BASE="_color_base";
	static final String COLOR_FADE="_color_fade";
	
	boolean active = false;
	ColorRegistry colReg;
	Rectangle constraint;
		
	TransitionModel model;
	MultiTransitionModel trans;
	org.tekkotsu.ui.editor.model.TransitionModel etrans;
	List nodeList;
	int offset = TimeLine.xoffset;
	
	public Transition() {
		super();
		colReg = new ColorRegistry();
		setColor(new RGB(0,0,0));
	}
	
	public Transition(Display d, TransitionModel model, MultiTransitionModel trans, TimeLine timeLine) {
		colReg = new ColorRegistry(d);
		this.model = model;
		this.trans = trans;
		//org.eclipse.draw2d.geometry.Rectangle bound = trans.getConstraint();		
		ViewModel view = (ViewModel)trans.getParent();
		SourceTransitionModel strans = (SourceTransitionModel)trans.getSource();
		int ymin=Integer.MAX_VALUE;
		int ymax=Integer.MIN_VALUE;
				
		List nodes = new ArrayList();
		nodes.addAll(strans.getSourceNodes());				
		nodes.addAll(strans.getDestNodes());
		nodeList = new ArrayList();		
		Iterator iter = nodes.iterator();
		while (iter.hasNext()) {
			StateNodeModel state = (StateNodeModel)view.getPartChild((String)iter.next());
			nodeList.add(state);
			int top = state.getConstraint().y;
			int bottom = state.getConstraint().y+state.getConstraint().height;
			if (top<ymin) ymin=top;
			if (bottom>ymax) ymax=bottom;
		}
		
		//int xoff = ((int)Math.round(model.getStart()*timeLine.getTimeScale())*metric+offset);
		int xoff = 0;
		if (model.getEnd()>-1)
			xoff = timeLine.getOffset(model.getEnd());
		else
			xoff = timeLine.getOffset(model.getStart());
		
		setConstraint(new Rectangle(xoff-3,ymin,7,ymax-ymin));
		setColor(trans.getColor().getRGB());
		this.setToolTip(new Label(trans.getId()));// .getLabel()));
	}
	
	private void setConstraint(Rectangle r) {
		constraint = r;
	}
	
	public Rectangle getConstraint() {
		return new Rectangle(constraint);
	}

	private int colorMod(int oldcol, int delta) {
		oldcol = (int)Math.round((double)oldcol*0.2) + delta;
		if (oldcol<0) return 0;
		if (oldcol>255) return 255;
		return oldcol;
	}
	
	public void setColor(RGB c) {
		colReg.put(COLOR_BASE,c);
		colReg.put(COLOR_FADE, new RGB(colorMod(c.red,200),colorMod(c.green,200),colorMod(c.blue,200)));
		setActive(active);
	}

	public void setActive(boolean b) {
		active = b;				
		if (active) {
			setBackgroundColor(colReg.get(COLOR_BASE));
			setForegroundColor(ColorConstants.black);
		} else {
			setForegroundColor(colReg.get(COLOR_FADE));
			setBackgroundColor(colReg.get(COLOR_FADE));			
		}
	}
	
	public void paint(Graphics g2d) {
		Rectangle bound = this.getConstraint();
		Rectangle clArea = this.getClientArea();
		
		g2d.setBackgroundColor(getLocalBackgroundColor());	
		g2d.setForegroundColor(getLocalForegroundColor());
		//g2d.drawLine(3+clArea.x,clArea.y,3+clArea.x,bound.height+clArea.y);
		
		Polygon p = new Polygon();
		int[] xp = new int[] {clArea.x,clArea.x+3,clArea.x+6,clArea.x+3};
		
		int miny=Integer.MAX_VALUE, maxy=-1;
		if (nodeList!=null) {
			Iterator iter = nodeList.iterator();			
			while (iter.hasNext()) {
				StateNodeModel state = (StateNodeModel)iter.next();
				Rectangle rect = state.getConstraint();
				int yoff = clArea.y+(rect.y-bound.y)+rect.height/2;
				if (yoff<miny) miny = yoff;
				if (yoff>maxy) maxy = yoff;
				int[] yp = new int[] {yoff,yoff-3,yoff,yoff+3};
				PointList plist = new PointList(4);
				plist.addPoint(xp[0],yp[0]);
				plist.addPoint(xp[1],yp[1]);
				plist.addPoint(xp[2],yp[2]);
				plist.addPoint(xp[3],yp[3]);				
				g2d.fillPolygon(plist);
			}		
		}
		if (miny==Integer.MAX_VALUE) miny=clArea.y;
		if (maxy==-1) maxy=clArea.y+bound.height;
		g2d.drawLine(3+clArea.x,miny,3+clArea.x,maxy);
	}
	/**
	 * @return Returns the model.
	 */
	public TransitionModel getModel() {
		return model;
	}
	
}
