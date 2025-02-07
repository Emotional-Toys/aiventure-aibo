/*
 * Created on Nov 4, 2004
 */
package org.tekkotsu.ui.editor;

/**
 * @author asangpet
 */
import java.io.*;
import java.net.*;
import java.util.logging.Level;
import java.util.logging.Logger;

import org.tekkotsu.ui.editor.debug.TestResource;

public class DummyServer {
    private static Logger logger = Logger.getLogger(DummyServer.class.getName());
	public static final int port = 10080;
	//public static final String src = "Q:\\workspaces\\runtime-workspace\\test\\runlog.txt";
	//public static final String src = "e:\\projects\\runtime-workspace\\runlog.txt";
	//public static final String src = "/home/asangpet/runtime-workspace/rtu.tse";
	//public static final String src = "e:\\tekkotsu\\viewer\\rtu.tse";
	public static final String src = "lt-trace.tse";
	//public static final String srcmod = "Q:\\workspaces\\runtime-workspace\\test\\rtu.tsm";
	//public static final String srcmod = "e:\\projects\\runtime-workspace\\rtu.tsm";
	//public static final String srcmod = "/home/asangpet/runtime-workspace/rtu.tsm";
	//public static final String srcmod = "e:\\tekkotsu\\viewer\\rtu.tsm";
	public static final String srcmod = "lt.tsm";
	ServerSocket serverSocket; 
	Socket clientSocket;
	String machineName = "SampleBehavior";
		
	public void process() {	
		try {
			serverSocket = new ServerSocket(port);			
			while (true) {
				logger.info("waiting...port "+port);
				serverSocket.setReuseAddress(true);
				clientSocket = serverSocket.accept();
				logger.info("Client connect "+clientSocket);
				PrintWriter out = new PrintWriter(clientSocket.getOutputStream(), true);
				BufferedReader in = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
				try {
				while (clientSocket.isConnected()) {
					String command = in.readLine();
					logger.info(command);
					if (command.startsWith("listen")) {
						logger.info("tracing: "+command);
						BufferedReader fin = null; 
						fin = new BufferedReader(new InputStreamReader(TestResource.class.getResourceAsStream(src)));			
						Thread.sleep(50);
						String inputLine="";
						while ((inputLine = fin.readLine())!=null) {						    
							logger.info(inputLine);
							if (inputLine.indexOf("trace")>=0) continue;
							out.println(inputLine);
							if (inputLine.startsWith("</event")) {
								Thread.sleep(2000);
							}
							if (in.ready()) {
								if (in.readLine().startsWith("clear")) break;
							}
						}
					} else if (command.startsWith("spider")) {
						machineName = command.substring(7);
					    logger.fine("modeling: "+machineName);
						BufferedReader fin = null; 
						fin = new BufferedReader(new InputStreamReader(TestResource.class.getResourceAsStream(srcmod)));			
						Thread.sleep(50);
						String inputLine="";
						while ((inputLine = fin.readLine())!=null) {
							//logger.fine(inputLine);
							out.println(inputLine);
						}
					} else if (command.startsWith("list")) {
						logger.info("list statemachine");
						out.println("1");
						out.println(machineName);
					}
				}
				} catch (Exception ee) {
					ee.printStackTrace();
					clientSocket.close();
				}
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	
	public static void main(String args[]) {	    
	    logger.setLevel(Level.FINE);
		new DummyServer().process();
	}
}