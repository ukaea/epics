/*
 * Run pvDatabase tests as a batch.
 *
 * Do *not* include performance measurements here, they don't help to
 * prove functionality (which is the point of this convenience routine).
 */

#include <stdio.h>
#include <epicsThread.h>
#include <epicsUnitTest.h>

/* src */
int testExampleRecord(void);
int testPVCopy(void);
int testPVRecord(void);

void pvDatabaseAllTests(void)
{
    testHarness();

    /* src */
    runTest(testExampleRecord);
    runTest(testPVCopy);
    runTest(testPVRecord);
}

