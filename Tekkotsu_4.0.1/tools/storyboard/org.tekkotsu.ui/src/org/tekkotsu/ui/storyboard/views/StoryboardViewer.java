/*
 * Created on Nov 7, 2004
 */
package org.tekkotsu.ui.storyboard.views;

import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;
import java.beans.PropertyChangeSupport;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

import org.eclipse.draw2d.ColorConstants;
import org.eclipse.draw2d.FigureCanvas;
import org.eclipse.draw2d.LightweightSystem;
import org.eclipse.draw2d.Panel;
import org.eclipse.draw2d.XYLayout;
import org.eclipse.draw2d.geometry.Rectangle;
import org.eclipse.jface.viewers.ISelection;
import org.eclipse.jface.viewers.StructuredSelection;
import org.eclipse.jface.viewers.Viewer;
import org.eclipse.swt.events.SelectionAdapter;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Control;
import org.eclipse.swt.widgets.Display;
import org.tekkotsu.ui.editor.model.MultiTransitionModel;
import org.tekkotsu.ui.editor.model.StateNodeModel;
import org.tekkotsu.ui.editor.model.ViewModel;
import org.tekkotsu.ui.editor.resources.Debugger;
import org.tekkotsu.ui.storyboard.ResourceRegistry;
import org.tekkotsu.ui.storyboard.components.Block;
import org.tekkotsu.ui.storyboard.components.ImageObject;
import org.tekkotsu.ui.storyboard.components.LogBar;
import org.tekkotsu.ui.storyboard.components.LogObject;
import org.tekkotsu.ui.storyboard.components.Storyboard;
import org.tekkotsu.ui.storyboard.components.TimeLine;
import org.tekkotsu.ui.storyboard.components.Transition;
import org.tekkotsu.ui.storyboard.model.AbstractModel;
import org.tekkotsu.ui.storyboard.model.BlockModel;
import org.tekkotsu.ui.storyboard.model.ImageModel;
import org.tekkotsu.ui.storyboard.model.LogModel;
import org.tekkotsu.ui.storyboard.model.TransitionModel;

/**
 * @author asangpet
 *  
 */
public class StoryboardViewer extends Viewer {
    Composite parent;

    private PropertyChangeSupport listeners = new PropertyChangeSupport(this);

    private ArrayList listenobjs = new ArrayList();

    int timeLimit = TimeLine.TIMELINE_INIT_LIMIT;

    StoryboardView host;

    LightweightSystem lws;

    Composite control = null;

    FigureCanvas canvas = null;

    StoryboardPanel panel = null;

    TimeLine timeline;

    LogBar logBar;

    Storyboard storyboard;

    ViewModel viewModel;

    StoryboardView.ContentProvider content;

    ISelection curSelect = null;

    XYLayout contentsLayout = null;

    Display display;

    class StoryboardPanel extends Panel implements PropertyChangeListener {
        public void setBounds(Rectangle rect) {
            Rectangle old = getBounds();
            super.setBounds(rect);
            firePropertyChange(Storyboard.PROP_DIM_CHANGE, null, rect);
        }

        public void propertyChange(PropertyChangeEvent evt) {
            if (TimeLine.TIMELINE_CURSOR_CHANGED.equals(evt.getPropertyName())) {
                //System.out.println(evt.getNewValue().getClass()+" :
                // "+evt.getNewValue());
                /*
                 * Integer value = (Integer)evt.getNewValue(); final int tcursor =
                 * TimeLine.xoffset + value.intValue() * TimeLine.halfgap;
                 * 
                 * System.out.println("scroll to "+tcursor); final FigureCanvas
                 * parent = (FigureCanvas)getParent(); display.syncExec(new
                 * Runnable() { public void run() {
                 * parent.scrollToX(tcursor+500); parent.update(); } });
                 */
            }
        }
    }

    public StoryboardViewer(Composite parent, StoryboardView hostView) {
        this.host = hostView;
        this.parent = parent;

        canvas = new FigureCanvas(parent);

        display = canvas.getDisplay();
        ResourceRegistry.init(display);

        panel = new StoryboardPanel();
        panel.setBackgroundColor(ColorConstants.white);
        canvas.setContents(panel);

        contentsLayout = new XYLayout();
        panel.setLayoutManager(contentsLayout);

        timeline = new TimeLine();
        panel.add(timeline);
        timeline.setLimit(TimeLine.TIMELINE_INIT_LIMIT);

        canvas.getVerticalBar().addSelectionListener(new SelectionAdapter() {
            public void widgetSelected(SelectionEvent event) {
                panel.remove(timeline);
                panel.add(timeline);
                timeline.setVerticalOffset(canvas.getVerticalBar()
                        .getSelection());

                panel.remove(logBar);
                panel.add(logBar);
                logBar
                        .setVerticalOffset(canvas.getVerticalBar()
                                .getSelection());
            }
        });

        storyboard = new Storyboard(this, timeline);
        contentsLayout.setConstraint(storyboard, new Rectangle(0,
                TimeLine.height + LogBar.height, timeline
                        .getOffset(timeLimit), 200));
        storyboard.setLayoutManager(new XYLayout());
        panel.add(storyboard);

        logBar = new LogBar(timeline);
        logBar.setLayoutManager(new XYLayout());
        panel.add(logBar);
        logBar.setLimit(TimeLine.TIMELINE_INIT_LIMIT);

        timeline.addPropertyChangeListener(storyboard);
        timeline.addPropertyChangeListener(panel);
        timeline.addPropertyChangeListener(logBar);

        logBar.addPropertyChangeListener(storyboard);
        logBar.addPropertyChangeListener(panel);
        logBar.addPropertyChangeListener(timeline);

        storyboard.addPropertyChangeListener(timeline);
        storyboard.addPropertyChangeListener(logBar);

        panel.addPropertyChangeListener(storyboard);
    }

    /*
     * (non-Javadoc)
     * 
     * @see org.eclipse.jface.viewers.Viewer#getControl()
     */
    public Control getControl() {
        return canvas;
    }

    public void setContent(StoryboardView.ContentProvider content) {
        this.content = content;
    }

    /*
     * (non-Javadoc)
     * 
     * @see org.eclipse.jface.viewers.IInputProvider#getInput()
     */
    public Object getInput() {
        return null;
    }

    /*
     * (non-Javadoc)
     * 
     * @see org.eclipse.jface.viewers.ISelectionProvider#getSelection()
     */
    public ISelection getSelection() {
        Debugger.printDebug(Debugger.DEBUG_ALL, "getting selection");
        return curSelect;
    }

    /*
     * (non-Javadoc)
     * 
     * @see org.eclipse.jface.viewers.Viewer#refresh()
     */
    public void refresh() {
        //storyboard.repaint();
    }

    /*
     * (non-Javadoc)
     * 
     * @see org.eclipse.jface.viewers.Viewer#setInput(java.lang.Object)
     */
    public void setInput(Object input) {
        setInput(input, true);
    }

    public TimeLine getTimeLine() {
        return timeline;
    }

    private void update(final Object newmodel) {
        if (newmodel instanceof ImageModel) {
            display.syncExec(new Runnable() {
                public void run() {
                    ImageModel obj = (ImageModel) newmodel;
                    if (obj.getStart() >= timeline.getLimit() - 1) {
                        timeline.setLimit(obj.getStart() + 100000);
                        logBar.setLimit(timeline.getLimit());
                        storyboard.setLimit(timeline.getLimit());
                    }
                    StateNodeModel state = (StateNodeModel) viewModel
                            .getPartChild(obj.getID());
                    // user in-state image object
                    ImageObject img = new ImageObject(obj, state, timeline);
                    storyboard.addChild(img);
                    storyboard.getLayoutManager().setConstraint(img,
                            img.getConstraint());
                }
            });
        }
        else if (newmodel instanceof LogModel) {
            display.syncExec(new Runnable() {
                public void run() {
                    LogModel obj = (LogModel) newmodel;
                    if (obj.getStart() >= timeline.getLimit() - 1) {
                        timeline.setLimit(obj.getStart() + 100000);
                        logBar.setLimit(timeline.getLimit());
                        storyboard.setLimit(timeline.getLimit());
                    }
                    StateNodeModel state = (StateNodeModel) viewModel
                            .getPartChild(obj.getID());
                    if (state != null) {
                        // user in-state log object
                        LogObject log = new LogObject(obj, state, timeline);
                        storyboard.addChild(log);
                        storyboard.getLayoutManager().setConstraint(log,
                                log.getConstraint());
                    } else {
                        // system log object
                        LogObject log = new LogObject(obj, null, timeline);
                        logBar.addChild(log);
                        logBar.getLayoutManager().setConstraint(log,
                                log.getConstraint());
                    }
                }
            });
        } else if (newmodel instanceof TransitionModel) {
            display.syncExec(new Runnable() {
                public void run() {
                    TransitionModel obj = (TransitionModel) newmodel;
                    if (obj.getStart() >= timeline.getLimit() - 1) {
                        timeline.setLimit(obj.getStart() + 100000);
                        logBar.setLimit(timeline.getLimit());
                        storyboard.setLimit(timeline.getLimit());
                    }
                    Object editor_obj = viewModel.getPartChild(obj.getID());
                    if (editor_obj instanceof MultiTransitionModel) {
                        MultiTransitionModel trans = (MultiTransitionModel) editor_obj;
                        Transition tr = new Transition(display, obj, trans,
                                timeline);
                        storyboard.addChild(tr);
                        storyboard.getLayoutManager().setConstraint(tr,
                                tr.getConstraint());
                    }
                }
            });
        } else if (newmodel instanceof BlockModel) {
            display.syncExec(new Runnable() {
                public void run() {
                    BlockModel obj = (BlockModel) newmodel;
                    // extends timeline limit for longer state
                    if (obj.getEnd() >= timeline.getLimit() - 1) {
                        timeline.setLimit(obj.getEnd() + 100000);
                        logBar.setLimit(timeline.getLimit());
                        storyboard.setLimit(timeline.getLimit());
                    }
                    StateNodeModel state = (StateNodeModel) viewModel
                            .getPartChild(obj.getID());
                    Block b = new Block(display, storyboard, obj, state,
                            timeline);

                    storyboard.addChild(b);
                    storyboard.getLayoutManager().setConstraint(b,
                            b.getConstraint());
                }
            });
        }
    }

    public void setInput(Object input, boolean rebuild) {
        Debugger.printDebug(Debugger.DEBUG_ALL, "Set input to model:" + input);
        viewModel = (ViewModel) input;

        if (rebuild) {
            // redraw all elements
            display.syncExec(new Runnable() {
                public void run() {
                    timeline.getMarker().clearMarker();
                    timeline.repaint();
                    storyboard.removeAll();
                    logBar.removeAll();
                }
            });

            Iterator iter = content.getElements().iterator();
            while (iter.hasNext()) {
                final Object next = iter.next();
                update(next);
            }
        } else {
            List elements = content.getUpdate();
            //System.out.println("Update size "+elements.size()+" children
            // "+storyboard.getChildren().size());

            Iterator iter = elements.iterator();
            while (iter.hasNext()) {
                final Object next = iter.next();
                update(next);
            }
        }

        display.syncExec(new Runnable() {
            public void run() {
                int value = content.latestTime;
                timeline.setValue(value);
                storyboard.setValue(value);
                logBar.setValue(value);
                storyboard.validate();
            }
        });
    }

    /*
     * (non-Javadoc)
     * 
     * @see org.eclipse.jface.viewers.Viewer#setSelection(org.eclipse.jface.viewers.ISelection,
     *      boolean)
     */
    public void setSelection(ISelection selection, boolean reveal) {
        Control control = getControl();
        if (control == null || control.isDisposed()) {
            return;
        }

        if (selection != null) {
            if (selection instanceof StructuredSelection) {
                StructuredSelection sselect = (StructuredSelection) selection;
                Debugger.printDebug(Debugger.DEBUG_ALL, "Selection List:"
                        + sselect.size());
                Iterator iter = sselect.toList().iterator();
                ArrayList nodes = new ArrayList();
                ArrayList objnodes = new ArrayList();
                // objnodes = integer node indicate current time + selected node
                objnodes.add(new Integer(timeline.getValue()));
                while (iter.hasNext()) {
                    Object next = iter.next();
                    if (next instanceof AbstractModel) {
                        nodes.add(((AbstractModel) next).getID());
                        objnodes.add(next);
                    }
                }
                Debugger.printDebug(Debugger.DEBUG_ALL, "selections:" + nodes);

                curSelect = sselect;
                // SelectionChangedEvent evt = new
                // SelectionChangedEvent(this,selection);
                // fireSelectionChanged(evt);
                this.firePropertyChange(ViewModel.P_ACTIVATE_CHANGE, null,
                        nodes);
                firePropertyChange("_BLOCK_INFO", null, objnodes);
                // fireSelectionChanged(new SelectionChangedEvent(this,
                // sselect));
            }
        }
    }

    public void addPropertyChangeListener(PropertyChangeListener listener) {
        listeners.addPropertyChangeListener(listener);
        listenobjs.add(listener);
    }

    public void firePropertyChange(String propName, Object oldValue,
            Object newValue) {
        listeners.firePropertyChange(propName, oldValue, newValue);
    }

    public void removePropertyChangeListener(PropertyChangeListener listener) {
        listeners.removePropertyChangeListener(listener);
        listenobjs.remove(listener);
    }

}
