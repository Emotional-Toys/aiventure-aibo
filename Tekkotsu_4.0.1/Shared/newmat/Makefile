# Adapted from Makefile for Independent JPEG Group's software

# This makefile is suitable for Unix-like systems with non-ANSI compilers.
# If you have an ANSI compiler, makefile.ansi is a better starting point.

ifndef TEKKOTSU_ENVIRONMENT_CONFIGURATION
$(error An error has occured, TEKKOTSU_ENVIRONMENT_CONFIGURATION was not defined)
endif
TEKKOTSU_ROOT:=$(shell cd ../.. && pwd)
include $(shell echo "$(TEKKOTSU_ENVIRONMENT_CONFIGURATION)" | sed 's/ /\\ /g')
BUILDDIR=$(TK_LIB_BD)/Shared/newmat
SRCSUFFIX=.cpp

# You may need to adjust these cc options:
CXXFLAGS:= -g $(if $(TEKKOTSU_DEBUG),-g -fno-inline -DDEBUG,) \
          -Wall -W -Wlarger-than-8192 -Wpointer-arith \
          -Woverloaded-virtual -Wdeprecated -Wnon-virtual-dtor \
          -fmessage-length=0 \
          -DWANT_MATH -DWANT_STREAM -DWANT_STRING $(CXXFLAGS) $(ENV_CXXFLAGS)

#          -O3 -frename-registers -fomit-frame-pointer -fno-common \
#          -Wshadow -Weffc++

# Link-time cc options:
LDFLAGS:= $(LDFLAGS)

# To link any special libraries, add the necessary -l commands here.
LDLIBS= 

# miscellaneous OS-dependent stuff
# linker
LN= $(CC)
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
LIBSOURCES= bandmat.cpp cholesky.cpp evalue.cpp fft.cpp hholder.cpp \
            jacobi.cpp myexcept.cpp newfft.cpp newmat1.cpp newmat2.cpp \
            newmat3.cpp newmat4.cpp newmat5.cpp newmat6.cpp newmat7.cpp \
            newmat8.cpp newmat9.cpp newmatex.cpp newmatnl.cpp newmatrm.cpp \
            sort.cpp submat.cpp svd.cpp

SOURCES= $(LIBSOURCES)

# files included by the precompiled header (not necessarily all headers... some give trouble)
INCLUDES= include.h newmat.h newmatap.h newmatrc.h \
          controlw.h myexcept.h newmatnl.h precisio.h

LIBOBJECTS= $(addprefix $(BUILDDIR)/,$(LIBSOURCES:.cpp=.o))
# newmat1.o newmat2.o newmat3.o newmat4.o newmat5.o newmat6.o newmat7.o newmat8.o newmat9.o newmatex.o bandmat.o submat.o myexcept.o cholesky.o evalue.o fft.o hholder.o jacobi.o newfft.o sort.o svd.o newmatrm.o newmatnl.o

all: $(BUILDDIR)/libnewmat.a
ifneq ($(TEKKOTSU_TARGET_PLATFORM),PLATFORM_APERIOS)
all: $(BUILDDIR)/libnewmat.$(if $(findstring Darwin,$(shell uname)),dylib,so)

  #enable -fPIC if we are building shared libraries on x86_64/amd64
  ifneq ($(filter __amd64__ __x86_64__,$(shell g++ $(ENV_CXXFLAGS) -dM -E - < /dev/null)),)
    CXXFLAGS:=$(CXXFLAGS) -fPIC
  endif

endif

.PHONY: all clean

$(BUILDDIR)/libnewmat.a: $(LIBOBJECTS)
	$(RM) $@
	@echo "Linking $@..."
	@$(AR) $@  $(LIBOBJECTS)
	@$(AR2) $@

$(BUILDDIR)/libnewmat.dylib: $(LIBOBJECTS)
	$(RM) $@
	@echo "Linking $@..."
	@libtool -dynamic -undefined dynamic_lookup -o $@ $(LIBOBJECTS);

$(BUILDDIR)/libnewmat.so: $(LIBOBJECTS)
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
	echo "Compiling NEWMAT::$$src... (Reduced warnings)"; \
	$(CXX) $(CXXFLAGS) $(INCLUDE_PCH) -o $@ -c $$src > $*.log 2>&1; \
	retval=$$?; \
	cat $*.log | $(FILTERSYSWARN) | $(COLORFILT) | $(TEKKOTSU_LOGVIEW); \
	test $$retval -eq 0;

$(BUILDDIR)/$(PCH).gch: $(PCH)
	@mkdir -p $(dir $@)
	@src=$(PCH); \
	echo "Pre-compiling NEWMAT::$$src... (Reduced warnings)"; \
	$(CXX) $(CXXFLAGS) -x c++-header -o $@ -c $$src > $*.log 2>&1; \
	retval=$$?; \
	cat $*.log | $(FILTERSYSWARN) | $(COLORFILT) | $(TEKKOTSU_LOGVIEW); \
	test $$retval -eq 0;

$(PCH): $(INCLUDES)
	@rm -f $@;
	@for x in $^ ; do \
		echo "#include \"$$x\"" >> $@; \
	done

$(BUILDDIR)/newmat1.o:     	newmat1.cpp newmat.h include.h myexcept.h

$(BUILDDIR)/newmat2.o:     	newmat2.cpp include.h newmat.h newmatrc.h myexcept.h controlw.h

$(BUILDDIR)/newmat3.o:     	newmat3.cpp include.h newmat.h newmatrc.h myexcept.h controlw.h

$(BUILDDIR)/newmat4.o:     	newmat4.cpp include.h newmat.h newmatrc.h myexcept.h controlw.h

$(BUILDDIR)/newmat5.o:     	newmat5.cpp include.h newmat.h newmatrc.h myexcept.h controlw.h

$(BUILDDIR)/newmat6.o:     	newmat6.cpp include.h newmat.h newmatrc.h myexcept.h controlw.h

$(BUILDDIR)/newmat7.o:     	newmat7.cpp include.h newmat.h newmatrc.h myexcept.h controlw.h

$(BUILDDIR)/newmat8.o:     	newmat8.cpp include.h newmat.h newmatrc.h precisio.h myexcept.h controlw.h

$(BUILDDIR)/newmat9.o:     	newmat9.cpp include.h newmat.h newmatio.h newmatrc.h myexcept.h controlw.h


$(BUILDDIR)/newmatex.o:    	newmatex.cpp include.h newmat.h myexcept.h

$(BUILDDIR)/vbandmat.o:     	bandmat.cpp include.h newmat.h newmatrc.h myexcept.h controlw.h

$(BUILDDIR)/submat.o:      	submat.cpp include.h newmat.h newmatrc.h myexcept.h controlw.h

$(BUILDDIR)/myexcept.o:    	myexcept.cpp include.h myexcept.h

$(BUILDDIR)/cholesky.o:    	cholesky.cpp include.h newmat.h myexcept.h

$(BUILDDIR)/evalue.o:      	evalue.cpp include.h newmatap.h newmatrm.h precisio.h newmat.h myexcept.h

$(BUILDDIR)/fft.o:         	fft.cpp include.h newmatap.h newmat.h myexcept.h

$(BUILDDIR)/hholder.o:     	hholder.cpp include.h newmatap.h newmat.h myexcept.h

$(BUILDDIR)/jacobi.o:      	jacobi.cpp include.h newmatap.h precisio.h newmatrm.h newmat.h myexcept.h

$(BUILDDIR)/newfft.o:      	newfft.cpp newmatap.h newmat.h include.h myexcept.h

$(BUILDDIR)/sort.o:        	sort.cpp include.h newmatap.h newmat.h myexcept.h

$(BUILDDIR)/svd.o:         	svd.cpp include.h newmatap.h newmatrm.h precisio.h newmat.h myexcept.h

$(BUILDDIR)/newmatrm.o:    	newmatrm.cpp newmat.h newmatrm.h include.h myexcept.h

$(BUILDDIR)/newmatnl.o:    	newmatnl.cpp newmatap.h newmatnl.h newmat.h include.h myexcept.h
