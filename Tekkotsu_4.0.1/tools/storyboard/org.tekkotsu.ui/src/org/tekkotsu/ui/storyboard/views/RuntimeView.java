package org.tekkotsu.ui.storyboard.views;

import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

import org.eclipse.swt.widgets.Composite;
import org.eclipse.ui.part.*;
import org.eclipse.jface.viewers.*;
import org.eclipse.swt.graphics.Image;
import org.eclipse.jface.action.*;
import org.eclipse.jface.dialogs.MessageDialog;
import org.eclipse.ui.*;
import org.eclipse.swt.widgets.Menu;
import org.eclipse.swt.SWT;
import org.eclipse.core.runtime.IAdaptable;
import org.tekkotsu.ui.editor.resources.Debugger;
import org.tekkotsu.ui.storyboard.model.BlockModel;
import org.tekkotsu.ui.storyboard.model.ImageModel;
import org.tekkotsu.ui.storyboard.model.LogModel;
import org.tekkotsu.ui.storyboard.model.TransitionModel;

public class RuntimeView extends ViewPart implements PropertyChangeListener {	
	private TreeViewer viewer;
	//private DrillDownAdapter drillDownAdapter;
	//private Action action1;
	//private Action action2;
	//private Action doubleClickAction;

	/*
	 * The content provider class is responsible for
	 * providing objects to the view. It can wrap
	 * existing objects in adapters or simply return
	 * objects as-is. These objects may be sensitive
	 * to the current input of the view, or ignore
	 * it and always show the same content 
	 * (like Task List, for example).
	 */
	 
	class TreeObject implements IAdaptable {
		private String name;
		private TreeParent parent;
		
		public TreeObject(String name) {
			this.name = name;
		}
		public String getName() {
			return name;
		}
		public void setParent(TreeParent parent) {
			this.parent = parent;
		}
		public TreeParent getParent() {
			return parent;
		}
		public String toString() {
			return getName();
		}
		public Object getAdapter(Class key) {
			return null;
		}
	}
	
	class TreeParent extends TreeObject {
		private ArrayList children;
		private int start,stop;
		public int getStop() {
			return stop;
		}
		public int getStart() {
			return start;
		}
		public TreeParent(String name, int start,int stop) {
			super(name);
			this.start = start; this.stop = stop;
			children = new ArrayList();
		}
		public TreeParent(String name) {
			this(name,0,0);
		}

		public void addChild(TreeObject child) {
			children.add(child);
			child.setParent(this);
		}
		public void removeChild(TreeObject child) {
			children.remove(child);
			child.setParent(null);
		}
		public TreeObject [] getChildren() {
			return (TreeObject [])children.toArray(new TreeObject[children.size()]);
		}
		public boolean hasChildren() {
			return children.size()>0;
		}
	}

	class ViewContentProvider implements IStructuredContentProvider, 
										   ITreeContentProvider {
		private TreeParent invisibleRoot;

		public void inputChanged(Viewer v, Object oldInput, Object newInput) {
		}
		public void dispose() {
		}
		public Object[] getElements(Object parent) {
			if (parent.equals(getViewSite())) {
				if (invisibleRoot==null) initialize();
				return getChildren(invisibleRoot);
			}
			return getChildren(parent);
		}
		public Object getParent(Object child) {
			if (child instanceof TreeObject) {
				return ((TreeObject)child).getParent();
			}
			return null;
		}
		public Object [] getChildren(Object parent) {
			if (parent instanceof TreeParent) {
				return ((TreeParent)parent).getChildren();
			}
			return new Object[0];
		}
		public boolean hasChildren(Object parent) {
			if (parent instanceof TreeParent)
				return ((TreeParent)parent).hasChildren();
			return false;
		}

		private void initialize() {
			invisibleRoot = new TreeParent("");
		}
		
		/**
		 * Fetch information on selected model
		 */
		public void setContent(List nodes) {
			Iterator iter = nodes.iterator();		    
			Integer curTime = (Integer)iter.next();
			
			TreeParent root = new TreeParent("Current selection :"+(curTime.intValue()/1000.0)+"s");
			while (iter.hasNext()) {
				Object next = iter.next();

				if (next instanceof ImageModel) {
				    ImageModel img = (ImageModel)next;
				    TreeParent par = new TreeParent("Image:"+img.getID(),img.getTime(),img.getTime());
					par.addChild(new TreeObject("type: image"));
					par.addChild(new TreeObject("record at: "+img.getTime()*0.001+"s"));
					root.addChild(par);
				} else
				if (next instanceof LogModel) {
					LogModel log = (LogModel)next;
					TreeParent par = new TreeParent(log.getID(),log.getTime(),log.getTime());
					par.addChild(new TreeObject("type: log"));
					par.addChild(new TreeObject("report at: "+log.getTime()*0.001+"s"));
					par.addChild(new TreeObject("message: "+log.getContent()));
					root.addChild(par);
				} else if (next instanceof TransitionModel) {
					TransitionModel trans = (TransitionModel)next;
					TreeParent par = new TreeParent(trans.getID(),trans.getTime(),trans.getTime());
					par.addChild(new TreeObject("type: transition"));
					par.addChild(new TreeObject("fire at: "+trans.getTime()*0.001+"s"));
					root.addChild(par);
				} else if (next instanceof BlockModel) {
					BlockModel node = (BlockModel)next;
					TreeParent par = null;
					if (node.getEnd()<0)
						par = new TreeParent(node.getID(),node.getStart(),Integer.MAX_VALUE);
					else 
						par = new TreeParent(node.getID(),node.getStart(),node.getEnd());
					par.addChild(new TreeObject("type: state"));
					par.addChild(new TreeObject("activate at: "+node.getStart()*.001+"s"));
					if (node.getEnd()<0) par.addChild(new TreeObject("deactivate at: still active")); else 
						par.addChild(new TreeObject("deactivate at: "+node.getEnd()*.001+"s"));
					root.addChild(par);
				} else {
				    root.addChild(new TreeObject("Unknown Entity"));
				}
				Thread.yield();
			}			
			invisibleRoot = new TreeParent("");
			invisibleRoot.addChild(root);
		}
	}
	
	private ViewContentProvider content;

	class ViewLabelProvider extends LabelProvider {

		public String getText(Object obj) {
			return obj.toString();
		}
		public Image getImage(Object obj) {
			return null;
			/*String imageKey = ISharedImages.IMG_OBJ_ELEMENT;
			if (obj instanceof TreeParent)
			   imageKey = ISharedImages.IMG_OBJ_FOLDER;
			return PlatformUI.getWorkbench().getSharedImages().getImage(imageKey);
			*/
		}
	}
	class NameSorter extends ViewerSorter {
	    public int compare(Viewer viewer, Object e1, Object e2) {
	        int cat1 = category(e1);
	        int cat2 = category(e2);

	        if (cat1 != cat2)
	            return cat1 - cat2;
	        // cat1 == cat2
	        if (e1 instanceof TreeParent) {
	        	TreeParent t1 = (TreeParent)e1;
	        	TreeParent t2 = (TreeParent)e2;
	        	int result = t1.getStop()-t2.getStop();
	        	if (result!=0) return result;
	        	result = t1.getStart()-t2.getStart();
	        	if (result!=0) return result;
	        }
	        return super.compare(viewer,e1,e2);
	    }
	        /*
	        String name1;
	        String name2;

	        if (viewer == null || !(viewer instanceof ContentViewer)) {
	            name1 = e1.toString();
	            name2 = e2.toString();
	        } else {
	            IBaseLabelProvider prov = ((ContentViewer) viewer)
	                    .getLabelProvider();
	            if (prov instanceof ILabelProvider) {
	                ILabelProvider lprov = (ILabelProvider) prov;
	                name1 = lprov.getText(e1);
	                name2 = lprov.getText(e2);
	            } else {
	                name1 = e1.toString();
	                name2 = e2.toString();
	            }
	        }
	        if (name1 == null)
	            name1 = "";//$NON-NLS-1$
	        if (name2 == null)
	            name2 = "";//$NON-NLS-1$
	        return collator.compare(name1, name2);
	    }	*/
	}

	/**
	 * The constructor.
	 */
	public RuntimeView() {
	}

	/**
	 * This is a callback that will allow us
	 * to create the viewer and initialize it.
	 */
	public void createPartControl(Composite parent) {
		viewer = new TreeViewer(parent, SWT.MULTI | SWT.H_SCROLL | SWT.V_SCROLL);
		//drillDownAdapter = new DrillDownAdapter(viewer);
		content = new ViewContentProvider();
		viewer.setContentProvider(content);
		viewer.setLabelProvider(new ViewLabelProvider());
		viewer.setSorter(new NameSorter());
		viewer.setInput(getViewSite());
		//makeActions();
		//hookContextMenu();
		//hookDoubleClickAction();
		//contributeToActionBars();
	}

	private void hookContextMenu() {
		MenuManager menuMgr = new MenuManager("#PopupMenu");
		menuMgr.setRemoveAllWhenShown(true);
		menuMgr.addMenuListener(new IMenuListener() {
			public void menuAboutToShow(IMenuManager manager) {
				RuntimeView.this.fillContextMenu(manager);
			}
		});
		Menu menu = menuMgr.createContextMenu(viewer.getControl());
		viewer.getControl().setMenu(menu);
		getSite().registerContextMenu(menuMgr, viewer);
	}

	private void contributeToActionBars() {
		IActionBars bars = getViewSite().getActionBars();
		fillLocalPullDown(bars.getMenuManager());
		fillLocalToolBar(bars.getToolBarManager());
	}

	private void fillLocalPullDown(IMenuManager manager) {
		/*
		manager.add(action1);
		manager.add(new Separator());
		manager.add(action2);
		*/
	}

	private void fillContextMenu(IMenuManager manager) {
		/*
		manager.add(action1);
		manager.add(action2);
		manager.add(new Separator());
		drillDownAdapter.addNavigationActions(manager);
		*/
		// Other plug-ins can contribute there actions here
		manager.add(new Separator(IWorkbenchActionConstants.MB_ADDITIONS));
	}
	
	private void fillLocalToolBar(IToolBarManager manager) {
		/*
		manager.add(action1);
		manager.add(action2);
		manager.add(new Separator());
		drillDownAdapter.addNavigationActions(manager);
		*/
	}

	private void makeActions() {
		/*
		action1 = new Action() {
			public void run() {
				showMessage("Action 1 executed");
			}
		};
		action1.setText("Action 1");
		action1.setToolTipText("Action 1 tooltip");
		action1.setImageDescriptor(PlatformUI.getWorkbench().getSharedImages().
			getImageDescriptor(ISharedImages.IMG_OBJS_INFO_TSK));
		
		action2 = new Action() {
			public void run() {
				showMessage("Action 2 executed");
			}
		};
		action2.setText("Action 2");
		action2.setToolTipText("Action 2 tooltip");
		action2.setImageDescriptor(PlatformUI.getWorkbench().getSharedImages().
				getImageDescriptor(ISharedImages.IMG_OBJS_INFO_TSK));
		doubleClickAction = new Action() {
			public void run() {
				ISelection selection = viewer.getSelection();
				Object obj = ((IStructuredSelection)selection).getFirstElement();
				showMessage("Double-click detected on "+obj.toString());
			}
		};
		*/
	}

	private void hookDoubleClickAction() {
		/*
		viewer.addDoubleClickListener(new IDoubleClickListener() {
			public void doubleClick(DoubleClickEvent event) {
				doubleClickAction.run();
			}
		});
		*/
	}
	private void showMessage(String message) {
		MessageDialog.openInformation(
			viewer.getControl().getShell(),
			"Runtime View",
			message);
	}

	/**
	 * Passing the focus request to the viewer's control.
	 */
	public void setFocus() {
		viewer.getControl().setFocus();
	}
	
	/* (non-Javadoc)
	 * @see java.beans.PropertyChangeListener#propertyChange(java.beans.PropertyChangeEvent)
	 */
	public void propertyChange(PropertyChangeEvent evt) {
		if (evt.getPropertyName().equals("_BLOCK_INFO")) {
			content.setContent((List)evt.getNewValue());
			Debugger.printDebug(Debugger.DEBUG_ALL,"PROC "+evt.getNewValue());
			viewer.getControl().getDisplay().syncExec(new Runnable() {			
				public void run() {
					viewer.setContentProvider(content);						
					viewer.expandToLevel(3);
				}
			});
		}
	}
}