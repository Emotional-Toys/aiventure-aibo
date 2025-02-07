#include "XMLLoadSave.h"
#include <iostream>
#include <string>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/parserInternals.h>

using namespace std;

unsigned int XMLLoadSave::AutoInit::libxmlrefc=0;

XMLLoadSave::AutoInit::AutoInit() {
	if(libxmlrefc==0) {
		//cout << "libxmlinit" << endl;
		xmlInitParser();
		xmlKeepBlanksDefault(1);
		xmlLineNumbersDefault(1);
		xmlIndentTreeOutput = 1;
	}
	libxmlrefc++;
}

XMLLoadSave::AutoInit::~AutoInit() {
	libxmlrefc--;
	if(libxmlrefc==0) {
		xmlCleanupParser();
		//cout << "libxmldest" << endl;
	}
}

XMLLoadSave::XMLLoadSave()
  : xmldocument(NULL), compressionLevel(-1), autoFormat(true), libxmlInit()
{}

XMLLoadSave::XMLLoadSave(const XMLLoadSave& xls)
  : LoadSave(xls), xmldocument(xls.xmldocument==NULL?NULL:xmlCopyDoc(xls.xmldocument,true)), compressionLevel(xls.compressionLevel), autoFormat(xls.autoFormat), libxmlInit()
{}

XMLLoadSave& XMLLoadSave::operator=(const XMLLoadSave& xls) {
	LoadSave::operator=(xls);
	if(xls.xmldocument==NULL)
		clearParseTree();
	else
		setParseTree(xmlCopyDoc(xls.xmldocument,true));
	compressionLevel = xls.compressionLevel;
	autoFormat=xls.autoFormat;
	return *this;
}

XMLLoadSave::~XMLLoadSave() {
	clearParseTree();
}

void XMLLoadSave::reportError(const std::string& context, const bad_format& err) const {
	cerr << context << endl;
	cerr << "  " << err.what() << endl;
	if(err.getNode()!=NULL) {
		xmlChar* path=xmlGetNodePath(err.getNode());
		cerr << "  Error was flagged during processing of node at line " << xmlGetLineNo(err.getNode()) << ": " << path << " '" << xmlNodeGetContent(err.getNode()) << '\'' << endl;
		xmlFree(path);
	}
}

unsigned int XMLLoadSave::getBinSize() const {
	try {
		if(xmldocument==NULL)
			setParseTree(xmlNewDoc((const xmlChar*)"1.0"));
		if(compressionLevel>=0)
			xmlSetDocCompressMode(xmldocument,compressionLevel);
		xmlNode * cur = FindRootXMLElement(xmldocument);
		saveXML(cur);
		xmlChar* buf=NULL;
		int size=0;
		xmlDocDumpFormatMemory(xmldocument, &buf, &size, autoFormat);
		xmlFree(buf);
		return size;
	} catch(const bad_format& err) {
		reportError("During calculation of size:",err);
		return 0;
	}
}
unsigned int XMLLoadSave::loadBuffer(const char buf[], unsigned int len) {
	if(xmldocument!=NULL)
		xmlFreeDoc(xmldocument);
	
	//does actual low-level XML parsing
	xmlParserCtxt* ctxt=xmlCreateMemoryParserCtxt(buf,len);
	if(ctxt==NULL) {
		cerr << "Error: XMLLoadSave could not create memory parser context" << endl;
		return 0;
	}
	xmldocument = xmlParseDocument(ctxt)==0?ctxt->myDoc:NULL;
	if (xmldocument == NULL ) {
		cerr << "Error: XMLLoadSave buffer not parsed successfully. (xml syntax error)\n"
		     << "       Attempting to recover..." << endl;
		xmlFreeParserCtxt(ctxt);
		ctxt=xmlCreateMemoryParserCtxt(buf,len);
		if(ctxt==NULL) {
			cerr << "Error: XMLLoadSave could not create memory parser context" << endl;
			return 0;
		}
		ctxt->recovery=1;
		xmldocument = xmlParseDocument(ctxt)==0?ctxt->myDoc:NULL;
		if(xmldocument==NULL) {
			cerr << "Error: XMLLoadSave recovery failed." << endl;
			xmlFreeParserCtxt(ctxt);
			return 0;
		}
	}
	unsigned int size=ctxt->nbChars;
	xmlFreeParserCtxt(ctxt);
	
	try {
		xmlNodePtr cur = FindRootXMLElement(xmldocument);
		loadXML(cur);
		return size;
	} catch(const bad_format& err) {
		reportError("During load of memory buffer:",err);
		xmlFreeDoc(xmldocument);
		xmldocument=NULL;
		return 0;
	}
}
unsigned int XMLLoadSave::saveBuffer(char buf[], unsigned int len) const {
	try {
		if(xmldocument==NULL)
			setParseTree(xmlNewDoc((const xmlChar*)"1.0"));
		if(compressionLevel>=0)
			xmlSetDocCompressMode(xmldocument,compressionLevel);
		xmlNode * cur = FindRootXMLElement(xmldocument);
		saveXML(cur);
		xmlChar* xbuf=NULL;
		int size=0;
		xmlDocDumpFormatMemory(xmldocument, &xbuf, &size, autoFormat);
		if((unsigned int)size<=len)
			memcpy(buf,xbuf,size);
		else {
			cerr << "Error: XMLLoadSave::saveBuffer xmlDocDumpFormatMemory returned larger region than the target buffer" << endl;
			size=0;
		}
		return size;
	} catch(const bad_format& err) {
		reportError("During save to memory buffer:",err);
		return 0;
	}
}

unsigned int XMLLoadSave::loadFile(const char* filename) {
	if(xmldocument!=NULL)
		xmlFreeDoc(xmldocument);
	
	//does actual low-level XML parsing
	xmlParserCtxt* ctxt=xmlCreateFileParserCtxt(filename);
	if(ctxt==NULL) {
		cerr << "Error: XMLLoadSave could not create parser context for '"<< filename << "'" << endl;
		return 0;
	}
	xmldocument = xmlParseDocument(ctxt)==0?ctxt->myDoc:NULL;
	if (xmldocument == NULL ) {
		cerr << "Error: XMLLoadSave document '" << filename << "' not parsed successfully. (file not found or xml syntax error)\n"
		<< "       Attempting to recover..." << endl;
		xmlFreeParserCtxt(ctxt);
		ctxt=xmlCreateFileParserCtxt(filename);
		if(ctxt==NULL) {
			cerr << "Error: XMLLoadSave could not create parser context for '"<< filename << "'" << endl;
			return 0;
		}
		ctxt->recovery=1;
		xmldocument = xmlParseDocument(ctxt)==0?ctxt->myDoc:NULL;
		if(xmldocument==NULL) {
			cerr << "Error: XMLLoadSave document '" << filename << "' recovery failed." << endl;
			xmlFreeParserCtxt(ctxt);
			return 0;
		}
	}
	unsigned int size=ctxt->nbChars;
	xmlFreeParserCtxt(ctxt);
	
	try {
		xmlNodePtr cur = FindRootXMLElement(xmldocument);
		loadXML(cur);
		return size;
	} catch(const bad_format& err) {
		string context("During load of '");
		context+=filename;
		context+="':";
		reportError(context,err);
		xmlFreeDoc(xmldocument);
		xmldocument=NULL;
		return 0;
	}
}
unsigned int XMLLoadSave::saveFile(const char* filename) const {
	try {
		if(xmldocument==NULL)
			setParseTree(xmlNewDoc((const xmlChar*)"1.0"));
		if(compressionLevel>=0)
			xmlSetDocCompressMode(xmldocument,compressionLevel);
		xmlNode * cur = FindRootXMLElement(xmldocument);
		saveXML(cur);
		int size=xmlSaveFormatFile (filename, xmldocument, autoFormat);
		if(size==-1)
			cerr << "Error: XMLLoadSave::saveFile: xmlSaveFormatFile(\"" << filename << "\",...) returned -1" << endl;
		return size==-1?0:size;
	} catch(const bad_format& err) {
		string context("During save to '");
		context+=filename;
		context+="':";
		reportError(context,err);
		return 0;
	}
}

unsigned int XMLLoadSave::loadFileStream(FILE* f) {
	if(xmldocument!=NULL)
		xmlFreeDoc(xmldocument);
	
	//does actual low-level XML parsing
	//This is a little sketchy trying to shoehorn a SAX style call, but it seems to work
	xmlParserCtxt* ctxt=xmlCreateIOParserCtxt(NULL,NULL,fileReadCallback,fileCloseCallback,f,XML_CHAR_ENCODING_UTF8);
	if(ctxt==NULL) {
		cerr << "Error: XMLLoadSave could not create file stream parser context" << endl;
		return 0;
	}
	ctxt->recovery=1;
	xmldocument = xmlParseDocument(ctxt)==0?ctxt->myDoc:NULL;
	unsigned int size=ctxt->nbChars;
	bool wellFormed=ctxt->wellFormed;
	xmlFreeParserCtxt(ctxt);
	if (xmldocument == NULL ) {
		cerr << "Error: XMLLoadSave file stream not parsed successfully. (xml syntax error)\n" << endl;
		return 0;
	}
	if(!wellFormed)
		cerr << "Warning: XMLLoadSave file stream was not well formed (but was recovered)." << endl;
	
	try {
		xmlNodePtr cur = FindRootXMLElement(xmldocument);
		loadXML(cur);
		return size;
	} catch(const bad_format& err) {
		reportError("During load of file stream:",err);
		xmlFreeDoc(xmldocument);
		xmldocument=NULL;
		return 0;
	}
}
unsigned int XMLLoadSave::saveFileStream(FILE* f) const {
	try {
		if(xmldocument==NULL)
			setParseTree(xmlNewDoc((const xmlChar*)"1.0"));
		if(compressionLevel>=0)
			xmlSetDocCompressMode(xmldocument,compressionLevel);
		xmlNode * cur = FindRootXMLElement(xmldocument);
		saveXML(cur);
		int size=xmlDocFormatDump(f, xmldocument, autoFormat);
		if(size==-1)
			cerr << "Error: XMLLoadSave::saveFileStream: xmlDocFormatDump(...) returned -1" << endl;
		return size==-1?0:size;
	} catch(const bad_format& err) {
		reportError("During save to file stream:",err);
		return 0;
	}
}

void XMLLoadSave::clearParseTree() {
	xmlFreeDoc(xmldocument);
	xmldocument=NULL;
}
void XMLLoadSave::setParseTree(xmlDoc* doc) const {
	if(doc==xmldocument)
		return;
	xmlFreeDoc(xmldocument);
	xmldocument=doc;
}
void XMLLoadSave::readParseTree() {
	if(xmldocument==NULL)
		return;
	try {
		xmlNodePtr cur = FindRootXMLElement(xmldocument);
		loadXML(cur);
	} catch(const bad_format& err) {
		reportError("During XMLLoadSave::readParseTree:",err);
		xmlFreeDoc(xmldocument);
		xmldocument=NULL;
	}
}
void XMLLoadSave::writeParseTree() {
	try {
		if(xmldocument==NULL)
			setParseTree(xmlNewDoc((const xmlChar*)"1.0"));
		if(compressionLevel>=0)
			xmlSetDocCompressMode(xmldocument,compressionLevel);
		xmlNode * cur = FindRootXMLElement(xmldocument);
		saveXML(cur);
	} catch(const bad_format& err) {
		reportError("During writeParseTree:",err);
	}
}

void XMLLoadSave::setCompression(int level) {
	compressionLevel=level;
	if(xmldocument!=NULL)
		xmlSetDocCompressMode(xmldocument,compressionLevel);
}

xmlNode* XMLLoadSave::FindRootXMLElement(xmlDoc* doc) const {
	if(doc==NULL)
		return NULL;
	xmlNode* cur=xmlDocGetRootElement(doc);
	if(cur==NULL) {
		//empty file
		cur = xmlNewNode(NULL,(const xmlChar*)"");
		xmlDocSetRootElement(doc,cur);
	} 
	return cur;
}

int XMLLoadSave::fileReadCallback(void* f,char* buf, int len) {
	return ferror((FILE*)f)?-1:(int)fread(buf,sizeof(char),len,(FILE*)f);
}
int XMLLoadSave::fileCloseCallback(void* f) {
	return fclose((FILE*)f)==0?0:-1;
}

xmlNode* XMLLoadSave::skipToElement(xmlNode* cur) {
	while(cur!=NULL && cur->type!=XML_ELEMENT_NODE)
		cur=cur->next;
	return cur;
}

xmlNode* XMLLoadSave::skipToElement(xmlNode* cur, std::string& comment) {
	comment.clear();
	while(cur!=NULL && cur->type!=XML_ELEMENT_NODE) {
		if(cur->type==XML_COMMENT_NODE) {
			xmlChar* cont=xmlNodeGetContent(cur);
			comment=(char*)cont; //only take last comment in series
			xmlFree(cont);
		}
		cur=cur->next;
	}
	return cur;
}

/*! @file
 * @brief 
 * @author Ethan Tira-Thompson (ejt) (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.7 $
 * $State: Exp $
 * $Date: 2007/11/01 20:25:26 $
 */
