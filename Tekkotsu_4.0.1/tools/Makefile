
COMPONENTS:=filtersyswarn binstrswap mon mipaltools easytrain
SUBMAKE_TGTS:=$(shell find . test -mindepth 2 -maxdepth 2 -name Makefile)

.PHONY: all clean releaseCheck $(COMPONENTS) $(SUBMAKE_TGTS)

TOOLS_BUILT_FLAG?=.toolsBuilt

all: $(COMPONENTS)
	@if [ ! -f $(TOOLS_BUILT_FLAG) ] ; then \
		touch $(TOOLS_BUILT_FLAG); \
	fi

clean: $(COMPONENTS)
	@rm -f $(TOOLS_BUILT_FLAG)

$(COMPONENTS):
	@printf "Making tool $@: "
	@$(MAKE) -C "$@" $(MAKECMDGOALS)

releaseCheck: $(SUBMAKE_TGTS)

$(SUBMAKE_TGTS):
	@dir=$(dir $@); \
	printf "Checking tool $$dir:\n"; \
	$(MAKE) -C "$$dir" clean; \
	if [ $$? -ne 0 ] ; then \
		exit 1; \
	fi; \
	$(MAKE) -C "$$dir"; \
	if [ $$? -ne 0 ] ; then \
		exit 1; \
	fi; \
	$(MAKE) -C "$$dir" test; \
	if [ $$? -ne 0 ] ; then \
		exit 1; \
	fi; \
