/*
 * Created on Oct 26, 2004
 */
package org.tekkotsu.ui.editor.editparts;

import java.beans.PropertyChangeEvent;

import org.eclipse.draw2d.BendpointConnectionRouter;
import org.eclipse.draw2d.ColorConstants;
import org.eclipse.draw2d.IFigure;
import org.eclipse.draw2d.PolygonDecoration;
import org.eclipse.swt.graphics.Color;
import org.eclipse.ui.views.properties.IPropertyDescriptor;
import org.eclipse.ui.views.properties.IPropertySource;
import org.tekkotsu.ui.editor.BezierCurveConnection;
import org.tekkotsu.ui.editor.TransitionConnection;
import org.tekkotsu.ui.editor.model.MultiTransitionModel;
import org.tekkotsu.ui.editor.model.SubConnectionModel;

/**
 * @author asangpet
 *
 */
public class SubConnectionEditPart extends MyAbstractConnectionEditPart implements IPropertySource {
	//private BezierCurveConnection connection; // transition figure
	private TransitionConnection connection; // transition figure
	private PolygonDecoration arrowHead;
	private Color color = null;
	
	protected IFigure createFigure() {		
		SubConnectionModel model = (SubConnectionModel)this.getModel();
		connection = new BezierCurveConnection();
		//connection = new MyPolylineConnection();
		connection.setConnectionRouter(new BendpointConnectionRouter());
		connection.setLineWidth(model.getLineWidth());
		if (SubConnectionModel.ARROW_ARROWHEAD.equals(model.getArrowType())) {
			arrowHead = new PolygonDecoration();		
			connection.setTargetDecoration(arrowHead);
		}
		
		if (color==null) {
			if (((SubConnectionModel)getModel()).isPreview()) {
				connection.setForegroundColor(ColorConstants.lightGray);
			} else {
				connection.setForegroundColor(ColorConstants.black);
			}
		} else connection.setForegroundColor(color);
		
		return connection;
	}
	
	/* (non-Javadoc)
	 * @see java.beans.PropertyChangeListener#propertyChange(java.beans.PropertyChangeEvent)
	 */
	public void propertyChange(PropertyChangeEvent evt) {		
		if (evt.getPropertyName().equals(SubConnectionModel.P_ARROWHEAD)) {
			Object newVal = evt.getNewValue();
			if (newVal.equals(SubConnectionModel.ARROW_NOHEAD)) {
				connection.setTargetDecoration(null);
			} else if (newVal.equals(SubConnectionModel.ARROW_ARROWHEAD)){
				arrowHead = new PolygonDecoration();
				connection.setTargetDecoration(arrowHead);
			}
		} else if (evt.getPropertyName().equals(SubConnectionModel.P_LINEWIDTH)) {
			SubConnectionModel model = (SubConnectionModel)getModel();
			connection.setLineWidth(model.getLineWidth());
		} else if (evt.getPropertyName().equals(MultiTransitionModel.P_COLOR)) {
			connection.setForegroundColor((Color)evt.getNewValue());
			color = (Color)evt.getNewValue();
		} else {
			super.propertyChange(evt);
		}
	}
	
	public Object getEditableValue() {
		return ((SubConnectionModel)getModel()).getParent().getEditableValue();
	}
	
	public IPropertyDescriptor[] getPropertyDescriptors() {
		return ((SubConnectionModel)getModel()).getParent().getPropertyDescriptors();		
	}
	
	public Object getPropertyValue(Object id) {
		return ((SubConnectionModel)getModel()).getParent().getPropertyValue(id);
	}
	
	public boolean isPropertySet(Object id) {
		return ((SubConnectionModel)getModel()).getParent().isPropertySet(id);		
	}
	
	public void resetPropertyValue(Object id) {
		((SubConnectionModel)getModel()).getParent().resetPropertyValue(id);
	}
	public void setPropertyValue(Object id, Object value) {
		((SubConnectionModel)getModel()).getParent().setPropertyValue(id,value);
	}


}
