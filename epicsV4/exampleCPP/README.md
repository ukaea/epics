# exampleCPP

This project provides a set of examples of client and server code for 
pvAccess.
Each example can be built separately by building in the example subdirectory.
The complete set of examples, except for ChannelArchiverService can also be
built by building in exampleCPP itself.

Each example can be used as a template for building other applications:

* Choose an example that is similar to the desired application.
* Copy the complete example to where the desired application should reside.
* Edit the new application.

There is also a test that supports:

    make runtests

This test requires that at least **database** is built.

## Building via exampleCPP

This is only a good idea if a proper **RELEASE.local** is present one 
directory level above **exampleCPP**.

Just type:

    make

An example of a proper *RELEASE.local* is:

    EPICS4_DIR=/home/epicsv4/master
    EXAMPLE=${EPICS4_DIR}/exampleCPP
    PVDATABASE=${EPICS4_DIR}/pvDatabaseCPP
    PVACLIENT=${EPICS4_DIR}/pvaClientCPP
    PVASRV=${EPICS4_DIR}/pvaSrv
    PVACCESS=${EPICS4_DIR}/pvAccessCPP
    NORMATIVETYPES=${EPICS4_DIR}/normativeTypesCPP
    PVDATA=${EPICS4_DIR}/pvDataCPP
    PVCOMMON=${EPICS4_DIR}/pvCommonCPP
    EPICS_BASE=/home/install/epics/base

All examples can also be built by:

    cp configure/ExampleRELEASE.local configure/RELEASE.local
    edit file configure/RELEASE.local
    make

In **configure/RELEASE.local** it may only be necessary to change the definitions
of **EPICS4_DIR** and **EPICS_BASE**.


## Building An Individual Example

If a proper **RELEASE.local** is present one directory level above
**exampleCPP** then in the example directory just type:

    make

Each example has a **README.md** file. See it for how to build the example
if a proper **RELEASE.local** does not exist or if the example has special
requirements.

## Brief summary of examples.



### database

Provides an EPICS IOC that has both EPICS V3 DBRecords, V4 PVRecords, and
pvaSrv for accessing the DBRecords.

It can also create a standalone main that has a set of V4 PVRecords.


### exampleClient

Provides a set of examples that use pvaClientCPP.

In order to run the examples, **database** must also be built and then the
IOC database must be started as follows:

    mrk> pwd
    /home/epicsv4/master/exampleCPP/database/iocBoot/exampleDatabase
    mrk> ../../bin/$EPICS_HOST_ARCH/exampleDatabase st.cmd


### helloPutGet

An example of a PVRecord that implements a "Hello World" service that can be
accessed via ChannelPutGet.

### helloRPC

A very simple example of an EPICS V4 RPC service: This implements a
"Hello World" example of a service that is accessed via Channel RPC.

This is a starting point for writing an RPC service without using **pvDatabaseJava**.

But note that example **database** has a similar example implemented via **pvDatabaseJava**, which allows clients to monitor the result of each request.


### exampleLink

This implements a pvAccess server that has a record doubleArray and a PVRecord exampleLink that monitors changes to doubleArray. PVRecord exampleLink uses pvAccess client code to monitor for changes in doubleArray.
It can use provider local, pva, or ca to connect to doubleArray.
The doubleArray can exist in the same IOC or, with provider pva or ca,
can exist in another IOC.

The **exampleLink** can be stated as a main program or as part of a V3 IOC.

### powerSupply

This is an example of creating a PVRecord that uses a somewhat complicated
top level PVStructure. It simulates a power supply.

The example also has an example pvaClient for accessing the PVRecord.


### pvDatabaseRPC

An example of a pvDatabase PVRecord which also supports RPC services.

The record represents a 2-D position, but also provides an RPC service
which moves the position through a sequence of points before returning.
A channel to the record supports the usual operations (such as get, put and
monitor) but also supports Channel RPC, i.e. the RPC service has the same 
channel name as the position PV.

A client ("move") for calling the service is supplied.


###  arrayPerformance

This is an example that shows performance for an array of longs.

arrayPerformanceMain implements a PVRecord that is a long array.
It has a process method with code that causes the array to be updated at
selectable rates and sizes!

It also has pvaClient examples that can get, put and monitor the long array record.


### ChannelArchiverService

The ChannelArchiver Service is an EPICS V4 client and server which can be
used to query the data archived by an EPICS Channel Archiver. It includes a
bash script, gethist, which can parse a wide range of dates and times and
process the corresponding data. However the service can be queried by any
client issuing a pvAccess RPC request, such as eget. Requests and responses
use EPICS V4 normative types. Requests are in the form of an NTURI normative
type and the results are returned as an NTTable.


### test

This is **not** an embeded top level but just a subdirectory of **exampleCPP**.

This is an example that tests pvDatabase and pvaClient.   
This example requires that **database** must also be built. 

It is meant to be used to execute:

    make runtests

When this is done it starts the example database and then executes various
client tests.

