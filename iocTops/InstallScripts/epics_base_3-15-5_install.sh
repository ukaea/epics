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
    EPICS_ARCH='linux-x86'
    ;;
  x86_64|amd64|AMD64)
    EPICS_ARCH='linux-x86_64'
    ;;
  *)
    echo "Unknown architecture `uname -m`."
    exit 1
    ;;
esac

# get installation directory from command line argument
INSTALL_PATH="/usr/local/epics"; 
if [ $# -ge 1 ]; then INSTALL_PATH=$1; fi
echo "Base install path "$INSTALL_PATH

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
	yum -y install readline-devel
fi

# base
if [[ $BASE_VERSION =~ "3.15" ]]; then 
	BASE_DOWNLOAD="base-"$BASE_VERSION".tar.gz"
else
	BASE_DOWNLOAD="baseR"$BASE_VERSION".tar.gz"
fi

BASE_DIRECTORY="base-"$BASE_VERSION
wget --tries=3 --timeout=10 http://aps.anl.gov/epics/download/base/$BASE_DOWNLOAD
mkdir -p $INSTALL_PATH
tar xzvf $BASE_DOWNLOAD -C $INSTALL_PATH
rm -f $BASE_DOWNLOAD
make -C $INSTALL_PATH/$BASE_DIRECTORY install
ln -s $INSTALL_PATH/$BASE_DIRECTORY $INSTALL_PATH/base

# set environment variables
touch $INSTALL_PATH/siteEnv
echo \# main EPICS env var >> $INSTALL_PATH/siteEnv


echo export EPICS_HOST_ARCH=$EPICS_ARCH >> $INSTALL_PATH/siteEnv
echo export EPICS_ROOT=$INSTALL_PATH >> $INSTALL_PATH/siteEnv
echo export EPICS_BASE=$INSTALL_PATH/base >> $INSTALL_PATH/siteEnv
echo export PATH=\${PATH}:\${EPICS_ROOT}/base/bin/\${EPICS_HOST_ARCH} >> $INSTALL_PATH/siteEnv
echo "" >> $INSTALL_PATH/siteEnv
chmod a+x $INSTALL_PATH/siteEnv

# This sets the environment variables following a reboot.
if [ ! -f /etc/redhat-release ];
then
	./bashrc.sh $INSTALL_PATH
else
	su -c './bashrc.sh $INSTALL_PATH' $USERNAME
fi
