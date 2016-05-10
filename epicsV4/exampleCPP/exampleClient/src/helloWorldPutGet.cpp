// Copyright information and license terms for this software can be
// found in the file LICENSE that is included with the distribution

/*helloWorldPutGet.cpp */

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
    PvaClientChannelPtr channel = pva->channel("PVRhelloPutGet");
    PvaClientPutGetPtr putGet = channel->createPutGet();
    putGet->connect();
    PvaClientPutDataPtr putData = putGet->getPutData();
    PVStructurePtr arg = putData->getPVStructure();
    PVStringPtr pvValue = arg->getSubField<PVString>("argument.value");
    pvValue->put("World");
    putGet->putGet();
    PvaClientGetDataPtr getData = putGet->getGetData();
    cout << getData->getPVStructure() << endl;
}


int main(int argc,char *argv[])
{
    PvaClientPtr pva = PvaClient::get("pva");
    try {
        example(pva);
    } catch (std::runtime_error e) {
        cerr << "exception " << e.what() << endl;
        return 1;
    }
    return 0;
}
