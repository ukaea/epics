TOP=..

include $(TOP)/configure/CONFIG

CFG += CONFIG_PVDATABASE_VERSION

TARGETS = $(CONFIG_TARGETS)
CONFIGS += $(subst ../,,$(wildcard $(CONFIG_INSTALLS)))

include $(TOP)/configure/RULES

