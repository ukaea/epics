TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = LeyboldTurbo
# LeyboldTurbo.dbd will be created and installed
DBD += LeyboldTurbo.dbd

# LeyboldTurbo.dbd will be made up from these files:
LeyboldTurbo_DBD += base.dbd

# Include dbd files from all support applications:
#LeyboldTurbo_DBD += xxx.dbd

# Add all the support libraries needed by this IOC
LeyboldTurbo_LIBS += asyn
LeyboldTurbo_LIBS += $(EPICS_BASE_IOC_LIBS)
LeyboldTurbo_SYS_LIBS_WIN32 += ws2_32

# LeyboldTurbo_registerRecordDeviceDriver.cpp derives from LeyboldTurbo.dbd
LeyboldTurbo_SRCS += LeyboldTurbo_registerRecordDeviceDriver.cpp
LeyboldTurbo_SRCS += LeyboldTurboPortDriver.cpp
INC += USSPacket.h
INC += ParameterDefns.h
INC += LeyboldTurboPortDriver.h

# Build the main IOC entry point on workstation OSs.
LeyboldTurbo_SRCS_DEFAULT += LeyboldTurboMain.cpp
LeyboldTurbo_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#LeyboldTurbo_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
LeyboldTurbo_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

