TOP=..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#==================================================
# Build an IOC support library
#


DBD += exampleServer.dbd

INC += exampleServer.h

LIBRARY_IOC += exampleServer
exampleServer_SRCS += exampleServer.cpp
exampleServer_SRCS += exampleServerRegister.cpp
exampleServer_LIBS += pvDatabase
exampleServer_LIBS += pvAccess
exampleServer_LIBS += pvData
exampleServer_LIBS += Com
exampleServer_LIBS += $(EPICS_BASE_IOC_LIBS)

PROD_HOST += exampleServerMain
exampleServerMain_SRCS += exampleServerMain.cpp
exampleServerMain_LIBS += exampleServer
exampleServerMain_LIBS += pvDatabase
exampleServerMain_LIBS += pvAccess
exampleServerMain_LIBS += pvData
exampleServerMain_LIBS += Com

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

