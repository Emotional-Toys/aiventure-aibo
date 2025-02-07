package org.tekkotsu.mon;

import java.io.*;
import java.net.*;
import java.util.*;

public class WatchableMemoryListener extends TCPListener {
  LinkedList changesList;
  PrintStream _out;

  public void connected(Socket socket) {
    changesList=new LinkedList();
    _isConnected=true;
    try {
      InputStream in=socket.getInputStream();
      _out = new PrintStream(socket.getOutputStream());
      fireConnected();
      while (true) {
        int format=readInt(in);
        if (format!=PACKET_WMCLASS)
          throw new java.lang.NoSuchFieldException("fake exception");
        int timestamp=readInt(in);

        String var_type=new String(readBytes(in, readInt(in)));
        String var_name=new String(readBytes(in, readInt(in)));
        byte[] var_data=readBytes(in, readInt(in));
        
        WMvar wmvar=new WMvar(var_type, var_name, var_data, timestamp);
        synchronized(changesList) { changesList.add(wmvar); }
				//System.out.println(var_type+" "+var_name);
      }
    } catch (Exception ex) { if((SocketException)ex==null) ex.printStackTrace();
    } finally {
      fireDisconnected();
    }

    try { socket.close(); } catch (Exception ex) { }
    _isConnected=false;
  }
 
  public boolean hasData() {
    boolean hasdata;
    synchronized(changesList) { hasdata=!changesList.isEmpty(); }
    return hasdata;
  }

  public WMvar getData() {
    WMvar wmvar;
    synchronized(changesList) { wmvar=(WMvar)changesList.removeFirst(); }
    return wmvar;
  }

  public boolean isConnected() {
    return _isConnected;
  }

  public void write(String s) {
    if (_isConnected) {
      _out.print(s);
      _out.flush();
    }
  }

  public WatchableMemoryListener() { super(); }
  public WatchableMemoryListener(int port) { super(port); }
  public WatchableMemoryListener(String host, int port) { super(host,port); }
}
