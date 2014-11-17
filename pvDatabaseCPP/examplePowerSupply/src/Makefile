TOP=..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#==================================================
# Build an IOC support library
#


PROD_HOST += powerSupplyMain
powerSupplyMain_SRCS += powerSupplyMain.cpp

powerSupplyMain_LIBS += pvDatabase
powerSupplyMain_LIBS += pvAccess
powerSupplyMain_LIBS += pvData
powerSupplyMain_LIBS += Com
powerSupplyMain_LIBS += powerSupply

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

