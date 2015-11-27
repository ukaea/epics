TOP = ../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE

#  Generate snc main programs (not needed under VxWorks)
SNCFLAGS_DEFAULT += +m
SNCFLAGS_vxWorks += -nil-

SNC = $(INSTALL_HOST_BIN)/snc$(HOSTEXE)

# test programs that should give a specified number of
# warnings and/or errors are listed in snc_test.plt
TESTSCRIPTS_HOST += snc_test.t

# uncomment these tests if building for 32 bit systems
#TESTSCRIPTS_HOST += build_test.t
#TESTSCRIPTS_CROSS += build_test.t

# uncomment these tests if building for 64 bit systems
#TESTSCRIPTS_HOST += build64_test.t
#TESTSCRIPTS_CROSS += build64_test.t

# test programs that should build w/o warnings or errors,
# not even from the C compiler
TESTPROD_HOST += cppComment
TESTPROD_HOST += exOpt_DuplOpt
TESTPROD_HOST += funcdef
TESTPROD_HOST += include_windows_h
TESTPROD_HOST += member
TESTPROD_HOST += namingConflict
TESTPROD_HOST += subscript
TESTPROD_HOST += sync_not_monitored
TESTPROD_HOST += syncq_not_monitored
TESTPROD_HOST += type_expr

PROD_LIBS += seq pv
PROD_LIBS += $(EPICS_BASE_IOC_LIBS)

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE
