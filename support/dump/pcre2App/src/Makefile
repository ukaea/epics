TOP=../..
include $(TOP)/configure/CONFIG

## see file NON-AUTOTOOLS-BUILD in pcre-src for details and file lists to include here
SRC_DIRS += $(TOP)/pcre2-src/src

PROD_HOST += pcre2grep pcre2test pcre2_jit_test
LIBRARY_HOST += pcre2 pcre2posix

ifneq ($(findstring static,$(EPICS_HOST_ARCH)),)
USR_CPPFLAGS += -DPCRE2_STATIC=1
endif

pcre2_SRCS += \
       pcre2_auto_possess.c \
       pcre2_chartables.c \
       pcre2_compile.c \
       pcre2_config.c \
       pcre2_context.c \
       pcre2_convert.c \
       pcre2_dfa_match.c \
       pcre2_error.c \
       pcre2_extuni.c \
       pcre2_find_bracket.c \
       pcre2_jit_compile.c \
       pcre2_maketables.c \
       pcre2_match.c \
       pcre2_match_data.c \
       pcre2_newline.c \
       pcre2_ord2utf.c \
       pcre2_pattern_info.c \
       pcre2_script_run.c \
       pcre2_serialize.c \
       pcre2_string_utils.c \
       pcre2_study.c \
       pcre2_substitute.c \
       pcre2_substring.c \
       pcre2_tables.c \
       pcre2_ucd.c \
       pcre2_valid_utf.c \
       pcre2_xclass.c

INC += pcre2.h pcre2posix.h

pcre2posix_SRCS += pcre2posix.c

pcre2test_SRCS += pcre2test.c
pcre2test_LIBS += pcre2 pcre2posix

pcre2_jit_test_SRCS += pcre2_jit_test.c
pcre2_jit_test_LIBS += pcre2 pcre2posix

pcre2grep_SRCS += pcre2grep.c
pcre2grep_LIBS += pcre2

USR_CPPFLAGS += -DHAVE_CONFIG_H=1 -DPCRE2_CODE_UNIT_WIDTH=8

## define items from config.h
ifeq ($(OS_CLASS),WIN32)
HAVE_UNSIGNED_LONG_LONG=1
HAVE_LONG_LONG=1
USR_CPPFLAGS += -DHAVE_WINDOWS_H=1 -DHAVE_LIMITS_H=1 -DHAVE_SYS_TYPES_H=1
USR_CPPFLAGS += -DHAVE_MEMMOVE=1 -DSUPPORT_JIT=1 -DSUPPORT_PCRE2_8=1
USR_CPPFLAGS += -DSUPPORT_PCRE2GREP_JIT=1 -DHAVE_STDINT_H=1 -DHAVE_STRING=1
USR_CPPFLAGS += -DHAVE_STDLIB_H=1 -DHAVE_STRING_H=1
USR_CPPFLAGS += -DHAVE_UNSIGNED_LONG_LONG=1 -DHAVE_LONG_LONG=1
USR_CPPFLAGS += -DHAVE__STRTOI64=1 -DHAVE_STRERROR=1
else
HAVE_UNSIGNED_LONG_LONG=1
HAVE_LONG_LONG=1
USR_CPPFLAGS += -DHAVE_LIMITS_H=1 -DHAVE_SYS_TYPES_H=1
USR_CPPFLAGS += -DHAVE_MEMMOVE=1 -DSUPPORT_JIT=1 -DSUPPORT_PCRE2_8=1
USR_CPPFLAGS += -DSUPPORT_PCRE2GREP_JIT=1 -DHAVE_STDINT_H=1 -DHAVE_STRING=1
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
pcre2_auto_possess.c : ../O.Common/config.h

../O.Common/config.h : $(TOP)/pcre2-src/src/config.h.generic
	$(CP) $< $@

../O.Common/pcre2.h : $(TOP)/pcre2-src/src/pcre2.h.generic
	$(CP) $< $@

pcre2_chartables.c : $(TOP)/pcre2-src/src/pcre2_chartables.c.dist
	$(CP) $< $@
