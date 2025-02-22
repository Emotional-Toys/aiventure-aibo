/*
 * Created on Dec 22, 2004
 */
package org.tekkotsu.ui.editor.editpolicies;

import org.eclipse.draw2d.geometry.Point;
import org.eclipse.gef.commands.Command;
import org.eclipse.gef.editpolicies.BendpointEditPolicy;
import org.eclipse.gef.requests.BendpointRequest;
import org.tekkotsu.ui.editor.model.commands.CreateBendpointCommand;
import org.tekkotsu.ui.editor.model.commands.DeleteBendpointCommand;
import org.tekkotsu.ui.editor.model.commands.MoveBendpointCommand;
import org.tekkotsu.ui.editor.resources.Debugger;

/**
 * @author asangpet
 * 
 */
public class MyBendpointEditPolicy extends BendpointEditPolicy {

	/*
	 * (non-Javadoc)
	 * 
	 * @see org.eclipse.gef.editpolicies.BendpointEditPolicy#getCreateBendpointCommand(org.eclipse.gef.requests.BendpointRequest)
	 */
	protected Command getCreateBendpointCommand(BendpointRequest request) {
		Point point = request.getLocation();
		getConnection().translateToRelative(point);

		CreateBendpointCommand command = new CreateBendpointCommand();
		command.setLocation(point);
		command.setConnection(getHost().getModel());
		command.setIndex(request.getIndex());
		Debugger.printDebug(Debugger.DEBUG_ALL,"create BEND POINT: "+getHost().getModel());

		return command;
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see org.eclipse.gef.editpolicies.BendpointEditPolicy#getDeleteBendpointCommand(org.eclipse.gef.requests.BendpointRequest)
	 */
	protected Command getDeleteBendpointCommand(BendpointRequest request) {
	    DeleteBendpointCommand command = new DeleteBendpointCommand();
	    command.setConnectionModel(getHost().getModel());
	    command.setIndex(request.getIndex());
	    return command;		
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see org.eclipse.gef.editpolicies.BendpointEditPolicy#getMoveBendpointCommand(org.eclipse.gef.requests.BendpointRequest)
	 */
	protected Command getMoveBendpointCommand(BendpointRequest request) {
		Point location = request.getLocation();
		getConnection().translateToRelative(location);

		MoveBendpointCommand command = new MoveBendpointCommand();
		command.setConnectionModel(getHost().getModel());
		command.setIndex(request.getIndex());
		command.setNewLocation(location);

		return command;
	}

}
