
EPICS VERSION 4 C++ IMPLEMENTATION README
=========================================

This README is a guide to the build of the C++ implementation of EPICS Version 4. 

Status: This README is up-to-date with respect to release v4.5.0.1 of EPICS Version 4.

Auth:  
Dave Hickin, Diamond, 28-Oct-2015  

Mod:  
Dave Hickin, Diamond, 28-Jan-2016  
Update for 4.5.0.1.  


Prerequisites
-------------

The EPICS V4 C++ bundle requires recent versions of the following
software:

1. EPICS Base (v3.14.12 or 3.15)
2. Standard development tools (gcc, make, etc.)
 
The pvAccess for Python package, pvaPy, requires:

1. Python development header files/libraries (v2.6.6) 
2. Boost (v1.41.0); must have the boost_python library built.
3. autoconf
4. Sphinx (optional, for generating documentation)
 
The Channel Archiver Service requires:

1. the EPICS Channel Archiver.
 

Build
-----

More detailed build instructions can be found in the
[Getting started guide](http://epics-pvdata.sourceforge.net/gettingStarted.html).

Building uses the "make" utility and the EPICS base build system.
For more information on the EPICS build system consult the
[Application Development guide](http://www.aps.anl.gov/epics/base/R3-14/12-docs/AppDevGuide.pdf).

The build system needs the location of the prerequisites for each module.
The easiest way to do this is to create a file called RELEASE.local in the top-level
directory, and to it add lines of the form:

    EV4_BASE=/path/to/epics4
    PVDATABASE=$(EV4_BASE)/pvDatabaseCPP
    PVASRV=$(EV4_BASE)/pvaSrv
    PVACCESS=$(EV4_BASE)/pvAccessCPP
    NORMATIVETYPES=$(EV4_BASE)/normativeTypesCPP
    PVDATA=$(EV4_BASE)/pvDataCPP
    PVCOMMON=$(EV4_BASE)/pvCommonCPP
    EPICS_BASE=/path/to/epics/base

where each line points to the location of the respective EPICS Version 4 module.
Add your paths to the EPICS base (EPICS\_BASE) and the location of the bundle (EV4\_BASE).
An example (ExampleRelease.local) is included in the bundle.

With this in place, to build type:

    make

A bash configration script is provided for building on Linux. In which case
the following will work:

    make EPICS_BASE=/path/to/epics/base make configure
    make

This will build everything except pvaPy and the Channel Archiver Service.

To build pvaPy, having first built the other modules as above, from the top
directory cd into the pvaPy directory, configure and build:

    cd pvaPy
    EPICS_BASE=/path/to/epics/base EPICS4_dir=/path/to/epics4 make configure
    make

To build the Channel Archiver Service, place the line:

    ARCHIVER=/path/to/channel_archiver

in RELEASE.local. Then:

    cd exampleCPP/ChannelArchiverService
    make 


Further information
===================

For the individual modules, consult the documentation in each one. In 
particular:

* README.md
* RELEASE_VERSIONS.md
* The documentation directory

For more information visit the
[EPICS V4 website](http://epics-pvdata.sourceforge.net).

In particular:

* [Getting started guide](http://epics-pvdata.sourceforge.net/gettingStarted.html) - 
  for detailed build instructions and where to go next.
* [Developer guide](http://epics-pvdata.sourceforge.net/informative/developerGuide/developerGuide.html) -
  currently under development.
* [Documentation page](http://epics-pvdata.sourceforge.net/literature.html) -
  Overview documents and doxygen for the various modules.

