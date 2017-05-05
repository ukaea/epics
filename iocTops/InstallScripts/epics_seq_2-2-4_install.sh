#!/bin/bash 

#	Author:  Peter Heesterman (Tessella plc). Date: 13 Aug 2015.
#	Written for CCFE (Culham Centre for Fusion Energy).

# this script install seq. 
#

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
echo "Seq install path "$EPICS_ROOT

SEQ_VER="2.2.4"
if [ $# -ge 2 ]; then SEQ_VER=$2; fi
echo "Seq version "$SEQ_VER

if [ ! -d $EPICS_ROOT/base ]; 
then
    ./epics_base_3-15-5_install.sh $EPICS_ROOT
fi

if [ ! -f /etc/redhat-release ]; then
	# dependencies
	apt-get -y install re2c
else
	yum -y install re2c
fi

#So the environment is set for this shell - $EPICS_HOST_ARCH is exported.
. $EPICS_ROOT/siteEnv

# seq
SEQ_DOWNLOAD="seq-"$SEQ_VER".tar.gz"
SEQ_DIRECTORY="seq-"$SEQ_VER
wget --tries=3 --timeout=10  http://www-csr.bessy.de/control/SoftDist/sequencer/releases/$SEQ_DOWNLOAD

SUPPORT_PATH=$EPICS_ROOT/support
ASYN_PATH=$SUPPORT_PATH/seq

mkdir -p $ASYN_PATH
tar xzvf $SEQ_DOWNLOAD -C $ASYN_PATH
rm $SEQ_DOWNLOAD

#symbolic link
rm -f $ASYN_PATH/current
ln -s $ASYN_PATH/$SEQ_DIRECTORY $ASYN_PATH/current

chmod 666 $ASYN_PATH/current/configure/RELEASE
sed -i -e "/^EPICS_BASE\s*=/ s,=.*,=$EPICS_ROOT/base," $ASYN_PATH/current/configure/RELEASE

make -C $ASYN_PATH/current install

