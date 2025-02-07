/*
 * Created on Sep 18, 2004
 */
package org.tekkotsu.ui.editor.editparts;

import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;
import java.util.ArrayList;
import java.util.List;

import org.eclipse.draw2d.AbsoluteBendpoint;
import org.eclipse.draw2d.geometry.Point;
import org.eclipse.gef.EditPolicy;
import org.eclipse.gef.editparts.AbstractConnectionEditPart;
import org.tekkotsu.ui.editor.editpolicies.MyBendpointEditPolicy;
import org.tekkotsu.ui.editor.editpolicies.MyConnectionEditPolicy;
import org.tekkotsu.ui.editor.editpolicies.MyConnectionEndpointEditPolicy;
import org.tekkotsu.ui.editor.model.AbstractConnectionModel;

/**
 * @author asangpet
 */
public abstract class MyAbstractConnectionEditPart extends
		AbstractConnectionEditPart implements PropertyChangeListener {

	/*
	 * (non-Javadoc)
	 * 
	 * @see org.eclipse.gef.editparts.AbstractEditPart#createEditPolicies()
	 */
	protected void createEditPolicies() {
		installEditPolicy(EditPolicy.CONNECTION_ROLE,
				new MyConnectionEditPolicy());
		installEditPolicy(EditPolicy.CONNECTION_ENDPOINTS_ROLE,
				new MyConnectionEndpointEditPolicy());
		installEditPolicy(EditPolicy.CONNECTION_BENDPOINTS_ROLE,
				new MyBendpointEditPolicy());
	}

	public void activate() {
		super.activate();
		((AbstractConnectionModel) getModel()).addPropertyChangeListener(this);
	}

	public void deactivate() {
		super.deactivate();
		((AbstractConnectionModel) getModel())
				.removePropertyChangeListener(this);
	}

	public void propertyChange(PropertyChangeEvent evt) {
		if (evt.getPropertyName().equals(AbstractConnectionModel.P_BEND_POINT))
			refreshBendpoints();
	}

	protected void refreshBendpoints() {
		List bendpoints = ((AbstractConnectionModel) getModel())
				.getBendpoints();
		List constraint = new ArrayList();

		for (int i = 0; i < bendpoints.size(); i++) {
			constraint.add(new AbsoluteBendpoint((Point) bendpoints.get(i)));
		}
		getConnectionFigure().setRoutingConstraint(constraint);
	}

	protected void refreshVisuals() {
		refreshBendpoints();
	}
}
