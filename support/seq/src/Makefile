TOP = ..
include $(TOP)/configure/CONFIG

DIRS += pv
DIRS += common

DIRS += seq
seq_DEPEND_DIRS += pv
seq_DEPEND_DIRS += common

DIRS += lemon

DIRS += snc
seq_DEPEND_DIRS += common
snc_DEPEND_DIRS += lemon

include $(TOP)/configure/RULES_DIRS
