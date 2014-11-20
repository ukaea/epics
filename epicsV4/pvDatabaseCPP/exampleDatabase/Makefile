# Makefile at top of application tree

TOP = .
include $(TOP)/configure/CONFIG

DIRS += configure

DIRS += src
src_DEPEND_DIRS = configure

DIRS += ioc
ioc_DEPEND_DIRS = src

DIRS += iocBoot
iocBoot_DEPEND_DIRS = src

include $(TOP)/configure/RULES_TOP


