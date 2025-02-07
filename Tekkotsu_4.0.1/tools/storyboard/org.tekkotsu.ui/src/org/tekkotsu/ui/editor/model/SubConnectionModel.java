/*
 * Created on Oct 26, 2004
 */
package org.tekkotsu.ui.editor.model;

import org.eclipse.swt.graphics.Color;
import org.jdom.Content;

/**
 * @author asangpet
 *
 */
public class SubConnectionModel extends AbstractConnectionModel {
	public static final String P_ARROWHEAD = "_arrowhead";
	public static final String P_LINEWIDTH = "_linewidth";
	public static final String ARROW_ARROWHEAD = "_arrow_head";
	public static final String ARROW_NOHEAD = "_arrow_nohead";	
	
	private String arrowType;
	private int lineWidth;
	
	
	public SubConnectionModel(AbstractModel parent, StateNodeModel source, StateNodeModel dest) {
		this(parent ,source, dest, ARROW_ARROWHEAD, 1);		
	}

	public SubConnectionModel(AbstractModel parent, StateNodeModel source, StateNodeModel dest, int lineWidth) {
		this(parent ,source, dest, ARROW_ARROWHEAD, lineWidth);		
	}

	public SubConnectionModel(AbstractModel parent, StateNodeModel source, StateNodeModel dest, String arrowType, int lineWidth) {
		setParent(parent);
		this.arrowType = arrowType;
		this.lineWidth = lineWidth;
		setSource(source);
		setTarget(dest);
		attachSource();
		attachTarget();
	}
	
	/* (non-Javadoc)
	 * @see org.tekkotsu.ui.editor.model.AbstractModel#getXML()
	 */
	public Content getXML() {
		return null;
	}

	/**
	 * @return Returns the arrowType.
	 */
	public String getArrowType() {
		return arrowType;
	}
	/**
	 * @param arrowType The arrowType to set.
	 */
	public void setArrowType(String arrowType) {
		this.arrowType = arrowType;
		firePropertyChange(P_ARROWHEAD, null, arrowType);
	}
	
	public void setLineWidth(int width) {
		lineWidth = width;
		firePropertyChange(P_LINEWIDTH, null, new Integer(width));
	}
	
	public int getLineWidth() {
		return lineWidth;
	}
	
	public String toString() {
		//return "SubConnectionModel<"+this.getSource()+","+this.getTarget()+">";
		return this.getSource()+"->"+this.getTarget();
	}
	
	public void setColor(Color color) {
		firePropertyChange(MultiTransitionModel.P_COLOR,null,color);
	}
}
