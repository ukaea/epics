# Known Problems

## monitors

When a monitor is issued to a channel that is served by dbPv (pvaSrv),
Each monitors shows that all requested fields have changed.

## helloRPC

helloService does not terminate.

## exampleLink

Run as:

    mrk> pwd
    /home/epicsv4/master/exampleCPP/exampleLink/iocBoot/exampleLink
    mrk> ../../bin/linux-x86_64/exampleLink st.ca

When exit is requested a Segmentation fault occurs.

Note that this does not happen when run as:

    mrk> pwd
    /home/epicsv4/master/exampleCPP/exampleLink
    mrk> bin/linux-x86_64/exampleLinkMain  ca exampleLink doubleArray false

## powerSupply

The powerSupplyClient ends with setting voltage to 0.
This causes the PVRecord to throw an exception, which pvAccess passes back to the client.
All appears OK except that if another client has a monitor on the PVRecord,
that client no longer gets an monitor updates.

## memory leaks

When any of the examples terminate valgrind shows memory leaks.
Clients not so bad but servers not so good.



