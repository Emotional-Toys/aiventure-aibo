package org.tekkotsu.ui.rcp.editors.xml;

import org.eclipse.jface.text.rules.IWhitespaceDetector;


public class XMLWhitespaceDetector implements IWhitespaceDetector {

	public boolean isWhitespace(char c) {
		return (c == ' ' || c == '\t' || c == '\n' || c == '\r');
	}
}
