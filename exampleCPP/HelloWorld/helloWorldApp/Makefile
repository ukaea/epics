TOP=..

include $(TOP)/configure/CONFIG

PROD_HOST += HelloService
HelloService_SRCS += HelloServiceMain.cpp
HelloService_SRCS += HelloService.cpp
HelloService_LIBS += pvAccess pvData

PROD_HOST += HelloClient
HelloClient_SRCS += HelloClient.cpp
HelloClient_LIBS += pvAccess pvData

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

