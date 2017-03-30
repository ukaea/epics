TOP = ../../../..
include $(TOP)/configure/CONFIG

LIBRARY = CORE_wmf

USR_CXXFLAGS_WIN32 += -DWIN32
USR_CXXFLAGS_WIN32 += -D_VISUALC_
USR_CXXFLAGS_WIN32 += -DHAVE_CONFIG_H

USR_CXXFLAGS += -DNeedFunctionPrototypes

USR_INCLUDES += -I$(TOP)/supportApp/GraphicsMagick
USR_INCLUDES += -I$(TOP)/supportApp/GraphicsMagick/wmf/inc
USR_INCLUDES += -I$(TOP)/supportApp/GraphicsMagick/ttf/inc

LIB_SRCS += api.c
LIB_SRCS += bbuf.c
LIB_SRCS += meta.c
LIB_SRCS += player.c
LIB_SRCS += recorder.c

include $(TOP)/configure/RULES
