# This sets the environment variables following a reboot.
#	Author:  Peter Heesterman (Tessella plc). Date: 13 Aug 2015.
#	Written for CCFE (Culham Centre for Fusion Energy).

DEFAULT_EPICS_ROOT="/usr/local/epics"
if [ -z "$*" ]; then EPICS_ROOT=$DEFAULT_EPICS_ROOT; else EPICS_ROOT=$1;fi

#So the environment is set for this shell, too.
. $EPICS_ROOT/siteEnv

ls ~/.bashrc
echo "" >> ~/.bashrc
echo \#EPICS >> ~/.bashrc
echo . $EPICS_ROOT/siteEnv >> ~/.bashrc
