/*PVScalarArray.cpp*/
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvData is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/**
 *  @author mrk
 */
#include <cstddef>
#include <cstdlib>
#include <string>
#include <cstdio>
#include <pv/pvData.h>
#include <pv/factory.h>

using std::tr1::static_pointer_cast;using std::tr1::static_pointer_cast;

namespace epics { namespace pvData {

    PVScalarArray::~PVScalarArray() {}

    PVScalarArray::PVScalarArray(ScalarArrayConstPtr const & scalarArray)
    : PVArray(scalarArray) {}

    const ScalarArrayConstPtr PVScalarArray::getScalarArray() const
    {
       return static_pointer_cast<const ScalarArray>(PVField::getField());
    }

    template<>
    std::ostream& PVValueArray<int8>::dumpValue(std::ostream& o, size_t index) const
    {
    	return o << static_cast<int>(*(get() + index));
    }

    template<>
    std::ostream& PVValueArray<uint8>::dumpValue(std::ostream& o, size_t index) const
    {
    	return o << static_cast<unsigned int>(*(get() + index));
    }

}}
