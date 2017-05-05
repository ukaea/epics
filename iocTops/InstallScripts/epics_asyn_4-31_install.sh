#!/bin/bash 

#	Author:  Peter Heesterman (Tessella plc). Date: 13 Aug 2015.
#	Written for CCFE (Culham Centre for Fusion Energy).

#
# Usage:
# sudo ./epics_asyn_4-28_install.sh

# check if user has right permissions
if [ "$(id -u)" != "0" ]; then
	echo "Sorry, you are not root. Please try again using sudo."
	exit 1

fi

# terminate script after first line that fails
set -e

# get installation directory from command line argument
EPICS_ROOT="/usr/local/epics"; 
if [ $# -ge 1 ]; then EPICS_ROOT=$1; fi
echo "Asyn install path "$EPICS_ROOT

ASYN_VER="4-31";
if [ $# -ge 2 ]; then ASYN_VER=$2; fi
echo "Asyn version "$ASYN_VER

# dependencies
# base
if [ ! -d $EPICS_ROOT/base ]; 
then
    ./epics_base_3-15-5_install.sh $EPICS_ROOT
fi

# seq
if [ ! -d $EPICS_ROOT/support/seq ]; 
then
    ./epics_seq_2-2-4_install.sh $INSTALLATH
fi

#So the environment is set for this shell - $EPICS_HOST_ARCH is exported.
. $EPICS_ROOT/siteEnv

# asyn
ASYN_DOWNLOAD="asyn"$ASYN_VER".tar.gz"
ASYN_DIRECTORY="asyn"$ASYN_VER
wget --tries=3 --timeout=10  http://www.aps.anl.gov/epics/download/modules/$ASYN_DOWNLOAD

SUPPORT_PATH=$EPICS_ROOT/support
ASYN_PATH=$SUPPORT_PATH/asyn

mkdir -p $ASYN_PATH
tar xzvf $ASYN_DOWNLOAD -C $ASYN_PATH
rm $ASYN_DOWNLOAD

#symbolic link
rm -f $ASYN_PATH/current
ln -s $ASYN_PATH/$ASYN_DIRECTORY $ASYN_PATH/current

chmod 666 $ASYN_PATH/current/configure/RELEASE
sed -i -e "/^SUPPORT\s*=/ s,=.*,=$EPICS_ROOT/support," $ASYN_PATH/current/configure/RELEASE
sed -i -e "/^EPICS_BASE\s*=/ s,=.*,=$EPICS_ROOT/base," $ASYN_PATH/current/configure/RELEASE
sed -i -e "/^SNCSEQ\s*=/ s,=.*,=$SUPPORT_PATH/seq/current," $ASYN_PATH/current/configure/RELEASE

make -C $ASYN_PATH/current install

