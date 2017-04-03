TOP = ../../..
include $(TOP)/configure/CONFIG

LIBRARY = CORE_bzlib

USR_CFLAGS_WIN32 += -DWIN32
USR_CFLAGS_WIN32 += -D_VISUALC_

ifeq ($(SHARED_LIBRARIES),YES)
  USR_CFLAGS_WIN32 += -D_DLL
else
  USR_CFLAGS_WIN32 += -D_LIB
endif

USR_CFLAGS += -DNeedFunctionPrototypes
USR_CFLAGS += -DBZ_EXPORT

INC += bzlib.h

LIB_SRCS += blocksort.c
LIB_SRCS += bzlib.c
LIB_SRCS += compress.c
LIB_SRCS += crctable.c
LIB_SRCS += decompress.c
LIB_SRCS += huffman.c
LIB_SRCS += randtable.c
LIB_RCS  += bzlib.rc


include $(TOP)/configure/RULES
