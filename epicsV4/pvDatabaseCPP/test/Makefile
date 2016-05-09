# Makefile for the pvData tests

TOP = ..
include $(TOP)/configure/CONFIG

PVDATABASE_TEST = $(TOP)/test

PROD_LIBS += pvDatabase pvAccess pvData Com

include $(PVDATABASE_TEST)/src/Makefile

# The testHarness runs all the test programs in a known working order.
testHarness_SRCS += pvDatabaseAllTests.c

PROD_vxWorks = vxTestHarness
vxTestHarness_SRCS += $(testHarness_SRCS)
TESTSPEC_vxWorks = vxTestHarness.$(MUNCH_SUFFIX); pvDatabaseAllTests

PROD_RTEMS += rtemsTestHarness
rtemsTestHarness_SRCS += rtemsTestHarness.c rtemsConfig.c
rtemsTestHarness_SRCS += $(testHarness_SRCS)
TESTSPEC_RTEMS = rtemsTestHarness.$(MUNCH_SUFFIX); pvDatabaseAllTests

TESTSCRIPTS_HOST += $(TESTS:%=%.t)

include $(TOP)/configure/RULES


