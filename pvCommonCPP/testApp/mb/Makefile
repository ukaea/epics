TOP=../..

include $(TOP)/configure/CONFIG

PROD_HOST += mb_test
mb_test_CXXFLAGS += -DPV_MB
USR_LDFLAGS += -LIBPATH:../../../boostApp/boost/stage/lib/
mb_test_SRCS += mb_test.cpp
mb_test_LIBS += Com pvMB

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

