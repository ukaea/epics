TOP = ../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE

SNC = $(INSTALL_HOST_BIN)/snc$(HOSTEXE)

DBD += demo.dbd

LIBRARY_IOC  += demoLib
demoLib_SRCS += demo.st
demoLib_LIBS += seq pv
demoLib_LIBS += $(EPICS_BASE_IOC_LIBS)

#  Products
PROD_IOC = demo

# <name>_registerRecordDeviceDriver.cpp will be created from <name>.dbd
demo_SRCS += demo_registerRecordDeviceDriver.cpp
demo_SRCS_DEFAULT += demoMain.c
demo_SRCS_vxWorks += -nil-

#The following adds support from base/src/vxWorks
demo_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

demo_LIBS += demoLib
demo_LIBS += seq pv
demo_LIBS += $(EPICS_BASE_IOC_LIBS)

demo_LIBS += $(EPICS_BASE_IOC_LIBS)

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE
