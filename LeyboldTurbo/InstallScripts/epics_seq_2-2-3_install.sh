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
INSTALL_PATH="/usr/local/epics"; 
if [ $# -ge 1 ]; then INSTALL_PATH=$1; fi
echo "Seq install path "$INSTALL_PATH

SEQ_VER="2.1.15"
if [ $# -ge 2 ]; then SEQ_VER=$2; fi
echo "Seq version "$SEQ_VER

if [ ! -d $INSTALL_PATH/base ]; 
then
    ./epics_base_3-15-2_install.sh $INSTALL_PATH
fi

if [ ! -f /etc/redhat-release ]; then
	# dependencies
	apt-get -y install re2c
else
	if $(uname -a | grep 'x86_64'); then
		yum -y install http://ftp.scientificlinux.org/linux/extra/dag/packages/re2c/re2c-0.13.2-1.el5.rf.x86_64.rpm
	else
		yum -y install http://ftp.scientificlinux.org/linux/extra/dag/packages/re2c/re2c-0.13.2-1.el5.rf.i386.rpm
	fi
fi
# seq
SEQ_DOWNLOAD="seq-"$SEQ_VER".tar.gz"
SEQ_DIRECTORY="seq-"$SEQ_VER
wget --tries=3 --timeout=10  http://www-csr.bessy.de/control/SoftDist/sequencer/releases/$SEQ_DOWNLOAD
SUPPORT_PATH=$INSTALL_PATH/support/seq
mkdir -p $SUPPORT_PATH
tar xzvf $SEQ_DOWNLOAD -C $SUPPORT_PATH
rm $SEQ_DOWNLOAD

#symbolic link
rm -f $SUPPORT_PATH/current
ln -s $SUPPORT_PATH/$SEQ_DIRECTORY $SUPPORT_PATH/current

chmod 666 $SUPPORT_PATH/current/configure/RELEASE
sed -i -e "/^EPICS_BASE\s*=/ s,=.*,=$INSTALL_PATH/base," $SUPPORT_PATH/current/configure/RELEASE

make -C $SUPPORT_PATH/current install

