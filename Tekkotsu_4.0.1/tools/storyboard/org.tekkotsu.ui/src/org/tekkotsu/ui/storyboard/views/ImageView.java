package org.tekkotsu.ui.storyboard.views;

import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;
import java.text.MessageFormat;
import java.util.Iterator;
import java.util.List;

import org.eclipse.swt.SWT;
import org.eclipse.swt.graphics.Device;
import org.eclipse.swt.graphics.Image;
import org.eclipse.swt.graphics.ImageData;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.FileDialog;
import org.eclipse.swt.widgets.Shell;
import org.eclipse.core.runtime.IPath;
import org.eclipse.core.runtime.Path;
import org.eclipse.jface.action.Action;
import org.eclipse.jface.action.IMenuListener;
import org.eclipse.jface.action.IMenuManager;
import org.eclipse.jface.action.IToolBarManager;
import org.eclipse.jface.action.MenuManager;
import org.eclipse.jface.action.Separator;
import org.eclipse.jface.dialogs.MessageDialog;
import org.eclipse.jface.resource.ImageDescriptor;
import org.eclipse.jface.viewers.DoubleClickEvent;
import org.eclipse.jface.viewers.IDoubleClickListener;
import org.eclipse.jface.viewers.IStructuredContentProvider;
import org.eclipse.jface.viewers.Viewer;
import org.eclipse.swt.widgets.Menu;
import org.eclipse.ui.IActionBars;
import org.eclipse.ui.IWorkbenchActionConstants;
import org.eclipse.ui.part.ViewPart;
import org.tekkotsu.ui.editor.resources.Debugger;
import org.tekkotsu.ui.storyboard.ResourceRegistry;
import org.tekkotsu.ui.storyboard.components.ImageDialog;
import org.tekkotsu.ui.storyboard.model.ImageModel;

public class ImageView extends ViewPart implements PropertyChangeListener {
    ImageViewer viewer;

    Action doubleClickAction, saveAction;

    class ViewContentProvider implements IStructuredContentProvider {
        ImageModel model = null;

        public Object[] getElements(Object inputElement) {
            return null;
        }

        public void inputChanged(Viewer v, Object oldInput, Object newInput) {
        }

        public void dispose() {
        }

        public void setContent(ImageModel img) {
            model = img;
        }

        public ImageModel getDefaultModel() {
            return model;
        }
    }

    private ViewContentProvider content;

    /**
     * The constructor.
     */
    public ImageView() {
    }

    /**
     * This is a callback that will allow us to create the viewer and initialize
     * it.
     */
    public void createPartControl(Composite parent) {
        //drillDownAdapter = new DrillDownAdapter(viewer);
        viewer = new ImageViewer(parent);
        content = new ViewContentProvider();
        makeActions();
        hookContextMenu();
        hookDoubleClickAction();
        contributeToActionBars();
    }

    private void hookContextMenu() {
        MenuManager menuMgr = new MenuManager("#ImagePopupMenu");
        menuMgr.setRemoveAllWhenShown(true);
        menuMgr.addMenuListener(new IMenuListener() {
            public void menuAboutToShow(IMenuManager manager) {
                ImageView.this.fillContextMenu(manager);
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
    }

    private void fillContextMenu(IMenuManager manager) {
        manager.add(saveAction);
        manager.add(new Separator(IWorkbenchActionConstants.MB_ADDITIONS));
    }

    private void fillLocalToolBar(IToolBarManager manager) {
        manager.add(saveAction);
    }

    private void makeActions() {
        saveAction = new Action() {
            private IPath queryFile() {
                FileDialog dialog = new FileDialog(viewer.getControl()
                        .getShell(), SWT.SAVE);
                dialog.setFilterExtensions(new String[] { "*.jpg" });
                dialog.setText("Save Image as"); //$NON-NLS-1$
                String path = dialog.open();
                if (path != null && path.length() > 0) {
                    if (!path.endsWith(".jpg"))
                        path = path + ".jpg";
                    return new Path(path);
                }
                return null;
            }

            public void run() {
                if (viewer.getImageModel() == null) {
                    MessageDialog.openWarning(viewer.getControl().getShell(),
                            "Error", "No image has been selected");
                    return;
                }
                IPath path = queryFile();
                boolean success = false;
                if (path != null) {
                    success = viewer.getImageModel().saveTo(path);
                    if (!success) {
                        String msg = MessageFormat
                                .format("Cannot write file: {0}",
                                        new String[] { path.toOSString() });
                        MessageDialog.openWarning(viewer.getControl()
                                .getShell(), "Problem", msg);
                    }
                }
            }
        };
        saveAction.setText("Save Image As");
        saveAction.setToolTipText("Save Image As");
        saveAction.setImageDescriptor(ImageDescriptor.createFromFile(
                org.tekkotsu.ui.storyboard.icons.IconDummy.class, "save.gif"));

        doubleClickAction = new Action() {
            public void run() {
                ImageData data = viewer.getImageModel().getImageData();
                Shell shell = ResourceRegistry.getInstance().getDisplay()
                        .getActiveShell();
                Device device = ResourceRegistry.getInstance().getDisplay();
                ImageDialog imgDialog = new ImageDialog(shell, new Image(
                        device, data));
                imgDialog.setTitle("Image: "
                        + viewer.getImageModel().getImageName());
                imgDialog.open();
            }
        };
    }

    private void hookDoubleClickAction() {
        viewer.addDoubleClickListener(new IDoubleClickListener() {
            public void doubleClick(DoubleClickEvent e) {
                doubleClickAction.run();
            }
        });

    }

    private void showMessage(String message) {
        MessageDialog.openInformation(viewer.getControl().getShell(),
                "Image Preview", message);
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
        if (evt.getPropertyName().equals("_BLOCK_INFO")) {
            List list = (List) evt.getNewValue();
            if (list != null) {
                ImageModel img = null;
                for (Iterator iter = list.iterator(); iter.hasNext();) {
                    Object next = iter.next();
                    if (next instanceof ImageModel) {
                        img = (ImageModel) next;
                        break;
                    }
                }
                if (img != null) {
                    content.setContent(img);

                    Debugger.printDebug(Debugger.DEBUG_ALL, "PROC "
                            + evt.getNewValue());
                    viewer.getControl().getDisplay().syncExec(new Runnable() {
                        public void run() {
                            viewer.setContent(content);
                        }
                    });
                }
            }
        }

    }
}