/*
 * Created on Sep 12, 2004
 */
package org.tekkotsu.ui.editor.model.commands;

import org.eclipse.draw2d.geometry.Rectangle;
import org.eclipse.gef.commands.Command;
import org.tekkotsu.ui.editor.model.StateNodeModel;

/**
 * @author asangpet
 */
public class ChangeConstraintCommand extends Command {
	private StateNodeModel stateModel;
	private Rectangle constraint;
	private Rectangle oldConstraint;
	
	public void execute() {
		stateModel.setConstraint(constraint);
	}
	
	public void setConstraint(Rectangle rect) {
		constraint = rect;
	}
	
	public void setModel(Object model) {
		stateModel = (StateNodeModel) model;
		oldConstraint = stateModel.getConstraint();
	}
	
	public void undo() {
		stateModel.setConstraint(oldConstraint);
	}
}
