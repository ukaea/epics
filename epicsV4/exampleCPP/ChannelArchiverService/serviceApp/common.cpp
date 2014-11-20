/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS exampleCPP is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */


#include "common.h"

using namespace epics::pvData;

namespace epics
{

namespace channelArchiverService
{

const std::string ntTableStr = "epics:nt/NTTable:1.0";
const std::string ntURIStr = "epics:nt/NTURI:1.0";

const std::string nameStr  = "entity";
const std::string startStr = "starttime";
const std::string endStr   = "endtime";
const std::string countStr = "maxrecords";


StructureConstPtr makeQueryStructure(FieldCreate & factory,
    const std::vector<std::string> & queryFields)
{
    FieldConstPtrArray fields;
    StringArray names;

    for (std::vector<std::string>::const_iterator it = queryFields.begin();
         it!= queryFields.end();
         ++it)
    {
        fields.push_back(factory.createScalar(pvString));
        names.push_back(*it);
    }

    return factory.createStructure(names, fields);
}

StructureConstPtr makeRequestStructure(FieldCreate & factory,
    const std::vector<std::string> & queryFields)
{
    FieldConstPtrArray fields;
    StringArray names;

    fields.push_back(factory.createScalar(pvString));
    fields.push_back(factory.createScalar(pvString));
    fields.push_back(makeQueryStructure(factory, queryFields));

    names.push_back("scheme");
    names.push_back("path");
    names.push_back("query");

    return factory.createStructure(ntURIStr, names, fields);
}


StructureConstPtr makeValuesStructure(FieldCreate & factory)
{
    FieldConstPtrArray fields;
    StringArray names;

    names.push_back("value");
    names.push_back("secondsPastEpoch");
    names.push_back("nanoseconds");
    names.push_back("status");
    names.push_back("severity");

    fields.push_back(factory.createScalarArray(pvDouble));
    fields.push_back(factory.createScalarArray(pvLong));
    fields.push_back(factory.createScalarArray(pvInt));
    fields.push_back(factory.createScalarArray(pvInt));
    fields.push_back(factory.createScalarArray(pvInt));

    return factory.createStructure(names, fields);
}

StructureConstPtr makeArchiverResponseStructure(FieldCreate & factory)
{
    FieldConstPtrArray fields;
    StringArray names;

    names.push_back("labels");
    names.push_back("value");

    fields.push_back(factory.createScalarArray(pvString));
    fields.push_back(makeValuesStructure(factory));

    return factory.createStructure(ntTableStr, names, fields);
}

std::string getTypeIdBase(const std::string & id)
{
    std::string idBase(id);
    size_t pos = id.find_last_of(':');
    if (pos != std::string::npos)
        idBase = id.substr(0, pos);
    return idBase;
}

std::string getTypeIdBasePlusMajor(const std::string & id)
{
    std::string idMajor(id);
    size_t pos = id.find_last_of('.');
    if (pos != std::string::npos)
        idMajor = id.substr(0, pos);
    return idMajor;
}

}

}
