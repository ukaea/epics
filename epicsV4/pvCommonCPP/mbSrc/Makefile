TOP = ..
include $(TOP)/configure/CONFIG

ifdef WITH_MICROBENCH
    USR_CPPFLAGS += -DWITH_MICROBENCH
endif

INSTALL_INCLUDE = $(INSTALL_LOCATION)/include/pv
USR_CPPFLAGS += -I$(INSTALL_LOCATION)/include

INC += mb.h

LIBRARY = pvMB

SHRLIB_VERSION = 4.1.0

pvMB_SRCS += mb.cpp
pvMB_LIBS = Com

ifdef WITH_MICROBENCH
    PROD_HOST += mb_stat

    mb_stat_SRCS += mb_stat.cpp
    mb_stat_LIBS += pvMB Com
endif

include $(TOP)/configure/RULES

