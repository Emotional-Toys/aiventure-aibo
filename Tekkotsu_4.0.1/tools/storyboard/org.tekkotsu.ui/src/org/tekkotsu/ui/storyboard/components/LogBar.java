/*
 * Created on Mar 29, 2005
 */
package org.tekkotsu.ui.storyboard.components;

import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;
import java.util.Iterator;

import org.eclipse.draw2d.Graphics;
import org.eclipse.draw2d.IFigure;
import org.eclipse.draw2d.MouseEvent;
import org.eclipse.draw2d.MouseListener;
import org.eclipse.draw2d.MouseMotionListener;
import org.eclipse.draw2d.Panel;
import org.eclipse.draw2d.geometry.Point;
import org.eclipse.draw2d.geometry.Rectangle;
import org.eclipse.jface.resource.ColorRegistry;
import org.tekkotsu.ui.storyboard.ResourceRegistry;

/**
 * 
 * @author asangpet
 */
public class LogBar extends Panel implements PropertyChangeListener
{
	public static int gap = 8, halfgap = 4, xoffset = 20, height=28;//25;
		
	private ColorRegistry colorReg;
	private boolean timeLock = false;
	
	int cursor = 0;
	int timeLimit = TimeLine.TIMELINE_INIT_LIMIT;
	int maxValue = 0;
	double timeScale = 0.001;
	Rectangle constraint;
	TimeLine timeLine;
	
	public void setLimit(int time) {
		timeLimit = time;
		int voffset = TimeLine.height-1; //getParent().getClientArea().height - TimeLine.height;
		constraint = new Rectangle(0,voffset,timeLine.getOffset(time),height);
		getParent().getLayoutManager().setConstraint(this,constraint);
		firePropertyChange(TimeLine.TIMELINE_LIMIT_CHANGED,null,constraint);
	}
	
	public void setVerticalOffset(int voff) {
	    int voffset = TimeLine.height-1; // getParent().getClientArea().height - TimeLine.height;
	    Rectangle constraint = new Rectangle(0,voffset+voff,timeLine.getOffset(timeLimit),height);		
		getParent().getLayoutManager().setConstraint(this,constraint);		
		// firePropertyChange(TIMELINE_LIMIT_CHANGED,null,constraint);	    
	}
	
	public int getLimit() {
		return timeLimit;
	}
	
	public void setLock(boolean locked) {
		timeLock = locked;
		if (locked) {
			this.setValue(maxValue);
		}
	}
	
	public boolean isLock() {
		return timeLock;
	}
	
	public Rectangle getConstraint() {
		return constraint;
	}
	
	public double getTimeScale() {
		return timeScale;
	}
	
	/* (non-Javadoc)
	 * @see org.eclipse.draw2d.Figure#paintFigure(org.eclipse.draw2d.Graphics)
	 */
	protected void paintFigure(Graphics gc) {
	    super.paintFigure(gc);		    

	    Rectangle bounds = getBounds();
		
		gc.setBackgroundColor(colorReg.get("timeline_bg"));
		gc.fillRectangle(getBounds());
		gc.setForegroundColor(colorReg.get("timeline_bgdim1"));
		gc.drawLine(bounds.x, bounds.y+bounds.height-2, bounds.x+bounds.width, bounds.y+bounds.height-2);
		gc.setForegroundColor(colorReg.get("timeline_bgdim2"));
		gc.drawLine(bounds.x, bounds.y+bounds.height - 3, bounds.x+bounds.width, bounds.y+bounds.height - 3);
		gc.setForegroundColor(colorReg.get("timeline_bgdim3"));
		gc.drawLine(bounds.x, bounds.y+bounds.height - 4, bounds.x+bounds.width, bounds.y+bounds.height - 4);
		
		gc.setForegroundColor(colorReg.get("timeline_tick"));
		gc.drawLine(bounds.x, bounds.y, bounds.x+bounds.width, bounds.y);
		gc.drawLine(bounds.x, bounds.y+bounds.height - 1, bounds.x+bounds.width, bounds.y+bounds.height - 1);

		//Font sansSerifFont = new Font("Sans", Font.PLAIN, 10);
		//g2d.setFont(sansSerifFont);
		int count = 4, metric = 0, xoff;
		
		// draw cursor
		gc.setXORMode(true);
		gc.setBackgroundColor(colorReg.get("timeline_cursor"));
		gc.fillRectangle(bounds.x+ timeLine.getOffset(getValue())- TimeLine.halfgap, bounds.y+1, TimeLine.gap,
				bounds.height - 2);
		gc.setXORMode(false);
	}
	
	/**
	 * 
	 */
	public LogBar(TimeLine timeline) {	
		//setBounds(new Rectangle(0,0,limit*gap,25));
		timeLine = timeline;
		
		// initiate color registry
		colorReg = ResourceRegistry.getInstance().getColorRegistry();
		
		// initialize event listener
		addMouseListener(new MouseListener.Stub() {
			public void mousePressed(MouseEvent me) {
				if (me.button==1)
					moveCursor(me.getLocation());
			}
		});
		
		addMouseMotionListener(new MouseMotionListener.Stub() {
			public void mouseDragged(MouseEvent me) {
				if (me.getState() == MouseEvent.BUTTON1)
					moveCursor(me.getLocation());
			}
		});
		
	}

	// move cursor to specific coordinate
	public void moveCursor(Point p) {		
	    int cursor = timeLine.getTime(p.x);
		if (cursor<0) cursor = 0;
		setValue(cursor);
		firePropertyChange(TimeLine.TIMELINE_CURSOR_CHANGED, null, new Integer(this.cursor));
	}
	
	// set cursor to the specified time unit
	public void setValue(int value) {
		if (maxValue<value) maxValue = value;
		cursor = value;
		if (cursor<0) cursor = 0;		
		repaint();
	}
	
	// get the current cursor position
	public int getValue() {
		return cursor;
	}

	public void propertyChange(PropertyChangeEvent evt) {	
		if (evt.getPropertyName().equals(TimeLine.TIMELINE_CURSOR_CHANGED)) {
			setValue(((Integer) evt.getNewValue()).intValue());
		}
	}	
	
    public void addChild(IFigure figure) {
        Rectangle rect = null;
        add(figure);
              
        if (figure instanceof LogObject) {
            LogObject fig = (LogObject)figure;
            rect = fig.getConstraint();            
            // scan for close figure
            int curoff = timeLine.getOffset(fig.getModel().getTime());
            int idx = 0;
            for (Iterator iter = getChildren().iterator(); iter.hasNext(); ) {
                Object child = iter.next();
                if ((child instanceof LogObject) && (fig.getModel()!=((LogObject)child).getModel())) {
                    LogObject clog = (LogObject)child;
                    int coff = timeLine.getOffset(clog.getModel().getTime());
                    if (Math.abs(coff-curoff)<8) {
                        clog.setLayoutIndex(++idx);
                    }
                }
            }
            fig.setLayoutIndex(++idx);
        }
    }

}
