// Copyright information and license terms for this software can be
// found in the file LICENSE that is included with the distribution

/*helloWorldRPC.cpp */

/**
 * @author mrk
 */

/* Author: Marty Kraimer */

#include <iostream>
#include <pv/pvData.h>
#include <pv/pvAccess.h>
#include <pv/rpcClient.h>

using namespace std;
using namespace epics::pvData;
using namespace epics::pvAccess;

static FieldCreatePtr fieldCreate = getFieldCreate();
static PVDataCreatePtr pvDataCreate = getPVDataCreate();


static void exampleSimple()
{
    StructureConstPtr  topStructure = fieldCreate->createFieldBuilder()->
        add("value",pvString)->
        createStructure();
    PVStructurePtr pvRequest = pvDataCreate->createPVStructure(topStructure);
    PVStringPtr pvArgument = pvRequest->getSubField<PVString>("value");
    pvArgument->put("World");
    cout << "example channeRPC simple\n";
    try {
        PVStructurePtr pvResult =
            RPCClient::sendRequest("helloRPC", pvRequest);
        cout << "result\n" << pvResult << endl;
    } catch (RPCRequestException &e) {
        cout << e.what() << endl;
    }
}

static void exampleMore()
{
    StructureConstPtr  topStructure = fieldCreate->createFieldBuilder()->
        add("value",pvString)->
        createStructure();
    PVStructurePtr pvRequest = pvDataCreate->createPVStructure(topStructure);
    PVStringPtr pvArgument = pvRequest->getSubField<PVString>("value");
    pvArgument->put("World");
    cout << "example channeRPC more\n";
    try {
        RPCClient::shared_pointer client = RPCClient::create("helloRPC");
        client->issueConnect();
        if (client->waitConnect())
        {
            client->issueRequest(pvRequest);
            PVStructure::shared_pointer result = client->waitResponse();
            std::cout << *result << std::endl;
        }
        else {
            cout << "waitConnect timeout\n";
            return;
        }
    } catch (RPCRequestException &e) {
        cout << e.what() << endl;
    }
}


int main(int argc,char *argv[])
{
    exampleSimple();
    exampleMore();
    return 0;
}
