package org.tekkotsu.ui.rcp;

import org.eclipse.core.runtime.IPlatformRunnable;
import org.eclipse.swt.widgets.Display;
import org.eclipse.ui.PlatformUI;
import org.eclipse.ui.application.WorkbenchAdvisor;
import org.tekkotsu.ui.editor.resources.GlobalRegistry;

public class StoryboardApplication implements IPlatformRunnable {
	
	public Object run(Object args) {
		GlobalRegistry.ParamObject param = new GlobalRegistry.ParamObject();
		try {
			String[] arg = (String[])args;
			for (int i=0;i<arg.length;i++) System.out.println(arg[i]);
			param.setDogName(arg[0]);
		} catch (Exception e) {}

		WorkbenchAdvisor workbenchAdvisor = new StoryboardWorkbenchAdvisor(param);
        Display display = PlatformUI.createDisplay();

        try {
            int returnCode = PlatformUI.createAndRunWorkbench(display,
                    workbenchAdvisor);
            if (returnCode == PlatformUI.RETURN_RESTART) {
                return IPlatformRunnable.EXIT_RESTART;
            } else {
                return IPlatformRunnable.EXIT_OK;
            }    	    
        } finally {
            //display.dispose();
        }        
    }
}
