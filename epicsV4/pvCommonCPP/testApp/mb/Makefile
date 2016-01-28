TOP = ../..
include $(TOP)/configure/CONFIG

ifdef WITH_MICROBENCH
PROD_HOST += mb_test
endif

mb_test_CXXFLAGS += -DPV_MB
USR_LDFLAGS += -LIBPATH:../../../boostApp/boost/stage/lib/
mb_test_SRCS += mb_test.cpp
mb_test_LIBS += Com pvMB

include $(TOP)/configure/RULES
