/*testExampleRecordMain.cpp */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvData is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
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
#include <iostream>

#include <epicsStdio.h>
#include <epicsMutex.h>
#include <epicsEvent.h>
#include <epicsThread.h>

#include <pv/standardField.h>
#include <pv/standardPVField.h>
#include <pv/pvData.h>
#include <pv/pvAccess.h>
#include "powerSupply.h"

#include <epicsExport.h>

using namespace std;
using std::tr1::static_pointer_cast;
using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvDatabase;




void test()
{
    StandardPVFieldPtr standardPVField = getStandardPVField();
    string properties;
    ScalarType scalarType;
    string recordName;
    properties = "alarm,timeStamp";
    scalarType = pvDouble;
    recordName = "exampleDouble";
    PVStructurePtr pvStructure;
    pvStructure = standardPVField->scalar(scalarType,properties);
    PVRecordPtr pvRecord = PVRecord::create(recordName,pvStructure);
    {
        pvRecord->lock();
        pvRecord->process();
        pvRecord->unlock();
    }
    cout << "processed exampleDouble "  << endl;
    pvRecord->destroy();
    pvRecord.reset();
    recordName = "powerSupplyExample";
    pvStructure.reset();
    PowerSupplyPtr psr;
    pvStructure = createPowerSupply();
    psr = PowerSupply::create("powerSupply",pvStructure);
    if(psr.get()==NULL) {
        cout << "PowerSupplyRecordTest::create failed" << endl;
        return;
    }
    pvStructure.reset();
    double voltage,power,current;
    {
        psr->lock();
        voltage = psr->getVoltage();
        power = psr->getPower();
        current = psr->getCurrent();
        psr->unlock();
    }
    cout << "initial ";
    cout << " voltage " << voltage ;
    cout << " power " << power;
    cout <<  " current " << current;
    cout << endl;
    voltage = 1.0;
    power = 1.0;
    cout << "before put ";
    cout << " voltage " << voltage ;
    cout << " power " << power;
    cout << endl;
    {
        psr->lock();
        psr->put(power,voltage);
        psr->process();
        psr->unlock();
    }
    {
        psr->lock();
        cout << "after put ";
        cout << " voltage " << psr->getVoltage() ;
        cout << " power " << psr->getPower();
        cout <<  " current " << psr->getCurrent();
        cout << endl;
        psr->unlock();
    }
    PVDatabasePtr pvDatabase = PVDatabase::getMaster();
    pvDatabase->addRecord(psr);
    psr.reset();
    pvDatabase->destroy();
}

int main(int argc,char *argv[])
{
    test();
    return 0;
}

