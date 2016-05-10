/*
 * Copyright information and license terms for this software can be
 * found in the file LICENSE that is included with the distribution
 */

/*
 * Run pvaClient tests as a batch.
 *
 * Do *not* include performance measurements here, they don't help to
 * prove functionality (which is the point of this convenience routine).
 */

#include <stdio.h>
#include <epicsExit.h>
#include <epicsThread.h>
#include <epicsUnitTest.h>

int pvaClientTest(void);
int pvaClientTestGetData(void);
int pvaClientTestPutData(void);
int pvaClientTestMonitorData(void);
int pvaClientTestPutGetMonitor(void);
int pvaClientTestPutGet(void);

void pvaClientAllTests(void)
{
    testHarness();

    /* Start the IOC here ... */

    runTest(pvaClientTest);
    runTest(pvaClientTestGetData);
    runTest(pvaClientTestPutData);
    runTest(pvaClientTestMonitorData);
    runTest(pvaClientTestPutGetMonitor);
    runTest(pvaClientTestPutGet);

    epicsExit(0);
}

