/*
 * Created on Sep 21, 2004
 */
package org.tekkotsu.ui.editor.model.commands;

import org.eclipse.gef.commands.Command;
import org.tekkotsu.ui.editor.model.AbstractConnectionModel;
import org.tekkotsu.ui.editor.model.StateNodeModel;

/**
 * @author asangpet
 */
public class ReconnectConnectionCommand extends Command {
	private AbstractConnectionModel connection;

	private StateNodeModel newSource = null;

	private StateNodeModel newTarget = null;

	private StateNodeModel oldSource = null;

	private StateNodeModel oldTarget = null;

	public void execute() {
		if (newSource != null) {
			oldSource = connection.getSource();
			reconnectSource(newSource);
		}
		if (newTarget != null) {
			oldTarget = connection.getTarget();
			reconnectTarget(newTarget);
		}
	}

	private void reconnectSource(StateNodeModel source) {
		connection.detachSource();
		connection.setSource(source);
		connection.attachSource();
	}

	private void reconnectTarget(StateNodeModel target) {
		connection.detachTarget();
		connection.setTarget(target);
		connection.attachTarget();
	}

	public void setConnectionModel(Object model) {
		connection = (AbstractConnectionModel) model;
	}

	public void setNewSource(Object model) {
		newSource = (StateNodeModel) model;
	}

	public void setNewTarget(Object model) {
		newTarget = (StateNodeModel) model;
	}

	public void undo() {
		if (oldSource != null)
			reconnectSource(oldSource);
		if (oldTarget != null)
			reconnectTarget(oldTarget);
		oldSource = null;
		oldTarget = null;
	}
}