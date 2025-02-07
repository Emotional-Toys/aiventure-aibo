/*
 * Created on Sep 11, 2004
 */
package org.tekkotsu.ui.editor.editparts;

/**
 * @author asangpet
 *
 */
import org.eclipse.gef.EditPart;
import org.eclipse.gef.EditPartFactory;
import org.tekkotsu.ui.editor.model.MultiTransitionModel;
import org.tekkotsu.ui.editor.model.StateNodeModel;
import org.tekkotsu.ui.editor.model.SubConnectionModel;
import org.tekkotsu.ui.editor.model.ViewModel;

public class MyEditPartFactory implements EditPartFactory {
	public EditPart createEditPart(EditPart context, Object model) {
		EditPart part = null;
		
		if (model instanceof ViewModel)
			part = new ViewEditPart();
		else if (model instanceof MultiTransitionModel)
			part = new MultiTransitionEditPart();
		else if (model instanceof StateNodeModel)
			part = new StateNodeEditPart();
		//else if (model instanceof TransitionModel)
		//	part = new TransitionEditPart();
		else if (model instanceof SubConnectionModel)
			part = new SubConnectionEditPart();
		
		part.setModel(model);
		return part;
	}
}
