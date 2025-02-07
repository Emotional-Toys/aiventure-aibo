package org.tekkotsu.mon;

import javax.swing.*;
import javax.swing.event.*;
import javax.swing.table.*;
import java.awt.*;
import java.awt.event.*;
import java.util.*;

public class TextWatchableMemory {
  WatchableMemoryListener listener;

  public static void main(String args[]) {
    if (args.length>1) {
      System.out.println("usage: java TextWatchableMemory [host]");
      System.exit(1);
    }

    try {
      DogConfig dogConfig=new DogConfig (args);
      TextWatchableMemory watchableMemory=new TextWatchableMemory(
        dogConfig.getIP(),
        Integer.parseInt(dogConfig.getValue("Main", "wmmonitor_port")));
    } catch (IllegalArgumentException ex) {}
  }

  public TextWatchableMemory(String ip, int port) {
    listener=new WatchableMemoryListener(ip, port);

    while (true) {
      if (listener.isConnected() && listener.hasData()) {
        System.out.println(listener.getData());
      } else {
        sleep(1);
      }
    }
  }

  void sleep(long ms) {
    try { Thread.sleep(ms); } catch (Exception e) {}
  }
}
