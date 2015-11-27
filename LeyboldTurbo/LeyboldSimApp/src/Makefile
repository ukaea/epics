TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = LeyboldSim
# LeyboldSim.dbd will be created and installed
DBD += LeyboldSim.dbd

# LeyboldSim.dbd will be made up from these files:
LeyboldSim_DBD += base.dbd

# Include dbd files from all support applications:
#LeyboldSim_DBD += xxx.dbd
USR_INCLUDES += -I../../../LeyboldTurboApp/src

# Add all the support libraries needed by this IOC
LeyboldSim_LIBS += LeyboldTurboDev
LeyboldSim_LIBS += asyn
LeyboldSim_SYS_LIBS_WIN32 += ws2_32

# LeyboldSim_registerRecordDeviceDriver.cpp derives from LeyboldSim.dbd
LeyboldSim_SRCS += LeyboldSim_registerRecordDeviceDriver.cpp
LeyboldSim_SRCS += LeyboldSimPortDriver.cpp

# Build the main IOC entry point on workstation OSs.
LeyboldSim_SRCS_DEFAULT += LeyboldSimMain.cpp
LeyboldSim_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#LeyboldSim_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
LeyboldSim_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

