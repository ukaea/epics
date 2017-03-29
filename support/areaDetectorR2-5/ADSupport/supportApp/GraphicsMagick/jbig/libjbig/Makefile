TOP = ../../../..
include $(TOP)/configure/CONFIG

LIBRARY = CORE_jbig

USR_CFLAGS_WIN32 += -DWIN32
USR_CFLAGS_WIN32 += -D_VISUALC_

USR_CFLAGS += -DNeedFunctionPrototypes

USR_INCLUDES += -I$(TOP)/supportApp/GraphicsMagick

#INC += jbig.h
LIB_SRCS += jbig.c
LIB_SRCS += jbig_tab.c

include $(TOP)/configure/RULES
