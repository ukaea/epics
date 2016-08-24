ADBinaries Releases
===================

The latest untagged master branch can be obtained at
https://github.com/areaDetector/ADBinaries.

Tagged releases from R2-0 onward can be obtained at 
https://github.com/areaDetector/ADBinaries/releases.

The versions of EPICS base, asyn, and other synApps modules used for each release can be obtained from 
the EXAMPLE_RELEASE_PATHS.local, EXAMPLE_RELEASE_LIBS.local, and EXAMPLE_RELEASE_PRODS.local
files respectively, in the configure/ directory of the appropriate release of the 
[top-level areaDetector](https://github.com/areaDetector/areaDetector) repository.



Release Notes
=============

R2-2 (March 23, 2015)
----
* Updated the HDF5 libraries from release 1.8.7 to 1.8.14.  The names of the libraries have changed, so
  Makefiles must be modified.  Both static and dynamic libraries are provided.  The symbol HDF5_STATIC_BUILD 
  must be defined to instruct the build system which version of the library to use: YES=static, NO=dynamic.


R2-1 (October 17, 2014)
----
* Removed the Cygwin files from hdf5Support.  libhdf5-devel is available as
  part of the Cygwin installation, and must now be installed rather than using
  files in ADBinaries.


R2-0 (March 7, 2014)
----
* Moved the repository to [Github](https://github.com/areaDetector/ADBinaries).
* Moved the pre-built Windows binary libraries into this repository.


R1-9-1 and earlier
------------------
Release notes are part of the
[areaDetector Release Notes](http://cars.uchicago.edu/software/epics/areaDetectorReleaseNotes.html).
