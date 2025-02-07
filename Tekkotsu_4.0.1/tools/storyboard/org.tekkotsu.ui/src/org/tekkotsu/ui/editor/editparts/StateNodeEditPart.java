package org.tekkotsu.ui.editor.editparts;

/**
 * @author asangpet
 */

import java.beans.PropertyChangeEvent;
import java.util.List;

import org.eclipse.draw2d.ChopboxAnchor;
import org.eclipse.draw2d.ColorConstants;
import org.eclipse.draw2d.ConnectionAnchor;
import org.eclipse.draw2d.Ellipse;
import org.eclipse.draw2d.*;
import org.eclipse.draw2d.Figure;
import org.eclipse.draw2d.IFigure;
import org.eclipse.draw2d.Label;
import org.eclipse.draw2d.RectangleFigure;
import org.eclipse.draw2d.RoundedRectangle;
import org.eclipse.draw2d.geometry.Rectangle;
import org.eclipse.gef.ConnectionEditPart;
import org.eclipse.gef.EditPolicy;
import org.eclipse.gef.GraphicalEditPart;
import org.eclipse.gef.NodeEditPart;
import org.eclipse.gef.Request;
import org.eclipse.swt.graphics.Color;
import org.eclipse.swt.graphics.RGB;
import org.tekkotsu.ui.editor.editpolicies.MyComponentEditPolicy;
import org.tekkotsu.ui.editor.model.*;
import org.tekkotsu.ui.editor.resources.ConvexPolygon;
import org.tekkotsu.ui.editor.resources.Debugger;

public class StateNodeEditPart 
	extends EditPartWithListener 
	implements NodeEditPart {

	protected IFigure shape, holder;
	protected Label label;
	boolean isSrc = true;

	private ConnectionAnchor getAnchor() {
		StateNodeModel model = (StateNodeModel)getModel();
		if (model.getShape().equals(StateNodeModel.P_SHAPE_STYLE_ELLIPSE)) {
			return new EllipseAnchor(getFigure());
		} else return new ChopboxAnchor(getFigure());
	}
	
	public ConnectionAnchor getSourceConnectionAnchor(ConnectionEditPart connection) {
		return getAnchor();
	}
	
	public ConnectionAnchor getTargetConnectionAnchor(ConnectionEditPart connection) {
		return getAnchor();
	}
	
	public ConnectionAnchor getSourceConnectionAnchor(Request request) {
		return getAnchor();
	}
	
	public ConnectionAnchor getTargetConnectionAnchor(Request request) {
		return getAnchor();
	}
	
	public void propertyChange(PropertyChangeEvent evt) {
		if (evt.getPropertyName().equals(StateNodeModel.P_ACTIVATE)) {
			try {
				shape.setBackgroundColor(((StateNodeModel)getModel()).getColor());
				setLabelColor(true);
			} catch (Exception e) {
				e.printStackTrace();
			}
		} else if (evt.getPropertyName().equals(StateNodeModel.P_DEACTIVATE)) {
			try {
				shape.setBackgroundColor(((StateNodeModel)getModel()).getPreviewColor());
				setLabelColor(false);
			} catch (Exception e) {
				
			}			
		} else if (evt.getPropertyName().equals(StateNodeModel.P_CONSTRAINT)) {
			Debugger.printDebug(Debugger.DEBUG_ALL, this.toString()+"\n\tP_CONSTRAINT change to "+evt.getNewValue());			
			refreshVisuals();
		} else if (evt.getPropertyName().equals(StateNodeModel.P_LABEL)) {
			label.setText((String)evt.getNewValue());
		} else if (evt.getPropertyName().equals(StateNodeModel.P_COLOR)) {
			shape.setBackgroundColor((Color)evt.getNewValue());
			setLabelColor();
		} else if (evt.getPropertyName().equals(StateNodeModel.P_SOURCE_CONNECTION)) {
			refreshSourceConnections();
		} else if (evt.getPropertyName().equals(StateNodeModel.P_TARGET_CONNECTION)) {
			refreshTargetConnections();
		} else if (evt.getPropertyName().equals(StateNodeModel.P_SHAPE)) {
			resetShape(evt.getNewValue().toString());
		} else if (evt.getPropertyName().equals(ViewModel.P_SOURCE_EXIST)) {
			isSrc = ((Boolean)evt.getNewValue()).booleanValue();
			setPreviewColor();
		} 
	}

	private void setPreviewColor() {
		StateNodeModel model = (StateNodeModel)getModel();		
		if (!isSrc) {
			shape.setBackgroundColor(model.getPreviewColor());
			shape.setForegroundColor(ColorConstants.lightGray);
		} else {
			shape.setBackgroundColor(model.getColor());
			shape.setForegroundColor(ColorConstants.black);			
		}
		
		setLabelColor();
	}
	
	protected IFigure createCustomFigure(IFigure shapeFigure) {
		StateNodeModel model = (StateNodeModel)getModel();
		holder = new Figure();
		
		shape = shapeFigure;
		if (((ViewModel)(getParent().getModel())).isPreview() || (!isSrc)) {
			shape.setBackgroundColor(model.getPreviewColor());
			shape.setBounds(model.getConstraint());			
		} else {
			shape.setBackgroundColor(model.getColor());
			shape.setBounds(model.getConstraint());			
		}
		
		label = new Label();
		label.setText(model.getLabel());
		label.setBounds(model.getConstraint());		
		setLabelColor();
		
		holder.add(shape);
		holder.add(label);
		holder.setBounds(model.getConstraint());		
		return holder;		
	}
	
	private void resetShape(String shapeType) {
		IFigure shapeFigure = null;
		if (shapeType.equals(StateNodeModel.P_SHAPE_STYLE_RECTANGLE)) {
			shapeFigure = new RectangleFigure();
		} else if (shapeType.equals(StateNodeModel.P_SHAPE_STYLE_ELLIPSE)){
			shapeFigure = new Ellipse();
		} else if (shapeType.equals(StateNodeModel.P_SHAPE_STYLE_ROUNDED_RECTANGLE)) {
			shapeFigure = new RoundedRectangle();
		} else if (shapeType.equals(StateNodeModel.P_SHAPE_STYLE_HEXAGON)) {
			shapeFigure = new ConvexPolygon();
		}
		shapeFigure.setBackgroundColor(shape.getBackgroundColor());
		shapeFigure.setBounds(shape.getBounds());
				
		if (((ViewModel)(getParent().getModel())).isPreview() || !isSrc) {
			((Shape)shapeFigure).setForegroundColor(ColorConstants.lightGray);			
		} else {
			((Shape)shapeFigure).setForegroundColor(ColorConstants.black);			
		}
		
		holder.remove(shape);
		holder.remove(label);
		shape = shapeFigure;
		holder.add(shape);
		holder.add(label);
	}
	
	protected IFigure createFigure() {
		createCustomFigure(new RectangleFigure());
		resetShape(((StateNodeModel)getModel()).getShape());
		return holder;
	}
	
	/**
	 * Adjust label color according to background color of the figure
	 */
	protected void setLabelColor(boolean isActive) {
		try {
			RGB bgcolor = (shape.getBackgroundColor().getRGB());
			if (bgcolor.green+bgcolor.red-bgcolor.blue < 250) label.setForegroundColor(ColorConstants.white);
			else {
			    // TODO check this again
				
			    if (!isActive) {// || !isSrc) {
					label.setForegroundColor(ColorConstants.lightGray);
				} else {
					label.setForegroundColor(ColorConstants.black);
				}
			}
		} catch (Exception e) {
			label.setForegroundColor(ColorConstants.black);
		}
	}
	
	protected void setLabelColor() {
	    //setLabelColor(((StateNodeModel)getModel()).getParent()).isPreview());
	    setLabelColor(!(((ViewModel)(((StateNodeModel)getModel())).getParent())).isPreview());
	}
	
	protected void createEditPolicies() {
		installEditPolicy(EditPolicy.COMPONENT_ROLE, new MyComponentEditPolicy());
		//installEditPolicy(EditPolicy.GRAPHICAL_NODE_ROLE, new MyGraphicalNodeEditPolicy());
	}
	
	protected void refreshVisuals() {
		Rectangle constraint = ((StateNodeModel)getModel()).getConstraint();
		((GraphicalEditPart)getParent()).setLayoutConstraint(this, getFigure(), constraint);
		holder.setBounds(constraint);
		shape.setBounds(constraint);
		label.setBounds(constraint);				
	}
	
	protected List getModelSourceConnections() {
		return ((StateNodeModel)getModel()).getModelSourceConnections();
	}
	
	protected List getModelTargetConnections() {
		return ((StateNodeModel)getModel()).getModelTargetConnections();
	}
	
	public String toString() {
		String result = getClass().getName().toString() + ":" + ((StateNodeModel)getModel()).getId() + 
			":\n\tsrc:" + getSourceConnections() + 
			":\n\tdest:" + getTargetConnections();			
		return result;
	}
}
