/*
 * Created on Sep 12, 2004
 */
package org.tekkotsu.ui.editor.model.commands;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

import org.eclipse.gef.commands.Command;
import org.tekkotsu.ui.editor.model.AbstractConnectionModel;
import org.tekkotsu.ui.editor.model.MultiTransitionModel;
import org.tekkotsu.ui.editor.model.StateNodeModel;
import org.tekkotsu.ui.editor.model.ViewModel;

/**
 * @author asangpet
 */

public class DeleteCommand extends Command {
	private ViewModel contentsModel;
	private StateNodeModel stateNodeModel;
	private List sourceConnections = new ArrayList();
	private List targetConnections = new ArrayList();
	
	public void execute() {
		if (stateNodeModel instanceof MultiTransitionModel) {
			((MultiTransitionModel)stateNodeModel).doPreDelete(contentsModel);
		} else {
			sourceConnections.addAll(stateNodeModel.getModelSourceConnections());
			targetConnections.addAll(stateNodeModel.getModelTargetConnections());		
			for (Iterator iter = sourceConnections.iterator(); iter.hasNext();) {
				AbstractConnectionModel model = (AbstractConnectionModel)iter.next();
				DeleteConnectionCommand delComm = new DeleteConnectionCommand();
				delComm.setConnectionModel(model);
				delComm.setContentsModel(contentsModel);
				delComm.execute();
			}

			for (Iterator iter = targetConnections.iterator(); iter.hasNext();) {
				AbstractConnectionModel model = (AbstractConnectionModel)iter.next();
				DeleteConnectionCommand delComm = new DeleteConnectionCommand();
				delComm.setConnectionModel(model);
				delComm.setContentsModel(contentsModel);
				delComm.execute();
			}
		}
		contentsModel.removeChild(stateNodeModel);
	}
	
	public void setContentsModel(Object model) {
		contentsModel = (ViewModel) model;		
	}
	
	public void setStateNodeModel(Object model) {
		stateNodeModel = (StateNodeModel) model;
	}
	
	public void undo() {
		contentsModel.addChild(stateNodeModel);

		if (stateNodeModel instanceof MultiTransitionModel) {
			((MultiTransitionModel)stateNodeModel).doPostAdd();
		} else {
			for (Iterator iter = sourceConnections.iterator(); iter.hasNext();) {
				AbstractConnectionModel model = (AbstractConnectionModel)iter.next();
				model.attachSource();
				model.attachTarget();
			}

			for (Iterator iter = targetConnections.iterator(); iter.hasNext();) {
				AbstractConnectionModel model = (AbstractConnectionModel)iter.next();
				model.attachSource();
				model.attachTarget();
			}
		
			sourceConnections.clear();
			targetConnections.clear();
		}
	}
}
