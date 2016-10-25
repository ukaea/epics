TOP=..

include $(TOP)/configure/CONFIG

exampleRPCSRC = $(TOP)/src

INC += pv/exampleRPC.h
INC += pv/device.h
INC += pv/point.h

DBD += exampleRPCRegister.dbd

LIBRARY = exampleRPC
LIBSRCS += exampleRPC.cpp
LIBSRCS += exampleRPCRegister.cpp
LIBSRCS += device.cpp
exampleRPC_LIBS += pvDatabase pvAccess pvData Com

PROD_HOST += exampleRPCMain
exampleRPCMain_SRCS += exampleRPCMain.cpp
exampleRPCMain_LIBS += exampleRPC
exampleRPCMain_LIBS += pvDatabase pvAccess pvData Com

PROD_HOST += control
control_SRCS += positionClient.cpp
control_LIBS += pvAccess pvData Com


include $(TOP)/configure/RULES


