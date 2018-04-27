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
	./epics_asyn_4-33_install.sh $EPICS_ROOT
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

# dependencies
apt-get -y install libx11-dev
apt-get -y install libxext-dev
# areaDetector build now depends on the Boost library
apt-get -y install libboost-all-dev

SUPPORT_PATH=$EPICS_ROOT/support

# areadetector
AREADETECTOR_DOWNLOAD="R3-2.zip"
AREADETECTOR_DIRECTORY="areaDetector-R3-2"
AREADETECTOR_ZIP="areaDetector.zip"
AREADETECTOR_LINK="current"
AREADETECTOR_PATH=$SUPPORT_PATH/areaDetector

wget --tries=3 --timeout=10 https://github.com/areaDetector/areaDetector/archive/$AREADETECTOR_DOWNLOAD -O $AREADETECTOR_ZIP

ADCORE_DOWNLOAD="R3-2.zip"
ADCORE_DIRECTORY="ADCore-R3-2"
ADCORE_PATH=$AREADETECTOR_PATH/$AREADETECTOR_DIRECTORY/$ADCORE_DIRECTORY
ADCORE_ZIP="ADCore.zip"
ADCORE_LINK="ADCore"

wget --tries=3 --timeout=10 https://github.com/areaDetector/ADCore/archive/$ADCORE_DOWNLOAD -O $ADCORE_ZIP

ADSUPPORT_DOWNLOAD="R1-4.zip"
ADSUPPORT_DIRECTORY="ADSupport-R1-4"
ADSUPPORT_PATH=$AREADETECTOR_PATH/$AREADETECTOR_DIRECTORY/$ADSUPPORT_DIRECTORY
ADSUPPORT_ZIP="ADSupport.zip"
ADSUPPORT_LINK="ADSupport"

wget --tries=3 --timeout=10 https://github.com/areaDetector/ADSupport/archive/$ADSUPPORT_DOWNLOAD -O $ADSUPPORT_ZIP

ADURL_DOWNLOAD="R2-2.zip"
ADURL_DIRECTORY="ADURL-R2-2"
ADURL_PATH=$AREADETECTOR_PATH/$AREADETECTOR_DIRECTORY/$ADURL_DIRECTORY
ADURL_ZIP="ADURL.zip"
ADURL_LINK="ADURL"

wget --tries=3 --timeout=10 https://github.com/areaDetector/ADURL/archive/$ADURL_DOWNLOAD -O $ADURL_ZIP

ADSIMDETECTOR_DOWNLOAD="R2-6.zip"
ADSIMDETECTOR_DIRECTORY="ADSimDetector-R2-6"
ADSIMDETECTOR_PATH=$AREADETECTOR_PATH/$AREADETECTOR_DIRECTORY/$ADSIMDETECTOR_DIRECTORY
ADSIMDETECTOR_ZIP="ADSimDetector.zip"
ADSIMDETECTOR_LINK="ADSimDetector"

wget --tries=3 --timeout=10 https://github.com/areaDetector/ADSimDetector/archive/$ADSIMDETECTOR_DOWNLOAD -O $ADSIMDETECTOR_ZIP

ADVIEWERS_DOWNLOAD="R1-0.zip"
ADVIEWERS_DIRECTORY="ADViewers-R1-0"
ADVIEWERS_PATH=$AREADETECTOR_PATH/$AREADETECTOR_DIRECTORY/$ADVIEWERS_DIRECTORY
ADVIEWERS_ZIP="ADViewers.zip"
ADVIEWERS_LINK="ADViewers"

wget --tries=3 --timeout=10 https://github.com/areaDetector/ADViewers/archive/$ADVIEWERS_DOWNLOAD -O $ADVIEWERS_ZIP

mkdir -p $SUPPORT_PATH
mkdir -p $AREADETECTOR_PATH

unzip $AREADETECTOR_ZIP -d $AREADETECTOR_PATH
rm $AREADETECTOR_ZIP

unzip $ADCORE_ZIP -d $AREADETECTOR_PATH/$AREADETECTOR_DIRECTORY
rm $ADCORE_ZIP

unzip $ADSUPPORT_ZIP -d $AREADETECTOR_PATH/$AREADETECTOR_DIRECTORY
rm $ADSUPPORT_ZIP

unzip $ADURL_ZIP -d $AREADETECTOR_PATH/$AREADETECTOR_DIRECTORY
rm $ADURL_ZIP

unzip $ADSIMDETECTOR_ZIP -d $AREADETECTOR_PATH/$AREADETECTOR_DIRECTORY
rm $ADSIMDETECTOR_ZIP

unzip $ADVIEWERS_ZIP -d $AREADETECTOR_PATH/$AREADETECTOR_DIRECTORY
rm $ADVIEWERS_ZIP

#parent symbolic link.
rm -f $AREADETECTOR_PATH/current
ln -s $AREADETECTOR_PATH/$AREADETECTOR_DIRECTORY $AREADETECTOR_PATH/current

#submodule symbolic link(s). NB, the original contents of the areaDetector download are empty directories.
rmdir $AREADETECTOR_PATH/$AREADETECTOR_DIRECTORY/$ADCORE_LINK
ln -s $ADCORE_PATH $AREADETECTOR_PATH/$AREADETECTOR_DIRECTORY/$ADCORE_LINK

rmdir $AREADETECTOR_PATH/$AREADETECTOR_DIRECTORY/$ADSUPPORT_LINK
ln -s $ADSUPPORT_PATH $AREADETECTOR_PATH/$AREADETECTOR_DIRECTORY/$ADSUPPORT_LINK

rmdir $AREADETECTOR_PATH/$AREADETECTOR_DIRECTORY/$ADURL_LINK
ln -s $ADURL_PATH $AREADETECTOR_PATH/$AREADETECTOR_DIRECTORY/$ADURL_LINK

rmdir $AREADETECTOR_PATH/$AREADETECTOR_DIRECTORY/$ADSIMDETECTOR_LINK
ln -s $ADSIMDETECTOR_PATH $AREADETECTOR_PATH/$AREADETECTOR_DIRECTORY/$ADSIMDETECTOR_LINK

rmdir $AREADETECTOR_PATH/$AREADETECTOR_DIRECTORY/$ADVIEWERS_LINK
ln -s $ADVIEWERS_PATH $AREADETECTOR_PATH/$AREADETECTOR_DIRECTORY/$ADVIEWERS_LINK

# this script needs to be run within its own directory
pushd $AREADETECTOR_PATH/current/configure
./copyFromExample
popd

# sed -i -e "/^SUPPORT\s*=/ s,=.*,=$SUPPORT_PATH," $ASYN_PATH/current/configure/RELEASE

##hack config_site.local
sed -i -e "/^WITH_PVA\s*=/ s,=.*,=NO," $AREADETECTOR_PATH/current/configure/CONFIG_SITE.local
sed -i -e "/^WITH_OPENCV\s*=/ s,=.*,=NO," $AREADETECTOR_PATH/current/configure/CONFIG_SITE.local

# hack RELEASE.local file
sed -i -e "/^#ADURL\s*=/ s,=.*,=\$(AREA_DETECTOR)/ADURL," $AREADETECTOR_PATH/current/configure/RELEASE.local
sed -i "s/#ADURL/ADURL/g" $AREADETECTOR_PATH/current/configure/RELEASE.local
sed -i -e "/^#ADSIMDETECTOR\s*=/ s,=.*,=\$(AREA_DETECTOR)/ADSimDetector," $AREADETECTOR_PATH/current/configure/RELEASE.local
sed -i "s/#ADSIMDETECTOR/ADSIMDETECTOR/g" $AREADETECTOR_PATH/current/configure/RELEASE.local
sed -i -e "/^ADSUPPORT\s*=/ s,=.*,=\$(AREA_DETECTOR)/ADSupport," $AREADETECTOR_PATH/current/configure/RELEASE.local

# hack RELEASE_PATHS file
sed -i -e "/^SUPPORT\s*=/ s,=.*,=$SUPPORT_PATH," $AREADETECTOR_PATH/current/configure/RELEASE_PATHS.local
sed -i -e "/^AREA_DETECTOR\s*=/ s,=.*,=$AREADETECTOR_PATH/current," $AREADETECTOR_PATH/current/configure/RELEASE_PATHS.local
sed -i -e "/^EPICS_BASE\s*=/ s,=.*,=$EPICS_ROOT/base," $AREADETECTOR_PATH/current/configure/RELEASE_PATHS.local

# hack RELEASE_LIBS file
sed -i -e "/^ASYN\s*=/ s,=.*,=$SUPPORT_PATH/asyn/current," $AREADETECTOR_PATH/current/configure/RELEASE_LIBS.local

# hack RELEASE_PRODS file
sed -i -e "/^CALC\s*=/ s,=.*,=$SUPPORT_PATH/calc/current," $AREADETECTOR_PATH/current/configure/RELEASE_PRODS.local
sed -i -e "/^BUSY\s*=/ s,=.*,=$SUPPORT_PATH/busy/current," $AREADETECTOR_PATH/current/configure/RELEASE_PRODS.local
sed -i -e "/^SSCAN\s*=/ s,=.*,=$SUPPORT_PATH/sscan/current," $AREADETECTOR_PATH/current/configure/RELEASE_PRODS.local
sed -i -e "/^AUTOSAVE\s*=/ s,=.*,=$SUPPORT_PATH/autosave/current," $AREADETECTOR_PATH/current/configure/RELEASE_PRODS.local
sed -i -e "/^SNCSEQ\s*=/ s,=.*,=$SUPPORT_PATH/seq/current," $AREADETECTOR_PATH/current/configure/RELEASE_PRODS.local
# Don't think we need this.
sed -i "s/DEVIOCSTATS/#DEVIOCSTATS/g" $AREADETECTOR_PATH/current/configure/RELEASE_PRODS.local

#These are example plugin files, but we will just use them
mv $ADCORE_PATH/iocBoot/EXAMPLE_commonPlugins.cmd $ADCORE_PATH/iocBoot/commonPlugins.cmd 
mv $ADCORE_PATH/iocBoot/EXAMPLE_commonPlugin_settings.req $ADCORE_PATH/iocBoot/commonPlugin_settings.req 

#environment variable - both for now and for new shell.
echo -e \# areaDetector >> $EPICS_ROOT/siteEnv
export AREA_DETECTOR=$AREADETECTOR_PATH/current
echo export AREA_DETECTOR=$AREA_DETECTOR >> $EPICS_ROOT/siteEnv
echo export ADCORE=$AREA_DETECTOR/ADCore >> $EPICS_ROOT/siteEnv
echo export ADURL=$AREA_DETECTOR/ADURL >> $EPICS_ROOT/siteEnv
echo export ADSIMDETECTOR=$AREA_DETECTOR/ADSimDetector >> $EPICS_ROOT/siteEnv

# add to binaries to PATH
echo export PATH=\${PATH}:$AREA_DETECTOR/ADURL/iocs/urlIOC/bin/\${EPICS_HOST_ARCH} >> $EPICS_ROOT/siteEnv
echo export PATH=\${PATH}:$AREA_DETECTOR/ADSimDetector/iocs/urlIOC/bin/\${EPICS_HOST_ARCH} >> $EPICS_ROOT/siteEnv

# build
make -j2 -O -C $AREADETECTOR_PATH/current install

# ImageJ
# see https://imagej.nih.gov/ij/docs/install/linux.html
IMAGEJ_DOWNLOAD=ij150-linux64-java8.zip
USRLOCALPATH=/usr/local
IMAGEJ_PATH=$USRLOCALPATH/ImageJ

if [ ! -d $IMAGEJ_PATH ];
then
	echo export PATH=\${PATH}:$IMAGEJ_PATH >> $EPICS_ROOT/siteEnv

	wget --tries=3 --timeout=10 http://wsr.imagej.net/distros/linux/$IMAGEJ_DOWNLOAD
	unzip $IMAGEJ_DOWNLOAD -d $USRLOCALPATH
	chmod u=rwx,g=rwx,o=rx $IMAGEJ_PATH
	rm $IMAGEJ_DOWNLOAD
fi
