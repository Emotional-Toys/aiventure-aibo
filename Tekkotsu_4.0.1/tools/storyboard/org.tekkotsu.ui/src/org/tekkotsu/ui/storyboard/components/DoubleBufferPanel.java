/*
 * Created on Nov 17, 2004
 */
package org.tekkotsu.ui.storyboard.components;

import java.awt.Graphics;
import java.awt.Image;
import java.awt.Panel;

/**
 * @author asangpet
 */
public class DoubleBufferPanel extends Panel {
	static final long serialVersionUID = 0xACAC0006;

	private int bufferWidth, bufferHeight;
	private Image bufferImage;
	private Graphics bufferGraphics;
	
	public DoubleBufferPanel() {
		super(null);
	}

	public void update(Graphics g) {
		paint(g);
	}
	
	public void paint(Graphics g) {
		if (bufferWidth!=getSize().width || bufferHeight!=getSize().height
			|| bufferImage==null || bufferGraphics==null)
			resetBuffer();
			
		if (bufferGraphics!=null) {
			bufferGraphics.clearRect(0,0,bufferWidth,bufferHeight);
			//super.paint(bufferGraphics);
			paintBuffer(bufferGraphics);
			g.drawImage(bufferImage,0,0,this);
		}
	}
	
	public void paintChildren(Graphics g) {
		if (bufferGraphics!=null)
			super.paint(g);
	}
	
	private void resetBuffer() {
		bufferWidth = getSize().width;
		bufferHeight = getSize().height;
		
		if (bufferGraphics!=null) {
			bufferGraphics.dispose();
			bufferGraphics=null;
		}
		if (bufferImage != null) {
			bufferImage.flush();
			bufferImage=null;
		}
		System.gc();
		bufferImage = createImage(bufferWidth,bufferHeight);
		bufferGraphics = bufferImage.getGraphics();
	}
	
	
	/**
	 * Paint this component to graphics buffer,
	 * subclass should override this method for drawing
	 * @param g
	 */
	public void paintBuffer(Graphics g) {
	}

}
