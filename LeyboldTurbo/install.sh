#!/bin/bash 

# Copyright (c) Tessella 2014

# This script install asyn 4.23
# It assumes EPICS base is already installed and that
# the environment variable EPICS_ROOT is set and points to the installation directory.
#
# Usage:
# sudo -s
# source ./epics_asyn_4-23_install.sh

# check if user has right permissions
if [ "$(id -u)" != "0" ]; then
	echo "Sorry, you are not root. Please try again using sudo."
	exit 1

fi

# terminate script after first line that fails
set -e

# get installation directory from command line argument
DEFAULT_INSTALL_PATH="/usr/local/epics"
if [ -z "$*" ]; then INSTALL_PATH=$DEFAULT_INSTALL_PATH; else INSTALL_PATH=$1;fi

# dependencies
# base
if [ ! -d $INSTALL_PATH/base ]; 
then
    ./epics_base_3-15-1_install.sh $INSTALL_PATH
fi

# asyn
if [ ! -d $INSTALL_PATH/support/asyn ]; 
then
    ./epics_asyn_4-25_install.sh $INSTALL_PATH
fi

if [ ! -d $INSTALL_PATH/extensions/src/pyepics ]; 
then
    ./epics_pyepics_3-2-4_install.sh $INSTALL_PATH
fi

