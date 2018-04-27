#!/bin/bash 

# copyright (c) Tessella 2014

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
# sscan
if [ ! -d $EPICS_ROOT/support/sscan ]; 
then
    ./epics_sscan_2-10_install.sh $EPICS_ROOT
fi

#So the environment is set for this shell - $EPICS_HOST_ARCH is exported.
. $EPICS_ROOT/siteEnv

# calc
CALC_DOWNLOAD="R3-6-1.tar.gz"
CALC_DIRECTORY="calc-R3-6-1"

wget --tries=3 --timeout=10 https://github.com/epics-modules/calc/archive/$CALC_DOWNLOAD

SUPPORT_PATH=$EPICS_ROOT/support

CALC_PATH=$SUPPORT_PATH/calc
mkdir -p $CALC_PATH
tar xzvf $CALC_DOWNLOAD -C $CALC_PATH
rm $CALC_DOWNLOAD

#symbolic link
rm -f $CALC_PATH/current
ln -s $CALC_PATH/$CALC_DIRECTORY $CALC_PATH/current

# This variable is required by the areaDetector module
echo export CALC=$CALC_PATH/current >> $EPICS_ROOT/siteEnv

# hack the 'RELEASE' file to put the settings we want.
chmod 666 $CALC_PATH/current/configure/RELEASE
sed -i -e "/^SUPPORT\s*=/ s,=.*,=$EPICS_ROOT/support," $CALC_PATH/current/configure/RELEASE
sed -i -e "/^EPICS_BASE\s*=/ s,=.*,=$EPICS_ROOT/base," $CALC_PATH/current/configure/RELEASE
sed -i -e "/^SSCAN\s*=/ s,=.*,=$SUPPORT_PATH/sscan/current," $CALC_PATH/current/configure/RELEASE
sed -i -e "/^SNCSEQ\s*=/ s,=.*,=$SUPPORT_PATH/seq/current," $CALC_PATH/current/configure/RELEASE
# build
make -j2 -O -C $CALC_PATH/current install

