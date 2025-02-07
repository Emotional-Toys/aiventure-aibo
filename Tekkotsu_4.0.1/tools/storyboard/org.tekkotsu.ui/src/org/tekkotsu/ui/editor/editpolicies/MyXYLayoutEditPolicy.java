package org.tekkotsu.ui.editor.editpolicies;

/**
 * @author asangpet
 */

import org.eclipse.draw2d.geometry.Rectangle;
import org.eclipse.gef.EditPart;
import org.eclipse.gef.Request;
import org.eclipse.gef.commands.Command;
import org.eclipse.gef.editpolicies.XYLayoutEditPolicy;
import org.eclipse.gef.requests.CreateRequest;
import org.tekkotsu.ui.editor.model.MultiTransitionModel;
import org.tekkotsu.ui.editor.model.StateNodeModel;
import org.tekkotsu.ui.editor.model.ViewModel;
import org.tekkotsu.ui.editor.model.commands.ChangeConstraintCommand;
import org.tekkotsu.ui.editor.model.commands.CreateCommand;
import org.tekkotsu.ui.editor.model.commands.CreateMultiTransitionCommand;
import org.tekkotsu.ui.editor.resources.Debugger;

public class MyXYLayoutEditPolicy extends XYLayoutEditPolicy{
	
	public Command getCommand(Request request) {
		Debugger.printDebug(Debugger.DEBUG_ALL, "XYLayoutEditPolicy getCommand() : "+request.getType());			
		return super.getCommand(request);
	}

	protected Command createAddCommand(EditPart child, Object constraint) {
		return null;
	}

	protected Command createChangeConstraintCommand(EditPart child, Object constraint) {
		ChangeConstraintCommand command = new ChangeConstraintCommand();
		command.setModel(child.getModel());
		command.setConstraint((Rectangle) constraint);
		
		return command;
	}
	
	protected Command getCreateCommand(CreateRequest request) {		
		Debugger.printDebug(Debugger.DEBUG_ALL, "XYLayoutEditPolicy getCreateCommand() : "+request.getNewObjectType());
		if (request.getNewObjectType().equals(StateNodeModel.class)) {
			CreateCommand command = new CreateCommand();
			Rectangle constraint = new Rectangle(0,0,50,20);
			try {
				constraint.setLocation(request.getLocation());
				//constraint = (Rectangle) getConstraintFor(request);		
			} catch (Exception e) {}
			StateNodeModel model = (StateNodeModel)request.getNewObject();
		
			// check if the node is already present
			if (((ViewModel)getHost().getModel()).getPartChild(model.getId()) != null) return null;			
			
			model.setConstraint(constraint);			
			command.setContentsModel(getHost().getModel());
			command.setStateNodeModel(model);			
			return command;
		} /* else if (request.getNewObjectType().equals(TransitionModel.class)){
			CreateConnectionCommand command = new CreateConnectionCommand();
			TransitionModel model = (TransitionModel)request.getNewObject();
			if (((ViewModel)getHost().getModel()).getPartChild(model.getId()) != null) return null;									
			command.setContentsModel(getHost().getModel());
			command.setConnection(model);
			command.setSource(request.getExtendedData().get("_source"));
			//model.getSource());
			command.setTarget(request.getExtendedData().get("_target"));
			//model.getTarget());	
			return command;
		} */ else if (request.getNewObjectType().equals(MultiTransitionModel.class)) {			
			CreateMultiTransitionCommand command = new CreateMultiTransitionCommand();
			Rectangle constraint = new Rectangle(0,0,50,20);
			try {
				constraint.setLocation(request.getLocation());		
			} catch (Exception e) {}
			MultiTransitionModel model = (MultiTransitionModel)request.getNewObject();
		
			// check if the transition node is already present
			if (((ViewModel)getHost().getModel()).getPartChild(model.getId()) != null) return null;			
			model.setConstraint(constraint);			
			command.setContentsModel(getHost().getModel());
			command.setMultiTransitionModel(model);			
			Debugger.printDebug(Debugger.DEBUG_ALL, "XYLayoutEditPolicy getCreateCommand() : CreateMultiTransitionCommand : "+command);
			return command;			
		} else return null;		
	}
	
	protected Command getDeleteDependantCommand(Request request) {
		return null;
	}
}
