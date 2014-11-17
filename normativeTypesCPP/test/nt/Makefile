TOP=../..

include $(TOP)/configure/CONFIG

PROD_LIBS += nt pvData Com

TESTPROD_HOST += ntfieldTest
ntfieldTest_SRCS += ntfieldTest.cpp
TESTS += ntfieldTest

TESTPROD_HOST += ntscalarTest
ntscalarTest_SRCS += ntscalarTest.cpp
TESTS += ntscalarTest

TESTPROD_HOST += ntscalarArrayTest
ntscalarArrayTest_SRCS += ntscalarArrayTest.cpp
TESTS += ntscalarArrayTest

TESTPROD_HOST += ntnameValueTest
ntnameValueTest_SRCS += ntnameValueTest.cpp
TESTS += ntnameValueTest

TESTPROD_HOST += ntmultiChannelTest
ntmultiChannelTest_SRCS += ntmultiChannelTest.cpp
TESTS += ntmultiChannelTest

TESTPROD_HOST += nttableTest
nttableTest_SRCS = nttableTest.cpp
TESTS += nttableTest

TESTPROD_HOST += ntndarrayTest
ntndarrayTest_SRCS = ntndarrayTest.cpp
TESTS += ntndarrayTest

TESTSCRIPTS_HOST += $(TESTS:%=%.t)

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

