/*helloWorldPutGet.cpp */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvData is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/**
 * @author mrk
 */

/* Author: Marty Kraimer */

#include <iostream>

#include <pv/pvaClient.h>

using namespace std;
using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvaClient;


static void example(PvaClientPtr const &pva)
{
    cout << "helloWorldPutGet\n";
    try {
        PvaClientChannelPtr channel = pva->channel("exampleHello");
        PvaClientPutGetPtr putGet = channel->createPutGet();
        putGet->connect();
        PvaClientPutDataPtr putData = putGet->getPutData();
        PVStructurePtr arg = putData->getPVStructure();
        PVStringPtr pvValue = arg->getSubField<PVString>("argument.value");
        pvValue->put("World");
        putGet->putGet();
        PvaClientGetDataPtr getData = putGet->getGetData();
        cout << getData->getPVStructure() << endl;
    } catch (std::runtime_error e) {
        cout << "exception " << e.what() << endl;
    }
}


int main(int argc,char *argv[])
{
    PvaClientPtr pva = PvaClient::create();
    example(pva);
    return 0;
}
