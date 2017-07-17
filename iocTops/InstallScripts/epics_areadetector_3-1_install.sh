#!/bin/bash 

# copyright (c) Tessella 2016-2017

# This script install areadetector 2.0
# It assumes EPICS base is already installed and that
# the environment variable EPICS_ROOT is set and points to the installation directory.
#

# check if user has right permissions
if [ "$(id -u)" != "0" ]; then
	echo "Sorry, you are not root. Please try again using sudo."
	exit 1
fi

# terminate script after first line that fails
set -e

DEFAULT_EPICS_ROOT="/usr/local/epics"
if [ -z "$*" ]; then EPICS_ROOT=$DEFAULT_EPICS_ROOT; else EPICS_ROOT=$1;fi

SUPPORT_PATH=$EPICS_ROOT/support

# module dependencies
# base
if [ ! -d $EPICS_ROOT/base ]; 
then
	./epics_base_3-15-5_install.sh $EPICS_ROOT
fi

# asyn
if [ ! -d $SUPPORT_PATH/asyn ]; 
then
	./epics_asyn_4-31_install.sh $EPICS_ROOT
fi

# calc
if [ ! -d $SUPPORT_PATH/calc ]; 
then
	./epics_calc_3-6-1_install.sh $EPICS_ROOT
fi

# busy
if [ ! -d $SUPPORT_PATH/busy ]; 
then
	./epics_busy_1-6-1_install.sh $EPICS_ROOT
fi

# sscan
if [ ! -d $SUPPORT_PATH/sscan ]; 
then
	./epics_sscan_2-10_install.sh $EPICS_ROOT
fi
# autosave
if [ ! -d $SUPPORT_PATH/autosave ]; 
then
	./epics_autosave_5-7-1_install.sh $EPICS_ROOT
fi

USRLOCALPATH=/usr/local
IMAGEJ_PATH=$USRLOCALPATH/ImageJ
if [ ! -d $IMAGEJ_PATH ];
then
	# ImageJ
	# see https://imagej.nih.gov/ij/docs/install/linux.html
	IMAGEJ_DOWNLOAD=ij150-linux64-java8.zip

	wget --tries=3 --timeout=10 http://wsr.imagej.net/distros/linux/$IMAGEJ_DOWNLOAD
	unzip $IMAGEJ_DOWNLOAD -d $USRLOCALPATH
	chmod u=rwx,g=rwx,o=rx $IMAGEJ_PATH
	rm $IMAGEJ_DOWNLOAD
fi

#So the environment is set for this shell - $EPICS_HOST_ARCH is exported.
. $EPICS_ROOT/siteEnv

# dependencies
if [ ! -f /etc/redhat-release ];
then
	apt-get -y install libx11-dev
	apt-get -y install libxext-dev
#	apt-get -y install libxml2-dev
#	apt-get -y install libpng12-dev
#	apt-get -y install libbz2-dev
#	apt-get -y install freetype*
#	apt-get -y install libjpeg-dev
#used by URLDriver in areaDetector
#apt-get -y install GraphicsMagick
#else
#	yum -y install libX11
#	yum -y install xml2
#	yum -y install libpng12
#	yum -y install bzip2-libs
#	yum -y install libXext
#	yum -y install freetype*
#	yum -y install libjpeg
fi

# see http://superuser.com/questions/428553/install-compiled-binary-in-non-standard-environment
# but I shouldn't need to do this..!
#echo export MAGICK_CONFIGURE_PATH=/usr/lib/GraphicsMagick-1.3.18/config

# areadetector
#AREADETECTOR_DOWNLOAD="areaDetectorR1-9-1.tgz"
AREADETECTOR_DIRECTORY="areaDetectorR3-1"

#wget --tries=3 --timeout=10 http://cars.uchicago.edu/software/pub/$AREADETECTOR_DOWNLOAD

SUPPORT_PATH=$EPICS_ROOT/support
AREADETECTOR_PATH=$SUPPORT_PATH/areaDetector

mkdir -p $AREADETECTOR_PATH

cp -r /media/sf_epics/support/$AREADETECTOR_DIRECTORY $AREADETECTOR_PATH
chmod -R 775 $AREADETECTOR_PATH
chmod -R 777 $AREADETECTOR_PATH/$AREADETECTOR_DIRECTORY/ADURL/iocs/urlIOC/iocBoot/iocURLDriver

#tar xzvf $AREADETECTOR_DOWNLOAD -C $AREADETECTOR_PATH
#rm $AREADETECTOR_DOWNLOAD


#symbolic link
rm -f $AREADETECTOR_PATH/current
ln -s $AREADETECTOR_PATH/$AREADETECTOR_DIRECTORY $AREADETECTOR_PATH/current
#ln -s /media/sf_epics/support/areaDetector $AREADETECTOR_PATH/current
EPICS_BASE=$EPICS_ROOT/base
# hack RELEASE_PATHS file
#sed -i -e "/^SUPPORT\s*=/ s,=.*,=$SUPPORT_PATH," $AREADETECTOR_PATH/current/configure/RELEASE
#sed -i -e "/^AREA_DETECTOR\s*=/ s,=.*,=$AREADETECTOR_PATH/current," $AREADETECTOR_PATH/current/configure/RELEASE
#sed -i -e "/^EPICS_BASE\s*=/ s,=.*,=$EPICS_ROOT/base," $AREADETECTOR_PATH/current/configure/RELEASE
#sed -i -e "/^ASYN\s*=/ s,=.*,=$SUPPORT_PATH/asyn/current," $AREADETECTOR_PATH/current/configure/RELEASE
#sed -i -e "/^CALC\s*=/ s,=.*,=$SUPPORT_PATH/calc/current," $AREADETECTOR_PATH/current/configure/RELEASE
#sed -i -e "/^BUSY\s*=/ s,=.*,=$SUPPORT_PATH/busy/current," $AREADETECTOR_PATH/current/configure/RELEASE
#sed -i -e "/^SSCAN\s*=/ s,=.*,=$SUPPORT_PATH/sscan/current," $AREADETECTOR_PATH/current/configure/RELEASE
#sed -i -e "/^AUTOSAVE\s*=/ s,=.*,=$SUPPORT_PATH/autosave/current," $AREADETECTOR_PATH/current/configure/RELEASE

# These hacks fix typos in the scripts
#echo "URLDriverApp st.cmd" >> $AREADETECTOR_PATH/current/iocBoot/iocURLDriver/start_epics

make -C $AREADETECTOR_PATH/current clean uninstall > NUL
# build
make -C $AREADETECTOR_PATH/current install > /media/sf_epics/support/$AREADETECTOR_DIRECTORY/BuildLogs/$AREADETECTOR_DIRECTORY.$EPICS_HOST_ARCH.log 2>&1

#environment variable - both for now and for new shell.
echo -e \# areaDetector >> $EPICS_ROOT/siteEnv
export AREA_DETECTOR=$AREADETECTOR_PATH/current
echo export AREA_DETECTOR=$AREA_DETECTOR >> $EPICS_ROOT/siteEnv

# add to binaries to PATH
echo export PATH=\${PATH}:$AREA_DETECTOR/ADURL/iocs/urlIOC/bin/\${EPICS_HOST_ARCH} >> $EPICS_ROOT/siteEnv
echo export PATH=\${PATH}:$IMAGEJ_PATH >> $EPICS_ROOT/siteEnv
