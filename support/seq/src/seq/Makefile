TOP = ../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE

#  Include files
INC += seqCom.h
INC += seqStats.h
INC += seq_snc.h

#  seq library
LIBRARY = seq
seq_LIBS = pv Com

seq_SRCS += seq_main.c
seq_SRCS += seq_task.c
seq_SRCS += seq_ca.c
seq_SRCS += seq_if.c
seq_SRCS += seq_mac.c
seq_SRCS += seq_prog.c
seq_SRCS += seq_qry.c
seq_SRCS += seq_cmd.c
seq_SRCS += seq_queue.c

# For R3.13 compatibility only
OBJLIB_vxWorks = seq
OBJLIB_SRCS = $(seq_SRCS)

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE
