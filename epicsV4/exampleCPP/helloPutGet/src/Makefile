TOP=..

include $(TOP)/configure/CONFIG

helloPutGetSRC = $(TOP)/src

INC += pv/helloPutGetRecord.h

DBD += helloPutGetRegister.dbd

LIBRARY = helloPutGet
LIBSRCS += helloPutGetRecord.cpp
LIBSRCS += helloPutGetRegister.cpp
helloPutGet_LIBS += pvDatabase pvAccess nt pvData Com

PROD_HOST += helloPutGetMain
helloPutGetMain_SRCS += helloPutGetMain.cpp
helloPutGetMain_LIBS += helloPutGet
helloPutGetMain_LIBS += pvDatabase pvAccess pvData Com

PROD_HOST += helloPutGetClient
helloPutGetClient_SRCS += helloPutGetClient.cpp
helloPutGetClient_LIBS += pvaClient
helloPutGetClient_LIBS += pvAccess pvData ca Com

# shared library ABI version.
SHRLIB_VERSION ?= 1.0


include $(TOP)/configure/RULES


