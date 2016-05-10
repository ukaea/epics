TOP=..

include $(TOP)/configure/CONFIG

exampleRPCSRC = $(TOP)/src

INC += pv/exampleRPC.h

DBD += exampleRPCRegister.dbd

LIBRARY = exampleRPC
LIBSRCS += exampleRPC.cpp
LIBSRCS += exampleRPCRegister.cpp
exampleRPC_LIBS += pvDatabase pvAccess pvData Com

PROD_HOST += exampleRPCMain
exampleRPCMain_SRCS += exampleRPCMain.cpp
exampleRPCMain_LIBS += exampleRPC
exampleRPCMain_LIBS += pvDatabase pvAccess pvData Com

PROD_HOST += move
move_SRCS += positionClient.cpp
move_LIBS += pvAccess pvData Com


include $(TOP)/configure/RULES


