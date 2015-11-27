#Makefile at top of application tree
TOP = .
include $(TOP)/configure/CONFIG
DIRS := $(DIRS) $(filter-out $(DIRS), configure)
DIRS := $(DIRS) $(filter-out $(DIRS), src)
DIRS := $(DIRS) $(filter-out $(DIRS), example)

EMBEDDED_TOPS := $(EMBEDDED_TOPS) $(filter-out $(EMBEDDED_TOPS), example)

define DIR_template
 $(1)_DEPEND_DIRS = configure
endef
$(foreach dir, $(filter-out configure,$(DIRS)),$(eval $(call DIR_template,$(dir))))

define EMB_template
 $(1)_DEPEND_DIRS = src
endef
$(foreach dir, $(EMBEDDED_TOPS),$(eval $(call EMB_template,$(dir))))

include $(TOP)/configure/RULES_TOP
