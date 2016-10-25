TOP=..

include $(TOP)/configure/CONFIG

exampleDatabaseSRC = $(TOP)/src

INC += pv/exampleHelloRecord.h
INC += pv/exampleHelloRPCRecord.h
INC += pv/exampleDatabase.h

DBD += exampleDatabaseRegister.dbd

LIBRARY = exampleDatabase
LIBSRCS += exampleHelloRecord.cpp
LIBSRCS += exampleHelloRPCRecord.cpp
LIBSRCS += exampleDatabase.cpp
LIBSRCS += exampleDatabaseRegister.cpp
exampleDatabase_LIBS += pvDatabase pvAccess nt pvData Com


PROD_HOST += exampleDatabaseMain
exampleDatabaseMain_SRCS += exampleDatabaseMain.cpp
exampleDatabaseMain_LIBS += exampleDatabase
exampleDatabaseMain_LIBS += pvDatabase pvAccess nt pvData Com

PROD_HOST += exampleHelloRPCClient
exampleHelloRPCClient_SRCS += exampleHelloRPCClient.cpp
exampleHelloRPCClient_LIBS += pvaClient pvAccess nt pvData ca Com


# shared library ABI version.
SHRLIB_VERSION ?= 1.0


include $(TOP)/configure/RULES

