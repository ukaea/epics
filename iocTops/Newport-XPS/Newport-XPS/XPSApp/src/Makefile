TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = XPS
# XPS.dbd will be created and installed
DBD += XPS.dbd

# XPS.dbd will be made up from these files:
XPS_DBD += base.dbd

# Include dbd files from all support applications:
#XPS_DBD += xxx.dbd
XPS_DBD += motorSupport.dbd
XPS_DBD += devNewport.dbd
XPS_DBD += drvAsynIPPort.dbd
XPS_DBD += asSupport.dbd
XPS_DBD += asyn.dbd
XPS_DBD += busySupport.dbd
XPS_DBD += sscan.dbd

# Add all the support libraries needed by this IOC
#XPS_LIBS += xxx
XPS_LIBS += asyn busy seqDev seq pv
XPS_LIBS += Newport
XPS_LIBS += motor
XPS_LIBS += autosave
XPS_LIBS += sscan

# XPS_registerRecordDeviceDriver.cpp derives from XPS.dbd
XPS_SRCS += XPS_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OSs.
XPS_SRCS_DEFAULT += XPSMain.cpp
XPS_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#XPS_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
XPS_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

