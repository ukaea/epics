exampleCPP
----------

exampleCPP is a small collection of examples of EPICS V4 servers and clients.

Presently, exampleCPP contains examples only of RPC style middleware - making
a request with arguments to a server over pvAccess, and getting a synchronous reply.

There are 2 examples:

1. HelloWorld

HelloWorld is the classic hello world idea, implemented as a server and a client.
The client sends an argument "your name" to the server, which replies just 
saying "hello <that name>". The server is built on top of the RPCSever framework 
in pvAccessCPP.

Start with its HELLOWORLD_README.txt. 


2. ChannelArchiver Service 

The ChannelArchiver Service is an EPICS V4 client and server which can be used
to query the data archived by an EPICS Channel Archiver. It includes a bash
script, gethist, which can parse a wide range of dates and times and process
the corresponding data. However the service can be queried by any client
issuing a pvAccess RPC request, such as eget. Requests and responses use EPICS V4
normative types. Requests are in the form of an NTURI normative type and the results are returned as an NTTable. 

Start with its CHANNEL_ARCHIVER_SERVICE__README.txt


