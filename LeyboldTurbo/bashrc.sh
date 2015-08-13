# This sets the environment variables following a reboot.

DEFAULT_INSTALL_PATH="/usr/local/epics"
if [ -z "$*" ]; then INSTALL_PATH=$DEFAULT_INSTALL_PATH; else INSTALL_PATH=$1;fi

echo "" >> ~/.bashrc
echo \#EPICS >> ~/.bashrc
echo . $INSTALL_PATH/siteEnv >> ~/.bashrc
ls ~/.bashrc
