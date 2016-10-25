/*
 * Copyright information and license terms for this software can be
 * found in the file LICENSE that is included with the distribution
 */

/**
 * @author mrk
 */

/* Author: Marty Kraimer */

#include <iostream>
#include <epicsThread.h>
#include <pv/pvData.h>
#include <pv/pvAccess.h>
#include <pv/rpcClient.h>
#include <pv/pvaClient.h>

using namespace std;
using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvaClient;

class ClientRPCRequester :
   public PvaClientRPCRequester
{
private:
     bool requestDoneCalled;
public:
    POINTER_DEFINITIONS(ClientRPCRequester);

    ClientRPCRequester() : requestDoneCalled(false){}
    virtual void requestDone(
        const Status& status,
        PvaClientRPCPtr const & pvaClientRPC,
        PVStructurePtr const & pvResponse)
   {
       cout << "response\n" << pvResponse << endl;
       requestDoneCalled = true;
   }
   void waitResponse()
   {
       while(true)
       {
            if(requestDoneCalled) {
                requestDoneCalled = false;
                return;
            }
            epicsThreadSleep(.1);
       }
   }
};

static FieldCreatePtr fieldCreate = getFieldCreate();
static PVDataCreatePtr pvDataCreate = getPVDataCreate();


static void exampleSimple(PvaClientPtr const &pva,string const & channelName)
{
    cout << "_____exampleSimple___\n";
    StructureConstPtr  topStructure = fieldCreate->createFieldBuilder()->
        add("value",pvString)->
        createStructure();
    PVStructurePtr pvRequest = pvDataCreate->createPVStructure(topStructure);
    PVStringPtr pvArgument = pvRequest->getSubField<PVString>("value");
    pvArgument->put("World");
    cout << "send " << pvArgument->get() << endl;
    PVStructurePtr pvResult = pva->channel(channelName)->rpc(pvRequest);
    cout << "result\n" << pvResult << endl;
    
}

static void exampleMore(PvaClientPtr const &pva,string const & channelName)
{
    cout << "_____exampleMore___\n";
    StructureConstPtr  topStructure = fieldCreate->createFieldBuilder()->
        add("value",pvString)->
        createStructure();
    PVStructurePtr pvRequest = pvDataCreate->createPVStructure(topStructure);
    PVStringPtr pvArgument = pvRequest->getSubField<PVString>("value");
    PvaClientRPCPtr rpc(pva->channel(channelName)->createRPC());
    pvArgument->put("World");
    cout << "send " << pvArgument->get() << endl;
    PVStructurePtr pvResult = rpc->request(pvRequest);
    cout << "result\n" << pvResult << endl;
    pvArgument->put("Again");
    cout << "send " << pvArgument->get() << endl;
    pvResult = rpc->request(pvRequest);
    cout << "result\n" << pvResult << endl;
}

static void exampleEvenMore(PvaClientPtr const &pva,string const & channelName)
{
    cout << "_____exampleEvenMore___\n";
    StructureConstPtr  topStructure = fieldCreate->createFieldBuilder()->
        add("value",pvString)->
        createStructure();
    PVStructurePtr pvRequest = pvDataCreate->createPVStructure(topStructure);
    PVStringPtr pvArgument = pvRequest->getSubField<PVString>("value");
    PvaClientChannelPtr pvaChannel = pva->createChannel(channelName);
    pvaChannel->issueConnect();
    Status status = pvaChannel->waitConnect(2.0);
    if(!status.isOK()) {cout << " connect failed\n"; return;}
    ClientRPCRequester::shared_pointer requester(new ClientRPCRequester());
    PvaClientRPCPtr rpc(pvaChannel->createRPC());
    rpc->issueConnect();
    status = rpc->waitConnect();
    if(!status.isOK()) {cout << " rpc connect failed\n"; return;}
    cout << "example channeRPC more\n";
    pvArgument->put("World");
    cout << "send " << pvArgument->get() << endl;
    rpc->request(pvRequest,requester);
    requester->waitResponse();
    pvArgument->put("Again");
    cout << "send " << pvArgument->get() << endl;
    rpc->request(pvRequest,requester);
    requester->waitResponse();
    rpc->setResponseTimeout(.001);
    pvArgument->put("Once again");
    cout << "send " << pvArgument->get() << endl;
    try {
        rpc->request(pvRequest, requester);
        requester->waitResponse();
    } catch (std::runtime_error e) {
          cerr << "Expected exception " << e.what() << endl;  
    }
    try {
        rpc->request(pvRequest, requester);
        rpc->request(pvRequest, requester);
    } catch (std::runtime_error e) {
          cerr << "Expected exception " << e.what() << endl;  
    }
}


int main(int argc,char *argv[])
{
    cout << "_____HelloWorldRPC starting_______\n";
    try {
        PvaClientPtr pva= PvaClient::get("pva");
//PvaClient::setDebug(true);
        string channelName("PVRhelloRPC");
        exampleSimple(pva,channelName);
        exampleMore(pva,channelName);
        exampleEvenMore(pva,channelName);
        cout << "_____HelloWorldRPC done_______\n";
    } catch (std::runtime_error e) {
        cerr << "exception " << e.what() << endl;
        return 1;
    }
    return 0;
}
