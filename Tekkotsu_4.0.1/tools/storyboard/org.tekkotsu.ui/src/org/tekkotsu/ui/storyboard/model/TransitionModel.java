/*
 * Created on Nov 17, 2004
 */
package org.tekkotsu.ui.storyboard.model;

import java.util.LinkedList;
import java.util.List;

import org.tekkotsu.ui.editor.model.MultiTransitionModel;
import org.tekkotsu.ui.editor.model.SourceTransitionModel;

/**
 * @author asangpet
 *
 */
public class TransitionModel extends AbstractModel {
	List sourceNodeList, destNodeList;
	int endCount = 0;
	
	public TransitionModel(String id, int start, MultiTransitionModel trans) {
		super(id,start,-1, true);
		sourceNodeList = new LinkedList();
		destNodeList = new LinkedList();
		
		SourceTransitionModel strans = (SourceTransitionModel)trans.getSource();
		sourceNodeList.addAll(strans.getSourceNodes());				
		destNodeList.addAll(strans.getDestNodes());
	}

	public boolean isSourceNode(String id) {
		return sourceNodeList.contains(id);
	}
	
	public boolean isDestNode(String id) {
		return destNodeList.contains(id);
	}
	
	public int getTime() {
		return start;
	}
	
	public void setEnd(int end) {
		if (this.end>=0) {
			endCount++;
			super.setEnd((end*(endCount-1)+end)/endCount);
		} else {
			super.setEnd(end);		
		}
	}
}
