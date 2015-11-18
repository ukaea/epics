TOP = ../..
include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE

INC += seq_release.h
INC += seq_mask.h
INC += seq_prim_types.h

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

$(COMMON_DIR)/seq_release.h: $(TOP)/configure/CONFIG ../seq_release.pl
	$(PERL) ../seq_release.pl $(SEQ_RELEASE) > $@
