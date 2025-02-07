/*
 * Created on Jan 27, 2005
 */
package org.tekkotsu.ui.editor;

import org.eclipse.jface.viewers.IStructuredSelection;
import org.eclipse.jface.wizard.Wizard;
import org.eclipse.jface.wizard.WizardPage;
import org.eclipse.swt.SWT;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.ui.IWorkbench;
import org.tekkotsu.ui.editor.resources.Debugger;

/**
 * @author asangpet
 */
public class NewViewWizard extends Wizard {
	
	IWorkbench workbench;
	IStructuredSelection selection;
	
	public void init(IWorkbench workbench, IStructuredSelection selection) {
		this.workbench = workbench;
		this.selection = selection;
		this.setWindowTitle("New view");
	}
	
	public void addPages() {
		ModelSelectPage modelSelectPage = new ModelSelectPage();
		addPage(modelSelectPage);		
	}
	
	/* (non-Javadoc)
	 * @see org.eclipse.jface.wizard.IWizard#performFinish()
	 */
	public boolean performFinish() {
		Debugger.printDebug(Debugger.DEBUG_ALL,"done");
		return true;
	}
	
	public String getViewPath() {
		return "noname.tsl";
	}

}

class ModelSelectPage extends WizardPage {

	public ModelSelectPage() {
		super("modelselect","Create a new view",null);
		this.setDescription("Select your model source");
	}
	
	/* (non-Javadoc)
	 * @see org.eclipse.jface.dialogs.IDialogPage#createControl(org.eclipse.swt.widgets.Composite)
	 */
	public void createControl(Composite parent) {
		GridData gd;
		Composite composite = new Composite(parent, SWT.NONE);
		
		GridLayout gl = new GridLayout();
		int ncol = 4;
		gl.numColumns = ncol;
		composite.setLayout(gl);
		
		Button robotButton = new Button(composite, SWT.RADIO);
		robotButton.setText("Retrieve a new model from AIBO");
		gd = new GridData(GridData.FILL_HORIZONTAL);
		gd.horizontalSpan = ncol;
		robotButton.setLayoutData(gd);
		robotButton.setSelection(true);

		Button fileButton = new Button(composite, SWT.RADIO);
		fileButton.setText("Use an existing model from file");
		gd = new GridData(GridData.FILL_HORIZONTAL);
		gd.horizontalSpan = ncol;
		fileButton.setLayoutData(gd);
		fileButton.setSelection(false);

		setControl(composite);
	}
}