/*
 * Created on Nov 9, 2004
 */
package org.tekkotsu.ui.storyboard.components;

import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;
import java.text.DecimalFormat;
import java.text.NumberFormat;

import org.eclipse.draw2d.Figure;
import org.eclipse.draw2d.Graphics;
import org.eclipse.draw2d.MouseEvent;
import org.eclipse.draw2d.MouseListener;
import org.eclipse.draw2d.MouseMotionListener;
import org.eclipse.draw2d.geometry.Point;
import org.eclipse.draw2d.geometry.Rectangle;
import org.eclipse.jface.resource.ColorRegistry;
import org.eclipse.swt.graphics.RGB;
import org.tekkotsu.ui.storyboard.ResourceRegistry;

public class TimeLine extends Figure implements PropertyChangeListener {
    public static int gap = 8, halfgap = 4, xoffset = 20, height = 25;

    public static final String TIMELINE_CURSOR_CHANGED = "_time_change";

    public static final String TIMELINE_LIMIT_CHANGED = "_time_limit";

    public static final int TIMELINE_INIT_LIMIT = 200000;

    private ColorRegistry colorReg;

    private boolean timeLock = false;

    TimeMarker marker;

    int cursor = 0;

    int timeLimit = TIMELINE_INIT_LIMIT;

    int maxValue = 0;

    double timeScale = 0.001;

    Rectangle constraint;

    public int getOffset(int time) {
        return marker.getOffset(time);
    }

    public int getTime(int offset) {
        return marker.getTime(offset);
    }

    public TimeMarker getMarker() {
        return marker;
    }

    public void setLimit(int time) {
        timeLimit = time;
        constraint = new Rectangle(0, 0, getOffset(timeLimit), TimeLine.height);
        getParent().getLayoutManager().setConstraint(this, constraint);
        firePropertyChange(TIMELINE_LIMIT_CHANGED, null, constraint);
    }

    public void setVerticalOffset(int voff) {
        Rectangle constraint = new Rectangle(0, voff, getOffset(timeLimit),
                TimeLine.height);
        getParent().getLayoutManager().setConstraint(this, constraint);
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

    /*
     * (non-Javadoc)
     * 
     * @see org.eclipse.draw2d.Figure#paintFigure(org.eclipse.draw2d.Graphics)
     */
    protected void paintFigure(Graphics gc) {
        Rectangle bounds = getBounds();

        gc.setBackgroundColor(colorReg.get("timeline_bg"));
        gc.fillRectangle(getBounds());
        gc.setForegroundColor(colorReg.get("timeline_bgdim1"));
        gc.drawLine(bounds.x, bounds.y + bounds.height - 2, bounds.x
                + bounds.width, bounds.y + bounds.height - 2);
        gc.setForegroundColor(colorReg.get("timeline_bgdim2"));
        gc.drawLine(bounds.x, bounds.y + bounds.height - 3, bounds.x
                + bounds.width, bounds.y + bounds.height - 3);
        gc.setForegroundColor(colorReg.get("timeline_bgdim3"));
        gc.drawLine(bounds.x, bounds.y + bounds.height - 4, bounds.x
                + bounds.width, bounds.y + bounds.height - 4);

        gc.setForegroundColor(colorReg.get("timeline_tick"));
        gc.drawLine(bounds.x, bounds.y, bounds.x + bounds.width, bounds.y);
        gc.drawLine(bounds.x, bounds.y + bounds.height - 1, bounds.x
                + bounds.width, bounds.y + bounds.height - 1);

        //Font sansSerifFont = new Font("Sans", Font.PLAIN, 10);
        //g2d.setFont(sansSerifFont);
        int count = 4, metric = 0, xoff;
        int x = 0;
        int cwidth = gc.getFontMetrics().getAverageCharWidth();
        for (int t = 0; x < bounds.width; t += 1000) {
            x = getOffset(t);
            gc.drawLine(bounds.x + x, bounds.y + 2, bounds.x + x, bounds.y + 4);
            gc.drawLine(bounds.x + x, bounds.y + bounds.height - 3, bounds.x
                    + x, bounds.y + bounds.height - 5);
            if (count == 4) {
                gc.setForegroundColor(colorReg.get("timeline_text"));
                xoff = x - ("" + metric).length() * cwidth / 2;
                gc.drawString(metric + "", bounds.x + xoff, bounds.y + 6);
                gc.setForegroundColor(colorReg.get("timeline_tick"));
                count = 0;
            } else
                count++;
            metric++;
        }

        NumberFormat nf = new DecimalFormat("0.###");
        nf.setMaximumFractionDigits(2);
        int lastoff = marker.getOffset(0), nextoff = marker.getOffset(5000);
        for (int i = 1; i < marker.timeVec.size(); i++) {
            int time = marker.timeVec.get(i);
            if (i < marker.offsetVec.size() - 1)
                nextoff = marker.offsetVec.get(i + 1);
            x = marker.offsetVec.get(i);

            // draw marker
            gc.setForegroundColor(colorReg.get("timeline_tick"));
            gc.drawLine(bounds.x + x, bounds.y + 2, bounds.x + x, bounds.y + 4);
            gc.drawLine(bounds.x + x, bounds.y + bounds.height - 3, bounds.x
                    + x, bounds.y + bounds.height - 5);

            // draw timer (if possible)
            String label = nf.format(time * 0.001f);
            int lwidth = cwidth * label.length() / 2;

            xoff = x - lwidth;
            int nextTick = getOffset(((time/5000)+1)*5000);
            if ((xoff - lastoff > 2) && (nextTick-lwidth>x+2)) {
                gc.setForegroundColor(colorReg.get("timeline_text"));
                gc.drawString(label, bounds.x + xoff, bounds.y + 6);
            }
            lastoff = x + lwidth;
        }

        // draw cursor
        gc.setXORMode(true);
        gc.setBackgroundColor(colorReg.get("timeline_cursor"));
        gc.fillRectangle(bounds.x + marker.getOffset(getValue()) - halfgap,
                bounds.y + 1, gap, bounds.height - 2);
        gc.setXORMode(false);
    }

    /**
     *  
     */
    public TimeLine() {
        //setBounds(new Rectangle(0,0,limit*gap,25));

        // initiate color registry
        colorReg = ResourceRegistry.getInstance().getColorRegistry();
        colorReg.put("timeline_bg", new RGB(235, 235, 228));
        colorReg.put("timeline_bgdim1", new RGB(209, 209, 203));
        colorReg.put("timeline_bgdim2", new RGB(218, 218, 211));
        colorReg.put("timeline_bgdim3", new RGB(226, 226, 220));
        colorReg.put("timeline_tick", new RGB(128, 128, 128));
        colorReg.put("timeline_text", new RGB(0, 0, 0));
        colorReg.put("timeline_cursor", new RGB(0, 128, 128));

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

        marker = new TimeMarker(xoffset, gap, (float) timeScale);
    }

    // move cursor to specific coordinate
    public void moveCursor(Point p) {
        int cursor = marker.getTime(p.x);
        if (cursor < 0)
            cursor = 0;
        setValue(cursor);
        firePropertyChange(TimeLine.TIMELINE_CURSOR_CHANGED, null, new Integer(
                this.cursor));
    }

    // set cursor to the specified time unit
    public void setValue(int value) {
        if (maxValue < value)
            maxValue = value;
        cursor = value;
        if (cursor < 0)
            cursor = 0;
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
}