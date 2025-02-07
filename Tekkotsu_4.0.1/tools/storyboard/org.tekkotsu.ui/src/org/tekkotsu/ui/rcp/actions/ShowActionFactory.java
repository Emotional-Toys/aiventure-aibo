/*
 * Created on Nov 7, 2004
 */
package org.tekkotsu.ui.rcp.actions;

import org.eclipse.jface.action.Action;
import org.eclipse.ui.IWorkbenchWindow;
import org.eclipse.ui.PartInitException;

/**
 * @author asangpet
 *
 */
public class ShowActionFactory {
	private IWorkbenchWindow window;	
	
	public ShowActionFactory(IWorkbenchWindow window) {
		this.window = window;
	}
	
	class ShowAction extends Action {
		private String viewID;
		public ShowAction(String ID) {
			viewID = ID;
		}
		public void run() {
			try {					
				window.getActivePage().showView(viewID);
			} catch (PartInitException e) {
				e.printStackTrace();
			}
		}
	}
	
	public Action create(String viewName, String ID) {		
		Action showAction = new ShowAction(ID);
		showAction.setText(viewName);
		return showAction;
	}
}
