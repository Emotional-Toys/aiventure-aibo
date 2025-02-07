# Adapted from Makefile for Independent JPEG Group's software

# This makefile is suitable for Unix-like systems with non-ANSI compilers.
# If you have an ANSI compiler, makefile.ansi is a better starting point.

ifndef TEKKOTSU_ENVIRONMENT_CONFIGURATION
$(error An error has occured, TEKKOTSU_ENVIRONMENT_CONFIGURATION was not defined)
endif
TEKKOTSU_ROOT:=$(shell cd ../.. && pwd)
include $(shell echo "$(TEKKOTSU_ENVIRONMENT_CONFIGURATION)" | sed 's/ /\\ /g')
BUILDDIR=$(TK_LIB_BD)/Motion/roboop
SRCSUFFIX=.cpp

# You may need to adjust these cc options:
CXXFLAGS:= -I ../../Shared/newmat -fno-inline \
          -Wall -W -Wlarger-than-8192 -Wpointer-arith -Wcast-qual \
          -Woverloaded-virtual -Wdeprecated -Wnon-virtual-dtor \
          -g \
          -fmessage-length=0 $(CXXFLAGS) $(ENV_CXXFLAGS) \

# -frename-registers -fomit-frame-pointer -fno-common \

ifeq ($(strip $(shell $(CXX) --version | head -n 1)),powerpc-apple-darwin8-g++-4.0.0 (GCC) 4.0.0 20041026 (Apple Computer, Inc. build 4061))
$(shell echo "Disabling ROBOOP optimization due to darwin gcc 4.0.0 compiler bug triggered by robot.cpp" > /dev/tty)
CXXFLAGS:=$(filter-out -O3,$(CXXFLAGS))
endif

#          -Wshadow -Weffc++

# Link-time cc options:
LDFLAGS:= $(LDFLAGS)

# To link any special libraries, add the necessary -l commands here.
LDLIBS= 

# miscellaneous OS-dependent stuff
# linker
LN= $(CXX)
# file deletion command
RM= rm -f
# file rename command
MV= mv

# if precompiled headers are enabled by the Environment.conf, then
# this file will be created with the names of all current .h files
# and then precompiled
PCH=all.h

# End of configurable options.

INCLUDE_PCH:=$(if $(TEKKOTSU_PCH),-include $(BUILDDIR)/$(PCH))

# source files:
LIBSOURCES:= $(wildcard *$(SRCSUFFIX))
ifeq ($(TEKKOTSU_TARGET_PLATFORM), PLATFORM_APERIOS)
LIBSOURCES:=$(filter-out gnugraph$(SRCSUFFIX),$(LIBSOURCES))
endif

SOURCES= $(LIBSOURCES)

INCLUDES:=$(filter-out $(PCH),$(wildcard *.h))
ifeq ($(TEKKOTSU_TARGET_PLATFORM), PLATFORM_APERIOS)
INCLUDES:=$(filter-out gnugraph.h,$(INCLUDES))
endif

LIBOBJECTS= $(addprefix $(BUILDDIR)/,$(LIBSOURCES:.cpp=.o))

all: $(BUILDDIR)/libroboop.a
ifneq ($(TEKKOTSU_TARGET_PLATFORM),PLATFORM_APERIOS)
all: $(BUILDDIR)/libroboop.$(if $(findstring Darwin,$(shell uname)),dylib,so)

  #enable -fPIC if we are building shared libraries on x86_64/amd64
  ifneq ($(filter __amd64__ __x86_64__,$(shell g++ $(ENV_CXXFLAGS) -dM -E - < /dev/null)),)
    CXXFLAGS:=$(CXXFLAGS) -fPIC
  endif

endif

.PHONY: all clean

$(BUILDDIR)/libroboop.a: $(LIBOBJECTS)
	$(RM) $@
	@echo "Linking $@..."
	@$(AR) $@  $(LIBOBJECTS)
	@$(AR2) $@

$(BUILDDIR)/libroboop.dylib: $(LIBOBJECTS)
	$(RM) $@
	@echo "Linking $@..."
	@libtool -dynamic -undefined dynamic_lookup -o $@ $(LIBOBJECTS);

$(BUILDDIR)/libroboop.so: $(LIBOBJECTS)
	$(RM) $@
	@echo "Linking $@..."
	@$(CXX) -shared -o $@ $(LIBOBJECTS);

clean:
	$(RM) *.o *.a *.log core $(PCH)
	$(RM) -r $(BUILDDIR)

%:: %.o;

%.o: $(if $(TEKKOTSU_PCH),$(BUILDDIR)/$(PCH).gch)
	@mkdir -p $(dir $@)
	@src=$(patsubst %.o,%$(SRCSUFFIX),$(patsubst $(BUILDDIR)/%,%,$@)); \
	echo "Compiling ROBOOP::$$src... (Reduced warnings)"; \
	$(CXX) $(CXXFLAGS) $(INCLUDE_PCH) -o $@ -c $$src > $*.log 2>&1; \
	retval=$$?; \
	cat $*.log | $(FILTERSYSWARN) | $(COLORFILT) | $(TEKKOTSU_LOGVIEW); \
	test $$retval -eq 0;

$(BUILDDIR)/$(PCH).gch: $(PCH)
	@mkdir -p $(dir $@)
	@src=$(PCH); \
	echo "Pre-compiling ROBOOP::$$src... (Reduced warnings)"; \
	$(CXX) $(CXXFLAGS) -x c++-header -o $@ -c $$src > $*.log 2>&1; \
	retval=$$?; \
	cat $*.log | $(FILTERSYSWARN) | $(COLORFILT) | $(TEKKOTSU_LOGVIEW); \
	test $$retval -eq 0;

$(PCH): $(INCLUDES)
	@rm -f $@;
	@for x in $^ ; do \
		echo "#include \"$$x\"" >> $@; \
	done

$(BUILDDIR)/controller.o: controller.cpp controller.h

$(BUILDDIR)/control_select.o: control_select.cpp control_select.h

$(BUILDDIR)/dynamics_sim.o: dynamics_sim.cpp dynamics_sim.h

$(BUILDDIR)/trajectory.o: trajectory.cpp trajectory.h

$(BUILDDIR)/clik.o :  clik.cpp clik.h utils.h robot.h

$(BUILDDIR)/robot.o :  robot.cpp utils.h robot.h

$(BUILDDIR)/config.o : config.cpp config.h

$(BUILDDIR)/quaternion.o : quaternion.cpp quaternion.h

$(BUILDDIR)/gnugraph.o :  gnugraph.cpp gnugraph.h utils.h robot.h

$(BUILDDIR)/comp_dq.o :  comp_dq.cpp utils.h robot.h

$(BUILDDIR)/comp_dqp.o :  comp_dqp.cpp utils.h robot.h

$(BUILDDIR)/delta_t.o :  delta_t.cpp utils.h robot.h

$(BUILDDIR)/dynamics.o :  dynamics.cpp utils.h robot.h

$(BUILDDIR)/homogen.o :  homogen.cpp utils.h robot.h

$(BUILDDIR)/invkine.o :  invkine.cpp utils.h robot.h

$(BUILDDIR)/kinemat.o :  kinemat.cpp utils.h robot.h

$(BUILDDIR)/sensitiv.o :  sensitiv.cpp utils.h robot.h

$(BUILDDIR)/utils.o :  utils.cpp utils.h robot.h
