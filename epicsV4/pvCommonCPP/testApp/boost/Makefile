TOP=../..

include $(TOP)/configure/CONFIG

USR_CXXFLAGS_WIN32 += -DBOOST_ALL_NO_LIB

PROD_DEFAULT += testShared
PROD_RTEMS += -nil-

testShared_SRCS += testShared.cpp

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

