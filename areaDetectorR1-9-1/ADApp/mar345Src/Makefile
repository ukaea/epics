TOP=../..
include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

LIBRARY_IOC_Linux += mar345
LIBRARY_IOC_Darwin += mar345

LIB_SRCS += mar345.cpp
LIB_SRCS += mar3xx_pck.c

PROD_NAME = mar345App
PROD_IOC_Linux += $(PROD_NAME)
PROD_IOC_Darwin += $(PROD_NAME)

#==================================

# <name>.dbd will be created from <name>Include.dbd
DBD += $(PROD_NAME).dbd
$(PROD_NAME)_DBD += mar345Support.dbd
DBD += mar345Support.dbd

# <name>_registerRecordDeviceDriver.cpp will be created from <name>.dbd
PROD_SRCS += mar345App_registerRecordDeviceDriver.cpp mar345AppMain.cpp

# Add locally compiled object code
PROD_LIBS += mar345

include $(TOP)/ADApp/commonDriverMakefile
$(PROD_NAME)_DBD += drvAsynIPPort.dbd

#=============================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

