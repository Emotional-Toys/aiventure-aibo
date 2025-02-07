/*
 * Created on Sep 19, 2004
 */
package org.tekkotsu.ui.editor.model.commands;

import org.eclipse.gef.commands.Command;
import org.tekkotsu.ui.editor.model.AbstractConnectionModel;
import org.tekkotsu.ui.editor.model.MultiTransitionModel;
import org.tekkotsu.ui.editor.model.SubConnectionModel;
import org.tekkotsu.ui.editor.model.ViewModel;

/**
 * @author asangpet
 */
public class DeleteConnectionCommand extends Command {
	private AbstractConnectionModel connection;
	private ViewModel contentsModel;
	
	public void execute() {
		connection.detachSource();
		connection.detachTarget();
		contentsModel.removeChild(connection);
				
		if (connection.getParent() instanceof MultiTransitionModel) {			
			MultiTransitionModel parent = (MultiTransitionModel)connection.getParent();
			parent.removeSubConnection((SubConnectionModel)connection);
			if (parent.getSubConnectionList().size()==0) {
				// no more subconnection, remove multitransition node too
				contentsModel.removeChild(parent);
			}
		}
	}
	
	public void setConnectionModel(Object model) {
		connection = (AbstractConnectionModel) model;
	}
	
	public void setContentsModel(Object model) {
		if (model instanceof ViewModel)
			contentsModel = (ViewModel) model;
		else if (model instanceof MultiTransitionModel) {
			contentsModel = (ViewModel) ((MultiTransitionModel)model).getParent();
		}
	}
	
	public void undo() {
		MultiTransitionModel parent = (MultiTransitionModel) connection.getParent();
		if (!contentsModel.getPartChildren().contains(parent)) {
			contentsModel.addChild(connection.getParent());
			parent.doPostAdd();
		}
		contentsModel.addChild(connection);
		connection.attachSource();
		connection.attachTarget();
	}
}
