TODO
===========


recordList
----------

This is putGet support that provides a list of all the records in an IOC.
Since pvAccess implements pvlist this is no longer needed.
Remove it from pvDatabaseCPP and pvIOCCPP.
Also remove channelList from swtshell.

monitorPlugin
-------------

A debate is on-going about what semantics should be.

Must test record delete.
-------------------

Must test removing a record from the PVDatabase while a pvAccess client
is attached. Also why do both unlisten and detach exists?


create more regression tests
----------------

Currently only some simple tests exist. Most of the testing has been via the examples
