TOP = ../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE

INC += pv.h pvAlarm.h pvType.h

LIBRARY += pv

pv_SRCS += pv.c
pv_LIBS += ca Com

# For R3.13 compatibility only
OBJLIB_vxWorks = pv
OBJLIB_SRCS = $(pv_SRCS)

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE
