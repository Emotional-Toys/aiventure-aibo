package org.tekkotsu.mon;

import javax.swing.*;
import javax.swing.event.*;
import javax.swing.table.*;
import java.awt.*;
import java.awt.event.*;
import java.util.*;
import java.util.prefs.Preferences;

public class WatchableMemory {
  Hashtable registries;
  WatchableMemoryListener listener;
  WMregistry globalWM;
  WMController wmController;
	listenThread runner;

  public static void main(String args[]) {
    if (args.length>1) {
      System.out.println("usage: java WatchableMemory [host]");
      System.exit(1);
    }

    try {
      DogConfig dogConfig=new DogConfig (args);
      WatchableMemory watchableMemory=new WatchableMemory(dogConfig.getIP(),
        Integer.parseInt(dogConfig.getValue("Main", "wmmonitor_port")),
        true);
    } catch (IllegalArgumentException ex) {}
  }

  public WatchableMemory(String ip, int port, String[] args) {
    this(ip, port, false);
  }

  public WatchableMemory(String ip, int port) {
    this(ip, port, false);
  }

  public WatchableMemory(String ip, int port, boolean appmode) {
    listener=new WatchableMemoryListener(ip, port);
    registries=new Hashtable();
    wmController=new WMController(listener);
    globalWM=new WMregistry("", wmController, registries);

    if (appmode) {
      globalWM.addWindowListener(new WindowAdapter() {
        public void windowClosing(WindowEvent e) { System.exit(0); } });
    }
		runner=new listenThread(this,listener);
		runner.start();
  }


	class listenThread extends Thread {
		WatchableMemory gui;
		WatchableMemoryListener listener;
		listenThread(WatchableMemory gui,WatchableMemoryListener listener) { this.gui=gui; this.listener=listener;}
		public void run() { 
			listener.needConnection();
			try {
				while (true) {
					if (listener.isConnected()) {
						if (listener.hasData()) {
							WMvar wmvar=listener.getData();
							String registry=wmvar.getRegistry();
							if (registry==null) {
								gui.globalWM.registerVar(wmvar);
							} else {
								WMregistry wmregistry;
								synchronized (registries) {
									wmregistry=(WMregistry)registries.get(registry);
									if (wmregistry==null) {
										wmregistry=new WMregistry(registry, gui.wmController, registries);
										registries.put(registry, wmregistry);
									}
								}
								wmregistry.registerVar(wmvar);
							}
						} else { sleep(10); }
					} else { sleep(100); }
				}
			} catch(Exception ex) { if((InterruptedException)ex==null) ex.printStackTrace(); }
		} 
	}

	public void close() {
		globalWM.dispose();
		listener.kill();
		runner.interrupt();
		runner=null;
	}

  void sleep(long ms) {
    try { Thread.sleep(ms); } catch (Exception e) {}
  }
}

class WMController {
  WatchableMemoryListener listener;

  public WMController(WatchableMemoryListener listener) {
    this.listener=listener;
  }

  public boolean update (WMvar wmvar) {
    if (wmvar.watched) return watch (wmvar);
    else return supress (wmvar);
  }

  public boolean watch (WMvar wmvar) {
    return watch(wmvar.getCompleteName());
  }

  public boolean watch (String s) {
    if (!listener.isConnected()) return false;
    listener.write("w "+s+"\n");
    return true;
  }

  public boolean supress (WMvar wmvar) {
    return supress(wmvar.getCompleteName());
  }

  public boolean supress (String s) {
    if (!listener.isConnected()) return false;
    listener.write("s "+s+"\n");
    return true;
  }

  public boolean examine (WMvar wmvar) {
    return examine(wmvar.getCompleteName());
  }

  public boolean examine (String s) {
    if (!listener.isConnected()) return false;
    listener.write("x "+s+"\n");
    return true;
  }

  public boolean dumpRegistry (String s) {
    if (!listener.isConnected()) return false;
    listener.write("r "+s+"\n");
    return true;
  }
}

class RegistryTableModel extends AbstractTableModel {
  final String[] columnNames = { "watch",
                                 "name",
                                 "type",
                                 "value" };
  Vector wmvars;
  WMController wmController;

  public RegistryTableModel(Vector wmvars, WMController wmController) {
    super();
    this.wmvars=wmvars;
    this.wmController=wmController;
  }

  public int getColumnCount() {
    return columnNames.length;
  }

  public int getRowCount() {
    return wmvars.size();
  }

  public String getColumnName(int col) {
    return columnNames[col];
  }

  public Object getValueAt(int row, int col) {
    WMvar wmvar=(WMvar)wmvars.get(row);
    switch (col) {
      case 0:
        return new Boolean(wmvar.watched);
      case 1:
        return wmvar.getName();
      case 2:
        return wmvar.getType();
      case 3:
        return wmvar.getValue();
      default:
        return null;
    }
  }

  public Class getColumnClass(int c) {
    switch (c) {
      case 0:
        return (new Boolean(true)).getClass();
      default:
        return (new String()).getClass();
    }
  }

  public boolean isCellEditable(int row, int col) {
    if (col==0) return true;
    return false;
  }

  public void setValueAt(Object value, int row, int col) {
    WMvar wmvar=(WMvar)wmvars.get(row);
    boolean val=((Boolean)value).booleanValue();
    if (val) {
      if (wmController.watch(wmvar))
        wmvar.watched=val;
    } else {
      if (wmController.supress(wmvar))
        wmvar.watched=val;
    }
  }

  public void varUpdated(int i) {
    fireTableCellUpdated(i, 3);
  }

  public void varAdded(int i) {
    fireTableRowsInserted(i, i);
  }
}

class WMregistry extends JFrame implements ActionListener, MouseListener {
  RegistryTableModel dataModel;
  String name;

  Vector wmvars_vec;
  Hashtable wmvars_hash;
  WMController wmController;
  Hashtable registries;
  
  JTable table;
  TableSorter sorter;
  JScrollPane scrollPane;
  JButton refresh;

	static Preferences prefs = Preferences.userNodeForPackage(WatchableMemory.class);

  public WMregistry(String s, WMController wmController, Hashtable registries) {
    super(s);
    name=s;
    this.wmController=wmController;

    addWindowListener(new WindowAdapter() {
      public void windowClosing(WindowEvent e) { e.getWindow().setVisible(false); } });

    wmvars_vec=new Vector();
    wmvars_hash=new Hashtable();

    this.registries=registries;
    wmController.dumpRegistry(name);
    dataModel=new RegistryTableModel(wmvars_vec, wmController);

    sorter=new TableSorter(dataModel, 1);
    table = new JTable(sorter);
    sorter.addMouseListenerToHeaderInTable(table);

    setTableSizes();
    scrollPane = new JScrollPane(table,
                                 JScrollPane.VERTICAL_SCROLLBAR_ALWAYS,
                                 JScrollPane.HORIZONTAL_SCROLLBAR_NEVER);
    table.addMouseListener(this);

    refresh=new JButton("refresh");
    refresh.addActionListener(this);

    getContentPane().add(scrollPane, BorderLayout.CENTER);
    getContentPane().add(refresh, BorderLayout.SOUTH);

    setLocation(prefs.getInt("WatchableMemory."+s+".location.x",50),prefs.getInt("WatchableMemory.location.y",50));
		addWindowListener(new CloseAdapter(this));

    setVisible(true);
  }

	class CloseAdapter extends WindowAdapter {
		WMregistry gui;
		CloseAdapter(WMregistry gui) {this.gui=gui;}
		public void windowClosing(WindowEvent e) {
			gui.prefs.putInt("WatchableMemory."+gui.name+".location.x",gui.getLocation().x);
			gui.prefs.putInt("WatchableMemory."+gui.name+".location.y",gui.getLocation().y);
			//I think I had needed to add getInsets() to keep the window from moving when reopening it, but now it *causes* it to move... weird.  what changed?
			//gui.prefs.putInt("WatchableMemory."+gui.name+".location.x",gui.getLocation().x+gui.getInsets().left);
			//gui.prefs.putInt("WatchableMemory."+gui.name+".location.y",gui.getLocation().y+gui.getInsets().top);
		}
	}

  public void setTableSizes() {
    setSize(new Dimension(500, 530));
    table.setPreferredScrollableViewportSize(new Dimension(500, 500));
    TableColumn column;
    column=table.getColumnModel().getColumn(0);
    column.setPreferredWidth(30);
    column=table.getColumnModel().getColumn(1);
    column.setPreferredWidth(100);
    column=table.getColumnModel().getColumn(2);
    column.setPreferredWidth(70);
    column=table.getColumnModel().getColumn(3);
    column.setPreferredWidth(200);
  }
  
  public void registerVar(WMvar wmvar) {
    // use hashtable to make updates faster!

    for (int i=0; i<wmvars_vec.size(); i++) {
      WMvar wmvar_i=(WMvar)wmvars_vec.get(i);
      int cmp=wmvar_i.getName().compareTo(wmvar.getName());
      if (cmp==0) {
        wmvar_i.update(wmvar);
        dataModel.varUpdated(i);
        return;
      }
    }
    wmvars_vec.add(wmvar);
    dataModel.varAdded(wmvars_vec.size()-1);
  }

  public void actionPerformed(ActionEvent e) {
    if (e.getSource()==refresh)
      wmController.dumpRegistry(name);
  }

  public void mouseClicked(MouseEvent e) {
    if (e.getClickCount()==2) {
      int col=table.getSelectedColumn();
      if (col==1 || col==2)
        openItem(sorter.translateRow(table.getSelectedRow()));
    }
  }

  void openItem(int row) {
    WMvar wmvar=(WMvar)wmvars_vec.get(row);
    if (wmvar.getType().equals("WMregistry")) {
      String registry=wmvar.getCompleteName();
      synchronized (registries) {
        WMregistry wmregistry=(WMregistry)registries.get(registry);
        if (wmregistry==null) {
          wmregistry=new WMregistry(registry, wmController, registries);
          registries.put(registry, wmregistry);
        } else {
          wmregistry.setVisible(true);
        }
      }
    } else {
    }
  }

  public void mouseEntered(MouseEvent e) { }
  public void mouseExited(MouseEvent e) { }
  public void mousePressed(MouseEvent e) { }
  public void mouseReleased(MouseEvent e) { }
}
