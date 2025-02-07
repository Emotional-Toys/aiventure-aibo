#include "StackTrace.h"
#include <stdio.h>

#if defined(HAVE_BFD) && HAVE_BFD!=0
#  include <bfd.h>
#  include "libiberty.h"
#  include "demangle.h"
#  include <sys/stat.h>
#  include <string.h>
#endif

#ifdef ST_UNUSED 
#elif defined(__GNUC__) && __GNUC__>3
//! portable access to compiler hint not to warn if a function argument is ignored (goes in argument list)
# define ST_UNUSED(x) UNUSED_##x __attribute__((unused)) 
//! portable access to compiler hint not to warn if a function argument is ignored (goes at beginning of function body)
# define ST_BODY_UNUSED(x) /*no body necessary*/

#elif defined(__LCLINT__) 
//! portable access to compiler hint not to warn if a function argument is ignored (goes in argument list)
# define ST_UNUSED(x) /*@unused@*/ x 
//! portable access to compiler hint not to warn if a function argument is ignored (goes at beginning of function body)
# define ST_BODY_UNUSED(x) /*no body necessary*/

#else 
//! portable access to compiler hint not to warn if a function argument is ignored (goes in argument list)
# define ST_UNUSED(x) UNUSED_##x 
//! portable access to compiler hint not to warn if a function argument is ignored (goes at beginning of function body)
# define ST_BODY_UNUSED(x) (void)UNUSED_##x /* ugly hack to avoid warning */
#endif

#ifdef __cplusplus
namespace stacktrace {
#endif /* __cplusplus */

#if defined(HAVE_BFD) && HAVE_BFD!=0
char * bfd_objfile=NULL;
#endif

//! target string to use when opening symbol files if host architecture can't be detected
const char * BFD_DEFAULT_TARGET="powerpc-apple-darwin8.5.0";
//const char * BFD_DEFAULT_TARGET="i686-pc-linux-gnu";

#if defined(HAVE_BFD) && HAVE_BFD!=0
int loadStackTraceSymbols(const char* objfile) {
	static int bfdInited=0;
	if(!bfdInited) {
		bfd_init();
		if(!bfd_set_default_target(BFD_DEFAULT_TARGET)) {
			bfd_error_type err=bfd_get_error();
			fprintf(stderr,"can't set BFD default target to `%s': %s\n",BFD_DEFAULT_TARGET,bfd_errmsg(err));
			fprintf(stderr,"stacktrace::loadStackTraceSymbols(%s) failed\n",objfile);
			return err;
		}
		bfdInited=1;
	}
	/*const char ** tgt=bfd_target_list();
	while(*tgt)
		printf("%s\n",*tgt++);
	free(bfd_objfile);
	if(objfile==NULL) {
		bfd_objfile=NULL;
		return 0;
	}
	struct stat sb;
	if(stat(objfile,&sb)) {
		char msg[128];
		snprintf(msg,128,"stacktrace::loadStackTraceSymbols(%s): stat",objfile);
		perror(msg);
		return -1;
	}
	bfd * abfd = bfd_openr(objfile, NULL);
	if(abfd==NULL) {
		bfd_error_type err=bfd_get_error();
		fprintf(stderr,"Can't open object file `%s': %s\n",objfile,bfd_errmsg(err));
		fprintf(stderr,"stacktrace::loadStackTraceSymbols(%s) failed\n",objfile);
		return err;
	}
	if(!bfd_close(abfd)) {
		bfd_error_type err=bfd_get_error();
		fprintf(stderr,"Can't close object file `%s': %s\n",objfile,bfd_errmsg(err));
		fprintf(stderr,"stacktrace::loadStackTraceSymbols(%s) failed\n",objfile);
		return err;
		}*/
	bfd_objfile=strdup(objfile);
	return 0;
}
#else
int loadStackTraceSymbols(const char* ST_UNUSED(objfile)) {
	ST_BODY_UNUSED(objfile);
	return 0;
}
#endif

int unrollStackFrame(struct StackFrame* curFrame, struct StackFrame* nextFrame) {
	void* nsp=NULL;
	machineInstruction * nra=NULL;

#if defined(__i386__) || defined(__x86_64__) || defined(__amd64__)
	if(curFrame==NULL)
		return 0;
	curFrame->caller=NULL;
	if(nextFrame==NULL)
		return 0;
	if(curFrame->sp==NULL)
		return 0;
	if(((void**)curFrame->sp)-1==NULL)
		return 0;
	nsp=((void***)curFrame->sp)[-1];
	if(nsp==NULL)
		return 0;
	nsp=(void**)nsp+1; //move from frame pointer to stack pointer of previous frame
	nra=*((machineInstruction**)curFrame->sp);
	if(nsp<=curFrame->sp) {
		fprintf(stderr,"stacktrace::unrollStackFrame(sp=%p,ra=%p) directed to invalid next frame: (sp=%p,ra=%p)\n",curFrame->sp,curFrame->ra,nsp,nra);
		return 0;
	}
#  ifdef DEBUG_STACKTRACE
	if(curFrame->debug)
		printf("( %p %p ) -> { %p %p }\n",curFrame->sp,curFrame->ra,nsp,nra);
	nextFrame->debug=curFrame->debug;
#  endif
	nextFrame->sp=nsp;
	nextFrame->ra=nra;
	curFrame->caller=nextFrame;
	return 1;
#endif
#ifdef __POWERPC__
	if(curFrame==NULL)
		return 0;
	curFrame->caller=NULL;
	if(nextFrame==NULL)
		return 0;
	if(curFrame->sp==NULL)
		return 0;
	if(*(void**)curFrame->sp==NULL)
		return 0;
	nsp=*(void**)curFrame->sp;
	nra=((machineInstruction**)nsp)[2];
	if(nsp<=curFrame->sp) {
		fprintf(stderr,"stacktrace::unrollStackFrame(sp=%p,ra=%p) directed to invalid next frame: (sp=%p,ra=%p)\n",curFrame->sp,curFrame->ra,nsp,nra);
		return 0;
	}
#  ifdef DEBUG_STACKTRACE
	if(curFrame->debug)
		printf("( %p %p ) -> { %p %p }\n",curFrame->sp,curFrame->ra,nsp,nra);
	nextFrame->debug=curFrame->debug;
#  endif
	nextFrame->sp=nsp;
	nextFrame->ra=nra;
	curFrame->caller=nextFrame;
	return 1;
#endif
#if defined(__MIPSEL__) || defined(__MIPS__) /* we're running on PLATFORM_APERIOS */
	/* Have to scan through intructions being executed because stack pointer is not stored directly on the stack */
	machineInstruction * ins;
	const machineInstruction * INS_BASE=(const machineInstruction *)0x2000; // lowest valid memory address?
	if(curFrame==NULL)
		return 0;
	curFrame->caller=NULL;
	if(nextFrame==NULL || curFrame==NULL || curFrame->sp==NULL)
		return 0;
	
#ifdef __PIC__
	ins = reinterpret_cast<machineInstruction*>(curFrame->gp-curFrame->ra);
#else
	ins = curFrame->ra;
#endif
	// find previous return address
	for(; ins>=INS_BASE; ins--) {
		// gcc will always save the return address with the instruction
		//     sw ra, offset(sp)
		// 
		// the high word in this case is sw sp ra
		if ( ( *ins & 0xffff0000 ) == 0xafbf0000 )
		{
			// the low word is the offset from sp
			int offset = *ins & 0x000ffff;
			
			// in case things went horribly awry, don't deref the non-aligned ptr
			if (offset & 0x3)
				return 0;
			
			nra = *reinterpret_cast<machineInstruction**>((char*)curFrame->sp + offset);
			break; // now search for stack pointer
		}
		
		//it appears the aperios stub entry functions always begin with "ori  t0,ra,0x0"
		//if we hit one of these, return 0, because we can't unroll any more
		//(or at least, I don't know how it returns from these... there's no return statements!)
		if ( *ins  == 0x37e80000 ) {
#  ifdef DEBUG_STACKTRACE
			if(curFrame->debug)
				printf("( %p %p %p ) -> { kernel? }\n",curFrame->sp,curFrame->ra,curFrame->gp);
#  endif
			return 0;
		}
	}
	// find previous stack pointer
	for(; ins>=INS_BASE; ins--) {
		// gcc will always change the stack frame with the instruction
		//     addiu sp,sp,offset
		//
		// at the beginning of the function the offset will be negative since the stack grows 
		// from high to low addresses
		//
		// first check the high word which will be instruction + regs in this case (I-type)
		if ( ( *ins & 0xffff0000 ) == 0x27bd0000 ) {
			// the offset is in the low word. since we're finding occurrence at the start of the function,
			// it will be negative (increase stack size), so sign extend it
			int offset = ( *ins & 0x0000ffff ) | 0xffff0000;

			// in case things went horribly awry, don't deref the non-aligned ptr
			if (offset & 0x3)
				return 0;
			
			nsp = (char*)curFrame->sp - offset;
			break;
		}
	}
	
	
	if(ins>=INS_BASE) {
		if(nsp<=curFrame->sp) {
#ifdef __PIC__
			fprintf(stderr,"stacktrace::unrollStackFrame(sp=%p,ra=%p,gp=%p) directed to invalid next frame: (sp=%p,ra=%p,gp=%p)\n",curFrame->sp,(void*)curFrame->ra,(void*)curFrame->gp,nsp,nra,(void*)(reinterpret_cast<size_t*>(nsp)[4]));
#else
			fprintf(stderr,"stacktrace::unrollStackFrame(sp=%p,ra=%p) directed to invalid next frame: (sp=%p,ra=%p)\n",curFrame->sp,(void*)curFrame->ra,nsp,nra);
#endif
			return 0;
		}

#ifdef __PIC__
#  ifdef DEBUG_STACKTRACE
		if(curFrame->debug)
			printf("( %p %p %p ) -> { %p %p %p }\n",curFrame->sp,curFrame->ra,curFrame->gp,nsp,nra,reinterpret_cast<size_t*>(nsp)[4]);
		nextFrame->debug=curFrame->debug;
#  endif
		// I'm not actually sure this is a valid stop criteria, but in testing,
		// after this it seems to cross into some kind of kernel code.
		// (We get a really low gp (0x106), although a fairly normal nra, and then go bouncing
		// around in memory until we hit sp=0x80808080, ra=0x2700, which seems to be the 'real' last frame)
		//if(reinterpret_cast<size_t>(nra)>reinterpret_cast<size_t*>(nsp)[4])
		//return 0;
		//instead of this however, now we check for the ori t0,ra,0 statement, and reuse previous gp below
		
		nextFrame->sp=nsp;
		//not sure how valid this is either:
		if(reinterpret_cast<size_t>(nra)>reinterpret_cast<size_t*>(nsp)[4]) {
			nextFrame->gp = curFrame->gp;
		} else {
			nextFrame->gp = reinterpret_cast<size_t*>(nsp)[4]; // gp is stored 4 words from stack pointer
		}
		nextFrame->ra = nextFrame->gp-reinterpret_cast<size_t>(nra);
#else
#  ifdef DEBUG_STACKTRACE
		if(curFrame->debug)
			printf("( %p %p ) -> { %p %p }\n",curFrame->sp,curFrame->ra,nsp,nra);
		nextFrame->debug=curFrame->debug;
#  endif
		nextFrame->sp=nsp;
		nextFrame->ra=nra;
#endif /* __PIC__ */
		curFrame->caller=nextFrame;
		return 1;
	}
#ifdef __PIC__
#  ifdef DEBUG_STACKTRACE
	if(curFrame->debug)
		printf("( %p %p %p ) -> { %p %p --- }\n",curFrame->sp,curFrame->ra,curFrame->gp,nsp,nra);
#  endif
#else
#  ifdef DEBUG_STACKTRACE
	if(curFrame->debug)
		printf("( %p %p ) -> { %p %p }\n",curFrame->sp,curFrame->ra,nsp,nra);
#  endif
#endif
	return 0;
#endif
}

void getCurrentStackFrame(struct StackFrame* frame) {
	void** csp=NULL;
	machineInstruction* cra=NULL;

#ifdef __POWERPC__
	__asm __volatile__ ("mr %0,r1" : "=r"(csp) ); // get the current stack pointer
	__asm __volatile__ ("mflr %0" : "=r"(cra) );  // get the current return address
#endif /* __POWERPC__ */
	
#if defined(__MIPSEL__) || defined(__MIPS__)
#ifdef __PIC__
	size_t cgp=0;
	__asm __volatile__ ("move %0,$gp" : "=r"(cgp) ); //get the gp register so we can compute link addresses
#endif /* __PIC__ */
	__asm __volatile__ ("move %0,$sp" : "=r"(csp) ); // get the current stack pointer
	__asm __volatile__ ("jal readepc; nop; readepc: move %0,$ra" : "=r"(cra) ); // get the current return address
#endif /* __MIPSEL__ */
	
#if defined(__i386__)
	__asm __volatile__ ("movl %%ebp,%0" : "=m"(csp) ); // get the caller's stack pointer
	csp++; //go back one to really be a stack pointer
	//__asm __volatile__ ("movl (%%esp),%0" : "=r"(cra) ); // get the caller's address
	cra=*((machineInstruction**)csp);
	csp=((void***)csp)[-1]+1;
#endif /* __i386__ */

// basically the same as i386, but movq instead of movl, and rbp instead of ebp
#if defined(__x86_64__) || defined(__amd64__)
	__asm __volatile__ ("movq %%rbp,%0" : "=m"(csp) ); // get the caller's stack pointer
	csp++; //go back one to really be a stack pointer
	//__asm __volatile__ ("movq (%%rsp),%0" : "=r"(cra) ); // get the caller's address
	cra=*((machineInstruction**)csp);
	csp=((void***)csp)[-1]+1;
#endif /* amd64/x86_64 */

	frame->sp=csp;
#if defined(__PIC__) && (defined(__MIPSEL__) || defined(__MIPS__))
	frame->ra=cgp-reinterpret_cast<size_t>(cra);
	frame->gp=cgp;
#else
	frame->ra=cra;
#endif /* __PIC__ */

#if !defined(__i386__) && !defined(__x86_64__) && !defined(__amd64__)
	//with ia-32 it was more convenient to directly provide caller, so don't need to unroll
	//otherwise we actually want to return *caller's* frame, so unroll once
	unrollStackFrame(frame,frame);
#endif /* not __i386__ */
}

void freeStackTrace(struct StackFrame* frame) {
	while(frame!=NULL) {
		struct StackFrame * next=frame->caller;
		free(frame);
		if(frame==next)
			return;
		frame=next;
	}
}

struct StackFrame* allocateStackTrace(unsigned int size) {
	struct StackFrame * frame=NULL;
	while(size--!=0) {
		struct StackFrame * prev = (struct StackFrame *)malloc(sizeof(struct StackFrame));
		prev->caller=frame;
		frame=prev;
	}
	return frame;
}


struct StackFrame * recordStackTrace(unsigned int limit/*=-1U*/, unsigned int skip/*=0*/) {
	struct StackFrame cur;
#ifdef DEBUG_STACKTRACE
	cur.debug=0;
#endif
	if(limit==0)
		return NULL;
	getCurrentStackFrame(&cur);
	for(; skip!=0; skip--)
		if(!unrollStackFrame(&cur,&cur))
			return NULL;
	struct StackFrame * prev = (struct StackFrame *)malloc(sizeof(struct StackFrame));
#ifdef DEBUG_STACKTRACE
	prev->debug=0;
#endif
	unrollStackFrame(&cur,prev); //unroll once more for the current frame
	for(; limit!=0; limit--) {
		struct StackFrame * next = (struct StackFrame *)malloc(sizeof(struct StackFrame));
#ifdef DEBUG_STACKTRACE
		next->debug=0;
#endif
		if(!unrollStackFrame(prev,next)) {
			// reached end of trace
			free(next);
			prev->caller=NULL; //denotes end was reached
			return cur.caller;
		}
		prev=next;
	}
	// reaching here implies limit was reached
	prev->caller=prev; //denotes limit was reached
	return cur.caller;
}

struct StackFrame * recordOverStackTrace(struct StackFrame* frame, unsigned int skip) {
	struct StackFrame cur;
#ifdef DEBUG_STACKTRACE
	cur.debug=0;
#endif
	if(frame==NULL)
		return frame;
	getCurrentStackFrame(&cur);
	for(; skip!=0; skip--)
		if(!unrollStackFrame(&cur,&cur))
			return frame;
	unrollStackFrame(&cur,frame); //unroll once more for the current frame
	for(; frame->caller!=NULL && frame->caller!=frame; frame=frame->caller) {
		cur.caller=frame->caller; //don't lose remainder of free list if we hit the end
		if(!unrollStackFrame(frame,frame->caller))
			return cur.caller; // reached end of trace
	}
	// reaching here implies limit was reached
	frame->caller=frame; //denotes limit was reached
	return cur.caller;
}

#if defined(HAVE_BFD) && HAVE_BFD!=0
bfd * abfd = NULL;
static asymbol **syms;		/* Symbol table.  */
#endif

//! attempts to read symbol information and displays stack trace header
void beginDisplay() {
#if defined(HAVE_BFD) && HAVE_BFD!=0
#  ifdef __APPLE__
	const char* consolation=", try 'atos' to make human-readable backtrace";
#  else
	const char* consolation=", try addr2line or trace_lookup";
#  endif
	if(bfd_objfile==NULL) {
		fprintf(stderr,"No symbols are loaded%s.\n",consolation);
		return;
	}
	abfd = bfd_openr(bfd_objfile, NULL);
	if(abfd==NULL) {
		bfd_error_type err=bfd_get_error();
		fprintf(stderr,"Can't open object file `%s': %s\n",bfd_objfile,bfd_errmsg(err));
		fprintf(stderr,"stacktrace::beginDisplay() failed%s.\n",consolation);
		return;
	}
	if(bfd_check_format (abfd, bfd_archive)) {
		bfd_error_type err=bfd_get_error();
		fprintf(stderr,"Can't get addresses from archive `%s': %s\n",bfd_objfile,bfd_errmsg(err));
		if(!bfd_close(abfd))
			fprintf(stderr,"Can't close object file `%s': %s\n",bfd_objfile,bfd_errmsg(bfd_get_error()));
		fprintf(stderr,"stacktrace::beginDisplay() failed%s.\n",consolation);
		abfd=NULL;
		return;
	}
	{
		char **matching;
		if(!bfd_check_format_matches(abfd, bfd_object, &matching)) {
			bfd_error_type err=bfd_get_error();
			fprintf(stderr,"Can't get archive `%s' format doesn't match bfd_object: %s\n",bfd_objfile,bfd_errmsg(err));
			if (err == bfd_error_file_ambiguously_recognized) {
				char** p=matching;
				fprintf(stderr,"Matching formats:");
				while (*p)
					fprintf (stderr, " %s", *p++);
				fputc ('\n', stderr);
				free (matching);
			}
			if(!bfd_close(abfd))
				fprintf(stderr,"Can't close object file `%s': %s\n",bfd_objfile,bfd_errmsg(bfd_get_error()));
			fprintf(stderr,"stacktrace::beginDisplay() failed%s.\n",consolation);
			abfd=NULL;
		}
	}
	{
		long symcount;
		unsigned int size;
		
		if ((bfd_get_file_flags (abfd) & HAS_SYMS) == 0)
			return;
		
		symcount = bfd_read_minisymbols (abfd, FALSE, (void *) &syms, &size);
		if (symcount == 0)
			symcount = bfd_read_minisymbols (abfd, TRUE /* dynamic */, (void *) &syms, &size);
		
		if (symcount < 0) {
			bfd_error_type err=bfd_get_error();
			fprintf(stderr,"Can't read minisymbols from `%s': %s\n",bfd_get_filename (abfd),bfd_errmsg(err));
			if(!bfd_close(abfd))
				fprintf(stderr,"Can't close object file `%s': %s\n",bfd_objfile,bfd_errmsg(bfd_get_error()));
			fprintf(stderr,"stacktrace::beginDisplay() failed%s.\n",consolation);
			abfd=NULL;
		}
		if(abfd!=NULL) {
			fprintf(stderr,"Stack trace:\n");
			return;
		}
	}
	fprintf(stderr,"Stack trace:");
#else
# ifdef PLATFORM_APERIOS
	fprintf(stderr,"Run trace_lookup:");
#  else
#    ifdef __APPLE__
	const char* consolation=", try 'atos' to make human-readable backtrace";
#    else
	const char* consolation=", try addr2line or trace_lookup";
#    endif
	fprintf(stderr,"libBFD unavaible%s:\n",consolation);
	fprintf(stderr,"Stack trace:");
#  endif
#endif
}

#if defined(HAVE_BFD) && HAVE_BFD!=0
//! from binutils' budemangle
/*! Wrapper around cplus_demangle.	 Strips leading underscores and
 *  other such chars that would otherwise confuse the demangler.	 */
char * demangle (const char *name) {
	char *res, *alloc;
	const char *pre, *suf;
	size_t pre_len;

	if (abfd != NULL && bfd_get_symbol_leading_char (abfd) == name[0])
		++name;

	/* This is a hack for better error reporting on XCOFF, PowerPC64-ELF
		 or the MS PE format.	 These formats have a number of leading '.'s
		 on at least some symbols, so we remove all dots to avoid
		 confusing the demangler.	 */
	pre = name;
	while (*name == '.')
		++name;
	pre_len = name - pre;

	alloc = NULL;
	suf = strchr (name, '@');
	if (suf != NULL)
		{
			alloc = xmalloc (suf - name + 1);
			memcpy (alloc, name, suf - name);
			alloc[suf - name] = '\0';
			name = alloc;
		}

	res = cplus_demangle (name, DMGL_ANSI | DMGL_PARAMS);
	if (res != NULL)
		{
			/* Now put back any suffix, or stripped dots.	 */
			if (pre_len != 0 || suf != NULL)
				{
					size_t len;
					size_t suf_len;
					char *final;

					if (alloc != NULL)
						free (alloc);

					len = strlen (res);
					if (suf == NULL)
						suf = res + len;
					suf_len = strlen (suf) + 1;
					final = xmalloc (pre_len + len + suf_len);

					memcpy (final, pre, pre_len);
					memcpy (final + pre_len, res, len);
					memcpy (final + pre_len + len, suf, suf_len);
					free (res);
					res = final;
				}

			return res;
		}

	if (alloc != NULL)
		free (alloc);

	return xstrdup (pre);
}

struct AddrLookupInfo {
	bfd_vma addr;
	const char *filename;
	const char *functionname;
	unsigned int line;
	bfd_boolean found;
};

/*! Look for an address in a section. This is called via bfd_map_over_sections.	 */
static void stackTraceFindAddress (bfd *fabfd, asection *section, void *data) {
	struct AddrLookupInfo * info=(struct AddrLookupInfo *)data;
	bfd_vma vma;
	bfd_size_type size;
	
#ifdef bfd_get_section_size
	printf("searching %p: %s %x-%x\n",info->addr,section->name,bfd_get_section_vma (fabfd, section),bfd_get_section_vma (fabfd, section)+bfd_get_section_size (section));
#else
	printf("searching %p: %s %x-%x\n",info->addr,section->name,bfd_get_section_vma (fabfd, section),bfd_get_section_vma (fabfd, section)+bfd_get_section_size_before_reloc (section));
#endif
	
	if (info->found)
		return;
	if ((bfd_get_section_flags (fabfd, section) & SEC_ALLOC) == 0)
		return;

	vma = bfd_get_section_vma (fabfd, section);
	if (info->addr < vma)
		return;
#ifdef bfd_get_section_size
	size = bfd_get_section_size (section);
#else
	size = bfd_get_section_size_before_reloc (section);
#endif
	if (info->addr >= vma + size)
		return;
	
	info->found = bfd_find_nearest_line (fabfd, section, syms, info->addr - vma, &info->filename, &info->functionname, &info->line);
	printf("found %d\n",info->found);
}
#endif /* HAVE_BFD */

#if defined(HAVE_BFD) && HAVE_BFD!=0
void displayStackFrame(unsigned int depth, const struct StackFrame* frame) {
	if(abfd!=NULL) {
		struct AddrLookupInfo info={(bfd_vma)frame->ra,NULL,NULL,0,FALSE};
		asection *p;
		for (p = abfd->sections; p != NULL; p = p->next)
			stackTraceFindAddress(abfd, p, &info);
		//bfd_map_over_sections (abfd, stackTraceFindAddress, &info);
		if (!info.found) {
			printf("%u: %p ????\n",depth,frame->ra);
		} else {
			const char *name=info.functionname;
			char *alloc = NULL;
			if (name == NULL || *name == '\0')
				name = "??";
			else {
				alloc = demangle(name);
				name = alloc;
			}
			
			printf ("%u: %p %s", depth, frame->ra, name);
			if (alloc != NULL) {
				free (alloc);
				alloc=NULL;
			}
	
			if(info.filename)
				printf(" (%s:%u)\n", info.filename, info.line);
			else
				printf(" (??:?)\n");
		}
		return;
	}
	fprintf(stderr," %p",frame->ra);
}
#else
void displayStackFrame(unsigned int ST_UNUSED(depth), const struct StackFrame* frame) {
	ST_BODY_UNUSED(depth);
	fprintf(stderr," %p",(void*)frame->ra);
}
#endif

//! releases symbol information used during display
void completeDisplay(int isend) {
#if defined(HAVE_BFD) && HAVE_BFD!=0
	if (syms != NULL) {
		free(syms);
		syms = NULL;
	}
	if(abfd!=NULL) {
		if(!bfd_close(abfd)) {
			bfd_error_type err=bfd_get_error();
			fprintf(stderr,"Can't close object file `%s': %s\n",bfd_get_filename (abfd),bfd_errmsg(err));
			fprintf(stderr,"stacktrace::completeDisplay() failed");
		}
		abfd=NULL;
		if(!isend)
			fprintf(stderr," ...\n");
		return;
	}
#endif
	if(!isend)
		fprintf(stderr," ...");
	fprintf(stderr,"\n");
}

void displayCurrentStackTrace(unsigned int limit/*=-1U*/, unsigned int skip/*=0*/) {
	struct StackFrame cur;
#ifdef DEBUG_STACKTRACE
	cur.debug=0;
#endif
	unsigned int i;
	int more;
	if(limit==0)
		return;
	getCurrentStackFrame(&cur);
	//printf(" initial (%p\t%p\t%p)\n",cur.ra,cur.sp,*(void**)cur.sp);
	beginDisplay();
	for(; skip!=0; skip--) {
		if(!unrollStackFrame(&cur,&cur)) {
			completeDisplay(1);
			return;
		}
		//printf(" skip (%p\t%p\t%p)\n",cur.ra,cur.sp,*(void**)cur.sp);
	}
	for(i=0; (more=unrollStackFrame(&cur,&cur)) && i<limit; i++) {
		//printf(" out (%p\t%p\t%p)\n",cur.ra,cur.sp,*(void**)cur.sp);
		displayStackFrame(i,&cur);
	}
	completeDisplay(!more);
}

void displayStackTrace(const struct StackFrame* frame) {
	int i;
	beginDisplay();
	for(i=0; frame!=NULL && frame->caller!=frame; i++) {
		displayStackFrame(i,frame);
		frame=frame->caller;
	}
	if(frame!=NULL)
		displayStackFrame(i+1,frame);
	completeDisplay(frame==NULL);
}


#ifdef __cplusplus
}
#endif /* __cplusplus */

/*! @file
 * @brief Implements functionality for performing stack traces
 * @author ejt (Generalized and implementation for non-MIPS platforms)
 * @author Stuart Scandrett (original inspiration, Aperios/MIPS stack operations)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.14.2.1 $
 * $State: Exp $
 * $Date: 2009/02/08 03:34:42 $
 */
