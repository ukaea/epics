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

# terminate script after first line that fails
set -e

# dependencies
# base
if [ ! -d $EPICS_ROOT/base ]; 
then
    ./epics_base_3-15-5_install.sh $EPICS_ROOT
fi
  
# Sourced, so the environment is set for this shell - $EPICS_HOST_ARCH is exported.
. $EPICS_ROOT/siteEnv

if [ ! -f /etc/redhat-release ];
then
    apt-get -y install libxmu-dev
    apt-get -y install build-essential libmotif-dev x11proto-print-dev libxp-dev libxmu-dev libxpm-dev xfonts-100dpi
fi
#else
#    yum -y install build-essential libmotif-dev x11proto-print-dev libxp-dev libxmu-dev libxpm-dev xfonts-100dpi
#fi

#So the environment is set for this shell - $EPICS_HOST_ARCH is exported.
. $EPICS_ROOT/siteEnv

# medm
MEDM_DOWNLOAD="medm3_1_7.tar.gz"
MEDM_DIRECTORY="medm3_1_7"

wget --tries=3 --timeout=10  http://www.aps.anl.gov/epics/download/extensions/$MEDM_DOWNLOAD
tar xzvf $MEDM_DOWNLOAD -C $EPICS_ROOT/extensions/src
rm -f $MEDM_DOWNLOAD

# need to modify the file below so to find gnu libraries for medm to compile
sed -i -e "/^MOTIF_LIB=/ s,=.*,=/usr/lib/x86_64-linux-gnu," $EPICS_ROOT/extensions/configure/os/CONFIG_SITE.$EPICS_HOST_ARCH.$EPICS_HOST_ARCH
sed -i -e "/^X11_LIB=/ s,=.*,=/usr/lib/x86_64-linux-gnu," $EPICS_ROOT/extensions/configure/os/CONFIG_SITE.$EPICS_HOST_ARCH.$EPICS_HOST_ARCH

make -C $EPICS_ROOT/extensions/src/$MEDM_DIRECTORY install

#symbolic link
rm -f $EPICS_ROOT/extensions/src/medm
ln -s $EPICS_ROOT/extensions/src/$MEDM_DIRECTORY $EPICS_ROOT/extensions/src/medm

# from http://www.aps.anl.gov/epics/EpicsDocumentation/ExtensionsManuals/MEDM/MEDM.html#Fonts
# and http://manpages.ubuntu.com/manpages/precise/man8/update-fonts-alias.8.html
WIDGETDM_ALIAS="/etc/X11/fonts/misc/widgetdm.alias"
echo widgetDM_4   -misc-fixed-medium-r-normal--8-60-100-100-c-50-iso8859-1 > $WIDGETDM_ALIAS
echo widgetDM_6   -misc-fixed-medium-r-normal--8-60-100-100-c-50-iso8859-1 >> $WIDGETDM_ALIAS
echo widgetDM_8   -misc-fixed-medium-r-normal--9-80-100-100-c-60-iso8859-1 >> $WIDGETDM_ALIAS
echo widgetDM_10  -misc-fixed-medium-r-normal--10-100-75-75-c-60-iso8859-1 >> $WIDGETDM_ALIAS
echo widgetDM_12  -misc-fixed-medium-r-normal--13-100-100-100-c-70-iso8859-1 >> $WIDGETDM_ALIAS
echo widgetDM_14  -misc-fixed-medium-r-normal--14-110-100-100-c-70-iso8859-1 >> $WIDGETDM_ALIAS
echo widgetDM_16  -misc-fixed-medium-r-normal--15-120-100-100-c-90-iso8859-1 >> $WIDGETDM_ALIAS
echo widgetDM_18  -sony-fixed-medium-r-normal--16-120-100-100-c-80-iso8859-1 >> $WIDGETDM_ALIAS
echo widgetDM_20  -misc-fixed-medium-r-normal--20-140-100-100-c-100-iso8859-1 >> $WIDGETDM_ALIAS
echo widgetDM_22  -sony-fixed-medium-r-normal--24-170-100-100-c-120-iso8859-1 >> $WIDGETDM_ALIAS
echo widgetDM_24  -sony-fixed-medium-r-normal--24-170-100-100-c-120-iso8859-1 >> $WIDGETDM_ALIAS
echo widgetDM_30  -adobe-times-medium-r-normal--25-180-100-100-p-125-iso8859-1 >> $WIDGETDM_ALIAS
echo widgetDM_36  -adobe-helvetica-medium-r-normal--34-240-100-100-p-176-iso8859-1 >> $WIDGETDM_ALIAS
echo widgetDM_40  -adobe-helvetica-bold-r-normal--34-240-100-100-p-182-iso8859-1 >> $WIDGETDM_ALIAS
echo widgetDM_48  -adobe-helvetica-bold-r-normal--34-240-100-100-p-182-iso8859-1 >> $WIDGETDM_ALIAS
echo widgetDM_60  -adobe-helvetica-bold-r-normal--34-240-100-100-p-182-iso8859-1 >> $WIDGETDM_ALIAS

update-fonts-alias misc

# set environment variables
echo -e \# medm >> $EPICS_ROOT/siteEnv
echo -e \export EPICS_DISPLAY_PATH=\${AREA_DETECTOR}/ADCore/ADApp/op/adl:\${EPICS_ROOT}/support/asyn/current/opi/medm:\${EPICS_ROOT}/support/autosave/asApp/op/adl >> $EPICS_ROOT/siteEnv
