TOP = ../../..
include $(TOP)/configure/CONFIG

# List here, the edl files to be be created using "velocity"
# Requires that you have the "*.v" file
#EDL_FROM_V += <name>.edl
#EDL_FROM_V += <name>.edl

buildInstall: $(EDL_FROM_V)

DATA += $(patsubst ../%, %, $(wildcard ../*.edl))
DATA += $(patsubst ../%, %, $(wildcard ../*.gif))
DATA += $(patsubst ../%, %, $(wildcard ../*.png))

clean::
	rm -f $(EDL_FROM_V) velocity.log

uninstall:
	$(foreach file,$(EDL_FROM_V),rm -f $(file))

include $(TOP)/configure/RULES


%.edl: %.v
	velocity -a $< > $@

%.edl: ../%.v
	velocity -a $< > ../$@
