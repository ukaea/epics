/*exampleDatabaseMain.cpp */
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

#include <pv/channelProviderLocal.h>

#include <pv/exampleDatabase.h>

using namespace std;
using namespace epics::pvDatabase;

int main(int argc,char *argv[])
{
    ExampleDatabase::create();

    ContextLocal::shared_pointer contextLocal = ContextLocal::create();
    contextLocal->start();

    cout << "pvAccess server exampleDatabase running..." << endl;
    epics::pvData::PVStringArrayPtr pvNames = PVDatabase::getMaster()->getRecordNames();
    cout << "Hosted records: " << endl << *pvNames << endl;

    contextLocal->waitForExit();

    return 0;
}
