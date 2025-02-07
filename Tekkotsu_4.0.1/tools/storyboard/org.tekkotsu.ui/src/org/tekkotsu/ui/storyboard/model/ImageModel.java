package org.tekkotsu.ui.storyboard.model;

import java.io.ByteArrayInputStream;
import java.io.FileOutputStream;

import org.eclipse.core.runtime.IPath;
import org.eclipse.swt.SWT;
import org.eclipse.swt.graphics.ImageData;
import org.eclipse.swt.graphics.ImageLoader;
import org.tekkotsu.ui.storyboard.Base64;

public class ImageModel extends AbstractModel {
	String icon="bookmark.gif";
	String content = "";
	String etid = "";
	public static final String ETID_ACTIVATE = "A";
	public static final String ETID_DEACTIVATE = "D";
	public static final String ETID_STATUS = "S";	    
    int layoutIndex = 0;
    ImageData imData;

	public ImageModel(String sid, int start, String content) {
		super(sid,start,-1, true);		
		if (content!=null) this.content = content;

		ImageLoader imLoad = new ImageLoader();
        ByteArrayInputStream is = new ByteArrayInputStream(Base64.decode(content)); 
        imLoad.load(is);
        imData = imLoad.data[0];        
	}
	
	public int getTime() {
		return start;
	}
	
	public ImageData getImageData() {
	    return imData;
	}
	
	public String getIconName() {
		return icon;
	}
	
	public String getContent() {
		return content;
	}
	
	public String getETID() {
	    return etid;
	}
	
	public String getETIDString() {
	    if (ETID_ACTIVATE.equals(etid)) return "Activate";
	    if (ETID_DEACTIVATE.equals(etid)) return "Deactivate";
	    if (ETID_STATUS.equals(etid)) return "Status";
	    return "Unknown";
	}
	
    public int getLayoutIndex() {
        return layoutIndex;
    }
    
    public void setLayoutIndex(int layoutIndex) {
        this.layoutIndex = layoutIndex;
    }
    
    public String getImageName() {
        return "img_"+getID()+getTime();
    }
    
    public boolean saveTo(IPath path) {        
        ImageLoader imLoad = new ImageLoader();
        imLoad.data = new ImageData[] {imData};
        try {
            FileOutputStream os = new FileOutputStream(path.toFile());
            imLoad.save(os,SWT.IMAGE_JPEG);
            os.close();
        } catch (Exception e) {
            return false;
        }
        return true;
    }
}
