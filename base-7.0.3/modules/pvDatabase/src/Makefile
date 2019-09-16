# Makefile for the pvDatabase library

TOP = ..
include $(TOP)/configure/CONFIG

# needed for Windows
LIB_SYS_LIBS_WIN32 += netapi32 ws2_32

PVDATABASE_SRC = $(TOP)/src

LIBRARY += pvDatabase

# shared library ABI version.
SHRLIB_VERSION ?= $(EPICS_PVDATABASE_MAJOR_VERSION).$(EPICS_PVDATABASE_MINOR_VERSION).$(EPICS_PVDATABASE_MAINTENANCE_VERSION)

INC += pv/pvPlugin.h
INC += pv/pvStructureCopy.h
INC += pv/pvArrayPlugin.h
INC += pv/pvDeadbandPlugin.h
INC += pv/pvTimestampPlugin.h

INC += pv/pvDatabase.h

INC += pv/channelProviderLocal.h

INC += pv/traceRecord.h
INC += pv/removeRecord.h
INC += pv/processRecord.h

INC += pv/pvSupport.h
INC += pv/controlSupport.h
INC += pv/scalarAlarmSupport.h


include $(PVDATABASE_SRC)/copy/Makefile
include $(PVDATABASE_SRC)/database/Makefile
include $(PVDATABASE_SRC)/pvAccess/Makefile
include $(PVDATABASE_SRC)/special/Makefile
include $(PVDATABASE_SRC)/support/Makefile

pvDatabase_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)
pvDatabase_LIBS += $(EPICS_BASE_IOC_LIBS)

include $(TOP)/configure/RULES
