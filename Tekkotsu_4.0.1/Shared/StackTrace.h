#ifndef INCLUDED_StackTrace_h_
#define INCLUDED_StackTrace_h_

#include <stdlib.h>

#ifdef __cplusplus
//! Holds the C-style interface for the stack trace routines
namespace stacktrace {
extern "C" {
#endif /* __cplusplus */

typedef int machineInstruction; //!< typedef in case type needs to change on other platforms (i.e. long for 64 bit architectures?)

//! Must be called with the object file containing debugging symbols before symbolic output can be displayed
/*! @param objfile the file containing debugging symbols, in a format supported by libBFD; generally this is the executable itself (compiled with -g), so pass argv[0]
 *
 *  Display done before this has been called will display numeric addresses, which may
 *  still be decoded externally -- this is handy for embedded systems which may not be
 *  able to hold the debugging symbols and BFD library themselves, but the numerical
 *  values can be looked up on the desktop workstation.
 *  @return zero on success, negative if file could not be found, positive if it could not be parsed (wrong file type) or any other internal error */
int loadStackTraceSymbols(const char* objfile);
	
//! Stores information about a single stack frame
struct StackFrame {

	//! stack pointer, points to end of stack frame
	void * sp; 

#if defined(__PIC__) && (defined(__MIPSEL__) || defined(__MIPS__))
	//! return address, points to instruction being executed within current frame
	/*! Note that this is the address that is being returned to by the @e sub-function,
	 *  @e not the address that this frame itself is returning to.
	 *  When executing position independent code (PIC), this is the address relative
	 *  to #gp.  In other words, subtract this from #gp to get current memory address,
	 *  or subtract from the binary's _gp symbol to get link-time address (for looking up file/line) */
	size_t ra; 
	
	//! global offset used in position independent code (PIC)
	/*! subtract #ra from this to get the actual run-time memory address of the instruction */
	size_t gp;

#else
	//! return address, points to instruction being executed within current frame
	/*! Note that this is the address that is being returned to by the @e sub-function,
	 *  @e not the address that this frame itself is returning to. */
	machineInstruction * ra; 
#endif /* __PIC__ */

	//! points to the caller's stack frame (stack frame of function which called this one), may be NULL or self-referential at end of list
	/*! a self-referential value indicates the frame is not the last on the stack, but is the last recorded. */
	struct StackFrame * caller;

#ifdef DEBUG_STACKTRACE
	//! if DEBUG_STACKTRACE is defined, this field is available which, if non-zero, will cause debugging info to be displayed on each unroll
	int debug;
#endif
};


//! stores information about the caller's stack frame into @a frame
void getCurrentStackFrame(struct StackFrame* frame);

//! stores information about the caller to @a curFrame into @a nextFrame
/*! @return 0 if error occurred (i.e. bottom of the stack), non-zero upon success
 *  @a nextFrame @e can be the same instance as @a curFrame, will update in place.
 *  @a curFrame->caller will be set to @a nextFrame. */
int unrollStackFrame(struct StackFrame* curFrame, struct StackFrame* nextFrame);

//! frees a list of StackFrames, such as is returned by recordStackTrace
void freeStackTrace(struct StackFrame* frame);

//! preallocates a stack trace of a particular size (doesn't actually perform a stack trace, merely allocates the linked list)
/*! this is a good idea if you want to do a stack trace within an exception handler, which might have been triggered by running out of heap */
struct StackFrame* allocateStackTrace(unsigned int size);

//! dumps stored stack trace to stderr
void displayStackTrace(const struct StackFrame* frame);

#ifndef __cplusplus

//! dumps current stack trace to stderr, up to @a limit depth and skipping the top @a skip frames
/*! pass -1U for limit to request unlimited trace, and 0 to start with the function calling recordStackTrace */
void displayCurrentStackTrace(unsigned int limit, unsigned int skip);

//! repeatedly calls unrollStackFrame() until the root frame is reached or @a limit is hit, skipping the top @a skip frames
/*! pass -1U for limit to request unlimited trace, and 0 to start with the function calling recordStackTrace */
struct StackFrame * recordStackTrace(unsigned int limit, unsigned int skip);
//! repeatedly calls unrollStackFrame() until the root frame is reached or end of @a frame list is hit, skipping the top @a skip frames
/*! This is handy for reusing previously allocated frames, returns the unused portion (if return value equals @a frame, none were used -- implies never cleared @a skip) */
struct StackFrame * recordOverStackTrace(struct StackFrame* frame, unsigned int skip);

#else /* __cplusplus */

//! dumps current stack trace to stderr, up to @a limit depth and skipping the top @a skip frames
/*! pass -1U for limit to request unlimited trace, and 0 to start with the function calling recordStackTrace */
void displayCurrentStackTrace(unsigned int limit=-1U, unsigned int skip=0);

//! repeatedly calls unrollStackFrame() until the root frame is reached or @a limit is hit, skipping the top @a skip frames
/*! pass -1U for limit to request unlimited trace, and 0 to start with the function calling recordStackTrace */
struct StackFrame * recordStackTrace(unsigned int limit=-1U, unsigned int skip=0);
//! repeatedly calls unrollStackFrame() until the root frame is reached or end of @a frame list is hit, skipping the top @a skip frames
/*! This is handy for reusing previously allocated frames, returns the unused portion (if return value equals @a frame, none were used -- implies never cleared @a skip) */
struct StackFrame * recordOverStackTrace(struct StackFrame* frame, unsigned int skip=0);

}
}

#endif /* __cplusplus */

/*! @file
 * @brief Describes functionality for performing stack traces
 * @author ejt (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.6.2.1 $
 * $State: Exp $
 * $Date: 2009/02/08 03:34:42 $
 */
#endif
