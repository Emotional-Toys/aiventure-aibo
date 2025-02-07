package org.tekkotsu.mon;

import javax.swing.*;
import java.awt.*;
import java.awt.geom.*;
import java.awt.event.*;
import java.util.Vector;

public class PointPick extends JComponent {
	static public void main(String s[]) {
		JFrame frame=new JFrame("Point Pick Test");
		frame.setSize(new Dimension(300, 300)); 
		PointPick pp=new PointPick(true);
		frame.getContentPane().setLayout(new BorderLayout());
		frame.getContentPane().add(pp,BorderLayout.CENTER);
		frame.addWindowListener(new WindowAdapter() {
			public void windowClosing(WindowEvent e) { System.exit(0); }
		});
		frame.setVisible(true);
	}
	
	float x=0;
	float y=0;
	float dotsize=8;
	float tracksize=14;
	PPTic mdot;
	PPTic track;
	Vector listeners=new Vector();
	Vector tics=new Vector();
	boolean isCirc=true;
	
	public interface PointPickedListener {
		public void pointPicked(Point2D.Float p, MouseEvent e, PointPick pp);
	}
	public void addPointPickedListener(PointPickedListener ppl) {
		listeners.add(ppl);
	}
	public void removePointPickedListener(PointPickedListener ppl) {
		listeners.remove(ppl);
	}
	public void firePointPicked(Point2D.Float p, MouseEvent e) {
		for(int i=0; i<listeners.size(); i++)
			((PointPickedListener)listeners.get(i)).pointPicked(p,e,this);
	}
	
	public class PPTic extends JComponent {
		float xp=1.0f;
		float yp=1.0f;
		BasicStroke s=new BasicStroke(1);
		Color line=Color.BLACK;
		Color fill=null;
		boolean isCirc=true;
		public PPTic() {}
		public PPTic(float percent, BasicStroke s) {
			xp=yp=percent;
			this.s=s;
		}
		public PPTic(float percent, BasicStroke s, Color line, Color fill) {
			xp=yp=percent;
			this.s=s;
			this.line=line;
			this.fill=fill;
		}
		public void paint(Graphics graphics) {
			Graphics2D g=(Graphics2D)graphics;
			g.setRenderingHint(RenderingHints.KEY_ANTIALIASING,RenderingHints.VALUE_ANTIALIAS_ON);
			float sw=s.getLineWidth();
			float w=getWidth();
			float h=getHeight();
			float l=w*(1-xp)/2;
			float r=l+w*xp;
			float t=h*(1-yp)/2;
			float b=t+h*yp;
			l+=sw/2-.5;
			r-=sw/2+.5;
			t+=sw/2-.5;
			b-=sw/2+.5;
			if(fill!=null) {
				g.setPaint(fill);
				if(isCirc)
					g.fill(new Ellipse2D.Float(l,t,r-l,b-t));
				else
					g.fill(new Rectangle2D.Float(l,t,r-l,b-t));
			}
			if(line!=null) {
				g.setPaint(line);
				g.setStroke(s);
				if(isCirc)
					g.draw(new Ellipse2D.Float(l,t,r-l,b-t));
				else
					g.draw(new Rectangle2D.Float(l,t,r-l,b-t));
			}
			super.paint(graphics);
		}
		public void setCircular(boolean circ) {
			isCirc=circ;
		}
	}
	
	public PointPick(boolean circ) {
		BasicStroke s=new BasicStroke(1);

		track=new PPTic(1,new BasicStroke(2),Color.RED,null);
		track.setBounds((int)((getWidth()*(1+x)-tracksize)/2),(int)((getHeight()*(1+y)-tracksize)/2),(int)tracksize,(int)tracksize);
		track.setVisible(false);
		add(track);

		mdot=new PPTic(1,s,null,isEnabled()?new Color(.7f,.0f,.0f):new Color(.4f,.4f,.4f));
		mdot.setBounds((int)((getWidth()*(1+x)-dotsize)/2),(int)((getHeight()*(1-y)-dotsize)/2),(int)dotsize,(int)dotsize);
		add(mdot);

		PPTic tmp,tmp2;
		tmp=new PPTic(1.0f,new BasicStroke(2),isEnabled()?Color.BLACK:Color.GRAY,Color.WHITE);
		tmp.setBounds(0,0,getWidth(),getHeight());
		tmp.setLayout(new BorderLayout());
		add(tmp);
		tics.add(tmp);
		for(float f=.75f; f>0; f-=.25f) {
			tmp2=new PPTic(f,s,Color.LIGHT_GRAY,null);
			tmp2.setLayout(new BorderLayout());
			tmp.add(tmp2,BorderLayout.CENTER);
			tics.add(tmp2);
			tmp=tmp2;
		}
		tmp2=new PPTic(.0175f,s,null,Color.LIGHT_GRAY);
		tmp2.setLayout(new BorderLayout());
		tmp.add(tmp2,BorderLayout.CENTER);
		tics.add(tmp2);
		tmp=tmp2;
		
		enableEvents(AWTEvent.MOUSE_MOTION_EVENT_MASK+AWTEvent.MOUSE_EVENT_MASK);
		
		setCircular(circ);
	}
	
	public void setCircular(boolean circ) {
		if(isCirc!=circ) {
			for(int i=0; i<tics.size(); i++) {
				((PPTic)tics.get(i)).setCircular(circ);
			}
			setOpaque(!circ);
			isCirc=circ;
			repaint();
		}
	}

	public void paint(Graphics graphics) {
		super.paint(graphics);
		int w=getWidth()-1;
		int h=getHeight()-1;
//		Rectangle r=graphics.getClipBounds();
//		graphics.setColor(Color.WHITE);
//		graphics.fillRect(r.x,r.y,r.width,r.height);
		graphics.setColor(isEnabled()?Color.BLACK:Color.GRAY);
		graphics.drawLine(0,w/2,h,w/2);
		graphics.drawLine(h/2,0,h/2,w);
//		graphics.setColor(isEnabled()?Color.DARK_GRAY:Color.LIGHT_GRAY);
//		graphics.fillOval((int)((getWidth()-dotsize+2)/2),(int)((getHeight()-dotsize+2)/2),(int)dotsize-2,(int)dotsize-2);
	}

	public void setEnabled(boolean b) {
		((PPTic)tics.get(0)).line=b?Color.BLACK:Color.GRAY;
		mdot.fill=b?new Color(.7f,.0f,.0f):new Color(.4f,.4f,.4f);
		super.setEnabled(b);
	}
	
	public void setBounds(int x, int y, int w, int h) {
		if(w>h)
			w=h;
		if(h>w)
			h=w;
		((PPTic)tics.get(0)).setBounds(0,0,w,h);
		mdot.setBounds((int)((w*(1+this.x)-dotsize)/2),(int)((h*(1-this.y)-dotsize)/2),(int)dotsize,(int)dotsize);
		super.setBounds(x,y,w,h);
	}
	
	public void processMouseEvent(MouseEvent e) {
		if(isEnabled()) {
			if(e.getID()==MouseEvent.MOUSE_PRESSED) {
				Point2D.Float mp=screenToModel(e.getPoint());
				Point sp=modelToScreen(mp);
				sp.translate((int)(-tracksize/2),(int)(-tracksize/2));
				track.setLocation(sp);
				track.setVisible(true);
				doSetPoint(mp);
				firePointPicked(mp,e);
			} else if(e.getID()==MouseEvent.MOUSE_RELEASED) {
				if((e.getModifiersEx()&(MouseEvent.BUTTON1_DOWN_MASK|MouseEvent.BUTTON2_DOWN_MASK|MouseEvent.BUTTON3_DOWN_MASK))==0)
					track.setVisible(false);
			}
		}
		super.processMouseEvent(e);
	}

	public void processMouseMotionEvent(MouseEvent e) {
		if(isEnabled()) {
			if(e.getID()==MouseEvent.MOUSE_DRAGGED) {
				Point2D.Float mp=screenToModel(e.getPoint());
				Point sp=modelToScreen(mp);
				sp.translate((int)(-tracksize/2),(int)(-tracksize/2));
				track.setLocation(sp);
				doSetPoint(mp);
				firePointPicked(mp,e);
			}
		}
		super.processMouseMotionEvent(e);
	}
	
	public Point2D.Float getPoint() { return new Point2D.Float(x,y); }
	public float getXValue() { return x; }
	public float getYValue() { return y; }

	public void setPoint(float x, float y) {
		doSetPoint(x,y);
		firePointPicked(new Point2D.Float(x,y),null);
	}
	
	protected Point2D.Float screenToModel(Point p) {
		return screenToModel(p.x,p.y);
	}
	protected Point2D.Float screenToModel(int x, int y) {
		float fx=x/(float)getWidth()*2-1;
		float fy=1-y/(float)getHeight()*2;
		if(isCirc) {
			if(fx*fx+fy*fy>1) {
				double a=Math.atan2(fy,fx);
				fx=(float)Math.cos(a);
				fy=(float)Math.sin(a);
			}
		} else {
			if(fx>1)
				fx=1;
			if(fx<-1)
				fx=-1;
			if(fy>1)
				fy=1;
			if(fy<-1)
				fy=-1;
		}
		return new Point2D.Float(fx,fy);
	}
	protected Point modelToScreen(Point2D.Float p) {
		return modelToScreen(p.x,p.y);
	}
	protected Point modelToScreen(float x, float y) {
		return new Point((int)(getWidth()*(1+x)/2),(int)(getHeight()*(1-y)/2));
	}
	
	public void doSetPoint(Point2D.Float p) {
		doSetPoint(p.x,p.y);
	}
	public void doSetPoint(float x, float y) {
		this.x=x;
		this.y=y;
		Point p=modelToScreen(x,y);
		p.translate((int)(-dotsize/2),(int)(-dotsize/2));
		mdot.setLocation(p);
	}
}
