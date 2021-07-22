TOP = ..
include $(TOP)/configure/CONFIG
DIRS := $(DIRS) $(filter-out $(DIRS), $(wildcard *src*))
DIRS := $(DIRS) $(filter-out $(DIRS), $(wildcard *Src*))
DIRS := $(DIRS) $(filter-out $(DIRS), $(wildcard *db*))
DIRS := $(DIRS) $(filter-out $(DIRS), $(wildcard *Db*))
DIRS := $(DIRS) $(filter-out $(DIRS), $(wildcard *opi*))
DIRS := $(DIRS) $(filter-out $(DIRS), $(wildcard *doc*))
DIRS := $(DIRS) $(filter-out $(DIRS), $(wildcard *Doc*))
include $(TOP)/configure/RULES_DIRS

# include $(EPICS_LOCAL)/drivers/makefileIncludes/InstallScriptMakefile.include
