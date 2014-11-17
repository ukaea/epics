TOP = ..
include $(TOP)/configure/CONFIG

SRC = $(TOP)/src/

SRC_DIRS += $(SRC)/nt

INC += nt.h
INC += ntfield.h
INC += ntscalar.h
INC += ntscalarArray.h
INC += ntnameValue.h
INC += nttable.h
INC += ntmultiChannel.h
INC += ntndarray.h

LIBSRCS += ntfield.cpp
LIBSRCS += ntscalar.cpp
LIBSRCS += ntscalarArray.cpp
LIBSRCS += ntnameValue.cpp
LIBSRCS += nttable.cpp
LIBSRCS += ntmultiChannel.cpp
LIBSRCS += ntndarray.cpp

LIBRARY=nt

nt_LIBS += pvData Com

include $(TOP)/configure/RULES

