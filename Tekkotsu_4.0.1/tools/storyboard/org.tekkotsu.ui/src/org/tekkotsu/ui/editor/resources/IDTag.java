/*
 * Created on Sep 18, 2004
 */
package org.tekkotsu.ui.editor.resources;

/**
 * @author asangpet
 */
import org.eclipse.swt.graphics.RGB;

public class IDTag {
	public static final String XML_state_tag	= "state";
	public static final String XML_state_id	= "id";
	public static final String XML_state_class	= "class";
	public static final String XML_state_label	= "label";
	public static final String XML_state_color	= "color";
	public static final String XML_state_top	= "top";
	public static final String XML_state_left	= "left";
	public static final String XML_state_width	= "width";
	public static final String XML_state_height = "height";
	public static final String XML_state_shape = "shape";

	public static final String XML_common_id	= "id";
	public static final String XML_common_class	= "class";
	public static final String XML_common_label	= "label";
	public static final String XML_common_color	= "color";
	public static final String XML_common_top	= "top";
	public static final String XML_common_left	= "left";
	public static final String XML_common_width	= "width";
	public static final String XML_common_height = "height";	
	
	public static final String XML_transition_tag	= "transition";
	public static final String XML_transition_id	= "id";
	public static final String XML_transition_source= "source";
	public static final String XML_transition_dest	= "destination";
	public static final String XML_transition_visible="visible";
	public static final String XML_transition_type	= "type";
	public static final String XML_transition_class = "class";
	public static final String XML_transition_linewidth = "linewidth";
	
	public static final String XML_view_modelsource="model";
	public static final String XML_view_tag="view";
	
	public static final String ACTION_add_state = "_ADD_STATE";
	public static final String ACTION_add_transition = "_ADD_TRANSITION";
	public static final String ACTION_add_all = "_ADD_ALL";

	/**
	 * Convert string color tag format #FFFFFF to RGB
	 * @param colortag
	 * @return
	 */
	public static RGB Tag2RGB(String colortag) {
		try {			
			int red = Integer.parseInt(colortag.substring(1,3),16);
			int green = Integer.parseInt(colortag.substring(3,5),16);
			int blue = Integer.parseInt(colortag.substring(5,7),16);
			return new RGB(red,green,blue);			
		} catch(Exception e) {
			return new RGB(255,255,255);
		}
	}
	
	private static String padZero(String str) {
		if (str.length()<2) str = "0"+str;
		return str;
	}
	public static String RGB2Tag(RGB rgb) {
		if (rgb == null) return "#FFFFFF";
		else return "#"+padZero(Integer.toHexString(rgb.red))+
				padZero(Integer.toHexString(rgb.green))+
				padZero(Integer.toHexString(rgb.blue));
	}
}
