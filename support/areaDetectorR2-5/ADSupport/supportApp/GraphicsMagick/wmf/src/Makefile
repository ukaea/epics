TOP = ../../../..
include $(TOP)/configure/CONFIG

LIBRARY = CORE_wmf

USR_CFLAGS_WIN32 += -DWIN32
USR_CFLAGS_WIN32 += -D_VISUALC_
USR_CFLAGS_WIN32 += -DHAVE_CONFIG_H
USR_CFLAGS       += -D_EXPORT

ifeq ($(SHARED_LIBRARIES),YES)
  USR_CFLAGS_WIN32 += -D_DLL
else
  USR_CFLAGS_WIN32 += -D_LIB
endif

USR_INCLUDES += -I$(TOP)/supportApp/GraphicsMagick
USR_INCLUDES += -I$(TOP)/supportApp/GraphicsMagick/wmf
USR_INCLUDES += -I$(TOP)/supportApp/GraphicsMagick/wmf/include
USR_INCLUDES += -I$(TOP)/supportApp/GraphicsMagick/ttf/include

LIB_SRCS += api.c
LIB_SRCS += bbuf.c
LIB_SRCS += meta.c
LIB_SRCS += player.c
LIB_SRCS += recorder.c

include $(TOP)/configure/RULES
