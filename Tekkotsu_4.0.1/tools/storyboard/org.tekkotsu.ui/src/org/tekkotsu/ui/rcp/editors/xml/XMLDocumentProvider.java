package org.tekkotsu.ui.rcp.editors.xml;

import org.eclipse.core.runtime.CoreException;

import org.eclipse.jface.text.IDocument;
import org.eclipse.jface.text.IDocumentPartitioner;
import org.eclipse.jface.text.rules.FastPartitioner;
import org.eclipse.jface.text.source.IAnnotationModel;

import org.tekkotsu.ui.rcp.editors.SimpleDocumentProvider;

public class XMLDocumentProvider extends SimpleDocumentProvider {

	protected void setupDocument(IDocument document) {
		if (document != null) {
			IDocumentPartitioner partitioner =
				new FastPartitioner(
					new XMLPartitionScanner(),
					new String[] {
						XMLPartitionScanner.XML_TAG,
						XMLPartitionScanner.XML_COMMENT });
			partitioner.connect(document);
			document.setDocumentPartitioner(partitioner);
		}
	}

	protected IAnnotationModel createAnnotationModel(Object element) throws CoreException {
		return null;
	}
}