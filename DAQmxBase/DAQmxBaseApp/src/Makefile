TOP=../..

include $(TOP)/configure/CONFIG

# -------------------------------
# Build an Diamond Support Module
# -------------------------------

USR_INCLUDES_DEFAULT += -I/usr/local/natinst/nidaqmx/include
USR_INCLUDES_WIN32 += -I"C:\Program Files (x86)\National Instruments\Shared\ExternalCompilerSupport\C\include"

PROD_IOC = DAQmxBase

# DAQmxBase.dbd will be installed into <top>/dbd
DBD = DAQmxBase.dbd

# The following are compiled and added to the support library


# DAQmxBase.dbd will be created from these files
DAQmxBase_DBD += base.dbd
DAQmxBase_DBD += DAQmxBaseSupport.dbd
DAQmxBase_DBD += asyn.dbd

# DAQmxBase_registerRecordDeviceDriver.cpp will be created
# DAQmxBase.dbd
DAQmxBase_SRCS += DAQmxBase_registerRecordDeviceDriver.cpp
DAQmxBase_SRCS += drvDaqMxBase.cpp

# These two lines are needed for non-vxWorks builds, such as Linux
DAQmxBase_SRCS_DEFAULT += DAQmxBaseMain.cpp


DAQmxBase_SRCS_vxWorks += -nil-

# This line says that this IOC Application depends on the
# xxx Support Module
DAQmxBase_LIBS += asyn
DAQmxBase_LIBS += $(EPICS_BASE_IOC_LIBS)

USR_LDFLAGS += -LIBPATH:"C:\Program Files (x86)\National Instruments\Shared\ExternalCompilerSupport\C\lib64\msvc"
USR_LIBS += nidaqmx

# The following adds object code from base/src/vxWorks
DAQmxBase_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

include $(TOP)/configure/RULES


