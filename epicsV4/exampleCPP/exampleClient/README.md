# exampleClient

Provides a set of examples that use pvaClientCPP.

In order to run the examples, **database** must also be built and then the IOC database must be started as follows:

    mrk> pwd
    /home/epicsv4/master/exampleCPP/database/iocBoot/exampleDatabase
    mrk> ../../bin/$EPICS_HOST_ARCH/exampleDatabase st.cmd

[developerGuide.html ](http://epics-pvdata.sourceforge.net/informative/developerGuide/developerGuide.html)provides tututorial information on how to use pvaClient


## Building

If a proper RELEASE.local file exists two directory levels above **exampleClient**,
or in **../configure/RELEASE.local** then just type:

    make

It can also be built by:

    cp configure/ExampleRELEASE.local configure/RELEASE.local
    edit file configure/RELEASE.local
    make

In **configure/RELEASE.local** it may only be necessary to change the definitions
of **EPICS4_DIR** and **EPICS_BASE**.



## examplePvaClientGet

This has a number of examples.

### exampleDouble

This shows both a short and long way to get data from a scalar channel.
The short way throws an execption if the request fails.
The long way allows the client more control of looking for problems and blocking.


### exampleDoubleArray

Like exampleDouble except the data is a scalarArray.

### exampleCADouble

This is like exampleDouble except it uses provider <b>ca</b>.

### exampleCADoubleArray


This is like exampleDoubleArray except it uses provider <b>ca</b>.



## examplePvaClientMonitor

This is an example of creating a monitor on a channel.
It monitors a scalar double field.
It also issues puts to the same channel so that it can make the monitors occur.


## examplePvaClientPut

This has the folllowing examples.

### examplePut

This shows use of get, put, and monitor.

### examplePVFieldPut

This does a put and then a get.

## examplePvaClientProcess

This example makes a process request to a channel

## examplePvaClientMultiDouble

This is an example of using pvaClientMultiChannel,
pvaClientMultiGetDouble, pvaClientMultiPutDouble, and pvaClientMultiMonitorDouble.


## examplePvaClientNTMulti

This is an example of using pvaClientMultiChannel to get data as an NTMultiChannel

## helloWorldPutGet

This is an example of issuing a channelPutGet.

## helloWorldRPC

This is an example of issuing a channelRPC request.
It does **not** use pva.


