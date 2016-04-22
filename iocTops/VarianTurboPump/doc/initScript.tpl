#!/bin/sh
#
# Linux rc script for ${_DESCRIPTION_}
#
# chkconfig: 345 ${_SEQ_} ${_SEQ_}
# description: ${_DESCRIPTION_}
#

. /etc/rc.d/init.d/functions

IOCNAME=${_IOCNAME_}
INSTALL_DIR=${_INSTALLDIR_}
TOP=/home/epics/src/R3.14.12/base
T_A=_ARCH_
# RUNCMD=
RUNAS=${_ACCT_}
RUNSCRIPT=${_RUNSCRIPT_}
DAEMON=VarianTurboPump
PORTNO=${_PORTNO_}
NETWORK=${_NETWORK_}
# uncomment and set this with any additional procServ command line args
# PROCSERV_OPTS=

source /iocApps/Common/startup.include
