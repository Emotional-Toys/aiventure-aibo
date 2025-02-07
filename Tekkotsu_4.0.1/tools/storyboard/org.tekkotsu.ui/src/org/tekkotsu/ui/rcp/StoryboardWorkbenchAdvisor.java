package org.tekkotsu.ui.rcp;

import org.eclipse.swt.graphics.Point;

import org.eclipse.ui.IWorkbenchWindow;
import org.eclipse.ui.application.IActionBarConfigurer;
import org.eclipse.ui.application.IWorkbenchWindowConfigurer;
import org.eclipse.ui.application.WorkbenchAdvisor;
import org.tekkotsu.ui.editor.resources.GlobalRegistry;
import org.tekkotsu.ui.rcp.actions.WorkbenchActionBuilder;


public class StoryboardWorkbenchAdvisor extends WorkbenchAdvisor {
	private WorkbenchActionBuilder fActionBuilder;
	
	public StoryboardWorkbenchAdvisor(GlobalRegistry.ParamObject param) {
		
	}
	
	public StoryboardWorkbenchAdvisor() {
	}

    public String getInitialWindowPerspectiveId() {
        return "org.tekkotsu.ui.rcp.StoryboardPerspective";
    }
    
	public void preWindowOpen(IWorkbenchWindowConfigurer configurer) {
		super.preWindowOpen(configurer);
        configurer.setInitialSize(new Point(1000, 700));
        configurer.setShowCoolBar(true);
        configurer.setShowStatusLine(false);
        configurer.setTitle("Storyboard Tool");
	}
	
	public void fillActionBars(IWorkbenchWindow window, IActionBarConfigurer configurer, int flags) {
		super.fillActionBars(window, configurer, flags);
		if (fActionBuilder == null)
			fActionBuilder= new WorkbenchActionBuilder(window);
		
		fActionBuilder.makeAndPopulateActions(getWorkbenchConfigurer(), configurer);
	}
	
	public void postShutdown() {
		if (fActionBuilder != null)
			fActionBuilder.dispose();
	}
	
}
