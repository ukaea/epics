TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

LIBRARY_IOC = VQM_ITMSDev
PROD_IOC = VQM_ITMSApp

# VQM_ITMS.dbd will be created and installed
DBD += VQM_ITMS.dbd

# VQM_ITMS.dbd will be made up from these files:
VQM_ITMSDev_DBD += base.dbd

# Add all the support libraries needed by this IOC
VQM_ITMSDev_LIBS += asyn
VQM_ITMSApp_LIBS += asyn
VQM_ITMSDev_SYS_LIBS_WIN32 += ws2_32

VQM_ITMSDev_SRCS += VQM_ITMSPortDriver.cpp
VQM_ITMSDev_SRCS += VQM_ITMSBase.cpp

# VQM_ITMS_registerRecordDeviceDriver.cpp derives from VQM_ITMS.dbd
VQM_ITMSApp_LIBS += VQM_ITMSDev
VQM_ITMSApp_SRCS += VQM_ITMS_registerRecordDeviceDriver.cpp
VQM_ITMSApp_SRCS += VQM_ITMSMain.cpp
INC += VQM_ITMSBase.h
INC += USSPacket.h
INC += ParameterDefns.h
INC += VQM_ITMSPortDriver.h

# Add support from base/src/vxWorks if needed
#VQM_ITMS_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
VQM_ITMSDev_LIBS += $(EPICS_BASE_IOC_LIBS)
VQM_ITMSApp_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

