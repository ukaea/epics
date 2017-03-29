TOP = ../../..
include $(TOP)/configure/CONFIG

LIBRARY = CORE_png

USR_CFLAGS_WIN32 += -DWIN32
USR_CFLAGS_WIN32 += -D_VISUALC_

USR_CFLAGS += -DNeedFunctionPrototypes
USR_CFLAGS += -D_MAGICKLIB_

#USR_INCLUDES += -I$(TOP)/supportApp/GraphicsMagick

#INC += png.h
#INC += pnglibconf.h
#INC += pngconf.h

LIB_SRCS +=png.c
#D:\downloads\GraphicsMagick\GraphicsMagick-1.3.25\VisualMagick\png\png.rc
LIB_SRCS +=pngerror.c
LIB_SRCS +=pngget.c
LIB_SRCS +=pngmem.c
LIB_SRCS +=pngpread.c
LIB_SRCS +=pngpread.c
LIB_SRCS +=pngrio.c
LIB_SRCS +=pngrtran.c
LIB_SRCS +=pngrutil.c
LIB_SRCS +=pngset.c
LIB_SRCS +=pngtrans.c
LIB_SRCS +=pngwio.c
LIB_SRCS +=pngwrite.c
LIB_SRCS +=pngwtran.c
LIB_SRCS +=pngwutil.c

include $(TOP)/configure/RULES
