/*
 * Created on Sep 12, 2004
 */
package org.tekkotsu.ui.editor.editparts;

import java.beans.PropertyChangeListener;

import org.eclipse.gef.editparts.AbstractGraphicalEditPart;
import org.tekkotsu.ui.editor.model.AbstractModel;

/**
 * EditPart component extension with properties listener.
 * @author asangpet
 */
abstract public class EditPartWithListener
	extends AbstractGraphicalEditPart 
	implements PropertyChangeListener {
	
	public void activate() {
		super.activate();
		((AbstractModel)getModel()).addPropertyChangeListener(this);
	}
	
	public void deactivate() {
		super.deactivate();
		((AbstractModel)getModel()).removePropertyChangeListener(this);		
	}
}
