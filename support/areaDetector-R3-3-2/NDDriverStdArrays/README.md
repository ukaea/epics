NDDriverStdArrays
=================
An 
[EPICS](http://www.aps.anl.gov/epics)
[areaDetector](http://cars.uchicago.edu/software/epics/areaDetector.html)
driver that allows any EPICS Channel Access client to create NDArrays in 
an areaDetector IOC.
It is the logical inverse of the NDPluginStdArrays plugin which converts
NDArrays in an IOC into standard EPICS waveform records for use by
Channel Access clients. The NDPluginStdArrays plugin also writes
to additional records to describe the array structure.

The NDDriverStdArrays driver receives EPICS waveform records from Channel Access
clients and converts them to an NDArray in the IOC.
The clients must also write to additional EPICS records to define the structure
of the NDArray.

It is similar in concept to the pvaDriver which receives EPICS V4 NTNDArrays
over the network and converts them to NDArrays in the IOC.
pvaDriver only works with EPICS V4 PVAccess clients, while NDDriverStdArrays
works with EPICS V3 Channel Access clients.

Additional information:
* [Documentation](http://cars.uchicago.edu/software/epics/NDDriverStdArraysDoc.html).
* [Release notes and links to source and binary releases](RELEASE.md).
