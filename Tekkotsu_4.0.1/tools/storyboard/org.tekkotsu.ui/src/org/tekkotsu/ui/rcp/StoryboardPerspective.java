package org.tekkotsu.ui.rcp;

import org.eclipse.ui.IFolderLayout;
import org.eclipse.ui.IPageLayout;
import org.eclipse.ui.IPerspectiveFactory;
import org.eclipse.ui.console.*;
import java.io.*;

public class StoryboardPerspective implements IPerspectiveFactory {
	public static final String ID_STORYBOARD_VIEW = "org.tekkotsu.ui.storyboard.views.StoryboardView"; 
	public static final String ID_RUNTIME_VIEW = "org.tekkotsu.ui.storyboard.views.RuntimeView";
	public static final String ID_IMAGE_VIEW = "org.tekkotsu.ui.storyboard.views.ImageView";
	
    public StoryboardPerspective() {
    }

    public void createInitialLayout(IPageLayout layout) {        
		IFolderLayout propRight = layout.createFolder("rightPropFolder", IPageLayout.RIGHT, 0.75f, IPageLayout.ID_EDITOR_AREA);
		layout.addView(ID_STORYBOARD_VIEW,IPageLayout.BOTTOM, (float)0.48,IPageLayout.ID_EDITOR_AREA);
		propRight.addView(IPageLayout.ID_PROP_SHEET);
		propRight.addView(ID_RUNTIME_VIEW);
		//propRight.addView(ID_IMAGE_VIEW);
		layout.addView(ID_IMAGE_VIEW, IPageLayout.BOTTOM, (float)0.48, ID_RUNTIME_VIEW);
    	//layout.addView(ID_RUNTIME_VIEW,IPageLayout.RIGHT,(float) 0.75,IPageLayout.ID_EDITOR_AREA);
    	initializeConsole();    			
    	//layout.addView(IConsoleConstants.ID_CONSOLE_VIEW,IPageLayout.BOTTOM, (float) 0.5, ID_STORYBOARD_VIEW);
    	/*String editorArea = layout.getEditorArea();
        IFolderLayout left =
            layout.createFolder("right", IPageLayout.RIGHT, (float) 0.26, editorArea);
        left.addView(IPageLayout.ID_PROP_SHEET);*/
    }
        
    protected void initializeConsole() {
    	ConsolePlugin console = ConsolePlugin.getDefault();
    	MessageConsole outputConsole = new MessageConsole("Debug Console",null);
    	
    	MessageConsoleStream outs = outputConsole.newMessageStream();
    	console.getConsoleManager().addConsoles(
    			new IConsole[] { outputConsole });
    	
    	StreamRedirect redir = new StreamRedirect(outs);
  
    	System.setOut(redir);   	
    }
    
    class StreamRedirect extends PrintStream {
    	private MessageConsoleStream console;
    	
    	public StreamRedirect(MessageConsoleStream output) {
    		super(System.err,true);
    		this.console = output;
    	}
    	
    	public void println(String s) {
    		super.println(s);
    		console.println(s);
    	}

    }

}

