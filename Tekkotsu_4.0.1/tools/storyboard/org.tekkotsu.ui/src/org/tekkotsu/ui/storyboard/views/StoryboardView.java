package org.tekkotsu.ui.storyboard.views;

import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;
import java.beans.PropertyChangeSupport;
import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.StringReader;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Comparator;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.SortedSet;
import java.util.TreeSet;

import org.eclipse.draw2d.IFigure;
import org.eclipse.jface.action.Action;
import org.eclipse.jface.action.IMenuListener;
import org.eclipse.jface.action.IMenuManager;
import org.eclipse.jface.action.IToolBarManager;
import org.eclipse.jface.action.MenuManager;
import org.eclipse.jface.action.Separator;
import org.eclipse.jface.dialogs.MessageDialog;
import org.eclipse.jface.resource.ImageDescriptor;
import org.eclipse.jface.viewers.ITableLabelProvider;
import org.eclipse.jface.viewers.LabelProvider;
import org.eclipse.jface.viewers.Viewer;
import org.eclipse.jface.viewers.ViewerSorter;
import org.eclipse.swt.SWT;
import org.eclipse.swt.graphics.Image;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.FileDialog;
import org.eclipse.swt.widgets.Menu;
import org.eclipse.ui.IActionBars;
import org.eclipse.ui.IPartListener;
import org.eclipse.ui.ISharedImages;
import org.eclipse.ui.IWorkbenchActionConstants;
import org.eclipse.ui.IWorkbenchPart;
import org.eclipse.ui.PlatformUI;
import org.eclipse.ui.part.ViewPart;
import org.jdom.Document;
import org.jdom.Element;
import org.jdom.input.SAXBuilder;
import org.jdom.output.Format;
import org.jdom.output.XMLOutputter;
import org.tekkotsu.ui.editor.ModelConnector;
import org.tekkotsu.ui.editor.StateMachineEditor;
import org.tekkotsu.ui.editor.model.MultiTransitionModel;
import org.tekkotsu.ui.editor.model.ViewModel;
import org.tekkotsu.ui.editor.resources.Debugger;
import org.tekkotsu.ui.storyboard.components.Block;
import org.tekkotsu.ui.storyboard.components.Transition;
import org.tekkotsu.ui.storyboard.model.AbstractModel;
import org.tekkotsu.ui.storyboard.model.BlockModel;
import org.tekkotsu.ui.storyboard.model.ImageModel;
import org.tekkotsu.ui.storyboard.model.LogModel;
import org.tekkotsu.ui.storyboard.model.TransitionModel;

public class StoryboardView extends ViewPart implements IPartListener,
		PropertyChangeListener {

	public static final double timeUnit = 1000.0;

	private StoryboardViewer viewer;

	private Action doubleClickAction;

	private StateMachineEditor editor;

	private RuntimeView runtime;

	private ContentProvider content;

	Action redrawAction, loadTraceAction, saveTraceAction;

	public class ContentProvider {
		private List blockList = new ArrayList();

		private List blockUpdate = new ArrayList();

		SAXBuilder builder;

		Element root;

		Map iconMap = new HashMap();

		int latestTime = 0;

		int offsetTime = -1;

		public ContentProvider() {
			builder = new SAXBuilder();
			blockList = new ArrayList();
			root = new Element("trace");
			initIconMap();
		}

		private void initIconMap() {
			iconMap = new HashMap();
			iconMap.put("button", "key_enter.png");
			iconMap.put("timer", "timer.png");
			iconMap.put("locomotion", "locomotion.png");
			iconMap.put("audio", "audio.png");
			iconMap.put("estop", "stop.png");
			iconMap.put("textmsg", "text.png");
		}

		public void readInput(File file) throws IOException {
			try {
				Document document = builder.build(file);
				blockList = new ArrayList();
				offsetTime = -1;
				latestTime = 0;
				parseInput(document.getRootElement());
			} catch (Exception e) {
				throw new IOException("Cannot read input file "+file);
			}
		}

		public void saveTrace(File file) throws IOException {
			// TODO mark the end of timeline and put statestop whenever user
			// pause/save
			Document document = new Document(root);			
			//document.setRootElement(root);
			try {
				XMLOutputter outputter = new XMLOutputter(Format
						.getPrettyFormat());
				Debugger.printDebug(Debugger.DEBUG_ALL, "Attempting to create "
						+ file.getCanonicalPath());
				BufferedOutputStream ostream = new BufferedOutputStream(
						new FileOutputStream(file));
				outputter.output(document, ostream);
				ostream.close();
			} catch (Exception e) {
				throw  new IOException("Unable to save trace file");				
			}
		}

		public void readInput(String filename) throws IOException {
			readInput(new File(filename));
		}

		public void getUpdate(String update) {
			try {
				Debugger.printDebug(Debugger.DEBUG_ALL, "Update:" + update);
				StringReader reader = new StringReader(update);
				Document document = builder.build(reader);
				parseInput(document.getRootElement());
			} catch (Exception e) {
				e.printStackTrace();
			}
		}

		public void clear() {
			root = new Element("trace");
			blockList = new ArrayList();
			blockUpdate = new ArrayList();
			offsetTime = -1;
			latestTime = 0;
		}
		
		/** 
		 * Mark ending for all unfinished blocks
		 */
		public void endBlocks(int time) {
            Iterator itr = blockList.iterator();
            while (itr.hasNext()) {
            	Object next = itr.next();
            	if (next instanceof BlockModel) {
            		BlockModel bm = (BlockModel) next;
            		if (!bm.isFinish()) {
            			bm.setEnd(time);
            			
            			Element evt = new Element("event");
            			Element stop = new Element("statestop");
            			stop.setAttribute("id", bm.getID());
            			stop.setAttribute("time", ""+(bm.getEnd()+offsetTime));
            			evt.addContent(stop);
            			root.addContent(evt);            			
            		}
            	}
            }
		}
		
		public void endBlocks() {
			endBlocks(latestTime);
		}

		/**
		 * @param time
		 *            (raw input from server)
		 * @return time offset relative to the beginning of the trace
		 */
		private int getTime(int time) {
			if (offsetTime == -1)
				offsetTime = time;
			time = time - offsetTime;
			if (time > latestTime)
				latestTime = time;
			firePropertyChange(ModelConnector.EVENT_TRACE_UPDATE_TIME, null,
					new Integer(time));
			return time;
		}

		private void parseInput(Element e) {
			Debugger.printDebug(Debugger.DEBUG_ALL, e.toString());
			if (e.getName().equals("trace")) {
				root = new Element("trace");

				Iterator iter = e.getChildren().iterator();
				while (iter.hasNext()) {
					Element child = (Element) iter.next();
					parseInput(child);
				}
			} else if (e.getName().equals("event")) {
				root.addContent((Element) e.clone());
				if ("image".equals(e.getAttributeValue("type"))) {
					String sid = e.getAttributeValue("sid");
					int time = getTime(Integer.parseInt(e
							.getAttributeValue("time")));
					String content = e.getChildText("image");
					ImageModel img = new ImageModel(sid, time, content);
					blockList.add(img);
					blockUpdate.add(img);
				} else if ("userlog".equals(e.getAttributeValue("type"))) {
					// retrieve log interface
					String sid = e.getAttributeValue("sid");
					int time = getTime(Integer.parseInt(e
							.getAttributeValue("time")));
					String icon = e.getAttributeValue("icon");
					String etid = e.getAttributeValue("etid");
					String content = e.getText();
					LogModel log = new LogModel(sid, time, icon, etid, content);
					blockList.add(log);
					blockUpdate.add(log);
				} else if ("log".equals(e.getAttributeValue("type"))) {
					// retrieve system log interface
					String sid = e.getAttributeValue("sid");
					int time = getTime(Integer.parseInt(e
							.getAttributeValue("time")));
					String content = "egid:" + e.getAttributeValue("egid")
							+ "\n" + e.getText();
					String egid = e.getAttributeValue("egid");
					String etid = e.getAttributeValue("etid");
					String filename = "flag.png";
					// select icon map
					String icon = (String) iconMap.get(egid);
					if (icon != null)
						filename = icon;
					LogModel log = new LogModel(sid, time, filename, etid,
							content);
					blockList.add(log);
					blockUpdate.add(log);
				} else {
					Iterator iter = e.getChildren().iterator();
					while (iter.hasNext()) {
						Element child = (Element) iter.next();
						parseInput(child);
					}
				}
			} else if (e.getName().equals("statestart")) {
				String id = e.getAttributeValue("id");
				int time = getTime(Integer
						.parseInt(e.getAttributeValue("time")));
				BlockModel bm = new BlockModel(id, time);

				for (int i = blockList.size() - 1; i >= 0; i--) {
					Object obj = blockList.get(i);
					if (obj instanceof TransitionModel) {
						TransitionModel tm = (TransitionModel) blockList.get(i);
						if (tm.isDestNode(id)) {
							tm.setEnd(time);
							blockUpdate.add(tm);
							break;
						}
					}
				}
				
				blockList.add(bm);
				blockUpdate.add(bm);
			} else if (e.getName().equals("statestop")) {
				String id = e.getAttributeValue("id");
				int time = getTime(Integer
						.parseInt(e.getAttributeValue("time")));
				for (int i = blockList.size() - 1; i >= 0; i--) {
					Object obj = blockList.get(i);
					if (obj instanceof BlockModel) {
						BlockModel bm = (BlockModel) blockList.get(i);
						if (bm.getID().equals(id)) {
							bm.setEnd(time);
							blockUpdate.add(bm);
							break;
						}
					} 					
				}
			} else if (e.getName().equals("fire")) {
				String id = e.getAttributeValue("id");
				int time = getTime(Integer
						.parseInt(e.getAttributeValue("time")));
				
				
				Object editor_obj = editor.getViewModel().getPartChild(id);
                if (editor_obj instanceof MultiTransitionModel) {
    				TransitionModel tm = new TransitionModel(id, time, (MultiTransitionModel)editor_obj);				
    				blockList.add(tm);
    				blockUpdate.add(tm);
                }
			}
		}

		public void inputChanged(Viewer v, Object oldInput, Object newInput) {
		}

		public void dispose() {
		}

		public Collection getElements() {
			// sort by ending time first, then instant objects
			SortedSet blockSet = new TreeSet(new Comparator() {
				public int compare(Object arg0, Object arg1) {
					if (arg0 instanceof AbstractModel && arg1 instanceof AbstractModel) {
						AbstractModel obj0 = (AbstractModel)arg0, obj1 = (AbstractModel)arg1;
						if (obj0.getEnd()==-1 && obj1.getEnd()>=0) return 1;
						else if (obj1.getEnd()==-1 && obj0.getEnd()>=0) return -1;

						int result = (obj0.getEnd()-obj1.getEnd());
						if (result == 0) result = obj0.getStart()-obj1.getStart();
						if (result == 0) result = obj0.getID().compareTo(obj1.getID());
						return result;
					} else throw new ClassCastException();
				};
			});
			blockSet.addAll(blockList);
			return blockSet;
		}

		public List getUpdate() {
			List updateList = new ArrayList();
			updateList.addAll(blockUpdate);
			blockUpdate.clear();
			return updateList;
		}
	}

	class ViewLabelProvider extends LabelProvider implements
			ITableLabelProvider {
		public String getColumnText(Object obj, int index) {
			return getText(obj);
		}

		public Image getColumnImage(Object obj, int index) {
			return getImage(obj);
		}

		public Image getImage(Object obj) {
			return PlatformUI.getWorkbench().getSharedImages().getImage(
					ISharedImages.IMG_OBJ_ELEMENT);
		}
	}

	class NameSorter extends ViewerSorter {
	}

	/**
	 * The constructor.
	 */
	public StoryboardView() {
	}

	/**
	 * This is a callback that will allow us to create the viewer and initialize
	 * it.
	 */
	public void createPartControl(Composite parent) {
		// viewer = new StoryboardViewer(parent, SWT.MULTI | SWT.H_SCROLL |
		// SWT.V_SCROLL);
		// viewer.setContentProvider(new ViewContentProvider());
		// viewer.setLabelProvider(new ViewLabelProvider());
		// viewer.setSorter(new NameSorter());

		viewer = new StoryboardViewer(parent, this);
		content = new ContentProvider();
		// getSite().setSelectionProvider(viewer);
		// content.readInput();
		// viewer.setInput(getViewSite());
		makeActions();
		hookContextMenu();
		// hookDoubleClickAction();
		contributeToActionBars();
		this.getViewSite().getWorkbenchWindow().getPartService()
				.addPartListener(this);
	}

	private void hookContextMenu() {
		MenuManager menuMgr = new MenuManager("#PopupMenu");
		menuMgr.setRemoveAllWhenShown(true);
		menuMgr.addMenuListener(new IMenuListener() {
			public void menuAboutToShow(IMenuManager manager) {
				StoryboardView.this.fillContextMenu(manager);
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
		manager.add(redrawAction);
		manager.add(loadTraceAction);
		manager.add(saveTraceAction);
	}

	private void fillContextMenu(IMenuManager manager) {
		manager.add(redrawAction);
		manager.add(loadTraceAction);
		manager.add(saveTraceAction);
		manager.add(new Separator(IWorkbenchActionConstants.MB_ADDITIONS));
	}

	private void fillLocalToolBar(IToolBarManager manager) {
		manager.add(redrawAction);
		manager.add(loadTraceAction);
		manager.add(saveTraceAction);
	}

	private void makeActions() {
		redrawAction = new Action() {
			public void run() {
				// showMessage("Redraw storyboard");
				viewer.setContent(content);
				viewer.setInput(editor.getViewModel(), true);
				viewer.refresh();

				// ImageObject.test();
			}
		};
		redrawAction.setText("Redraw Storyboard");
		redrawAction.setToolTipText("Redraw current storyboard");
		redrawAction.setImageDescriptor(ImageDescriptor
				.createFromFile(
						org.tekkotsu.ui.storyboard.icons.IconDummy.class,
						"desktop.gif"));

		loadTraceAction = new Action() {
			private File queryFile() {
				FileDialog dialog = new FileDialog(viewer.getControl()
						.getShell(), SWT.OPEN);
				dialog.setFilterExtensions(new String[] { "*.tse" });
				dialog.setText("Open Trace File"); //$NON-NLS-1$
				String path = dialog.open();
				if (path != null && path.length() > 0)
					return new File(path);
				return null;
			}

			public void run() {
				String filename = "";
				ContentProvider oldContent = content;
				try {
					File file = queryFile();
					filename = file.getName();
					content.readInput(file);
					content.endBlocks(viewer.timeline.getValue());
					viewer.setContent(content);
					viewer.setInput(editor.getViewModel(), true);
					viewer.refresh();
				} catch (Exception e) {
					String msg = "Cannot open trace file " + filename;
					MessageDialog.openError(viewer.getControl().getShell(),
							"Problem", msg); //$NON-NLS-1$
					content = oldContent;
					viewer.setContent(content);
					viewer.refresh();
				}
			}
		};
		loadTraceAction.setText("Load Trace");
		loadTraceAction.setToolTipText("Load Execution Trace");
		loadTraceAction.setImageDescriptor(ImageDescriptor.createFromFile(
				org.tekkotsu.ui.storyboard.icons.IconDummy.class,
				"fileopen.gif"));

		saveTraceAction = new Action() {
			private File queryFile() {
				FileDialog dialog = new FileDialog(viewer.getControl()
						.getShell(), SWT.SAVE);
				dialog.setFilterExtensions(new String[] { "*.tse" });
				dialog.setText("Save Trace File as"); //$NON-NLS-1$
				String path = dialog.open();
				if (path != null && path.length() > 0) {
					if (!path.endsWith(".tse"))
						path = path + ".tse";
					return new File(path);
				}
				return null;
			}

			public void run() {
				String filename="";
				try {
					File file = queryFile();
					filename = file.getName();
					viewer.setContent(content);
					viewer.setInput(editor.getViewModel(), true);
					viewer.refresh();
					content.saveTrace(file);
				} catch (Exception e) {
					String msg = "Cannot write trace file " + filename;
					MessageDialog.openError(viewer.getControl().getShell(), "Problem", msg);			
				}
			}
		};
		saveTraceAction.setText("Save Trace As");
		saveTraceAction.setToolTipText("Save Execution Trace");
		saveTraceAction.setImageDescriptor(ImageDescriptor.createFromFile(
				org.tekkotsu.ui.storyboard.icons.IconDummy.class, "save.gif"));
	}

	/*
	 * private void hookDoubleClickAction() { viewer.addDoubleClickListener(new
	 * IDoubleClickListener() { public void doubleClick(DoubleClickEvent event) {
	 * doubleClickAction.run(); } }); }
	 */

	private void showMessage(String message) {
		MessageDialog.openInformation(viewer.getControl().getShell(),
				"Storyboard View", message);
	}

	/**
	 * Passing the focus request to the viewer's control.
	 */
	public void setFocus() {
		viewer.getControl().setFocus();
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see java.beans.PropertyChangeListener#propertyChange(java.beans.PropertyChangeEvent)
	 */
	public void propertyChange(PropertyChangeEvent evt) {
		// System.out.println(evt.getPropertyName());
		if (evt.getPropertyName()
				.equals(ModelConnector.EVENT_TRACE_UPDATE_TIME)) {
			final PropertyChangeEvent ev = evt;
			viewer.display.syncExec(new Runnable() {
				public void run() {
					int value = ((Integer) ev.getNewValue()).intValue();
					// System.out.println("update evt: " + value);
					viewer.timeline.setValue(value);
					viewer.storyboard.setValue(value);
					viewer.logBar.setValue(value);
				}
			});
		} else if (evt.getPropertyName().equals(ViewModel.P_EXECUTION_TRACE)) {
			// load trace from file
			ContentProvider oldContent = content;
			try {
				String contentname = (String) evt.getNewValue();
				content.readInput(contentname);
				viewer.setContent(content);
				viewer.setInput(editor.getViewModel(), true);
				viewer.refresh();
			} catch (Exception e) {
				content = oldContent;
				viewer.setContent(content);
				viewer.setInput(editor.getViewModel(), true);
				viewer.refresh();
			}
		} else if (evt.getPropertyName().equals(
				ModelConnector.EVENT_TRACE_CLEAR)) {
			// clear trace
			content.clear();
			viewer.setContent(content);
			viewer.setInput(editor.getViewModel(), true);
			viewer.refresh();
		} else if (evt.getPropertyName().equals(
				ModelConnector.EVENT_TRACE_REFRESH)) {
			// refresh the board
			// content.getUpdate((String)evt.getNewValue());
			viewer.setContent(content);
			viewer.setInput(editor.getViewModel(), true);
			viewer.refresh();
		} else if (evt.getPropertyName().equals(
				ModelConnector.EVENT_TRACE_UPDATE)) {
			// update trace
			content.getUpdate((String) evt.getNewValue());
			viewer.setContent(content);
			viewer.setInput(editor.getViewModel(), false);
			viewer.refresh();
		} else if (evt.getPropertyName().equals(
				ModelConnector.EVENT_TRACE_MODE_ENTER)) {
			viewer.timeline.setLock(true);
		} else if (evt.getPropertyName().equals(
				ModelConnector.EVENT_TRACE_MODE_EXIT)) {
			content.endBlocks(viewer.timeline.getValue());
			viewer.timeline.setLock(false);			
		}
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see org.eclipse.ui.IPartListener#partActivated(org.eclipse.ui.IWorkbenchPart)
	 */
	public void partActivated(IWorkbenchPart part) {
		// partOpened(part);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see org.eclipse.ui.IPartListener#partBroughtToTop(org.eclipse.ui.IWorkbenchPart)
	 */
	public void partBroughtToTop(IWorkbenchPart part) {
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see org.eclipse.ui.IPartListener#partClosed(org.eclipse.ui.IWorkbenchPart)
	 */
	public void partClosed(IWorkbenchPart part) {
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see org.eclipse.ui.IPartListener#partDeactivated(org.eclipse.ui.IWorkbenchPart)
	 */
	public void partDeactivated(IWorkbenchPart part) {
		// partOpened(part);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see org.eclipse.ui.IPartListener#partOpened(org.eclipse.ui.IWorkbenchPart)
	 */
	public void partOpened(IWorkbenchPart part) {
		if (part instanceof StateMachineEditor) {
			editor = (StateMachineEditor) part;
			String contentname = editor.getViewModel().getTracePath();
			editor.getViewModel().addPropertyChangeListener(this);
			editor.getModelConnector().addPropertyChangeListener(this);
			this.addPropertyChangeListener(editor.getModelConnector());
			viewer.addPropertyChangeListener(editor);
			if (contentname != null) {
				ContentProvider oldContent = content;
				try {
					content.readInput(contentname);
					viewer.setContent(content);
					viewer.setInput(editor.getViewModel());
				} catch (Exception e) {
					content = oldContent;
					viewer.setContent(content);
					viewer.setInput(editor.getViewModel());
				}
			}
		} else if (part instanceof RuntimeView) {
			runtime = (RuntimeView) part;
			viewer.addPropertyChangeListener(runtime);
		} else if (part instanceof ImageView) {
			viewer.addPropertyChangeListener((ImageView) part);
		}
	}

	private PropertyChangeSupport listeners = new PropertyChangeSupport(this);

	public void addPropertyChangeListener(PropertyChangeListener listener) {
		listeners.addPropertyChangeListener(listener);
	}

	public void firePropertyChange(String propName, Object oldValue,
			Object newValue) {
		listeners.firePropertyChange(propName, oldValue, newValue);
	}

	public void removePropertyChangeListener(PropertyChangeListener listener) {
		listeners.removePropertyChangeListener(listener);
	}

}