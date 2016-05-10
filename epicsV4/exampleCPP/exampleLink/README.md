# exampleLink

This example implements a PVRecord that accesses another PVRecord.

## Building

If a proper RELEASE.local file exists two directory levels above **exampleLink**,
or in **../configure/RELEASE.local** then just type:

    make

It can also be built by:

    cp configure/ExampleRELEASE.local configure/RELEASE.local
    edit file configure/RELEASE.local
    make

In **configure/RELEASE.local** it may only be necessary to change the definitions
of **EPICS4_DIR** and **EPICS_BASE**.


## To start the exampleLink as part of a V3 IOC

    mrk> pwd
    /home/epicsv4/master/exampleLink/exampleLink/iocBoot/exampleLink
    mrk> ../../bin/$EPICS_HOST_ARCH/exampleLink st.local

or
 
mrk> pwd
    /home/epicsv4/master/exampleLink/exampleLink/iocBoot/exampleLink
    mrk> ../../bin/$EPICS_HOST_ARCH/exampleLink st.remote


