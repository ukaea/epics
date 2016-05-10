TOP=..

include $(TOP)/configure/CONFIG

exampleLinkSRC = $(TOP)/src

INC += pv/exampleLinkRecord.h

DBD += exampleLinkRegister.dbd

LIBRARY = exampleLink
LIBSRCS += exampleLinkRecord.cpp
LIBSRCS += exampleLinkRegister.cpp
exampleLink_LIBS += pvDatabase pvaClient pvAccess nt pvData ca Com

PROD_HOST += exampleLinkMain
exampleLinkMain_SRCS += exampleLinkMain.cpp
exampleLinkMain_LIBS += exampleLink
exampleLinkMain_LIBS += pvDatabase pvaClient pvAccess nt pvData ca Com

PROD_HOST += doubleArrayMain
doubleArrayMain_SRCS += doubleArrayMain.cpp
doubleArrayMain_LIBS += pvDatabase pvAccess nt pvData  Com


PROD_HOST += exampleLinkClient
exampleLinkClient_SRCS += exampleLinkClient.cpp
exampleLinkClient_LIBS += pvaClient pvAccess pvData ca Com


# shared library ABI version.
SHRLIB_VERSION ?= 1.0


include $(TOP)/configure/RULES


