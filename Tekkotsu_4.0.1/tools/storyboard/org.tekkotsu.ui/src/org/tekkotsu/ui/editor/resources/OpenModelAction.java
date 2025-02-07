/*
 * Created on Feb 17, 2005
 */
package org.tekkotsu.ui.editor.resources;

import java.io.File;

import org.eclipse.swt.SWT;
import org.eclipse.swt.widgets.FileDialog;

import org.eclipse.jface.action.Action;
import org.eclipse.jface.action.IAction;
import org.eclipse.jface.viewers.ISelection;

import org.eclipse.ui.IEditorPart;
import org.eclipse.ui.IWorkbenchWindow;
import org.eclipse.ui.IWorkbenchWindowActionDelegate;
import org.tekkotsu.ui.editor.StateMachineEditor;

public class OpenModelAction extends Action implements
		IWorkbenchWindowActionDelegate {

	private IWorkbenchWindow fWindow;

	public OpenModelAction() {
		setEnabled(true);
	}

	/*
	 * @see org.eclipse.ui.IWorkbenchWindowActionDelegate#dispose()
	 */
	public void dispose() {
		fWindow = null;
	}

	/*
	 * @see org.eclipse.ui.IWorkbenchWindowActionDelegate#init(org.eclipse.ui.IWorkbenchWindow)
	 */
	public void init(IWorkbenchWindow window) {
		fWindow = window;
	}

	/*
	 * @see org.eclipse.ui.IActionDelegate#run(org.eclipse.jface.action.IAction)
	 */
	public void run(IAction action) {
		run();
	}

	/*
	 * @see org.eclipse.ui.IActionDelegate#selectionChanged(org.eclipse.jface.action.IAction,
	 *      org.eclipse.jface.viewers.ISelection)
	 */
	public void selectionChanged(IAction action, ISelection selection) {
	}

	private File queryFile() {
		FileDialog dialog = new FileDialog(fWindow.getShell(), SWT.OPEN);
		dialog.setText("Open File"); //$NON-NLS-1$
		String path = dialog.open();
		if (path != null && path.length() > 0)
			return new File(path);
		return null;
	}

	/*
	 * @see org.eclipse.jface.action.Action#run()
	 */
	public void run() {
		IEditorPart part = fWindow.getActivePage().getActiveEditor();
		if (part instanceof StateMachineEditor) {
			String[] filterExtensions = { "*.tsm", "*.*" };
			FileDialog fileDialog = new FileDialog(fWindow.getShell(), SWT.OPEN);
			fileDialog.setText("Model source");
			fileDialog.setFilterExtensions(filterExtensions);
			String path = fileDialog.open();
			if (!path.endsWith(".tsm"))
				path.concat(".tsm");
			StateMachineEditor editor = (StateMachineEditor) part;
			editor.setModelSource(path);
		} else {
			//MessageDialog messageDialog = new MessageDialog(fWindow.getShell(),"Error",null,"Layout file required",			
		}
	}
}
