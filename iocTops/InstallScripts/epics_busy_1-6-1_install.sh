#!/bin/bash 

# copyright (c) Tessella 2014

# This script install busy 1.6
# It assumes EPICS base is already installed and that
# the environment variable EPICS_ROOT is set and points to the installation directory.
#
# Usage:
# sudo -s
# source ./epics_busy_1-6_install.sh

# check if user has right permissions
if [ "$(id -u)" != "0" ]; then
	echo "Sorry, you are not root. Please try again using sudo."
	exit 1
fi

# terminate script after first line that fails
set -e

DEFAULT_EPICS_ROOT="/usr/local/epics"
if [ -z "$*" ]; then EPICS_ROOT=$DEFAULT_EPICS_ROOT; else EPICS_ROOT=$1;fi

# dependencies
# asyn
if [ ! -d $EPICS_ROOT/support/asyn ]; 
then
    . ./epics_asyn_4-31_install.sh $EPICS_ROOT
fi

#So the environment is set for this shell - $EPICS_HOST_ARCH is exported.
. $EPICS_ROOT/siteEnv

# busy
BUSY_DOWNLOAD="busy_R1-6-1.tar.gz"
BUSY_DIRECTORY="busy-1-6-1"
wget --tries=3 --timeout=10  http://www.aps.anl.gov/bcda/synApps/tar/$BUSY_DOWNLOAD
BUSY_PATH=$EPICS_ROOT/support/busy
mkdir -p $BUSY_PATH
tar xzvf $BUSY_DOWNLOAD -C $BUSY_PATH
rm $BUSY_DOWNLOAD

#symbolic link
rm -f $BUSY_PATH/current
ln -s $BUSY_PATH/$BUSY_DIRECTORY $BUSY_PATH/current

# hack the 'RELEASE' file to put the settings we want.
#chmod 666 $BUSY_PATH/current/configure/RELEASE
sed -i -e "s|SUPPORT=/corvette/home/epics/devel|SUPPORT=$EPICS_ROOT/support|g" $BUSY_PATH/current/configure/RELEASE
sed -i -e "s|ASYN=\$(SUPPORT)/asyn-4-17|ASYN=\$(SUPPORT)/asyn/current|g" $BUSY_PATH/current/configure/RELEASE
sed -i -e "s|EPICS_BASE=/corvette/usr/local/epics/base-3.14.12.1|EPICS_BASE=$EPICS_ROOT/base|g" $BUSY_PATH/current/configure/RELEASE

make -C $BUSY_PATH/current install

