TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#==================================================
# Build an IOC support library
#

DBD += helloPutGet.dbd

#=============================
# build an ioc application

PROD_IOC += helloPutGet


# <name>_registerRecordDeviceDriver.cpp will be created from <name>.dbd
helloPutGet_SRCS += helloPutGet_registerRecordDeviceDriver.cpp
helloPutGet_SRCS_DEFAULT += helloPutGetMain.cpp
helloPutGet_SRCS_vxWorks += -nil-


# The following adds support from base/src/vxWorks
helloPutGet_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

helloPutGet_LIBS += helloPutGet
helloPutGet_LIBS += pvDatabase pvaSrv pvAccess pvData Com
helloPutGet_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

