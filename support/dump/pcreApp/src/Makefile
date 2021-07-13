TOP=../..
include $(TOP)/configure/CONFIG

## see file NON-AUTOTOOLS-BUILD in pcre-src for details and file lists to include here
SRC_DIRS += $(TOP)/pcre-src

PROD_HOST += pcregrep pcretest pcre_jit_test pcrecpp_unittest pcre_scanner_unittest pcre_stringpiece_unittest
LIBRARY_HOST += pcre pcrecpp pcreposix

ifneq ($(findstring static,$(EPICS_HOST_ARCH)),)
USR_CPPFLAGS += -DPCRE_STATIC=1
endif

pcre_SRCS += \
       pcre_byte_order.c \
       pcre_chartables.c \
       pcre_compile.c \
       pcre_config.c \
       pcre_dfa_exec.c \
       pcre_exec.c \
       pcre_fullinfo.c \
       pcre_get.c \
       pcre_globals.c \
       pcre_jit_compile.c \
       pcre_maketables.c \
       pcre_newline.c \
       pcre_ord2utf8.c \
       pcre_refcount.c \
       pcre_string_utils.c \
       pcre_study.c \
       pcre_tables.c \
       pcre_ucd.c \
       pcre_valid_utf8.c \
       pcre_version.c \
       pcre_xclass.c

INC += pcre.h pcreposix.h pcrecpp.h pcrecpparg.h pcre_stringpiece.h pcre_scanner.h

pcrecpp_SRCS += pcrecpp.cc pcre_scanner.cc pcre_stringpiece.cc
pcrecpp_LIBS += pcre

pcreposix_SRCS += pcreposix.c

pcretest_SRCS += pcretest.c pcre_printint.c
pcretest_LIBS += pcre pcreposix

pcre_jit_test_SRCS += pcre_jit_test.c
pcre_jit_test_LIBS += pcre pcreposix

pcregrep_SRCS += pcregrep.c
pcregrep_LIBS += pcre

pcrecpp_unittest_SRCS += pcrecpp_unittest.cc
pcrecpp_unittest_LIBS += pcrecpp pcre

pcre_scanner_unittest_SRCS += pcre_scanner_unittest.cc
pcre_scanner_unittest_LIBS += pcrecpp pcre

pcre_stringpiece_unittest_SRCS += pcre_stringpiece_unittest.cc
pcre_stringpiece_unittest_LIBS += pcrecpp pcre

USR_CPPFLAGS += -DHAVE_CONFIG_H=1

## define items from config.h
ifeq ($(OS_CLASS),WIN32)
HAVE_UNSIGNED_LONG_LONG=1
HAVE_LONG_LONG=1
USR_CPPFLAGS += -DHAVE_WINDOWS_H=1 -DHAVE_LIMITS_H=1 -DHAVE_SYS_TYPES_H=1
USR_CPPFLAGS += -DHAVE_MEMMOVE=1 -DSUPPORT_JIT=1 -DSUPPORT_PCRE8=1
USR_CPPFLAGS += -DSUPPORT_PCREGREP_JIT=1 -DHAVE_STDINT_H=1 -DHAVE_STRING=1
USR_CPPFLAGS += -DHAVE_STDLIB_H=1 -DHAVE_STRING_H=1
USR_CPPFLAGS += -DHAVE_UNSIGNED_LONG_LONG=1 -DHAVE_LONG_LONG=1
USR_CPPFLAGS += -DHAVE__STRTOI64=1 -DHAVE_STRERROR=1
else
HAVE_UNSIGNED_LONG_LONG=1
HAVE_LONG_LONG=1
USR_CPPFLAGS += -DHAVE_LIMITS_H=1 -DHAVE_SYS_TYPES_H=1
USR_CPPFLAGS += -DHAVE_MEMMOVE=1 -DSUPPORT_JIT=1 -DSUPPORT_PCRE8=1
USR_CPPFLAGS += -DSUPPORT_PCREGREP_JIT=1 -DHAVE_STDINT_H=1 -DHAVE_STRING=1
USR_CPPFLAGS += -DHAVE_STDLIB_H=1 -DHAVE_STRING_H=1
USR_CPPFLAGS += -DHAVE_UNSIGNED_LONG_LONG=$(HAVE_UNSIGNED_LONG_LONG) -DHAVE_LONG_LONG=$(HAVE_LONG_LONG)
USR_CPPFLAGS += -DHAVE_STRTOLL=1 -DHAVE_STRERROR=1 -DHAVE_UNISTD_H=1
endif

include $(TOP)/configure/RULES

ifeq ($(OS_CLASS),WIN32)
runtests :
	..\run_tests.bat
endif

## this just forces a reference, it is not listed in INC and so otherwise does not get built
pcre_byte_order.c : ../O.Common/config.h

../O.Common/config.h : $(TOP)/pcre-src/config.h.generic
	$(CP) $< $@

../O.Common/pcre.h : $(TOP)/pcre-src/pcre.h.generic
	$(CP) $< $@

../O.Common/pcrecpparg.h : $(TOP)/pcre-src/pcrecpparg.h.in
	$(EXPAND_TOOL) $(EXPANDARCH) $(EXPANDFLAGS) -Dpcre_have_long_long=$(HAVE_LONG_LONG) -Dpcre_have_ulong_long=$(HAVE_UNSIGNED_LONG_LONG) $< $@

../O.Common/pcre_stringpiece.h : $(TOP)/pcre-src/pcre_stringpiece.h.in
	$(EXPAND_TOOL) $(EXPANDARCH) $(EXPANDFLAGS) -Dpcre_have_type_traits=0 -Dpcre_have_bits_type_traits=0 $< $@

pcre_chartables.c : $(TOP)/pcre-src/pcre_chartables.c.dist
	$(CP) $< $@
