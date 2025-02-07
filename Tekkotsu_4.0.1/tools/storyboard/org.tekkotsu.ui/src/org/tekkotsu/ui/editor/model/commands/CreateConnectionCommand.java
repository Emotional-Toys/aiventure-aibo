/*
 * Created on Sep 18, 2004
 */
package org.tekkotsu.ui.editor.model.commands;

import org.eclipse.gef.commands.Command;
import org.tekkotsu.ui.editor.model.AbstractConnectionModel;
import org.tekkotsu.ui.editor.model.StateNodeModel;
import org.tekkotsu.ui.editor.model.ViewModel;
import org.tekkotsu.ui.editor.resources.Debugger;

/**
 * @author asangpet
 */
public class CreateConnectionCommand extends Command {
	private StateNodeModel source, target;
	private AbstractConnectionModel connection;
	private ViewModel contentsModel;
	
	public void setContentsModel(Object model) {
		contentsModel = (ViewModel) model;
	}
	
	public boolean canExecute() {
		if (source == null || target == null)
			return false;
		if (source.equals(target))
			return false;
		return true;
	}
	
	/**
	 * Attach source/target and connection model to content model
	 */
	public void execute() {
		Debugger.printDebug(Debugger.DEBUG_ALL,"Create connection command executed");
		connection.attachSource();
		connection.attachTarget();
		contentsModel.addChild(connection);
	}
	
	public void setConnection(Object model) {
		connection = (AbstractConnectionModel) model;
	}
	
	public void setSource(Object model) {
		source = (StateNodeModel) model;
		connection.setSource(source);
	}
	
	public void setTarget(Object model) {
		target = (StateNodeModel) model;
		connection.setTarget(target);
	}
	
	public void undo() {
		connection.detachSource();
		connection.detachTarget();
		contentsModel.removeChild(connection);
	}
}
