TOP = ../../..
include $(TOP)/configure/CONFIG

LIBRARY = CORE_png

USR_CFLAGS_WIN32 += -DWIN32
USR_CFLAGS_WIN32 += -D_VISUALC_

USR_CFLAGS += -DNeedFunctionPrototypes
USR_CFLAGS += -D_PNGLIB_
USR_CFLAGS += -DPNG_NO_MODULEDEF
USR_CFLAGS += -D__FLAT__

USR_CFLAGS_WIN32 += -D__WIN32DLL__

ifeq ($(SHARED_LIBRARIES),YES)
  USR_CFLAGS_WIN32 += -D_DLL
  USR_CFLAGS_WIN32 += -D_MAGICKMOD_
  USR_CFLAGS_WIN32 += -DPNG_BUILD_DLL
else
  USR_CFLAGS_WIN32 += -D_LIB
endif

LIB_SRCS +=png.c
LIB_SRCS += pngerror.c
LIB_SRCS += pngget.c
LIB_SRCS += pngmem.c
LIB_SRCS += pngpread.c
LIB_SRCS += pngrio.c
LIB_SRCS += pngrtran.c
LIB_SRCS += pngrutil.c
LIB_SRCS += pngset.c
LIB_SRCS += pngtrans.c
LIB_SRCS += pngwio.c
LIB_SRCS += pngwrite.c
LIB_SRCS += pngwtran.c
LIB_SRCS += pngwutil.c
LIB_SRCS += pngread.c

LIB_RCS  += png.rc

LIB_LIBS += CORE_bzlib zlib

include $(TOP)/configure/RULES
