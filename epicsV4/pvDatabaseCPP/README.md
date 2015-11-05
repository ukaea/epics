pvaDatabaseCPP
============


A brief description of a pvDatabase is that it is a set of network accessible, smart, memory resident records. Each record has data composed of a top level PVStructure. Each record has a name which is the channelName for pvAccess. A local Channel Provider implements the complete ChannelProvider and Channel interfaces as defined by pvAccess. The local provider provides access to the records in the pvDatabase. This local provider is accessed by the remote pvAccess server. A record is smart because code can be attached to a record, which is accessed via a method named process.
pvaDatabase is a synchronous Database interface to pvAccess,
which is callback based.
pvaDatabase is thus easier to use than pvAccess itself.

See documentation/pvaDatabaseCPP.html for details.

Building
--------

If a proper RELEASE.local file exists one directory level above pvaDatabaseCPP
then just type:

    make

If RELEASE.local does not exist then look at <b>configure/RELEASE</b>
for directions for how to build.

Examples
------------

The examples require the database in pvaDatabaseTestCPP.
For example:

    mrk> pwd
    /home/epicsv4/pvaDatabaseTestCPP/database/iocBoot/exampleDatabase
    mrk> ../../bin/linux-x86_64/exampleDatabase st.cmd 

Status
------

* The API is for EPICS Version 4 release 4.5.0


