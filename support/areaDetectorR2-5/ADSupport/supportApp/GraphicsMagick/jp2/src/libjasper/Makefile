TOP = ../../../../..
include $(TOP)/configure/CONFIG

LIBRARY = CORE_jp2

USR_CFLAGS_WIN32 += -DWIN32
USR_CFLAGS_WIN32 += -D_VISUALC_

USR_CFLAGS += -DNeedFunctionPrototypes

USR_INCLUDES += -I$(TOP)/supportApp/GraphicsMagick

LIBJASPER = $(TOP)/supportApp/GraphicsMagick/jp2/src/libJasper

USR_INCLUDES += -I$(LIBJASPER)/inc

#include $(LIBJASPER)/inc/Makefile
include $(LIBJASPER)/base/Makefile
include $(LIBJASPER)/jp2/Makefile
include $(LIBJASPER)/jpc/Makefile

include $(TOP)/configure/RULES
