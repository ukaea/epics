TOP=../..
include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#USR_CFLAGS += 
#USR_CXXFLAGS +=

LIBRARY_IOC = PSL

PROD_NAME = PSLApp
PROD_IOC += $(PROD_NAME)

LIB_SRCS += PSL.cpp

#==================================

# <name>.dbd will be created from <name>Include.dbd
DBD += $(PROD_NAME).dbd
$(PROD_NAME)_DBD += PSLSupport.dbd
DBD += PSLSupport.dbd

# <name>_registerRecordDeviceDriver.cpp will be created from <name>.dbd
PROD_SRCS_DEFAULT += PSLApp_registerRecordDeviceDriver.cpp PSLAppMain.cpp
PROD_SRCS_vxWorks += PSLApp_registerRecordDeviceDriver.cpp

# Add locally compiled object code
PROD_LIBS += PSL

include $(AREA_DETECTOR)/ADApp/commonDriverMakefile
$(PROD_NAME)_DBD += drvAsynIPPort.dbd

#=============================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

