/*
 * Created on Sep 18, 2004
 */
package org.tekkotsu.ui.editor.model;

import org.eclipse.ui.views.properties.ComboBoxPropertyDescriptor;
import org.eclipse.ui.views.properties.IPropertyDescriptor;
import org.eclipse.ui.views.properties.TextPropertyDescriptor;
import org.jdom.Content;
import org.jdom.Element;
import org.tekkotsu.ui.editor.resources.Debugger;
import org.tekkotsu.ui.editor.resources.IDTag;

/**
 * This model currently represent a single link between nodes.
 * However, actual transition is a state node with associated connection model connect to it.
 * The model is implemented in MultiTransitionModel.
 * @author asangpet
 */
public class TransitionModel extends AbstractConnectionModel {
	public static final String P_ID = "_tid";
	public static final String P_LABEL = "_label";
	public static final String P_VISIBLE = "_visible";
	public static final String P_CLASS = "_class";
	public static final String P_LINEWIDTH = "_linewidth";
	
	private String id = "", label = "";
	private boolean visible = true;
	private static int transitionID = 1;
	private int lineWidth = 1;
	private SourceObjectModel sourceModel = null;
	
	public TransitionModel() {
		super();				
		this.id = "trans"+Integer.toString(transitionID++);
	}
	
	public TransitionModel(SourceTransitionModel source, ViewModel parent) {
		super();
		this.setId(source.getId());
		this.setVisible(true);
		StateNodeModel sourceNode = (StateNodeModel)parent.getPartChild((String)source.getSourceNodes().get(0));
		StateNodeModel destNode = (StateNodeModel)parent.getPartChild((String)source.getDestNodes().get(0));
		setSourceModel(source);
		setSource(sourceNode);
		setTarget(destNode);
		attachSource();
		attachTarget();
	}
	
	public TransitionModel(StateNodeModel source, StateNodeModel dest, String id) {
		this.setId(id);
		this.setVisible(true);
		setSource(source);
		setTarget(dest);
		attachSource();
		attachTarget();		
	}

	public TransitionModel(Element xml, ViewModel parent) {
		this();		
		if (IDTag.XML_transition_tag.equals(xml.getName())) {			
			try {
				setId(xml.getAttributeValue(IDTag.XML_common_id));
				SourceTransitionModel model = parent.getSourceModel().getTransition(this.getId());
				StateNodeModel source = (StateNodeModel)parent.getPartChild((String)model.getSourceNodes().get(0));
				StateNodeModel dest = (StateNodeModel)parent.getPartChild((String)model.getDestNodes().get(0));
				setLabel(xml.getAttributeValue(IDTag.XML_common_label));				
				
				try {
					setLineWidth(Integer.parseInt(xml.getAttributeValue(IDTag.XML_transition_linewidth)));
				} catch (NumberFormatException e) {}
				
				setSourceModel(model);
				setSource(source);
				setTarget(dest);
				attachSource();
				attachTarget();
				if ("false".equals(xml.getAttributeValue(IDTag.XML_transition_visible))) setVisible(false);
			} catch (Exception e) {
				e.printStackTrace();		
			}			
		}
	}
	
	public String getId() { 
		return id;
	}
	
	public void setId(String name) {
		this.id = name;
		firePropertyChange(P_ID, null, name);
	}

	public void verifySource(SourceModel src) {
		Debugger.printDebug(Debugger.DEBUG_ALL,"Verify src:"+id+":"+src.getNode(id));
		if (src.getChild(id)==null) {
			this.firePropertyChange(ViewModel.P_SOURCE_EXIST,null,new Boolean(false));
		} else {
			this.firePropertyChange(ViewModel.P_SOURCE_EXIST,null,new Boolean(true));
		}
	}
	
	public IPropertyDescriptor[] getPropertyDescriptors() {
		TextPropertyDescriptor idProp = new TextPropertyDescriptor(P_ID,"ID");
		TextPropertyDescriptor classProp = new TextPropertyDescriptor(P_CLASS,"Class");
		String modelCat = "Info";
		idProp.setCategory(modelCat);
		classProp.setCategory(modelCat);

		String[] visibleValues = {"True","False"};
		
		IPropertyDescriptor[] descriptors =
			new IPropertyDescriptor[] {
				idProp,
				classProp,
				new TextPropertyDescriptor(P_LABEL,"Label"),
				new TextPropertyDescriptor(P_LINEWIDTH,"Line width"),
				new ComboBoxPropertyDescriptor(P_VISIBLE,"Visible",visibleValues)};
		return descriptors;
	}
	
	public Object getPropertyValue(Object id) {
		if (id.equals(P_ID)) {
			return getId();
		} else if (id.equals(P_VISIBLE)) {
			if (visible) return new Integer(0);
			else return new Integer(1);
		} else if (id.equals(P_CLASS)) {
			try {
				return getSourceModel().getClassName();
			} catch (Exception e) { return ""; }
		} else if (id.equals(P_LABEL)) {
			return getLabel();
		} else if (id.equals(P_LINEWIDTH)) {
			return String.valueOf(getLineWidth());
		}
		return null;
	}
	
	public boolean isPropertySet(Object id) {
		if (id.equals(P_VISIBLE) || id.equals(P_LABEL) || id.equals(P_LINEWIDTH))
			return true;		
		return false;
	}
	
	public void setPropertyValue(Object id, Object value) {
		if (id.equals(P_VISIBLE)) {
			if (value.equals(new Integer(0)))
				setVisible(true);
			else
				setVisible(false);
		} else if (id.equals(P_LABEL)) {
			setLabel(value.toString());
		} else if (id.equals(P_LINEWIDTH)) {
			setLineWidth(Integer.parseInt(value.toString()));
		}
	}
	
	public Content getXML() {
		try {
			Element content = new Element(IDTag.XML_transition_tag);
			content.setAttribute(IDTag.XML_transition_id,getId());
			content.setAttribute(IDTag.XML_transition_visible,Boolean.toString(isVisible()));
			content.setAttribute(IDTag.XML_common_label,getLabel());
			content.setAttribute(IDTag.XML_transition_linewidth,Integer.toString(getLineWidth()));
			return content;
		} catch (Exception e) {
			return null;
		}
	}
	/**
	 * @return Returns the visibility property.
	 */
	public boolean isVisible() {
		return visible;
	}
	/**
	 * @param visible Set visibility value.
	 */
	public void setVisible(boolean visible) {
		this.visible = visible;
		firePropertyChange(P_VISIBLE,null,Boolean.valueOf(visible));
	}
	/**
	 * @return Returns the sourceModel of this transition.
	 */
	public SourceObjectModel getSourceModel() {
		return sourceModel;
	}
	/**
	 * @param sourceModel set sourceModel for this transition.
	 */
	public void setSourceModel(SourceObjectModel sourceModel) {
		this.sourceModel = sourceModel;
	}
	/**
	 * @return Returns the label.
	 */
	public String getLabel() {
		return label;
	}
	/**
	 * @param label The label to set.
	 */
	public void setLabel(String label) {
		if (label == null) this.label="";
		else this.label = label;
		firePropertyChange(P_LABEL,null,this.label);
	}
	/**
	 * @return Returns the lineWidth.
	 */
	public int getLineWidth() {
		return lineWidth;
	}
	/**
	 * @param lineWidth The lineWidth to set.
	 */
	public void setLineWidth(int lineWidth) {
		this.lineWidth = lineWidth;
		firePropertyChange(P_LINEWIDTH, null, new Integer(lineWidth));
	}
	
	public String toString() {
		return this.getClass().toString() + ":" + getId();
	}
}
