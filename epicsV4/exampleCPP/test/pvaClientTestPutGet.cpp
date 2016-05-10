// Copyright information and license terms for this software can be
// found in the file LICENSE that is included with the distribution

/*pvaClientTestPutGet.cpp */

/**
 * @author mrk
 */

/* Author: Marty Kraimer */

#include <iostream>

#include <pv/pvaClient.h>
#include <epicsUnitTest.h>
#include <testMain.h>

using namespace std;
using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvaClient;


static void example(PvaClientPtr const &pvaClient)
{
    testDiag("== example ==");

    PvaClientChannelPtr pvaChannel;
    try {
        pvaChannel = pvaClient->createChannel("PVRhelloPutGet");
        pvaChannel->connect(2.0);
        testDiag("channel connected");
    } catch (std::runtime_error e) {
        testAbort("channel connection exception '%s'", e.what());
    }

    PvaClientPutGetPtr putGet;
    PvaClientPutDataPtr putData;
    PvaClientGetDataPtr getData;
    try {
        putGet = pvaChannel->createPutGet();
        testDiag("putGet created");
        putData = putGet->getPutData();
        getData = putGet->getGetData();
        if (!putData || !getData)
            testAbort("NULL data pointer from putGet");
    } catch (std::runtime_error e) {
        testAbort("putGet connection exception '%s'", e.what());
    }

    PVStructurePtr pvStructure;
    PVStringPtr putValue;
    try {
        pvStructure = putData->getPVStructure();
        putValue = pvStructure->getSubField<PVString>("argument.value");
        testOk(!!putValue, "putField value exists");
    } catch (std::runtime_error e) {
        testAbort("structure connection exception '%s'", e.what());
    }

    try {
        testDiag("Setting value to Marty");
        putValue->put("Marty");
        putGet->putGet();
    } catch (std::runtime_error e) {
        testAbort("putGet exception '%s'", e.what());
    }

    PVStringPtr gotValue;
    try {
        pvStructure = getData->getPVStructure();
        gotValue = pvStructure->getSubField<PVString>("result.value");
        testOk(gotValue && gotValue->get() == std::string("Hello Marty"), "returned correct value");

        testDiag("Setting Value to World");
        putValue->put("World");
        putGet->putGet();

        pvStructure = getData->getPVStructure();
        gotValue = pvStructure->getSubField<PVString>("result.value");
        testOk(gotValue && gotValue->get() == std::string("Hello World"), "returned correct value");
    } catch (std::runtime_error e) {
        testAbort("exception '%s'", e.what());
    }
}


MAIN(pvaClientTestPutGet)
{
    testPlan(3);
    testDiag("=== pvaClientTestPutGet ===");

    PvaClientPtr pvaClient = PvaClient::get("pva");
    example(pvaClient);

    testDone();
    return 0;
}
