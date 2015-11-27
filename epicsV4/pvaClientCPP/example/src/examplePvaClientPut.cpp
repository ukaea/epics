/*examplePvaClientPut.cpp */
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
#include <pv/convert.h>

using std::tr1::static_pointer_cast;
using namespace std;
using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvaClient;

static ConvertPtr convert = getConvert();


static void examplePut(PvaClientPtr const &pva)
{
    cout << "example put\n";
    PvaClientChannelPtr channel = pva->channel("exampleDouble");
    PvaClientPutPtr put = channel->put();
    PvaClientPutDataPtr putData = put->getData();
    PvaClientMonitorPtr monitor = pva->channel("exampleDouble")->monitor("");
    PvaClientMonitorDataPtr monitorData = monitor->getData();
    try {
        putData->putDouble(3.0); put->put();
        cout <<  channel->get("field()")->getData()->showChanged(cout) << endl;
        putData->putDouble(4.0); put->put();
        cout <<  channel->get("field()")->getData()->showChanged(cout) << endl;
        if(!monitor->waitEvent()) {
               cout << "waitEvent returned false. Why???";
        } else while(true) {
             cout << "monitor changed\n" << monitorData->showChanged(cout);
             monitor->releaseEvent();
             if(!monitor->poll()) break;
        }
    } catch (std::runtime_error e) {
        cout << "exception " << e.what() << endl;
    }
}

static void examplePVFieldPut(PvaClientPtr const &pva)
{
    cout << "example put\n";
    PvaClientChannelPtr channel = pva->channel("exampleDouble");
    PvaClientPutPtr put = channel->put();
    PvaClientPutDataPtr putData = put->getData();
    PVFieldPtr pvField = putData->getValue();
    PVScalarPtr pvScalar = static_pointer_cast<PVScalar>(pvField);
    try {
        convert->fromDouble(pvScalar,1.0); put->put();
        cout <<  channel->get("field()")->getData()->showChanged(cout) << endl;
        convert->fromDouble(pvScalar,2.0); put->put();
        cout <<  channel->get("field()")->getData()->showChanged(cout) << endl;
    } catch (std::runtime_error e) {
        cout << "exception " << e.what() << endl;
    }
}


int main(int argc,char *argv[])
{
    PvaClientPtr pva = PvaClient::create();
    examplePut(pva);
    examplePVFieldPut(pva);
    return 0;
}
