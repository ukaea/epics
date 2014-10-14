/*************************************************************************\
* Copyright (c) 2014 Brookhaven Science Assoc. as operator of Brookhaven
*               National Laboratory.
* EPICS BASE is distributed subject to a Software License Agreement found
* in file LICENSE that is included with this distribution.
 \*************************************************************************/

#include <string.h>

#include "dbAccess.h"

#include "dbUnitTest.h"
#include "errlog.h"

#include "waveformRecord.h"

#include "testMain.h"

void recTestIoc_registerRecordDeviceDriver(struct dbBase *);

static void testGetPutArray(void)
{
    double data[4] = {1, 2, 3, 4};
    DBADDR addr, save;
    long nreq;
    epicsInt32 *pbtr;
    waveformRecord *prec;

    testDiag("Test dbGet() and dbPut() of an array");

    testdbReadDatabase("recTestIoc.dbd", NULL, NULL);

    recTestIoc_registerRecordDeviceDriver(pdbbase);

    testdbReadDatabase("arrayOpTest.db", NULL, NULL);

    eltc(0);
    testIocInitOk();
    eltc(1);

    prec = (waveformRecord*)testdbRecordPtr("wfrec");
    if(!prec || dbNameToAddr("wfrec", &addr))
        testAbort("Failed to find record wfrec");
    memcpy(&save, &addr, sizeof(save));

    testDiag("Fetch initial value");

    dbScanLock(addr.precord);
    testOk1(prec->nord==0);

    nreq = NELEMENTS(data);
    if(dbGet(&addr, DBF_DOUBLE, &data, NULL, &nreq, NULL))
        testFail("dbGet fails");
    else {
        testOk1(nreq==0);
    }
    dbScanUnlock(addr.precord);

    testOk1(memcmp(&addr, &save, sizeof(save))==0);
    addr=save;

    testDiag("Write a new value");

    data[0] = 4.0;
    data[1] = 5.0;
    data[2] = 6.0;
    data[3] = 7.0;

    dbScanLock(addr.precord);
    testOk1(dbPut(&addr, DBF_DOUBLE, &data, NELEMENTS(data))==0);
    pbtr = prec->bptr;
    testOk1(prec->nord==4);
    testOk1(pbtr[0]==4 && pbtr[1]==5 && pbtr[2]==6 && pbtr[3]==7);
    dbScanUnlock(addr.precord);

    testOk1(memcmp(&addr, &save, sizeof(save))==0);
    addr=save;

    memset(&data, 0, sizeof(data));

    testDiag("reread the value");

    dbScanLock(addr.precord);
    nreq = NELEMENTS(data);
    if(dbGet(&addr, DBF_DOUBLE, &data, NULL, &nreq, NULL))
        testFail("dbGet fails");
    else {
        testOk1(nreq==NELEMENTS(data));
        testOk1(data[0]==4.0 && data[1]==5.0 && data[2]==6.0 && data[3]==7.0);
    }
    dbScanUnlock(addr.precord);

    testOk1(memcmp(&addr, &save, sizeof(save))==0);

    testIocShutdownOk();

    testdbCleanup();
}

MAIN(arrayOpTest)
{
    testPlan(10);
    testGetPutArray();
    return testDone();
}
