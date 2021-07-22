TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
VERSION_STRING=-r $(VERSION).$(REVISION)
VERSION_NUM=$(shell sh ../../version.sh $(VERSION_STRING) -n )
VERSION_DOT=$(shell sh ../../version.sh $(VERSION_STRING) -d )

#----------------------------------------
# List of edl files
DOCS += Installation.txt
DOCS += runScript.tpl
DOCS += initScript.tpl
DOCS += st.cmd.tpl

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE
