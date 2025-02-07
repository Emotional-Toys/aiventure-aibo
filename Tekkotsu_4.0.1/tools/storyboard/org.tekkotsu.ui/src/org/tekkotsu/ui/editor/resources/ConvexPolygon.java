/*
 * Created on Oct 28, 2004
 */
package org.tekkotsu.ui.editor.resources;

import org.eclipse.draw2d.Graphics;
import org.eclipse.draw2d.Polygon;
import org.eclipse.draw2d.geometry.Point;
import org.eclipse.draw2d.geometry.PointList;
import org.eclipse.draw2d.geometry.Rectangle;

public class ConvexPolygon extends Polygon {
	//private Rectangle polygonBound = new Rectangle();
	
	/**
	 * 
	 */
	public ConvexPolygon() {
		super();
		setBounds(new Rectangle(0,0,10,10));
	}
	
	/* (non-Javadoc)
	 * @see org.eclipse.draw2d.Figure#setBounds(org.eclipse.draw2d.geometry.Rectangle)
	 */
	public void setBounds(Rectangle rect) {

		PointList plist = new PointList();
		plist.addPoint(new Point(rect.x+rect.width/6,rect.y));
		plist.addPoint(new Point(rect.x+5*rect.width/6,rect.y));
		plist.addPoint(new Point(rect.x+rect.width-1,rect.y+rect.height/2));
		plist.addPoint(new Point(rect.x+5*rect.width/6,rect.y+rect.height-1));
		plist.addPoint(new Point(rect.x+rect.width/6,rect.y+rect.height-1));
		plist.addPoint(new Point(rect.x,rect.y+rect.height/2));
		this.setPoints(plist);
	}
	
//	@Override
	public void paintFigure(Graphics g) {
		g.fillPolygon(this.getPoints());
		g.drawPolygon(this.getPoints());
	}
}
