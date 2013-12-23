TOP=../..

include $(TOP)/configure/CONFIG

PROD_HOST += exampleServiceRPC
exampleServiceRPC_SRCS += exampleServiceRPCMain.cpp
exampleServiceRPC_SRCS += exampleServiceRPC.cpp
exampleServiceRPC_LIBS += pvIOC pvAccess pvData Com
#exampleServiceRPC_LIBS += pvIOC pvAccess pvData pvMB Com

PROD_HOST += exampleClient
exampleClient_SRCS += exampleClient.cpp
#exampleClient_LIBS += pvIOC pvAccess pvData pvMB Com
exampleClient_LIBS += pvIOC pvAccess pvData Com


LIBRARY_IOC += exampleServiceIOC
exampleServiceIOC_LIBS += $(EPICS_BASE_IOC_LIBS)
exampleServiceIOC_LIBS += Com pvData pvAccess pvIOC

DBD += exampleServiceRegister.dbd
exampleServiceIOC_SRCS += exampleServiceRegister.cpp
exampleServiceIOC_SRCS += exampleServiceRPC.cpp


include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

