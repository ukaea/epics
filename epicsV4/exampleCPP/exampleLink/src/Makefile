TOP=..

include $(TOP)/configure/CONFIG

exampleLinkSRC = $(TOP)/src

INC += pv/exampleMonitorLinkRecord.h
INC += pv/exampleGetLinkRecord.h
INC += pv/examplePutLinkRecord.h
INC += pv/doubleArrayRecord.h

DBD += exampleMonitorLinkRegister.dbd
DBD += exampleGetLinkRegister.dbd
DBD += examplePutLinkRegister.dbd
DBD += doubleArrayRegister.dbd

LIBRARY = exampleLink
LIBSRCS += exampleMonitorLinkRecord.cpp
LIBSRCS += exampleGetLinkRecord.cpp
LIBSRCS += examplePutLinkRecord.cpp
LIBSRCS += doubleArrayRecord.cpp
LIBSRCS += exampleMonitorLinkRegister.cpp
LIBSRCS += exampleGetLinkRegister.cpp
LIBSRCS += examplePutLinkRegister.cpp
LIBSRCS += doubleArrayRegister.cpp
exampleLink_LIBS += pvDatabase pvaClient pvAccess nt pvData ca Com

PROD_HOST += exampleLinkMain
exampleLinkMain_SRCS += exampleLinkMain.cpp
exampleLinkMain_LIBS += exampleLink
exampleLinkMain_LIBS += pvDatabase pvaClient pvAccess nt pvData ca Com


PROD_HOST += exampleLinkClient
exampleLinkClient_SRCS += exampleLinkClient.cpp
exampleLinkClient_LIBS += pvaClient pvAccess pvData ca Com


# shared library ABI version.
SHRLIB_VERSION ?= 1.0


include $(TOP)/configure/RULES


