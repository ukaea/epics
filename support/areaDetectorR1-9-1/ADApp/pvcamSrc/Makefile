TOP=../..
include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

USR_CFLAGS +=   -DWIN32
USR_CXXFLAGS += -DWIN32

PROD_NAME = pvCamApp
ifeq (win32-x86, $(findstring win32-x86, $(T_A)))
LIBRARY_IOC = pvCam
PROD_IOC += $(PROD_NAME)

else ifeq (cygwin-x86, $(findstring cygwin-x86, $(T_A)))
LIBRARY_IOC = pvCam
PROD_IOC += $(PROD_NAME)
endif

LIB_SRCS += pvCam.cpp
LIB_SRCS += drvPVCamEpics.c
LIB_LIBS += pvcam32 pv_icl32

# <name>.dbd will be created from <name>Include.dbd
DBD += $(PROD_NAME).dbd
$(PROD_NAME)_DBD += pvCamSupport.dbd
DBD += pvCamSupport.dbd

# <name>_registerRecordDeviceDriver.cpp will be created from <name>.dbd
PROD_SRCS += pvCamApp_registerRecordDeviceDriver.cpp pvCamAppMain.cpp

# Add locally compiled object code
PROD_LIBS += pvCam
PROD_LIBS += pvcam32 pv_icl32

include $(TOP)/ADApp/commonDriverMakefile

#=============================


include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

