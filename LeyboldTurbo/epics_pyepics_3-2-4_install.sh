#!/bin/bash 

# this script install pyepics
# It assumes EPICS base is already installed and that
# + the environment variable EPICS_ROOT is set and points to the installation directory.
# + the environment variable EPICS_HOST_ARCH is set 
# and points to the correct architecture (e.g. 32 or 64 bit)
#
# Usage:
# sudo -s
# source ./epics_pyepics_3-2-1_install.sh

# check if user has right permissions
if [ "$(id -u)" != "0" ]; then
	echo "Sorry, you are not root. Please try again using sudo."
	exit 1
fi

# get installation directory from command line argument
DEFAULT_INSTALL_PATH="/usr/local/epics"
if [ -z "$*" ]; then INSTALL_PATH=$DEFAULT_INSTALL_PATH; else INSTALL_PATH=$1;fi

# terminate script after first line that fails
set -e

# dependencies
if [! -a /etc/redhat-release ];
then
	sudo apt-get -y install python python-numpy python-setuptools
fi

export PYEPICS_LIBCA=${EPICS_ROOT}/base/lib/${EPICS_HOST_ARCH}/libca.so

# Make directory if not exist
mkdir -p $INSTALL_PATH/extensions/src

# install pyepics
PYEPICS_DOWNLOAD="pyepics-3.2.4.tar.gz"
PYEPICS_DIRECTORY="pyepics-3.2.4"
wget --tries=3 --timeout=10  http://cars9.uchicago.edu/software/python/pyepics3/src/$PYEPICS_DOWNLOAD
tar xzvf $PYEPICS_DOWNLOAD -C $INSTALL_PATH/extensions/src

pushd $INSTALL_PATH/extensions/src/$PYEPICS_DIRECTORY
sudo python setup.py install
popd
rm $PYEPICS_DOWNLOAD

#symbolic link
rm -f $INSTALL_PATH/extensions/src/pyepics
ln -s $INSTALL_PATH/extensions/src/$PYEPICS_DIRECTORY $INSTALL_PATH/extensions/src/pyepics

# set environment variables
echo -e \# pyepics >> $INSTALL_PATH/siteEnv
echo -e PYEPICS_LIBCA=\${EPICS_ROOT}/base/lib/\${EPICS_HOST_ARCH}/libca.so >> $INSTALL_PATH/siteEnv

