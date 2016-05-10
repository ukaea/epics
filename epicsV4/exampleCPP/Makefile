# Makefile for the EPICS V4 exampleCPP module

TOP = .
include $(TOP)/configure/CONFIG

EMBEDDED_TOPS += $(TOP)/database
EMBEDDED_TOPS += $(TOP)/exampleClient
EMBEDDED_TOPS += $(TOP)/exampleLink
EMBEDDED_TOPS += $(TOP)/powerSupply
EMBEDDED_TOPS += $(TOP)/helloPutGet
EMBEDDED_TOPS += $(TOP)/helloRPC
EMBEDDED_TOPS += $(TOP)/pvDatabaseRPC
EMBEDDED_TOPS += $(TOP)/arrayPerformance

DIRS := configure

DIRS += $(EMBEDDED_TOPS)

DIRS += test
test_DEPEND_DIRS = configure

# No inter-module dependencies

include $(TOP)/configure/RULES_TOP
