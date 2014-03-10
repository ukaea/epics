TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = motorIms
# motorIms.dbd will be created and installed
DBD += motorIms.dbd

# motorIms.dbd will be made up from these files:
motorIms_DBD += base.dbd

# Include dbd files from all support applications:
motorIms_DBD += motorSupport.dbd
motorIms_DBD += devImsMotor.dbd
motorIms_DBD += drvAsynSerialPort.dbd

# Add all the support libraries needed by this IOC
motorIms_LIBS += motor
motorIms_LIBS += Ims
motorIms_LIBS += asyn


# motorIms_registerRecordDeviceDriver.cpp derives from motorIms.dbd
motorIms_SRCS += motorIms_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OSs.
motorIms_SRCS_DEFAULT += motorImsMain.cpp
motorIms_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#motorIms_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
motorIms_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

