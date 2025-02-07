/*
 * Created on Nov 11, 2004
 */
package org.tekkotsu.ui.storyboard.components;

import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;
import java.util.HashSet;
import java.util.Iterator;
import java.util.List;
import java.util.Set;

import org.eclipse.draw2d.ColorConstants;
import org.eclipse.draw2d.Graphics;
import org.eclipse.draw2d.IFigure;
import org.eclipse.draw2d.MouseEvent;
import org.eclipse.draw2d.MouseListener;
import org.eclipse.draw2d.MouseMotionListener;
import org.eclipse.draw2d.Panel;
import org.eclipse.draw2d.geometry.Dimension;
import org.eclipse.draw2d.geometry.Point;
import org.eclipse.draw2d.geometry.Rectangle;
import org.eclipse.jface.viewers.StructuredSelection;
import org.tekkotsu.ui.storyboard.views.StoryboardViewer;

/**
 * @author asangpet
 *  
 */

public class Storyboard extends Panel implements PropertyChangeListener {

    static final int preferredWidth = 800, preferredHeight = 25;

    public static final String PROP_DIM_CHANGE = "_prop_dim_change";

    int gap = 8, halfgap = 4, xoffset = 20;

    int cursor = 0;

    boolean mouseDown = false;

    StoryboardViewer parentViewer;

    TimeLine timeLine;

    public Storyboard(StoryboardViewer viewer, TimeLine timeline) {
        super();
        timeLine = timeline;
        parentViewer = viewer;

        // initialize event listener
        addMouseListener(new MouseListener.Stub() {
            public void mousePressed(MouseEvent me) {
                if (me.button == 1)
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

    public void addChild(IFigure figure) {
        add(figure);
        Rectangle rect = null;
        if (figure instanceof Block)
            rect = ((Block) figure).getConstraint();
        if (figure instanceof Transition)
            rect = ((Transition) figure).getConstraint();
        if (figure instanceof LogObject)
            rect = ((LogObject) figure).getConstraint();
        if (figure instanceof ImageObject)
            rect = ((ImageObject) figure).getConstraint();
        if (rect != null) {
            Rectangle newrect = new Rectangle((Rectangle) getParent()
                    .getLayoutManager().getConstraint(this));
            if (rect.y + rect.height > newrect.height) {
                newrect.height = rect.y + rect.height;
                getParent().getLayoutManager().setConstraint(this, newrect);
            }
        }
    }

    public void propertyChange(PropertyChangeEvent evt) {
        if (evt.getPropertyName().equals(TimeLine.TIMELINE_CURSOR_CHANGED)) {
            setValue(((Integer) evt.getNewValue()).intValue());
        } else if (evt.getPropertyName().equals(PROP_DIM_CHANGE)) {
            Rectangle newrect = new Rectangle((Rectangle) getParent()
                    .getLayoutManager().getConstraint(this));
            newrect.height = ((Rectangle) evt.getNewValue()).height
                    - TimeLine.height - TimeLine.height;
            getParent().getLayoutManager().setConstraint(this, newrect);
        } else if (evt.getPropertyName()
                .equals(TimeLine.TIMELINE_LIMIT_CHANGED)) {
            Rectangle newrect = new Rectangle((Rectangle) getParent()
                    .getLayoutManager().getConstraint(this));
            newrect.width = ((Rectangle) evt.getNewValue()).width;
            //System.out.println(newrect);
            getParent().getLayoutManager().setConstraint(this, newrect);
        }
    }

    public void setLimit(int time) {
    	Rectangle newrect = new Rectangle((Rectangle) getParent()
                .getLayoutManager().getConstraint(this));        
        newrect.width = timeLine.getOffset(time);
        getParent().getLayoutManager().setConstraint(this, newrect);	
    }
    
    /**
     * Move cursor to a specific time located by point P
     * 
     * @param p
     *            a point which the cursor should move to
     */
    public void moveCursor(Point p) {
        int cursor = timeLine.getTime(p.x);
        if (cursor < 0)
            cursor = 0;
        setValue(cursor);
        firePropertyChange(TimeLine.TIMELINE_CURSOR_CHANGED, null, new Integer(
                this.cursor));
    }

    /**
     * Set cursor at a specific point in time
     * 
     * @param cursor
     *            time which the cursor should move to
     */
    public void setValue(int cursor) {
        boolean changed = this.cursor != cursor;

        this.cursor = cursor;
        if (cursor < 0)
            cursor = 0;

        // extends all unfinished block
        if (timeLine.isLock()) {
            Iterator itr = this.getChildren().iterator();
            while (itr.hasNext()) {
                IFigure figure = (IFigure) itr.next();
                if (figure instanceof Block) {
                    Block block = (Block) figure;
                    if (!block.model.isFinish()) {
                        block.extendTo(cursor);
                        getLayoutManager().setConstraint(block,
                                block.getConstraint());
                        //Rectangle realcon =
                        // (Rectangle)getLayoutManager().getConstraint(block);
                        this.layout();
                        //System.out.println(">"+block.model.getStart()+","+cursor*500+":"+realcon);
                    }
                }
            }
        }

        // set proper highlighting
        if (changed) {
            //Component[] components = this.getComponents();
            List components = this.getChildren();
            Set selectSet = new HashSet();
            int tcursor = timeLine.getOffset(getValue());

            Iterator iter = components.iterator();
            while (iter.hasNext()) {
                Object component = iter.next();
                if (component instanceof ImageObject) {
                    ImageObject img = (ImageObject) component;
                    Rectangle bound = img.getConstraint();
                    int starttime = bound.x;
                    int endtime = bound.x + bound.width - 1;
                    if (starttime <= tcursor && endtime >= tcursor) {
                        selectSet.add(img.getModel());
                    }
                } else if (component instanceof LogObject) {
                    LogObject log = (LogObject) component;
                    Rectangle bound = log.getConstraint();
                    int starttime = bound.x;
                    int endtime = bound.x + bound.width - 1;
                    if (starttime <= tcursor && endtime >= tcursor) {
                        selectSet.add(log.getModel());
                    }
                } else if (component instanceof Transition) {
                    Transition t = (Transition) component;
                    Rectangle bound = t.getConstraint();
                    int starttime = bound.x;
                    int endtime = bound.x + bound.width - 1;
                    int temp=0;
                    if (timeLine.isLock()) temp=8; 
                    if (starttime-temp <= tcursor && endtime+temp >= tcursor) {
                        selectSet.add(t.getModel());
                        t.setActive(true);
                    } else
                        t.setActive(false);
                } else if (component instanceof Block) {
                    Block block = (Block) component;
                    Rectangle bound = block.getConstraint();
                    int starttime = bound.x;
                    int endtime = bound.x + bound.width - 1;
                    if (starttime-2 <= tcursor && endtime+2 >= tcursor) {
                        selectSet.add(block.getModel());
                        block.setActive(true);
                    } else {
                        block.setActive(false);
                    }
                }
            }
            StructuredSelection selection = new StructuredSelection(selectSet
                    .toArray());
            parentViewer.setSelection(selection);
        }

        //drawBuffer();
        repaint();
    }

    /*
     * (non-Javadoc)
     * 
     * @see org.eclipse.draw2d.Figure#paintFigure(org.eclipse.draw2d.Graphics)
     */
    protected void paintFigure(Graphics g) {
        super.paintFigure(g);
        Dimension dim = getSize();
        //paintChildren(g);

        // draw cursor
        g.setForegroundColor(ColorConstants.red);
        g.drawLine(timeLine.getOffset(getValue()), TimeLine.height, timeLine
                .getOffset(getValue()), TimeLine.height + LogBar.height
                + dim.height - 1);

    }

    public int getValue() {
        return cursor;
    }
}