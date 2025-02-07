/*
 * Created on Oct 11, 2004
 */
package org.tekkotsu.ui.editor.model;

import org.eclipse.gef.requests.CreationFactory;
import org.jdom.Element;
import org.tekkotsu.ui.editor.resources.IDTag;

/**
 * @author asangpet
 */
public class SourceNodeModel extends SourceObjectModel {
	SourceModel parent;
	
	public SourceNodeModel(Element xml, SourceModel parent) {
		if (IDTag.XML_state_tag.equals(xml.getName())) {
			String idTag = xml.getAttributeValue(IDTag.XML_state_id); 
			if (!((idTag == null) || (idTag.equals("")))) setId(idTag);			
			setClassName(xml.getAttributeValue(IDTag.XML_state_class));
			this.parent = parent;
		}		
	}
		
	public CreationFactory getFactory() {
		return new SourceNodeCreationFactory(this);
	}
	
	public Element getXML() {
		Element content = new Element("state");
		content.setAttribute("id",this.getId());
		content.setAttribute("class",this.getClassName());
		return content;
	}
	
	public SourceModel getParentModel() {
		return parent;
	}
}

class SourceNodeCreationFactory implements CreationFactory {
	private SourceNodeModel model;
	
	public SourceNodeCreationFactory(SourceNodeModel model) {
		this.model = model;
	}
	public Object getNewObject() {
		return new StateNodeModel(model);
	}
	public Object getObjectType() {
		return StateNodeModel.class;
	}
}
