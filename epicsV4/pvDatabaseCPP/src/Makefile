# Makefile for the pvDatabase library

TOP = ..
include $(TOP)/configure/CONFIG

PVDATABASE_SRC = $(TOP)/src


LIBRARY += pvDatabase

# shared library ABI version.
SHRLIB_VERSION ?= 4.2

INC += pv/channelProviderLocal.h
INC += pv/pvDatabase.h
INC += pv/traceRecord.h
INC += pv/removeRecord.h

include $(PVDATABASE_SRC)/database/Makefile
include $(PVDATABASE_SRC)/pvAccess/Makefile
include $(PVDATABASE_SRC)/special/Makefile


pvDatabase_LIBS += pvAccess pvData Com
pvDatabase_LIBS += $(EPICS_BASE_IOC_LIBS)



include $(TOP)/configure/RULES

