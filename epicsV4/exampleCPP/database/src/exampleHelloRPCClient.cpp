/*
 * Copyright information and license terms for this software can be
 * found in the file LICENSE that is included with the distribution
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

class ClientRPCRequester;
typedef std::tr1::shared_ptr<ClientRPCRequester> ClientRPCRequesterPtr;

class ClientRPCRequester :
   public PvaClientRPCRequester
{
public:
    virtual void requestDone(
        const epics::pvData::Status& status,
        PvaClientRPCPtr const & pvaClientRPC,
        epics::pvData::PVStructure::shared_pointer const & pvResponse)
    {
        cout << "requestDone\n" << pvResponse << endl;
        waitForResult.signal();
    }
    Event waitForResult;
};

static void exampleSimple(
     PvaClientPtr const & pva,
     string const & channelName,
     PVStructurePtr const & pvArgument)
{
    
    cout << "example channeRPC simple\n";
    PVStringPtr pvValue = pvArgument->getSubField<PVString>("value");
    pvValue->put("World");
    PvaClientChannelPtr channel = pva->channel(channelName);
    PVStructurePtr pvResult = channel->rpc(pvArgument);
    cout << "result\n" << pvResult << endl;
    pvValue->put("World Again");
    pvResult = channel->rpc(pvArgument,pvArgument);
    cout << "result again\n" << pvResult << endl;
}

static void exampleLonger(
     PvaClientPtr const & pva,
     string const & channelName,
     PVStructurePtr const & pvArgument)
{
    
    cout << "example channeRPC longer\n";
    PVStringPtr pvValue = pvArgument->getSubField<PVString>("value");
    pvValue->put("World Longer");
    PvaClientRPCPtr rpc = pva->channel(channelName)->createRPC(pvArgument);
    PVStructurePtr pvResult = rpc->request(pvArgument);
    cout << "result\n" << pvResult << endl;
    pvValue->put("World Longer Again");
    pvResult = rpc->request(pvArgument);
    cout << "result again\n" << pvResult << endl;
}

static void exampleRequester(
     PvaClientPtr const & pva,
     string const & channelName,
     PVStructurePtr const & pvArgument)
{
    
    cout << "example channeRPC requester\n";
    ClientRPCRequesterPtr requester(new ClientRPCRequester());
    PVStringPtr pvValue = pvArgument->getSubField<PVString>("value");
    pvValue->put("World Requester");
    PvaClientRPCPtr rpc = pva->channel(channelName)->createRPC(pvArgument);
    rpc->request(pvArgument,requester);
    requester->waitForResult.wait();
    pvValue->put("World Requester Again");
    rpc->request(pvArgument,requester);
    requester->waitForResult.wait();
}


int main(int argc,char *argv[])
{
    string channelName("PVRhelloRPC");
    bool debug(false);
    if(argc==2 && string(argv[1])==string("-help")) {
        cout << "channelName debug" << endl;
        cout << "default" << endl;
        cout <<  channelName
             << (debug ? "true" : "false") << endl;
        return 0;
    }
    if(argc>1) channelName = argv[1];
    if(argc>2) {
        string value(argv[2]);
        if(value=="true") debug = true;
    }
    cout << " channelName " <<  channelName
         << " debug " << (debug ? "true" : "false") << endl;

    FieldCreatePtr fieldCreate = getFieldCreate();
    PVDataCreatePtr pvDataCreate = getPVDataCreate();
    StructureConstPtr  topStructure = fieldCreate->createFieldBuilder()->
        add("value",pvString)->
        createStructure();
    PVStructurePtr pvArgument(pvDataCreate->createPVStructure(topStructure));
    PVStringPtr pvValue = pvArgument->getSubField<PVString>("value");
    cout << "_____exampleHelloRPCClient starting_______\n";
    try {
        PvaClientPtr pva = PvaClient::get("pva");
        if(debug) PvaClient::setDebug(true);
        exampleSimple(pva,channelName,pvArgument);
        exampleLonger(pva,channelName,pvArgument);
        exampleRequester(pva,channelName,pvArgument);
    } catch (std::runtime_error e) {
        cerr << "exception " << e.what() << endl;
        return 1;
    }
    return 0;
}
