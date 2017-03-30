TOP = ../../../../..
include $(TOP)/configure/CONFIG

LIBRARY = CORE_jp2

USR_CFLAGS_WIN32 += -DWIN32
USR_CFLAGS_WIN32 += -D_VISUALC_
USR_CFLAGS_WIN32 += -DJAS_WIN_MSVC_BUILD

USR_CFLAGS += -DEXCLUDE_MIF_SUPPORT
USR_CFLAGS += -DEXCLUDE_PNM_SUPPORT
USR_CFLAGS += -DEXCLUDE_BMP_SUPPORT
USR_CFLAGS += -DEXCLUDE_RAS_SUPPORT
USR_CFLAGS += -DEXCLUDE_JPG_SUPPORT
USR_CFLAGS += -DEXCLUDE_PGX_SUPPORT

USR_CFLAGS += -DNeedFunctionPrototypes

USR_INCLUDES += -I$(TOP)/supportApp/GraphicsMagick

LIBJASPER = $(TOP)/supportApp/GraphicsMagick/jp2/src/libJasper

USR_INCLUDES += -I$(LIBJASPER)/inc

#include $(LIBJASPER)/inc/Makefile
include $(LIBJASPER)/base/Makefile
include $(LIBJASPER)/jp2/Makefile
include $(LIBJASPER)/jpc/Makefile

include $(TOP)/configure/RULES
