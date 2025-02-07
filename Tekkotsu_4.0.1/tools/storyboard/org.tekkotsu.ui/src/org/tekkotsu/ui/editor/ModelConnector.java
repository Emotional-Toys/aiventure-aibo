/*
 * Created on Nov 4, 2004
 */
package org.tekkotsu.ui.editor;

import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;
import java.beans.PropertyChangeSupport;
import java.io.*;
import java.net.*;
import java.util.logging.Logger;

import org.tekkotsu.ui.editor.resources.Debugger;

/**
 * @author asangpet
 */
public class ModelConnector implements Runnable, PropertyChangeListener {
    private static Logger logger = Logger.getLogger(ModelConnector.class
            .getName());

    public static final String EVENT_DATA_UPDATE = "_model_conn_update";

    public static final String EVENT_MODEL_UPDATE = "_model_conn_model_update";

    public static final String EVENT_TRACE_UPDATE = "_model_conn_trace_update";

    public static final String EVENT_TRACE_REFRESH = "_model_conn_trace_refresh";

    public static final String EVENT_TRACE_UPDATE_TIME = "_model_conn_trace_update_time";

    public static final String EVENT_TRACE_CLEAR = "_model_conn_trace_clear";

    public static final String EVENT_TRACE_MODE_ENTER = "_model_conn_trace_mode";

    public static final String EVENT_TRACE_MODE_EXIT = "_model_conn_trace_exit";

    static String defaultHostName = "localhost";

    static String defaultMachineName = "Explore State Machine";

    static int defaultPort = 10080;

    int hostPort = defaultPort;

    String hostName = defaultHostName;

    String command = "find";

    String lastCommand;

    String machineName = defaultMachineName;

    Socket modelSocket = null;

    PrintWriter out = null;

    StringBuffer outbuf = null;

    BufferedReader in = null;

    Thread worker;

    boolean isStop = false; // indicate whether the thread is sending info to

    // client

    boolean isReady = false; // indicate whether there is a thread running

    boolean isResume = false;

    boolean needReconnect = true;

    boolean commandChange = true;

    private PropertyChangeSupport listeners = new PropertyChangeSupport(this);

    TimerTick tick = new TimerTick(0, this);

    class TimerTick extends Thread {

        ModelConnector conn;

        int time;

        boolean isCount = true;

        public TimerTick(int startTime, ModelConnector conn) {
            time = startTime;
            this.conn = conn;
        }

        public synchronized void setTime(int time) {
            this.time = time;
        }

        public synchronized int getTime() {
            return time;
        }

        public void stopTime() {
            isCount = false;
        }

        public void run() {
            while (isCount) {
                try {
                    // sleep for 1 second
                    sleep(1000);
                    time = time + 1000;
                    conn.firePropertyChange(
                            ModelConnector.EVENT_TRACE_UPDATE_TIME, null,
                            new Integer(time));
                } catch (Exception e) {
                }
            }
        }
    }

    public void propertyChange(PropertyChangeEvent evt) {
        if (evt.getPropertyName().equals(EVENT_TRACE_UPDATE_TIME)) {
            int time = ((Integer) evt.getNewValue()).intValue();
            if (time > tick.getTime()) {
                tick.setTime(time);
            }
            if (time < tick.getTime()) {
                tick.setTime(time);
                firePropertyChange(EVENT_TRACE_REFRESH, null, null);
            }
        }

    }

    public void addPropertyChangeListener(PropertyChangeListener listener) {
        listeners.addPropertyChangeListener(listener);
    }

    public void firePropertyChange(String propName, Object oldValue,
            Object newValue) {
        listeners.firePropertyChange(propName, oldValue, newValue);
    }

    public void removePropertyChangeListener(PropertyChangeListener listener) {
        listeners.removePropertyChangeListener(listener);
    }

    public void setMachineName(String machName) {
        this.machineName = machName;
        defaultMachineName = machName;
        needReconnect = true;
    }

    public String getMachineName() {
        return machineName;
    }

    // send a query to the robot
    public void execute(String command) {
        isStop = false;
        setCommand(command);

        if (!isReady) {
            Thread t = new Thread(this);
            t.start();
        }
    }

    boolean connect() {
        try {
            try {
                if (out != null) out.close();
                if (in != null) in.close();
                if (modelSocket != null) modelSocket.close();
                modelSocket = null;
            } catch (Exception e) {
                e.printStackTrace();
            }

            firePropertyChange(EVENT_DATA_UPDATE, null, "Connecting to "
                    + getHostName() + ":" + getHostPort() + "...");
            modelSocket = new Socket(hostName, hostPort);
            out = new PrintWriter(modelSocket.getOutputStream(), true);
            in = new BufferedReader(new InputStreamReader(modelSocket
                    .getInputStream()));
            needReconnect = false;
            firePropertyChange(EVENT_DATA_UPDATE, null, "Connected");
            return true;
        } catch (Exception e) {
            firePropertyChange(EVENT_DATA_UPDATE, null,
                    "Unable to connect, please try again");
            e.printStackTrace();
            return false;
        }
    }

    public ModelConnector() {
    }

    public ModelConnector(String hostname, int port) {
        setHostName(hostname);
        setHostPort(port);
    }

    public String readLine() {
        try {
            String result = in.readLine();
            Debugger.printDebug(Debugger.DEBUG_ALL, "read: " + result);
            firePropertyChange(EVENT_DATA_UPDATE, null, result);
            return result;
        } catch (IOException e) {
            return null;
        }
    }

    public boolean isStop() {
        return isStop;
    }

    void disconnect() {
        try {
            firePropertyChange(EVENT_DATA_UPDATE, null, "Disconnect");
            if (out != null)
                out.close();
            if (in != null)
                in.close();
            if (modelSocket != null)
                modelSocket.close();
            modelSocket = null;
            // isStop = true;
            needReconnect = true;
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    /**
     * @return Returns the hostName.
     */
    public String getHostName() {
        return hostName;
    }

    /**
     * @param hostName
     *            The hostName to set.
     */
    public void setHostName(String hostName) {
        this.hostName = hostName;
        defaultHostName = hostName;
    }

    /**
     * @return Returns the hostPort.
     */
    public int getHostPort() {
        return hostPort;
    }

    /**
     * @param hostPort
     *            The hostPort to set.
     */
    public void setHostPort(int hostPort) {
        this.hostPort = hostPort;
        defaultPort = hostPort;
        needReconnect = true;
    }

    private String trimBuf(String prefix, String suffix, StringBuffer buf) {
        int idstart = buf.indexOf(prefix);
        int idend = buf.indexOf(suffix);
        if (idstart == -1)
            return prefix + ">" + suffix;       
        return buf.substring(idstart, idend + suffix.length());
    }

    public void run() {
        // event loop - keep reading
        String result = "";
        outbuf = new StringBuffer();
        while (true) {
            try {
                isReady = true;
                if (needReconnect) {
                    isReady = connect();
                }
                if (!isReady) {
                    isStop = true;
                    tick.stopTime();
                    return;
                }
                if (commandChange) {
                    out.println("clear");
                    if (getCommand().equals("clear")) {
                        commandChange = false;
                        logger.finest("reader spin");
                        continue;
                    }
                    out.println(getCommand() + " " + machineName); // spider

                    // start timer
                    if (isReady) {
                        if (command.startsWith("listen")) {
                            //tick.setTime(0);
                            //tick.start();
                        }
                    }

                    // MachineName
                    firePropertyChange(EVENT_DATA_UPDATE, null,
                            "Enter reading loop " + getCommand() + " "
                                    + machineName + " from " + getHostName()
                                    + "\r\n");
                    result = "";
                    outbuf = new StringBuffer();
                    if (!isResume) {
                        // firePropertyChange(EVENT_TRACE_CLEAR, null, null);
                        isResume = false;
                    } else {
                        // resuming
                        // TODO implement resume code for timer, do we need this?
                        /*
                         * int time = tick.getTime(); tick = new
                         * TimerTick(time,this); tick.start();
                         */
                    }
                    commandChange = false;
                }
                if (in.ready()) {
                    result = readLine();
                    if (result != null) {
                        outbuf.append(result + "\n");
                        if (result.indexOf("</model>") > -1) {
                            if (!isStop())
                                firePropertyChange(EVENT_MODEL_UPDATE, null,
                                        trimBuf("<model", "</model>", outbuf));
                            outbuf = new StringBuffer();
                        } else if (result.indexOf("</event>") > -1) {
                            if (!isStop()) {
                                while (outbuf.toString().indexOf("</event>") > -1) {
                                	if (outbuf.toString().indexOf("<event ") > -1) {
                                		String bufstr = outbuf.toString();
                                		int idstart = bufstr.indexOf("<event ");
                                		int idend = bufstr.indexOf("/>");
                                		if (idend>-1) {
                                			int idnend = bufstr.indexOf(">");
                                			if (!(idnend > -1 && idnend < idend)) {                                				
                                				firePropertyChange(EVENT_TRACE_UPDATE,
                                						null, bufstr.substring(idstart,idend+ "/>".length()));          
                                				outbuf.delete(0, idend+ ("/>".length()));
                                			}
                                		}
                                	}
                                    firePropertyChange(EVENT_TRACE_UPDATE,
                                            null, trimBuf("<event", "</event>",
                                                    outbuf));
                                    int idend = outbuf.indexOf("</event>");
                                    outbuf.delete(0, idend
                                            + "</event>".length());
                                }
                            }
                            outbuf = new StringBuffer();
                        }
                    }
                    continue;
                }
                Thread.sleep(1000);
                //Thread.yield();
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }

    public StringBuffer getOutputBuffer() {
        return outbuf;
    }

    public void stop() {
        tick.stopTime();
        lastCommand = command;
        setCommand("clear");
        isStop = true;
    }

    public void newTrace() {
        tick.stopTime();
        tick = new TimerTick(0, this);
        tick.start();
        execute("listen");
    }

    public void resume() {
        isResume = true;
        tick.stopTime();
        int time = tick.getTime();
        tick = new TimerTick(time, this);
        tick.start();
        execute(lastCommand);
    }

    public String getCommand() {
        return command;
    }

    public void setCommand(String command) {
        this.command = command;
        commandChange = true;
    }
}