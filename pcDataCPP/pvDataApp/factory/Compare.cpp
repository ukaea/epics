/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvData is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/**
 * @author mes
 */

#include <pv/convert.h>

#include <algorithm>
#include <iterator>
#include <sstream>

namespace epics { namespace pvData {

// PVXXX object comparison

bool operator==(PVField& left, PVField& right)
{
 return getConvert()->equals(left,right);
}

// Introspection object comparision

/** Field equality conditions:
 * 1) same instance
 * 2) same type (field and scalar/element), same name, same subfields (if any)
 */
bool operator==(const Field& a, const Field& b)
{
    if(&a==&b)
        return true;
    if(a.getType()!=b.getType())
        return false;
    switch(a.getType()) {
    case scalar: {
        const Scalar &A=static_cast<const Scalar&>(a);
        const Scalar &B=static_cast<const Scalar&>(b);
        return A==B;
    }
    case scalarArray: {
            const ScalarArray &A=static_cast<const ScalarArray&>(a);
            const ScalarArray &B=static_cast<const ScalarArray&>(b);
            return A==B;
        }
    case structure: {
            const Structure &A=static_cast<const Structure&>(a);
            const Structure &B=static_cast<const Structure&>(b);
            return A==B;
        }
    case structureArray: {
            const StructureArray &A=static_cast<const StructureArray&>(a);
            const StructureArray &B=static_cast<const StructureArray&>(b);
            return A==B;
        }
    default:
        throw std::logic_error("Invalid Field type in comparision");
    }
}

bool operator==(const Scalar& a, const Scalar& b)
{
    if(&a==&b)
        return true;
    return a.getScalarType()==b.getScalarType();
}

bool operator==(const ScalarArray& a, const ScalarArray& b)
{
    if(&a==&b)
        return true;
    return a.getElementType()==b.getElementType();
}

bool operator==(const Structure& a, const Structure& b)
{
    if(&a==&b)
        return true;
    if (a.getID()!=b.getID())
    	return false;
    size_t nflds=a.getNumberFields();
    if (b.getNumberFields()!=nflds)
        return false;

    // std::equals does not work, since FieldConstPtrArray is an array of shared_pointers
    FieldConstPtrArray af = a.getFields();
    FieldConstPtrArray bf = b.getFields();
    for (size_t i = 0; i < nflds; i++)
        if (*(af[i].get()) != *(bf[i].get()))
            return false;

    StringArray an = a.getFieldNames();
    StringArray bn = b.getFieldNames();
    return std::equal( an.begin(), an.end(), bn.begin() );
}

bool operator==(const StructureArray& a, const StructureArray& b)
{
    return *(a.getStructure().get())==*(b.getStructure().get());
}

namespace nconvert {

} // namespace nconvert

}} // namespace epics::pvData