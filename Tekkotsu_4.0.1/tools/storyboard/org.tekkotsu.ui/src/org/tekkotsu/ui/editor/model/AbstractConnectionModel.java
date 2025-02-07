/*
 * Created on Sep 18, 2004
 */
package org.tekkotsu.ui.editor.model;

import java.util.ArrayList;
import java.util.List;

import org.eclipse.draw2d.geometry.Point;

/**
 * @author asangpet
 * 
 */

public abstract class AbstractConnectionModel extends AbstractModel {
	private StateNodeModel source, target;

	private List bendpoints = new ArrayList();

	public static final String P_BEND_POINT = "_bend_point";

	public void addBendpoint(int index, Point point) {
		bendpoints.add(index, point);
		firePropertyChange(P_BEND_POINT, null, null);
	}

	public List getBendpoints() {
		return bendpoints;
	}

	public void removeBendpoint(int index) {
		bendpoints.remove(index);
		firePropertyChange(P_BEND_POINT, null, null);
	}

	public void replaceBendpoint(int index, Point point) {
		bendpoints.set(index, point);
		firePropertyChange(P_BEND_POINT, null, null);
	}

	public boolean isPreview() {
		try {
			return getParent().isPreview();
		} catch (Exception e) {
			return false;
		}
	}

	public void attachSource() {
		if (!source.getModelSourceConnections().contains(this))
			source.addSourceConnection(this);
	}

	public void attachTarget() {
		if (!target.getModelTargetConnections().contains(this))
			target.addTargetConnection(this);
	}

	public void detachSource() {
		source.removeSourceConnection(this);
	}

	public void detachTarget() {
		target.removeTargetConnection(this);
	}

	public StateNodeModel getSource() {
		return source;
	}

	public StateNodeModel getTarget() {
		return target;
	}

	public void setSource(StateNodeModel model) {
		source = model;
	}

	public void setTarget(StateNodeModel model) {
		target = model;
	}

}
