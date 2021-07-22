#!/bin/bash

set -e
DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd)

if [ -z "$*" ]; then
    echo "no arguments supplied. 1:configfile 2:destination"
    exit 1
fi

# read config file
CONFIG=$(readlink -f $1)

# source directory is one level up from the script folder
SOURCE=$(readlink -f $DIR/..)
echo Source folder: $SOURCE

ROOT=$(readlink -f $2)
# create the destination folder if it does not exists
DESTINATION=$ROOT/xps
echo Destination folder: $DESTINATION

if [ ! -d $DESTINATION ]; then
    echo "Specified destination directory does not exists. Will create it now."
    mkdir -p $DESTINATION
fi

# monitoring command file
monitoring_file=$DESTINATION/monitoring_xps.cmd
touch $monitoring_file
> $monitoring_file

# monitoring command file
traffic_light_file=$DESTINATION/traffic_light_xps.cmd
touch $traffic_light_file
> $traffic_light_file

# list of devices command file
devices_config_file=$DESTINATION/devices_xps.cmd
touch $devices_config_file
> $devices_config_file
devices=""

while read -a param ; do

    # skip empty line
    [[ -z ${param[0]} ]] && continue

    # skip line starting with #
    [[ ${param[0]} =~ ^#.* ]] && continue

    # get parameters from config file
    pv_name=${param[0]}
    ip_addr=${param[1]}
    ch1_name=${param[2]}
    ch2_name=${param[3]}
    ch3_name=${param[4]}
    ch4_name=${param[5]}
    ch5_name=${param[6]}

    echo "setting up" $pv_name "with IP address" $ip_addr

    # copy IOC (cannot be a 'cp' as this would copy the .svn files as well)
    rsync -arvud --exclude=".svn*" $SOURCE/ $DESTINATION/$pv_name
  
    #create autosave folder if it does not exist
    AUTOSAVE=$ROOT/../autosave/xps/$pv_name
    if [ ! -d $AUTOSAVE ]; then
        echo "Autosave directory does not exist. Will create it now."
        mkdir -p $AUTOSAVE
    fi
	
    # remove opi folder
    rm -rf $DESTINATION/$pv_name/opi

    # set PV name
    find $DESTINATION/$pv_name -type f -exec sed -i "s/XPS_PREFIX/$pv_name/g" {} +

    # set channel name
    find $DESTINATION/$pv_name -type f -exec sed -i "s/XPS_M1_NAME/$ch1_name/g" {} +
    find $DESTINATION/$pv_name -type f -exec sed -i "s/XPS_M2_NAME/$ch2_name/g" {} +
    find $DESTINATION/$pv_name -type f -exec sed -i "s/XPS_M3_NAME/$ch3_name/g" {} +
    find $DESTINATION/$pv_name -type f -exec sed -i "s/XPS_M4_NAME/$ch4_name/g" {} +
    find $DESTINATION/$pv_name -type f -exec sed -i "s/XPS_M5_NAME/$ch5_name/g" {} +
  
    # set IP address
    sed -i -e "/epicsEnvSet(\"XPSIP\"/ s|[0-9]\{1,3\}.[0-9]\{1,3\}.[0-9]\{1,3\}.[0-9]\{1,3\}|$ip_addr|" $DESTINATION/$pv_name/iocBoot/iocXPS/st.cmd

    # set autosave path
    sed -i -e "/set_savefile_path(\"/ s|\$(TOP)/autoSaveRestore|$AUTOSAVE|" $DESTINATION/$pv_name/iocBoot/iocXPS/st.cmd

    # build code
    pushd $DESTINATION/$pv_name/
    make
    popd

    # generate st.cmd commands for the monitoring IOC
    echo "dbLoadRecords(\"db/monitor_ioc.db\", \"DEVICE=MONITOR, NAME=$pv_name, IOC=$pv_name\")" >> $monitoring_file

    # generate st.cmd commands for the traffic light IOC
    echo "dbLoadRecords(\"db/traffic_light.db\",\"NAME=$pv_name\")" >> $traffic_light_file
    echo "dbLoadRecords(\"db/xps_adapter.db\",\"NAME=$pv_name, MONITOR=MONITOR\")" >> $traffic_light_file

    # make a list of PV names
    devices="$devices $pv_name"
    devices_count=$((devices_count+1))

    # add to the list of IOCs to be launched
    echo $pv_name $DESTINATION/$pv_name/ >> $(readlink -f $DIR/../../manage/process/iocBoot/iocprocess/iocs.conf)

done < $CONFIG

# add pv name to list of devices
echo "system \"caput -a -s DEVICES:XPS" $devices_count $devices"\"" >> $devices_config_file

# add list of PV prefixes to config file used by Manager OPI
echo $devices >> $DIR/../../manage/opi/pvs.conf

# deploy the CSS opi screens
DEVICE_NAME=${DESTINATION#$2}  
DEVICE_NAME=${DEVICE_NAME:1} #remove first slash from device name e.g. /scope -> scope 
DESTINATION_OPI=$(dirname $2)
mkdir -p $DESTINATION_OPI/opis/$DEVICE_NAME/opi
rsync -arvud --exclude=".svn*" $SOURCE/opi/ $DESTINATION_OPI/opis/$DEVICE_NAME/opi


