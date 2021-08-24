TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = bottles
# bottles.dbd will be created and installed
DBD += bottles.dbd

# bottles.dbd will be made up from these files:
bottles_DBD += base.dbd

# Include dbd files from all support applications:
bottles_DBD += calcSupport.dbd

#LIBS
bottles_LIBS += calc

# bottles_registerRecordDeviceDriver.cpp derives from bottles.dbd
bottles_SRCS += bottles_registerRecordDeviceDriver.cpp


# Build the main IOC entry point on workstation OSs.
bottles_SRCS_DEFAULT += bottlesMain.cpp
bottles_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#bottles_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
bottles_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

