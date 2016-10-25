# Copyright information and license terms for this software can be
# found in the file LICENSE that is included with the distribution

# Build the EPICS V4 C++ Bundle

# These are our main submodules
MODULES += pvCommonCPP
MODULES += pvDataCPP
MODULES += normativeTypesCPP
MODULES += pvAccessCPP
MODULES += pvaClientCPP
MODULES += pvaSrv
MODULES += pvDatabaseCPP
MODULES += exampleCPP
# pvaPy must not appear in MODULES, it's special

# Dependencies between modules, also used for RELEASE files
        pvDataCPP_DEPENDS_ON = pvCommonCPP
normativeTypesCPP_DEPENDS_ON = pvDataCPP
      pvAccessCPP_DEPENDS_ON = pvDataCPP
     pvaClientCPP_DEPENDS_ON = pvAccessCPP normativeTypesCPP
           pvaSrv_DEPENDS_ON = pvAccessCPP
            pvaPy_DEPENDS_ON = pvaClientCPP
    pvDatabaseCPP_DEPENDS_ON = pvAccessCPP
       exampleCPP_DEPENDS_ON = pvDatabaseCPP pvaSrv pvaClientCPP

# Embedded tops, which also need RELEASE files
    pvaSrv_CONTAINS_TOPS := testTop exampleTop
exampleCPP_CONTAINS_TOPS := database exampleClient exampleLink powerSupply
exampleCPP_CONTAINS_TOPS += helloPutGet helloRPC pvDatabaseRPC arrayPerformance

# Build tools
PERL = perl
RM = $(PERL) -MExtUtils::Command -e rm_f

# Set EPICS_HOST_ARCH if necessary
ifeq ($(origin EPICS_HOST_ARCH),undefined)
  export EPICS_HOST_ARCH := $(shell \
        $(PERL) $(EPICS_BASE)/startup/EpicsHostArch.pl)
endif

# Name of generated RELEASE files
RELEASE = RELEASE.$(EPICS_HOST_ARCH).Common

# If it exists the bundle's RELEASE file sets EPICS_BASE
ifneq ($(wildcard $(RELEASE)),)
  include $(RELEASE)
else
  # Make sure EPICS_BASE is set
  ifeq ($(wildcard $(EPICS_BASE)),)
    $(error EPICS_BASE is not set or doesn't exist)
  endif
endif

# Internal build targets
BUILD_TARGETS = $(MODULES:%=build.%)
HOST_TARGETS = $(MODULES:%=host.%)
DOXYGEN_TARGETS = $(addprefix doxygen.,$(filter-out exampleCPP,$(MODULES)))
PYTHON_TARGETS = host.pvaPy
SPHINX_TARGETS = sphinx.pvaPy
RUNTESTS_TARGETS = $(MODULES:%=runtests.%)
TAPFILES_TARGETS = $(MODULES:%=tapfiles.%)
CLEAN_TARGETS = $(MODULES:%=clean.%) clean.pvaPy
DISTCLEAN_TARGETS = $(MODULES:%=distclean.%) distclean.pvaPy
CLEAN_DEP = $(filter clean distclean,$(MAKECMDGOALS))
CONFIG_TARGETS = $(MODULES:%=config.%) $(foreach module, $(MODULES), \
    $(foreach top, $($(module)_CONTAINS_TOPS), config.$(module)/$(top)))
DECONF_TARGETS = $(MODULES:%=deconf.%) $(foreach module, $(MODULES), \
    $(foreach top, $($(module)_CONTAINS_TOPS), deconf.$(module)/$(top)))

# Public build targets
all: $(BUILD_TARGETS)
host: $(HOST_TARGETS)
doxygen: $(DOXYGEN_TARGETS)
python : $(PYTHON_TARGETS)
sphinx: $(SPHINX_TARGETS)
runtests: $(RUNTESTS_TARGETS)
tapfiles: $(TAPFILES_TARGETS)
clean: $(CLEAN_TARGETS)
distclean: $(DISTCLEAN_TARGETS) deconf
rebuild: clean
	$(MAKE) all
config: $(CONFIG_TARGETS)
deconf: $(DECONF_TARGETS) deconf.pvaPy
	$(RM) $(RELEASE)

# Generic build rules
$(MODULES): % : build.%

$(BUILD_TARGETS): build.% : $(CLEAN_DEP) config
	$(MAKE) -C $* all

$(HOST_TARGETS) host.pvaPy: host.% : $(CLEAN_DEP) config
	$(MAKE) -C $* $(EPICS_HOST_ARCH)

$(DOXYGEN_TARGETS): doxygen.% :
	cd $* && doxygen

$(RUNTESTS_TARGETS): runtests.% :
	$(MAKE) -C $* runtests CROSS_COMPILER_TARGET_ARCHS=

$(TAPFILES_TARGETS): tapfiles.% :
	$(MAKE) -C $* tapfiles CROSS_COMPILER_TARGET_ARCHS=

$(CLEAN_TARGETS): clean.% :
	$(MAKE) -C $* clean

$(DISTCLEAN_TARGETS): distclean.% :
	$(MAKE) -C $* distclean

# Expand %_DEPENDS_ON into %_DEPENDS_ALL
$(foreach module, $(MODULES), $(eval $(module)_DEPENDS_ALL = \
    $(foreach dep, $($(module)_DEPENDS_ON), $(dep) $($(dep)_DEPENDS_ALL))))

# Set %_DEPENDS_ALL for embedded tops too
$(foreach module, $(MODULES), $(foreach top, $($(module)_CONTAINS_TOPS), \
    $(eval $(module)/$(top)_DEPENDS_ALL = $(module) $($(module)_DEPENDS_ALL))))

# pvaPy configuration settings
PVAPY_CONFIG := EPICS_BASE=$(EPICS_BASE)
PVAPY_CONFIG += EPICS4_DIR=$(abspath .)
ifneq ($(wildcard $(BOOST_NUMPY)),)
  PVAPY_CONFIG += BOOST_NUM_PY_DIR=$(BOOST_NUMPY)
endif

ifeq ($(filter sphinx,$(MAKECMDGOALS)),sphinx)
  PYTHON_VER = $(shell python -c 'import sys; print sys.version[:3]')
  PYTHON_LIB = $(abspath pvaPy)/lib/python/$(PYTHON_VER)/$(EPICS_HOST_ARCH)
  ifeq ($(origin PYTHONPATH),undefined)
    export PYTHONPATH := $(PYTHON_LIB)
  else
    export PYTHONPATH := $(PYTHONPATH):$(PYTHON_LIB)
  endif
endif

# Special rules for pvaPy
pvaPy: host.pvaPy
config.pvaPy: pvaPy/configure/RELEASE.local
pvaPy/configure/RELEASE.local: host.pvaClientCPP
	$(MAKE) -C pvaPy configure $(PVAPY_CONFIG)
sphinx.pvaPy:
	$(MAKE) -C pvaPy doc
deconf.pvaPy:
	$(RM) pvaPy/configure/RELEASE.local pvaPy/configure/CONFIG_SITE.local

# Generic config rules
$(CONFIG_TARGETS): config.% : %/configure/$(RELEASE)
%/configure/$(RELEASE): | $(RELEASE)
	$(PERL) tools/genRelease.pl -o $@ -B $(EPICS_BASE) $($*_DEPENDS_ALL)
$(RELEASE):
	$(PERL) tools/genRelease.pl -o $@ -B $(EPICS_BASE)
$(DECONF_TARGETS): deconf.% :
	$(RM) $*/configure/$(RELEASE)

# Module build dependencies
define MODULE_DEPS_template
  $(1).$(2): $$(foreach dep, $$($(2)_DEPENDS_ON), \
      $$(addprefix $(1).,$$(dep))) $(2)/configure/$(RELEASE)
endef
# Dependencies for build.%
$(foreach module, $(MODULES), \
    $(eval $(call MODULE_DEPS_template,build,$(module))))
# Dependencies for host.%
$(foreach module, $(MODULES) pvaPy, \
    $(eval $(call MODULE_DEPS_template,host,$(module))))

# GNUmake hints
.PHONY: all host python runtests tapfiles clean distclean rebuild config deconf
.PHONY: $(BUILD_TARGETS) $(HOST_TARGETS) $(RUNTESTS_TARGETS) $(TAPFILES_TARGETS)
.PHONY: $(CLEAN_TARGETS) $(DISTCLEAN_TARGETS)
.PHONY: $(MODULES) pvaPy $(CONFIG_TARGETS) $(DECONF_TARGETS)
