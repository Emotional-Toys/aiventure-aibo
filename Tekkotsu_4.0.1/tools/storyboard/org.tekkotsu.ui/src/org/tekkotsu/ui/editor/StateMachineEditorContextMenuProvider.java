/*
 * Created on Oct 6, 2004
 */
package org.tekkotsu.ui.editor;

import java.util.Iterator;
import java.util.List;

import org.eclipse.draw2d.geometry.Point;
import org.eclipse.gef.ContextMenuProvider;
import org.eclipse.gef.EditPart;
import org.eclipse.gef.EditPartViewer;
import org.eclipse.gef.ui.actions.ActionRegistry;
import org.eclipse.gef.ui.actions.GEFActionConstants;
import org.eclipse.jface.action.Action;
import org.eclipse.jface.action.IAction;
import org.eclipse.jface.action.IMenuManager;
import org.eclipse.jface.action.MenuManager;
import org.eclipse.swt.events.MouseEvent;
import org.eclipse.swt.events.MouseMoveListener;
import org.eclipse.ui.actions.ActionFactory;
import org.tekkotsu.ui.editor.editparts.StateNodeEditPart;
import org.tekkotsu.ui.editor.model.StateNodeModel;
import org.tekkotsu.ui.editor.model.ViewModel;
import org.tekkotsu.ui.editor.resources.Debugger;
import org.tekkotsu.ui.editor.resources.IDTag;

/**
 * Provide context menu for state machine edit parts
 * @author asangpet
 */
public class StateMachineEditorContextMenuProvider extends ContextMenuProvider {
	/** The editor's action registry. */
	private ActionRegistry actionRegistry;
	private int mousex, mousey;
	
	class MouseLocationTrack implements MouseMoveListener {
		public void mouseMove(MouseEvent e) {
			mousex = e.x; mousey = e.y;
		}
	}
	
	/**
	 * Instantiate a new menu context provider for the specified EditPartViewer 
	 * and ActionRegistry.
	 * @param viewer	the editor's graphical viewer
	 * @param registry	the editor's action registry
	 * @throws IllegalArgumentException if registry is <tt>null</tt>. 
	 */
	public StateMachineEditorContextMenuProvider(EditPartViewer viewer, ActionRegistry registry) {
		super(viewer);
		if (registry == null) {
			throw new IllegalArgumentException();
		}
		getViewer().getControl().addMouseMoveListener(new MouseLocationTrack());
		actionRegistry = registry;
	}

	/**
	 * Called when the context menu is about to show. Actions, 
	 * whose state is enabled, will appear in the context menu.
	 * @see org.eclipse.gef.ContextMenuProvider#buildContextMenu(org.eclipse.jface.action.IMenuManager)
	 */
	public void buildContextMenu(IMenuManager menu) {		
		// Add standard action groups to the menu
		GEFActionConstants.addStandardActionGroups(menu);
		// Add actions to the menu
		menu.appendToGroup(
				GEFActionConstants.GROUP_UNDO, // target group id
				getAction(ActionFactory.UNDO.getId())); // action to add
		menu.appendToGroup(
				GEFActionConstants.GROUP_UNDO, 
				getAction(ActionFactory.REDO.getId()));
		menu.appendToGroup(
				GEFActionConstants.GROUP_EDIT,
				getAction(ActionFactory.DELETE.getId()));		

		menu.add(actionRegistry.getAction(IDTag.ACTION_add_all));
		// declare menu options based on selected objects
		List selectedParts = getViewer().getSelectedEditParts();
		if ((selectedParts.size()==1) && (((EditPart)selectedParts.get(0)) instanceof StateNodeEditPart)) {
			// add transition menu			
			menu.add(getTransitionMenu((EditPart)selectedParts.get(0)));			
		} else {			
			menu.add(getStateMenu());
			Debugger.printDebug(Debugger.DEBUG_ALL,"Mouse location: ("+mousex+","+mousey+")");
			menu.add(getTransitionMenu(null));
		}
	}
	
	private MenuManager getStateMenu() {
		MenuManager statesubmenu = new MenuManager("Add &State");
		ViewModel view = (ViewModel)this.getViewer().getContents().getModel();		
		
		Iterator iter = actionRegistry.getActions();
		while (iter.hasNext()) {
			Action action = (Action)iter.next();
			if (action instanceof StateMachineEditor.CreateNodeAction) {
				StateMachineEditor.CreateNodeAction act = (StateMachineEditor.CreateNodeAction)action;				
				action.setChecked(view.getPartChild(act.getModel().getId()) != null);
				action.setEnabled(!action.isChecked());
				act.getRequest().setLocation(new Point(mousex,mousey));				
				statesubmenu.add(action);
			}
		}		
		return statesubmenu;
	}
	
	private MenuManager getTransitionMenu(EditPart part) {
		MenuManager transmenu = new MenuManager("Add &Transition");
		// set checked / display only related node
		Iterator iter = actionRegistry.getActions();
		ViewModel view = (ViewModel)this.getViewer().getContents().getModel();
		while (iter.hasNext()) {
			Action action = (Action)iter.next();
			if (action instanceof StateMachineEditor.CreateTransitionAction) {
				StateMachineEditor.CreateTransitionAction transAct = (StateMachineEditor.CreateTransitionAction) action;
				action.setChecked((view.getPartChild(transAct.getModel().getId()) != null));
				action.setEnabled(!action.isChecked());
				transAct.getRequest().setLocation(new Point(mousex,mousey));
				if ((part == null) || (transAct.getModel().getDestNodes().contains(((StateNodeModel)part.getModel()).getId())) ||
					(transAct.getModel().getSourceNodes().contains(((StateNodeModel)part.getModel()).getId()))) {					
					transmenu.add(action);							
				}
				
			}			
		}
		return transmenu;
	}

	private IAction getAction(String actionId) {
		return actionRegistry.getAction(actionId);
	}

}
