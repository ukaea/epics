// Copyright information and license terms for this software can be
// found in the file LICENSE that is included with the distribution

/*examplePvaClientPut.cpp */

/**
 * @author mrk
 */

/* Author: Marty Kraimer */

#include <iostream>

#include <pv/pvaClient.h>
#include <pv/convert.h>

using std::tr1::static_pointer_cast;
using namespace std;
using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvaClient;

static ConvertPtr convert = getConvert();


static void exampleDouble(PvaClientPtr const &pva,string const & channelName,string const & providerName)
{
    cout << "__exampleDouble__ channelName " << channelName << " providerName " << providerName << endl;
    PvaClientChannelPtr channel = pva->channel(channelName,providerName,2.0);
    PvaClientPutPtr put = channel->put();
    PvaClientPutDataPtr putData = put->getData();
    PvaClientMonitorPtr monitor = pva->channel(channelName,providerName,2.0)->monitor("value");
    PvaClientMonitorDataPtr monitorData = monitor->getData();
    putData->putDouble(3.0); put->put();
    cout <<  channel->get("field(value)")->getData()->showChanged(cout) << endl;
    putData->putDouble(4.0); put->put();
    cout <<  channel->get("field(value)")->getData()->showChanged(cout) << endl;
    if(!monitor->waitEvent()) {
           cout << "waitEvent returned false. Why???";
    } else while(true) {
         cout << "monitor changed\n" << monitorData->showChanged(cout);
         monitor->releaseEvent();
         if(!monitor->poll()) break;
    }
}

static void exampleDoubleArray(PvaClientPtr const &pva,string const & channelName,string const & providerName)
{
    cout << "__exampleDoubleArray__ channelName " << channelName << " providerName " << providerName << endl;
    PvaClientChannelPtr channel = pva->channel(channelName,providerName,2.0);
    PvaClientPutPtr put = channel->put();
    PvaClientPutDataPtr putData = put->getData();
    PvaClientMonitorPtr monitor = pva->channel(channelName,providerName,2.0)->monitor("value");
    PvaClientMonitorDataPtr monitorData = monitor->getData();
    size_t num = 5;
    shared_vector<double> data(num,0);
    for(size_t i=0; i<num; ++i) data[i] = .1*i;
    putData->putDoubleArray(freeze(data)); put->put();
    cout <<  channel->get("field(value)")->getData()->showChanged(cout) << endl;
    data = shared_vector<double>(num,0);
    for(size_t i=0; i<num; ++i) data[i] = .1*(i + 1);
    putData->putDoubleArray(freeze(data)); put->put();
    cout <<  channel->get("field(value)")->getData()->showChanged(cout) << endl;
    if(!monitor->waitEvent()) {
           cout << "waitEvent returned false. Why???";
    } else while(true) {
         cout << "monitor changed\n" << monitorData->showChanged(cout);
         monitor->releaseEvent();
         if(!monitor->poll()) break;
    }
}


static void examplePVFieldPut(PvaClientPtr const &pva,string const & channelName,string const & providerName)
{
    cout << "__examplePVFieldPut__ channelName " << channelName << " providerName " << providerName << endl;
    PvaClientChannelPtr channel = pva->channel(channelName,providerName,2.0);
    PvaClientPutPtr put = channel->put();
    PvaClientPutDataPtr putData = put->getData();
    PVScalarPtr pvScalar = static_pointer_cast<PVScalar>(putData->getValue());
    convert->fromDouble(pvScalar,1.0); put->put();
    cout <<  channel->get("field(value)")->getData()->showChanged(cout) << endl;
    convert->fromDouble(pvScalar,2.0); put->put();
    cout <<  channel->get("field(value)")->getData()->showChanged(cout) << endl;
}


int main(int argc,char *argv[])
{
    cout << "_____examplePvaClientPut starting_______\n";
    PvaClientPtr pva = PvaClient::get("pva ca");
    try {
        exampleDouble(pva,"PVRdouble","pva");
        exampleDoubleArray(pva,"PVRdoubleArray","pva");
        examplePVFieldPut(pva,"PVRint","pva");
        PvaClientChannelPtr pvaChannel = pva->createChannel("DBRdouble00","ca");
        pvaChannel->issueConnect();
        Status status = pvaChannel->waitConnect(1.0);
        pvaChannel->destroy();
        if(status.isOK()) {
            exampleDouble(pva,"DBRdouble00","pva");
            exampleDouble(pva,"DBRdouble00","ca");
            exampleDoubleArray(pva,"DBRdoubleArray","pva");
            exampleDoubleArray(pva,"DBRdoubleArray","ca");
            examplePVFieldPut(pva,"DBRint00","pva");
            examplePVFieldPut(pva,"DBRint00","ca");
        } else {
             cout << "DBRdouble00 not found\n";
        }
        cout << "_____examplePvaClientPut done_______\n";
    } catch (std::runtime_error e) {
        cerr << "exception " << e.what() << endl;
        return 1;
    }
    return 0;
}
