HELLOWORLD_README.txt

This is the README file of the HelloWorld service.

Auth: Dave Hickin, 17-Jan-2013 (david.hickin@diamond.ac.uk)


SUMMARY
-------

HelloWorld is a minimum client and server in the EPICS v4 services framework,
intended to show the basic programming required to implement a service that
just takes an argument and returns a string.


EXAMPLE
-------
The following is an example of running the client, giving the argument "Jessica"

          % ./helloClientRunner Jessica
          Hello Jessica  


FILES THAT COMPRISE THE HELLOWORLD EXAMPLE
------------------------------------------
HelloService*.*               C++ source code of the server side of the example
HelloClient.cpp               C++ source code of the client side of the example
helloClientRunner             A unix (bash) executable script to run the client 
helloServerRunner             A unix (bash) executable script to run the server 


PREREQUISITES
-------------

1. EPICS V3 Base          - for a large number of EPICS types
2. pvDataCPP              - for pvData 
3. pvAccessCPP            - for pvAccess

In addition, depending on your target/compiler, you may need

4. pvCommonCPP            - for (Boost) shared pointers


SETUP
-----

1. EPICS V3 Base and the EPICS V4 prerequisites (pvData, pvAccess and, if
required, pvCommon) should be built.

2. The HelloWorld service needs to know the location of these. This can be
achieved by creating a RELEASE.local file in the configure directory of the
HelloWorld directory.

For example if your EPICS V3 Base code is in /epics and the directories of the
EPICS V4 prerequisites you wish to link to are in /epics4 then it could be of
the form 

PVACCESS=/epics4/pvAccessCPP
PVDATA=/epics4/pvDataCPP
PVCOMMON=/epics4/pvCommonCPP
EPICS_BASE=/epics/base

(PVCOMMON may not be needed depending on your target/compiler.)

RELEASE.local will also be looked for in the two parent directories above
HelloWorld. So instead a RELEASE.local, which should include the above
definitions, can be placed in either of those locations.

3. The HelloWorld server and client are built by the command "make" issued from
the HelloWorld directory.


EXECUTION
---------
This section describes how you start server and client sides of the helloWorld
Service. Start the server side first.

To start the HelloWorld server
-------------------------------
 * cd to the directory containing HelloWorld

   E.g. % cd ~/Development/epicsV4/exampleCPP/HelloWorld
  
 * Start the server in one terminal 

   E.g. % ./helloServerRunner
  
 * Terminate the server with a SIGTERM (like CTRL-C in its process window)
   - after you've tested it with the client below of course.
   
   
To run a HelloWorld Client
---------------------------
 * In another window from the server:
 
 * cd to the directory containing HelloWorld (both client and server are in the
   same directory for demo purposes)
   
   E.g. % cd ~/Development/epicsV4/exampleCPP/HelloWorld
 
 * Execute the client side demo, optionally giving your name!
 
   E.g. % ./helloClientRunner Dave
        Hello Dave


Querying the service using eget
-------------------------------
The service can be queried using the eget utility with the -z flag.
With the server started, assuming eget is in your path:

 * Execute the eget utility:

   eget -z -N -s helloService -a "personsname=Dave"

This should output:

structure 
    string greeting Hello Dave

(For help with eget type "eget -h".)

Remark
------
This is the C++ version of the equivalent Java service (exampleJava) written by
Greg White. 
