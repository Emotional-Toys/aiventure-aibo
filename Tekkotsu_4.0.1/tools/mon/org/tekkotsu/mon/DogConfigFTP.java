package org.tekkotsu.mon;

import com.enterprisedt.net.ftp.*;
import java.util.*;
import java.net.*;
import java.io.*;

public class DogConfigFTP {
  String _ip;
  int _port;
  String _user;
  String _pass;

  public DogConfigFTP(String ip) {
    this(ip, 21);
  }

  public DogConfigFTP(String ip, int port) {
    _ip=ip;
    _port=port;
    _user="anonymous";
    _pass="DogConfigFTP@tekkotsu.org";
  }

  public DogConfigFTP(String ip, int port, String user, String pass) {
    this(ip, port);
    _user=user;
    _pass=pass;
  }
  
  public boolean test() {
    return test(500);
  }

  public boolean test(int millis) {
    Socket _ftpSocket=new Socket();
    try {
      _ftpSocket.connect(new InetSocketAddress(_ip, _port), millis);
    } catch (Exception ex) { }
    if (_ftpSocket.isConnected()) {
      try { _ftpSocket.close(); } catch (Exception ex) { }
      return true;
    } else {
      return false;
    }
  }

  public String getFile(String remotename) throws FTPException, IOException {
    FTPClient ftp=new FTPClient(_ip, _port);
    ftp.login(_user, _pass);
    ftp.setConnectMode(FTPConnectMode.ACTIVE);
    ftp.setType(FTPTransferType.ASCII);
    String ret=new String(ftp.get(remotename));
    ftp.quit();
    return ret;
  }

  public String getFile(String user, String pass, String remotename)
      throws FTPException, IOException {
    _user=user;
    _pass=pass;
    return getFile(remotename);
  }

  public static void main(String args[]) {
    System.out.println("This is an internal class - run DogConfig instead");
    System.exit(0);
  }
}
