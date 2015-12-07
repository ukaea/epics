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
DEFAULT_INSTALL_PATH="/usr/local/epics"
if [ -z "$*" ]; then INSTALL_PATH=$DEFAULT_INSTALL_PATH; else INSTALL_PATH=$1;fi

# dependencies
# base
if [ ! -d $INSTALL_PATH/base ]; 
then
    ./epics_base_3-15-3_install.sh $INSTALL_PATH 3.14.12.3
fi

# seq
if [ ! -d $INSTALL_PATH/support/seq ]; 
then
    ./epics_seq_2-2-3_install.sh $INSTALL_PATH 2.1.11
fi

# asyn
if [ ! -d $INSTALL_PATH/support/asyn ]; 
then
    ./epics_asyn_4-27_install.sh $INSTALL_PATH 4-19
fi

#pyepics
if [ ! -d $INSTALL_PATH/extensions/src/pyepics ]; 
then
    ./epics_pyepics_3-2-4_install.sh $INSTALL_PATH
fi

