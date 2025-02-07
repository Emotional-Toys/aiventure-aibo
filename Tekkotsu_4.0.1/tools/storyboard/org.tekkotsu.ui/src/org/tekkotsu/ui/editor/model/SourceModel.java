/*
 * Created on Oct 4, 2004
 */
package org.tekkotsu.ui.editor.model;

import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.Reader;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;

import org.eclipse.core.runtime.IPath;
import org.jdom.Document;
import org.jdom.Element;
import org.jdom.input.SAXBuilder;
import org.jdom.output.Format;
import org.jdom.output.XMLOutputter;
import org.tekkotsu.ui.editor.resources.Debugger;
import org.tekkotsu.ui.editor.resources.IDTag;

/**
 * This class represents the organization of model file
 * @author asangpet
 */
public class SourceModel {
	List nodes = new ArrayList();
	List transitions = new ArrayList();
	
	Map nodeMap = new HashMap();
	Map transitionMap = new HashMap();
	
	IPath sourcePath = null;
	
	public SourceModel(IPath sourceFile) {
		setSourcePath(sourceFile);
	}
	
	public SourceModel() {}
	
	public void createModel(Element xml) {
		try {
			nodeMap = new HashMap();
			transitionMap = new HashMap();
			nodes = new ArrayList();
			transitions = new ArrayList();
			if ("model".equals(xml.getName())) {
				Iterator iter = xml.getDescendants();
				while (iter.hasNext()) {					
					Object next = iter.next();					
					if (next instanceof Element) {
						Element child = (Element) next;
						if (IDTag.XML_state_tag.equals(child.getName())) {							
							SourceNodeModel temp = new SourceNodeModel(child,this);
							nodes.add(temp);
							nodeMap.put(temp.getId(),temp);
						} else if (IDTag.XML_transition_tag.equals(child.getName())) {
							SourceTransitionModel temp = new SourceTransitionModel(child);
							transitions.add(new SourceTransitionModel(child));				
							transitionMap.put(temp.getId(),temp);
						}
					}
				}
			}
		} catch (Exception e) {
			e.printStackTrace();
		}		
	}
	
	public SourceNodeModel getNode(String id) {
		return (SourceNodeModel)nodeMap.get(id);
	}
	
	public SourceTransitionModel getTransition(String id) {
		return (SourceTransitionModel)transitionMap.get(id);
	}
	
	public SourceObjectModel getChild(String id) {
		SourceObjectModel result = (SourceObjectModel)nodeMap.get(id);
		if (nodeMap.get(id)!=null) return result;
		else return (SourceObjectModel)transitionMap.get(id);
	}
	
	public List getNodes() {
		return nodes;
	}
	
	public List getTransitions() {
		return transitions;
	}
	/**
	 * @return Returns the sourcePath.
	 */
	public IPath getSourcePath() {
		return sourcePath;
	}
	
	public boolean setSourcePath(IPath path) {
		this.sourcePath = path;
		
		SAXBuilder builder = new SAXBuilder();
		try {
			Document sourcedoc = builder.build(path.toFile());
			createModel(sourcedoc.getRootElement());
			return true;
		} catch (Exception e) {
		    // attempt to create new model file
		    this.doSave();
			// error reading input file
			//e.printStackTrace();
		}		
		return false;
	}

	public void setSourceInput(Reader reader) {
		SAXBuilder builder = new SAXBuilder();
		try {
			Document sourcedoc = builder.build(reader);
			createModel(sourcedoc.getRootElement());	
		} catch (Exception e) {
			// error reading input file
			e.printStackTrace();
		}				
	}
	
	public void doSave(IPath outputPath) {
		try {
			Document doc = new Document();			
			doc.addContent(getXML());
		
			XMLOutputter outputter = new XMLOutputter(Format.getPrettyFormat());
			Debugger.printDebug(Debugger.DEBUG_ALL,"Attempting to create "+outputPath);
			BufferedOutputStream ostream = new BufferedOutputStream(new FileOutputStream(outputPath.toOSString()));
			outputter.output(doc,ostream);

			ostream.close();
		} catch (Exception e) {
			//e.printStackTrace();
		}
	}
	
	public void doSave() {
		doSave(this.getSourcePath());
	}
	
	public Element getXML() {
		Element content = new Element("model");
		
		List nodes = getNodes();
		List transitions = getTransitions();

		Iterator iter = nodes.iterator();
		while (iter.hasNext()) {
			SourceNodeModel nodeModel = (SourceNodeModel)iter.next();
			content.addContent(nodeModel.getXML());
		}
		
		iter = transitions.iterator();
		while (iter.hasNext()) {
			SourceTransitionModel transModel = (SourceTransitionModel)iter.next();
			content.addContent(transModel.getXML());
		}
		return content;
	}
	
}