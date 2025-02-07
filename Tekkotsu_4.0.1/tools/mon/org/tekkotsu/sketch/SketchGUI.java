package org.tekkotsu.sketch;

import org.tekkotsu.mon.*;

import java.awt.event.*;
import javax.swing.*;
import javax.swing.tree.*;
import javax.swing.event.*;
import java.lang.String;
import java.awt.*;
import javax.imageio.ImageIO;
import java.awt.geom.AffineTransform;
import java.awt.image.BufferedImage;
import java.awt.image.IndexColorModel;
import java.util.Date;
import java.util.Vector;
import java.util.Arrays;
import java.util.Enumeration;
import java.util.Comparator;
import java.util.Hashtable;
import java.io.PrintWriter;
import java.io.FileOutputStream;
import java.util.prefs.Preferences;
import java.io.File;
import java.net.*;
import java.io.*;
import java.util.StringTokenizer;
import java.awt.geom.*;


public class SketchGUI extends JFrame implements ActionListener,
						 TreeSelectionListener,
						 VisionUpdatedListener,
						 MouseListener
{
    public enum Space { unknown, cam, local, world }
    Space space;          // 1 == cam, 2 == local, 3 == world

    JFrame sketchFrame;
    SketchPanel sketchPanel;
    JButton rescaleBut;
    JButton refreshListBut;
    JCheckBox checkAllBut;
    boolean defaultSelected = true;
    JCheckBox invertAllBut;
    boolean defaultInversion = false;

    JTree sketchTree = null;
    DefaultMutableTreeNode root;
    AffineTransform Tmat, TmatInv;
    JLabel status;
    float mspf=0;
    float mspfGamma = 0.9f;
    String state="";
    String host;
    int listingPort;
    int sketchPort;
    static Preferences prefs = Preferences.userNodeForPackage(SketchGUI.class);

    // the socket over which listings are retrieved and Sketch commands are sent
    Socket listingSocket=null;
    PrintWriter netout = null; // network output
    BufferedReader netin = null; // network input

    VisionListener listener;

    PopupMenu popMenu;
    SketchOrShapeInfo currentMenuObject;

    Vector sketchStack;
    BufferedImage img;
    boolean imageNeedsUpdate;
    boolean focusJustChanged = false;
    TreeSelectionEvent lastEvent = null;

    int imageWidth = -1, imageHeight = -1;

    Color backgroundColor = new Color(64,64,64);
    int defaultWidth = 208, defaultHeight = 160; // Use ERS-7 defaults if it cannot connect. 
    boolean panelBoundsSet = false;

    SketchPanel curSketchPanel;
    SketchOrShapeInfo curObject;
    String panelTitle;
    int panelCount = 1;

    public static void main(String args[]) {
	if(args.length<2)
	     usage();

	Space _space = Space.unknown;
	int _listingPort = 0;
	int _sketchPort = 0;
	if ( args[1].equals("cam") ) {
	    _space = Space.cam;
	    _listingPort = 5800;
	    _sketchPort = 5801;
	}
	else if ( args[1].equals("local") ) {
	    _space = Space.local;
	    _listingPort = 5802;
	    _sketchPort = 5803;
	}
	else if ( args[1].equals("world") ) {
	    _space = Space.world;
	    _listingPort = 5804;
	    _sketchPort = 5805;
	}
	else usage();

	SketchGUI gui = new SketchGUI(args[0], _listingPort, _sketchPort, _space);
	gui.addWindowListener(new WindowAdapter() {
		 public void windowClosing(WindowEvent e) {System.exit(0);}});
	gui.setVisible(true);
    }

    public static void usage() {
	System.out.println("Usage: java org/tekkotsu/sketch/SketchGUI hostname [cam/local/world]");
	System.exit(2);
    }

    public void actionPerformed(ActionEvent e) {
	try {
	    if(e.getActionCommand().compareTo("rescale")==0)
		rescaleAction();
	    else if(e.getActionCommand().compareTo("refreshlist")==0)
		refreshlistAction();
	    else if(e.getActionCommand().compareTo("checkall")==0)
		checkAllAction(e);
	    else if(e.getActionCommand().compareTo("invertall")==0)
		invertAllAction(e);
	}
	catch(IOException ioe) {
	    System.err.println("Transfer error");
	    reconnect();
	    actionPerformed(e);
	}
    }

    public void checkAllAction(ActionEvent e) {
	defaultSelected = ((JCheckBox)(e.getSource())).isSelected();

	DefaultMutableTreeNode n = root;
	Vector<TreePath> newPaths = new Vector<TreePath>();
	TreePath[] paths = sketchTree.getSelectionPaths();

	if (paths != null) {
	    for (int i=0; i<paths.length; i++) 
		newPaths.add(paths[i]);
	}
	while((n = n.getNextNode())!= null) {
	    if (n.getUserObject() instanceof SketchOrShapeInfo) {
		SketchOrShapeInfo obj = (SketchOrShapeInfo)n.getUserObject();
		if (defaultSelected) {
		    // Reset all shapes to follow the default when turning on select all
		    if (obj instanceof ShapeInfo) {
			((ShapeInfo)obj).setUseDefaultSelection(true);
			newPaths.addElement(new TreePath(n.getPath()));
		    }
		}
		else {
		    if (obj instanceof ShapeInfo && ((ShapeInfo)obj).getUseDefaultSelection()) {
			newPaths.removeElement(new TreePath(n.getPath()));
			obj.setVisible(false);
		    }
		}
	    }
	}
	sketchTree.setSelectionPaths((TreePath[])newPaths.toArray(new TreePath[0]));
    }

    public void invertAllAction(ActionEvent e) {

	defaultInversion = ((JCheckBox)(e.getSource())).isSelected();

	DefaultMutableTreeNode n = root;
	while((n = n.getNextNode())!= null) {
	    if (n.getUserObject() instanceof SketchOrShapeInfo) {
		SketchOrShapeInfo obj = (SketchOrShapeInfo)n.getUserObject();
		// Reset all shapes to follow the default when turning on invert all
		if (defaultInversion && obj instanceof ShapeInfo) {
		    ((ShapeInfo)obj).setUseDefaultInversion(true);
		}
		if (obj instanceof ShapeInfo && ((ShapeInfo)obj).getUseDefaultInversion()) {
		    obj.setInverted(defaultInversion);
		}
	    }
	}
	valueChanged(lastEvent);
	sketchTree.treeDidChange();
    }

    public void rescaleAction() {
	curSketchPanel.leftBound = 0;
	curSketchPanel.rightBound = imageWidth;
	curSketchPanel.topBound = 0;
	curSketchPanel.bottomBound = imageHeight;
	TreePath[] paths = sketchTree.getSelectionPaths();
	if(paths!=null) {
	    for (int path_i = 0; path_i < paths.length; path_i++) {
		DefaultMutableTreeNode node 
		    =(DefaultMutableTreeNode)(paths[path_i].getLastPathComponent());
		
		if (node == root) continue;
		if(!(node.getUserObject() instanceof SketchOrShapeInfo)) {
		    System.out.println("rescaleAction:: placeholder text can't be selected");
		    continue;
		}
		
		SketchOrShapeInfo vinfo = (SketchOrShapeInfo)(node.getUserObject());
		if ( vinfo.getVisible() )
		    curSketchPanel.scaleToSketchOrShape(vinfo);
		if (vinfo instanceof SketchInfo)
		    {
			((SketchInfo)vinfo).unloadImage();
		    }
	    }
	}
	valueChanged(null); // redraw
    }

    public void refreshlistAction() throws IOException {

	// Save existing selection settings for any objects that might remain
	Hashtable oldInverted = new Hashtable();
	Hashtable oldSelected = new Hashtable();

	DefaultMutableTreeNode n = root;
	if (n!= null) {
	    while((n = n.getNextNode())!= null) {
		if (n.getUserObject() instanceof ShapeInfo) {
		    ShapeInfo sinfo = (ShapeInfo)n.getUserObject();
		    if (!sinfo.getUseDefaultSelection())
			oldSelected.put(new Integer(sinfo.getId()),new Boolean(sinfo.getVisible()));
		    if (!sinfo.getUseDefaultInversion())
			oldInverted.put(new Integer(sinfo.getId()),new Boolean(sinfo.inverted));
		} else if (n.getUserObject() instanceof SketchInfo) {
		    ((SketchInfo)n.getUserObject()).unloadImage();
		}
	    }
	}

	// send command to refresh the sketch tree list
	if(netout==null) reconnect();
	if(netout==null) return;
	root.removeAllChildren();
	sketchStack = new Vector();

	String inputLine;
	
	if (!panelBoundsSet){
	    // Get image bounds from the robot
	    netout.println("size");
	    System.out.println(inputLine = readLine());
	    while((inputLine=readLine()).compareTo("size end") != 0) {
		System.out.println(inputLine);
		StringTokenizer sizetoken = new StringTokenizer(inputLine);
		String token = sizetoken.nextToken();
		if (token.equals("width")){
		    imageWidth = Integer.parseInt(sizetoken.nextToken());
		}
		else if (token.equals("height")){
		    imageHeight = Integer.parseInt(sizetoken.nextToken());
		}
	    }
	    System.out.println(inputLine);
	    panelBoundsSet = true;
	}

	sketchPanel.leftBound = 0;
	sketchPanel.rightBound = imageWidth;
	sketchPanel.topBound = 0;
	sketchPanel.bottomBound = imageHeight;

	System.out.println("Setting bounds to "+sketchPanel.rightBound + " x "+sketchPanel.bottomBound);
	
	// read in new sketch/shape list
	netout.println("list");
	System.out.println(inputLine = readLine());  // eat the "list begin"
	while((inputLine=readLine()).compareTo("list end") != 0) {
	    // parse type (sketch or shape)	
	    StringTokenizer st = new StringTokenizer(inputLine,": ");
	    String type = st.nextToken();
	    
	    if (type.equals("tmat")) {
		float[] tvals = new float[16];
		inputLine = readLine();
		st = new StringTokenizer(inputLine,": ");
		for (int i=0; i<16; i++)
		    tvals[i] = Float.parseFloat(st.nextToken());
		float s = 1/tvals[15];
		System.out.println("tmat:  tx="+tvals[3]+"  ty="+tvals[7]+"  scale="+s);
		Tmat.setTransform(tvals[0]*s, tvals[1]*s, tvals[4]*s, tvals[5]*s, tvals[3]*s, tvals[7]*s);
		try { TmatInv.setTransform(Tmat.createInverse()); } 
		catch (java.awt.geom.NoninvertibleTransformException nte) 
		    { System.out.println("Error occured while trying to make the inverse of Tmat"); }
		continue;
	    }

	    // if we get here, it's a sketch or a shape

	    // parse id
	    inputLine = readLine();
	    st = new StringTokenizer(inputLine,": ");
	    st.nextToken();
	    int id = Integer.parseInt(st.nextToken());
	    
	    //parse parentId
	    inputLine = readLine();
	    st = new StringTokenizer(inputLine,": ");
	    st.nextToken();
	    int parentId = Integer.parseInt(st.nextToken());
	    
	    // parse name
	    inputLine = readLine();
	    st = new StringTokenizer(inputLine,":\r\n");
	    st.nextToken();
	    String name = st.nextToken();
	    
	    // parse sketch or shape subtype
	    inputLine = readLine();
	    st = new StringTokenizer(inputLine,": ");
	    st.nextToken();
	    int subtype = Integer.parseInt(st.nextToken());
	    
	    // parse color
	    inputLine = readLine();
	    st = new StringTokenizer(inputLine,": ");
	    st.nextToken();
	    Color color = new Color( Integer.parseInt(st.nextToken()),
				     Integer.parseInt(st.nextToken()),
				     Integer.parseInt(st.nextToken()));
	    
	    //parse colormap if sketch
	    int colormap = SketchInfo.COLORMAP_SEG_TYPE;
	    if(type.equals("sketch")) {
		inputLine = readLine();
		st = new StringTokenizer(inputLine,": ");
		st.nextToken();
		colormap = Integer.parseInt(st.nextToken());
	    }

	    // create node
	    System.out.println(type + 
			       " id:"+id+" parentId:"+parentId+" name:"+name +
			       " type:" + subtype);
	    String colormsg = "  color: " + color +
		(type.equals("sketch") ? (" colormap:" + colormap) : "");
	    System.out.println(colormsg);
	    SketchOrShapeInfo oinfo;
	    if(type.equals("sketch")) {
		oinfo =  new SketchInfo(id, parentId, name, color, colormap, 
					subtype, imageWidth, imageHeight);
	    }
	    else if (type.equals("shape")) {
		oinfo = parseShape(id, parentId, name, color, subtype, imageWidth, imageHeight);
		ShapeInfo sinfo = (ShapeInfo)oinfo;
		if (oldSelected.containsKey(new Integer(id))) {
		    sinfo.setUseDefaultSelection(false);
		    sinfo.setVisible(((Boolean)oldSelected.get(new Integer(id))).booleanValue());
		}
		else {
		    sinfo.setVisible(defaultSelected);
		}
		if (oldInverted.containsKey(new Integer(id))) {
		    sinfo.setUseDefaultInversion(false);
		    sinfo.setInverted(((Boolean)oldInverted.get(new Integer(id))).booleanValue());
		}
		else {
		    sinfo.setInverted(defaultInversion);
		}
	    }
	    else {
		System.out.println("Invalid type!");
		color = new Color(0);
		oinfo = new SketchOrShapeInfo(id, parentId, name, color);
	    }
	    
	    if (space != Space.cam) sketchPanel.scaleToSketchOrShape(oinfo);
	    
	    DefaultMutableTreeNode newnode = new DefaultMutableTreeNode(oinfo);
	    root.add(newnode);
	    
	    sketchTree.updateUI();
	}
	System.out.println(inputLine);

	// pair children with parents
	DefaultMutableTreeNode curNode;
	Vector allnodes = new Vector();
	Enumeration nodeEn = root.preorderEnumeration();
	while(nodeEn.hasMoreElements()){
	    allnodes.add(nodeEn.nextElement());
	}
	for (int i=0; i<allnodes.size(); i++) {
	    curNode = (DefaultMutableTreeNode)(allnodes.elementAt(i));
	    DefaultMutableTreeNode potentialParent = root;
	    while((potentialParent = potentialParent.getNextNode()) != null) {
		if (curNode != root) {
		    if(((SketchOrShapeInfo)(curNode.getUserObject())).parentId == 
		       ((SketchOrShapeInfo)(potentialParent.getUserObject())).id && 
		       !potentialParent.isNodeAncestor(curNode)) {
			potentialParent.add(curNode);
			break;
		    }
		}
	    }
	    
	}
	sortTree(root);
	
	for (int i = 0; i < sketchTree.getRowCount(); i++) {
	    sketchTree.expandRow(i);
	}
	sketchTree.clearSelection();
	sketchTree.updateUI();
	valueChanged(lastEvent);
    }
    
    public SketchOrShapeInfo parseShape(int id, int parentId, String name, 
					Color color, int shapetype, 
					int width, int height) 
	throws IOException
    {
	String inputLine;
	StringTokenizer st;
    
	inputLine = readLine();
	st = new StringTokenizer(inputLine,": ");
	st.nextToken();
	float cx = Float.parseFloat(st.nextToken());
	float cy = Float.parseFloat(st.nextToken());
	float cz = Float.parseFloat(st.nextToken());
    
	if(shapetype == 1) { // lineshape
	    inputLine = readLine();
	    st = new StringTokenizer(inputLine,": ");
	    st.nextToken();
	    float e1x = Float.parseFloat(st.nextToken());
	    float e1y = Float.parseFloat(st.nextToken());
	    float e1z = Float.parseFloat(st.nextToken());
	    
	    inputLine = readLine();
	    st = new StringTokenizer(inputLine,": ");
	    st.nextToken();
	    float e2x = Float.parseFloat(st.nextToken());
	    float e2y = Float.parseFloat(st.nextToken());
	    float e2z = Float.parseFloat(st.nextToken());
	    
	    inputLine = readLine();
	    st = new StringTokenizer(inputLine,": ");
	    st.nextToken();
	    float r = Float.parseFloat(st.nextToken());
	    
	    inputLine = readLine();
	    st = new StringTokenizer(inputLine,": ");
	    st.nextToken();
	    float theta = Float.parseFloat(st.nextToken());

	    inputLine = readLine();
	    st = new StringTokenizer(inputLine,": ");
	    st.nextToken();
	    boolean end1_valid = Integer.parseInt(st.nextToken()) == 1;
	    boolean end1_active = Integer.parseInt(st.nextToken()) == 1;
	    inputLine = readLine();
	    st = new StringTokenizer(inputLine,": ");
	    st.nextToken();
	    boolean end2_valid = Integer.parseInt(st.nextToken()) == 1;
	    boolean end2_active = Integer.parseInt(st.nextToken()) == 1;
	
	    System.out.println("  cxyz: " +cx+" "+cy+" "+cz);
	    System.out.println("  e1xyz/va: "+e1x+" "+e1y +" "+e1z+" / "+end1_valid+" "+end1_active);
	    System.out.println("  e2xyz/va: "+e2x+" "+e2y +" "+e2z+" / "+end2_valid+" "+end2_active);
	    System.out.println("  r: " + r + "  theta: "+theta);
	    return new LineShapeInfo(id, parentId, name, color,
				     cx,cy,cz, e1x, e1y, e1z, e2x, e2y, e2z, 
				     r, theta, end1_valid, end1_active, end2_valid, end2_active);
	
	} else if(shapetype == 2) { // ellipseshape
	    inputLine = readLine();
	    st = new StringTokenizer(inputLine,": ");
	    st.nextToken();
	    float semimajor = Float.parseFloat(st.nextToken());
	    float semiminor = Float.parseFloat(st.nextToken());
	    System.out.println(" axes:"+semimajor+" "+semiminor);
	
	    inputLine = readLine();
	    st = new StringTokenizer(inputLine,": ");
	    st.nextToken();
	    float orientation = Float.parseFloat(st.nextToken());
	    System.out.println(" orientation:"+orientation);
	
	    return new EllipseShapeInfo(id,parentId,name,color,
					cx,cy,cz, semimajor, semiminor, orientation);
	
	} else if (shapetype == 3) { // pointshape
	    return new PointShapeInfo(id,parentId, name, color, cx, cy, cz);

	} else if (shapetype == 4) { // agentshape
	    inputLine = readLine();
	    st = new StringTokenizer(inputLine,": ");
	    st.nextToken();
	    float orientation = Float.parseFloat(st.nextToken());
	    System.out.println(" orientation:"+orientation);
	
	    return new AgentShapeInfo(id,parentId, name, color,
				      cx,cy,cz, orientation);
	} else if (shapetype == 5) { // sphereshape
	    inputLine = readLine();
	    st = new StringTokenizer(inputLine,": ");
	    st.nextToken();
	    float radius = Float.parseFloat(st.nextToken());
	    System.out.println(" radius:"+radius);
	
	    return new SphereShapeInfo(id,parentId,name,color,cx,cy,cz,radius);

	} else if (shapetype == 6) { // polygonshape
	    inputLine = readLine();
	    st = new StringTokenizer(inputLine,": ");
	    st.nextToken();
	    int num_vertices = Integer.parseInt(st.nextToken());
	    System.out.println("num_vtx: " + num_vertices);
	    float[][] vertices = new float[num_vertices][3];
	
	    for (int i = 0; i < num_vertices; i++) {
		inputLine = readLine();
		st = new StringTokenizer(inputLine,": ");
		st.nextToken();
		vertices[i][0] = Float.parseFloat(st.nextToken());
		vertices[i][1] = Float.parseFloat(st.nextToken());
		vertices[i][2] = Float.parseFloat(st.nextToken());
		System.out.println("vertex "+i+": "+vertices[i][0]+" "+vertices[i][1] +" "+vertices[i][2]);
	    }
	    inputLine = readLine();
	    st = new StringTokenizer(inputLine,": ");
	    st.nextToken();
	    //	System.out.println("end1_ln valid: " + st.nextToken());
	    boolean end1_valid = Integer.parseInt(st.nextToken()) == 1;
	    inputLine = readLine();
	    st = new StringTokenizer(inputLine,": ");
	    st.nextToken();
	    boolean end2_valid = Integer.parseInt(st.nextToken()) == 1;
	    System.out.println("End1_valid: " + end1_valid);
	    System.out.println("End2_valid: " + end2_valid);
	    return new PolygonShapeInfo(id, parentId, name, color, cx, cy, cz,
					num_vertices, vertices, end1_valid, end2_valid);
	
	} else if(shapetype == 7) { // blobshape
	    return parseBlob(id,parentId,name,color,cx,cy,cz);
	} else if(shapetype == 8) { // brickshape
	    inputLine = readLine();
	    st = new StringTokenizer(inputLine, ": ");
	    st.nextToken();
	    float GFLx = Float.parseFloat(st.nextToken());
	    float GFLy = Float.parseFloat(st.nextToken());
	    System.out.println("GFL: " + GFLx + " " + GFLy);
	    inputLine = readLine();
	    st = new StringTokenizer(inputLine, ": ");
	    st.nextToken();
	    float GFRx = Float.parseFloat(st.nextToken());
	    float GFRy = Float.parseFloat(st.nextToken());
	    System.out.println("GFR: " + GFRx + " " + GFRy);
	    inputLine = readLine();
	    st = new StringTokenizer(inputLine, ": ");
	    st.nextToken();
	    float GBLx = Float.parseFloat(st.nextToken());
	    float GBLy = Float.parseFloat(st.nextToken());
	    System.out.println("GBL: " + GBLx + " " + GBLy);
	    inputLine = readLine();
	    st = new StringTokenizer(inputLine, ": ");
	    st.nextToken();
	    float GBRx = Float.parseFloat(st.nextToken());
	    float GBRy = Float.parseFloat(st.nextToken());
	    System.out.println("GBR: " + GBRx + " " + GBRy);
	    inputLine = readLine();
	    st = new StringTokenizer(inputLine, ": ");
	    st.nextToken();
	    float TFLx = Float.parseFloat(st.nextToken());
	    float TFLy = Float.parseFloat(st.nextToken());
	    System.out.println("TFL: " + TFLx + " " + TFLy);
	    inputLine = readLine();
	    st = new StringTokenizer(inputLine, ": ");
	    st.nextToken();
	    float TFRx = Float.parseFloat(st.nextToken());
	    float TFRy = Float.parseFloat(st.nextToken());
	    System.out.println("TFR: " + TFRx + " " + TFRy);
	    inputLine = readLine();
	    st = new StringTokenizer(inputLine, ": ");
	    st.nextToken();
	    float TBLx = Float.parseFloat(st.nextToken());
	    float TBLy = Float.parseFloat(st.nextToken());
	    System.out.println("TBL: " + TBLx + " " + TBLy);
	    inputLine = readLine();
	    st = new StringTokenizer(inputLine, ": ");
	    st.nextToken();
	    float TBRx = Float.parseFloat(st.nextToken());
	    float TBRy = Float.parseFloat(st.nextToken());
	    System.out.println("TBR: " + TBRx + " " + TBRy);
       
	    return new BrickShapeInfo(id, parentId, name, color, cx, cy, cz,
				      GFLx, GFLy, GFRx, GFRy, GBLx, GBLy, GBRx, GBRy,
				      TFLx, TFLy, TFRx, TFRy, TBLx, TBLy, TBRx, TBRy);
	
	} else if(shapetype == 9) { // pyramidshape
	    inputLine = readLine();
	    st = new StringTokenizer(inputLine, ": ");
	    st.nextToken();
	    float FLx = Float.parseFloat(st.nextToken());
	    float FLy = Float.parseFloat(st.nextToken());
	    System.out.println("FL: " + FLx + " " + FLy);
	    inputLine = readLine();
	    st = new StringTokenizer(inputLine, ": ");
	    st.nextToken();
	    float BLx = Float.parseFloat(st.nextToken());
	    float BLy = Float.parseFloat(st.nextToken());
	    System.out.println("BL: " + BLx + " " + BLy);
	    inputLine = readLine();
	    st = new StringTokenizer(inputLine, ": ");
	    st.nextToken();
	    float FRx = Float.parseFloat(st.nextToken());
	    float FRy = Float.parseFloat(st.nextToken());
	    System.out.println("FR: " + FRx + " " + FRy);
	    inputLine = readLine();
	    st = new StringTokenizer(inputLine, ": ");
	    st.nextToken();
	    float BRx = Float.parseFloat(st.nextToken());
	    float BRy = Float.parseFloat(st.nextToken());
	    System.out.println("BR: " + BRx + " " + BRy);
	    inputLine = readLine();
	    st = new StringTokenizer(inputLine, ": ");
	    st.nextToken();
	    float Topx = Float.parseFloat(st.nextToken());
	    float Topy = Float.parseFloat(st.nextToken());
	    System.out.println("Top: " + Topx + " " + Topy);

	    return new PyramidShapeInfo(id, parentId, name, color, cx, cy, cz,
				      FLx, FLy, FRx, FRy, BLx, BLy, BRx, BRy,
				      Topx, Topy);
	} else if(shapetype == 10) { // localzationparticleshape
	    inputLine = readLine();
	    st = new StringTokenizer(inputLine, ": ");
	    st.nextToken();
	    float orient = Float.parseFloat(st.nextToken());
	    float weight = Float.parseFloat(st.nextToken());
	    System.out.println("  orient: " + orient + "   weight: " + weight);
	    return new LocalizationParticleShapeInfo(id, parentId, name, color, cx, cy, cz,
						orient, weight);
	} else {
	    System.out.println("Invalid shape!");
	    return new ShapeInfo(id, parentId, name, color, cx, cy, cz);
	}
    }

    public BlobShapeInfo parseBlob(int id, int parentId, String name, Color color,
				   float cx, float cy, float cz)
	throws IOException
    {
	String inputLine;
	StringTokenizer st;

	inputLine = readLine();
	st = new StringTokenizer(inputLine,": ");
	st.nextToken();
	float area = Float.parseFloat(st.nextToken());
	System.out.println(" area: "+area);

	inputLine = readLine();
	st = new StringTokenizer(inputLine,": ");
	st.nextToken();
	int orient = Integer.parseInt(st.nextToken());
	System.out.println(" orient: "+orient);
	if ( space == Space.cam )
	    orient = 0; // always display cam-space blobs as lieing in the camera plane

	inputLine = readLine();
	st = new StringTokenizer(inputLine,": ");
	st.nextToken();
	float topLeft_x = Float.parseFloat(st.nextToken());
	float topLeft_y = Float.parseFloat(st.nextToken());
	float topLeft_z = Float.parseFloat(st.nextToken());
	System.out.println(" topLeft: " + topLeft_x + " " + 
			   topLeft_y + " " + topLeft_z);

	inputLine = readLine();
	st = new StringTokenizer(inputLine,": ");
	st.nextToken();
	float topRight_x = Float.parseFloat(st.nextToken());
	float topRight_y = Float.parseFloat(st.nextToken());
	float topRight_z = Float.parseFloat(st.nextToken());
	System.out.println(" topRight: " + topRight_x + " " + 
			   topRight_y + " " + topRight_z);

	inputLine = readLine();
	st = new StringTokenizer(inputLine,": ");
	st.nextToken();
	float bottomLeft_x = Float.parseFloat(st.nextToken());
	float bottomLeft_y = Float.parseFloat(st.nextToken());
	float bottomLeft_z = Float.parseFloat(st.nextToken());
	System.out.println(" bottomLeft: " + bottomLeft_x + " " + 
			   bottomLeft_y + " " + bottomLeft_z);

	inputLine = readLine();
	st = new StringTokenizer(inputLine,": ");
	st.nextToken();
	float bottomRight_x = Float.parseFloat(st.nextToken());
	float bottomRight_y = Float.parseFloat(st.nextToken());
	float bottomRight_z = Float.parseFloat(st.nextToken());
	System.out.println(" bottomRight: " + bottomRight_x + " " + 
			   bottomRight_y + " " + bottomRight_z);

	return new BlobShapeInfo(id,parentId,name,color,cx,cy,cz,area,orient,
				 topLeft_x, topLeft_y, topLeft_z,
				 topRight_x, topRight_y, topRight_z,
				 bottomLeft_x, bottomLeft_y, bottomLeft_z,
				 bottomRight_x, bottomRight_y, bottomRight_z);
    }

    public SketchGUI(String _host, int _listingPort, int _sketchPort, 
		     Space _space) {
	super();
	host = _host;
	listingPort = _listingPort;
	sketchPort = _sketchPort;
	space = _space;

	Tmat = new AffineTransform();
	TmatInv = new AffineTransform();
    
	if(space == Space.cam)
	    root = new DefaultMutableTreeNode("camspace");
	else if (space == Space.local)
	    root = new DefaultMutableTreeNode("localspace");
	else if (space == Space.world)
	    root = new DefaultMutableTreeNode("worldspace");


	listener = new TCPVisionListener(host, sketchPort);

    
	// network setup
	reconnect();

	// Get the image size immediately after connecting
	if (netout != null) {
	    try{
		String inputLine;
		netout.println("size");
		System.out.println(inputLine = readLine());
		while((inputLine=readLine()).compareTo("size end") != 0) {
		    System.out.println(inputLine);
		    StringTokenizer sizetoken = new StringTokenizer(inputLine);
		    String token = sizetoken.nextToken();
		    if (token.equals("width")){
			imageWidth = Integer.parseInt(sizetoken.nextToken());

		    }
		    else if (token.equals("height")){
			imageHeight = Integer.parseInt(sizetoken.nextToken());
		    }
		}
		System.out.println(inputLine);
		
	    }
	    catch(IOException ioe) {
		System.err.println("Transfer error");
	    }
	    panelBoundsSet = true;
	}
	else {
	    imageWidth = defaultWidth;
	    imageHeight = defaultHeight;
	}
    
	sketchPanel = new SketchPanel(this, listener, space, prefs, imageWidth, imageHeight);

	img = new BufferedImage(imageWidth, imageHeight, BufferedImage.TYPE_INT_RGB);
	Graphics2D g = img.createGraphics();
	g.clearRect(0,0,imageWidth, imageHeight);
	sketchStack = new Vector();

	init();
    
    }
	
    public void init() {

	int strutsize=10;
	int sepsize=5;
	getContentPane().setLayout(new BorderLayout());
	getContentPane().add(Box.createVerticalStrut(strutsize),BorderLayout.NORTH);
	getContentPane().add(Box.createHorizontalStrut(strutsize),BorderLayout.WEST);
	getContentPane().add(Box.createHorizontalStrut(strutsize),BorderLayout.EAST);
	setTitle(space.name()+" GUI: "+host);

	sketchPanel.setMinimumSize(new Dimension(imageWidth/2, imageHeight/2));
	sketchPanel.setPreferredSize(new Dimension(imageWidth*2, imageHeight*2));
	sketchPanel.setLockAspectRatio(true);

	curSketchPanel = sketchPanel;
	panelTitle = space.name() + " view"; // don't append host here because of clones
	sketchPanel.makeSketchFrame(sketchPanel, panelTitle+": "+host);
	{

	    Box tmp1 = Box.createHorizontalBox();
	    tmp1.add(Box.createHorizontalStrut(strutsize));
	    {		
		Box tmp2 = Box.createVerticalBox();
		tmp2.add(Box.createVerticalStrut(strutsize));

		{
		    Box buttonBox = Box.createVerticalBox();
		    {
			Box tmp3 = Box.createHorizontalBox();
			tmp3.add(status=new JLabel(state));
			//tmp3.add(Box.createHorizontalGlue());

			//tmp3.add(Box.createHorizontalStrut(strutsize));
		
			rescaleBut = new JButton("Rescale the Image");
			rescaleBut.setAlignmentX(0.5f);
			rescaleBut.addActionListener(this);
			rescaleBut.setActionCommand("rescale");
			rescaleBut.setEnabled(true);
			rescaleBut.setToolTipText("Rescales the displayed sketch;");
			tmp3.add(rescaleBut);
			tmp3.add(Box.createHorizontalStrut(strutsize));

			refreshListBut = new JButton("Refresh Listing");
			refreshListBut.setAlignmentX(0.5f);
			refreshListBut.addActionListener(this);
			refreshListBut.setActionCommand("refreshlist");
			refreshListBut.setEnabled(true);
			refreshListBut.setToolTipText("Refreshes the sketch listing;");
			tmp3.add(refreshListBut);
			tmp3.add(Box.createGlue());

			buttonBox.add(tmp3);
		    }

		    buttonBox.add(Box.createVerticalStrut(strutsize));
		    {
			Box tmp4 = Box.createHorizontalBox();
			//tmp4.add(Box.createHorizontalGlue());
			//tmp4.add(Box.createHorizontalStrut(strutsize));
			
			checkAllBut = new JCheckBox("Select All Shapes", defaultSelected);
			checkAllBut.setAlignmentX(0.5f);
			checkAllBut.addActionListener(this);
			checkAllBut.setActionCommand("checkall");
			checkAllBut.setEnabled(true);
			checkAllBut.setToolTipText("Selects all shapes for display.");
			tmp4.add(checkAllBut);
			tmp4.add(Box.createHorizontalStrut(strutsize));

			invertAllBut = new JCheckBox("Invert All Shapes", defaultInversion);
			invertAllBut.setAlignmentX(0.5f);
			invertAllBut.addActionListener(this);
			invertAllBut.setActionCommand("invertall");
			invertAllBut.setEnabled(true);
		        invertAllBut.setToolTipText("Inverts all shapes.");
			tmp4.add(invertAllBut);
			tmp4.add(Box.createGlue());

			buttonBox.add(tmp4);
		    }
		    
		    tmp2.add(buttonBox, BorderLayout.CENTER);
		}

		tmp2.add(Box.createVerticalStrut(strutsize));
		// Sketch Tree:
		sketchTree = new JTree(new DefaultTreeModel(initSketchTree(host, listingPort)));
		tmp2.add(new JScrollPane(sketchTree));
		// set up sketch node selection
		sketchTree.getSelectionModel().setSelectionMode
		    (TreeSelectionModel.DISCONTIGUOUS_TREE_SELECTION);
		//Listen for when the selection changes.
		sketchTree.addTreeSelectionListener(this);
		sketchTree.addMouseListener(this);
		sketchTree.setCellRenderer(new SketchTreeRenderer());

		tmp2.add(Box.createVerticalStrut(strutsize));
		{
		    Box tmp4 = Box.createHorizontalBox();
		    tmp4.add(status=new JLabel(state));
		    tmp4.add(Box.createHorizontalGlue());
		    tmp2.add(tmp4);
		}
		tmp2.add(Box.createVerticalStrut(strutsize));
		tmp1.add(tmp2);
	    }	    tmp1.add(Box.createHorizontalStrut(strutsize));
	    getContentPane().add(tmp1,BorderLayout.CENTER);
	}
	pack();
	
	String name="SketchGUI"+".location";
	setLocation(prefs.getInt(name+".x",50),prefs.getInt(name+".y",50));
	addWindowListener(new CloseSketchGUIAdapter(this));

	//sketchPanel.getListener().addListener(this);


	popMenu = new PopupMenu();
	MenuItem invBox = new MenuItem("Invert");
	invBox.addActionListener(new PopupInvertListener());
	popMenu.add(invBox);
	MenuItem newWindow = new MenuItem("Clone current window");
	newWindow.addActionListener(new PopupNewWindowListener(this));
	popMenu.add(newWindow);
	this.add(popMenu);

	refreshListBut.doClick();
    }

    class CloseSketchGUIAdapter extends WindowAdapter {
	SketchGUI gui;
	CloseSketchGUIAdapter(SketchGUI _gui) { gui = _gui; }
	public void windowClosing(WindowEvent e) {
	    gui.close();
	}
    }

    class PopupInvertListener implements ActionListener {
 	public void actionPerformed(ActionEvent e)
	{
	    currentMenuObject.invert();
	    if (currentMenuObject instanceof ShapeInfo)
		{
		    ((ShapeInfo)currentMenuObject).setUseDefaultInversion(false);
		}
	    valueChanged(lastEvent);
	    sketchTree.treeDidChange();
	}
    }

    class PopupNewWindowListener implements ActionListener {
	SketchGUI gui;

        public PopupNewWindowListener(SketchGUI _gui) { gui = _gui; }

	public void actionPerformed(ActionEvent e)
	{
	    SketchPanel sketchPanel=new SketchPanel(gui, listener, Space.cam, prefs, imageWidth, imageHeight);
	    sketchPanel.setMinimumSize(new Dimension(imageWidth/2, imageHeight/2));
	    sketchPanel.setPreferredSize(new Dimension(imageWidth*2, imageHeight*2));
	    sketchPanel.setLockAspectRatio(true);
	    
	    sketchPanel.makeSketchFrame(sketchPanel, "Dummy");
	    
	    sketchPanel.imageUpdated(getSketchImage(),sketchTree.getSelectionPaths());   
	}
    }

    public void close() {
	try {
	    if(listingSocket!=null && !listingSocket.isClosed())
		listingSocket.close();
	} catch (IOException ioe) {
	    System.err.println("close failed:");
	    ioe.printStackTrace();
	}
	prefs.putInt("SketchGUI.location.x",getLocation().x);
	prefs.putInt("SketchGUI.location.y",getLocation().y);
	sketchPanel.getListener().kill();
	Component p=sketchPanel;
	while(p.getParent()!=null)
	    p=p.getParent();
	if(p instanceof Window) {
	    Window w=(Window)p;
	    prefs.putInt("SketchPanel.location.x",w.getLocation().x);
	    prefs.putInt("SketchPanel.location.y",w.getLocation().y);
	    w.dispose();
	} else
	    System.out.println("That's weird - root container isn't window");
	dispose();
    }

    public TreeNode initSketchTree(String host, int port) {

	// Just for demonstration:
	DefaultMutableTreeNode cam = new DefaultMutableTreeNode("hit refresh");
	root.insert(cam,0);
	return root;
    }

    public void sortTree(DefaultMutableTreeNode curNode) {
	// set up comparator
	Comparator comp =
	    new Comparator() {
		public int compare(Object o1, Object o2) {
		    return compare((DefaultMutableTreeNode) o1, (DefaultMutableTreeNode) o2);  
		}
		public int compare(DefaultMutableTreeNode n1, DefaultMutableTreeNode n2) {
		    Integer i1 = new Integer(((SketchOrShapeInfo)n1.getUserObject()).id);
		    Integer i2 = new Integer(((SketchOrShapeInfo)n2.getUserObject()).id);
		    return i1.compareTo(i2);
		}
	    };

	// sort the roots children
	Object[] objs = new Object[curNode.getChildCount()];
	Enumeration children = curNode.children();
	for (int i=0;children.hasMoreElements();i++) {
	    DefaultMutableTreeNode child = (DefaultMutableTreeNode) children.nextElement();
	    objs[i] = child;
	}

	Arrays.sort(objs, comp);
	curNode.removeAllChildren();

	// insert newly ordered children
	for (int i=0;i<objs.length;i++) {
	    DefaultMutableTreeNode orderedNode = (DefaultMutableTreeNode) objs[i];
	    curNode.add(orderedNode);
	    if (!orderedNode.isLeaf()) {
		sortTree(orderedNode);
	    }
	}
    }

    // gets called when a Sketch selection is clicked
    public void valueChanged(TreeSelectionEvent e) {
	imageNeedsUpdate = true;
	lastEvent = e;
	
	// Go through all objects and set visible to false
	// If a sketch was selected, only de-select shapes, and vice-versa
	DefaultMutableTreeNode n = root;
	boolean curIsSketch = curObject instanceof SketchInfo;
	while((n = n.getNextNode())!= null) {
	    if (!(n.getUserObject() instanceof SketchOrShapeInfo)) {
		continue;
	    }
	    if ((n.getUserObject() instanceof SketchInfo) == curIsSketch ||
		focusJustChanged) {
		((SketchOrShapeInfo)n.getUserObject()).setVisible(false);
		
		// Override the default behavior of the tree
		// If the default is to have shapes selected, then re-add any shapes that follow the default
		if (defaultSelected && n.getUserObject() instanceof ShapeInfo && 
		    ((ShapeInfo)n.getUserObject()).getUseDefaultSelection()) {
		    ShapeInfo sinfo = (ShapeInfo)n.getUserObject();
		    sinfo.setVisible(true);
		    sketchTree.addSelectionPath(new TreePath(n.getPath()));
		}
	    }
	    // Keep shapes if a sketch was selected, keep sketches if a shape was selected
	    else if (((SketchOrShapeInfo)n.getUserObject()).getVisible()) {
		sketchTree.addSelectionPath(new TreePath(n.getPath()));
	    }
	}
	
	focusJustChanged = false;
	TreePath[] paths = sketchTree.getSelectionPaths();

	if (paths == null) {
	    curSketchPanel.imageUpdated(null, null);
	    return;
	}


	sketchStack.clear();

	for (int path_i = 0; path_i < paths.length; path_i++) {
	    DefaultMutableTreeNode node 
		=(DefaultMutableTreeNode)(paths[path_i].getLastPathComponent());
	    
	    if (node == root) continue;	
	    if (node == null) {System.err.println("node is null???"); return; }
	    
	    if(!(node.getUserObject() instanceof SketchOrShapeInfo)) {
		System.out.println("valueChanged:: placeholder text can't be displayed");
		continue;
	    }
	    
	    
	    SketchOrShapeInfo vinfo = (SketchOrShapeInfo)(node.getUserObject());
	    if (!node.isRoot()) {
		if (vinfo instanceof SketchInfo && ! ((SketchInfo)vinfo).isImageLoaded()) {
		    ((TCPVisionListener)listener).setReadingImage();
		    netout.println("get "+vinfo.id);
		    try {
			String inputLine;
			while((inputLine=readLine()).compareTo("get end") != 0) {
			    System.out.println(inputLine);
			}
		    } catch (IOException ioe) {
			System.err.println("Transfer error");
			reconnect();
			valueChanged(null);
		    }
		    while(((TCPVisionListener)listener).isReadingImage())
			// thread.yield() in java?
			;
		    ((SketchInfo)vinfo).copyImage(((TCPVisionListener)listener).getImage());
		    System.out.println("done with id:"+vinfo.id);
		}
		else {
		    curSketchPanel.scaleToSketchOrShape(vinfo);
		}
		vinfo.setVisible(true);
		if (!sketchStack.contains(vinfo)) { 
		    sketchStack.add(vinfo); 
		}
	    }
	}
	
	curSketchPanel.imageUpdated(getSketchImage(), paths);
    }

    protected String readLine() throws java.io.IOException {
	if(netin==null)
	    reconnect();
	if(netin==null)
	    throw new java.io.IOException("no connection");
	String ans=netin.readLine();
	if(ans==null)
	    throw new java.io.IOException("lost connection");
	return ans;
    }
	
    public void reconnect() {
	System.out.print(host+":"+listingPort+" reconnecting...");
	netout = null;
	try {
	    if(listingSocket!=null && !listingSocket.isClosed())
		listingSocket.close();
	    netin=null;
	    netout=null;
	    listingSocket = new Socket(host,listingPort);
	    netout = new PrintWriter(listingSocket.getOutputStream(), true);
	    netin = new BufferedReader(new InputStreamReader(listingSocket.getInputStream()));
        } catch (UnknownHostException e) {
	    System.err.println("Don't know about host:"+host);
	    System.exit(1);
	} catch (IOException ioe) {
	    System.err.println("reconnection failed:");
	    //ioe.printStackTrace();
	    return;
	}
	System.out.println("done");
	if (refreshListBut != null) {
	    refreshListBut.doClick(); //auto refresh on reconnect
	}
    }
    
    public void visionUpdated(VisionListener listener) {
	//renderSelectedInTree(l.getImage().createGraphics());
	/*listener.removeListener(this);
	listener.fireVisionUpdate();
	listener.addListener(this);*/
    }

    public void sensorsUpdated(VisionListener l) {}
	
    // renders all the currently selected elements in the tree
    public void renderSelectedInTree(Graphics2D g) {
	if(g==null) {
	    g = img.createGraphics();
	    // g.setBackground(Color.GRAY);
	    g.clearRect(0,0,img.getWidth(),img.getHeight());
	}
	TreePath[] paths = sketchTree.getSelectionPaths();
	if (paths == null) {
	    sketchPanel.repaint();
	    return;
	}
	for (int path_i = 0; path_i < paths.length; path_i++) {
	    DefaultMutableTreeNode node 
		=(DefaultMutableTreeNode)(paths[path_i].getLastPathComponent());

	    if (node == root) continue;
	    if (node == null) return;
			
	    if(!(node.getUserObject() instanceof SketchOrShapeInfo)) {
		System.out.println("renderSelectedInTree:: placeholder text can't be displayed");
		continue;
	    }
			
	    SketchOrShapeInfo vinfo = (SketchOrShapeInfo)(node.getUserObject());
	    g.setColor(vinfo.getColor());

	    //g.setTransform(sketchPanel.resultAtrans);
	    //vinfo.renderTo(g);
	}
	sketchPanel.repaint();
	//repaint(); // repaints this frame
	//paintAll(); // repaints everything in this frame
    }

    public void mousePressed(MouseEvent e)
    {
	TreePath selectedPath = sketchTree.getPathForLocation(e.getX(),e.getY());
	//System.out.println("Got mouse event for "+selectedPath);
	if (selectedPath != null)
	    {
		Object obj = ((DefaultMutableTreeNode)(selectedPath.getLastPathComponent())).getUserObject();
		if (! (obj instanceof SketchOrShapeInfo))
		    {
			return;
		    }
		curObject = (SketchOrShapeInfo)obj;
		if (e.getButton() == MouseEvent.BUTTON3)
		    {
			currentMenuObject = curObject;
			popMenu.show(sketchTree,e.getX(), e.getY());
		    }
		else if (e.getButton() == MouseEvent.BUTTON1)
		    {
			if (curObject instanceof ShapeInfo)
			    {
				((ShapeInfo)curObject).setUseDefaultSelection(false);
			    }
		    }
	    }
    }

    public void mouseClicked(MouseEvent e){}
    public void mouseReleased(MouseEvent e){}
    public void mouseEntered(MouseEvent e){}
    public void mouseExited(MouseEvent e){}

    public BufferedImage getSketchImage()
    {
	if (imageNeedsUpdate) createSketchImage();
	return img;
    }

    private void createSketchImage()
    {
	// Cull not visible objects from vector
	// render all the sketches in the vector

	img = new BufferedImage(imageWidth, imageHeight, BufferedImage.TYPE_INT_RGB);
	Graphics2D g = img.createGraphics();
	g.setBackground(backgroundColor);
	g.clearRect(0,0,imageWidth, imageHeight);

	int pixcount = img.getWidth()*img.getHeight();
	int rarr[] = new int[pixcount];
	int garr[] = new int[pixcount];
	int barr[] = new int[pixcount];
	int counts[] = new int[pixcount];
	for (int i=0; i < pixcount; i++) {
	    rarr[i] = 0;
	    garr[i] = 0;
	    barr[i] = 0;
	    counts[i] = 0;
	}

	// Draw SketchINTS
	for (int i=0; i<sketchStack.size(); i++) {
	    SketchOrShapeInfo vinfo = (SketchOrShapeInfo)sketchStack.elementAt(i);
	    if (vinfo.getVisible()) {
		if (vinfo instanceof SketchInfo && ((SketchInfo)vinfo).getSketchType() != SketchInfo.SKETCH_BOOL_TYPE) {
		    ((SketchInfo)vinfo).renderToArrays(rarr,garr,barr,counts);
		}
	    } else {
		sketchStack.removeElementAt(i--);
	    }
	}
	
	for (int y=0; y<img.getHeight(); y++) {
	    for (int x=0; x<img.getWidth(); x++) {
		int pos = y*img.getWidth()+x;
		if (counts[pos] > 0) {
		    g.setColor(new Color(rarr[pos]/counts[pos],
					 garr[pos]/counts[pos],
					 barr[pos]/counts[pos]));
		    g.drawLine(x,y,x,y);
		}
	    }
	}
	
	// Draw SketchBOOLs
	for (int i=0; i < img.getWidth()*img.getHeight(); i++) {
	    rarr[i] = 0;
	    garr[i] = 0;
	    barr[i] = 0;
	    counts[i] = 0;
	}

	for (int i=0; i<sketchStack.size(); i++) {
	    SketchOrShapeInfo vinfo = (SketchOrShapeInfo)sketchStack.elementAt(i);
	    if (vinfo.getVisible()) {
		if (vinfo instanceof SketchInfo && ((SketchInfo)vinfo).getSketchType() == SketchInfo.SKETCH_BOOL_TYPE) {
		    ((SketchInfo)vinfo).renderToArrays(rarr,garr,barr,counts);
		}
	    }
	    else {
		sketchStack.removeElementAt(i--);
	    }
	}
	
	for (int y=0; y<img.getHeight(); y++) {
	    for (int x=0; x<img.getWidth(); x++) {
		int pos = y*img.getWidth()+x;
		if (counts[pos] > 0) {
		    g.setColor(new Color(rarr[pos]/counts[pos],
					 garr[pos]/counts[pos],
					 barr[pos]/counts[pos]));
		    g.drawLine(x,y,x,y);
		}
	    }
	}
	
	imageNeedsUpdate = false;
    }

    public void setCurrentSketchPanel(SketchPanel s, TreePath[] paths)
    {
	if (curSketchPanel != s) {
	    curSketchPanel.loseFocus();
	    focusJustChanged = true;
	}
	curSketchPanel = s;
	if (sketchTree != null) {
	    sketchTree.setSelectionPaths(paths);
	}
	
    }


    private class SketchTreeRenderer extends DefaultTreeCellRenderer {
	public SketchTreeRenderer(/*Icon icon*/) {
	    //tutorialIcon = icon;
	}
	
	public Component getTreeCellRendererComponent(JTree tree,
						      Object value,
						      boolean sel,
						      boolean expanded,
						      boolean leaf,
						      int row,
						      boolean hasFocus) {
	    super.getTreeCellRendererComponent(tree, value, sel,
					       expanded, leaf, row,
					       hasFocus);
	    try {
		SketchOrShapeInfo vinfo = (SketchOrShapeInfo)(((DefaultMutableTreeNode)value).getUserObject());
		
		setIcon(vinfo.getIcon());
		setToolTipText(vinfo.toString());
	    } catch (ClassCastException e) {}
	    
	    return this;
	}
    }
}
