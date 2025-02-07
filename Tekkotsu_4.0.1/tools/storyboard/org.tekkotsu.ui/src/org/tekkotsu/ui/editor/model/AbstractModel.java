/*
 * Created on Sep 12, 2004
 */
package org.tekkotsu.ui.editor.model;

/**
 * AbstractModel containing property control methods
 * @author asangpet
 */

import java.beans.PropertyChangeListener;
import java.beans.PropertyChangeSupport;

import org.eclipse.ui.views.properties.IPropertyDescriptor;
import org.eclipse.ui.views.properties.IPropertySource;
import org.jdom.Content;

abstract public class AbstractModel implements IPropertySource {
	private PropertyChangeSupport listeners = new PropertyChangeSupport(this);
	private AbstractModel parent;
	
	public void setParent(AbstractModel parent) {
		this.parent = parent;
	}
	
	public AbstractModel getParent() {
		return parent;
	}
	
	public void addPropertyChangeListener(PropertyChangeListener listener) {
		listeners.addPropertyChangeListener(listener);
	}
	
	public void firePropertyChange(String propName, Object oldValue, Object newValue) {
		listeners.firePropertyChange(propName,oldValue,newValue);
	}
	
	public void removePropertyChangeListener(PropertyChangeListener listener) {
		listeners.removePropertyChangeListener(listener);
	}
	
	public Object getEditableValue() {
		return this;
	}
	
	public IPropertyDescriptor[] getPropertyDescriptors() {
		return new IPropertyDescriptor[0];
	}
	
	public Object getPropertyValue(Object id) {
		return null;
	}
	
	public boolean isPropertySet(Object id) {
		return false;
	}
	
	abstract public boolean isPreview();
	
	public void resetPropertyValue(Object id) {}
	public void setPropertyValue(Object id, Object value) {}
	
	/**
	 * Generate XML code representation of this model
	 * @return
	 */
	abstract public Content getXML();	
}
