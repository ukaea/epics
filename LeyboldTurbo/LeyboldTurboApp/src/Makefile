TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

LIBRARY_IOC = LeyboldTurboDev
PROD_IOC = LeyboldTurboApp

# LeyboldTurbo.dbd will be created and installed
DBD += LeyboldTurbo.dbd

# LeyboldTurbo.dbd will be made up from these files:
LeyboldTurboDev_DBD += base.dbd

# Add all the support libraries needed by this IOC
LeyboldTurboDev_LIBS += asyn
LeyboldTurboApp_LIBS += asyn
LeyboldTurboDev_SYS_LIBS_WIN32 += ws2_32

LeyboldTurboDev_SRCS += LeyboldTurboPortDriver.cpp

# LeyboldTurbo_registerRecordDeviceDriver.cpp derives from LeyboldTurbo.dbd
LeyboldTurboApp_LIBS += LeyboldTurboDev
LeyboldTurboApp_SRCS += LeyboldTurbo_registerRecordDeviceDriver.cpp
LeyboldTurboApp_SRCS += LeyboldTurboMain.cpp
INC += USSPacket.h
INC += ParameterDefns.h
INC += LeyboldTurboPortDriver.h

# Add support from base/src/vxWorks if needed
#LeyboldTurbo_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
LeyboldTurboDev_LIBS += $(EPICS_BASE_IOC_LIBS)
LeyboldTurboApp_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

