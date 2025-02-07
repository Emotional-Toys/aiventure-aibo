package org.tekkotsu.mon;

import java.util.*;
import java.io.*;
import java.lang.*;

public class DogConfig {
  List IPs;
  String IP;
  Hashtable config;

  public static void main(String args[]) {
    try {
      DogConfig config=new DogConfig(args);
    } catch (IllegalArgumentException ex) { }
  }

  void usage() {
    System.out.println("java DogConfig Internet_Address");
  }

  public DogConfig(String args[]) {
    try { ClassPathModifier.addFile("ftp.jar"); }
    catch (Exception ex) { System.out.println(ex); System.exit(1); }

    for (int i=0; i<args.length; i++) {
      if (args[i].charAt(0)=='-') {
        System.out.println(args[i] + "is not a valid internet address");
        throw new IllegalArgumentException();
      }
    }

    readConfig();
    for (int i=0; i<args.length; i++) {
      if (tryIP(args[i])) {
        IP=args[i];
        break;
      }
    }

    if (IP!=null) {
      int insertpos=0;
      boolean ipbegun=false;

      for (ListIterator iter = IPs.listIterator(); iter.hasNext(); ) {
        String cline=(String)iter.next();
        int clength=cline.length();

        if (clength!=0 && cline.charAt(0)!='#') ipbegun=true;
        else if (!ipbegun) insertpos++;

        if (cline.compareToIgnoreCase(IP)==0) {
          insertpos=-1;
          break;
        }
      }
      
      if (insertpos>=0) {
        IPs.add(insertpos,IP); 
        writeConfig();
      }
    } else if (args.length==0) {
      for (ListIterator iter = IPs.listIterator(); iter.hasNext(); ) {
        String cline=(String)iter.next();
        int clength=cline.length();
        if (clength!=0 && cline.charAt(0)!='#') {
          if (tryIP(cline)) {
            IP=cline;
            break;
          }
        }
      }
    }
  
    if (IP==null) {
      System.out.println("Dog not found; please switch on dog or specify" +
                         " correct IP address");
      throw new IllegalArgumentException();
      // pop up dialog box asking for IP address?
    }
  }

  boolean tryIP(String ip) {
    DogConfigFTP dog_ftp=new DogConfigFTP(ip, 21, "config", "config");
    System.out.print("Trying "+ip+" ... ");
    System.out.flush();
    if (dog_ftp.test()) {
      System.out.println("found");
      try {
        readAiboConfig(dog_ftp.getFile("tekkotsu.cfg"));
        return true;
      } catch (Exception ex) {
        System.out.println("ftp error: "+ ex);
      }
    } else {
      System.out.println("not found");
    }
    return false;
  }

  public DogConfig() {
    this(new String[]{});
  }

  public DogConfig(String s) {
    this(new String[]{s});
  }

  public String getIP() {
    return IP;
  }

  void readAiboConfig(String conf) {
    String[] lines=conf.split("[\r\n]");
    config=new Hashtable();
    Hashtable current=config;

    for (int i=0; i<lines.length; i++) {
      String cline=lines[i];
      int clength=cline.length();
      if (clength!=0 && cline.charAt(0)!='#') {
        if (cline.charAt(0)=='<' && cline.charAt(clength-1)=='>') {
					//todo
				} else if (cline.charAt(0)=='[' && cline.charAt(clength-1)==']') {
          String category=cline.substring(1,clength-1);
          current=new Hashtable();
          config.put(category, current);
        } else {
          int ind=cline.indexOf('=');
          if (ind<0) {
            System.out.println("warning: line "+(i+1)+" ignored");
            System.out.println("       > "+cline);
          } else {
            String key=cline.substring(0,ind);
            String value=cline.substring(ind+1,clength);
            current.put(key, value);
          }
        }
      }
    }
  }

  public String getValue(String section, String key) {
    Hashtable sectionhash=(Hashtable)config.get(section);
    if (sectionhash!=null)
      return (String)sectionhash.get(key);
    return null;
  }

  File configFile() {
    String homedir="";
    try { homedir=System.getProperty("user.home"); } catch (Exception ex) {}
    return new File(homedir, ".aibo_ip");
  }

  void readConfig() {
    IPs=new ArrayList(10);
    try {
      BufferedReader in = new BufferedReader
        ( new FileReader (configFile()) );
      String curLine=in.readLine();
      while (curLine!=null) {
        IPs.add(curLine);
        curLine=in.readLine();
      }
      in.close();
    } catch (FileNotFoundException ex) {
      // we just start off with 0 IPs
      // also creating a new file, so add an intro comment
      IPs.add("# Ordered list of IPs to be used by Tekkotsu tools\n");
    } catch (IOException ex) {
      // we read what we could
    }
    // no real need for parsing and verifying correctness of IPs
  }

  void writeConfig() {
    try {
      PrintWriter out = new PrintWriter ( new BufferedWriter (
            new FileWriter (configFile())));
      for (ListIterator iter = IPs.listIterator(); iter.hasNext(); ) {
        out.println(iter.next());
      }
      out.close();
    } catch (IOException ex) {
      // can't do much about it
    }
  }
}
