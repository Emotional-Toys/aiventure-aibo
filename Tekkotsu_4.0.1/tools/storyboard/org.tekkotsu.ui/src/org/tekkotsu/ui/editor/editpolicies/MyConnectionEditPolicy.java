/*
 * Created on Sep 19, 2004
 */
package org.tekkotsu.ui.editor.editpolicies;

import org.eclipse.gef.commands.Command;
import org.eclipse.gef.editpolicies.ConnectionEditPolicy;
import org.eclipse.gef.requests.GroupRequest;
import org.tekkotsu.ui.editor.model.AbstractConnectionModel;
import org.tekkotsu.ui.editor.model.commands.DeleteConnectionCommand;
/**
 * @author asangpet
 *
 */
public class MyConnectionEditPolicy extends ConnectionEditPolicy {

	protected Command getDeleteCommand(GroupRequest request) {
		DeleteConnectionCommand command = new DeleteConnectionCommand();
		command.setConnectionModel(getHost().getModel());			
		command.setContentsModel(((AbstractConnectionModel)(getHost().getModel())).getParent());
		return command;
	}

}
