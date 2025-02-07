/*
 * Created on Nov 7, 2004
 */
package org.tekkotsu.ui.storyboard.views;

import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;
import java.beans.PropertyChangeSupport;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

import org.eclipse.jface.viewers.DoubleClickEvent;
import org.eclipse.jface.viewers.IDoubleClickListener;
import org.eclipse.jface.viewers.ISelection;
import org.eclipse.jface.viewers.StructuredSelection;
import org.eclipse.jface.viewers.Viewer;
import org.eclipse.swt.SWT;
import org.eclipse.swt.custom.ScrolledComposite;
import org.eclipse.swt.events.MouseAdapter;
import org.eclipse.swt.events.MouseEvent;
import org.eclipse.swt.events.PaintEvent;
import org.eclipse.swt.events.PaintListener;
import org.eclipse.swt.graphics.Image;
import org.eclipse.swt.graphics.ImageData;
import org.eclipse.swt.graphics.Point;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Canvas;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Control;
import org.eclipse.swt.widgets.Display;
import org.tekkotsu.ui.editor.model.ViewModel;
import org.tekkotsu.ui.storyboard.ResourceRegistry;
import org.tekkotsu.ui.storyboard.model.ImageModel;

/**
 * @author asangpet
 *  
 */
public class ImageViewer extends Viewer implements PropertyChangeListener {
    Composite parent;

    private PropertyChangeSupport listeners = new PropertyChangeSupport(this);

    private ArrayList listenobjs = new ArrayList();

    ScrolledComposite control = null;

    ViewModel viewModel;

    ImageView.ViewContentProvider content;

    ISelection curSelect = null;
    Canvas canvas = null;
    Display display;
    ImageData imageData = null;
    Image image = null;
    ImageModel imageModel = null;

    List doubleClickListeners = null;
    
    public ImageViewer(Composite parent) {
        this.parent = parent;
        doubleClickListeners = new ArrayList();
        try {
        control = new ScrolledComposite(parent,SWT.H_SCROLL | SWT.V_SCROLL);
        canvas = new Canvas(control,SWT.FILL);
        if (imageData!=null) {            
        	canvas.setSize(imageData.width,imageData.height);
        } else canvas.setSize(200,200);
        canvas.addPaintListener(new PaintListener() {
            public void paintControl(PaintEvent e) {
                if (imageData!=null) {
                    canvas.setSize(imageData.width,imageData.height);
                    e.gc.drawImage(image,0,0);
                }
                else e.gc.fillRectangle(0,0,control.getSize().x,control.getSize().y);
            }
        });        
        canvas.addMouseListener(new MouseAdapter() {             
            /* (non-Javadoc)
             * @see org.eclipse.swt.events.MouseAdapter#mouseDoubleClick(org.eclipse.swt.events.MouseEvent)
             */
            public void mouseDoubleClick(MouseEvent e) {
                for (Iterator iter=doubleClickListeners.iterator();iter.hasNext();) {
                    try {
                    IDoubleClickListener listener = (IDoubleClickListener)iter.next();
                    listener.doubleClick(new DoubleClickEvent(ImageViewer.this,new StructuredSelection()));
                    } catch (Exception ex) {ex.printStackTrace();}
                }
            }
        });        
        
        control.setContent(canvas);
        GridLayout layout = new GridLayout();
        layout.numColumns = 1;
        parent.setLayout(layout);
        GridData gd = new GridData();
        gd.horizontalAlignment = SWT.FILL;
        gd.verticalAlignment = SWT.FILL;
        gd.grabExcessVerticalSpace = true;
        gd.grabExcessHorizontalSpace = true;
        control.setLayoutData(gd);

        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    /*
     * (non-Javadoc)
     * 
     * @see java.beans.PropertyChangeListener#propertyChange(java.beans.PropertyChangeEvent)
     */
    public void propertyChange(PropertyChangeEvent evt) {
    }

    /*
     * (non-Javadoc)
     * 
     * @see org.eclipse.jface.viewers.Viewer#getControl()
     */
    public Control getControl() {
        return control;
    }

    public void setContent(ImageView.ViewContentProvider content) {
        this.content = content;
        if (content.getDefaultModel() != imageModel) {
            imageModel = content.getDefaultModel();
            imageData = content.getDefaultModel().getImageData();               
            if (image!=null) image.dispose();
            Point size = new Point(control.getSize().x,control.getSize().y);
            if (size.x>size.y) size.x = imageData.width*size.y/imageData.height;
            else size.y = imageData.height*size.x/imageData.width;
            imageData = imageData.scaledTo(size.x,size.y);
            image = new Image(ResourceRegistry.getInstance().getDisplay(),imageData);
            canvas.redraw();
        }
    }
    
    public void addDoubleClickListener(IDoubleClickListener listener) {
        doubleClickListeners.add(listener);
    }

    /*
     * (non-Javadoc)
     * 
     * @see org.eclipse.jface.viewers.IInputProvider#getInput()
     */
    public Object getInput() {
        return null;
    }

    /*
     * (non-Javadoc)
     * 
     * @see org.eclipse.jface.viewers.Viewer#setSelection(org.eclipse.jface.viewers.ISelection,
     *      boolean)
     */
    public void setSelection(ISelection selection, boolean reveal) {
    }

    /*
     * (non-Javadoc)
     * 
     * @see org.eclipse.jface.viewers.ISelectionProvider#getSelection()
     */
    public ISelection getSelection() {
        return null;
    }
    
    

    /*
     * (non-Javadoc)
     * 
     * @see org.eclipse.jface.viewers.Viewer#refresh()
     */
    public void refresh() {
        //storyboard.repaint();
    }

    /*
     * (non-Javadoc)
     * 
     * @see org.eclipse.jface.viewers.Viewer#setInput(java.lang.Object)
     */
    public void setInput(Object input) {
        setInput(input, true);
    }

    public void setInput(Object input, boolean rebuild) {
    }

    public void addPropertyChangeListener(PropertyChangeListener listener) {
        listeners.addPropertyChangeListener(listener);
        listenobjs.add(listener);
    }

    public void firePropertyChange(String propName, Object oldValue,
            Object newValue) {
        listeners.firePropertyChange(propName, oldValue, newValue);
    }

    public void removePropertyChangeListener(PropertyChangeListener listener) {
        listeners.removePropertyChangeListener(listener);
        listenobjs.remove(listener);
    }

    public ImageModel getImageModel() {
        return imageModel;
    }
}
