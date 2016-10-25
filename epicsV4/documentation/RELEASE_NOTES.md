# EPICS V4 Release 4.6.0 C++ Bundle Release Notes

This document contains the Release Notes entries from each individual C++
submodule for this bundled version of EPICS V4. For future patch releases any notes about patch changes will supplement the note entries from the original release.


## pvCommonCPP 4.2.2

The Boost header files are now only installed for VxWorks target architectures, since they are only essential for that OS. This prevents clashes with sofware that has been built with a different version of Boost.


------

## pvDataCPP 6.0.1

The main changes since release 5.0.4 are:

* Linux shared library version added
* Headers have been moved into pv directories
* Bitset functions declared const where possible
* Bitset::swap added
* Requester::message has default implementation
* Serialization/deserialization helpers added
* Non-template getSubField char* overload added
* MonitorPlugin deprecated
* Field name validation performed
* Now builds for Cygwin and MinGW targets
* Fix for debug build issue.
* New license file replaces LICENSE and COPYRIGHT

### Shared library version added

Linux shared library version numbers have been added by setting SHRLIB_VERSION
(to 6.0 in this case). So shared object will be libpvData.so.6.0 instead of
libpvData.so.

### Headers have been moved into pv directories

For example

    src/property/alarm.h -> src/property/pv/alarm.h

This facilitates using some IDEs such as Qt Creator.

### Requester::message has default implementation

Requester::message is no longer pure virtual. Default implementation sends
string to `std::cerr`.

### Serialization/deserialization helpers added

A helper function, serializeToVector, has been added which serializes a
Serializable object into a standard vector of `UInt8`s.

Similarly a function deserializeFromVector deserializes a standard vector into
a Deserializable object.

A function deserializeFromBuffer deserializes a `ByteBuffer` into a
Deserializable object.

### Field name validation performed

On creating a Structure or Union the field names are now validated.

Valid characters for a field name are upper or lowercase letters, numbers and
underscores and intial numbers are invalid, i.e. names must be of the form
`[A-Za-z_][A-Za-z0-9_]*`.

### Now builds for Cygwin and MinGW targets

Includes cross-compiling MinGW on Linux.


------

## pvAccessCPP 5.0.0

* Remote channel destroy support
* Multiple network inteface support
* Local multicast (repetitor) reimplemented
* Monitor reconnect when channel type changes fix
* C++11 compilation fixes
* Added version to pvaTools
* Memory management improved
* pipeline: ackAny argument percentage support
* Monitor overrun memory issues fixed
* CA provider destruction fixed
* Replaced LGPL wildcard matcher with simplistic EPICS version


------

## normativeTypesCPP 5.1.2

The main changes since release 5.0 are:

* Linux shared library version added
* Headers and source locations have changed
* Missing is_a implementations added
* NTAttribute::addTags() is now non-virtual
* New license file replaces LICENSE and COPYRIGHT
* NTUnionBuilder: Add missing value() function
* Updated document: Now document all Normative Types

### Shared library version added

Linux shared library version numbers have been added by setting SHRLIB_VERSION
(to 5.1 in this case). So shared object will be libnt.so.5.1 instead of
libpvData.so.

### Headers and source locations have changed

Source has moved out of nt directory directly into src.

Headers have been moved into a pv directory. This facilitates using some IDEs
such as Qt Creator.

    src/nt/ntscalar.cpp -> src/ntscalar.cpp
    src/nt/ntscalar.h   -> src/pv/ntscalar.h

### Missing is_a implementations added

`is_a(PVStructurePtr const &)` implementation has been added for each type.


------

## pvaClientCPP 4.2.1

* The examples are moved to exampleCPP.
* Support for channelRPC is now available.
* In PvaClientMultiChannel `checkConnected()` now throws an exception if connect fails.


------

## pvaSrv 0.12.1

* Major clean-up wrt returned structures on gets and monitors
* Treat enum as uint16 (not int32)
* Security plugin improvements
* Fix issues #3, #5
* SHRLIB_VERSION can be overridden
* Changes in Jenkins jobs (@CloudBees)


------

## pvDatabaseCPP 4.2.1

* The examples are moved to exampleCPP
* Support for channelRPC is now available.
* removeRecord and traceRecord are now available.

The test is now a regression test that can be run using

    make runtests


------

## exampleCPP 4.2.2

* HelloWorld has been renamed helloRPC.

* The following examples have been moved from pvDatabaseCPP:
  * arrayPerformance
  * database
  * exampleLink
  * helloPutGet
  * powerSupply
  * pvDatabaseRPC

* The following have been moved from pvaClientCPP:
  * exampleClient
  * test

* The pvDatabaseRPC example sub-module generates build warnings on Windows
related to name decoration. These issues will be fixed in an upcoming patch
release.

------

## pvaPy 0.6.2

- added support for channel putGet() and getPut() operations
- introduced PvObject support for field path notation (e.g, 'x.y.z')
- introduced PvObject support for `getitem`, `setitem`, `contains`
- new constructor for PvObject allows optional value dictionary
- added PvObject support for retrieving numeric scalar arrays as
  read-only numpy arrays (requires compiling with Boost.NumPy)
- added ability to specify PV request in RPC client code
- fixed PvObject type ID issues
- reworked monitor code to allow two processing modes: immediate processing
  and processing in a separate thread (requires copying/queueing monitor data);
  the processing thread mode now utilizes monitor requester callbacks, and
  unnecessary monitor polling when monitor queue is full has been eliminated


------

## bundleCPP 4.6.0

* New module created for bundling the other EPICS V4 C++ modules.
* Makefile supports simple configuration and parallel builds.

