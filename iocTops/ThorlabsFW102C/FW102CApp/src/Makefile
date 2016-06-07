TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = FW102C
# FW102C.dbd will be created and installed
DBD += FW102C.dbd

# FW102C.dbd will be made up from these files:
FW102C_DBD += base.dbd

# Include dbd files from all support applications:
FW102C_DBD += asyn.dbd
FW102C_DBD += drvAsynSerialPort.dbd
FW102C_DBD += $(STREAM)/dbd/stream.dbd

# Add all the support libraries needed by this IOC
FW102C_LIBS += stream asyn

# FW102C_registerRecordDeviceDriver.cpp derives from FW102C.dbd
FW102C_SRCS += FW102C_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OSs.
FW102C_SRCS_DEFAULT += FW102CMain.cpp
FW102C_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#FW102C_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
FW102C_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

