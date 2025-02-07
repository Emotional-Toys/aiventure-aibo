/*
 * Created on Jan 27, 2005
 */
package org.tekkotsu.ui.rcp.actions;

import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileOutputStream;

import org.eclipse.core.runtime.IPath;
import org.eclipse.core.runtime.Path;
import org.eclipse.jface.action.Action;
import org.eclipse.jface.action.IAction;
import org.eclipse.jface.dialogs.MessageDialog;
import org.eclipse.jface.viewers.ISelection;
import org.eclipse.ui.IEditorDescriptor;
import org.eclipse.ui.IEditorInput;
import org.eclipse.ui.IEditorRegistry;
import org.eclipse.ui.IWorkbench;
import org.eclipse.ui.IWorkbenchPage;
import org.eclipse.ui.PartInitException;

import org.eclipse.ui.IWorkbenchWindow;
import org.eclipse.ui.IWorkbenchWindowActionDelegate;
import org.jdom.Document;
import org.jdom.output.Format;
import org.jdom.output.XMLOutputter;

import org.tekkotsu.ui.editor.model.ViewModel;
import org.tekkotsu.ui.rcp.editors.PathEditorInput;

/**
 * @author asangpet
 */
public class NewFileAction extends Action implements
		IWorkbenchWindowActionDelegate {

	private IWorkbenchWindow fWindow;
	private static int fileCount = -1;
	
	public NewFileAction() {
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

	/*
	 * @see org.eclipse.jface.action.Action#run()
	 */
	public void run() {		
		//File file = queryFile();//new File(wizard.getViewPath());
		fileCount++;
		String filename = "noname"+fileCount+".tsl";
		File file = new File(filename);		
		if (file==null) return;
		// generate a new file
		try {		    
			Document doc = new Document();
			filename = file.getAbsolutePath();
			ViewModel rootModel = new ViewModel(new Path(filename));
			rootModel.setModelSource("noname"+fileCount+".tsm");			
			doc.addContent(rootModel.getXML());

			XMLOutputter outputter = new XMLOutputter(Format.getPrettyFormat());
			BufferedOutputStream ostream = new BufferedOutputStream(
					new FileOutputStream(file));
			outputter.output(doc, ostream);
			ostream.close();
		} catch (Exception e) {
			//e.printStackTrace();
			MessageDialog.openError(null,"Problem","cannot create temporary model file ");
			e.printStackTrace();
		}		
		
		// open editor
		IEditorInput input= createEditorInput(file);
		String editorId= getEditorId(file);
		IWorkbenchPage page= fWindow.getActivePage();
		try {
			page.openEditor(input, editorId);
		} catch (PartInitException e) {
			e.printStackTrace();
		}
	}

	private String getEditorId(File file) {
		IWorkbench workbench = fWindow.getWorkbench();
		IEditorRegistry editorRegistry = workbench.getEditorRegistry();
		IEditorDescriptor descriptor = editorRegistry.getDefaultEditor(file
				.getName());
		if (descriptor != null)
			return descriptor.getId();
		return "org.tekkotsu.ui.rcp.editors.SimpleEditor"; //$NON-NLS-1$
	}

	private IEditorInput createEditorInput(File file) {
		IPath location = new Path(file.getAbsolutePath());
		PathEditorInput input = new PathEditorInput(location);
		return input;
	}
}