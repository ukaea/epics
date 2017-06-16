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

int main(int argc,char *argv[])
{
    string provider("pva");
    string channelName("DBRdouble00");
    string request("value,alarm,timeStamp");
    bool debug(false);
    if(argc==2 && string(argv[1])==string("-help")) {
        cout << "provider channelName request debug" << endl;
        cout << "default" << endl;
        cout << provider << " " <<  channelName
             << " " << '"' << request << '"'
             << " " << (debug ? "true" : "false") << endl;
        return 0;
    }
    if(argc>1) provider = argv[1];
    if(argc>2) channelName = argv[2];
    if(argc>3) request = argv[3];
    if(argc>4) {
        string value(argv[4]);
        if(value=="true") debug = true;
    }
    bool pvaSrv(((provider.find("pva")==string::npos) ? false : true));
    bool caSrv(((provider.find("ca")==string::npos) ? false : true));
    if(pvaSrv&&caSrv) {
        cerr<< "multiple providers are not allowed\n";
        return 1;
    }
    cout << "provider \"" << provider << "\""
         << " pvaSrv " << (pvaSrv ? "true" : "false")
         << " caSrv " << (caSrv ? "true" : "false")
         << " channelName " <<  channelName
         << " request " << request
         << " debug " << (debug ? "true" : "false") << endl;

    cout << "_____getForever starting__\n";
    try {
        PvaClientPtr pva= PvaClient::get(provider);
        if(debug) PvaClient::setDebug(true);
        PvaClientChannelPtr channel(pva->channel(channelName,provider,0.0));
        ChannelStateChangeRequester::shared_pointer stateChangeRequester(
            new ChannelStateChangeRequester());
        channel->setStateChangeRequester(stateChangeRequester);
        PvaClientGetPtr pvaClientGet;
        while(true) {
            if(stateChangeRequester->isConnected()) {
                if(!pvaClientGet) {
                      pvaClientGet = channel->createGet(request);
                }
                pvaClientGet->get();
                PvaClientGetDataPtr data = pvaClientGet->getData();
                BitSetPtr bitSet =  data->getChangedBitSet();
                if(bitSet->cardinality()>0) {
                    cout << "changed\n";
                    data->showChanged(cout);
                    cout << "bitSet " << *bitSet << endl;
                }
            } else {
                cout <<"did not issue get because connection lost\n";
            }
            int c = std::cin.peek();  // peek character
            if ( c == EOF ) continue;
            cout << "Type exit to stop: \n";
            string str;
            getline(cin,str);
            if(str.compare("exit")==0) break;
        }
    } catch (std::runtime_error e) {
        cerr << "exception " << e.what() << endl;
        return 1;
    }
    return 0;
}
