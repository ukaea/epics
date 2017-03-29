TOP = ../../../../..
include $(TOP)/configure/CONFIG

LIBRARY = CORE_webp

USR_CFLAGS_WIN32 += -DWIN32
USR_CFLAGS_WIN32 += -D_VISUALC_

USR_CFLAGS += -DNeedFunctionPrototypes
USR_CFLAGS += -D_MAGICKLIB_
USR_CFLAGS += -D_WEBPLIB_

USR_INCLUDES += -I$(TOP)/supportApp/GraphicsMagick

LIB_SRCS += bit_reader.c
LIB_SRCS += bit_writer.c
LIB_SRCS += color_cache.c
LIB_SRCS += filters.c
LIB_SRCS += huffman.c
LIB_SRCS += huffman_encode.c
LIB_SRCS += quant_levels.c
LIB_SRCS += quant_levels_dec.c
LIB_SRCS += random.c
LIB_SRCS += rescaler.c
LIB_SRCS += thread.c
LIB_SRCS += utils.c

include $(TOP)/configure/RULES
