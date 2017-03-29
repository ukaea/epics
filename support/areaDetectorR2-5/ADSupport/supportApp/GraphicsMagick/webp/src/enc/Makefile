TOP = ../../../../..
include $(TOP)/configure/CONFIG

LIBRARY = CORE_webp

USR_CFLAGS_WIN32 += -DWIN32
USR_CFLAGS_WIN32 += -D_VISUALC_

USR_CFLAGS += -DNeedFunctionPrototypes
USR_CFLAGS += -D_MAGICKLIB_
USR_CFLAGS += -D_WEBPLIB_

USR_INCLUDES += -I$(TOP)/supportApp/GraphicsMagick

LIB_SRCS += vp8l.c
LIB_SRCS += tree.c
LIB_SRCS += token.c
LIB_SRCS += syntax.c
LIB_SRCS += near_lossless.c
LIB_SRCS += picture.c
LIB_SRCS += picture_csp.c
LIB_SRCS += picture_psnr.c
LIB_SRCS += picture_rescale.c
LIB_SRCS += picture_tools.c
LIB_SRCS += quant.c
LIB_SRCS += iterator.c
LIB_SRCS += histogram.c
LIB_SRCS += frame.c
LIB_SRCS += filter.c
LIB_SRCS += config.c
LIB_SRCS += cost.c
LIB_SRCS += delta_palettization.c
LIB_SRCS += backward_references.c
LIB_SRCS += alpha.c
LIB_SRCS += analysis.c
LIB_SRCS += webpenc.c

include $(TOP)/configure/RULES
