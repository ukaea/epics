# Makefile for the pvaSrv library

TOP = ..
include $(TOP)/configure/CONFIG

LIBRARY_IOC = pvaSrv

SHRLIB_VERSION = 0.11.0

PVASRV_SRC = $(TOP)/src

include $(PVASRV_SRC)/dbPv/Makefile
#include $(PVASRV_SRC)/dbGroup/Makefile

pvaSrv_LIBS += pvAccess pvData
pvaSrv_LIBS += $(EPICS_BASE_IOC_LIBS)

include $(TOP)/configure/RULES
