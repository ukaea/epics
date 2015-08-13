# This sets the environment variables following a reboot.
#	Author:  Peter Heesterman (Tessella plc). Date: 13 Aug 2015.
#	Written for CCFE (Culham Centre for Fusion Energy).

DEFAULT_INSTALL_PATH="/usr/local/epics"
if [ -z "$*" ]; then INSTALL_PATH=$DEFAULT_INSTALL_PATH; else INSTALL_PATH=$1;fi

ls ~/.bashrc
echo "" >> ~/.bashrc
echo \#EPICS >> ~/.bashrc
echo . $INSTALL_PATH/siteEnv >> ~/.bashrc
