/*
 * Created on Oct 12, 2004
 */
package org.tekkotsu.ui.editor.model;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

import org.eclipse.draw2d.geometry.Rectangle;
import org.eclipse.swt.graphics.RGB;
import org.eclipse.ui.views.properties.IPropertyDescriptor;
import org.eclipse.ui.views.properties.TextPropertyDescriptor;
import org.jdom.Content;
import org.jdom.Element;
import org.tekkotsu.ui.editor.resources.Debugger;
import org.tekkotsu.ui.editor.resources.IDTag;

/**
 * Multi-transition model, which actually looks like a node.
 * 
 * @author asangpet
 */
public class MultiTransitionModel extends StateNodeModel {
	public static final String P_LINEWIDTH = "_linewidth";

	protected int lineWidth = 1;

	List subConnectionList = new ArrayList();

	private MultiTransitionModel() {
		super();
		Debugger.printDebug(Debugger.DEBUG_ALL,
				"execute MultiTransitionModel()");
		setId("transition" + Integer.toString(stateID++));
		setLabel(getId());
		setShape(StateNodeModel.P_SHAPE_STYLE_ELLIPSE);
		setColor(new RGB(0, 0, 0));
	}

	/* (non-Javadoc)
	 * @see org.tekkotsu.ui.editor.model.StateNodeModel#setColor(org.eclipse.swt.graphics.RGB)
	 */
	public void setColor(RGB color) {		
		super.setColor(color);
		Iterator iter = subConnectionList.iterator();
		while (iter.hasNext()) {
			SubConnectionModel sub = (SubConnectionModel)iter.next();
			sub.setColor(getColor());
		}
	}
	
	public MultiTransitionModel(Element xml, ViewModel parent) {
		this();
		Debugger.printDebug(Debugger.DEBUG_ALL, "execute MultiTransitionModel("
				+ xml + "," + parent + ")");
		if (IDTag.XML_transition_tag.equals(xml.getName())) {
			try {
				super.initModelConfig(xml, parent);
				setId(xml.getAttributeValue(IDTag.XML_transition_id));
				setLabel(xml.getAttributeValue(IDTag.XML_common_label));
				SourceTransitionModel model = parent.getSourceModel()
						.getTransition(this.getId());
				setLineWidth(Integer.parseInt(xml
						.getAttributeValue(IDTag.XML_transition_linewidth)));
				initModel(model, parent);
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
	}

	/**
	 * A constructor which were used when user add transition to the layout
	 * 
	 * @param source
	 *            Model source for this transition
	 * @param parent
	 *            View model which will contain this transition
	 */
	public MultiTransitionModel(SourceTransitionModel source, ViewModel parent) {
		this();
		Debugger.printDebug(Debugger.DEBUG_ALL, "execute MultiTransitionModel("
				+ source + "," + parent + ")");
		initModel(source, parent);
	}

	/**
	 * Perform initial action after created the transition node. Attach all
	 * available connection from source to transition node and from transition
	 * node to destination
	 */
public void doPostAdd() {
		doPreDelete((ViewModel) (this.getParent()));
		/* Connect associate transition to multitransition node */
		SourceTransitionModel source = (SourceTransitionModel) getSource();
		ViewModel parent = (ViewModel) getParent();
		subConnectionList = new ArrayList();

		
		if ((source.getSourceNodes().size() == 1) &&
		  (source.getDestNodes().size()==1)) {
			StateNodeModel sourceNode = (StateNodeModel) parent.getPartChild((String)source.getSourceNodes().get(0));
			StateNodeModel destNode = (StateNodeModel)parent.getPartChild((String)source.getDestNodes().get(0));
			SubConnectionModel connx = new SubConnectionModel(this, sourceNode,destNode, SubConnectionModel.ARROW_ARROWHEAD, lineWidth);
			subConnectionList.add(connx);
			if (!isPreview()) connx.setColor(this.getColor());
			else connx.setColor(this.getPreviewColor());
			return;
		}
		 

		// generate src/target set.

		// connect source node
		Iterator iter = source.getSourceNodes().iterator();
		while (iter.hasNext()) {
			String srcId = (String) iter.next();
			StateNodeModel sourceNode = (StateNodeModel) parent
					.getPartChild(srcId);
			if (sourceNode != null) {
				SubConnectionModel inConnx = new SubConnectionModel(this,
						sourceNode, this, SubConnectionModel.ARROW_NOHEAD,
						lineWidth);
				subConnectionList.add(inConnx);
				// parent.addChild(inConnx);
				if (!isPreview()) inConnx.setColor(this.getColor());
				else inConnx.setColor(this.getPreviewColor());
			}
		}

		// connect destination node
		iter = source.getDestNodes().iterator();
		while (iter.hasNext()) {
			String srcId = (String) iter.next();
			StateNodeModel destNode = (StateNodeModel) parent
					.getPartChild(srcId);
			if (destNode != null) {
				SubConnectionModel outConnx = new SubConnectionModel(this,
						this, destNode, lineWidth);
				subConnectionList.add(outConnx);
				// parent.addChild(outConnx);
				if (!isPreview()) outConnx.setColor(this.getColor());
				else outConnx.setColor(this.getPreviewColor());
			}
		}
		// }
	}
	/**
	 * Perform finalize action before removal of transition node. (Detach all
	 * available connections from the content)
	 * 
	 * @param contents
	 */
	public void doPreDelete(ViewModel contents) {
		List subConn = new ArrayList();
		subConn.addAll(subConnectionList);
		for (Iterator iter = subConn.iterator(); iter.hasNext();) {
			SubConnectionModel model = (SubConnectionModel) iter.next();
			Debugger.printDebug(Debugger.DEBUG_ALL, ("Detach " + model));
			model.detachSource();
			model.detachTarget();
			// contents.removeChild(model);
		}
	}
	
	public void removeSubConnection(SubConnectionModel model) {
		subConnectionList.remove(model);		
	}

	/**
	 * @return Returns the lineWidth.
	 */
	public int getLineWidth() {
		return lineWidth;
	}

	public IPropertyDescriptor[] getPropertyDescriptors() {
		IPropertyDescriptor[] parentDesc = super.getPropertyDescriptors();
		ArrayList list = new ArrayList();
		for (int i = 0; i < parentDesc.length; i++)
			list.add(parentDesc[i]);
		list.add(new TextPropertyDescriptor(P_LINEWIDTH, "Line width"));
		return (IPropertyDescriptor[]) list.toArray(new IPropertyDescriptor[1]);
	}

	public Object getPropertyValue(Object id) {
		if (id.equals(P_LINEWIDTH)) {
			return String.valueOf(getLineWidth());
		} else
			return super.getPropertyValue(id);
	}

	/**
	 * @return Returns the subConnectionList.
	 */
	public List getSubConnectionList() {
		return subConnectionList;
	}

	public Content getXML() {
		Element content = new Element(IDTag.XML_transition_tag);
		content.setAttribute(IDTag.XML_state_id, getId());
		if (getSource().getClassName() != null)
			content.setAttribute(IDTag.XML_state_class, getSource()
					.getClassName());
		content.setAttribute(IDTag.XML_state_label, getLabel());
		content.setAttribute(IDTag.XML_state_color, IDTag.RGB2Tag(getRGB()));
		Rectangle rect = getConstraint();
		content.setAttribute(IDTag.XML_state_top, "" + rect.y);
		content.setAttribute(IDTag.XML_state_left, "" + rect.x);
		content.setAttribute(IDTag.XML_state_width, "" + rect.width);
		content.setAttribute(IDTag.XML_state_height, "" + rect.height);
		content.setAttribute(IDTag.XML_state_shape, getShape());
		content.setAttribute(IDTag.XML_transition_linewidth, "" + lineWidth);
		return content;
	}

	public void initModel(SourceTransitionModel source, ViewModel parent) {
		setShape(StateNodeModel.P_SHAPE_STYLE_ELLIPSE);
		setId(source.getId());
		//setLabel(source.getId());

		super.setSource(source);
		Debugger.printDebug(Debugger.DEBUG_ALL, "src:"
				+ source.getSourceNodes());
		Debugger
				.printDebug(Debugger.DEBUG_ALL, "dest:" + source.getDestNodes());

		// setConstraint(new Rectangle(0,0,0,0));
	}

	public boolean isPropertySet(Object id) {
		return id.equals(P_LINEWIDTH) || super.isPropertySet(id);
	}

	/**
	 * @param lineWidth
	 *            The lineWidth to set.
	 */
	public void setLineWidth(int lineWidth) {
		this.lineWidth = lineWidth;
		firePropertyChange(P_LINEWIDTH, null, new Integer(lineWidth));
	}

	public void setPropertyValue(Object id, Object value) {
		if (id.equals(P_LINEWIDTH)) {
			setLineWidth(Integer.parseInt(value.toString()));
		} else
			super.setPropertyValue(id, value);
	}

	public boolean isSingleTransition() {
		SourceTransitionModel source = (SourceTransitionModel) getSource();
		return source.getSourceNodes().size() == 1
				&& source.getDestNodes().size() == 1;
	}
	
	public void activate() {
		super.activate();
		Iterator iter = subConnectionList.iterator();
		while (iter.hasNext()) {
			SubConnectionModel sub = (SubConnectionModel)iter.next();
			sub.firePropertyChange(MultiTransitionModel.P_COLOR,null,this.getColor());
		}				
	}

	public void deactivate() {
		super.deactivate();
		Iterator iter = subConnectionList.iterator();
		while (iter.hasNext()) {
			SubConnectionModel sub = (SubConnectionModel)iter.next();
			sub.firePropertyChange(MultiTransitionModel.P_COLOR,null,this.getPreviewColor());
		}						
	}
}
