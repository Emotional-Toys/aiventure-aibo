/*
 * Created on Dec 22, 2004
 */
package org.tekkotsu.ui.editor;

import org.eclipse.draw2d.Connection;
import org.eclipse.draw2d.RotatableDecoration;

/**
 * @author asangpet
 *
 */
public interface TransitionConnection extends Connection {
	public void setLineWidth(int w);
	public void setTargetDecoration(RotatableDecoration dec);
}
