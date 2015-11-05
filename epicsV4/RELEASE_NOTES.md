4.5.0 (2015/10/16)
==================

The main changes since release 4.4.0 are:


pvCommonCPP (4.1.0)
-------------------

* Made microbench library and header optional


pvDataCPP (5.0.2)
-----------------

* Deprecated getXXXField() methods have been removed from PVStructure.
  (Calls have been removed from all modules.)
* Convert copy methods and equals operators (re)moved.
* Convert::copyUnion now always copies between subfields.
* New method getSubFieldT which throws for non-existent fields.
* findSubField method removed from PVStructure.
* New stream operators for Field and PVField are provided.
* New template versions of Structure::getField.
* Fixes for static initialisation order issues.
* CreateRequest prevents a possible SEGFAULT.


pvAccessCPP (4.1.2)
-------------------

* CA support for pvget, pvput and pvinfo.
* vxWorks/RTEMS compiler warnings resolved.
* Transport shutdown improved.
* CA DBR status fix.
* Monitor queue handling improved.
* Fixed bad performance on 10Gbit or faster networks.
* Async RPC service.


normativeTypesCPP (5.0.2)
-------------------------

* This release adds support through wrapper classes and builders for the
 remaining Normative Types:
    * NTEnum
    * NTMatrix
    * NTURI
    * NTAttribute
    * NTContinuum
    * NTHistogram
    * NTAggregate
    * NTUnion
    * NTScalarMultiChannel
* Implements fully the
[16 Mar 2015 version](http://epics-pvdata.sourceforge.net/alpha/normativeTypes/normativeTypes_20150316.html)
 of the normativeTypes specification.
* API for each wrapper class extended. (is_a and isCompatble take either a
  Structure or a PVStructure. isValid function added.)
* Support for NTAttributes extended as required by NTNDArray
  (NTNDArrayAttributes).
* A new class for parsing NT IDs (NTID).
* Confusion between column names and labels in NTTable resolved. API extended.
* isConnected is treated as an optional rather than a required field in
  NTMultiChannelArray.


pvaSrv (4.1.2)
--------------

* Multiple issues fixed:

    * #16 Alarm status field is incorrect for both gets and monitors.
    * #14 Monitor failing with string exception.
    * #13 Scalar Array PVs have wrong type ID.
    * #8 Choices field incorrect for menu and device fields.
    * #4 gets fail for alarm and timeStamp fields of pvaSrv enum PV.
    * #2 No alarm.status mapping.

* Add variant for building against EPICS base 3.15 variant (new API in Base).
  pvaSrv now works on recent 3.14 and all 3.15.


pvaClientCPP (4.1.1)
--------------------

Initial release. Higher-level API for created pvAccess clients. Support for Channel Get, Put and Monitor. Initial support for multi-channel.  


pvDatabaseCPP (4.1.1)
---------------------

* Fixed race condition when server server exits
* Monitors simplified and race condition fixed. API changed.


pvaPy (0.5.2)
-------------

* Added support for unions (both variant and restricted)
* Added getObject/setObject methods for retrieving/setting field values
  as/using native python types
* Reorganized build configuration
* Removed code for building against EPICS 4.3.0
* Converted to pvaClient API from pvAccessCPP module
* README and Release notes converted to Markdown


exampleCPP (4.1.1)
------------------

(No significant changes)
