/*
 * Created on Nov 11, 2004
 */
package org.tekkotsu.ui.storyboard.model;

/**
 * @author asangpet
 */
public class BlockModel extends AbstractModel{	
	public BlockModel(String id, int start, int end, boolean finish) {
		super(id,start,end,finish);
	}
	
	public BlockModel(String id, int start) {
		this(id,start,-1, false);
	}
	
	/* (non-Javadoc)
	 * @see java.lang.Object#equals(java.lang.Object)
	 */
	public boolean equals(Object obj) {
		if (obj==null) return false;
		if (obj instanceof BlockModel) {
			BlockModel comp = (BlockModel)obj;
			return comp.getID().equals(this.getID()) && comp.getStart()==(this.getStart()) && comp.getEnd()==(this.getEnd());
		}
		return false;
	}
}
