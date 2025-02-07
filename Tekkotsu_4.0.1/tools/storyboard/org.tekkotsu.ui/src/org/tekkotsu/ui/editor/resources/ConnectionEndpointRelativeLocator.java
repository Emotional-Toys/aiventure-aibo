/*
 * Created on Oct 28, 2004
 */
package org.tekkotsu.ui.editor.resources;

import org.eclipse.draw2d.Connection;
import org.eclipse.draw2d.ConnectionEndpointLocator;
import org.eclipse.draw2d.IFigure;

/**
 * @author asangpet
 */
public class ConnectionEndpointRelativeLocator extends
		ConnectionEndpointLocator {
	public ConnectionEndpointRelativeLocator(Connection c, boolean isEnd) {
		super(c, isEnd);
	}

	/* (non-Javadoc)
	 * @see org.eclipse.draw2d.ConnectionEndpointLocator#relocate(org.eclipse.draw2d.IFigure)
	 */
	public void relocate(IFigure figure) {
		
	}	
}
