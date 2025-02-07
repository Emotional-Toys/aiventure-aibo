/*
 * Created on Oct 24, 2004
 */
package org.tekkotsu.ui.editor.model.commands;

import java.util.ArrayList;
import java.util.List;

import org.eclipse.gef.commands.Command;
import org.tekkotsu.ui.editor.model.MultiTransitionModel;
import org.tekkotsu.ui.editor.model.ViewModel;

/**
 * @author asangpet
 *
 */
public class CreateMultiTransitionCommand extends Command {
	private ViewModel contentsModel;
	private MultiTransitionModel model;
	private List sourceConnections = new ArrayList();
	private List targetConnections = new ArrayList();
	
	public void execute() {		
		contentsModel.addChild(model);
		model.doPostAdd();		
	}
	
	public void setContentsModel(Object model) {
		contentsModel = (ViewModel) model;
	}
	
	public void setMultiTransitionModel(Object model) {
		this.model = (MultiTransitionModel)model;		
	}
		
	public void undo() {
		model.doPreDelete(contentsModel);
		contentsModel.removeChild(model);
	}
}
