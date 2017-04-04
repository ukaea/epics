TOP = ../../../..
include $(TOP)/configure/CONFIG

LIBRARY = jbig

USR_CFLAGS_WIN32 += -DWIN32
USR_CFLAGS_WIN32 += -D_VISUALC_

USR_CFLAGS += -DNeedFunctionPrototypes

ifeq ($(SHARED_LIBRARIES),YES)
  USR_CFLAGS_WIN32 += -D_DLL
else
  USR_CFLAGS_WIN32 += -D_LIB
endif

USR_INCLUDES += -I$(TOP)/supportApp/GraphicsMagick

#INC += jbig.h
LIB_SRCS += jbig.c
LIB_SRCS += jbig_tab.c
LIB_RCS +=  jbig.rc
include $(TOP)/configure/RULES
