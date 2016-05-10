# arrayPerformance

This example implements main programs that demonstrate performance
of large arrays and can also be used to check for memory leaks.
Checking for memory leaks can be accomplished by running the programs with valgrind
or some other memory check program.


## Building

If a proper RELEASE.local file exists two directory levels above **arrayPerformance**,
or in **../configure/RELEASE.local** then just type:

    make

It can also be built by:

    cp configure/ExampleRELEASE.local configure/RELEASE.local
    edit file configure/RELEASE.local
    make

In **configure/RELEASE.local** it may only be necessary to change the definitions
of **EPICS4_DIR** and **EPICS_BASE**.

## More Information
 

To understand the examples read documentation/arrayPerformance.html
