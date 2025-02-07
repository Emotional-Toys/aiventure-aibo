############### FRAMEWORK MAKEFILE ################
############# $Name: tekkotsu-4_0-branch $ ###############
############### $Revision: 1.72.2.1 $ #################
########## $Date: 2009/02/08 03:34:39 $ ###########

# Make sure the default target is 'all' by listing it first
all:


###################################################
##             ENVIRONMENT SETUP                 ##
###################################################
# Use the default project Environment.conf if none has been
# specified.  If a project initiated make, it should have
# defined this variable for us to use its configuration.
TEKKOTSU_ROOT:=$(shell pwd | sed 's/ /\\ /g')
TEKKOTSU_ENVIRONMENT_CONFIGURATION?=$(TEKKOTSU_ROOT)/project/Environment.conf
include $(shell echo "$(TEKKOTSU_ENVIRONMENT_CONFIGURATION)" | sed 's/ /\\ /g')

ifeq ($(MAKELEVEL),0)
INDENT=#empty string just to tokenize leading whitespace removal from the *actual* indentation
  $(shell echo "  ** Targeting $(TEKKOTSU_TARGET_MODEL) for build on $(TEKKOTSU_TARGET_PLATFORM) **" > /dev/tty)
  $(shell echo "  ** TEKKOTSU_DEBUG is $(if $(TEKKOTSU_DEBUG),ON: $(TEKKOTSU_DEBUG),OFF) **" > /dev/tty)
  $(shell echo "  ** TEKKOTSU_OPTIMIZE is $(if $(TEKKOTSU_DEBUG),DISABLED BY DEBUG,$(if $(TEKKOTSU_OPTIMIZE),ON: $(TEKKOTSU_OPTIMIZE),OFF)) **" > /dev/tty)
endif

#sanity checks
ifeq ($(filter clean% docs alldocs,$(MAKECMDGOALS)),)
  ifeq ($(TEKKOTSU_TARGET_PLATFORM),PLATFORM_APERIOS)
    $(if $(shell [ -d "$(OPENRSDK_ROOT)" ] || echo "not found"),$(error OPEN-R SDK not found at '$(OPENRSDK_ROOT)', check installation.))
    $(if $(shell [ -d "$(OPENRSDK_ROOT)/OPEN_R" ] || echo "not found"),$(error OPEN-R SDK header files missing, check installation.))
  endif
  $(if $(shell $(CXX) -v > /dev/null 2>&1 || echo "not found"),$(error C++ compiler not found at '$(CXX)', check installation.))
endif

$(shell mkdir -p $(TK_BD))


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
	  -isystem aperios/include \
	  $(if $(TEKKOTSU_DEBUG),-DOPENR_DEBUG,) -DLOADFILE_NO_MMAP \
	  $(shell aperios/bin/xml2-config --cflags)
else
  PLATFORM_FLAGS:=$(shell xml2-config --cflags) -isystem /usr/include/libpng12 \
	$(shell if [ -d "$(ICE_ROOT)" ] ; then echo "-DHAVE_ICE -I$(ICE_ROOT)/include"; fi) \
	$(shell if [ -d "$(CWIID_ROOT)" ] ; then echo "-DCWIID_ICE -I$(CWIID_ROOT)/include"; fi)
  #enable -fPIC if we are building shared libraries on x86_64/amd64
  ifneq ($(filter __amd64__ __x86_64__,$(shell g++ $(CXXFLAGS) -dM -E - < /dev/null)),)
    ifneq ($(suffix $(LIBTEKKOTSU)),.a)
      PLATFORM_FLAGS:=$(PLATFORM_FLAGS) -fPIC
    endif
  endif
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
	-I$(TEKKOTSU_ROOT) -I$(TEKKOTSU_ROOT)/Shared/newmat \
	-D$(TEKKOTSU_TARGET_PLATFORM)  -D$(TEKKOTSU_TARGET_MODEL) \
	$(PLATFORM_FLAGS) $(ENV_CXXFLAGS)

INCLUDE_PCH=$(if $(TEKKOTSU_PCH),-include $(TK_BD)/$(TEKKOTSU_PCH))


###################################################
##              SOURCE CODE LIST                 ##
###################################################

# Find all of the source files: (except temp files in build directory)
# You shouldn't need to change anything here unless you want to add
# external libraries or new directories for the search
SRCSUFFIX:=.cc
SRC_DIRS:=Behaviors DualCoding Events IPC Motion Shared Sound Vision Wireless
SRCS:=$(shell find $(SRC_DIRS) -name "[^.]*$(SRCSUFFIX)")

# We should also make sure these libraries are ready to go
# Note we've been lucky that these libraries happen to use a different
# source suffix, so we don't have to filter them out of SRCS
USERLIBS:= Shared/newmat Motion/roboop


###################################################
##             MAKEFILE MACHINERY                ##
###################################################
# Hopefully, you shouldn't have to change anything down here...

#delete automatic suffix list
.SUFFIXES:

.PHONY: all compile clean cleanDeps docs alldocs cleanDoc updateTools updateLibs $(USERLIBS) platformBuild update install static shared

ifeq ($(filter TGT_ERS%,$(TEKKOTSU_TARGET_MODEL)),)
all:
	@echo "Running $(MAKE) from the root directory will build the"
	@echo "Tekkotsu library which is linked against by executables."
	@echo "The Environment.conf from the template 'project' directory"
	@echo "will be used, which can be overridden by environment"
	@echo "variables.  Current settings are:"
	@echo "";
	@echo "  Target model: $(TEKKOTSU_TARGET_MODEL)"
	@echo "  Build directory: $(TEKKOTSU_BUILDDIR)"
	@echo "";
	@echo "You will want to run 'make' from your project directory in order"
	@echo "to produce executables..."
	@echo ""
	$(MAKE) TEKKOTSU_TARGET_PLATFORM=PLATFORM_LOCAL compile static shared
	@echo "Build successful."
else
all:
	@echo "Running $(MAKE) from the root directory will first build the"
	@echo "Tekkotsu library for Aperios (AIBO), and then for the local"
	@echo "platform.  The Environment.conf from the template 'project'"
	@echo "directory will be used, which can be overridden by environment"
	@echo "variables.  Current settings are:"
	@echo "";
	@echo "  Target model: $(TEKKOTSU_TARGET_MODEL)"
	@echo "  Build directory: $(TEKKOTSU_BUILDDIR)"
	@echo "";
	@echo "You will want to run 'make' from your project directory in order"
	@echo "to produce executables..."
	@echo ""
	$(MAKE) TEKKOTSU_TARGET_PLATFORM=PLATFORM_APERIOS compile static
	$(MAKE) TEKKOTSU_TARGET_PLATFORM=PLATFORM_LOCAL compile static shared
	@echo "Build successful."
endif

update install sim:
	@echo ""
	@echo "You probably want to be running make from within your project's directory"
	@echo ""
	@echo "You can run $(MAKE) from within the root Tekkotsu directory to build"
	@echo "libtekkotsu for both Aperios and the host platform, which will then"
	@echo "be linked against by the projects and tools."
	@echo ""
	@echo "However, you can only install or update to memory stick from within a project."
	@echo "You can use the template project directory if you want to build a stick"
	@echo "with the standard demo behaviors."

# Don't want to try to remake this - give an error if not found
$(TEKKOTSU_ROOT)/project/Environment.conf:
	@echo "Could not find Environment file - check the default project directory still exists"
	@exit 1

TOOLS_BUILT_FLAG:=$(TEKKOTSU_BUILDDIR)/.toolsBuilt

ifeq ($(TEKKOTSU_TARGET_PLATFORM),PLATFORM_APERIOS)
include aperios/Makefile.aperios
else
include local/Makefile.local
endif

# Sort by modification date
SRCS:=$(shell ls -t $(SRCS))

# The object file for each of the source files
OBJS:=$(addprefix $(TK_BD)/,$(SRCS:$(SRCSUFFIX)=.o))

# list of all source files of all components, sorted to remove
# duplicates.  This gives us all the source files which we care about,
# all in one place.
DEPENDS:=$(addprefix $(TK_BD)/,$(SRCS:$(SRCSUFFIX)=.d) $(addsuffix .d,$(TEKKOTSU_PCH)))


%.gch:
	@mkdir -p $(dir $@)
	@src=$(patsubst $(TK_BD)/%,%,$*); \
	echo "Pre-compiling $$src..."; \
	$(CXX) $(CXXFLAGS) -x c++-header -c $$src -o $@ > $*.log 2>&1; \
        retval=$$?; \
        cat $*.log | $(FILTERSYSWARN) | $(COLORFILT) | $(TEKKOTSU_LOGVIEW); \
        test $$retval -eq 0;

$(TK_BD)/%.d: %.cc
	@mkdir -p $(dir $@)
	@src=$(patsubst %.d,%.cc,$(patsubst $(TK_BD)%,$(TEKKOTSU_ROOT)%,$@)); \
	if [ ! -f "$$src" ] ; then \
		echo "ERROR: Missing source file '$$src'... you shouldn't be seeing this"; \
		exit 1; \
	fi; \
	echo "$@..." | sed 's@.*$(TK_BD)/@Generating @'; \
	$(CXX) $(CXXFLAGS) -MP -MG -MT "$@" -MT "$(@:.d=.o)" -MM "$$src" > $@

$(TK_BD)/$(TEKKOTSU_PCH).d:
	@mkdir -p $(dir $@)
	@src=$(TEKKOTSU_PCH); \
	echo "$@..." | sed 's@.*$(TK_BD)/@Generating @'; \
	$(CXX) $(CXXFLAGS) -MP -MG -MT "$@" -MT "$(@:.d=.gch)" -MM "$$src" > $@

EMPTYDEPS:=$(shell find $(TK_BD) -type f -name "*\.d" -size 0 -print -exec rm \{\} \;)
ifneq ($(EMPTYDEPS),)
  $(shell echo "Empty dependency files detected: $(EMPTYDEPS)" > /dev/tty)
endif

ifeq ($(filter clean% docs alldocs newstick,$(MAKECMDGOALS)),)
-include $(DEPENDS)
ifeq ($(TEKKOTSU_TARGET_PLATFORM),PLATFORM_APERIOS)
-include $(TK_BD)/aperios/aperios.d
endif
endif

compile: updateTools updateLibs platformBuild

$(TOOLS_BUILT_FLAG):
	@$(MAKE) TOOLS_BUILT_FLAG="$(TOOLS_BUILT_FLAG)" -C tools

docs:
	docs/builddocs --update --tree

alldocs:
	docs/builddocs --update --all --tree

updateTools: | $(TOOLS_BUILT_FLAG) 
	$(MAKE) -C tools

updateLibs: $(USERLIBS)

$(USERLIBS): | $(TOOLS_BUILT_FLAG)
	@echo "$@:"; \
	export TEKKOTSU_ENVIRONMENT_CONFIGURATION="$(TEKKOTSU_ENVIRONMENT_CONFIGURATION)"; \
	$(MAKE) -C "$@"

ifeq ($(findstring compile,$(MAKECMDGOALS)),compile)
ifeq ($(TEKKOTSU_TARGET_PLATFORM),PLATFORM_APERIOS)
static: $(TK_BD)/libtekkotsu.a ;
shared:
	@echo "PLATFORM_APERIOS does not support shared libraries... Make goal 'shared' ignored."
else
static: $(TK_BD)/libtekkotsu.a ;
shared: $(TK_BD)/libtekkotsu.$(if $(findstring Darwin,$(shell uname)),dylib,so) ;
endif
else
static shared: all ;
endif

$(TK_BD)/libtekkotsu.a: $(OBJS)
	@echo "Linking object files..."
	@printf "$@ <- "; echo "[...]" | sed 's@$(TK_BD)/@@g';
	@rm -f $@;
	@$(AR) $@ $(OBJS);
	@$(AR2) $@

$(TK_BD)/libtekkotsu.dylib: $(OBJS)
	@echo "Linking object files..."
	@printf "$@ <- "; echo "[...]" | sed 's@$(TK_BD)/@@g';
	@libtool -dynamic -undefined dynamic_lookup -o $@ $(OBJS);

$(TK_BD)/libtekkotsu.so: $(OBJS)
	@echo "Linking object files..."
	@printf "$@ <- "; echo "[...]" | sed 's@$(TK_BD)/@@g';
	@$(CXX) -shared -o $@ $(OBJS);

%.h :
	@echo "ERROR: Seems to be a missing header file '$@'...";
	@if [ "$(notdir $@)" = "def.h" -o "$(notdir $@)" = "entry.h" ] ; then \
		echo "WARNING: You shouldn't be seeing this message.  Report that you did." ; \
		echo "         Try a clean recompile." ; \
		exit 1; \
	fi;
	@echo "       Someone probably forgot to check a file into CVS.";
	@echo "       I'll try to find where it's being included from:";
	@echo "       if this was a file you recently deleted, just make again after this completes. (will update dependency files)";
	@find . -name "*.h" -exec grep -H "$(notdir $@)" \{\} \; ;
	@find . -name "*.cc" -exec grep -H "$(notdir $@)" \{\} \; ;
	@find $(TK_BD) -name "*.d" -exec grep -qH "$(notdir $@)" \{\} \; -exec rm \{\} \; ;
	@exit 1

#don't try to make random files via this implicit chain
%:: %.o ;

%.o: $(if $(TEKKOTSU_PCH),$(TK_BD)/$(TEKKOTSU_PCH).gch) | $(TOOLS_BUILT_FLAG)
	@mkdir -p $(dir $@)
	@src=$(patsubst %.o,%$(SRCSUFFIX),$(patsubst $(TK_BD)/%,%,$@)); \
	echo "Compiling $$src..."; \
	$(CXX) $(CXXFLAGS) $(INCLUDE_PCH) -o $@ -c $$src > $*.log 2>&1; \
	retval=$$?; \
	cat $*.log | $(FILTERSYSWARN) | $(COLORFILT) | $(TEKKOTSU_LOGVIEW); \
	test $$retval -eq 0;

clean:
	@printf "Cleaning all ~ files corresponding to .cc files..."
	@rm -f $(addsuffix ~,$(SRCS)) $(SRCS:$(SRCSUFFIX)=.h~)
	@printf "done.\n"
	rm -rf $(TEKKOTSU_BUILDDIR)
	$(MAKE) TOOLS_BUILT_FLAG="$(TOOLS_BUILT_FLAG)" -C tools clean;
	for dir in `ls aperios` ; do \
		if [ "$$dir" = "CVS" ] ; then continue; fi; \
		if [ "$$dir" = ".svn" ] ; then continue; fi; \
		if [ -f "aperios/$$dir" ] ; then continue; fi; \
		rm -f "aperios/$$dir/$${dir}Stub.h" "aperios/$$dir/$${dir}Stub.cc" "aperios/$$dir/def.h" "aperios/$$dir/entry.h" ; \
	done

cleanDeps:
	@printf "Cleaning all .d files from build directory..."
	@find "$(TEKKOTSU_BUILDDIR)" -name "*.d" -exec rm \{\} \;
	@printf "done.\n"

cleanDoc:
	docs/builddocs --clean



