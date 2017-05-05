#!/bin/bash 

# copyright (c) Tessella 2014

# This script install autosave 5.4
# It assumes EPICS base is already installed and that
# the environment variable EPICS_ROOT is set and points to the installation directory.
#
# Usage:
# sudo -s
# source ./epics_auto_5-4_install.sh

# check if user has right permissions
if [ "$(id -u)" != "0" ]; then
	echo "Sorry, you are not root. Please try again using sudo."
	exit 1
fi

DEFAULT_EPICS_ROOT="/usr/local/epics"
if [ -z "$*" ]; then EPICS_ROOT=$DEFAULT_EPICS_ROOT; else EPICS_ROOT=$1;fi

# terminate script after first line that fails
set -e

# dependencies

# base
if [ ! -d $EPICS_ROOT/base ]; 
then
    ./epics_base_3-15-5_install.sh $EPICS_ROOT
fi

#So the environment is set for this shell - $EPICS_HOST_ARCH is exported.
. $EPICS_ROOT/siteEnv

# autosave
AUTOSAVE_DOWNLOAD="R5-7-1.tar.gz"
AUTOSAVE_DIRECTORY="autosave-R5-7-1"

wget --tries=3 --timeout=10 https://github.com/epics-modules/autosave/archive/$AUTOSAVE_DOWNLOAD

SUPPORT_PATH=$EPICS_ROOT/support
AUTOSAVE_PATH=$SUPPORT_PATH/autosave

mkdir -p $AUTOSAVE_PATH
tar xzvf $AUTOSAVE_DOWNLOAD -C $AUTOSAVE_PATH
rm $AUTOSAVE_DOWNLOAD

#symbolic link
rm -f $AUTOSAVE_PATH/current
ln -s $AUTOSAVE_PATH/$AUTOSAVE_DIRECTORY $AUTOSAVE_PATH/current

# This variable is required by the areaDetector module
echo export AUTOSAVE=$AUTOSAVE_PATH/current >> $EPICS_ROOT/siteEnv

# hack the 'RELEASE' file to put the settings we want.
chmod 666 $AUTOSAVE_PATH/current/configure/RELEASE
sed -i -e "/^EPICS_BASE\s*=/ s,=.*,=$EPICS_ROOT/base," $AUTOSAVE_PATH/current/configure/RELEASE

# build
make -C $AUTOSAVE_PATH/current install

