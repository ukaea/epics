#Makefile at top of application tree
TOP = .
include $(TOP)/configure/CONFIG
DIRS += configure
DIRS += boostApp
boostApp_DEPEND_DIRS = configure
DIRS += mbApp
mbApp_DEPEND_DIRS = configure boostApp
DIRS += testApp
testApp_DEPEND_DIRS = boostApp mbApp

include $(TOP)/configure/RULES_TOP


