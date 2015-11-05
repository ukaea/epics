CHANNEL_ARCHIVER_SERVICE__README.txt

This is the README file of the ChannelArchiver service.

Auth: Dave Hickin, 20-Mar-2012 (david.hickin@diamond.ac.uk)


SUMMARY
-------

The ChannelArchiver Service is an EPICS V4 client and server which can be used
to query the data archived by an EPICS Channel Archiver. It includes a bash
script, gethist, which can parse a wide range of dates and times and process
the corresponding data. However the service can be queried by any client
issuing a pvAccess RPC request, such as eget.


EXAMPLES
--------

The server is run as follows:

    % start_server [-S service_name] [index_1 ... index_n]
    
E.g.

    % ./start_server
    % ./start_server -S dccthist data/DCCT/index 
    % ./start_server data/fredjanet/index data/DCCT/index 

start_server calls the ArchiverService executable (which should be
bin/$EPICS_HOST_ARCH/ArchiverServiceRPC) supplying the service name and the
locations of the index files. By default start_server supplies the index file
in data/fredjanet and the service name "archiveService". Should you wish to use
other data or another service name then these should be supplied as arguments
or the defaults changed. The service name is supplied with the -S option.
Subsequent arguments are one or more index files.  


The following are examples of running the client using the bash script gethist:

0) Use the -h or --help options to get help

    % ./gethist -h
    % ./gethist --help

1) Retrieve archived data for the channel "fred" with the default start and end
times, which are 1 hour ago and now.

    % ./gethist fred

2) Retrieve archived data for the channel "janet" between Jan 1 1990 and now.

    % ./gethist -s "Jan 1 1990" -e "now" -f out.txt -x janet

3) Long options are also supported.

    % ./gethist --start "Jan 1 1990" --end "now" --file out.txt
         --scientific janet

4) The options without argument can precede another:

    % ./gethist -xn fred

5) A wide variety of date and time formats are supported (those supported by
date -d). The following are all valid:

    % ./gethist -s "31 Jan 1990" janet
    % ./gethist -s "Jan 31 1990" janet
    % ./gethist -s "January 31 1990" janet
    % ./gethist -s 1990-01-31 janet
    % ./gethist -s 1990-01-31Z janet

You can also supply a time, for example

    % ./gethist -s "30 Jun 2001 17:34" fred
    % ./gethist -s "30 Jun 2001 5:34 p.m." fred
    % ./gethist -s "30 Jun 2001 5:34 p.m." fred
    % ./gethist -s "30 Jun 2001 17:34:56" fred
    % ./gethist -s "30 Jun 2001 17:34 56 seconds" fred
    % ./gethist -s "30 Jun 2001 5:34 p.m. 56 seconds" fred

The following are also valid

    % ./gethist -s  yesterday      -e  now janet
    % ./gethist -s "last week"     -e  today janet
    % ./gethist -s "last year"     -e "last month" janet
    % ./gethist -s "5 minutes ago" -e "10 seconds ago" janet
    % ./gethist -s "2 weeks ago"   -e "1 hour ago" janet
    % ./gethist -s "4 years ago"   -e "3 months ago" janet

6) Multiple channels can be queried with a given set of parameters as follows:

    % ./gethist -s "4 years ago"   -e "3 months ago" fred janet

gethist by default queries the service with name "archiveService". Services
with other names can be queried using the -S option:

    % ./gethist -S a_service -s "yesterday" -e "now" a_channel

The gethist script calls the archive service client (ArchiveClient), which can
be called directly. It takes the same short options as gethist, except that the
start and end times are in seconds past EPICS epoch and the service must always
be specified with the -S argument. For example

    % bin/linux-x86/ArchiverClient -S archiveService -s 405012094
        -e 720631294 fred

ArchiveClient does not support long opts. Both gethist and ArchiveClient
support a number of options see the help for each function for details:

    % ./gethist -h
    % ./gethist --help
    % bin/linux-x86/ArchiverClient -h
    

The service can be also queried using the eget utility. The service name is
specified using the -s argument and the query parameters using -a. For example 

    % eget -s archiveService -a "starttime=405012094" -a "endtime=720631294"
        -a "entity=fred" 

(See below for an explanation of the query parameters).


SERVICE INTERFACE AND RESPONSE FORMAT
-------------------------------------

Requests to the service can be made using the NTURI normative type (see the
EPICS V4 website:

 http://epics-pvdata.sourceforge.net/alpha/normativeTypes/normativeTypes.html).

The query parameters are "entity", "starttime", "endtime" and "maxrecords",
which are respectively the channel name, the start and end times between which
to query (in seconds past the EPICS epoch, 01-01-1990 00:00:00 UTC") and the
maximum number of entries to retrieve. The "entity" field is required. The
server will error the request if it is absent. The others have default values
of 0, 2,147,483,647 and 1,000,000,000 respectively.

Thus the service can be queried using eget by supplying the parameters above.
For example

    % eget -s archiveService -a "starttime=600825600" -a "endtime=600912000"
        -a "maxrecords=1000"-a "entity=SR-DI-DCCT-01:SIGNAL" 

retrieves the first 1000 archived values of SR-DI-DCCT-01:SIGNAL between the
start and end times specified in seconds past the EPICS epoch - in this case
between 15-01-2009 00:00 UTC and 16-01-2009 00:00 UTC - from the service
"archiveService" and formats the result in columns.

The service also accepts queries in the pure query form, i.e. a structure
containing the string fields specified and thus the service can be queried
using the eget -z option.

The service's response is in the form of an NTTable normative type with the
columns "value", "secondsPastEpoch", "nanoseconds", "status" and "severity",
which are arrays of respectively doubles, longs, ints, ints and ints.

Currently the service queries the index files in turn and appends the results.
If the index files cover different time periods therefore the file names should
be supplied chronologically.


FILES THAT COMPRISE THE CHANNEL ARCHIVER SERVICE
------------------------------------------------

ArchiverServiceRPC.cpp                C++ source code of the server side of
ArchiverServiceRPC.h                  the channel archiver service
ArchiverServiceRPCMain.cpp                

ArchiverClient.cpp                    C++ source code of the client application
                                      (top level)

ArchiverClientResponseHandler.cpp     C++ source code for the client handling
ArchiverClientResponseHandler.h       of the response from the server
 
common.cpp                            C++ source code of types and utilities
common.h                              common to client and server

start_server                          bash script to start server

gethist                               bash script to query service

data                                  directory containing sample Channel
                                      Archiver data

test                                  directory containing a script for testing
                                      the service. Type run_test -h for details
                                      of how to use this.


SUPPLIED DATA
-------------

By default the server queries some supplied data which has two channels ("fred"
and "janet") and which covers the period on the 21st September 2005 between
17:49:57 and 19:33:16. This is located in the directory data/fredjanet.

The data directory also includes some real data (storage ring beam current).
The channel name is SR-DI-DCCT-01:SIGNAL and has values between 12/01/2009
11:20:41 and 21/01/2009 07:30:12. This is located in data/DCCT.


PREREQUISITES
-------------

1. EPICS V3 Base         - for a large number of EPICS types
2. pvDataCPP             - for pvData
3. pvAccessCPP           - for pvAccess
4. The Channel Archiver  - for Channel Archiver access and types

In addition, depending on your target/compiler, you may need

5. pvCommonCPP           - for (Boost) shared pointers


SETUP
-----

1. EPICS V3 Base, the Channel Archiver and the EPICS V4 prerequisites (pvData,
pvAccess and, if required, pvCommon) should be built.

2. The Channel Archiver service needs to know the location of these. This can
be achieved by creating a RELEASE.local file in the configure directory of the
ChannelArchiverService directory.

For example if your EPICS V3 Base and Extensions code is in /epics and the
directories of the EPICS V4 prerequisites you wish to link to are in /epics4
then it could be of the form

PVACCESS=/epics4/pvAccessCPP
PVDATA=/epics4/pvDataCPP
PVCOMMON=/epics4/pvCommonCPP
ARCHIVER=/epics/extensions
EPICS_BASE=/epics/base

(PVCOMMON may not be needed depending on your target/compiler.)

ARCHIVER should point to the directory containing src/ChannelArchiver. This
will depend on the layout of your EPICS installation.

RELEASE.local will also be looked for in the two parent directories above
ChannelArchiverService. So instead a RELEASE.local, which should include the
above definitions, can be placed in either of those locations.

3. The Archiver server and client are built by the command "make" issued from
the ChannelArchiverService directory.

4. The start_server script starts the ChannelArchiverService server using the
data supplied in the directory "data". Should you wish to use other data you
need have data in the usual format created by the ChannelArchiver application,
including the index file, and will need to modify the start_server script to
point to this as described below (see the ChannelArchiver manual, for example,

  http://www.slac.stanford.edu/grp/lcls/controls/global/sw/epics/extensions/
    ChannelArchiver/manual.pdf).


EXECUTION
---------
This section describes how you start server and client sides of the Channel
Archiver Service. Start the server side first.

To start the Channel Archiver server
------------------------------------

 * cd to the directory containing start_server

   E.g. % cd ~/Development/epicsV4/exampleCPP/ChannelArchiverService

 * Start the channel archiver service using the start_server command.
   The command can take arguments specifying the service name and index files 
   or use default values. The service name is specified via the -S option and
   the subsequent arguments are the names of the index files. The script
   calls the ArchiverService executable for the host architecture (which should
   be bin/$EPICS_HOST_ARCH/ArchiverServiceRPC). By default it queries the index
   file in data/fredjanet with service name archiveService. 

 * Start the channel archiver service using the start_server command as
   described above using default arguments or supplying service name and index
   files

   E.g. % ./start_server 
        % ./start_server -S dccthist data/DCCT/index 
        % ./start_server data/fredjanet/index data/DCCT/index
          
 * The server can be terminated with a SIGTERM (like CTRL-C in its process
   window).
   
   
To run a Channel Archiver Service Client using gethist
------------------------------------------------------

With the server started, 

 * In another window from the server cd to the directory containing gethist
   (both client and server are in the same directory for demo purposes)
   
   E.g. % cd ~/Development/epicsV4/exampleCPP/ChannelArchiverService
 
 * Execute the gethist as described above
 
   E.g. % ./gethist --start "Jan 1 1990" --end "now" --file out.txt
             --scientific janet 
        % ./gethist -S dccthist --start "10 years ago" --end "yesterday"
             SR-DI-DCCT-01:SIGNAL


Querying the service using eget
-------------------------------

With the server started, assuming eget is in your path:

 * Execute the eget utility:

   eget -s archiveService -a "starttime=600825600" -a "endtime=600912000"
        -a "entity=SR-DI-DCCT-01:SIGNAL"

(For help with eget type "eget -h".)

