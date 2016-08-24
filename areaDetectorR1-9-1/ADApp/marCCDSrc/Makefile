TOP=../..
include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

# We add _AD (for areaDetector) to the library name to avoid conflict
# with the library marCCD in the ccd module
LIBRARY_IOC_Linux = marCCD_AD
LIBRARY_IOC_Darwin = marCCD_AD
LIB_SRCS += marCCD.cpp

PROD_NAME = marCCDApp
PROD_IOC_Linux += $(PROD_NAME)
PROD_IOC_Darwin += $(PROD_NAME)

# <name>.dbd will be created from <name>Include.dbd
DBD += $(PROD_NAME).dbd
$(PROD_NAME)_DBD += marCCD_ADSupport.dbd
DBD += marCCD_ADSupport.dbd

# <name>_registerRecordDeviceDriver.cpp will be created from <name>.dbd
PROD_SRCS += marCCDApp_registerRecordDeviceDriver.cpp marCCDAppMain.cpp

# Add locally compiled object code
PROD_LIBS += marCCD_AD

include $(TOP)/ADApp/commonDriverMakefile
$(PROD_NAME)_DBD += drvAsynIPPort.dbd

#=============================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

