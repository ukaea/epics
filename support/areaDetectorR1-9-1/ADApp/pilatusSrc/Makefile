TOP=../..
include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

LIBRARY_IOC_Linux = pilatusDetector
LIBRARY_IOC_Darwin = pilatusDetector

PROD_NAME = pilatusDetectorApp
PROD_IOC_Linux += $(PROD_NAME)
PROD_IOC_Darwin += $(PROD_NAME)

USR_INCLUDES += -I$(TOP)/ADApp/cbfSrc

LIB_SRCS += pilatusDetector.cpp
# <name>.dbd will be created from <name>Include.dbd
DBD += $(PROD_NAME).dbd
$(PROD_NAME)_DBD += pilatusDetectorSupport.dbd
DBD += pilatusDetectorSupport.dbd

# <name>_registerRecordDeviceDriver.cpp will be created from <name>.dbd
PROD_SRCS += pilatusDetectorApp_registerRecordDeviceDriver.cpp pilatusDetectorAppMain.cpp

# Add locally compiled object code
PROD_LIBS += pilatusDetector
PROD_LIBS += cbfad

include $(TOP)/ADApp/commonDriverMakefile
$(PROD_NAME)_DBD += drvAsynIPPort.dbd

#=============================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

