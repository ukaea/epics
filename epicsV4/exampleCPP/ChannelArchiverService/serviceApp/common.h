#ifndef CHANNELARCHIVERSERVICE_COMMON_H
#define CHANNELARCHIVERSERVICE_COMMON_H

/* common.h - channel archiver service types and utilities common to the server and client */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS exampleCPP is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */

#include <string>
#include <vector>

#include "pv/pvIntrospect.h"
#include "pv/pvData.h"

namespace epics
{

namespace channelArchiverService
{


/**
 * Returns a double array from PVStructure.
 *
 * @param  pvStructure  the PVStructure
 * @param  name         the name of the field
 * @return              the double array
 */
inline pvData::PVDoubleArrayPtr getDoubleArrayField(const pvData::PVStructurePtr & pvStructure, std::string name)
{
    return pvStructure->getSubField<pvData::PVDoubleArray>(name);
}

/**
 * Returns a long array from PVStructure.
 *
 * @param  pvStructure  the PVStructure
 * @param  name         the name of the field
 * @return              the long array
 */
inline pvData::PVLongArrayPtr getLongArrayField(const pvData::PVStructurePtr & pvStructure, std::string name)
{
    return pvStructure->getSubField<pvData::PVLongArray>(name);
}

/**
 * Returns a int array from PVStructure.
 *
 * @param  pvStructure  the PVStructure
 * @param  name         the name of the field
 * @return              the int array
 */
inline pvData::PVIntArrayPtr getIntArrayField(const pvData::PVStructurePtr & pvStructure, std::string name)
{
    return pvStructure->getSubField<pvData::PVIntArray>(name);
}

/**
 * Returns a string array from PVStructure.
 *
 * @param  pvStructure  the PVStructure
 * @param  name         the name of the field
 * @return              the string array
 */
inline pvData::PVStringArrayPtr getStringArrayField(const pvData::PVStructurePtr & pvStructure, std::string name)
{
    return pvStructure->getSubField<pvData::PVStringArray>(name);
}


/**
 * Calculates the maximum of the length for a collection of strings or other objects with
 * a length() function.
 *
 * @param  t  the object of type T to be converted
 * @return    the result of the string conversion
 */
template <typename T>
size_t maxWidth(const T & t)
{
    typedef typename T::const_iterator const_iterator;

    size_t maxWidth = 0;
    for (const_iterator it = t.begin(); it != t.end(); ++it)
    {
	    size_t width = it->length();
	    if (width > maxWidth)
	    {
			maxWidth = width;
		}
	}
	return maxWidth;
}


// IDs for normative types
extern const std::string ntTableStr;
extern const std::string ntURIStr;

// Fieldnames of the channel archiver service query fields.
extern const std::string nameStr;
extern const std::string startStr;
extern const std::string endStr;
extern const std::string countStr;

/**
 * Creates a structure for a query to a service with the supplied fields.
 *
 * @param  factory      the factory used to create the structure
 * @param  queryFields  the fields which the query is to contain
 * @return              the query structure
 */
pvData::StructureConstPtr makeQueryStructure(pvData::FieldCreate & factory,
    const std::vector<std::string> & queryFields);

/**
 * Creates a request structure for a query to a service with the supplied fields.
 *
 * @param  factory      the factory used to create the structure
 * @param  queryFields  the fields which the request query part is to contain
 * @return              the request structure
 */
pvData::StructureConstPtr makeRequestStructure(pvData::FieldCreate & factory,
    const std::vector<std::string> & queryFields);

/**
 * Creates the structure for the response to an archiver query.
 *
 * @param  factory  the factory used to create the structure
 * @return          the table structure.
 */
pvData::StructureConstPtr makeArchiverResponseStructure(pvData::FieldCreate & factory);

/**
 * Returns the type id including any namespace and excluding any version
 * numbers of a given type id. If the supplied string is of the form 
 * <namespace>/<typename>:<Major>.<Minor> it will return
 * <namespace>/<typename>
 *
 * @param   id  the type id to parse
 * @return      the type id string without version numbers
 */
std::string getTypeIdBase(const std::string & id);

/**
 * Returns the type id including any namespace or major version number, but
 * excluding the minor version for a given type id. If the supplied string is
 * of the form <namespace>/<typename>:<Major>.<Minor> it will return
 * <namespace>/<typename>:<Major>
 *
 * @param   id  the type id to parse
 * @return      the type id string without minor version 
 */
std::string getTypeIdBasePlusMajor(const std::string & id);
}

}

#endif
