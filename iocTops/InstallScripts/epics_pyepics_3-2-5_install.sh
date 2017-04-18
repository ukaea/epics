#!/bin/bash 

#	Author:  Peter Heesterman (Tessella plc). Date: 13 Aug 2015.
#	Written for CCFE (Culham Centre for Fusion Energy).

# this script install pyepics
# It assumes EPICS base is already installed and that
# + the environment variable EPICS_ROOT is set and points to the installation directory.
# + the environment variable EPICS_HOST_ARCH is set 
# and points to the correct architecture (e.g. 32 or 64 bit)
#

# check if user has right permissions
if [ "$(id -u)" != "0" ]; then
	echo "Sorry, you are not root. Please try again using sudo."
	exit 1
fi

# get installation directory from command line argument
DEFAULT_EPICS_ROOT="/usr/local/epics"
if [ -z "$*" ]; then EPICS_ROOT=$DEFAULT_EPICS_ROOT; else EPICS_ROOT=$1;fi

# terminate script after first line that fails
set -e

# dependencies
if [ ! -f /etc/redhat-release ];
then
	sudo apt-get -y install python python-numpy python-setuptools
fi

export PYEPICS_LIBCA=${EPICS_ROOT}/base/lib/${EPICS_HOST_ARCH}/libca.so

# Make directory if not exist
mkdir -p $EPICS_ROOT/extensions/src

# install pyepics
PYEPICS_DOWNLOAD="pyepics-3.2.5.tar.gz"
PYEPICS_DIRECTORY="pyepics-3.2.5"
wget --tries=3 --timeout=10  http://cars9.uchicago.edu/software/python/pyepics3/src/$PYEPICS_DOWNLOAD
tar xzvf $PYEPICS_DOWNLOAD -C $EPICS_ROOT/extensions/src

pushd $EPICS_ROOT/extensions/src/$PYEPICS_DIRECTORY
sudo python setup.py install
popd
rm $PYEPICS_DOWNLOAD

#symbolic link
rm -f $EPICS_ROOT/extensions/src/pyepics
ln -s $EPICS_ROOT/extensions/src/$PYEPICS_DIRECTORY $EPICS_ROOT/extensions/src/pyepics

# set environment variables
echo -e \# pyepics >> $EPICS_ROOT/siteEnv
echo -e PYEPICS_LIBCA=\${EPICS_ROOT}/base/lib/\${EPICS_HOST_ARCH}/libca.so >> $EPICS_ROOT/siteEnv

