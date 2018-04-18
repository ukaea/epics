# src Makefile for example_EdwardsNextTurbo.
#
TOP=../..

include $(TOP)/configure/CONFIG

SCRIPTS=stExample-gui

# ------------------------
# Build an IOC Application
# ------------------------

PROD_IOC += example_EdwardsNextTurbo

# example_EdwardsNextTurbo.dbd will be installed into <top>/dbd
DBD += example_EdwardsNextTurbo.dbd

# exampleEdwardsNextTurbo.dbd will be created from these files
example_EdwardsNextTurbo_DBD += base.dbd
#example_EdwardsNextTurbo_DBD += busySupport.dbd
# From module streamDevice.
example_EdwardsNextTurbo_DBD += stream.dbd
# From module asyn.
example_EdwardsNextTurbo_DBD += asyn.dbd
# From module asyn.
example_EdwardsNextTurbo_DBD += drvAsynSerialPort.dbd
example_EdwardsNextTurbo_DBD += drvAsynIPPort.dbd
# From module calc.
example_EdwardsNextTurbo_DBD += calcSupport.dbd

# example_EdwardsNextTurbo_registerRecordDeviceDriver.cpp will be created
# from example_EdwardsNextTurbo.dbd
example_EdwardsNextTurbo_SRCS += example_EdwardsNextTurbo_registerRecordDeviceDriver.cpp

# These two lines are needed for non-vxWorks builds, such as Linux
example_EdwardsNextTurbo_SRCS_DEFAULT += example_EdwardsNextTurboMain.cpp
example_EdwardsNextTurbo_SRCS_vxWorks += -nil-

# Add locally compiled object code
#example_EdwardsNextTurbo_SRCS +=

# The following adds object code from base/src/vxWorks
example_EdwardsNextTurbo_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# This line says that this IOC Application depends on the
# xxx Support Module
#example_EdwardsNextTurbo_LIBS += busy
# From module streamDevice.
example_EdwardsNextTurbo_LIBS += stream
# From module asyn.
example_EdwardsNextTurbo_LIBS += asyn
# From module calc.
example_EdwardsNextTurbo_LIBS += calc

# We need to link this IOC Application against the EPICS Base libraries
example_EdwardsNextTurbo_LIBS += $(EPICS_BASE_IOC_LIBS)

# ---------------------------------------------------
include $(TOP)/configure/RULES
