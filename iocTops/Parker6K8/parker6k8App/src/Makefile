TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = parker6k8
# parker6k8.dbd will be created and installed
DBD += parker6k8.dbd

# parker6k8.dbd will be made up from these files:
parker6k8_DBD += base.dbd
parker6k8_DBD += asyn.dbd
parker6k8_DBD += drvAsynIPPort.dbd
parker6k8_DBD += drvAsynSerialPort.dbd
parker6k8_DBD += motorSupport.dbd
parker6k8_DBD += parker6kSupport.dbd
parker6k8_DBD += asSupport.dbd
parker6k8_DBD += calcSupport.dbd

# Include dbd files from all support applications:
#parker6k8_DBD += xxx.dbd

# Add all the support libraries needed by this IOC
#parker6k8_LIBS += xxx
parker6k8_LIBS += asyn
parker6k8_LIBS += motor
parker6k8_LIBS += parker6kSupport
parker6k8_LIBS += autosave
parker6k8_LIBS += calc

# Finally link to the EPICS Base libraries
parker6k8_LIBS += $(EPICS_BASE_IOC_LIBS)

# parker6k8_registerRecordDeviceDriver.cpp derives from parker6k8.dbd
parker6k8_SRCS += parker6k8_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OSs.
parker6k8_SRCS_DEFAULT += parker6k8Main.cpp
parker6k8_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#parker6k8_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
parker6k8_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

