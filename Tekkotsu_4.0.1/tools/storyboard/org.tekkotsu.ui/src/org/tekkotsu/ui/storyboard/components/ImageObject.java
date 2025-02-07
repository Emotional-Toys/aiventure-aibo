package org.tekkotsu.ui.storyboard.components;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.FileWriter;

import org.eclipse.draw2d.Figure;
import org.eclipse.draw2d.Graphics;
import org.eclipse.draw2d.IFigure;
import org.eclipse.draw2d.Label;
import org.eclipse.draw2d.MouseEvent;
import org.eclipse.draw2d.MouseListener;
import org.eclipse.draw2d.geometry.Point;
import org.eclipse.draw2d.geometry.Rectangle;
import org.eclipse.jface.resource.ImageDescriptor;
import org.eclipse.jface.resource.ImageRegistry;
import org.eclipse.swt.SWT;
import org.eclipse.swt.graphics.Image;
import org.eclipse.swt.graphics.ImageData;
import org.eclipse.swt.graphics.ImageLoader;
import org.tekkotsu.ui.editor.model.StateNodeModel;
import org.tekkotsu.ui.storyboard.ResourceRegistry;
import org.tekkotsu.ui.storyboard.model.ImageModel;
import org.tekkotsu.ui.storyboard.Base64;


/**
 * @author asangpet
 *  
 */
public class ImageObject extends Figure
{
    boolean active = false;
    
    ImageRegistry imgReg;

    Rectangle constraint;

    ImageModel model;

    StateNodeModel state;

    TimeLine timeline;

    org.tekkotsu.ui.editor.model.TransitionModel etrans;

    int offset = TimeLine.xoffset;

    int metric = TimeLine.gap;

    public ImageObject(ImageModel model, StateNodeModel state,
            TimeLine timeLine)
    {
        imgReg = ResourceRegistry.getInstance().getImageRegistry();
        this.state = state;
        this.timeline = timeLine;
        setModel(model);
        this.setToolTip(getToolTipFigure());
        
        addMouseListener(new MouseListener() {
            /* (non-Javadoc)
             * @see org.eclipse.draw2d.MouseListener#mouseDoubleClicked(org.eclipse.draw2d.MouseEvent)
             */
            public void mouseDoubleClicked(MouseEvent me) {                
                ImageDialog imgDialog = new ImageDialog(ResourceRegistry.getInstance().getDisplay().getActiveShell(),imgReg.get(getImageName()));
                imgDialog.setTitle("Image: "+getImageName());
                imgDialog.open();
            }
            
            /* (non-Javadoc)
             * @see org.eclipse.draw2d.MouseListener#mousePressed(org.eclipse.draw2d.MouseEvent)
             */
            public void mousePressed(MouseEvent me) {
            }
            
            /* (non-Javadoc)
             * @see org.eclipse.draw2d.MouseListener#mouseReleased(org.eclipse.draw2d.MouseEvent)
             */
            public void mouseReleased(MouseEvent me) {
            }
        });
    }

    private IFigure getToolTipFigure() {
        Label label = new Label("Record at:" + model.getTime() * .001 + "s");        
        return label;
    }

    private String getImageName() {
        return model.getImageName();
    }
    
    private String getThumbImageName() {
        return "thumb_"+model.getID()+model.getTime();
    }
    
    private void setModel(ImageModel model)
    {
        this.model = model;
        Rectangle bound = null;
        if (state == null)
            bound = new Rectangle(0, 6, 10, TimeLine.height);
        else
            bound = state.getConstraint();
        
        int xoff = timeline.getOffset(model.getStart());
        try {            
            final Image image = new Image(ResourceRegistry.getInstance().getDisplay(),model.getImageData());
            ImageData tb = model.getImageData();
            tb.alpha = 100;
            final Image thumb = new Image(ResourceRegistry.getInstance().getDisplay(),tb.scaledTo(48,48));//bound.width,bound.height));
            tb.alpha = -1;
            
            imgReg.put(getImageName(), image);
            imgReg.put(getThumbImageName(), thumb);
        } catch (IllegalArgumentException ex) {}
        setConstraint(new Rectangle(xoff/*-data.width/2*/, bound.y
                /*+ (bound.height - data.height) / 2 */, 48,48));//data.width, data.height));
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
        
        Image img = imgReg.get(getThumbImageName());
        if (img!=null)
            g2d.drawImage(imgReg.get(getThumbImageName()), new Point(clArea.x,
                    clArea.y));
    }

    /**
     * @return Returns the model.
     */
    public ImageModel getModel()
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
    
    public static void test() {
        try {
        System.out.println("getting img");
        ImageDescriptor desc = ImageDescriptor.createFromFile(
                org.tekkotsu.ui.storyboard.icons.IconDummy.class, "vision.jpg");
        
        Image img = desc.createImage();        
        ImageData imData = img.getImageData();
        System.out.println("init image "+imData.width+" "+imData.height);
        ImageLoader imLoad = new ImageLoader();
        imLoad.data = new ImageData[] {imData};
        ByteArrayOutputStream os = new ByteArrayOutputStream();
        System.out.println("save to outstream");
        imLoad.save(os,SWT.IMAGE_JPEG);
        img.dispose();
        
        System.out.println("b64");        
        String b64 = (Base64.encode(os.toByteArray()));
        System.out.println("length:"+b64.length()+"\n");
        FileWriter fw = new FileWriter("test.b64");        
        fw.write(b64);
        fw.close();
        imLoad = new ImageLoader();
        ByteArrayInputStream is = new ByteArrayInputStream(Base64.decode(b64));
        imLoad.load(is);
        imLoad.save("aug.jpg",SWT.IMAGE_JPEG);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}