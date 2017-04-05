#!/bin/bash 

# copyright (c) Tessella 2014

# This script install sscan 2.9
# It assumes EPICS base is already installed and that
# the environment variable EPICS_ROOT is set and points to the installation directory.
#
# Usage:
# sudo -s
# source ./epics_sscan_2-9_install.sh

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
# seq
if [ ! -d $EPICS_ROOT/support/seq ]; 
then
    ./epics_seq_2-2-4_install.sh $EPICS_ROOT
fi

# sscan
SSCAN_DOWNLOAD="sscan_R2-10.tar.gz"
SSCAN_DIRECTORY="sscan-2-10"
wget --tries=3 --timeout=10 http://www.aps.anl.gov/bcda/synApps/tar/$SSCAN_DOWNLOAD

SUPPORT_PATH=$EPICS_ROOT/support
SSCAN_PATH=$SUPPORT_PATH/sscan

mkdir -p $SSCAN_PATH
tar xzvf $SSCAN_DOWNLOAD -C $SSCAN_PATH
rm $SSCAN_DOWNLOAD

# This variable is required by the areaDetector module
echo export SSCAN=$SSCAN_PATH/current >> $EPICS_ROOT/siteEnv

#symbolic link
rm -f $SSCAN_PATH/current
ln -s $SSCAN_PATH/$SSCAN_DIRECTORY $SSCAN_PATH/current

# hack the 'RELEASE' file to put the settings we want.
chmod 666 $SSCAN_PATH/current/configure/RELEASE
sed -i -e "/^SUPPORT\s*=/ s,=.*,=$EPICS_ROOT/support," $SSCAN_PATH/current/configure/RELEASE
sed -i -e "/^EPICS_BASE\s*=/ s,=.*,=$EPICS_ROOT/base," $SSCAN_PATH/current/configure/RELEASE
sed -i -e "/^SNCSEQ\s*=/ s,=.*,=$SUPPORT_PATH/seq/current," $SSCAN_PATH/current/configure/RELEASE
    
# build
make -C $SSCAN_PATH/current install

