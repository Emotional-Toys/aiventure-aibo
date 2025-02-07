/*
 * Created on Apr 13, 2005
 */
package org.tekkotsu.ui.storyboard;

import org.eclipse.jface.resource.ColorRegistry;
import org.eclipse.jface.resource.ImageRegistry;
import org.eclipse.swt.widgets.Display;

/**
 * 
 * @author asangpet
 */
public class ResourceRegistry {
    private ImageRegistry imgReg = null;
    private ColorRegistry colReg = null;
    private Display display = null;
    
    private static ResourceRegistry resourceReg = null;
    
    private ResourceRegistry(Display display) {
        this.display = display;
        imgReg = new ImageRegistry(display);
        colReg = new ColorRegistry(display);        
    }
    
    public static void init(Display display) {
        resourceReg = new ResourceRegistry(display);
    }
    
    public static ResourceRegistry getInstance() {
        return resourceReg; 
    }
    
    public ImageRegistry getImageRegistry() {        
        return imgReg;
    }
    
    public Display getDisplay() {     
        return display;
    }
    
    public ColorRegistry getColorRegistry() {
        return colReg;
    }
}
