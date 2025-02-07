package org.tekkotsu.ui.storyboard.model;

public class AbstractModel {
	int start, end;	//raw time (integer in ms)
	boolean finish = false;
	String id;
	
	public AbstractModel(String id, int start, int end, boolean finish) {
		this.id = id;
		this.start = start;
		this.end = end;
		this.finish = finish;
	}
	
	public AbstractModel(String id, int start) {
		this(id,start,-1, false);
	}
	
	public String getID() { return id; }
	public int getStart() { return start; }
	public int getEnd() { return end; }
	public void setEnd(int end) { this.end = end; this.finish = true; }
	
	public boolean isFinish() {
		return finish;
	}
	
	public String toString() {
		return super.toString() + id + ":" + start+":"+end;
	}
	
	/* (non-Javadoc)
	 * @see java.lang.Object#equals(java.lang.Object)
	 */
	public boolean equals(Object obj) {
		if (obj==null) return false;
		if (obj instanceof AbstractModel) {
			AbstractModel comp = (AbstractModel)obj;
			return comp.getID().equals(this.getID()) && comp.getStart()==(this.getStart()) && comp.getEnd()==(this.getEnd());
		}
		return false;
	}

}
