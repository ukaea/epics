TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = VarianTurboPump
# VarianTurboPump.dbd will be created and installed
DBD += VarianTurboPump.dbd

# VarianTurboPump.dbd will be made up from these files:
VarianTurboPump_DBD += base.dbd

# Include dbd files from all support applications:
VarianTurboPump_DBD += VarianTurboSupport.dbd

# Add all the support libraries needed by this IOC
#VarianTurboPump_LIBS += VarianTurboPump
VarianTurboPump_LIBS += stream
VarianTurboPump_LIBS += asyn
VarianTurboPump_LIBS += calc
VarianTurboPump_LIBS += sscan

# VarianTurboPump_registerRecordDeviceDriver.cpp derives from VarianTurboPump.dbd
VarianTurboPump_SRCS += VarianTurboPump_registerRecordDeviceDriver.cpp

# Adding support for Moxa-specific iocsh commands
ifeq ($(T_A),linux-armv5teb)
#DBD += MoxaSerialSupport.dbd
MoxaSerialSupport_DBD += MoxaSerialModeSupport.dbd
VarianTurboPump_LIBS += MoxaSerialMode
VarianTurboPump_SRCS += MoxaSerialSupport_registerRecordDeviceDriver.cpp
endif
ifeq ($(T_A),linux-armb)
#DBD += MoxaSerialSupport.dbd
MoxaSerialSupport_DBD += MoxaSerialModeSupport.dbd
VarianTurboPump_LIBS += MoxaSerialMode
VarianTurboPump_SRCS += MoxaSerialSupport_registerRecordDeviceDriver.cpp
endif

# Build the main IOC entry point on workstation OSs.
VarianTurboPump_SRCS_DEFAULT += VarianTurboPumpMain.cpp
VarianTurboPump_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#VarianTurboPump_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
VarianTurboPump_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

