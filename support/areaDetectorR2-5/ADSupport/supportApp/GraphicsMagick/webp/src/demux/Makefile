TOP = ../../../../..
include $(TOP)/configure/CONFIG

LIBRARY = CORE_webp

USR_CFLAGS_WIN32 += -DWIN32
USR_CFLAGS_WIN32 += -D_VISUALC_

USR_CFLAGS += -DNeedFunctionPrototypes
USR_CFLAGS += -D_MAGICKLIB_
USR_CFLAGS += -D_WEBPLIB_

USR_INCLUDES += -I$(TOP)/supportApp/GraphicsMagick

LIB_SRCS += anim_decode.c
LIB_SRCS += demux.c

include $(TOP)/configure/RULES
