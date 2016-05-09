TOP = ../..
include $(TOP)/configure/CONFIG

ifdef WITH_MICROBENCH
    PROD_HOST += mb_test
    mb_test_CPPFLAGS += -DWITH_MICROBENCH
endif

mb_test_SRCS += mb_test.cpp
mb_test_LIBS += Com pvMB

include $(TOP)/configure/RULES
