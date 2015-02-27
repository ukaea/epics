TOP=../..

include $(TOP)/configure/CONFIG

# We need to link against the EPICS Base libraries
#xxx_LIBS += $(EPICS_BASE_IOC_LIBS)
USR_CXXFLAGS_DEFAULT += -fno-operator-names
USR_CXXFLAGS_DEFAULT += -pedantic


# ------------------------
# Build an IOC Application
# ------------------------

PROD_IOC = RGA-TS-IOC-01

# RGA-TS-IOC-01.dbd will be installed into <top>/dbd
DBD += RGA-TS-IOC-01.dbd

# RGA-TS-IOC-01.dbd will be created from these files
RGA-TS-IOC-01_DBD += base.dbd
RGA-TS-IOC-01_DBD += genSubRecord.dbd
RGA-TS-IOC-01_DBD += calcSupport.dbd
RGA-TS-IOC-01_DBD += asyn.dbd
RGA-TS-IOC-01_DBD += drvAsynSerialPort.dbd
RGA-TS-IOC-01_DBD += stream.dbd
RGA-TS-IOC-01_DBD += rgamvplusInclude.dbd





# RGA-TS-IOC-01_registerRecordDeviceDriver.cpp will be created
# RGA-TS-IOC-01.dbd
RGA-TS-IOC-01_SRCS += RGA-TS-IOC-01_registerRecordDeviceDriver.cpp

# These two lines are needed for non-vxWorks builds, such as Linux
RGA-TS-IOC-01_SRCS_DEFAULT += RGA-TS-IOC-01Main.cpp

################################################################
# This IOC Application depends on the following support modules
#
RGA-TS-IOC-01_LIBS += genSub
RGA-TS-IOC-01_LIBS += calc
RGA-TS-IOC-01_LIBS += sscan
RGA-TS-IOC-01_LIBS += stream
RGA-TS-IOC-01_LIBS += pcre
RGA-TS-IOC-01_LIBS += asyn
RGA-TS-IOC-01_LIBS += seq
RGA-TS-IOC-01_LIBS += pv
RGA-TS-IOC-01_LIBS += $(EPICS_BASE_IOC_LIBS)

RGA-TS-IOC-01_LIBS += rgamvplus

################################################################




# We need to link this IOC Application against the EPICS Base libraries
RGA-TS-IOC-01_LIBS += $(EPICS_BASE_IOC_LIBS)

# ---------------------------------------------------
include $(TOP)/configure/RULES

