#!/bin/bash 

# copyright (c) Tessella 2014

# WORK IN PROGRESS


# check if user has right permissions
if [ "$(id -u)" != "0" ]; then
	echo "Sorry, you are not root. Please try again using sudo."
	exit 1
fi

DEFAULT_EPICS_ROOT="/usr/local/epics"
if [ -z "$*" ]; then EPICS_ROOT=$DEFAULT_EPICS_ROOT; else EPICS_ROOT=$1;fi

# in order to get the EPICS_HOST_ARCH variable, it needed later in this script
. $EPICS_ROOT/siteEnv

# terminate script after first line that fails
set -e

# medm
MEDM_DOWNLOAD="MEDM3_1_11.tar.gz"
MEDM_DIRECTORY="medm-MEDM3_1_11"

apt-get -y install build-essential
apt-get -y install -f libmotif-dev
apt-get -y install -f libxmu-dev

wget --no-check-certificate --tries=3 --timeout=10  http://www.aps.anl.gov/epics/EpicsDocumentation/ExtensionsManuals/MEDM/medmfonts.ali.txt
cat medmfonts.ali.txt >> /usr/share/fonts/X11/misc/fonts.alias
rm -f medmfonts.ali.txt

wget --no-check-certificate --tries=3 --timeout=10  https://epics.anl.gov/download/extensions/$MEDM_DOWNLOAD
tar xzvf $MEDM_DOWNLOAD -C $EPICS_ROOT/extensions/src
rm -f $MEDM_DOWNLOAD

# need to modify the file below so to find gnu libraries for medm to compile
sed -i -e "/^MOTIF_LIB=/ s,=.*,=/usr/lib/x86_64-linux-gnu," $EPICS_ROOT/extensions/configure/os/CONFIG_SITE.$EPICS_HOST_ARCH.$EPICS_HOST_ARCH
sed -i -e "/^X11_LIB=/ s,=.*,=/usr/lib/x86_64-linux-gnu," $EPICS_ROOT/extensions/configure/os/CONFIG_SITE.$EPICS_HOST_ARCH.$EPICS_HOST_ARCH

#The Xp library is not available and not needed on Linux Mint 18 (or Ubuntu 16.04, apparently)
sed -i "s/USR_LIBS_Linux = Xm Xt Xp Xmu X11 Xext/USR_LIBS_Linux = Xm Xt Xmu X11 Xext/g" $EPICS_ROOT/extensions/src/$MEDM_DIRECTORY/medm/Makefile

make -j2 -O -C $EPICS_ROOT/extensions/src/$MEDM_DIRECTORY install

#symbolic link
rm -f $EPICS_ROOT/extensions/src/medm
ln -s $EPICS_ROOT/extensions/src/$MEDM_DIRECTORY $EPICS_ROOT/extensions/src/medm

# from http://www.aps.anl.gov/epics/EpicsDocumentation/ExtensionsManuals/MEDM/MEDM.html#Fonts
mkdir  -p /usr/lib/X11/fonts/misc
echo widgetDM_4   -misc-fixed-medium-r-normal--8-60-100-100-c-50-iso8859-1 > fonts.alias
echo widgetDM_6   -misc-fixed-medium-r-normal--8-60-100-100-c-50-iso8859-1 >> fonts.alias
echo widgetDM_8   -misc-fixed-medium-r-normal--9-80-100-100-c-60-iso8859-1 >> fonts.alias
echo widgetDM_10  -misc-fixed-medium-r-normal--10-100-75-75-c-60-iso8859-1 >> fonts.alias
echo widgetDM_12  -misc-fixed-medium-r-normal--13-100-100-100-c-70-iso8859-1 >> fonts.alias
echo widgetDM_14  -misc-fixed-medium-r-normal--14-110-100-100-c-70-iso8859-1 >> fonts.alias
echo widgetDM_16  -misc-fixed-medium-r-normal--15-120-100-100-c-90-iso8859-1 >> fonts.alias
echo widgetDM_18  -sony-fixed-medium-r-normal--16-120-100-100-c-80-iso8859-1 >> fonts.alias
echo widgetDM_20  -misc-fixed-medium-r-normal--20-140-100-100-c-100-iso8859-1 >> fonts.alias
echo widgetDM_22  -sony-fixed-medium-r-normal--24-170-100-100-c-120-iso8859-1 >> fonts.alias
echo widgetDM_24  -sony-fixed-medium-r-normal--24-170-100-100-c-120-iso8859-1 >> fonts.alias
echo widgetDM_30  -adobe-times-medium-r-normal--25-180-100-100-p-125-iso8859-1 >> fonts.alias
echo widgetDM_36  -adobe-helvetica-medium-r-normal--34-240-100-100-p-176-iso8859-1 >> fonts.alias
echo widgetDM_40  -adobe-helvetica-bold-r-normal--34-240-100-100-p-182-iso8859-1 >> fonts.alias
echo widgetDM_48  -adobe-helvetica-bold-r-normal--34-240-100-100-p-182-iso8859-1 >> fonts.alias
echo widgetDM_60  -adobe-helvetica-bold-r-normal--34-240-100-100-p-182-iso8859-1 >> fonts.alias
mv fonts.alias /usr/lib/X11/fonts/misc

# set environment variables
echo -e \# medm >> $EPICS_ROOT/siteEnv
echo -e \export EPICS_DISPLAY_PATH=\${ADCORE}/ADApp/op/adl:\${ADURL}/urlApp/op/adl:\${ADSIMDETECTOR}/simDetectorApp/op/adl:\${EPICS_ROOT}/support/asyn/current/opi/medm:\${EPICS_ROOT}/support/autosave/asApp/op/adl >> $EPICS_ROOT/siteEnv


