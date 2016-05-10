// Copyright information and license terms for this software can be
// found in the file LICENSE that is included with the distribution

/*examplePvaClientGet.cpp */

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


static void exampleDouble(PvaClientPtr const &pva,string const & channelName,string const & providerName )
{
    cout << "__exampleDouble__ channelName " << channelName << " providerName " << providerName << endl;
    double value;
    cout << "short way\n";
    value =  pva->channel(channelName,providerName,2.0)->get()->getData()->getDouble();
    cout << "as double " << value << endl;
    cout << "repeat short way\n";
    value =  pva->channel(channelName,providerName,2.0)->get()->getData()->getDouble();
    
    cout << "as double " << value << endl;
    cout << "long way\n";
    PvaClientChannelPtr pvaChannel = pva->createChannel(channelName,providerName);
    pvaChannel->issueConnect();
    Status status = pvaChannel->waitConnect(2.0);
    if(!status.isOK()) {cout << " connect failed\n"; return;}
    PvaClientGetPtr pvaGet = pvaChannel->createGet();
    pvaGet->issueConnect();
    status = pvaGet->waitConnect();
    if(!status.isOK()) {cout << " createGet failed\n"; return;}
    PvaClientGetDataPtr pvaData = pvaGet->getData();
    value = pvaData->getDouble();
    cout << "as double " << value << endl;
    pvaChannel->destroy();
}

static void exampleDoubleArray(PvaClientPtr const &pva,string const & channelName,string const & providerName)
{
    cout << "__exampleDoubleArray__  channelName " << channelName << " providerName " << providerName << endl;
    shared_vector<const double> value;
    cout << "short way\n";
    value =  pva->channel(channelName,providerName,2.0)->get()->getData()->getDoubleArray();
    cout << "as doubleArray " << value << endl;
    cout << "repeat short way\n";
    value =  pva->channel(channelName,providerName,2.0)->get()->getData()->getDoubleArray();
    cout << "as doubleArray " << value << endl;
   
    cout << "long way\n";
    PvaClientChannelPtr pvaChannel = pva->createChannel(channelName,providerName);
    pvaChannel->connect(2.0);
    PvaClientGetPtr pvaGet = pvaChannel->createGet();
    PvaClientGetDataPtr pvaData = pvaGet->getData();
    value = pvaData->getDoubleArray();
    cout << "as doubleArray " << value << endl;
    pvaChannel->destroy();
}

int main(int argc,char *argv[])
{
    cout << "_____examplePvaClientGet starting_______\n";
    PvaClientPtr pva= PvaClient::get("pva ca");
    try {
        exampleDouble(pva,"PVRdouble","pva");
        exampleDoubleArray(pva,"PVRdoubleArray","pva");
        PvaClientChannelPtr pvaChannel = pva->createChannel("DBRdouble00","ca");
        pvaChannel->issueConnect();
        Status status = pvaChannel->waitConnect(1.0);
        pvaChannel->destroy();
        if(status.isOK()) {
            exampleDouble(pva,"DBRdouble00","pva");
            exampleDouble(pva,"DBRdouble00","ca");
            exampleDoubleArray(pva,"DBRdoubleArray","pva");
            exampleDoubleArray(pva,"DBRdoubleArray","ca");
        } else {
             cout << "DBRdouble00 not found\n";
        }
        cout << "_____examplePvaClientGet done_______\n";
    } catch (std::runtime_error e) {
        cerr << "exception " << e.what() << endl;
        return 1;
    }
    return 0;
}
