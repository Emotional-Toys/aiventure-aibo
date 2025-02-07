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
public class CreateBendpointCommand extends Command {
	  private AbstractConnectionModel connection;
	  private Point location;
	  private int index;

	  public void execute() {
	    connection.addBendpoint(index, location);
	  }

	  public void setConnection(Object model) {
	    connection = (AbstractConnectionModel) model;
	  }

	  public void setIndex(int i) {
	    index = i;
	  }

	  public void setLocation(Point point) {
	    location = point;
	  }

	  public void undo() {
	    connection.removeBendpoint(index);
	  }
}
