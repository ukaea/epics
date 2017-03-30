TOP = ../../..
include $(TOP)/configure/CONFIG

LIBRARY = CORE_png

USR_CFLAGS_WIN32 += -DWIN32
USR_CFLAGS_WIN32 += -D_VISUALC_

USR_CFLAGS += -DNeedFunctionPrototypes
USR_CFLAGS += -D_PNGLIB_
USR_CFLAGS += -DPNG_NO_MODULEDEF

USR_CFLAGS_WIN32 += -D__WIN32DLL__

ifeq ($(SHARED_LIBRARIES),YES)
  USR_CFLAGS_WIN32 += -D_DLL
else
  USR_CFLAGS_WIN32 += -D_LIB
endif

LIB_SRCS +=png.c
#D:\downloads\GraphicsMagick\GraphicsMagick-1.3.25\VisualMagick\png\png.rc
LIB_SRCS += pngerror.c
LIB_SRCS += pngget.c
LIB_SRCS += pngmem.c
LIB_SRCS += pngpread.c
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

include $(TOP)/configure/RULES
