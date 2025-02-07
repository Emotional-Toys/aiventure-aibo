package org.tekkotsu.ui.editor.model;

/**
 * @author asangpet
 */

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

import org.eclipse.draw2d.ColorConstants;
import org.eclipse.draw2d.geometry.Rectangle;
import org.eclipse.jface.resource.ColorRegistry;
import org.eclipse.swt.graphics.Color;
import org.eclipse.swt.graphics.RGB;
import org.eclipse.ui.views.properties.ColorPropertyDescriptor;
import org.eclipse.ui.views.properties.ComboBoxPropertyDescriptor;
import org.eclipse.ui.views.properties.IPropertyDescriptor;
import org.eclipse.ui.views.properties.TextPropertyDescriptor;
import org.jdom.Content;
import org.jdom.Element;
import org.tekkotsu.ui.editor.resources.Debugger;
import org.tekkotsu.ui.editor.resources.IDTag;

public class StateNodeModel extends AbstractModel
{
    public static final String P_CLASS = "_class";

    public static final String P_COLOR = "_color";

    public static final String P_BLEND_COLOR = "_blend_color";

    public static final String P_CONSTRAINT = "_constraint";

    public static final String P_ID = "_id";

    public static final String P_INTRANS = "_intrans";

    public static final String P_OUTTRANS = "_outtrans";

    public static final String P_LABEL = "_label";

    public static final String P_SHAPE = "_shape";

    public static final String P_SHAPE_STYLE_ELLIPSE = "Ellipse";

    public static final String P_SHAPE_STYLE_RECTANGLE = "Rectangle";

    public static final String P_SHAPE_STYLE_ROUNDED_RECTANGLE = "Rounded Rectangle";

    public static final String P_SHAPE_STYLE_HEXAGON = "Hexagon";

    public static final String P_SOURCE_CONNECTION = "_source_connection";

    public static final String P_TARGET_CONNECTION = "_target_connection";

    public static final String P_ACTIVATE = "_state_activate";

    public static final String P_DEACTIVATE = "_state_deactivate";

    private static final String[] shapeValues = { P_SHAPE_STYLE_RECTANGLE,
            P_SHAPE_STYLE_ELLIPSE, P_SHAPE_STYLE_ROUNDED_RECTANGLE,
            P_SHAPE_STYLE_HEXAGON };

    public static int stateID = 1;

    private ColorRegistry colorRegistry = new ColorRegistry();

    private Rectangle constraint;

    private String id = "State";

    private String label = "State";

    private String shape = P_SHAPE_STYLE_RECTANGLE;

    private int shapeIndex = 0;

    private SourceObjectModel source;

    protected List sourceConnections = new ArrayList();

    protected List targetConnections = new ArrayList();

    public StateNodeModel()
    {
        super();
        Debugger.printDebug(Debugger.DEBUG_ALL, "execute StateNodeModel()");
        setId("state" + Integer.toString(stateID++));
        setLabel(getId());
    }

    public StateNodeModel(Element xml, ViewModel parent)
    {
        this();
        Debugger.printDebug(Debugger.DEBUG_ALL, "execute StateNodeModel(" + xml
                + "," + "parent)");
        if (IDTag.XML_state_tag.equals(xml.getName()))
        {
            initModelConfig(xml, parent);
        }
    }

    private static final Color[] colorArray = { ColorConstants.red,
            ColorConstants.blue, ColorConstants.orange, ColorConstants.green,              
            ColorConstants.cyan, ColorConstants.lightGreen };

    public StateNodeModel(SourceNodeModel source)
    {
        this();
        Debugger.printDebug(Debugger.DEBUG_ALL, "execute StateNodeModel("
                + source + ")");
        setId(source.getId());
        setLabel(source.getId());
        setSource(source);

        SourceModel parent = source.getParentModel();
        int numState = parent.getNodes().size();
        //java.awt.Color color =
        // java.awt.Color.getHSBColor((float)((1.0*stateID %
        // numState)/numState),.99f,.8f);
        Color color = colorArray[stateID % colorArray.length];
        RGB rgb = new RGB(color.getRed(), color.getGreen(), color.getBlue());
        colorRegistry.put("Color" + stateID, rgb);
        setColor(colorRegistry.getRGB("Color" + stateID));
        //setColor(ColorConstants.yellow.getRGB());
    }

    public void verifySource(SourceModel src)
    {
        Debugger.printDebug(Debugger.DEBUG_ALL, "Verify src:" + id + ":"
                + src.getNode(id));
        if (src.getChild(id) == null)
        {
            this.firePropertyChange(ViewModel.P_SOURCE_EXIST, null,
                    new Boolean(false));
        }
        else
        {
            this.firePropertyChange(ViewModel.P_SOURCE_EXIST, null,
                    new Boolean(true));
        }
    }

    public void doPostAdd()
    {
        ViewModel parent = (ViewModel) getParent();
        List children = new ArrayList();
        children.addAll(parent.getPartChildren());
        Iterator iter = children.iterator();
        while (iter.hasNext())
        {
            AbstractModel model = (AbstractModel) (iter.next());
            if (model instanceof MultiTransitionModel)
            {
                ((MultiTransitionModel) model).doPostAdd();
            }
        }
    }

    public boolean isPreview()
    {
        return ((ViewModel) getParent()).isPreview();
    }

    public void addSourceConnection(Object connx)
    {
        sourceConnections.add(connx);
        firePropertyChange(P_SOURCE_CONNECTION, null, null);
    }

    public void addTargetConnection(Object connx)
    {
        targetConnections.add(connx);
        firePropertyChange(P_TARGET_CONNECTION, null, null);
    }

    public Color getColor()
    {
        return colorRegistry.get(P_COLOR);
    }

    public Color getPreviewColor()
    {
        return colorRegistry.get(P_BLEND_COLOR);
    }

    public Rectangle getConstraint()
    {
        return constraint;
    }

    /**
     * @return Returns the id.
     */
    public String getId()
    {
        return id;
    }

    public String getLabel()
    {
        return label;
    }

    public List getModelSourceConnections()
    {
        return sourceConnections;
    }

    public List getModelTargetConnections()
    {
        return targetConnections;
    }

    public IPropertyDescriptor[] getPropertyDescriptors()
    {
        TextPropertyDescriptor idProp = new TextPropertyDescriptor(P_ID, "ID");
        TextPropertyDescriptor classProp = new TextPropertyDescriptor(P_CLASS,
                "Class");
        TextPropertyDescriptor intransProp = new TextPropertyDescriptor(
                P_INTRANS, "InTrans");
        TextPropertyDescriptor outtransProp = new TextPropertyDescriptor(
                P_OUTTRANS, "OutTrans");

        String modelCat = "Info";
        idProp.setCategory(modelCat);
        classProp.setCategory(modelCat);
        intransProp.setCategory(modelCat);
        outtransProp.setCategory(modelCat);

        IPropertyDescriptor[] descriptors = new IPropertyDescriptor[] { idProp,
                classProp, intransProp, outtransProp,
                new TextPropertyDescriptor(P_LABEL, "Label"),
                new ColorPropertyDescriptor(P_COLOR, "Color"),
                new ComboBoxPropertyDescriptor(P_SHAPE, "Shape", shapeValues) };
        return descriptors;
    }

    public Object getPropertyValue(Object id)
    {
        if (id.equals(P_ID))
        {
            return getId();
        }
        else if (id.equals(P_CLASS))
        {
            if (getSource() != null) return getSource().getClassName();
        }
        else if (id.equals(P_LABEL))
        {
            return getLabel();
        }
        else if (id.equals(P_COLOR))
        {
            return getRGB();
        }
        else if (id.equals(P_SHAPE))
        {
            return new Integer(shapeIndex);
        }
        else if (id.equals(P_OUTTRANS))
        {
            return sourceConnections.toString();
        }
        else if (id.equals(P_INTRANS)) { return targetConnections.toString(); }
        return null;
    }

    public RGB getRGB()
    {
        return colorRegistry.getRGB(P_COLOR);
    }

    /**
     * @return Returns the shape.
     */
    public String getShape()
    {
        return shape;
    }

    /**
     * @return Returns the source.
     */
    public SourceObjectModel getSource()
    {
        return source;
    }

    public Content getXML()
    {
        Element content = new Element(IDTag.XML_state_tag);
        content.setAttribute(IDTag.XML_state_id, getId());
        //content.setAttribute(IDTag.XML_state_class,getSource().getClassName());
        content.setAttribute(IDTag.XML_state_label, getLabel());
        content.setAttribute(IDTag.XML_state_color, IDTag.RGB2Tag(getRGB()));
        Rectangle rect = getConstraint();
        content.setAttribute(IDTag.XML_state_top, "" + rect.y);
        content.setAttribute(IDTag.XML_state_left, "" + rect.x);
        content.setAttribute(IDTag.XML_state_width, "" + rect.width);
        content.setAttribute(IDTag.XML_state_height, "" + rect.height);
        content.setAttribute(IDTag.XML_state_shape, getShape());

        return content;
    }

    protected void initModelConfig(Element xml, ViewModel parent)
    {
        String idTag = xml.getAttributeValue(IDTag.XML_common_id);
        if (!((idTag == null) || (idTag.equals("")))) setId(idTag);
        setSource(parent.getSourceModel().getChild(idTag));

        if (xml.getAttributeValue(IDTag.XML_common_label) == null)
            setLabel(getId());
        else
            setLabel(xml.getAttributeValue(IDTag.XML_common_label));

        RGB color = IDTag
                .Tag2RGB(xml.getAttributeValue(IDTag.XML_common_color));
        setColor(color);
        try
        {
            int y = xml.getAttribute(IDTag.XML_common_top).getIntValue();
            int x = xml.getAttribute(IDTag.XML_common_left).getIntValue();
            int width = xml.getAttribute(IDTag.XML_common_width).getIntValue();
            int height = xml.getAttribute(IDTag.XML_common_height)
                    .getIntValue();

            constraint = new Rectangle(x, y, width, height);
            setConstraint(constraint);
        }
        catch (Exception e)
        {
            e.printStackTrace();
        }
        try
        {
            setShape(xml.getAttributeValue(IDTag.XML_state_shape));
        }
        catch (Exception e)
        {
        }
    }

    public boolean isPropertySet(Object id)
    {
        if (id.equals(P_LABEL) || id.equals(P_COLOR) || id.equals(P_SHAPE))
                return true;
        return false;
    }

    public void removeSourceConnection(Object connx)
    {
        sourceConnections.remove(connx);
        firePropertyChange(P_SOURCE_CONNECTION, null, null);
    }

    public void removeTargetConnection(Object connx)
    {
        targetConnections.remove(connx);
        firePropertyChange(P_TARGET_CONNECTION, null, null);
    }

    private int getBlendColor(int oldValue)
    {
        int newCol = (int) Math.round((double) oldValue * 0.2) + 200;
        return (newCol > 255) ? 255 : newCol;
    }

    public void setColor(RGB color)
    {
        colorRegistry.put(P_COLOR, color);
        colorRegistry.put(P_BLEND_COLOR, new RGB(getBlendColor(color.red),
                getBlendColor(color.green), getBlendColor(color.blue)));
        firePropertyChange(P_COLOR, null, getColor());
    }

    public void setConstraint(Rectangle rect)
    {
        constraint = rect;
        firePropertyChange(P_CONSTRAINT, null, constraint);
    }

    public void setId(String text)
    {
        this.id = text;
    }

    public void setLabel(String text)
    {
        this.label = text;
        firePropertyChange(P_LABEL, null, text);
    }

    public void setPropertyValue(Object id, Object value)
    {
        if (id.equals(P_ID))
        {
            setId((String) value);
        }
        else if (id.equals(P_CLASS))
        {
            //setClassName((String)value);
        }
        else if (id.equals(P_LABEL))
        {
            setLabel((String) value);
        }
        else if (id.equals(P_COLOR))
        {
            setColor((RGB) value);
            Debugger
                    .printDebug(Debugger.DEBUG_ALL, value.getClass().toString());
        }
        else if (id.equals(P_SHAPE))
        {
            setShapeIndex(((Integer) value).intValue());
        }
    }

    /**
     * @param shape
     *            The shape to set.
     */
    public void setShape(String shape)
    {
        // verify shape style
        if (!shape.equals(shapeValues[shapeIndex]))
        {
            for (int i = 0; i < shapeValues.length; i++)
                if (shapeValues[i].equals(shape))
                {
                    shapeIndex = i;
                    break;
                }
        }
        if (!shape.equals(shapeValues[shapeIndex])) return;

        this.shape = shape;
        firePropertyChange(P_SHAPE, null, shape);
    }

    public void setShapeIndex(int index)
    {
        shapeIndex = index;
        setShape(shapeValues[index]);
    }

    /**
     * @param source
     *            The source to set.
     */
    public void setSource(SourceObjectModel source)
    {
        this.source = source;
    }

    public String toString()
    {
        //return getClass().getSimpleName() + ":" + this.getId();
        return this.getId();
    }

    public void activate()
    {
        this.firePropertyChange(StateNodeModel.P_ACTIVATE, null, null);
    }

    public void deactivate()
    {
        this.firePropertyChange(StateNodeModel.P_DEACTIVATE, null, null);
    }

}