/*
 * Created on Oct 24, 2004
 */
package org.tekkotsu.ui.editor.resources;

/**
 * @author asangpet
 *
 */
public class Debugger {
	public static int DEBUG_ALL = 4;
	public static int DEBUG_RELEASE = 3;
	public static int getDebugLevel() {
		return DEBUG_RELEASE;
	}
	
	public static void printDebug(int level, String s) {
		if (level<=getDebugLevel()) System.err.println(s);
	}
	
	public static void printDebug(String s) {
		System.err.println(s);
	}
}
