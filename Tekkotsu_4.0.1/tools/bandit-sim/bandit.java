/*
* @(#)SwingShapeMover.java   1.2 98/07/31
*/


import java.awt.*;
import java.awt.event.*;
import java.awt.image.*;
import javax.swing.*;
import java.util.Random;
import javax.sound.sampled.*;
import java.io.File;
import java.io.IOException;

/*
 * This applet allows the user to move a texture painted rectangle around the applet
 * window.  The rectangle flickers and draws slowly because this applet does not use 
 * double buffering.
*/

public class bandit extends JApplet {
    static protected JLabel label;
    DPanel d;

    public void init(){
        getContentPane().setLayout(new BorderLayout());

		getContentPane().setBackground(Color.black);

        label = new JLabel("Press keys to play...");
		label.setBackground(Color.black);
		label.setForeground(Color.white);

        d = new DPanel(label);
        d.setBackground(Color.black);
        getContentPane().add(d);

        getContentPane().add("South", label);
        
        TextField tmp = new TextField(3);
        tmp.setBackground(Color.black);
        getContentPane().add("North",tmp);
        
        tmp.addKeyListener(d);
        tmp.requestFocus();

        (new MoveBall(d)).start();
    }

    public static void main(String s[]) {
        JFrame f = new JFrame("k-Armed Bandit");
        f.addWindowListener(new WindowAdapter() {
            public void windowClosing(WindowEvent e) {System.exit(0);}
        });
        JApplet applet = new bandit();
        f.getContentPane().add("Center", applet);
        applet.init();
        f.pack();
        f.setSize(new Dimension(700,500));
        f.show();
    }
    
}

class MoveBall extends Thread {
	DPanel panel;
	long last;
	Random rand;
	double perterb;
	public MoveBall(DPanel dp) {
		panel=dp;
		last=System.currentTimeMillis();
		rand = new Random(last);
		perterb=20;
	}
	public void run() {
		while(true) {
			long now=System.currentTimeMillis();
			double diff=(now-last)/1000.0;
			last=now;
			double d=panel.dx*diff;
			panel.x=panel.x+(d>panel.maxd?panel.maxd:d);
			d=panel.dy*diff;
			panel.y=panel.y+(d>panel.maxd?panel.maxd:d);
			if(panel.x+panel.r>panel.getSize().width) {
				panel.dx=-Math.abs(panel.dx);
				panel.x=panel.getSize().width-panel.r;
				panel.dy=panel.dy+(rand.nextDouble()-.5)*perterb;
			} else if(panel.x-panel.r<0) {
				panel.dx=Math.abs(panel.dx);
				panel.x=panel.r;
				panel.dy=panel.dy+(rand.nextDouble()-.5)*perterb;
			}
			if(panel.y+panel.r>panel.getSize().height) {
				panel.dy=-Math.abs(panel.dy);
				panel.y=panel.getSize().height-panel.r;
				panel.dx=panel.dx+(rand.nextDouble()-.5)*perterb;
			} else if(panel.y-panel.r<0) {
				panel.dy=Math.abs(panel.dy);
				panel.y=panel.r;
				panel.dx=panel.dx+(rand.nextDouble()-.5)*perterb;
			}
			panel.repaint();
			try {
				sleep(1000/30);
			} catch(Exception e) {}
		}
	}
}

class ShowReward extends Thread {
	DPanel panel;
	public ShowReward(DPanel dp) {
		panel=dp;
	}
	public void run() {
		panel.reward=true;
		try {
			sleep(1500);
		} catch(Exception e) {
			interrupted();
		}
		panel.reward=false;
		panel.showreward=null;
	}
}

class DPanel extends JPanel implements KeyListener {
	BufferedImage redball;
	BufferedImage pinkball;
	protected double dx,dy;
	protected double x,y,r;
	protected double maxd;
	double probLeft,probRight;
	long lastkeypress;
	Random rand;
	protected boolean reward;
	protected ShowReward showreward;
	int trial;
	Clip rsnd;
	Clip lsnd;

	JLabel label;

    public DPanel(JLabel l) {
    	label=l;
		dx=200;
		dy=80;
		x=130;
		y=150;
		r=150;
		maxd=30;
		probLeft=.2;
		probRight=.7;
		lastkeypress=0;
		rand = new Random(System.currentTimeMillis());
		reward=false;
		showreward=new ShowReward(this);
		trial=0;
		DataLine.Info info;

		redball = new BufferedImage((int)(r*2+maxd*2), (int)(r*2+maxd*2), BufferedImage.TYPE_INT_RGB);
		Graphics2D big = redball.createGraphics();
		big.setColor(new Color(220,44,0));
		big.fillOval((int)maxd, (int)maxd, (int)(r*2), (int)(r*2));
		pinkball = new BufferedImage((int)(r*2+maxd*2), (int)(r*2+maxd*2), BufferedImage.TYPE_INT_RGB);
		big = pinkball.createGraphics();
		big.setColor(new Color(255,0,0));
		big.fillOval((int)maxd, (int)maxd, (int)(r*2), (int)(r*2));

		try {
			info = new DataLine.Info(Clip.class, AudioSystem.getAudioFileFormat(new File("reward.aif")).getFormat());
		} catch(Exception e) {
			System.out.println("Sound file not found: "+e);
			return;
		}
		if (!AudioSystem.isLineSupported(info)) { System.out.println("Sound is screwy"); }
		try {
			rsnd = (Clip)AudioSystem.getLine(info);
			rsnd.open(AudioSystem.getAudioInputStream(new File("reward.aif")));
		} catch (Exception ex) {
			System.out.println("Couldn't load reward sound"+ex);
		}
		try {
			info = new DataLine.Info(Clip.class, AudioSystem.getAudioFileFormat(new File("loss.aif")).getFormat());
		} catch(Exception e) {
			System.out.println("Sound file not found: "+e);
			return;
		}
		if (!AudioSystem.isLineSupported(info)) { System.out.println("Sound is screwy"); }
		try {
			lsnd = (Clip)AudioSystem.getLine(info);
			lsnd.open(AudioSystem.getAudioInputStream(new File("loss.aif")));
		} catch (Exception ex) {
			System.out.println("Couldn't load loss sound"+ex);
		}

    }
    public void paintComponent(Graphics g){
		super.paintComponent(g);
		Graphics2D g2 = (Graphics2D)g;
		if(reward)
    	    g2.drawImage(pinkball,null,(int)(x-maxd-r),(int)(y-maxd-r));
        else
	        g2.drawImage(redball,null,(int)(x-maxd-r),(int)(y-maxd-r));
	}
	public void keyPressed(KeyEvent e) {
		char c=e.getKeyChar();
		float x=getKeyboardPos(c);
		if(x<0) {
			if(c>='0' && c<='9') {
				probLeft=(((char)c)-'0')/10.0;
			} else {
				switch(c) {
					case '!':
						probRight=.1; break;
					case '@':
						probRight=.2; break;
					case '#':
						probRight=.3; break;
					case '$':
						probRight=.4; break;
					case '%':
						probRight=.5; break;
					case '^':
						probRight=.6; break;
					case '&':
						probRight=.7; break;
					case '*':
						probRight=.8; break;
					case '(':
						probRight=.9; break;
					case ')':
						probRight=0; break;
				}
			}
			label.setText("P(R|Left)="+probLeft+"  P(R|Right)="+probRight+"   Trial="+trial);
		} else if(e.getWhen()-1500>lastkeypress){
			double prob=(x>.5)?probRight:probLeft;
			double choice=rand.nextDouble();
			System.out.println(prob+" "+choice);
			String text;
			trial++;
			if(choice<prob) {
				text="Reward! :)";
				if(showreward!=null)
					showreward.interrupt();
				showreward=new ShowReward(this);
				showreward.start();
				if(rsnd!=null) {
						rsnd.setMicrosecondPosition(0);
						rsnd.start();
				}
			} else {
				text="No reward. :(";
				if(showreward!=null)
					showreward.interrupt();
				if(lsnd!=null) {
						lsnd.setMicrosecondPosition(0);
						lsnd.start();
				}
			}
			label.setText("P(R|Left)="+probLeft+"  P(R|Right)="+probRight+"   Trial="+trial+(x>.5?"     Right - ":"     Left - ")+text);
			lastkeypress=e.getWhen();
		}
	}
	public void keyReleased(KeyEvent e) {}
	public void keyTyped(KeyEvent e) {}
	float getKeyboardPos(char c) {
		double x;
		switch(c) {
			case 'q':
				x=0; break;
			case 'a':
				x=.5; break;
			case 'w':
			case 'z':
				x=1; break;
			case 's':
				x=1.5; break;
			case 'e':
			case 'x':
				x=2; break;
			case 'd':
				x=2.5; break;
			case 'r':
			case 'c':
				x=3; break;
			case 'f':
				x=3.5; break;
			case 't':
			case 'v':
				x=4; break;
			case 'g':
				x=4.5; break;
			case 'y':
			case 'b':
				x=5; break;
			case 'h':
				x=5.5; break;
			case 'u':
			case 'n':
				x=6; break;
			case 'j':
				x=6.5; break;
			case 'i':
			case 'm':
				x=7; break;
			case 'k':
				x=7.5; break;
			case 'o':
			case ',':
				x=8; break;
			case 'l':
				x=8.5; break;
			case 'p':
			case '.':
				x=9; break;
			case ';':
				x=9.5; break;
			case '[':
			case '/':
				x=10; break;
			case '\'':
				x=10.5; break;
			case ']':
				x=11; break;
			default:
				x=-11; break;
		}
		return (float)(x/11);
	}
}
