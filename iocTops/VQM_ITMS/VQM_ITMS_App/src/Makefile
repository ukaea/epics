TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

SRC = $(TOP)/VQM_ITMS_App/src

LIBRARY_IOC = VQM_ITMS_Dev
PROD_IOC = VQM_ITMS_App

include $(SRC)/VQM/CrossPlatform/FIR-filter-class/Makefile
include $(SRC)/VQM/CrossPlatform/Makefile
include $(SRC)/VQM/Include/Makefile

# VQM_ITMS.dbd will be created and installed
DBD += VQM_ITMS.dbd

# VQM_ITMS.dbd will be made up from these files:
VQM_ITMS_DBD += VQM_ITMS_Include.dbd

# Add all the support libraries needed by this IOC
VQM_ITMS_Dev_LIBS += asyn
VQM_ITMS_App_LIBS += asyn

SRC_DIRS += VQM

VQM_ITMS_Dev_SRCS += VQM_ITMS_Driver.cpp
VQM_ITMS_Dev_SRCS += Exception.cpp
VQM_ITMS_Dev_SRCS += VQM_ITMS_Base.cpp

VQM_ITMS_App_LIBS += VQM_ITMS_Dev
VQM_ITMS_App_SRCS += VQM_ITMS_registerRecordDeviceDriver.cpp
VQM_ITMS_App_SRCS += VQM_ITMS_Main.cpp

INC += VQM_ITMS_Base.h
INC += Exception.h
INC += ParameterDefns.h
INC += VQM_ITMS_Driver.h

# Add support from base/src/vxWorks if needed
#VQM_ITMS_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
VQM_ITMS_Dev_LIBS += $(EPICS_BASE_IOC_LIBS)
VQM_ITMS_App_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

