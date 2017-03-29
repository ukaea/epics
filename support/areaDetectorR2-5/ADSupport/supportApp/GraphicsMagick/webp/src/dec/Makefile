TOP = ../../../../..
include $(TOP)/configure/CONFIG

LIBRARY = CORE_webp

USR_CFLAGS_WIN32 += -DWIN32
USR_CFLAGS_WIN32 += -D_VISUALC_

USR_CFLAGS += -DNeedFunctionPrototypes
USR_CFLAGS += -D_MAGICKLIB_
USR_CFLAGS += -D_WEBPLIB_

USR_INCLUDES += -I$(TOP)/supportApp/GraphicsMagick

LIB_SRCS += alpha.c
LIB_SRCS += buffer.c
LIB_SRCS += frame.c
LIB_SRCS += idec.c
LIB_SRCS += io.c
LIB_SRCS += quant.c
LIB_SRCS += tree.c
LIB_SRCS += vp8.c
LIB_SRCS += vp8l.c
LIB_SRCS += webp.c

include $(TOP)/configure/RULES
