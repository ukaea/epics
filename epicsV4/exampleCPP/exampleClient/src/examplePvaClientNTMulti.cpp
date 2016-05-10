// Copyright information and license terms for this software can be
// found in the file LICENSE that is included with the distribution

/*examplePvaClientNTMulti.cpp */

/**
 * @author mrk
 */

/* Author: Marty Kraimer */

#include <iostream>

#include <pv/pvaClientMultiChannel.h>
#include <pv/convert.h>

using std::tr1::static_pointer_cast;
using namespace std;
using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvaClient;

static PVDataCreatePtr pvDataCreate = getPVDataCreate();
static ConvertPtr convert = getConvert();

static void setValue(PVUnionPtr const &pvUnion, double value)
{
    UnionConstPtr u = pvUnion->getUnion();
    FieldConstPtr field = u->getField(0);
    Type type = field->getType();
    if(type==scalar) {
         ScalarConstPtr scalar = static_pointer_cast<const Scalar>(field);
         ScalarType scalarType = scalar->getScalarType();
         if(scalarType==pvDouble) {
              PVDoublePtr pvValue = static_pointer_cast<PVDouble>(
                   pvDataCreate->createPVScalar(pvDouble));
              pvValue->put(value);
              pvUnion->set(0,pvValue);
              return;
         }
         if(scalarType==pvString) {
              PVStringPtr pvValue = static_pointer_cast<PVString>(
                   pvDataCreate->createPVScalar(pvString));
              stringstream ss;
              ss << "value" << value;
              pvValue->put(ss.str());
              pvUnion->set(0,pvValue);
              return;
         }
         throw std::runtime_error("only pvDouble and pvString are supported");
    }
    if(type==scalarArray) {
         ScalarArrayConstPtr scalarArray = static_pointer_cast<const ScalarArray>(field);
         ScalarType scalarType = scalarArray->getElementType();
         if(scalarType==pvDouble) {
              size_t num = 5;
              PVDoubleArrayPtr pvValue = static_pointer_cast<PVDoubleArray>(
                   pvDataCreate->createPVScalarArray(pvDouble));
              shared_vector<double> data(num);
              for(size_t i=0; i<num; ++i) data[i] = value +i;
              pvValue->replace(freeze(data));
              pvUnion->set(0,pvValue);
              return;
         }
         if(scalarType==pvString) {
              size_t num = 5;
              PVStringArrayPtr pvValue = static_pointer_cast<PVStringArray>(
                   pvDataCreate->createPVScalarArray(pvString));
              shared_vector<string> data(num);
              for(size_t i=0; i<num; ++i) {
                  stringstream ss;
                  ss << "value" << value << i;
                  data[i] = ss.str();
              }
              pvValue->replace(freeze(data));
              pvUnion->set(0,pvValue);
              return;
         }
         throw std::runtime_error("only pvDouble and pvString are supported");
    }
    throw std::runtime_error("only scalar and scalarArray fields are supported");
}

static void example(
     PvaClientPtr const &pva,
     string provider,
     shared_vector<const string> const &channelNames)
{
    cout << "_example provider " << provider << " channels " << channelNames << "_\n";
    size_t num = channelNames.size();
    PvaClientMultiChannelPtr multiChannel(
        PvaClientMultiChannel::create(pva,channelNames,provider));
    Status status = multiChannel->connect();
    if(!status.isSuccess()) {
         cout << "Did not connect: ";
         shared_vector<epics::pvData::boolean> isConnected = multiChannel->getIsConnected();
         for(size_t i=0; i<num; ++i) {
             if(!isConnected[i]) cout << channelNames[i] << " ";
         }
         cout << endl;
         multiChannel->destroy();
         return;
    }
    PvaClientNTMultiGetPtr multiGet(multiChannel->createNTGet());
    PvaClientNTMultiPutPtr multiPut(multiChannel->createNTPut());
    PvaClientNTMultiMonitorPtr multiMonitor(multiChannel->createNTMonitor());
    shared_vector<epics::pvData::PVUnionPtr> data = multiPut->getValues();
    for(double value = 0.0; value< 2.1; value+= 1.0) {
        for(size_t i=0; i<num ; ++i) {
             PVUnionPtr pvUnion = data[i];
             setValue(pvUnion,value);
        }
        multiPut->put();
        multiGet->get();
        PvaClientNTMultiDataPtr multiData = multiGet->getData();
        PVStructurePtr pvStructure = multiData->getNTMultiChannel()->getPVStructure();
        cout << "pvStructure\n" << pvStructure << endl;
        bool result = multiMonitor->waitEvent(.1);
        while(result) {
            multiData = multiMonitor->getData();
            pvStructure = multiData->getNTMultiChannel()->getPVStructure();
            cout << "monitor pvStructure\n" << pvStructure << endl;
            result = multiMonitor->poll();
        }
    }
}

int main(int argc,char *argv[])
{
    cout << "_____examplePvaClientNTMulti starting_______\n";
    PvaClientPtr pva = PvaClient::get("pva ca");
    size_t num = 4;
    shared_vector<string> channelNames(num);
    channelNames[0] = "PVRdouble";
    channelNames[1] = "PVRstring";
    channelNames[2] = "PVRdoubleArray";
    channelNames[3] = "PVRstringArray";
    shared_vector<const string> names(freeze(channelNames));
    example(pva,"pva",names);
    PvaClientChannelPtr pvaChannel = pva->createChannel("DBRdouble00","ca");
    pvaChannel->issueConnect();
    Status status = pvaChannel->waitConnect(1.0);
    if(status.isOK()) {
        channelNames = shared_vector<string>(num);
        channelNames[0] = "DBRdouble01";
        channelNames[1] = "DBRstring01";
        channelNames[2] = "DBRdoubleArray01";
        channelNames[3] = "DBRstringArray01";
        names = freeze(channelNames);
        example(pva,"pva",names);
        example(pva,"ca",names);
    } else {
         cout << "DBRdouble00 not found\n";
    }
    cout << "_____examplePvaClientNTMulti done_______\n";
    return 0;
}
