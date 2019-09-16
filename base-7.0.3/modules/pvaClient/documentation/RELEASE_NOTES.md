# pvaClientCPP Module

This document summarizes the changes to the module between releases.

## Release 4.6.0 (EPICS 7.0.3, Jul 2019)

* pvaClient now handles exceptions from the server properly (issue #54).
* MultiChannel classes now properly handle PV structures that don't have a top-level `value` field (issue #56), and are more tolerant of other missing fields (issue #57).


## Release 4.5.0 (EPICS 7.0.2.2, Apr 2019)

Changes have been made for getDouble, putDouble, getDoubleArray, putDoubleArray, getString, putString, getStringArray, and putStringArray.

1) Previously each only had support for a top level field named value.
Each now allows access to a single sub field that has the correct type.
Thus pvRequest must select a single field. For example

    pva->channel("PVRdumbPowerSupply")->putDouble(1.0,"power.value" );

2) PvaChannel now has a method for each of the above.
For example instead of

    PvaClientChannelPtr channel = pva->channel("PVRdouble");
    PvaClientPutPtr clientPut = channel->put();
    PvaClientPutDataPtr putData = clientPut->getData();
    putData->putDouble(1.0); clientPut->put();

now it can be

    pva->channel("PVRdouble")->putDouble(1.0 );

3) getDoubleArray and putDoubleArray work with any numeric scalar array

4) getStringArray and putStringArray work with any scalar array.

## Release 4.4 (EPICS 7.0.2, Dec 2018)

### API changes to PvaClientMonitor

The create method that had arguments for stateChangeRequester and monitorRequester no longer exists.

### API changes to PvaClientGet, ..., PvaClientMonitor

Previously the pvaClientGet, ..., pvaClientMonitor classes all implemented PvaClientChannelStateChangeRequester(). This method was never called and has been removed.


## Release 4.3 (EPICS 7.0.1, Dec 2017)

### Requires pvDataCPP-7.0 and pvAccessCPP-6.0 versions

This release will not work with older versions of these modules.

### Destroy methods removed

All the destroy() methods have been removed, implementation is RAII compliant.

### API changes to PvaClientMonitor

The second argument of method

    static PvaClientMonitorPtr create(
        PvaClientPtr const &pvaClient,
        epics::pvAccess::Channel::shared_pointer const & channel,
        epics::pvData::PVStructurePtr const &pvRequest
    );

is now changed to

    static PvaClientMonitorPtr create(
        PvaClientPtr const &pvaClient,
        PvaClientChannelPtr const & pvaClientChannel,
        epics::pvData::PVStructurePtr const &pvRequest
    );

A new method is also implemented

    static PvaClientMonitorPtr create(
        PvaClientPtr const &pvaClient,
        std::string const & channelName,
        std::string const & providerName,
        std::string const & request,
        PvaClientChannelStateChangeRequesterPtr const & stateChangeRequester,
        PvaClientMonitorRequesterPtr const & monitorRequester
    );


## Release 4.2 (EPICS V4.6, Aug 2016)

* The examples are moved to exampleCPP.
* Support for channelRPC is now available.
* In PvaClientMultiChannel checkConnected() now throws an exception if connect fails.


## Release 4.1 (EPICS V4.5, Oct 2015)

pvaClient is a synchronous API for pvAccess.

This is the first release of pvaClientCPP.
It provides an API that is similar to pvaClientJava.
