# Makefile for the pvDatabase library

TOP = ..
include $(TOP)/configure/CONFIG

PVDATABASE_SRC = $(TOP)/src


LIBRARY += pvDatabase

INC += pv/channelProviderLocal.h
INC += pv/pvDatabase.h
INC += pv/traceRecord.h

include $(PVDATABASE_SRC)/database/Makefile
include $(PVDATABASE_SRC)/pvAccess/Makefile
include $(PVDATABASE_SRC)/special/Makefile


pvDatabase_LIBS += pvAccess pvData Com
pvDatabase_LIBS += $(EPICS_BASE_IOC_LIBS)

# shared library ABI version.
SHRLIB_VERSION ?= 4.2


include $(TOP)/configure/RULES

