TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

LIBRARY_IOC = VQM_ITMS_Dev
PROD_IOC = VQM_ITMS_App

# VQM_ITMS.dbd will be created and installed
DBD += VQM_ITMS.dbd

# VQM_ITMS.dbd will be made up from these files:
VQM_ITMS_DBD += base.dbd
VQM_ITMS_DBD += VQM_ITMS_Include.dbd

# Add all the support libraries needed by this IOC
VQM_ITMS_Dev_LIBS += asyn
VQM_ITMS_App_LIBS += asyn

VQM_ITMS_Dev_SRCS += VQM_ITMS_Driver.cpp
VQM_ITMS_Dev_SRCS += VQM_ITMS_Base.cpp
VQM_ITMS_Dev_SRCS += /VQM/CrossPlatform/GaugeDAQ.cpp
VQM_ITMS_Dev_SRCS += /VQM/CrossPlatform/IServiceWrapper.cpp
VQM_ITMS_Dev_SRCS += /VQM/CrossPlatform/FIR-filter-class/filt.cpp

# VQM_ITMS_registerRecordDeviceDriver.cpp derives from VQM_ITMS.dbd
VQM_ITMS_App_LIBS += VQM_ITMS_Dev
VQM_ITMS_App_SRCS += VQM_ITMS_registerRecordDeviceDriver.cpp
VQM_ITMS_App_SRCS += VQM_ITMS_Main.cpp
INC += VQM_ITMS_Base.h
INC += Exception.h
INC += ParameterDefns.h
INC += VQM_ITMSPortDriver.h
INC += /VQM/CrossPlatform/IServiceWrapper.h
INC += /VQM/CrossPlatform/IHeaderData.h
INC += /VQM/CrossPlatform/GaugeDAQ.h
INC += /VQM/CrossPlatform/FIR-filter-class/filt.h

# Add support from base/src/vxWorks if needed
#VQM_ITMS_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
VQM_ITMS_Dev_LIBS += $(EPICS_BASE_IOC_LIBS)
VQM_ITMS_App_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

