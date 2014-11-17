TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#==================================================
# Build an IOC support library
#

DBD += powerSupply.dbd

#=============================
# build an ioc application

PROD_IOC += powerSupply


# <name>_registerRecordDeviceDriver.cpp will be created from <name>.dbd
powerSupply_SRCS += powerSupply_registerRecordDeviceDriver.cpp
powerSupply_SRCS_DEFAULT += powerSupplyMain.cpp
powerSupply_SRCS_vxWorks += -nil-


# The following adds support from base/src/vxWorks
powerSupply_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

powerSupply_LIBS += powerSupply
powerSupply_LIBS += pvDatabase
powerSupply_LIBS += pvaSrv
powerSupply_LIBS += pvAccess
powerSupply_LIBS += pvData
powerSupply_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

