#!/bin/bash

THISDIR=${PWD}
EV4_BASE=$THISDIR


clean()
{
   echo "Removing old RELEASE.locals"
   find . -name RELEASE.local -exec rm {} \; 
}


check_for_config_files()
{
    if [ `find . -name RELEASE.local | wc -l` -ne 0 ]; then
       echo "Existing configuration files found. Remove before configuring"
       echo "Run \"./configuration.sh clean\" to do this."
       exit 3
    fi 
}

check_shell_vars()
{
    if [ -z "${EPICS_BASE}" ]; then
        echo "EPICS_BASE unspecified." 
        exit 2       
    fi

    if [ ! -d "${EPICS_BASE}" ]; then
        echo "EPICS_BASE (${EPICS_BASE}) does not exist."
        exit 2
    fi

    if [ ! -f "${EPICS_BASE}/include/epicsVersion.h" ]; then
        echo "EPICS_BASE version file (${EPICS_BASE}/include/epicsVersion.h) does not exist."
        echo "Incorrect directory specified or not a valid EPICS installation."
        exit 2
    fi

    if [ ! -z "${ARCHIVER_DIR}" ]; then
        if [ ! -d ${ARCHIVER_DIR} ]; then
            echo "ARCHIVER_DIR specified (${ARCHIVER_DIR}), but does not exist."
            exit 2
        fi

        if [ ! -d ${ARCHIVER_DIR}/include ]; then
            echo "ARCHIVER_DIR specified but ${ARCHIVER_DIR}/include does not exist."
            echo "Not a valid channel archiver or channel archiver has not been built."
            exit 2
        fi
    fi
}

start_common()
{
    echo "configuring ..."
    check_for_config_files
    echo "EPICS_BASE =  ${EPICS_BASE}"
    echo "ARCHIVER_DIR = ${ARCHIVER_DIR}"
    check_shell_vars
}


top_level()
{
    echo "EV4_BASE=${EV4_BASE}" > RELEASE.local
    echo "PVDATABASE=\$(EV4_BASE)/pvDatabaseCPP" >> RELEASE.local
    echo "PVASRV=\$(EV4_BASE)/pvaSrv" >> RELEASE.local
    echo "PVACCESS=\$(EV4_BASE)/pvAccessCPP" >> RELEASE.local
    echo "NORMATIVETYPES=\$(EV4_BASE)/normativeTypesCPP" >> RELEASE.local
    echo "PVDATA=\$(EV4_BASE)/pvDataCPP" >> RELEASE.local
    echo "PVCOMMON=\$(EV4_BASE)/pvCommonCPP" >> RELEASE.local
    if [ -d "${ARCHIVER_DIR}" ]; then
        echo "ARCHIVER=${ARCHIVER_DIR}" >> RELEASE.local
    fi
    echo "EPICS_BASE=${EPICS_BASE}" >> RELEASE.local
    echo "RELEASE.local file created"
}

pvcommon()
{
    if [ -e pvCommonCPP/configure ]; then
        pushd pvCommonCPP/configure
        echo "EPICS_BASE=${EPICS_BASE}" > RELEASE.local
        popd
    else
        echo "Skipping pvCommonCPP/configure - doesn't exist" 
    fi
}

pvdata()
{
    if [ -e pvDataCPP/configure ]; then
        echo "Making config files for pvCommonCPP" 
        pushd pvDataCPP/configure
        echo "EV4_BASE=${EV4_BASE}" > RELEASE.local
        echo "PVCOMMON=\$(EV4_BASE)/pvCommonCPP" >> RELEASE.local
        echo "EPICS_BASE=${EPICS_BASE}" >> RELEASE.local   
        popd
    else
        echo "Skipping pvDataCPP: configure - doesn't exist" 
    fi
}

pvaccess()
{
    if [ -e pvAccessCPP/configure ]; then
        echo "Making config files for pvAccessCPP" 
        cd pvAccessCPP/configure
        echo "EV4_BASE=${EV4_BASE}" > RELEASE.local
        echo "PVDATA=\$(EV4_BASE)/pvDataCPP" >> RELEASE.local
        echo "PVCOMMON=\$(EV4_BASE)/pvCommonCPP" >> RELEASE.local
        echo "EPICS_BASE=${EPICS_BASE}" >> RELEASE.local
        cd ../..
    else
        echo "Skipping pvAccessCPP: configure - doesn't exist" 
    fi
}

pvasrv()
{
    if [ -e pvaSrv/configure ]; then
        echo "Making config files for pvaSrv" 
        cd pvaSrv/configure
        echo "EV4_BASE=${EV4_BASE}" > RELEASE.local
        echo "PVACCESS=\$(EV4_BASE)/pvAccessCPP" >> RELEASE.local
        echo "PVDATA=\$(EV4_BASE)/pvDataCPP" >> RELEASE.local
        echo "PVCOMMON=\$(EV4_BASE)/pvCommonCPP" >> RELEASE.local
        echo "EPICS_BASE=${EPICS_BASE}" >> RELEASE.local
        cd ../..
    else
        echo "Skipping pvaSrv: configure doesn't exist" 
    fi
}

pvDatabase()
{
    if [ -e pvDatabaseCPP/configure ]; then
        echo "Making config files for pvDatabaseCPP" 
        cd pvDatabaseCPP/configure
        echo "EV4_BASE=${EV4_BASE}" > RELEASE.local
        echo "PVASRV=\$(EV4_BASE)/pvaSrv" >> RELEASE.local
        echo "PVACCESS=\$(EV4_BASE)/pvAccessCPP" >> RELEASE.local
        echo "PVDATA=\$(EV4_BASE)/pvDataCPP" >> RELEASE.local
        echo "PVCOMMON=\$(EV4_BASE)/pvCommonCPP" >> RELEASE.local
        echo "EPICS_BASE=${EPICS_BASE}" >> RELEASE.local
        cd ../..
    else
        echo "Skipping pvDatabaseCPP: configure doesn't exist" 
    fi
}

nt()
{
    if [ -e normativeTypesCPP/configure ]; then
        echo "Making config files for normativeTypesCPP" 
        cd normativeTypesCPP/configure
        echo "EV4_BASE=${EV4_BASE}" > RELEASE.local
        echo "PVDATA=\$(EV4_BASE)/pvDataCPP" >> RELEASE.local
        echo "PVCOMMON=\$(EV4_BASE)/pvCommonCPP" >> RELEASE.local
        echo "EPICS_BASE=${EPICS_BASE}" >> RELEASE.local
        cd ../..
    else
        echo "Skipping normativeTypesCPP: configure - doesn't exist" 
    fi
}

helloWorld()
{
    if [ -e exampleCPP/HelloWorld/configure ]; then
        echo "Making config files for exampleCPP/HelloWorld" 
        cd exampleCPP/HelloWorld/configure
        echo "EV4_BASE=${EV4_BASE}" > RELEASE.local
        echo "PVACCESS=\$(EV4_BASE)/pvAccessCPP" >> RELEASE.local
        echo "PVDATA=\$(EV4_BASE)/pvDataCPP" >> RELEASE.local
        echo "PVCOMMON=\$(EV4_BASE)/pvCommonCPP" >> RELEASE.local
        echo "EPICS_BASE=${EPICS_BASE}" >> RELEASE.local
        cd ../../..
    else
        echo "Skipping exampleCPP/HelloWorld: configure doesn't exist" 
    fi
}

archiverService()
{
    if [ -e exampleCPP/ChannelArchiverService/configure ]; then
        echo "Making config files for exampleCPP/ChannelArchiverService"
        if [ -z "${ARCHIVER_DIR}" ]; then
            echo "ARCHIVER_DIR not specified, but required for ChannelArchiverService."
            echo "Skipping making config."
        else 
            cd exampleCPP/ChannelArchiverService/configure
            echo "EV4_BASE=${EV4_BASE}" > RELEASE.local
            echo "PVACCESS=\$(EV4_BASE)/pvAccessCPP" >> RELEASE.local
            echo "PVDATA=\$(EV4_BASE)/pvDataCPP" >> RELEASE.local
            echo "PVCOMMON=\$(EV4_BASE)/pvCommonCPP" >> RELEASE.local
            echo "ARCHIVER=${ARCHIVER_DIR}" >> RELEASE.local
            echo "EPICS_BASE=${EPICS_BASE}" >> RELEASE.local
            cd ../../..
        fi
    else
        echo "Skipping exampleCPP/ChannelArchiverService: configure doesn't exist" 
    fi
}


if [ "$1" = "clean" ]; then
    clean

elif [ "$1" = "" ]; then
    start_common
    top_level
    echo "Configuration successful"

elif [ "$1" = "all" ]; then
    start_common
    pvcommon
    pvdata
    pvaccess
    pvasrv
    pvDatabase
    nt
    helloWorld
    archiverService

else
    echo "Unknown option $1"
    exit 1
fi

