#!/bin/bash 

#copyright (c) Tessella 2014

# this script installs EPICS base 3.14.12.3, set necessary environment variables
# as well as the extension directory file structure (so it's ready to install extensions)

# check if user has right permissions
if [ "$(id -u)" != "0" ]; then
	echo "Sorry, you are not root. Please try again using sudo."
	exit 1
fi

# terminate script after first line that fails
set -e

# 32 or 64bit?
case `uname -m` in
  i[3456789]86|x86|i86pc)
    EPICS_ARCH='linux-x86'
    ;;
  x86_64|amd64|AMD64)
    EPICS_ARCH='linux-x86_64'
    ;;
  *)
    echo "Unknown architecture `uname -m`."
    exit 1
    ;;
esac

# get installation directory from command line argument
DEFAULT_INSTALL_PATH="/usr/local/epics"
if [ -z "$*" ]; then INSTALL_PATH=$DEFAULT_INSTALL_PATH; else INSTALL_PATH=$1;fi

if [ ! -f /etc/redhat-release ];
then
    # see http://stackoverflow.com/questions/6486449/the-problem-of-using-sudo-apt-get-install-build-essentials
    apt-get update

    # install dependencies
    apt-get -y install build-essential g++ libreadline-dev
fi

# base
BASE_DOWNLOAD="base-3.15.1.tar.gz"
BASE_DIRECTORY="base-3.15.1"
wget --tries=3 --timeout=10 http://aps.anl.gov/epics/download/base/$BASE_DOWNLOAD
mkdir -p $INSTALL_PATH
tar xzvf $BASE_DOWNLOAD -C $INSTALL_PATH
rm -f $BASE_DOWNLOAD
make -C $INSTALL_PATH/$BASE_DIRECTORY install
ln -s $INSTALL_PATH/$BASE_DIRECTORY $INSTALL_PATH/base

# set environment variables
touch $INSTALL_PATH/siteEnv
echo \# main EPICS env var >> $INSTALL_PATH/siteEnv
echo export EPICS_HOST_ARCH=$EPICS_ARCH >> $INSTALL_PATH/siteEnv
echo export EPICS_BASE=$INSTALL_PATH/base >> $INSTALL_PATH/siteEnv
echo export PATH=\${PATH}:\${INSTALL_PATH}/base/bin/\${EPICS_HOST_ARCH}:\${EPICS_HOST_ARCH} >> $INSTALL_PATH/siteEnv
echo "" >> $INSTALL_PATH/siteEnv
echo \# channel access >> $INSTALL_PATH/siteEnv
echo export EPICS_CA_MAX_ARRAY_BYTES=100000000 >> $INSTALL_PATH/siteEnv
echo export EPICS_CA_AUTO_ADDR_LIST=YES >> $INSTALL_PATH/siteEnv
echo export EPICS_CA_ADDR_LIST= >> $INSTALL_PATH/siteEnv
chmod a+x $INSTALL_PATH/siteEnv

# This sets the environment variables following a reboot.
su -c bashrc.sh --login $USERNAME
