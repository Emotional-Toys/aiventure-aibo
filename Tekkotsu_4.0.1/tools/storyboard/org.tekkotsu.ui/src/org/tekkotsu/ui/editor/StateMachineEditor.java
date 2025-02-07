package org.tekkotsu.ui.editor;

/**
 * @author asangpet
 */

import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;
import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.StringReader;
import java.io.StringWriter;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;

import org.eclipse.core.runtime.IPath;
import org.eclipse.core.runtime.IProgressMonitor;
import org.eclipse.core.runtime.Path;
import org.eclipse.draw2d.geometry.Point;
import org.eclipse.gef.ContextMenuProvider;
import org.eclipse.gef.DefaultEditDomain;
import org.eclipse.gef.EditDomain;
import org.eclipse.gef.EditPart;
import org.eclipse.gef.EditPolicy;
import org.eclipse.gef.GraphicalViewer;
import org.eclipse.gef.commands.CommandStack;
import org.eclipse.gef.palette.CreationToolEntry;
import org.eclipse.gef.palette.MarqueeToolEntry;
import org.eclipse.gef.palette.PaletteDrawer;
import org.eclipse.gef.palette.PaletteGroup;
import org.eclipse.gef.palette.PaletteRoot;
import org.eclipse.gef.palette.SelectionToolEntry;
import org.eclipse.gef.palette.ToolEntry;
import org.eclipse.gef.requests.CreateRequest;
import org.eclipse.gef.requests.SimpleFactory;
import org.eclipse.gef.ui.actions.ActionRegistry;
import org.eclipse.gef.ui.parts.ScrollingGraphicalViewer;
import org.eclipse.jface.action.Action;
import org.eclipse.jface.dialogs.MessageDialog;
import org.eclipse.jface.resource.ImageDescriptor;
import org.eclipse.jface.viewers.ISelection;
import org.eclipse.swt.SWT;
import org.eclipse.swt.custom.SashForm;
import org.eclipse.swt.events.FocusAdapter;
import org.eclipse.swt.events.FocusEvent;
import org.eclipse.swt.events.SelectionAdapter;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.layout.FillLayout;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.FileDialog;
import org.eclipse.swt.widgets.Label;
import org.eclipse.swt.widgets.Shell;
import org.eclipse.swt.widgets.Text;
import org.eclipse.ui.IEditorDescriptor;
import org.eclipse.ui.IEditorInput;
import org.eclipse.ui.IEditorRegistry;
import org.eclipse.ui.IEditorSite;
import org.eclipse.ui.IPathEditorInput;
import org.eclipse.ui.IPersistableElement;
import org.eclipse.ui.IWorkbench;
import org.eclipse.ui.IWorkbenchPage;
import org.eclipse.ui.IWorkbenchPart;
import org.eclipse.ui.PartInitException;
import org.eclipse.ui.PlatformUI;
import org.jdom.Document;
import org.jdom.Element;
import org.jdom.input.SAXBuilder;
import org.jdom.output.Format;
import org.jdom.output.XMLOutputter;
import org.tekkotsu.ui.editor.editparts.MyEditPartFactory;
import org.tekkotsu.ui.editor.editparts.ViewEditPart;
import org.tekkotsu.ui.editor.editpolicies.MyXYLayoutEditPolicy;
import org.tekkotsu.ui.editor.icons.IconDummy;
import org.tekkotsu.ui.editor.model.AbstractModel;
import org.tekkotsu.ui.editor.model.SourceModel;
import org.tekkotsu.ui.editor.model.SourceNodeModel;
import org.tekkotsu.ui.editor.model.SourceTransitionModel;
import org.tekkotsu.ui.editor.model.StateNodeModel;
import org.tekkotsu.ui.editor.model.ViewModel;
import org.tekkotsu.ui.editor.resources.Debugger;
import org.tekkotsu.ui.editor.resources.IDTag;

/**
 * @author asangpet
 */
public class StateMachineEditor extends MultiPageGraphicalEditorWithPalette
		implements PropertyChangeListener {

	static final int PAGE_PREVIEW = 0;
	static final int PAGE_EDITOR = 1;
	static final int PAGE_SOURCE_MODEL = 2;
	static final int PAGE_SOURCE_LAYOUT = 3;

	private Text modelText, modelSourceText, modelPreviewText;
	private Button pauseButton;
	
	private Text viewSourceText;

	private ViewModel viewModel;

	private MyXYLayoutEditPolicy editPolicy;

	private ScrollingGraphicalViewer testViewer;

	ModelConnector modelConnector = null;

	private PropertyChangeEvent event;
	private boolean dirty_flag = false;

	public StateMachineEditor() {
		setEditDomain(new DefaultEditDomain(this));
		initModelConnector();
		editPolicy = new MyXYLayoutEditPolicy();
	}

	public void setDirty() {
		dirty_flag = true;
		firePropertyChange(PROP_DIRTY);
	}
	
	public void propertyChange(PropertyChangeEvent evt) {
		if (evt.getPropertyName().equals(ModelConnector.EVENT_DATA_UPDATE)) {
			this.event = evt;
			modelPreviewText.getDisplay().syncExec(new Runnable() {
				public void run() {
					modelPreviewText.append(event.getNewValue()+"\r\n");
				}
			});
			modelSourceText.getDisplay().syncExec(new Runnable() {
				public void run() {
					modelSourceText.append(event.getNewValue() + "\r\n");
				}
			});
		} else if (evt.getPropertyName().equals(
				ModelConnector.EVENT_MODEL_UPDATE)) {
			String buf = (String) evt.getNewValue();
			Debugger.printDebug(Debugger.DEBUG_ALL,buf);
			viewModel.getSourceModel().setSourceInput(new StringReader(buf));
			modelPreviewText.getDisplay().syncExec(new Runnable() {
				public void run() {
					viewModelSource();
					createModelActions(viewModel);
					viewModel.verifySource();
					if (viewModel.getPartChildren().size()==0) {						
						if (MessageDialog.openQuestion(testViewer.getControl().getShell(),"New Layout","Would you like the state machine layout to be automatically generated?")) {
							CreateLayoutAction action = new CreateLayoutAction(viewModel.getSourceModel());
							action.run();							
						}
					}
				}
			});
		} else {
			// forward change to ViewModel
			final PropertyChangeEvent ev = evt;
			testViewer.getControl().getDisplay().syncExec(new Runnable() {
				public void run() {
					viewModel.propertyChange(ev);
				}
			});
		}
	}

	protected void configureGraphicalViewer() {
		super.configureGraphicalViewer();
		GraphicalViewer viewer = getGraphicalViewer();
		viewer.setEditPartFactory(new MyEditPartFactory());

		// configure the context menu provider
		ContextMenuProvider cmProvider = new StateMachineEditorContextMenuProvider(
				viewer, getActionRegistry());
		viewer.setContextMenu(cmProvider);
		getSite().registerContextMenu(cmProvider, viewer);
	}

	public class CreateNodeAction extends Action {
		private SourceNodeModel model;

		private CreateRequest request;

		public CreateNodeAction(SourceNodeModel model) {
			super(model.getId(), Action.AS_CHECK_BOX);
			this.model = model;
			request = new CreateRequest();
		}

		public SourceNodeModel getModel() {
			return model;
		}

		public CreateRequest getRequest() {
			return request;
		}

		public void run() {
			EditDomain domain = getEditDomain();
			CommandStack stack = domain.getCommandStack();
			GraphicalViewer view = getGraphicalViewer();
			request.setFactory(model.getFactory());
			stack.execute(view.getContents().getEditPolicy(
					EditPolicy.LAYOUT_ROLE).getCommand(request));
			setDirty();
		}
	}

	public class CreateTransitionAction extends Action {
		private SourceTransitionModel model;

		private CreateRequest request;

		public CreateTransitionAction(SourceTransitionModel model) {
			super(model.getId(), Action.AS_CHECK_BOX);
			this.model = model;
			request = new CreateRequest();
		}

		public SourceTransitionModel getModel() {
			return model;
		}

		public CreateRequest getRequest() {
			return request;
		}

		/**
		 * generate source/target node for single pair transition model
		 */
		private void createSourceTarget() {
			SourceModel src = viewModel.getSourceModel();
			CreateNodeAction createSrc = new CreateNodeAction(
					(SourceNodeModel) src.getChild(model.getSourceNodes()
							.get(0).toString()));
			CreateNodeAction createDest = new CreateNodeAction(
					(SourceNodeModel) src.getChild(model.getDestNodes().get(0)
							.toString()));
			createSrc.getRequest().setLocation(request.getLocation());
			createDest.getRequest().setLocation(
					(new Point(request.getLocation())).translate(40, 40));
			createSrc.run();
			createDest.run();
			HashMap data = new HashMap();
			data.put("_source", viewModel.getPartChild(model.getSourceNodes()
					.get(0).toString()));
			data.put("_target", viewModel.getPartChild(model.getDestNodes()
					.get(0).toString()));
			request.setExtendedData(data);
		}

		public void run() {
			EditDomain domain = getEditDomain();
			CommandStack stack = domain.getCommandStack();
			GraphicalViewer view = getGraphicalViewer();
			request.setFactory(model.getFactory((ViewModel) view.getContents()
					.getModel()));
			Debugger.printDebug(Debugger.DEBUG_ALL,
					"Create transition action runned: "
							+ request.getNewObjectType());
			if (!model.isMultiTransition()) {
				createSourceTarget();
			}
			stack.execute(view.getContents().getEditPolicy(
					EditPolicy.LAYOUT_ROLE).getCommand(request));
			setDirty();
		}
	}

	/**
	 * This action automatically generate layout for all node and transition
	 */
	public class CreateLayoutAction extends Action {
		private SourceModel model;

		private CreateRequest request;

		public CreateLayoutAction(SourceModel model) {
			super("Generate layout");
			this.model = model;
			request = new CreateRequest();
		}

		public SourceModel getModel() {
			return model;
		}

		public CreateRequest getRequest() {
			return request;
		}

		private void createLayout() {
			// create nodes
			List nodeList = model.getNodes();
			Iterator iter = nodeList.iterator();
			Point curPoint = request.getLocation();
			
			int radius = 1000;
			int wmin = getContainer().getBounds().width/2;
			int hmin = getContainer().getBounds().height/2;
			if (wmin<hmin) radius = wmin-60; else radius = hmin-60;
			curPoint = new Point(radius+60,getContainer().getBounds().height/2-30);

			int count = 0;
			while (iter.hasNext()) {
				SourceNodeModel node = (SourceNodeModel)iter.next();
				CreateNodeAction genNode = new CreateNodeAction(node);
				int dx = (int)Math.round(radius*Math.cos(2*Math.PI/(nodeList.size())*count));
				int dy = (int)Math.round(radius*Math.sin(2*Math.PI/(nodeList.size())*count));
				genNode.getRequest().setLocation(curPoint.getTranslated(dx,dy));
				//HashMap extendedData = new HashMap();
				//extendedData.put(IDTag.XML_common_color, java.awt.Color.getHSBColor((float)(count/nodeList.size()),.99f,.8f));
				//genNode.getRequest().setExtendedData(extendedData);
				genNode.run();
				count++;
			}
			
			//create transitions
			List transList = model.getTransitions();
			iter = transList.iterator();
			while (iter.hasNext()) {
				SourceTransitionModel trans = (SourceTransitionModel)iter.next();
				CreateTransitionAction genTrans = new CreateTransitionAction(trans);
				genTrans.getRequest().setLocation(curPoint);
				curPoint.translate(40,0);
				genTrans.run();
			}			
		}

		public void run() {
			EditDomain domain = getEditDomain();
			CommandStack stack = domain.getCommandStack();
			GraphicalViewer view = getGraphicalViewer();
			
			try {
			SourceNodeModel nodeModel = (SourceNodeModel)model.getNodes().get(0);
			request.setFactory(nodeModel.getFactory());

			Debugger.printDebug(Debugger.DEBUG_ALL,
					"Generate entire layout.");
			createLayout();
			setDirty();
			} catch (Exception e) {
				MessageDialog msgDialog = new MessageDialog(
						view.getControl().getShell(),"Error",null,"No model loaded, you must pick an existing model or upload a new one.",SWT.ICON_ERROR,new String[] {"OK"},0);
				msgDialog.open();
			}
		}
	}
	
	private void createModelActions(ViewModel viewModel) {
		ActionRegistry actionRegistry = this.getActionRegistry();
		Iterator iter = actionRegistry.getActions();
		List keyList = new ArrayList();
		while (iter.hasNext()) {
			keyList.add(((Action) iter.next()));
		}
		iter = keyList.iterator();
		while (iter.hasNext()) {
			actionRegistry.removeAction((Action) iter.next());
		}
		// actionRegistry.dispose();
		// actionRegistry = new ActionRegistry();

		super.createActions();

		// generate actions for adding state and transition
		iter = viewModel.getSourceModel().getNodes().iterator();
		while (iter.hasNext()) {
			SourceNodeModel model = (SourceNodeModel) iter.next();
			Action testAction = new CreateNodeAction(model);
			testAction.setId(IDTag.ACTION_add_state + model.getId());
			actionRegistry.registerAction(testAction);
		}
		iter = viewModel.getSourceModel().getTransitions().iterator();
		while (iter.hasNext()) {
			SourceTransitionModel model = (SourceTransitionModel) iter.next();
			Action testAction = new CreateTransitionAction(model);
			testAction.setId(IDTag.ACTION_add_transition + model.getId());
			actionRegistry.registerAction(testAction);
		}
		Action layoutAction = new CreateLayoutAction(viewModel.getSourceModel());
		layoutAction.setId(IDTag.ACTION_add_all);
		actionRegistry.registerAction(layoutAction);
	}

	/**
	 * @see org.eclipse.ui.IWorkbenchPart#createPartControl(org.eclipse.swt.widgets.Composite)
	 */
	public void createLayoutEditorPage() {
		Composite parent = this.getContainer();
		/*
		 * Splitter splitter = new Splitter(parent, SWT.HORIZONTAL);
		 * createPaletteViewer(splitter); createGraphicalViewer(splitter);
		 * 
		 * splitter.maintainSize(getPaletteViewer().getControl());
		 * splitter.setFixedSize(getInitialPaletteSize());
		 * splitter.addFixedSizeChangeListener(new PropertyChangeListener() {
		 * public void propertyChange(PropertyChangeEvent evt) {
		 * handlePaletteResized(((Splitter)evt.getSource()).getFixedSize()); }
		 * });
		 */
		createGraphicalViewer(parent);

		// int index = addPage(splitter);
		createModelActions(viewModel);

		//
		//int index = addPage(getGraphicalViewer().getControl());
		//setPageText(index, "Layout");
	}

	protected void createPages() {
		// initialize editor first
		createLayoutEditorPage();

		createPreviewPage();
		
		// add editor page to tab
		int index = addPage(getGraphicalViewer().getControl());
		setPageText(index, "Layout");

		createModelEditorPage();
		createSourceEditorPage();
	}

	void initModelConnector() {
		modelConnector = new ModelConnector();
		modelConnector.addPropertyChangeListener(this);
	}

	public void createModelEditorPage() {
		if (modelConnector == null)
			initModelConnector();
		Composite composite = new Composite(getContainer(), SWT.NONE);
		GridLayout layout = new GridLayout();
		composite.setLayout(layout);
		layout.numColumns = 6;

		GridData gd = new GridData();
		gd.horizontalSpan = 1;
		Label modelLabel = new Label(composite, SWT.NONE);
		modelLabel.setText("Model source:");
		modelLabel.setLayoutData(gd);

		gd = new GridData(GridData.FILL_HORIZONTAL);
		gd.horizontalSpan = 3;
		modelText = new Text(composite, SWT.BORDER);
		modelText.setLayoutData(gd);
		modelText.setText(viewModel.getSourceModel().getSourcePath()+"");
		modelText.setEditable(false);

		gd = new GridData();
		gd.horizontalSpan = 1;
		Button modelButton = new Button(composite, SWT.NONE);
		modelButton.setLayoutData(gd);
		modelButton.setText("Browse...");

		modelButton.addSelectionListener(new SelectionAdapter() {
			public void widgetSelected(SelectionEvent event) {
				String[] filterExtensions = { "*.tsm", "*.*" };
				FileDialog fileDialog = new FileDialog(getContainer()
						.getShell(), SWT.OPEN);
				fileDialog.setText("Model source");
				fileDialog.setFilterExtensions(filterExtensions);
				viewModel.setModelSource(fileDialog.open());
				createModelActions(viewModel);
				modelText.setText(viewModel.getSourceModel().getSourcePath().toString());
				viewModelSource();
			}
		});

		gd = new GridData();
		gd.horizontalSpan = 1;
		Button saveButton = new Button(composite, SWT.NONE);
		saveButton.setText("Save");
		saveButton.setLayoutData(gd);
		saveButton.addSelectionListener(new SelectionAdapter() {
			public void widgetSelected(SelectionEvent event) {
				viewModel.getSourceModel().doSave();
			}
		});

		gd = new GridData(GridData.FILL_BOTH);
		gd.horizontalSpan = 6;
		modelSourceText = new Text(composite, SWT.H_SCROLL | SWT.V_SCROLL);
		modelSourceText.setEditable(false);
		modelSourceText.setLayoutData(gd);

		int index = addPage(composite);
		setPageText(index, "Model Source");

	}

	public void setModelSource(String path) {
		viewModel.setModelSource(path);
		createModelActions(viewModel);
		modelText.setText(viewModel.getSourceModel().getSourcePath().toString());
		viewModelSource();		
	}
	
	public void createSourceEditorPage() {
		Composite composite = new Composite(getContainer(), SWT.NONE);
		FillLayout layout = new FillLayout();
		composite.setLayout(layout);
		viewSourceText = new Text(composite, SWT.H_SCROLL | SWT.V_SCROLL);
		viewSourceText.setEditable(false);

		int index = addPage(composite);
		// setPageText(index, getEditorInput().getName());
		setPageText(index, "Layout Source");
	}

	public void createPreviewPage() {
		SashForm splitter = new SashForm(getContainer(), SWT.HORIZONTAL);
		testViewer = new ScrollingGraphicalViewer();
		testViewer.createControl(splitter);

		viewModel.setPreview(true);
		testViewer.setEditPartFactory(new MyEditPartFactory());
		testViewer.setContents(viewModel);
		final ImageDescriptor iconPause = ImageDescriptor.createFromFile(IconDummy.class, "pause_icon.gif");
		final ImageDescriptor iconResume = ImageDescriptor.createFromFile(IconDummy.class, "resume_icon.gif");

		Composite controlComposite = new Composite(splitter, SWT.NORMAL);

		GridLayout layout = new GridLayout();
		controlComposite.setLayout(layout);
		layout.numColumns = 8;

		GridData gd = new GridData();
		gd.horizontalSpan = 1;
		Label label = new Label(controlComposite, SWT.NONE);
		label.setText("Host");
		label.setLayoutData(gd);

		gd = new GridData(SWT.FILL, SWT.FILL, true, false);
		gd.horizontalSpan = 4;
		Text hostText = new Text(controlComposite, SWT.BORDER);
		hostText.setLayoutData(gd);
		hostText.setText(modelConnector.getHostName());
		hostText.addFocusListener(new FocusAdapter() {
			public void focusLost(FocusEvent e) {
				modelConnector.setHostName(((Text) e.getSource()).getText());
			}
		});

		gd = new GridData();
		gd.horizontalSpan = 1;
		label = new Label(controlComposite, SWT.NONE);
		label.setText("Port");

		gd = new GridData(SWT.FILL, SWT.FILL, true, false);
		gd.horizontalSpan = 2;
		Text portText = new Text(controlComposite, SWT.BORDER);
		portText.setLayoutData(gd);
		portText.setText("" + modelConnector.getHostPort());
		portText.addFocusListener(new FocusAdapter() {
			public void focusLost(FocusEvent e) {
				modelConnector.setHostPort(Integer.parseInt(((Text) e
						.getSource()).getText()));
			}
		});

		gd = new GridData();
		gd.horizontalSpan = 1;
		Label labelmach = new Label(controlComposite, SWT.NONE);
		labelmach.setText("Name");
		gd.horizontalAlignment = SWT.RIGHT;
		labelmach.setLayoutData(gd);

		gd = new GridData(GridData.FILL_HORIZONTAL);
		gd.horizontalSpan = 7;
		Text machText = new Text(controlComposite, SWT.BORDER);
		machText.setLayoutData(gd);
		machText.setText(modelConnector.getMachineName());
		machText.addFocusListener(new FocusAdapter() {
			public void focusLost(FocusEvent e) {
				modelConnector.setMachineName(((Text) e.getSource()).getText());
			}
		});

		gd = new GridData(GridData.FILL_HORIZONTAL);
		gd.horizontalSpan = 3;
		final Button loadModelButton = new Button(controlComposite, SWT.NONE);
		loadModelButton.setText("Download Model");
		loadModelButton.setToolTipText("Download Model");
		loadModelButton.setLayoutData(gd);
		loadModelButton.addSelectionListener(new SelectionAdapter() {
			public void widgetSelected(SelectionEvent event) {
				// start up client loop
				modelConnector.execute("spider");
			}
		});

		gd = new GridData(SWT.FILL, SWT.FILL, true, false);
		gd.horizontalSpan = 3;
		final Button updateButton = new Button(controlComposite, SWT.NONE);
		updateButton.setText("New Trace");
		updateButton.setToolTipText("New Trace");
		updateButton.setLayoutData(gd);
		updateButton.addSelectionListener(new SelectionAdapter() {
			public void widgetSelected(SelectionEvent event) {
				// start up client loop				
				modelConnector.firePropertyChange(ModelConnector.EVENT_TRACE_CLEAR, null, null);
				modelConnector.firePropertyChange(ModelConnector.EVENT_TRACE_MODE_ENTER, null, null);
				modelConnector.newTrace();
				//modelConnector.execute("listen");
				pauseButton.setToolTipText("Pause");
				pauseButton.setImage(iconPause.createImage());
			}
		});

		gd = new GridData(SWT.FILL, SWT.FILL, true, false);
		gd.horizontalSpan = 1;
		pauseButton = new Button(controlComposite, SWT.NONE);
		
		pauseButton.setImage(iconResume.createImage());
		pauseButton.setImage(iconPause.createImage());	
		
		pauseButton.setToolTipText("Pause");
		pauseButton.setLayoutData(gd);
		pauseButton.addSelectionListener(new SelectionAdapter() {
			public void widgetSelected(SelectionEvent event) {
				if (!modelConnector.isStop()) {
					modelConnector.firePropertyChange(ModelConnector.EVENT_TRACE_MODE_EXIT, null, null);
					modelConnector.stop();
					pauseButton.setToolTipText("Resume");
					pauseButton.setImage(iconResume.createImage());
				} else {
					modelConnector.firePropertyChange(ModelConnector.EVENT_TRACE_MODE_ENTER, null, null);
					modelConnector.resume();
					pauseButton.setToolTipText("Pause");
					pauseButton.setImage(iconPause.createImage());
				}
			}
		});

		gd = new GridData(SWT.FILL, SWT.FILL, true, false);
		gd.horizontalSpan = 1;
		Button reconnectButton = new Button(controlComposite, SWT.NONE);
		//reconnectButton.setText("Reconnect");
		ImageDescriptor iconReconn = ImageDescriptor.createFromFile(IconDummy.class, "chasingarrows.gif");
		reconnectButton.setImage(iconReconn.createImage());
		reconnectButton.setToolTipText("Reconnect");
		reconnectButton.setLayoutData(gd);
		reconnectButton.addSelectionListener(new SelectionAdapter() {
			public void widgetSelected(SelectionEvent event) {
				modelConnector.stop();
				modelConnector.disconnect();
				modelConnector.connect();
			}
		});
		
		gd = new GridData(SWT.FILL, SWT.FILL, true, true);
		gd.horizontalSpan = 8;
		modelPreviewText = new Text(controlComposite, SWT.H_SCROLL
				| SWT.V_SCROLL);
		modelPreviewText.setLayoutData(gd);

		splitter.setWeights(new int[] { 60, 40 });
		int index = addPage(splitter);
		setPageText(index, "Monitor");
	}

	private void doSaveOutput(String outputPath) {
		try {
			GraphicalViewer viewer = getGraphicalViewer();
			EditPart content = viewer.getContents();
			AbstractModel rootModel = (AbstractModel) content.getModel();

			Document doc = new Document();
			doc.addContent(rootModel.getXML());

			XMLOutputter outputter = new XMLOutputter(Format.getPrettyFormat());
			BufferedOutputStream ostream = new BufferedOutputStream(
					new FileOutputStream(outputPath));
			outputter.output(doc, ostream);
			ostream.close();

			Debugger.printDebug(Debugger.DEBUG_ALL,getEditorInput().getName());
			Debugger.printDebug(Debugger.DEBUG_ALL,outputter.outputString(doc));
			
			getEditDomain().getCommandStack().markSaveLocation();
			
			// save model (always) vs if in model page
			// if (this.getActivePage() == PAGE_SOURCE_MODEL)
			viewModel.getSourceModel().doSave();
			
			dirty_flag = false;
			firePropertyChange(PROP_DIRTY);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	public void doSave(IProgressMonitor monitor) {
		if (getEditorInput().getName().indexOf("noname")>-1) {
			doSaveAs();
		} else {
			doSaveOutput(getEditorInput().getName());
		}
	}

	private IEditorInput createEditorInput(File file) {
		IPath location = new Path(file.getAbsolutePath());
		PathEditorInput input = new PathEditorInput(location);
		return input;
	}

	private String getEditorId(File file) {
		IWorkbench workbench= getEditorSite().getWorkbenchWindow().getWorkbench();
		IEditorRegistry editorRegistry= workbench.getEditorRegistry();
		IEditorDescriptor descriptor= editorRegistry.getDefaultEditor(file.getName());
		if (descriptor != null)
			return descriptor.getId();
		return "org.tekkotsu.ui.rcp.editors.SimpleEditor"; //$NON-NLS-1$
	}

	public void doSaveAs() {
		String[] filterExtensions = { "*.tsl" };
		FileDialog fileDialog = new FileDialog(getContainer().getShell(),
				SWT.SAVE);
		fileDialog.setText("Save As");
		fileDialog.setFilterExtensions(filterExtensions);
		String filename = fileDialog.open();
		if (filename!=null) {
			String namefix = filename;
			if (!filename.endsWith(".tsl")) {
				namefix=filename;
				filename+=".tsl";
			} else namefix = filename.substring(0,filename.length()-4);
			// optionally save model file
			if (viewModel.getSourceModel().getSourcePath().toString().indexOf("noname")>-1) {
				viewModel.getSourceModel().doSave(new Path(namefix+".tsm"));
				viewModel.setModelSource(namefix+".tsm");
			};
			
			doSaveOutput(filename);

			// open/close editor
			File file = new File(filename);
			IEditorInput input= createEditorInput(file);
			String editorId= getEditorId(file);			
			IWorkbenchPage page= getEditorSite().getWorkbenchWindow().getActivePage();
			page.closeEditor(page.getActiveEditor(),false);
			try {				
				page.openEditor(input, editorId);				
			} catch (PartInitException e) {
				//e.printStackTrace();
			}
		}
	}

	/**
	 * Generate the layout model from XML document
	 * 
	 * @param document
	 *            view layout file
	 * @return generated layout model for GraphicalViewer
	 */
	private ViewModel generateModel(IEditorSite site, Document document, IPath path) {
		Element root = document.getRootElement();
		Debugger.printDebug(Debugger.DEBUG_ALL,root.getName());		

		if ("view".equals(root.getName())) {								
			return new ViewModel(site, root,path);
		} else
			return new ViewModel(path);
	}

	protected PaletteRoot getPaletteRoot() {
		PaletteRoot root = new PaletteRoot();
		PaletteGroup toolGroup = new PaletteGroup("Tool");

		// Add SelectionTool entry
		ToolEntry tool = new SelectionToolEntry();
		toolGroup.add(tool);
		root.setDefaultEntry(tool);

		// Add MarqueeTool entry
		tool = new MarqueeToolEntry();
		toolGroup.add(tool);

		PaletteDrawer drawer = new PaletteDrawer("Object");
		ImageDescriptor descriptor = ImageDescriptor.createFromFile(
				StateMachineEditor.class, "icons/newModel.gif");
		CreationToolEntry creationEntry = new CreationToolEntry("State node",
				"Create a new state", new SimpleFactory(StateNodeModel.class),
				descriptor, descriptor);
		drawer.add(creationEntry);

		/*
		 * descriptor = ImageDescriptor.createFromFile(StateMachineEditor.class,
		 * "icons/arrowConnection.gif"); CreationToolEntry
		 * arrowConnxCreationEntry = new
		 * ConnectionCreationToolEntry("Transition","Create a new
		 * transition",new
		 * SimpleFactory(TransitionModel.class),descriptor,descriptor);
		 * drawer.add(arrowConnxCreationEntry);
		 */

		root.add(toolGroup);
		root.add(drawer);
		return root;
	}

	public void init(IEditorSite site, IEditorInput input)
			throws PartInitException {
		super.init(site, input);
		this.setPartName(input.getName());
		String pathName = ((IPathEditorInput) input).getPath().toOSString();
		Debugger.printDebug(Debugger.DEBUG_ALL,pathName);
				
		// read state information from file and construct the model
		try {
			SAXBuilder builder = new SAXBuilder();
			Document document = builder.build(new File(pathName));
			viewModel = generateModel(site, document,((IPathEditorInput)input).getPath().makeAbsolute());						
			
			getEditDomain().getCommandStack().markSaveLocation();
		} catch (Exception e) {
			// This is just an example. All exceptions caught here.
			// e.printStackTrace();
			// Initialize a simple model
			ViewModel parent = new ViewModel(((IPathEditorInput)input).getPath().makeAbsolute());

			//StateNodeModel state1 = new StateNodeModel();
			//state1.setConstraint(new Rectangle(0, 0, -1, -1));
			//parent.addChild(state1);

			viewModel = parent;
		}
		viewModel.setEditor(this);
	}

	protected void initializeGraphicalViewer() {
		GraphicalViewer viewer = getGraphicalViewer();
		viewer.setContents(viewModel);
	}

	public boolean isDirty() {
		return dirty_flag;
		//return getEditDomain().getCommandStack().isDirty();
	}

	public boolean isSaveAsAllowed() {
		return true;
	}
	
	protected void pageChange(int newPageIndex) {
		super.pageChange(newPageIndex);
		setDirty();
		if (newPageIndex == PAGE_EDITOR) {
			viewModel.setPreview(false);
			getSite().getPage().removeSelectionListener(this);
		} else if (newPageIndex == PAGE_SOURCE_MODEL) {
			viewModelSource();
		} else if (newPageIndex == PAGE_SOURCE_LAYOUT) {
			viewSource();
		} else if (newPageIndex == PAGE_PREVIEW) {
			viewModel.setPreview(true);
			testViewer.setEditPartFactory(new MyEditPartFactory());
			testViewer.setContents(viewModel);
			getSite().getPage().addSelectionListener(this);
		}
	}

	public void selectionChanged(IWorkbenchPart part, ISelection selection) {
		if (part.getSite().getWorkbenchWindow().getActivePage() == null)
			return;
		Debugger.printDebug(Debugger.DEBUG_ALL,"Selection change receive " + selection);

		super.selectionChanged(part, selection);
	}

	// display model source on the model source page
	private void viewModelSource() {
		SourceModel sourceModel = viewModel.getSourceModel();
		modelText.setText(sourceModel.getSourcePath().toString());
		try {
			Document doc = new Document();
			doc.addContent(sourceModel.getXML());

			XMLOutputter outputter = new XMLOutputter(Format.getPrettyFormat());
			StringWriter displayText = new StringWriter();
			outputter.output(doc, displayText);
			modelSourceText.setText(displayText.toString());
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	private void viewSource() {
		try {
			GraphicalViewer viewer = getGraphicalViewer();
			EditPart content = viewer.getContents();
			AbstractModel rootModel = (AbstractModel) content.getModel();

			Document doc = new Document();
			doc.addContent(rootModel.getXML());

			XMLOutputter outputter = new XMLOutputter(Format.getPrettyFormat());
			StringWriter displayText = new StringWriter();
			outputter.output(doc, displayText);

			if (Debugger.getDebugLevel() == Debugger.DEBUG_ALL) {
				ViewEditPart viewContent = (ViewEditPart) content;
				displayText.write("\r\nCommand stack size:"
						+ this.getCommandStack().getCommands().length);
				Object[] cmds = this.getCommandStack().getCommands();
				for (int i = 0; i < cmds.length; i++)
					displayText.write("\r\n" + cmds[i]);

				displayText.write("\r\nViewEditPart list size : "
						+ viewContent.getChildren().size() + " ");
				Iterator iter = viewContent.getChildren().iterator();
				while (iter.hasNext()) {
					displayText.write("\r\n" + iter.next());
				}

				ViewModel viewModel = (ViewModel) content.getModel();
				displayText.write("\r\nViewModel list size : "
						+ viewModel.getPartChildren().size() + " ");
				iter = viewModel.getPartChildren().iterator();
				while (iter.hasNext()) {
					displayText.write("\r\n" + iter.next());
				}

			}

			viewSourceText.setText(displayText.toString());
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	/**
	 * @return Returns the viewModel.
	 */
	public ViewModel getViewModel() {
		return viewModel;
	}

	/**
	 * @return Returns the modelConnector.
	 */
	public ModelConnector getModelConnector() {
		return modelConnector;
	}
}

class PathEditorInput implements IPathEditorInput {
	private IPath fPath;
	
	/**
	 * Creates an editor input based of the given file resource.
	 *
	 * @param path the file
	 */
	public PathEditorInput(IPath path) {
		if (path == null) {
			throw new IllegalArgumentException();
		}
		this.fPath = path;
	}
	
	/*
	 * @see java.lang.Object#hashCode()
	 */
	public int hashCode() {
		return fPath.hashCode();
	}
	
	/*
	 * @see java.lang.Object#equals(java.lang.Object)
	 */
	public boolean equals(Object obj) {
		if (this == obj)
			return true;
		if (!(obj instanceof PathEditorInput))
			return false;
		PathEditorInput other = (PathEditorInput) obj;
		return fPath.equals(other.fPath);
	}
	
	/*
	 * @see org.eclipse.ui.IEditorInput#exists()
	 */
	public boolean exists() {
		return fPath.toFile().exists();
	}
	
	/*
	 * @see org.eclipse.ui.IEditorInput#getImageDescriptor()
	 */
	public ImageDescriptor getImageDescriptor() {
		return PlatformUI.getWorkbench().getEditorRegistry().getImageDescriptor(fPath.toString());
	}
	
	/*
	 * @see org.eclipse.ui.IEditorInput#getName()
	 */
	public String getName() {
		return fPath.toString();
	}
	
	/*
	 * @see org.eclipse.ui.IEditorInput#getToolTipText()
	 */
	public String getToolTipText() {
		return fPath.makeRelative().toString();
	}
	
	/*
	 * @see org.eclipse.ui.IPathEditorInput#getPath()
	 */
	public IPath getPath() {
		return fPath;
	}

	/*
	 * @see org.eclipse.core.runtime.IAdaptable#getAdapter(java.lang.Class)
	 */
	public Object getAdapter(Class adapter) {
		return null;
	}

	/*
	 * @see org.eclipse.ui.IEditorInput#getPersistable()
	 */
	public IPersistableElement getPersistable() {
		// no persistence
		return null;
	}
}

