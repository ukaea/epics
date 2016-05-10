# powerSupply


This is an example of creating a service that requires a somewhat complicated top level PVStructure.

The example record has the structure:

    powerSupply
    structure 
        alarm_t alarm
            int severity
            int status
            string message
        time_t timeStamp
            long secondsPastEpoch
            int nanoseconds
            int userTag
        structure power
            double value
        structure voltage
            double value
        structure current
            double value

The record is meant to be accessed via a channelPutGet request.
The client can provide values for power.value and voltage.value.
The process routine computes current.value.

## Building

If a proper RELEASE.local file exists two directory levels above **powerSupply**,
or in **../configure/RELEASE.local** then just type:

    make

It can also be built by:

    cp configure/ExampleRELEASE.local configure/RELEASE.local
    edit file configure/RELEASE.local
    make

In **configure/RELEASE.local** it may only be necessary to change the definitions
of **EPICS4_DIR** and **EPICS_BASE**.


## To start the powerSupply as part of a V3 IOC

    mrk> pwd
    /home/epicsv4/master/exampleCPP/powerSupply/iocBoot/powerSupply
    mrk> ../../bin/$EPICS_HOST_ARCH/powerSupply st.cmd 

## To start the powerSupply as a standalone main

    mrk> pwd
    /home/epicsv4/master/exampleCPP/powerSupply
    mrk> bin/$EPICS_HOST_ARCH/powerSupplyMain

## After starting powerSupply

    mrk> pwd
    /home/epicsv4/master/exampleCPP/powerSupply
    mrk> bin/$EPICS_HOST_ARCH/powerSupplyClient

This client calls channelPutGet several times specifying different value for power and voltage.

The last call sets the voltage to 0, which results in process throwing an exception.

## powerSupplyMonitor

There is also an example to monitor changes in the powerSupply.

    mrk> pwd
    /home/epicsv4/master/exampleCPP/powerSupply
    mrk> bin/$EPICS_HOST_ARCH/powerSupplyMonitor
