TOP=..

include $(TOP)/configure/CONFIG

USR_SYS_LIBS += expat

PROD_HOST += ArchiverServiceRPC
ArchiverServiceRPC_SRCS += ArchiverServiceRPCMain.cpp
ArchiverServiceRPC_SRCS += ArchiverServiceRPC.cpp
ArchiverServiceRPC_SRCS += common.cpp
ArchiverServiceRPC_LIBS += dbIoc pvAccess pvData Com Storage Tools

PROD_HOST += ArchiverClient
ArchiverClient_SRCS += ArchiverClient.cpp
ArchiverClient_SRCS += ArchiverClientResponseHandler.cpp
ArchiverClient_SRCS += common.cpp
ArchiverClient_LIBS += pvAccess pvData Com

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

