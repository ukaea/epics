TOP = ..
include $(TOP)/configure/CONFIG

arrayPerformanceSRC = $(TOP)/src

INC+= pv/arrayPerformance.h
INC+= pv/longArrayMonitor.h
INC+= pv/longArrayGet.h
INC+= pv/longArrayPut.h

LIBRARY = arrayPerform
LIBSRCS += arrayPerformance.cpp
LIBSRCS += longArrayGet.cpp
LIBSRCS += longArrayMonitor.cpp
LIBSRCS += longArrayPut.cpp
arrayPerform_LIBS += pvDatabase pvaClient pvAccess pvData Com

PROD_HOST += arrayPerformanceMain
arrayPerformanceMain_SRCS += arrayPerformanceMain.cpp
arrayPerformanceMain_LIBS += arrayPerform
arrayPerformanceMain_LIBS += pvDatabase pvaClient pvAccess pvData ca Com

PROD_HOST += longArrayGetMain
longArrayGetMain_SRCS += longArrayGetMain.cpp
longArrayGetMain_LIBS += arrayPerform
longArrayGetMain_LIBS += pvaClient pvAccess pvData ca Com

PROD_HOST += longArrayMonitorMain
longArrayMonitorMain_SRCS += longArrayMonitorMain.cpp
longArrayMonitorMain_LIBS += arrayPerform
longArrayMonitorMain_LIBS += pvaClient pvAccess ca pvData Com


PROD_HOST += longArrayPutMain
longArrayPutMain_SRCS += longArrayPutMain.cpp
longArrayPutMain_LIBS += arrayPerform
longArrayPutMain_LIBS += pvaClient pvAccess pvData ca Com


PROD_HOST += vectorPerformanceMain
vectorPerformanceMain_SRCS += vectorPerformanceMain.cpp
vectorPerformanceMain_LIBS +=  pvData Com


# shared library ABI version.
SHRLIB_VERSION ?= 1.0


include $(TOP)/configure/RULES

