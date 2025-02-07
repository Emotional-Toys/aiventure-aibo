#include "StewartPlatformBehavior.h"
#include "Motion/EmergencyStopMC.h"
#include "Motion/MMAccessor.h"
#include "Wireless/Wireless.h"
#include "Events/EventRouter.h"
#include "Events/EventBase.h"

#include <libxml/parser.h>

StewartPlatformBehavior* StewartPlatformBehavior::theOne = NULL;

void StewartPlatformBehavior::DoStart() {
	// Behavior startup
	BehaviorBase::DoStart();
	// Turn on wireless
	cmdsock=wireless->socket(Socket::SOCK_STREAM, 300, 300);
	sock=cmdsock->sock;
	wireless->setDaemon(cmdsock,true);
	wireless->setReceiver(cmdsock->sock, callback);
	wireless->listen(cmdsock->sock, config->main.stewart_port);
}

void StewartPlatformBehavior::DoStop() {
	// Turn off timers
	erouter->removeListener(this);
	// Close socket; turn wireless off
  wireless->setDaemon(cmdsock,false);
	closeComm();
	cmdsock=NULL;
	// Total behavior stop
	BehaviorBase::DoStop();
}

void StewartPlatformBehavior::setParam(const std::string& cmd, float value) {
	//cout << "got " << cmd << ' ' << value << endl;
	if(cmd=="x") {
	} else if(cmd=="y") {
	} else if(cmd=="z") {
	} else if(cmd=="tilt") {
	} else if(cmd=="pan") {
	} else if(cmd=="roll") {
	} else {
		serr->printf("StewartPlatformBehavior::runCommand() - bad message: '%s' == %g\n",cmd.c_str(),value);
	}
}

void StewartPlatformBehavior::processEvent(const EventBase & /*e*/) {
}

void* StewartPlatformBehavior::getSAXHandler() {
  static xmlSAXHandler handler;
	static bool isinit=false;
	if(!isinit) {
		memset(&handler,0,sizeof(handler));
    handler.endDocument  = (endDocumentSAXFunc)  StewartPlatformBehavior::sax_end_doc;
    handler.startElement = (startElementSAXFunc) StewartPlatformBehavior::sax_start_element;
    handler.endElement   = (endElementSAXFunc)   StewartPlatformBehavior::sax_end_element;
    handler.warning      = (warningSAXFunc)      StewartPlatformBehavior::sax_warning;
		handler.error        = (errorSAXFunc)        StewartPlatformBehavior::sax_error;
		handler.fatalError   = (fatalErrorSAXFunc)   StewartPlatformBehavior::sax_fatal_error;
		isinit=true;
  }
	return &handler;
}

// The command packet reassembly mechanism
int StewartPlatformBehavior::callback(char *buf, int bytes) {
	if(StewartPlatformBehavior::theOne==NULL)
		return 0;
	
	static xmlSAXHandler * communicationParser = (xmlSAXHandler*)getSAXHandler();

	buf[bytes]='\0';
	theOne->docClosed=false;
  if(theOne->ctxt==NULL) {
		theOne->ctxt = xmlCreatePushParserCtxt(communicationParser, theOne, buf, bytes, NULL);
		if (theOne->ctxt == NULL) {
			serr->printf("StewartPlatform could not create parsing context\n");
			theOne->closeComm();
			return 1;
		}
		//this will trigger libxml to actually do processing on buf, *right now*
		if(xmlParseChunk((xmlParserCtxtPtr)theOne->ctxt,NULL,0,0)!=0) {
			serr->printf("StewartPlatform communication encountered an initial parsing error\n");
			serr->printf("closing StewartPlatformBehavior communication.\n");
			theOne->closeComm();
			return 1;
		}
	} else {
		if(xmlParseChunk((xmlParserCtxtPtr)theOne->ctxt,buf,bytes,0)!=0) {
			serr->printf("StewartPlatform communication encountered a parsing error\n");
			serr->printf("closing StewartPlatformBehavior communication.\n");
			theOne->closeComm();
			return 1;
		}
	}
	if(theOne->docClosed) {
		theOne->closeComm();
	}
  return 0;
}

void StewartPlatformBehavior::sax_start_element(StewartPlatformBehavior * inst, const char * name, const char ** attrs) {
	/*cout << "Receiving " << name << " with attributes: ";
	if(attrs!=NULL) {
		while(*attrs!=NULL)
			cout << *attrs++ << ' ';
	}
	cout << endl; */
	if(strcmp(name,"param")==0) {
		ASSERTRET(attrs!=NULL,"StewartPlatformBehavior: parser error, param missing attributes");
		const char * pname=NULL;
		const char * val=NULL;
		for(; *attrs!=NULL; attrs++) {
			if(strcmp(*attrs,"name")==0)
				pname=*++attrs;
			else if(strcmp(*attrs,"value")==0)
				val=*++attrs;
			else
				serr->printf("StewartPlatformBehavior: warning unknown attribute %s\n",*attrs++);
		}
		ASSERTRET(pname!=NULL,"StewartPlatformBehavior: parser error, param missing name attribute");
		ASSERTRET(val!=NULL,"StewartPlatformBehavior: parser error, param missing value attribute");
		inst->setParam(pname,atof(val));
	} else if(strcmp(name,"connection")==0) {
		if(inst!=NULL && inst->cmdsock!=NULL)
			inst->cmdsock->printf("<connection>\n");
	} 
}
void StewartPlatformBehavior::sax_end_element(StewartPlatformBehavior * inst, const char * name) {
	//cout << "end element " << name << endl;
	if(strcmp(name,"connection")==0) {
		inst->docClosed=true;
	}
}

void StewartPlatformBehavior::sax_end_doc(StewartPlatformBehavior * inst) {
	inst->docClosed=true;
}

void StewartPlatformBehavior::sax_warning(StewartPlatformBehavior * /*inst*/, const char *msg, ...) {
	va_list args;
	va_start(args, msg);
	serr->printf("StewartPlatformBehavior communication encountered a warning:\n");
	serr->vprintf(msg, args);
	va_end(args);
}

void StewartPlatformBehavior::sax_error(StewartPlatformBehavior * /*inst*/, const char *msg, ...) {
	va_list args;
	va_start(args, msg);
	serr->printf("StewartPlatformBehavior communication encountered an error:\n");
	serr->vprintf(msg, args);
	va_end(args);
}

void StewartPlatformBehavior::sax_fatal_error(StewartPlatformBehavior * /*inst*/, const char *msg, ...) {
	va_list args;
	va_start(args, msg);
	serr->printf("StewartPlatformBehavior communication encountered a fatal error:\n");
	serr->vprintf(msg, args);
	va_end(args);
}

bool StewartPlatformBehavior::isConnected() {
	bool con=theOne!=NULL && theOne->cmdsock!=NULL && wireless->isConnected(theOne->sock);
	//since we're using a daemon socket, we don't need to reset the cmdsock
	//if(!con)
	//theOne->cmdsock=NULL;
	if(!con && theOne->ctxt!=NULL) {
		serr->printf("StewartPlatform connection was dropped\n");
		xmlParserCtxtPtr tmp=(xmlParserCtxtPtr)theOne->ctxt;
		theOne->ctxt=NULL; //do this first to prevent recursive errors
		if(xmlParseChunk(tmp,"",0,1)!=0)
			serr->printf("StewartPlatform communication encountered a parsing error in isConnected\n");
		tmp->sax = NULL;
		xmlFreeParserCtxt(tmp);	
	}
	return con;
}

void StewartPlatformBehavior::closeComm() {
	if(cmdsock!=NULL) {
		cmdsock->printf("</connection>\n");
		wireless->close(cmdsock); //cmdsock is daemon, so it will still be valid
		if(ctxt!=NULL) {
			xmlParserCtxtPtr tmp=(xmlParserCtxtPtr)ctxt;
			ctxt=NULL; //do this first to prevent any recursive errors
			tmp->sax = NULL;
			xmlFreeParserCtxt(tmp);	
		}
	} else {
		ASSERT(ctxt==NULL,"non null parsing context with closed connection?");
		if(cmdsock!=NULL)
			wireless->close(cmdsock); //cmdsock is daemon, so it will still be valid
	}
}




/*! @file
 * @brief Implements StewartPlatformBehavior, moves the legs in synchrony to emulate the capabilities of a stewart platform
 * @author tss (Creator)
 * 
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.3 $
 * $State: Exp $
 * $Date: 2007/05/21 20:51:20 $
 */

