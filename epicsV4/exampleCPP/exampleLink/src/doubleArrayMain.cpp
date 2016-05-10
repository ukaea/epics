// Copyright information and license terms for this software can be
// found in the file LICENSE that is included with the distribution

/*doubleArrayMain.cpp */
/**
 * @author mrk
 */

/* Author: Marty Kraimer */

#include <cstddef>
#include <cstdlib>
#include <cstddef>
#include <string>
#include <cstdio>
#include <memory>
#include <vector>
#include <iostream>

#include <pv/channelProviderLocal.h>
#include <pv/serverContext.h>
#include <pv/ntscalarArray.h>

#include <pv/pvDatabase.h>

using namespace std;
using std::tr1::static_pointer_cast;
using namespace epics::pvData;
using namespace epics::nt;
using namespace epics::pvAccess;
using namespace epics::pvDatabase;

int main(int argc,char *argv[])
{
    string doubleArrayRecordName("doubleArray");
    if(argc==2 && string(argv[1])==string("-help")) {
        cout << "doubleArrayRecordName" << endl;
        cout << "default" << endl;
        cout << doubleArrayRecordName << endl;
        return 0;
    }
    if(argc>1) doubleArrayRecordName = argv[1];
    try {
        PVDatabasePtr master = PVDatabase::getMaster();
        ChannelProviderLocalPtr channelProvider = getChannelProviderLocal();
        NTScalarArrayBuilderPtr ntScalarArrayBuilder = NTScalarArray::createBuilder();
        PVStructurePtr pvStructure = ntScalarArrayBuilder->
            value(pvDouble)->
            addAlarm()->
            addTimeStamp()->
            createPVStructure();
        master->addRecord(PVRecord::create(doubleArrayRecordName,pvStructure));
        ServerContext::shared_pointer ctx =
            startPVAServer("local",0,true,true);
        string str;
        while(true) {
            cout << "Type exit to stop: \n";
            getline(cin,str);
            if(str.compare("exit")==0) break;
        }
        ctx->destroy();
    } catch (std::runtime_error e) {
        cerr << "exception " << e.what() << endl;
        return 1;
    }
    return 0;
}
