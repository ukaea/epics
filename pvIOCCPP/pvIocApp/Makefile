TOP = ..
include $(TOP)/configure/CONFIG

IOC = $(TOP)/pvIocApp/

LIBRARY_IOC += pvIOC
pvIOC_LIBS += $(EPICS_BASE_IOC_LIBS)
pvIoc_LIBS += Com pvData pvAccess

SRC_DIRS += $(IOC)/service
DBD += pvServiceChannelRegister.dbd
INC += channelBase.h
INC += pvServiceProvider.h
INC += service.h
LIBSRCS += channelBase.cpp
LIBSRCS += channelBaseProvider.cpp
LIBSRCS += pvServiceProvider.cpp
LIBSRCS += pvServiceChannelRegister.cpp
LIBSRCS += pvServiceChannelCTX.cpp
LIBSRCS += serviceChannelRPC.cpp

SRC_DIRS += $(IOC)/ezchannelRPC
INC += ezchannelRPC.h
LIBSRCS += ezchannelRPC.cpp

SRC_DIRS += $(IOC)/channelProviderLocal
DBD += channelProviderLocal.dbd
INC += channelProviderLocal.h
LIBSRCS += channelProviderLocal.cpp
LIBSRCS += channelProviderLocalRegister.cpp

#SRC_DIRS += $(IOC)/support
#INC += support.h
#LIBSRCS += support.cpp
#LIBSRCS += supportState.cpp
#LIBSRCS += recordProcess.cpp

#SRC_DIRS += $(IOC)/database
#INC += pvDatabase.h
#LIBSRCS += pvRecord.cpp
#LIBSRCS += pvDatabase.cpp

#SRC_DIRS += $(IOC)/install
#INC += install.h
#LIBSRCS += afterStart.cpp
#LIBSRCS += install.cpp
#LIBSRCS += supportCreation.cpp

#SRC_DIRS += $(IOC)/support/basic
#INC += basicSupport.h
#INC += noopSupport.cpp
#INC += genericSupport.cpp
#INC += scanFieldSupport.cpp

#SRC_DIRS += $(IOC)/util
#INC += util.h
#LIBSRCS += scanTypeFunc.cpp
#LIBSRCS += requestResultFunc.cpp
#LIBSRCS += eventScanner.cpp
#LIBSRCS += periodicScanner.cpp
#LIBSRCS += scanField.cpp

#SRC_DIRS += $(IOC)/monitor
#INC += monitorAlgorithm.h

#SRC_DIRS += $(IOC)/pvCopy
#INC += pvCopy.h
#LIBSRCS += pvCopy.cpp
#LIBSRCS += pvShare.cpp
##LIBSRCS += pvShareStructureArray.cpp


include $(TOP)/configure/RULES

