TOP=..

include $(TOP)/configure/CONFIG

PROD_HOST += HelloService
HelloService_SRCS += HelloServiceMain.cpp
HelloService_SRCS += HelloService.cpp
HelloService_LIBS += pvAccess pvData Com

PROD_HOST += HelloClient
HelloClient_SRCS += HelloClient.cpp
HelloClient_LIBS += pvAccess pvData Com

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

