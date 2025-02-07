/*
 * Created on Oct 11, 2004
 */
package org.tekkotsu.ui.editor.model;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

import org.eclipse.gef.requests.CreationFactory;
import org.jdom.Element;
import org.tekkotsu.ui.editor.resources.IDTag;

/**
 * This is the source model for associated MultiTransitionModel
 * @author asangpet
 */
public class SourceTransitionModel extends SourceObjectModel {
	String type;
	List sourceNodes = new ArrayList();
	List destNodes = new ArrayList();
	
	public SourceTransitionModel(Element xml) {
		if (IDTag.XML_transition_tag.equals(xml.getName())) {
			try {
				setId(xml.getAttributeValue(IDTag.XML_transition_id));
				setType(xml.getAttributeValue(IDTag.XML_transition_type));
				setClassName(xml.getAttributeValue(IDTag.XML_transition_class));
				
				// iterate through children to get source/dest nodes
				Iterator iter = xml.getChildren().iterator();
				while (iter.hasNext()) {
					Element child = (Element)(iter.next());
					if (child.getName().equals(IDTag.XML_transition_source)) {
						sourceNodes.add(child.getTextTrim());						
					} else if (child.getName().equals(IDTag.XML_transition_dest)) {
						destNodes.add(child.getTextTrim());
					}
				}
			} catch (Exception e) {
				e.printStackTrace();
			}
		}	
	}
	
	
	/**
	 * @return true if the transition is multi-transition, otherwise, return false
	 */
	public boolean isMultiTransition() {
		return (sourceNodes.size()>1) || (destNodes.size()>1);
	}

	/**
	 * @return Returns the destNodes.
	 */
	public List getDestNodes() {
		return destNodes;
	}
	/**
	 * @return Returns the sourceNodes.
	 */
	public List getSourceNodes() {
		return sourceNodes;
	}

	/**
	 * @return Returns the type.
	 */
	public String getType() {
		return type;
	}
	
	/**
	 * @param type The type to set.
	 */
	public void setType(String type) {
		this.type = type;
	}
	
	public CreationFactory getFactory(ViewModel viewmodel) {
		return new SourceTransitionCreationFactory(this, viewmodel);
	}

	public Element getXML() {
		Element content = new Element(IDTag.XML_transition_tag);
		content.setAttribute(IDTag.XML_common_id,this.getId());
		content.setAttribute(IDTag.XML_common_class,this.getClassName());
		
		Iterator iter = getSourceNodes().iterator();
		while (iter.hasNext()) {
			Element node = new Element(IDTag.XML_transition_source);
			node.setText((String)iter.next());	
			content.addContent(node);
		}
		iter = getDestNodes().iterator();
		while (iter.hasNext()) {
			Element node = new Element(IDTag.XML_transition_dest);
			node.setText((String)iter.next());		
			content.addContent(node);
		}
		
		return content;
	}	
}


class SourceTransitionCreationFactory implements CreationFactory {
	private SourceTransitionModel model;
	private ViewModel viewModel;
	
	public SourceTransitionCreationFactory(SourceTransitionModel model, ViewModel viewModel) {
		this.model = model;
		this.viewModel = viewModel;
	}
	public Object getNewObject() {
		return new MultiTransitionModel(model,viewModel);
		/*
		if ((model.sourceNodes.size()>1) || (model.destNodes.size()>1))
			return new MultiTransitionModel(model,viewModel);
		else
			return new TransitionModel(model,viewModel);
		*/	
	}
	public Object getObjectType() {
		return MultiTransitionModel.class;
		/*
		if ((model.sourceNodes.size()>1) || (model.destNodes.size()>1))		
			return MultiTransitionModel.class;
		else
			return TransitionModel.class;
		*/
	}
}