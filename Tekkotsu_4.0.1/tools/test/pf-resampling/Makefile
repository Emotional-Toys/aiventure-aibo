
# This Makefile will handle most aspects of compiling and
# linking a tool against the Tekkotsu framework.  You probably
# won't need to make any modifications, but here's the major controls

# Executable name, defaults to:
#   `basename \`pwd\``-$(TEKKOTSU_TARGET_MODEL)
TEKKOTSU_TARGET_MODEL?=TGT_ERS7
BIN:=$(shell pwd | sed 's@.*/@@')-$(shell echo $(patsubst TGT_%,%,$(TEKKOTSU_TARGET_MODEL)))

# Build directory
PROJECT_BUILDDIR:=build

# Other default values are drawn from the template project's
# Environment.conf file.  This is found using $(TEKKOTSU_ROOT)
# Remove the '?' if you want to override an environment variable
# with a value of your own.
TEKKOTSU_ROOT:=./../../..

# Source files, defaults to all files ending matching *$(SRCSUFFIX)
SRCSUFFIX:=.cc
PROJ_SRC:=$(shell find . -name "*$(SRCSUFFIX)") $(TEKKOTSU_ROOT)/Shared/zignor.cc $(TEKKOTSU_ROOT)/Shared/zigrandom.cc

.PHONY: all test

TEMPLATE_PROJECT:=$(TEKKOTSU_ROOT)/project
TEKKOTSU_ENVIRONMENT_CONFIGURATION?=$(TEMPLATE_PROJECT)/Environment.conf
$(if $(shell [ -r $(TEKKOTSU_ENVIRONMENT_CONFIGURATION) ] || echo "failure"),$(error An error has occured, '$(TEKKOTSU_ENVIRONMENT_CONFIGURATION)' could not be found.  You may need to edit TEKKOTSU_ROOT in the Makefile))

TEKKOTSU_TARGET_PLATFORM:=PLATFORM_LOCAL
include $(shell echo "$(TEKKOTSU_ENVIRONMENT_CONFIGURATION)" | sed 's/ /\\ /g')
FILTERSYSWARN:=$(patsubst $(TEKKOTSU_ROOT)/%,$(TEKKOTSU_ROOT)/%,$(FILTERSYSWARN))
COLORFILT:=$(patsubst $(TEKKOTSU_ROOT)/%,$(TEKKOTSU_ROOT)/%,$(COLORFILT))
$(shell mkdir -p $(PROJ_BD))

PROJ_OBJ:=$(patsubst ./%$(SRCSUFFIX),$(PROJ_BD)/%.o,$(PROJ_SRC))

# this one doesn't actually need libtekkotsu.a
LIBS:= 
# $(TK_BD)/libtekkotsu.a $(TK_BD)/../Motion/roboop/libroboop.a $(TK_BD)/../Shared/newmat/libnewmat.a

DEPENDS:=$(PROJ_OBJ:.o=.d)

CXXFLAGS:=-g -Wall \
         -I$(TEKKOTSU_ROOT) \
         -I$(TEKKOTSU_ROOT)/Shared/jpeg-6b $(shell xml2-config --cflags) \
         -D$(TEKKOTSU_TARGET_PLATFORM) -D$(TEKKOTSU_TARGET_MODEL) 


all: $(BIN)

$(BIN): $(PROJ_OBJ) $(LIBS)
	@echo "Linking $@..."
	@$(CXX) $(PROJ_OBJ) $(LIBS) $(shell xml2-config --libs) -lpthread -o $@

ifeq ($(findstring clean,$(MAKECMDGOALS)),)
-include $(DEPENDS)
endif

%.a :
	@echo "ERROR: $@ was not found.  You may need to compile the Tekkotsu framework."
	@echo "Press return to attempt to build it, ctl-C to cancel."
	@read;
	$(MAKE) -C $(TEKKOTSU_ROOT) compile

%.d :
	@mkdir -p $(dir $@)
	@src=$(patsubst %.d,%.cc,$(patsubst $(PROJ_BD)/%,%,$@)); \
	echo "$@..." | sed 's@.*$(TGT_BD)/@Generating @'; \
	$(CXX) $(CXXFLAGS) -MP -MG -MT "$@" -MT "$(@:.d=.o)" -MM "$$src" > $@

%.o:
	@mkdir -p $(dir $@)
	@src=$(patsubst %.o,%$(SRCSUFFIX),$(patsubst $(PROJ_BD)/%,%,$@)); \
	echo "Compiling $$src..."; \
	$(CXX) $(CXXFLAGS) -o $@ -c $$src > $*.log 2>&1; \
	retval=$$?; \
	cat $*.log | $(FILTERSYSWARN) | $(COLORFILT) | $(TEKKOTSU_LOGVIEW); \
	test $$retval -eq 0; \

clean:
	rm -rf $(BIN) $(PROJECT_BUILDDIR) Shared *~

test: ./$(BIN)
	@for x in * ; do \
		if [ -r "ideal-$$x" ] ; then \
			if diff -u "ideal-$$x" "$$x" ; then \
				echo "Test '$$x' passed"; \
			else \
				echo "Test output '$$x' does not match ideal"; \
			fi; \
		fi; \
	done