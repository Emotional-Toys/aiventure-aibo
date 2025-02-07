package org.tekkotsu.ui.storyboard.model;

public class LogModel extends AbstractModel {
	String icon="bookmark.gif";
	String content = "";
	String etid = "";
	public static final String ETID_ACTIVATE = "A";
	public static final String ETID_DEACTIVATE = "D";
	public static final String ETID_STATUS = "S";	    
    int layoutIndex = 0;

	public LogModel(String sid, int start, String iconName, String etid, String content) {
		super(sid,start,-1, true);
		if (iconName!=null) icon = iconName;
		if (content!=null) this.content = content;
		if (etid!=null) this.etid = etid; else this.etid = ETID_ACTIVATE;
	}
	
	public int getTime() {
		return start;
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
}
