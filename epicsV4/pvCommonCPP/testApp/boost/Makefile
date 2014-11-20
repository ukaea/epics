TOP=../..

include $(TOP)/configure/CONFIG

USR_CXXFLAGS_WIN32 += -DBOOST_ALL_NO_LIB

USR_LDFLAGS += -LIBPATH:../../boostApp/boost/stage/lib/

ifdef WITH_MICROBENCH
    PROD_HOST += testBoost
    testBoost_SRCS += testBoost.cpp
endif

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

