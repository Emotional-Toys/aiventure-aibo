################ PROJECT MAKEFILE #################
############# $Name: tekkotsu-4_0-branch $ ###############
############### $Revision: 1.155.2.1 $ #################
########## $Date: 2009/02/08 03:34:44 $ ###########

# Make sure the default target is 'all' by listing it first
all:

###################################################
##             ENVIRONMENT SETUP                 ##
###################################################
TEKKOTSU_ENVIRONMENT_CONFIGURATION?=$(shell pwd | sed 's/ /\\ /g')/Environment.conf
ifneq ($(filter sim% tekkotsu%,$(MAKECMDGOALS)),)
	TEKKOTSU_TARGET_PLATFORM:=PLATFORM_LOCAL
	ifneq ($(filter sim-%,$(MAKECMDGOALS)),)
		TEKKOTSU_TARGET_MODEL:=$(patsubst sim-%,TGT_%,$(filter sim-%,$(MAKECMDGOALS)))
	endif
	ifneq ($(filter tekkotsu-%,$(MAKECMDGOALS)),)
		TEKKOTSU_TARGET_MODEL:=$(patsubst tekkotsu-%,TGT_%,$(filter tekkotsu-%,$(MAKECMDGOALS)))
	endif
endif
include $(TEKKOTSU_ENVIRONMENT_CONFIGURATION)

ifeq ($(MAKELEVEL),0)
  $(shell echo "  ** Project targeting $(TEKKOTSU_TARGET_MODEL) for build on $(TEKKOTSU_TARGET_PLATFORM) **" > /dev/tty)
  $(shell echo "  ** TEKKOTSU_DEBUG is $(if $(TEKKOTSU_DEBUG),ON: $(TEKKOTSU_DEBUG),OFF) **" > /dev/tty)
  $(shell echo "  ** TEKKOTSU_OPTIMIZE is $(if $(TEKKOTSU_DEBUG),DISABLED BY DEBUG,$(if $(TEKKOTSU_OPTIMIZE),ON: $(TEKKOTSU_OPTIMIZE),OFF)) **" > /dev/tty)
endif

#sanity checks
$(if $(shell [ -d "$(TEKKOTSU_ROOT)" ] || echo "not found"),$(error Could not access TEKKOTSU_ROOT: $(TEKKOTSU_ROOT)))
ifeq ($(filter clean%,$(MAKECMDGOALS)),)
  ifeq ($(TEKKOTSU_TARGET_PLATFORM),PLATFORM_APERIOS)
    $(if $(shell [ -d "$(OPENRSDK_ROOT)" ] || echo "not found"),$(error OPEN-R SDK not found at '$(OPENRSDK_ROOT)', check installation.))
    $(if $(shell [ -d "$(OPENRSDK_ROOT)/OPEN_R" ] || echo "not found"),$(error OPEN-R SDK header files missing, check installation.))
  endif
  $(if $(shell $(CXX) -v > /dev/null 2>&1 || echo "not found"),$(error C++ compiler not found at '$(CXX)', check installation.))
endif

$(shell mkdir -p $(PROJ_BD))


#############  MAKEFILE VARIABLES  ################

# Would you like some more compiler flags?  We like lots of warnings.
# There are some files with exceptions to these flags - MMCombo*.cc
# need to have optimizations turned off, and several TinyFTPD sources
# have -Weffc++ and -DOPENR_DEBUG turned off.  If you want to modify
# these exceptions, look in the middle of the 'Makefile Machinery'
# section. (grep/search for the file name)

ifeq ($(TEKKOTSU_TARGET_PLATFORM),PLATFORM_APERIOS)
  PLATFORM_FLAGS:= \
	  -isystem $(OPENRSDK_ROOT)/OPEN_R/include/MCOOP \
	  -isystem $(OPENRSDK_ROOT)/OPEN_R/include/R4000 \
	  -isystem $(OPENRSDK_ROOT)/OPEN_R/include \
	  -isystem $(TEKKOTSU_ROOT)/aperios/include \
	  $(if $(TEKKOTSU_DEBUG),-DOPENR_DEBUG,) -DLOADFILE_NO_MMAP \
	  $(shell $(TEKKOTSU_ROOT)/aperios/bin/xml2-config --cflags)
  LDFLAGS:=$(LDFLAGS) -lObjectComm -lOPENR  -lInternet -lantMCOOP -lERA201D1 \
	  $(shell $(TEKKOTSU_ROOT)/aperios/bin/xml2-config --libs) -ljpeg -lpng -lregex
else
  PLATFORM_FLAGS:=$(shell xml2-config --cflags) -isystem /usr/include/libpng12 \
	 $(shell if [ -d "$(ICE_ROOT)" ] ; then echo "-DHAVE_ICE -I$(ICE_ROOT)/include"; fi)
  #enable -fPIC if we are building shared libraries on x86_64/amd64
  ifneq ($(filter __amd64__ __x86_64__,$(shell g++ $(CXXFLAGS) -dM -E - < /dev/null)),)
    ifneq ($(suffix $(LIBTEKKOTSU)),.a)
      PLATFORM_FLAGS:=$(PLATFORM_FLAGS) -fPIC
    endif
  endif
  LDFLAGS:=$(LDFLAGS) $(shell xml2-config --libs) $(if $(shell locate librt.a 2> /dev/null),-lrt) \
          -lreadline -lncurses -ljpeg -lpng12 -lpthread \
          $(if $(HAVE_ICE),-L$(ICE_ROOT)/lib -lIce -lGlacier2 -lIceUtil) \
          $(if $(findstring Darwin,$(shell uname)),-bind_at_load -framework Quicktime -framework Carbon)
endif

ifeq ($(MAKELEVEL),0)
  export ENV_CXXFLAGS:=$(CXXFLAGS)
endif
unexport CXXFLAGS
CXXFLAGS:= \
	$(if $(TEKKOTSU_DEBUG),$(TEKKOTSU_DEBUG),$(TEKKOTSU_OPTIMIZE)) \
	-pipe -ffast-math -fno-common \
	-Wall -Wshadow -Wlarger-than-200000 -Wpointer-arith -Wcast-qual \
	-Woverloaded-virtual -Weffc++ -Wdeprecated -Wnon-virtual-dtor \
	-fmessage-length=0 \
	-I. -I$(TEKKOTSU_ROOT) -I$(TEKKOTSU_ROOT)/Shared/newmat -I$(PROJ_BD) \
	-isystem $(TEKKOTSU_ROOT)/Shared/jpeg-6b \
	-D$(TEKKOTSU_TARGET_PLATFORM)  -D$(TEKKOTSU_TARGET_MODEL) \
	$(PLATFORM_FLAGS) $(ENV_CXXFLAGS)

INCLUDE_PCH=$(if $(TEKKOTSU_PCH),-include $(TK_BD)/$(TEKKOTSU_PCH))

TEKKOTSU_VERSION=Tekkotsu 4.0 CVS

###################################################
##              SOURCE CODE LIST                 ##
###################################################

# Find all of the source files: (except temp files in build directory,
# or platform-specific files in either aperios or sim)
# You shouldn't need to change anything here unless you want to add
# external libraries
SRCSUFFIX=.cc
SRCS:=$(patsubst ./%,%,$(shell find . -name "[^.]*$(SRCSUFFIX)" -or -path "*$(PROJECT_BUILDDIR)" \! -prune -or -name "templates" \! -prune -or -name "aperios" \! -prune -or -name "local" \! -prune))
FSMS:=$(patsubst ./%,%,$(shell find . -name "[^.]*\.fsm" -or -path "*$(PROJECT_BUILDDIR)" \! -prune -or -name "templates" \! -prune -or -name "aperios" \! -prune -or -name "local" \! -prune))

# We can also link in third-party libraries
USERLIBS:= $(TK_BD)/$(LIBTEKKOTSU) \
           $(TK_LIB_BD)/Shared/newmat/libnewmat$(suffix $(LIBTEKKOTSU)) \
           $(TK_LIB_BD)/Motion/roboop/libroboop$(suffix $(LIBTEKKOTSU)) \

###################################################
##             MAKEFILE MACHINERY                ##
###################################################
# Hopefully, you shouldn't have to change anything down here,
# except one or two little things ;)

.PHONY: all compile clean cleanDeps cleanProj checkLibs testLibs buildLibs sim tekkotsu

sim all update install ftpupdate: %: checkLibs compile-%

#DEFTARGET is set when using XCode to trigger 'update' instead of 'all'
ifeq ($(TEKKOTSU_DEFTARGET),)
compile-all: compile
else
compile-all: compile-$(TEKKOTSU_DEFTARGET) ;
endif

# Don't want to try to remake this - give an error if not found
$(TEKKOTSU_ROOT)/project/Environment.conf:
	@echo "Could not find Environment file - check the default project directory still exists"
	@exit 1

checkLibs: $(if $(TEKKOTSU_ALWAYS_BUILD),buildLibs,testLibs)

$(TK_BD)/$(TEKKOTSU_PCH).gch $(TK_BD)/%.o: checkLibs

ifeq ($(TEKKOTSU_ALWAYS_BUILD),)
$(TK_BD)/$(LIBTEKKOTSU): testLibs

else
$(TK_BD)/$(LIBTEKKOTSU): buildLibs

endif

testLibs:
	@broken=false; \
	for x in $(USERLIBS) ; do \
		if [ \! -r "$$x" ] ; then \
			echo "Could not read framework library '$$x'"; \
			broken=true; \
		fi; \
	done; \
	if $$broken ; then \
		echo "It appears that the framework itself has not been compiled."; \
		echo "TEKKOTSU_ROOT = $(TEKKOTSU_ROOT)"; \
		echo "TK_BD = $(TK_BD)"; \
		echo "Press enter to compile it now, ctl-C to cancel."; \
		read; \
		export TEKKOTSU_ENVIRONMENT_CONFIGURATION=$(TEKKOTSU_ENVIRONMENT_CONFIGURATION); \
		$(MAKE) TEKKOTSU_TARGET_MODEL=$(TEKKOTSU_TARGET_MODEL) TEKKOTSU_TARGET_PLATFORM=$(TEKKOTSU_TARGET_PLATFORM) -C "$(TEKKOTSU_ROOT)" compile ; \
	fi;

buildLibs:
	@echo "Updating framework build...";
	@export TEKKOTSU_ENVIRONMENT_CONFIGURATION=$(TEKKOTSU_ENVIRONMENT_CONFIGURATION); \
	$(MAKE) TEKKOTSU_TARGET_MODEL=$(TEKKOTSU_TARGET_MODEL) TEKKOTSU_TARGET_PLATFORM=$(TEKKOTSU_TARGET_PLATFORM) -C "$(TEKKOTSU_ROOT)" compile ; 

ifeq ($(TEKKOTSU_TARGET_PLATFORM),PLATFORM_APERIOS)
include aperios/Makefile.aperios
else
include local/Makefile.local
endif

# Sort by modification date
SRCS:=$(shell ls -t $(SRCS) $(filter %$(SRCSUFFIX).fsm,$(FSMS)))

# The object file for each of the source files
OBJS:=$(addprefix $(PROJ_BD)/,$(SRCS:$(SRCSUFFIX)=.o))
OBJS:=$(OBJS:$(SRCSUFFIX).fsm=-fsm.o)

# list of all source files of all components, sorted to remove
# duplicates.  This gives us all the source files which we care about,
# all in one place.
DEPENDS:=$(addprefix $(PROJ_BD)/,$(SRCS:$(SRCSUFFIX)=.d))
DEPENDS:=$(DEPENDS:$(SRCSUFFIX).fsm=-fsm.d)

SRCS:=$(patsubst %$(SRCSUFFIX).fsm,$(PROJ_BD)/%-fsm$(SRCSUFFIX),$(SRCS))

EMPTYDEPS:=$(shell find $(PROJ_BD) -type f -name "*\.d" -size 0 -print -exec rm \{\} \;)
ifneq ($(EMPTYDEPS),)
EMPTYDEPS:=$(shell echo "Empty dependency files detected: $(EMPTYDEPS)" > /dev/tty)
endif

ifeq ($(filter clean% docs newstick,$(MAKECMDGOALS)),)
-include $(DEPENDS)
ifeq ($(TEKKOTSU_TARGET_PLATFORM),PLATFORM_APERIOS)
-include $(PROJ_BD)/aperios/aperios.d
else
-include $(PROJ_BD)/local/local.d
endif
endif

$(patsubst %.fsm,$(PROJ_BD)/%,$(filter %.h.fsm,$(FSMS))) : $(PROJ_BD)/%.h : %.h.fsm
	@mkdir -p $(dir $@)
	@echo "Generating state machine: tools/stateparser $^"
	@$(TEKKOTSU_ROOT)/tools/stateparser "$^" "$@"

$(patsubst %$(SRCSUFFIX).fsm,$(PROJ_BD)/%-fsm$(SRCSUFFIX),$(filter %$(SRCSUFFIX).fsm,$(FSMS))) : $(PROJ_BD)/%-fsm$(SRCSUFFIX) : %$(SRCSUFFIX).fsm
	@mkdir -p $(dir $@)
	@echo "Generating state machine: tools/stateparser $^"
	@$(TEKKOTSU_ROOT)/tools/stateparser "$^" "$@"

$(filter %-fsm.d,$(DEPENDS)) : $(PROJ_BD)/%.d : $(PROJ_BD)/%$(SRCSUFFIX)

$(filter-out %-fsm.d,$(DEPENDS)) : $(PROJ_BD)/%.d : %$(SRCSUFFIX) | $(patsubst %.fsm,$(PROJ_BD)/%,$(filter %.h.fsm,$(FSMS)))

$(PROJ_BD)/%.d : 
	@mkdir -p $(dir $@)
	@src=$(if $(filter %-fsm.d,$@),$(@:.d=$(SRCSUFFIX)),$*$(SRCSUFFIX)); \
	echo "$@..." | sed 's@$(PROJ_BD)/@Generating @'; \
	$(CXX) $(CXXFLAGS) -MP -MG -MT "$@" -MT "$(@:.d=.o)" -MM "$$src" > "$@" || (rm -f "$@"; exit 1)

%.h :
	@if [ "$(notdir $@)" = "def.h" -o "$(notdir $@)" = "entry.h" ] ; then \
		echo "WARNING: You shouldn't be seeing this message.  Report that you did." ; \
		echo "         Try a clean recompile." ; \
	fi;
	@echo "ERROR: Seems to be a missing header file '$@'...";
	@echo "       Someone probably forgot to check a file into CVS.";
	@echo "       I'll try to find where it's being included from:";
	@echo "       if this was a file you recently deleted, just make again after this completes. (will update dependency files)";
	@find . -name "*.h" -exec grep -H "$(notdir $@)" \{\} \; ;
	@find . -name "*.cc" -exec grep -H "$(notdir $@)" \{\} \; ;
	@find $(TEKKOTSU_ROOT) -name "*.h" -exec grep -H "$(notdir $@)" \{\} \; ;
	@find $(TEKKOTSU_ROOT) -name "*.cc" -exec grep -H "$(notdir $@)" \{\} \; ;
	@find $(TK_BD) -name "*.d" -exec grep -qH "$(notdir $@)" \{\} \; -exec rm \{\} \; ;
	@find $(PROJ_BD) -name "*.d" -exec grep -qH "$(notdir $@)" \{\} \; -exec rm \{\} \; ;
	@exit 1

#don't try to make random files via this implicit chain
%:: %.o
	echo "I don't know anything about ‘$*’";
	exit 1;

# Don't locally rebuild framework files
# (sometimes platform_local targets want to doubly-compile the target specific object files... might be a make bug)
$(TK_BD)/%.o: checkLibs
	
$(PROJ_BD)/%.o: $(if $(TEKKOTSU_PCH),$(TK_BD)/$(TEKKOTSU_PCH).gch)
	@mkdir -p $(dir $@)
	@src=$(if $(filter %-fsm.o,$@),$(@:.o=$(SRCSUFFIX)),$*$(SRCSUFFIX)); \
	if [ -r dist_hosts.txt ] ; then \
		echo "Adding $@ to job list"; \
		echo "$(CXX) $(CXXFLAGS) -o $@ -c $$src > $(PROJ_BD)/$*.log 2>&1; retval=\$$?; cat $(PROJ_BD)/$*.log | $(FILTERSYSWARN) | $(COLORFILT); test \$$retval -eq 0;" >> $(PROJ_BD)/joblist.txt ; \
		touch $@; \
		echo "$@" >> $(PROJ_BD)/touched.txt ; \
	else \
		echo "Compiling $$src..."; \
		$(CXX) $(CXXFLAGS) $(INCLUDE_PCH) -o $@ -c $$src > $(PROJ_BD)/$*.log 2>&1; \
		retval=$$?; \
		cat $(PROJ_BD)/$*.log | $(FILTERSYSWARN) | $(COLORFILT) | $(TEKKOTSU_LOGVIEW); \
		test $$retval -eq 0; \
	fi;

$(PROJ_BD)/project.a: $(OBJS)
	@echo "Linking project files..."
	@printf "$@ <- "; echo "$(OBJS)" | sed 's@$(PROJ_BD)/@@g';
	@rm -f $@;
	@$(AR) $@ $(OBJS);
	@$(AR2) $@

clean: cleanProj
	$(if $(TEKKOTSU_ALWAYS_BUILD),$(MAKE) -C $(TEKKOTSU_ROOT) $@)

cleanProj:
	@printf "Cleaning all ~ files corresponding to project .cc files..."
	@rm -f $(addsuffix ~,$(PROJ_SRCS)) $(PROJ_SRCS:$(SRCSUFFIX)=.h~)
	@printf "done.\n"
	rm -rf $(PROJECT_BUILDDIR)
	rm -f $(INSTALL_BINS)
	for dir in `ls aperios` ; do \
		if [ "$$dir" = "CVS" ] ; then continue; fi; \
		if [ "$$dir" = ".svn" ] ; then continue; fi; \
		if [ -f "aperios/$$dir" ] ; then continue; fi; \
		rm -f "aperios/$$dir/$${dir}Stub.h" "aperios/$$dir/$${dir}Stub.cc" "aperios/$$dir/def.h" "aperios/$$dir/entry.h" ; \
	done

cleanDeps:
	@echo "Cleaning all .d files from build directory..."
	find $(PROJECT_BUILDDIR) -name "*.d" -exec rm \{\} \;
	@echo "done."
	$(if $(TEKKOTSU_ALWAYS_BUILD),$(MAKE) -C $(TEKKOTSU_ROOT) $@)
