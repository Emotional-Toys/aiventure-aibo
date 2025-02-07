package org.tekkotsu.ui.storyboard.components;

import java.util.List;

import org.eclipse.draw2d.ColorConstants;
import org.eclipse.draw2d.Figure;
import org.eclipse.draw2d.Graphics;
import org.eclipse.draw2d.IFigure;
import org.eclipse.draw2d.Label;
import org.eclipse.draw2d.geometry.Point;
import org.eclipse.draw2d.geometry.Rectangle;
import org.eclipse.jface.resource.ImageDescriptor;
import org.eclipse.jface.resource.ImageRegistry;
import org.eclipse.swt.graphics.ImageData;
import org.tekkotsu.ui.editor.model.StateNodeModel;
import org.tekkotsu.ui.storyboard.ResourceRegistry;
import org.tekkotsu.ui.storyboard.model.LogModel;

/**
 * @author asangpet
 *  
 */
public class LogObject extends Figure
{
    boolean active = false;

    ImageRegistry imgReg;

    Rectangle constraint;

    LogModel model;

    StateNodeModel state;

    TimeLine timeline;

    org.tekkotsu.ui.editor.model.TransitionModel etrans;

    List nodeList;

    int offset = TimeLine.xoffset;

    int metric = TimeLine.gap;

    public LogObject(LogModel model, StateNodeModel state,
            TimeLine timeLine)
    {
        imgReg = ResourceRegistry.getInstance().getImageRegistry();
        this.state = state;
        this.timeline = timeLine;
        setModel(model);
        this.setToolTip(getToolTipFigure());
    }

    private IFigure getToolTipFigure() {
        Label label = new Label("Time:" + model.getTime() * .001 + "s\n"
                + model.getETIDString()+ "\n"+ model.getContent());
        if (model.getETID().equals(LogModel.ETID_ACTIVATE)) {
            label.setForegroundColor(ColorConstants.darkGreen);
        } else
        if (model.getETID().equals(LogModel.ETID_DEACTIVATE)) {
            label.setForegroundColor(ColorConstants.red);
        } else if (model.getETID().equals(LogModel.ETID_STATUS)) {
            label.setForegroundColor(ColorConstants.blue);
        }
        return label;
    }
    
    private void setModel(LogModel model)
    {
        this.model = model;
        Rectangle bound = null;
        if (state == null)
            bound = new Rectangle(0, 6, 10, TimeLine.height);
        else
            bound = state.getConstraint();
        
        int xoff = timeline.getOffset(model.getStart());
        try {
            imgReg.put(model.getIconName(), ImageDescriptor.createFromFile(
                org.tekkotsu.ui.storyboard.icons.IconDummy.class, model
                        .getIconName()));
        } catch (IllegalArgumentException ex) {}
        ImageData data = imgReg.getDescriptor(model.getIconName())
                .getImageData();
        setConstraint(new Rectangle(xoff-data.width/2, bound.y
                + (bound.height - data.height) / 2, data.width, data.height));
        //setConstraint(new Rectangle(xoff,bound.y+bound.height/2,16,16));
    }

    private void setConstraint(Rectangle r)
    {
        constraint = r;
    }

    public Rectangle getConstraint()
    {
        return constraint;
    }

    public void setActive(boolean b)
    {
    }

    protected void paintFigure(Graphics g2d)
    {
        Rectangle clArea = getClientArea();
        Rectangle bound = getConstraint();

        g2d.drawImage(imgReg.get(model.getIconName()), new Point(clArea.x,
                clArea.y));
        
        if (model.getETID().equals(LogModel.ETID_DEACTIVATE)) {
            g2d.setForegroundColor(ColorConstants.red);
            g2d.setLineWidth(3);
            g2d.drawLine(clArea.x,clArea.y,clArea.x+bound.width, clArea.y+bound.height);            
        } else if (model.getETID().equals(LogModel.ETID_STATUS)) {
            g2d.setForegroundColor(ColorConstants.blue);
            g2d.setLineWidth(3);
            g2d.drawLine(clArea.x,clArea.y+bound.height-2,clArea.x+bound.width, clArea.y+bound.height-2);                    
        }
    }

    /**
     * @return Returns the model.
     */
    public LogModel getModel()
    {
        return model;
    }
    
    /**
     * Set layout index for this log object
     * @param idx position (1-top,2-left,3-right)
     */
    public void setLayoutIndex(int idx) {
        if (model.getLayoutIndex() == 0) model.setLayoutIndex(idx);
        int xoff = timeline.getOffset(model.getStart());
        idx = model.getLayoutIndex();
        Rectangle bound = getConstraint();
        if (idx==1) {            
            bound.y=2;
            bound.x=xoff-8;
        } else if (idx==2) {
            bound.y=10;
            bound.x=xoff-12;
        } else if (idx==3) {
            bound.y=10;
            bound.x=xoff-4;
        } else {
            bound.y=LogBar.height;
        }
        setConstraint(new Rectangle(bound)); 
    }
}