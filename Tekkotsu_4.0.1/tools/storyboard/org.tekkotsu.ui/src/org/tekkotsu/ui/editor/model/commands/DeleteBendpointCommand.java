/*
 * Created on Dec 22, 2004
 */
package org.tekkotsu.ui.editor.model.commands;

import org.eclipse.draw2d.geometry.Point;
import org.eclipse.gef.commands.Command;
import org.tekkotsu.ui.editor.model.AbstractConnectionModel;

/**
 * @author asangpet
 *
 */
public class DeleteBendpointCommand extends Command {
	  private AbstractConnectionModel connection;
	  private Point oldLocation; 
	  private int index;
	  
	  public void execute() {
	    oldLocation = (Point) connection.getBendpoints().get(index);
	    connection.removeBendpoint(index);
	  }

	  public void setConnectionModel(Object model) {
	    connection = (AbstractConnectionModel) model;
	  }

	  public void setIndex(int i) {
	    index = i;
	  }

	  public void undo() {
	    connection.addBendpoint(index, oldLocation);
	  }
}
