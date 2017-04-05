#!/bin/bash 

#	Author:  Peter Heesterman (Tessella plc). Date: 13 Aug 2015.
#	Written for CCFE (Culham Centre for Fusion Energy).

#
# Usage:
# sudo ./epics_asyn_4-28_install.sh

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

ASYN_VER="4-31";
if [ $# -ge 2 ]; then ASYN_VER=$2; fi
echo "Asyn version "$ASYN_VER

# dependencies
# base
if [ ! -d $INSTALL_PATH/base ]; 
then
    ./epics_base_3-15-5_install.sh $INSTALL_PATH
fi

# seq
if [ ! -d $INSTALL_PATH/support/seq ]; 
then
    ./epics_seq_2-2-4_install.sh $INSTALLATH
fi

if [ ! -f /etc/redhat-release ]; then
	# required by libusb
	apt-get -y install libudev-devel
else
	yum -y install libudev-devel
fi


# NB, at least v16 is required by the Asyn build. Newer versions seem not to be available packaged.
# See http://www.linuxfromscratch.org/blfs/view/svn/general/libusb.html
LIBUSB_DOWNLOAD=libusb-1.0.20.tar.bz2
LIBUSB_DIRECTORY=libusb-1.0.20

rm -f $LIBUSB_DOWNLOAD

wget --tries=3 --timeout=10  http://downloads.sourceforge.net/libusb/$LIBUSB_DOWNLOAD
tar xvf $LIBUSB_DOWNLOAD

cd $LIBUSB_DIRECTORY

# 32 or 64bit?
case `uname -m` in
	i[3456789]86|x86|i86pc)
		./configure --prefix=/usr --disable-static && make -j1
	;;
	x86_64|amd64|AMD64)
		./configure --prefix=/usr --disable-static --libdir=/usr/lib64 && make -j1
	;;
	*)
		echo "Unknown architecture `uname -m`."
		exit 1
	;;
esac

make install
cd ..


# asyn
ASYN_DOWNLOAD="asyn"$ASYN_VER".tar.gz"
ASYN_DIRECTORY="asyn"$ASYN_VER
wget --tries=3 --timeout=10  http://www.aps.anl.gov/epics/download/modules/$ASYN_DOWNLOAD

SUPPORT_PATH=$INSTALL_PATH/support
ASYN_PATH=$SUPPORT_PATH/asyn

mkdir -p $ASYN_PATH
tar xzvf $ASYN_DOWNLOAD -C $ASYN_PATH
rm $ASYN_DOWNLOAD

#symbolic link
rm -f $ASYN_PATH/current
ln -s $ASYN_PATH/$ASYN_DIRECTORY $ASYN_PATH/current

#enable USB Test and Measurement Class
chmod 666 $ASYN_PATH/current/configure/CONFIG_SITE
sed -i -e "s/#DRV_USBTMC=YES/DRV_USBTMC=YES/" $ASYN_PATH/current/configure/CONFIG_SITE

chmod 666 $ASYN_PATH/current/configure/RELEASE
sed -i -e "/^SUPPORT\s*=/ s,=.*,=$INSTALL_PATH/support," $ASYN_PATH/current/configure/RELEASE
sed -i -e "/^EPICS_BASE\s*=/ s,=.*,=$INSTALL_PATH/base," $ASYN_PATH/current/configure/RELEASE
sed -i -e "/^SNCSEQ\s*=/ s,=.*,=$SUPPORT_PATH/seq/current," $ASYN_PATH/current/configure/RELEASE

make -C $ASYN_PATH/current install

