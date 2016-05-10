// Copyright information and license terms for this software can be
// found in the file LICENSE that is included with the distribution

/*powerSupplyClient.cpp */

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

int main(int argc,char *argv[])
{
    PvaClientPtr pva = PvaClient::get("pva");
    try {
        PvaClientChannelPtr pvaChannel = pva->channel("powerSupply"); 
        PvaClientPutGetPtr putGet(pvaChannel->createPutGet(
                "putField(power.value,voltage.value)getField()"));
        putGet->connect();
        PvaClientPutDataPtr putData(putGet->getPutData());
        PvaClientGetDataPtr getData(putGet->getGetData());
        PVStructurePtr pvStructure = putData->getPVStructure();
        pvStructure = putData->getPVStructure();
        PVDoublePtr putPower = pvStructure->getSubField<PVDouble>("power.value");
        PVDoublePtr putVoltage = pvStructure->getSubField<PVDouble>("voltage.value");
        putPower->put(5.0);
        putVoltage->put(5.0);
        putGet->putGet();
        
        pvStructure = getData->getPVStructure();
        PVDoublePtr gotPower = pvStructure->getSubField<PVDouble>("power.value");
        PVDoublePtr gotVoltage = pvStructure->getSubField<PVDouble>("voltage.value"); 
        PVDoublePtr gotCurrent = pvStructure->getSubField<PVDouble>("current.value");
        if(gotPower->get() == 5.0) cout <<  "returned correct power\n";
        if(gotVoltage->get() == 5.0) cout <<  "returned correct voltage\n";
        if(gotCurrent->get() == 1.0) cout <<  "returned correct current\n";
    
        putPower->put(10.0);
        putGet->putGet();
    
        pvStructure = getData->getPVStructure();
        gotPower = pvStructure->getSubField<PVDouble>("power.value");
        gotVoltage = pvStructure->getSubField<PVDouble>("voltage.value");
        gotCurrent = pvStructure->getSubField<PVDouble>("current.value");
        if(gotPower->get() == 10.0) cout << "returned correct power\n"; 
        if(gotVoltage->get() == 5.0) cout << "returned correct voltage\n";
        if(gotCurrent->get() == 2.0) cout << "returned correct current\n";
        
        putPower->put(5.0);
        putVoltage->put(0.0);
        cout << "NOTE!!! an exception will be thrown because voltage is 0\n";
        putGet->putGet();
    } catch (std::runtime_error e) {
        cout << "exception " << e.what() << endl;
        exit(1);
    }
    return 0;
}

