/* monitorAlgorithm.h */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvData is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/* Marty Kraimer 2011.03 */
#ifndef MONITORALGORITHM_H
#define MONITORALGORITHM_H
#include <string>

#include <pv/pvData.h>

namespace epics { namespace pvIOC { 

class MonitorAlgorithm {
public:
     MonitorAlgorithm(){}
     virtual ~MonitorAlgorithm(){}
     virtual epics::pvData::String getAlgorithmName() = 0;
     virtual bool causeMonitor() = 0;
     void monitorIssued() = 0;
}}

class MonitorAlgorithmCreate {
public:
    virtual String getAlgorithmName() = 0;
    virtual std::auto_ptr<MonitorAlgorithm> create(
        PVRecord &pvRecord;
        MonitorRequester &requester;
        PVRecordField &pvRecordField;
        PVStructure &pvStructure);
};

extern MonitorAlgorithmCreate& getAlgorithmDeadband();
extern MonitorAlgorithmCreate& getAlgorithmOnChange();

#endif  /* MONITORALGORITHM_H */
