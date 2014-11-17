/*testPVRecordMain.cpp */
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
#include <pv/pvCopy.h>
#include "powerSupply.h"


using namespace std;
using std::tr1::static_pointer_cast;
using namespace epics::pvData;
using namespace epics::pvDatabase;
using std::string;

static PVRecordPtr createScalar(
    string const & recordName,
    ScalarType scalarType,
    string const & properties)
{
    PVStructurePtr pvStructure = getStandardPVField()->scalar(scalarType,properties);
    PVRecordPtr pvRecord = PVRecord::create(recordName,pvStructure);
    pvStructure.reset();
    return pvRecord;
}

static PVRecordPtr createScalarArray(
    string const & recordName,
    ScalarType scalarType,
    string const & properties)
{
    PVStructurePtr pvStructure = getStandardPVField()->scalarArray(scalarType,properties);
    return PVRecord::create(recordName,pvStructure);
}

static void scalarTest()
{
    cout << endl << endl << "****scalarTest****" << endl;
    PVRecordPtr pvRecord;
    pvRecord = createScalar("doubleRecord",pvDouble,"alarm,timeStamp.display");
    pvRecord->destroy();
}

static void arrayTest()
{
    cout << endl << endl << "****arrayTest****" << endl;
    PVRecordPtr pvRecord;
    pvRecord = createScalarArray("doubleArrayRecord",pvDouble,"alarm,timeStamp");
    pvRecord->destroy();
}

static void powerSupplyTest()
{
    cout << endl << endl << "****powerSupplyTest****" << endl;
    PVRecordPtr pvRecord;
    PVStructurePtr pv = createPowerSupply();
    pvRecord = PowerSupply::create("powerSupply",pv);
    pvRecord->destroy();
}

int main(int argc,char *argv[])
{
    scalarTest();
    arrayTest();
    powerSupplyTest();
    return 0;
}

