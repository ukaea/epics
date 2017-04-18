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
DEFAULT_EPICS_ROOT="/usr/local/epics"
if [ -z "$*" ]; then EPICS_ROOT=$DEFAULT_EPICS_ROOT; else EPICS_ROOT=$1;fi

# dependencies
# base
if [ ! -d $EPICS_ROOT/base ]; 
then
    ./epics_base_3-15-5_install.sh $EPICS_ROOT 3.14.12.3
fi

# seq
if [ ! -d $EPICS_ROOT/support/seq ]; 
then
    ./epics_seq_2-2-4_install.sh $EPICS_ROOT 2.1.11
fi

# asyn
if [ ! -d $EPICS_ROOT/support/asyn ]; 
then
    ./epics_asyn_4-28_install.sh $EPICS_ROOT 4-19
fi

#pyepics
if [ ! -d $EPICS_ROOT/extensions/src/pyepics ]; 
then
    ./epics_pyepics_3-2-5_install.sh $EPICS_ROOT
fi

