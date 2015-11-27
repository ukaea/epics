/*powerSupplyMain.cpp */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvData is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/**
 * @author mrk
 */

/* Author: Marty Kraimer */

#include <string>
#include <iostream>

#include <pv/powerSupply.h>
#include <pv/traceRecord.h>
#include <pv/channelProviderLocal.h>

using namespace std;
using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvDatabase;


int main(int argc,char *argv[])
{
    PVDatabasePtr master = PVDatabase::getMaster();

    PVRecordPtr pvRecord;
    bool result = false;
    string recordName;

    recordName = "powerSupply";
    PVStructurePtr pv = createPowerSupply();
    pvRecord = PowerSupply::create(recordName,pv);
    result = master->addRecord(pvRecord);
    cout << "result of addRecord " << recordName << " " << result << endl;

    recordName = "traceRecordPGRPC";
    pvRecord = TraceRecord::create(recordName);
    result = master->addRecord(pvRecord);
    if (!result) cout<< "record " << recordName << " not added" << endl;

    ContextLocal::shared_pointer contextLocal = ContextLocal::create();
    contextLocal->start(true);

    return 0;
}

