TOP=..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#==================================================
# Build an IOC support library
#

DBD += exampleLink.dbd

INC += exampleLink.h

LIBRARY_IOC += exampleLink
exampleLink_SRCS += exampleLink.cpp
exampleLink_SRCS += exampleLinkRegister.cpp
exampleLink_LIBS += pvDatabase
exampleLink_LIBS += pvAccess
exampleLink_LIBS += pvData
exampleLink_LIBS += Com
exampleLink_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

