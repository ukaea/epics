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
#include <pv/convert.h>

using namespace std;
using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvaClient;

static PVDataCreatePtr pvDataCreate = getPVDataCreate();
static ConvertPtr convert = getConvert();


class  ChannelStateChangeRequester :
     public PvaClientChannelStateChangeRequester
{
    bool connected;
public:
    POINTER_DEFINITIONS(ChannelStateChangeRequester);
    ChannelStateChangeRequester()
    : connected(false)
     {}
    virtual void channelStateChange(PvaClientChannelPtr const & channel, bool isConnected )
    {
        cout << "channelStateChange state " << (isConnected ? "true" : "false") << endl; 
        connected = isConnected;
    }
    bool isConnected() {return connected;}
};

vector<string> split(string const & blankSeparatedList) {
        string::size_type numValues = 1;
        string::size_type index=0;
        while(true) {
            string::size_type pos = blankSeparatedList.find(' ',index);
            if(pos==string::npos) break;
            numValues++;
            index = pos +1;
	}
        vector<string> valueList(numValues,"");
        index=0;
        for(size_t i=0; i<numValues; i++) {
            size_t pos = blankSeparatedList.find(' ',index);
            string value = blankSeparatedList.substr(index,pos-index);
            valueList[i] = value;
            index = pos +1;
        }
        return valueList;
    }

int main(int argc,char *argv[])
{
    string provider("pva");
    string channelName("PVRrestrictedUnion");
    string request("value");
    bool debug(false);
    if(argc==2 && string(argv[1])==string("-help")) {
        cout << "channelName request debug" << endl;
        cout << "default" << endl;
        cout <<  channelName << " " 
             << " " << '"' << request << '"'
             << " debug " << (debug ? "true" : "false") << endl;
        return 0;
    }
    if(argc>1) channelName = argv[1];
    if(argc>2) request = argv[2];
    if(argc>3) {
        string value(argv[3]);
        if(value=="true") debug = true;
    }
    cout << " channelName " <<  channelName 
         << " request " << request
         << " debug " << (debug ? "true" : "false") 
         << endl;
    cout << "_____PutUnionForever starting__\n";
    try {
        PvaClientPtr pva= PvaClient::get(provider);
        if(debug) PvaClient::setDebug(true);
        PvaClientChannelPtr channel(pva->channel(channelName,provider,0.0));
        ChannelStateChangeRequester::shared_pointer stateChangeRequester(
            new ChannelStateChangeRequester());
        channel->setStateChangeRequester(stateChangeRequester);
        PvaClientPutPtr pvaClientPut = channel->put(request);
        PvaClientPutDataPtr putData = pvaClientPut->getData();
        PVFieldPtr pvField = putData->getPVStructure()->getPVFields()[0];
        if(!pvField) throw std::runtime_error("no value field");
        if(pvField->getField()->getType()!=union_) throw std::runtime_error("value is not a PVUnion");
        PVUnionPtr pvUnion = std::tr1::static_pointer_cast<PVUnion>(pvField);
        UnionConstPtr u = pvUnion->getUnion();
        if(!u->isVariant()) {
            FieldConstPtr field = u->getField("string");
            if(!field) throw std::runtime_error("union does not have a field named string");
            if(field->getType()!=scalar) throw std::runtime_error("union field string is not a scalar");
            ScalarConstPtr scalar = std::tr1::static_pointer_cast<const Scalar>(field);
            if(scalar->getScalarType()!=pvString) {
                 throw std::runtime_error("union field string does not have type string");
            }
            field = u->getField("stringArray");
            if(!field) throw std::runtime_error("union does not have a field named stringArray");
            if(field->getType()!=scalarArray) {
                throw std::runtime_error("union field stringArray is not a scalarArray");
            }
            ScalarArrayConstPtr scalarArray = std::tr1::static_pointer_cast<const ScalarArray>(field);
            if(scalarArray->getElementType()!=pvString) {
                throw std::runtime_error("union field stringArray does not have elementType string");
            }
        }
        string value("firstPut");
        while(true) {
            if(stateChangeRequester->isConnected()) {
               cout << "value " << value << endl;
               vector<string> items = split(value);
               int nitems = items.size();
               bool isArray = (nitems==1) ? false : true;
               if(isArray) {
                   if(u->isVariant()) {
                       PVStringArrayPtr pvStringArray = pvDataCreate->createPVScalarArray<PVStringArray>();
                       convert->fromStringArray(pvStringArray,0,nitems,items,0);
                       pvUnion->set(pvStringArray);
                   } else {
                        PVStringArrayPtr pvStringArray = pvUnion->select<PVStringArray>("stringArray");
                        convert->fromStringArray(pvStringArray,0,nitems,items,0);
                   }
               } else {
                    if(u->isVariant()) {
                        PVStringPtr pvString = pvDataCreate->createPVScalar<PVString>();
                        pvString->put(value);
                        pvUnion->set(pvString);
                    } else {
                        PVStringPtr pvString = pvUnion->select<PVString>("string");
                        pvString->put(value);
                    }
                }
                putData->getChangedBitSet()->set(pvUnion->getFieldOffset());
                pvaClientPut->put();
            } else {
                cout <<"did not issue get because connection lost\n";
            }
            int c = std::cin.peek();  // peek character
            if ( c == EOF ) continue;
            cout << "Type exit to stop: \n";
            getline(cin,value);
            if(value.compare("exit")==0) break;
        }
    } catch (std::runtime_error e) {
        cerr << "exception " << e.what() << endl;
        return 1;
    }
    return 0;
}
