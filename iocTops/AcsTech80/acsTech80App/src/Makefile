TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

# The following are used for debugging messages.
!USR_CXXFLAGS += -DDEBUG

#=============================
# Build the IOC application

PROD_IOC = acsTech80
# acsTech80.dbd will be created and installed
DBD += acsTech80.dbd

# acsTech80.dbd will be made up from these files:
acsTech80_DBD += base.dbd

# Include dbd files from all support applications:
#ifdef ASYN
acsTech80_DBD += asyn.dbd
acsTech80_DBD += drvAsynIPPort.dbd
#endif
acsTech80_DBD += motorSupport.dbd
acsTech80_DBD += devSPiiPlus.dbd

# Add all the support libraries needed by this IOC
acsTech80_LIBS += acsTech80
acsTech80_LIBS += motor
#ifdef ASYN
acsTech80_LIBS += asyn
#endif
#ifdef SNCSEQ
acsTech80_LIBS += seq pv
#endif

# acsTech80_registerRecordDeviceDriver.cpp derives from acsTech80.dbd
acsTech80_SRCS += acsTech80_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OSs.
acsTech80_SRCS_DEFAULT += acsTech80Main.cpp
acsTech80_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#acsTech80_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
acsTech80_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

