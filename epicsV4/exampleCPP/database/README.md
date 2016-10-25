# database

This example provides an EPICS IOC that has both EPICS V3 DBRecords, V4 PVRecords, and pvaSrv for accessing the DBRecords.

It can also create a standalone main that has a set of V4 PVRecords.


## Building

If a proper RELEASE.local file exists two directory levels above **database**,
or in **../configure/RELEASE.local** then just type:

    make

It can also be built by:

    cp configure/ExampleRELEASE.local configure/RELEASE.local
    edit file configure/RELEASE.local
    make

In **configure/RELEASE.local** it may only be necessary to change the definitions
of **EPICS4_DIR** and **EPICS_BASE**.


## To start the database as part of a V3 IOC

    mrk> pwd
    /home/epicsv4/master/exampleCPP/database/iocBoot/exampleDatabase
    mrk> ../../bin/$EPICS_HOST_ARCH/exampleDatabase st.cmd 

## To start the database as a standalone main

    mrk> pwd
    /home/epicsv4/master/exampleCPP/database
    mrk> bin/$EPICS_HOST_ARCH/exampleDatabaseMain

## database/src/pv

This directory has the following files:

     exampleHelloRecord.h
     exampleHelloRPC.h
     exampleDatabase.h
  

## database/src

This directory has the following files:

* exampleHelloRecord.cpp
Code for an example PVRecord that is accessed via channelPutGet.

* exampleHelloRPC.cpp  
Code for an example that is accesed via channelRPC.

* exampleDatabase.cpp 
Code that creates many PVRecords.    
Most are soft records but also exampleHello and exampleHelloRPC.

* exampleDatabaseInclude.dbd and exampleDatabaseRegister.cpp
Code that allows the PVRecords to be part of a V3 IOC.

* exampleDatabaseMain.cpp
Code that allows the PVRecords to be available via a standalone main program.

## database/ioc

Code that allows the database to be part of a V3 IOC.

##  database/ioc/Db
Files for creating V3 DBRecords

## database/ioc/src

* exampleDatabaseMain.cpp   
Standard file for a V3 IOC.
* exampleDatabaseInclude.dbd    
Standard V3 dbd file with additional includes for PVRecords and pvaSrv and local pvAccess.

### database/ioc/iocBoot/exampleDatabase

Support for starting V3 IOC that also has support for pvaSrv and PVRecords.


