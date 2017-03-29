TOP = ../../..
include $(TOP)/configure/CONFIG

LIBRARY = CORE_bzlib

USR_CFLAGS_WIN32 += -DWIN32
USR_CFLAGS_WIN32 += -D_VISUALC_

USR_CFLAGS += -DNeedFunctionPrototypes
USR_CFLAGS += -DBZ_EXPORT

INC += bzlib.h

SRCS += blocksort.c
SRCS += bzlib.c
SRCS += compress.c
SRCS += crctable.c
SRCS += decompress.c
SRCS += huffman.c
SRCS += randtable.c
SRCS_WIN32 += bzlib.rc


include $(TOP)/configure/RULES
