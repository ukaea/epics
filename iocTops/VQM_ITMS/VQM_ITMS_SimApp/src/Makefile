TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = VQM_ITMS_Sim
# VQM_ITMS_Sim.dbd will be created and installed
DBD += VQM_ITMS_Sim.dbd

# VQM_ITMS_Sim.dbd will be made up from these files:
VQM_ITMS_Sim_DBD += base.dbd

# Include dbd files from all support applications:
#VQM_ITMS_Sim_DBD += xxx.dbd
USR_INCLUDES += -I../../../VQM_ITMSApp/src

# Add all the support libraries needed by this IOC
VQM_ITMS_Sim_LIBS += VQM_ITMSDev
VQM_ITMS_Sim_LIBS += asyn
VQM_ITMS_Sim_SYS_LIBS_WIN32 += ws2_32

# VQM_ITMS_Sim_registerRecordDeviceDriver.cpp derives from VQM_ITMS_Sim.dbd
VQM_ITMS_Sim_SRCS += VQM_ITMS_Sim_registerRecordDeviceDriver.cpp
VQM_ITMS_Sim_SRCS += VQM_ITMS_SimPortDriver.cpp

# Build the main IOC entry point on workstation OSs.
VQM_ITMS_Sim_SRCS_DEFAULT += VQM_ITMS_SimMain.cpp
VQM_ITMS_Sim_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#VQM_ITMS_Sim_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
VQM_ITMS_Sim_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

