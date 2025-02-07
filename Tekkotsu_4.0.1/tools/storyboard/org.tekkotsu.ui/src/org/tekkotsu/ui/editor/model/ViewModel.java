/*
 * Created on Sep 12, 2004
 */
package org.tekkotsu.ui.editor.model;

import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;
import java.io.File;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

import org.eclipse.core.runtime.IPath;
import org.eclipse.core.runtime.Path;
import org.eclipse.jface.dialogs.MessageDialog;
import org.eclipse.swt.SWT;
import org.eclipse.swt.widgets.FileDialog;
import org.eclipse.ui.IEditorSite;
import org.eclipse.ui.IWorkbench;
import org.eclipse.ui.views.properties.IPropertyDescriptor;
import org.eclipse.ui.views.properties.TextPropertyDescriptor;
import org.jdom.Content;
import org.jdom.Document;
import org.jdom.Element;
import org.tekkotsu.ui.editor.StateMachineEditor;
import org.tekkotsu.ui.editor.resources.Debugger;
import org.tekkotsu.ui.editor.resources.IDTag;

/**
 * Parent model for all contents, representing the view
 * 
 * @author asangpet
 */

public class ViewModel extends AbstractModel implements PropertyChangeListener {
	public static final String P_CHILDREN = "_children";

	public static final String P_EXECUTION_TRACE = "_trace";

	public static final String P_MODELSOURCE = "_source";

	public static final String P_SOURCE_EXIST = "_view_source_exist";

	public static final String P_ACTIVATE_CHANGE = "_activation_change";

	public static final String P_EDITING_MODE = "_editing_mode";

	// this list contains all edit part children
	// (state+transition+subconnection)
	private List editPartChildren = new ArrayList();

	private boolean previewMode = false;

	private SourceModel sourceModel;

	private String tracePath = null;

	private IPath path = null;
	private StateMachineEditor editor = null;
	
	public ViewModel(IPath path) {
		super();
		this.setPath(path);
	}
	
	public void setEditor(StateMachineEditor editor) {
		this.editor = editor;
	}
	
	public void setPreview(boolean preview) {
		previewMode = preview;
		// reactivate all nodes
		if (!previewMode) {
			// activate new nodes
			List value = this.getPartChildrenNode();
			Iterator iter = value.iterator();
			Debugger.printDebug(Debugger.DEBUG_ALL,"Activate(for editing) " + value);
			while (iter.hasNext()) {
				Object child = this.getPartChild(iter.next().toString());
				if (child instanceof StateNodeModel) {
					StateNodeModel state = (StateNodeModel) child;
					state.activate();
				}
			}
		}
	}

	public boolean isPreview() {
		return previewMode;
	}
	
	public ViewModel(IEditorSite site, Element xml, IPath path) {
		super();
		setPath(path);
		try {
			if ("view".equals(xml.getName())) {
				String sourcePath = xml
						.getAttributeValue(IDTag.XML_view_modelsource);
				if (sourcePath != null) {				
					Path modelPath = new Path(sourcePath);
					File file = modelPath.toFile();//new File((getAbsolutePath(path, sourcePath)).toOSString());								
					while (!file.exists()) {
						// file not found, ask for source path						
						String[] filterExtensions = { "*.tsm", "*.*" };
						FileDialog fileDialog = new FileDialog(site.getShell(), SWT.OPEN);
						fileDialog.setText("Cannot find model source");
						fileDialog.setFilterExtensions(filterExtensions);
						sourcePath = fileDialog.open();					
						modelPath = new Path(sourcePath);
						file = modelPath.toFile();
					}
					this.setModelSource(sourcePath);
					//this.sourceModel = new SourceModel(); //getRelativePath(sourcePath));
					//this.sourceModel.setSourcePath(modelPath); //getRelativePath(sourcePath));					
				}
				
				Iterator iter = xml.getDescendants();
				while (iter.hasNext()) {
					Object next = iter.next();
					if (next instanceof Element) {
						Element child = (Element) next;
						if (IDTag.XML_state_tag.equals(child.getName())) {
							if (sourceModel.getNode(child
									.getAttributeValue(IDTag.XML_common_id)) != null)
								this.addChild(new StateNodeModel(child, this));
						} else if (IDTag.XML_transition_tag.equals(child
								.getName())) {
							try {
								String transId = child
										.getAttributeValue(IDTag.XML_transition_id);
								MultiTransitionModel multiNode = new MultiTransitionModel(
										child, this);
								this.addChild(multiNode);
								multiNode.doPostAdd();
								/*
								 * if
								 * (sourceModel.getTransition(transId).isMultiTransition() ) {
								 * MultiTransitionModel multiNode = new
								 * MultiTransitionModel(child, this);
								 * this.addChild(multiNode);
								 * multiNode.doPostAdd(); //this.addChild(new
								 * MultiTransitionModel(child,this)); } else {
								 * this.addChild(new
								 * TransitionModel(child,this)); }
								 */
							} catch (Exception e) {
								System.err
										.println("Found incompatible view :"
												+ child
														.getAttributeValue(IDTag.XML_transition_id));
							}
						}
					}
				}
			}
		} catch (Exception e) {
			//e.printStackTrace();
		}
	}

	public void verifySource() {
		List children = this.getPartChildren();
		Iterator iter = children.iterator();
		SourceModel source = this.getSourceModel();
		while (iter.hasNext()) {
			Object next = iter.next();
			if (next instanceof StateNodeModel) {
				((StateNodeModel) next).verifySource(source);
			} 
		}
	}

	public void addChild(Object child) {
		((AbstractModel) child).setParent(this);
		editPartChildren.add(child);
		firePropertyChange(P_CHILDREN, null, null);
	}

	public List getPartChildren() {
		return editPartChildren;
	}

	public List getPartChildrenNode() {
		List nodes = new ArrayList();
		Iterator iter = editPartChildren.iterator();
		while (iter.hasNext()) {
			Object next = iter.next();
			if (next instanceof StateNodeModel) {
				nodes.add(next);
			}
		}
		return nodes;
	}

	/**
	 * Search for a specific child model
	 * 
	 * @param id
	 *            String identification of the model
	 * @return Return a child model of this content
	 */
	public Object getPartChild(String id) {
		Iterator iter = getPartChildren().iterator();
		while (iter.hasNext()) {
			Object nextModel = iter.next();
			if (nextModel instanceof StateNodeModel) {
				StateNodeModel model = (StateNodeModel) nextModel;
				if (id.equals(model.getId()))
					return model;
			} /*
				 * else if (nextModel instanceof TransitionModel) {
				 * TransitionModel model = (TransitionModel)nextModel; if
				 * (id.equals(model.getId())) return model; }
				 */
		}
		return null;
	}

	public void removeChild(Object child) {
		editPartChildren.remove(child);
		firePropertyChange(P_CHILDREN, null, null);
	}

	/*
	public static IPath getAbsolutePath(IPath refPath, String inPath) {
	    IPath viewPath = refPath.removeLastSegments(1);
	    Path path = new Path(inPath);
	    if ((path.getDevice()!=null) && (!viewPath.getDevice().equals(path.getDevice()))) {
	        return path.makeAbsolute();
	    }
	    int match = path.matchingFirstSegments(viewPath);
	    IPath result = viewPath.append(path.removeFirstSegments(match)).makeAbsolute();
	    return result;		
	}
	
	public static IPath getRelativePath(IPath refPath, String inPath) {
	    Path path = new Path(inPath);
	    IPath viewPath = refPath.removeLastSegments(1);
	    String pathDevice = ""+path.getDevice();
	    String viewPathDevice = ""+viewPath.getDevice();
	    if (path.isAbsolute() && (pathDevice.equals(viewPathDevice))) {
	        int match = path.matchingFirstSegments(viewPath);	        
	        if (match==0) return path;
	        return path.setDevice(null).removeFirstSegments(match);
	    } else return path;		
	}
	
	public IPath getRelativePath(String inPath) {
		return getRelativePath(this.getPath(), inPath);
	}
	*/
	
	public Content getXML() {
		Element content = new Element(IDTag.XML_view_tag);
		if (this.sourceModel == null)
			content.setAttribute(IDTag.XML_view_modelsource, "");
		else {
			content.setAttribute(IDTag.XML_view_modelsource, this.sourceModel.getSourcePath().toString());//getRelativePath(this.sourceModel.getSourcePath().toString()).toString());
		}
		List children = getPartChildren();
		List transitions = new ArrayList();

		Iterator iter = children.iterator();
		while (iter.hasNext()) {
			AbstractModel childModel = (AbstractModel) iter.next();
			if (childModel instanceof MultiTransitionModel) {
				transitions.add(childModel);
			} else {
				Content child = childModel.getXML();
				if (child != null)
					content.addContent(child);
			}
		}

		iter = transitions.iterator();
		while (iter.hasNext()) {
			AbstractModel childModel = (AbstractModel) iter.next();
			Content child = childModel.getXML();
			if (child != null)
				content.addContent(child);
		}
		return content;
	}

	/**
	 * @return Returns model source.
	 */
	public SourceModel getSourceModel() {
		if (sourceModel == null) {
			sourceModel = new SourceModel();
		}
		return sourceModel;
	}

	/**
	 * @param modelSource
	 *            File name of model source to set.
	 */
	public void setModelSource(String modelSource) {
	    IPath path = new Path(modelSource);
		if (sourceModel == null)
			this.sourceModel = new SourceModel(path);
		this.sourceModel.setSourcePath(path);
		
		firePropertyChange(P_MODELSOURCE, null, sourceModel.getSourcePath());
	}

	public void setTracePath(String tracePath) {
		this.tracePath = tracePath;
		firePropertyChange(P_EXECUTION_TRACE, null, tracePath);
	}

	public void setModelSource(SourceModel modelSource) {
		this.sourceModel = modelSource;
	}

	public IPropertyDescriptor[] getPropertyDescriptors() {
		IPropertyDescriptor[] descriptors = new IPropertyDescriptor[] { new TextPropertyDescriptor(
				P_MODELSOURCE, "Model source") };
		return descriptors;
	}

	public Object getPropertyValue(Object id) {
		if (id.equals(P_MODELSOURCE)) {
			return sourceModel.getSourcePath();
		}
		return null;
	}

	public void setPropertyValue(Object id, Object value) {
		if (id.equals(P_MODELSOURCE))
			setModelSource((String) value);
	}

	public boolean isPropertySet(Object id) {
		if (id.equals(P_MODELSOURCE))
			return true;
		else
			return false;
	}

	/**
	 * @return Returns the tracePath.
	 */
	public String getTracePath() {
		return tracePath;
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see java.beans.PropertyChangeListener#propertyChange(java.beans.PropertyChangeEvent)
	 */
	public void propertyChange(PropertyChangeEvent evt) {		
		if (evt.getPropertyName().equals(ViewModel.P_ACTIVATE_CHANGE)) {
			if (this.isPreview()) {
				// deactivate nodes
				Iterator iter = this.getPartChildrenNode().iterator();
				while (iter.hasNext()) {
					Object child = iter.next();
					if (child instanceof StateNodeModel) {
						StateNodeModel state = (StateNodeModel) child;
						state.deactivate();
					}
				}

				// activate new nodes
				List value = (List) evt.getNewValue();
				iter = value.iterator();
				Debugger.printDebug(Debugger.DEBUG_ALL,"Activate " + value);
				while (iter.hasNext()) {
					Object child = this.getPartChild(iter.next().toString());
					if (child instanceof StateNodeModel) {
						StateNodeModel state = (StateNodeModel) child;
						state.activate();
					}
				}
			}
		} else if (evt.getPropertyName().equals(ViewModel.P_EDITING_MODE)) {
			// activate new nodes
			List value = this.getPartChildrenNode();
			Iterator iter = value.iterator();
			Debugger.printDebug(Debugger.DEBUG_ALL,"Activate(for editing) " + value);
			while (iter.hasNext()) {
				Object child = this.getPartChild(iter.next().toString());
				if (child instanceof StateNodeModel) {
					StateNodeModel state = (StateNodeModel) child;
					state.activate();
				}
			}
		}
	}
    public IPath getPath() {
        return path;
    }
    public void setPath(IPath path) {
        this.path = path;
    }
}