TOP = ../../..
include $(TOP)/configure/CONFIG

LIBRARY = filters

USR_CFLAGS_WIN32 += -DWIN32
USR_CFLAGS_WIN32 += -D_VISUALC_

USR_CFLAGS += -DNeedFunctionPrototypes
USR_CFLAGS += -D_MAGICKLIB_

ifeq ($(SHARED_LIBRARIES),YES)
  USR_CFLAGS_WIN32 += -D_DLL
else
  USR_CFLAGS_WIN32 += -D_LIB
endif

USR_INCLUDES += -I$(TOP)/supportApp/GraphicsMagick
USR_INCLUDES += -I$(TOP)/supportApp/GraphicsMagick/lcms/inc

LIB_SRCS += analyze.c

LIB_LIBS += magick ttf

include $(TOP)/configure/RULES
