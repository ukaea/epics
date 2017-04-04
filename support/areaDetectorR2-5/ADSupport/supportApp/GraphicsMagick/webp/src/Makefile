TOP = ../../../..
include $(TOP)/configure/CONFIG

LIBRARY = webp

USR_CFLAGS_WIN32 += -DWIN32
USR_CFLAGS_WIN32 += -D_VISUALC_

USR_CFLAGS += -DNeedFunctionPrototypes
USR_CFLAGS += -D_WEBPLIB_

ifeq ($(SHARED_LIBRARIES),YES)
  USR_CFLAGS_WIN32 += -D_DLL
else
  USR_CFLAGS_WIN32 += -D_LIB
endif

USR_INCLUDES += -I$(TOP)/supportApp/GraphicsMagick

WEBP = $(TOP)/supportApp/GraphicsMagick/webp
WEBPSRC = $(WEBP)/src

include $(WEBPSRC)/dec/Makefile
include $(WEBPSRC)/demux/Makefile
include $(WEBPSRC)/dsp/Makefile
include $(WEBPSRC)/enc/Makefile
include $(WEBPSRC)/mux/Makefile
include $(WEBPSRC)/utils/Makefile

include $(TOP)/configure/RULES
