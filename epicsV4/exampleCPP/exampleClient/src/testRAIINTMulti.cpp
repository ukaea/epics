/*
 * Copyright information and license terms for this software can be
 * found in the file LICENSE that is included with the distribution
 */

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
    string provider("pva");
    size_t nelements(4);
    size_t ntimes(1);
    bool debug(false);
    if(argc==2 && string(argv[1])==string("-help")) {
        cout << "provider nelements ntimes debug" << endl;
        cout << "default" << endl;
        cout << provider << " " <<  nelements << " " << ntimes  << " " << (debug ? "true" : "false") << endl;
        return 0;
    }
    if(argc>1) provider = argv[1];
    if(argc>2) nelements = strtoul(argv[2],0,0);
    if(argc>3) ntimes = strtoul(argv[3],0,0);
    if(argc>4) {
        string value(argv[4]);
        if(value=="true") debug = true;
    }
    if(nelements<1) nelements = 1;
    if(nelements>4) nelements = 4;
    bool pvaSrv((provider.find("pva")==string::npos ? false : true));
    bool caSrv((provider.find("ca")==string::npos ? false : true));
    cout << "provider \"" << provider << "\""
         << " pvaSrv " << (pvaSrv ? "true" : "false")
         << " caSrv " << (caSrv ? "true" : "false")
         << " nelements " <<  nelements
         << " ntimes " << ntimes
         << " debug " << (debug ? "true" : "false") << endl;
    cout << "_____testRAIINTMulti starting_______\n";
    try {
        PvaClientPtr pva = PvaClient::get(provider);
        if(debug) PvaClient::setDebug(true);
        size_t num = nelements;
        shared_vector<string> channelNames(num);
        channelNames[0] = "DBRdouble01";
        if(nelements>1) channelNames[1] = "DBRstring01";
        if(nelements>2) channelNames[2] = "DBRdoubleArray01";
        if(nelements>3)channelNames[3] = "DBRstringArray01";
        shared_vector<const string> names(freeze(channelNames));
        if(pvaSrv) {
            for(size_t i=0; i<ntimes ; ++i) {
                example(pva,"pva",names);
            }
        }
        if(caSrv) {
            for(size_t i=0; i<ntimes ; ++i) {
                example(pva,"ca",names);
            }
        }
        cout << "_____testRAIINTMulti done_______\n";
    } catch (std::runtime_error e) {
        cout << "exception " << e.what() << endl;
        return 1;
    }
    return 0;
}
