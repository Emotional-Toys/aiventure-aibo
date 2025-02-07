/*
 * Created on Sep 12, 2004
 */
package org.tekkotsu.ui.editor;

import org.eclipse.gef.ui.actions.ActionBarContributor;
import org.eclipse.gef.ui.actions.DeleteRetargetAction;
import org.eclipse.gef.ui.actions.RedoRetargetAction;
import org.eclipse.gef.ui.actions.UndoRetargetAction;
import org.eclipse.jface.action.IToolBarManager;
import org.eclipse.ui.actions.ActionFactory;

/**
 * @author asangpet
 */
public class MyContributor extends ActionBarContributor {

	public MyContributor() {
	}
	
	protected void buildActions() {
		addRetargetAction(new UndoRetargetAction());
		addRetargetAction(new RedoRetargetAction());
		addRetargetAction(new DeleteRetargetAction());
	}
	
	protected void declareGlobalActionKeys() {
	}
	
	public void contributeToToolBar(IToolBarManager toolBarManager) {		
		toolBarManager.add(getActionRegistry().getAction(ActionFactory.DELETE.getId()));	
		toolBarManager.add(getActionRegistry().getAction(ActionFactory.UNDO.getId()));
		toolBarManager.add(getActionRegistry().getAction(ActionFactory.REDO.getId()));
	}

}
