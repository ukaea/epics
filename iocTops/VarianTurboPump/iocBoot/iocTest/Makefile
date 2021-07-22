TOP = ../..
include $(TOP)/configure/CONFIG
ARCH = linux-x86_64
TARGETS = envPaths simVarianTurbo_IP.pl simVarianTurbo.pl
include $(TOP)/configure/RULES.ioc

simVarianTurbo_IP.pl:	simStreamDevice.tpl simVarianTurbo.tpl
	cat simStreamDevice.tpl simVarianTurbo.tpl > $@

simVarianTurbo.pl:	simSerialDevice.tpl simVarianTurbo.tpl
	cat simSerialDevice.tpl simVarianTurbo.tpl > $@
