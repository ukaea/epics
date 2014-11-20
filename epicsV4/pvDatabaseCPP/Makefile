#Makefile at top of application tree
TOP = .
include $(TOP)/configure/CONFIG
DIRS := $(DIRS) $(filter-out $(DIRS), configure)
DIRS := $(DIRS) $(filter-out $(DIRS), src)
DIRS := $(DIRS) $(filter-out $(DIRS), test)
DIRS := $(DIRS) $(filter-out $(DIRS), arrayPerformance)
DIRS := $(DIRS) $(filter-out $(DIRS), $(wildcard example*))

EMBEDDED_TOPS := $(EMBEDDED_TOPS) $(filter-out $(EMBEDDED_TOPS), test)
EMBEDDED_TOPS := $(EMBEDDED_TOPS) $(filter-out $(EMBEDDED_TOPS), arrayPerformance)
EMBEDDED_TOPS := $(EMBEDDED_TOPS) $(filter-out $(EMBEDDED_TOPS), $(wildcard example*))

define DIR_template
 $(1)_DEPEND_DIRS = configure
endef
$(foreach dir, $(filter-out configure,$(DIRS)),$(eval $(call DIR_template,$(dir))))

define EMB_template
 $(1)_DEPEND_DIRS = src
endef
$(foreach dir, $(EMBEDDED_TOPS),$(eval $(call EMB_template,$(dir))))

#exampleDatabase_DEPEND_DIRS += test
#examplePowerSupply_DEPEND_DIRS += test

include $(TOP)/configure/RULES_TOP
