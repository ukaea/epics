TOP = ..
include $(TOP)/configure/CONFIG

ifdef WITH_MICROBENCH
    USR_CPPFLAGS += -DWITH_MICROBENCH -I$(INSTALL_LOCATION)/include
endif

INSTALL_INCLUDE = $(INSTALL_LOCATION)/include/pv

INC += mb.h

LIBRARY = pvMB

pvMB_SRCS += mb.cpp
pvMB_LIBS = Com

ifdef WITH_MICROBENCH
    PROD_HOST += mb_stat

    mb_stat_SRCS += mb_stat.cpp
    mb_stat_LIBS += pvMB Com
endif

include $(TOP)/configure/RULES

