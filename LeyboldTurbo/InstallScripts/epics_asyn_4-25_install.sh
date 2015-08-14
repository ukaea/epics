#!/bin/bash 

#	Author:  Peter Heesterman (Tessella plc). Date: 13 Aug 2015.
#	Written for CCFE (Culham Centre for Fusion Energy).

#
# Usage:
# sudo ./epics_asyn_4-23_install.sh

# check if user has right permissions
if [ "$(id -u)" != "0" ]; then
	echo "Sorry, you are not root. Please try again using sudo."
	exit 1

fi

# terminate script after first line that fails
set -e

# get installation directory from command line argument
INSTALL_PATH="/usr/local/epics"; 
if [ $# -ge 1 ]; then INSTALL_PATH=$1; fi
echo "Asyn install path "$INSTALL_PATH

ASYN_VER="4-25";
if [ $# -ge 2 ]; then ASYN_VER=$2; fi
echo "Asyn version "$ASYN_VER

# dependencies
# base
if [ ! -d $INSTALL_PATH/base ]; 
then
    ./epics_base_3-15-1_install.sh $INSTALL_PATH
fi

# seq
if [ ! -d $INSTALL_PATH/support/seq ]; 
then
    ./epics_seq_2-1-15_install.sh $INSTALL_PATH
fi

# asyn
ASYN_DOWNLOAD="asyn"$ASYN_VER".tar.gz"
ASYN_DIRECTORY="asyn"$ASYN_VER
wget --tries=3 --timeout=10  http://www.aps.anl.gov/epics/download/modules/$ASYN_DOWNLOAD
SUPPORT_PATH=$INSTALL_PATH/support/asyn
mkdir -p $SUPPORT_PATH
tar xzvf $ASYN_DOWNLOAD -C $SUPPORT_PATH
rm $ASYN_DOWNLOAD

#symbolic link
rm -f $SUPPORT_PATH/current
ln -s $SUPPORT_PATH/$ASYN_DIRECTORY $SUPPORT_PATH/current

chmod 666 $SUPPORT_PATH/current/configure/RELEASE
sed -i -e "/^SUPPORT\s*=/ s,=.*,=$INSTALL_PATH/support," $SUPPORT_PATH/current/configure/RELEASE
sed -i -e "/^EPICS_BASE\s*=/ s,=.*,=$INSTALL_PATH/base," $SUPPORT_PATH/current/configure/RELEASE
sed -i -e "/^SNCSEQ\s*=/ s,=.*,=$INSTALL_PATH/support/seq/current," $SUPPORT_PATH/current/configure/RELEASE

make -C $SUPPORT_PATH/current install

