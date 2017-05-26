#!/bin/bash 

#	Author:  Peter Heesterman (Tessella plc). Date: 13 Aug 2015.
#	Written for CCFE (Culham Centre for Fusion Energy).

# this script installs EPICS base, set necessary environment variables
# as well as the extension directory file structure (so it's ready to install extensions)

# check if user has right permissions
if [ "$(id -u)" != "0" ]; then
	echo "Sorry, you are not root. Please try again using sudo."
	exit 1
fi

# terminate script after first line that fails
set -e

# 32 or 64bit?
case `uname -m` in
  i[3456789]86|x86|i86pc)
    export EPICS_HOST_ARCH='linux-x86'
    ;;
  x86_64|amd64|AMD64)
    export EPICS_HOST_ARCH='linux-x86_64'
    ;;
  *)
    echo "Unknown architecture `uname -m`."
    exit 1
    ;;
esac

# get installation directory from command line argument
DEFAULT_EPICS_ROOT="/usr/local/epics"
if [ -z "$*" ]; then EPICS_ROOT=$DEFAULT_EPICS_ROOT; else EPICS_ROOT=$1;fi
echo "Base install path "$EPICS_ROOT

BASE_VERSION="3.15.5";
if [ $# -ge 2 ]; then BASE_VERSION=$2; fi
echo "Base version "$BASE_VERSION

if [ ! -f /etc/redhat-release ];
then
    # see http://stackoverflow.com/questions/6486449/the-problem-of-using-sudo-apt-get-install-build-essentials
    apt-get update

    # install dependencies
    apt-get -y install build-essential g++ libreadline-dev
else
	yum -y update
	yum makecache fast
	#systemctl stop packagekit
	yum -y install readline-devel
fi

# base
if [[ $BASE_VERSION =~ "3.15" ]]; then 
	BASE_DOWNLOAD="base-"$BASE_VERSION".tar.gz"
else
	BASE_DOWNLOAD="baseR"$BASE_VERSION".tar.gz"
fi

BASE_DIRECTORY="base-"$BASE_VERSION

wget --tries=3 --timeout=10 https://www.aps.anl.gov/epics/download/base/$BASE_DOWNLOAD
mkdir -p $EPICS_ROOT
tar xzvf $BASE_DOWNLOAD -C $EPICS_ROOT
rm -f $BASE_DOWNLOAD
make -C $EPICS_ROOT/$BASE_DIRECTORY install
ln -s $EPICS_ROOT/$BASE_DIRECTORY $EPICS_ROOT/base

# set environment variables
touch $EPICS_ROOT/siteEnv
echo \# main EPICS env var >> $EPICS_ROOT/siteEnv


echo export EPICS_HOST_ARCH=$EPICS_HOST_ARCH >> $EPICS_ROOT/siteEnv
echo export EPICS_ROOT=$EPICS_ROOT >> $EPICS_ROOT/siteEnv
echo export EPICS_BASE=$EPICS_ROOT/base >> $EPICS_ROOT/siteEnv
echo export PATH=\${PATH}:\${EPICS_ROOT}/base/bin/\${EPICS_HOST_ARCH}:\${EPICS_ROOT}/extensions/bin/\${EPICS_HOST_ARCH} >> $EPICS_ROOT/siteEnv
echo ulimit -c unlimited >> $EPICS_ROOT/siteEnv

echo \# channel access >> $EPICS_ROOT/siteEnv
echo export EPICS_CA_MAX_ARRAY_BYTES=100000000 >> $EPICS_ROOT/siteEnv
# See http://www.aps.anl.gov/epics/tech-talk/2009/msg00924.php
echo export EPICS_CA_AUTO_ADDR_LIST=NO >> $EPICS_ROOT/siteEnv
echo export EPICS_CA_ADDR_LIST=127.0.0.1 >> $EPICS_ROOT/siteEnv

echo "" >> $EPICS_ROOT/siteEnv
chmod a+x $EPICS_ROOT/siteEnv

# This sets the environment variables for this shell, now.
. $EPICS_ROOT/siteEnv

# This sets the environment variables following a reboot.
if [ ! -f /etc/redhat-release ];
then
    ./bashrc.sh $EPICS_ROOT
else
    su -c './bashrc.sh $EPICS_ROOT' $USERNAME
fi

# extensions top
EXTENSION_TOP_DOWNLOAD="extensionsTop_20120904.tar.gz"
EXTENSION_CONFIG_DOWNLOAD="extensionsConfig_20040406.tar.gz"
EXTENSION_DIRECTORY="extensions"
wget --tries=3 --timeout=10 http://www.aps.anl.gov/epics/download/extensions/$EXTENSION_CONFIG_DOWNLOAD
tar xzvf $EXTENSION_CONFIG_DOWNLOAD -C $EPICS_ROOT 
rm -f $EXTENSION_CONFIG_DOWNLOAD
wget --tries=3 --timeout=10 http://www.aps.anl.gov/epics/download/extensions/$EXTENSION_TOP_DOWNLOAD
tar xzvf $EXTENSION_TOP_DOWNLOAD -C $EPICS_ROOT 
rm -f $EXTENSION_TOP_DOWNLOAD
make -C $EPICS_ROOT/$EXTENSION_DIRECTORY install
