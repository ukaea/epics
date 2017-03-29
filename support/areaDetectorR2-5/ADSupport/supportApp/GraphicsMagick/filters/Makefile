TOP = ../../..
include $(TOP)/configure/CONFIG

LIBRARY = CORE_filters

USR_CFLAGS_WIN32 += -DWIN32
USR_CFLAGS_WIN32 += -D_VISUALC_

USR_CFLAGS += -DNeedFunctionPrototypes
USR_CFLAGS += -D_MAGICKLIB_

USR_INCLUDES += -I$(TOP)/supportApp/GraphicsMagick
USR_INCLUDES += -I$(TOP)/supportApp/GraphicsMagick/lcms/inc

LIB_SRCS += analyze.c

include $(TOP)/configure/RULES
