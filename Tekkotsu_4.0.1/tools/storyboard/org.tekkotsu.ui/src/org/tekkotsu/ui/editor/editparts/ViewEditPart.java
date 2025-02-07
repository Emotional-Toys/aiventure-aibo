/*
 * Created on Sep 12, 2004
 */
package org.tekkotsu.ui.editor.editparts;

/**
 * @author asangpet
 */

import java.beans.PropertyChangeEvent;
import java.util.List;

import org.eclipse.draw2d.IFigure;
import org.eclipse.draw2d.Layer;
import org.eclipse.draw2d.XYLayout;
import org.eclipse.gef.EditPolicy;
import org.tekkotsu.ui.editor.editpolicies.MyXYLayoutEditPolicy;
import org.tekkotsu.ui.editor.model.ViewModel;

public class ViewEditPart extends EditPartWithListener {
	protected IFigure createFigure() {
		Layer figure = new Layer();
		figure.setLayoutManager(new XYLayout());
		return figure;
	}
	
	protected List getModelChildren() {
		return ((ViewModel)getModel()).getPartChildrenNode();
	}

	protected void createEditPolicies() {
		installEditPolicy(EditPolicy.LAYOUT_ROLE, new MyXYLayoutEditPolicy());
	}
	
	public void propertyChange(PropertyChangeEvent evt) {
		if (evt.getPropertyName().equals(ViewModel.P_CHILDREN))
			refreshChildren();
		else if (evt.getPropertyName().equals(ViewModel.P_MODELSOURCE))
			refreshVisuals();			
	}	
}
