/*
 * Created on Sep 12, 2004
 */
package org.tekkotsu.ui.editor.editpolicies;

import org.eclipse.gef.commands.Command;
import org.eclipse.gef.editpolicies.ComponentEditPolicy;
import org.eclipse.gef.requests.GroupRequest;
import org.tekkotsu.ui.editor.model.commands.DeleteCommand;

/**
 * @author asangpet
 */

public class MyComponentEditPolicy extends ComponentEditPolicy {
	protected Command createDeleteCommand(GroupRequest deleteReqeust) {
		DeleteCommand command = new DeleteCommand();
		command.setContentsModel(getHost().getParent().getModel());
		command.setStateNodeModel(getHost().getModel());
		return command;
	}
}
