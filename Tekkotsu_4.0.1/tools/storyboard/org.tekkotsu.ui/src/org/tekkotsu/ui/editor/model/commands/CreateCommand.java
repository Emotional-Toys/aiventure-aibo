/*
 * Created on Sep 13, 2004
 */
package org.tekkotsu.ui.editor.model.commands;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

import org.eclipse.gef.commands.Command;
import org.tekkotsu.ui.editor.model.AbstractConnectionModel;
import org.tekkotsu.ui.editor.model.StateNodeModel;
import org.tekkotsu.ui.editor.model.ViewModel;

/**
 * @author asangpet
 */
public class CreateCommand extends Command {
	private ViewModel contentsModel;
	private StateNodeModel stateNodeModel;
	
	public void execute() {		
		contentsModel.addChild(stateNodeModel);		
		stateNodeModel.doPostAdd();
	}
	
	public void setContentsModel(Object model) {
		contentsModel = (ViewModel) model;
	}
	
	public void setStateNodeModel(Object model) {
		stateNodeModel = (StateNodeModel) model;
	}
		
	public void undo() {
		contentsModel.removeChild(stateNodeModel);
		List sourceConnections = new ArrayList();
		List targetConnections = new ArrayList();
		sourceConnections.addAll(stateNodeModel.getModelSourceConnections());
		targetConnections.addAll(stateNodeModel.getModelTargetConnections());		
		for (Iterator iter = sourceConnections.iterator(); iter.hasNext();) {
			AbstractConnectionModel model = (AbstractConnectionModel)iter.next();
			model.detachSource();
			model.detachTarget();
			contentsModel.removeChild(model);
		}

		for (Iterator iter = targetConnections.iterator(); iter.hasNext();) {
			AbstractConnectionModel model = (AbstractConnectionModel)iter.next();
			model.detachSource();
			model.detachTarget();
			contentsModel.removeChild(model);
		}		
	}
}
